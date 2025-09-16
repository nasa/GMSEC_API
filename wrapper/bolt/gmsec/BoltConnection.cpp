/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec_bolt.h>

#include <BoltConnection.h>

#include <gmsec5/ConfigOptions.h>

#include <gmsec5/internal/InternalConnection.h>
#include <gmsec5/internal/InternalMessageFactory.h>
#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/Rawbuf.h>
#include <gmsec5/internal/StringUtil.h>

#include <gmsec5/Connection.h>
#include <gmsec5/Errors.h>

#include <gmsec5/util/Buffer.h>
#include <gmsec5/util/Condition.h>
#include <gmsec5/util/Log.h>
#include <gmsec5/util/Mutex.h>

#include <gmsec_version.h>

#include <sstream>
#include <iostream>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;
using namespace bolt;

using std::list;


namespace gmsec_bolt {


class MyPacketHandler : public PacketHandler
{
public:
	MyPacketHandler(BoltConnection& conn, bool forReplies)
		: connection(conn),
	      reply(forReplies)
	{
	}

	virtual void onPacket(SharedPacket packet)
	{
		if (reply)
		{
			connection.handleReply(packet);
		}
		else
		{
			connection.handlePacket(packet);
		}
	}

private:
	BoltConnection& connection;
	bool            reply;
};


static int applyResult(const char* tag, Status& status, const Result& result, bool quiet = false)
{
	if (!quiet)
	{
		GMSEC_DEBUG << tag << ": " << result;
	}

	if (result.isValid())
	{
		return 0;
	}

	if (result.getCode() == CODE_TIMEOUT)
	{
		std::string msg = std::string(tag) + " -- timeout occurred";
		status.set(CONNECTION_ERROR, TIMEOUT_OCCURRED, msg.c_str());
	}
	else
	{
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, result.getText().c_str(), (GMSEC_I32) result.getCode());
	}

	return 1;
}


class MultiStatus
{
public:
	MultiStatus(const char* tag0) : tag(tag0), good(0), bad(0)
	{
	}

	void update(const Result &result)
	{
		if (result.isValid())
		{
			++good;
		}
		else
		{
			++bad;
			applyResult(tag, status, result);
		}
	}

	Status get()
	{
		GMSEC_DEBUG << "MultiStatus.get: good=" << good << " bad=" << bad;
		if (good > 0)
		{
			status.reset();
		}
		return status;
	}

private:
	const char* tag;
	int good;
	int bad;
	Status status;
};


static unsigned int nextID()
{
	static unsigned int id = 0;
	return ++id;
}


BoltConnection::BoltConnection(const Config& config)
	: counter(0),
	  myDefaultHandler(0),
	  myReplyHandler(0),
	  myReplyTo()
{
	{
		Rawbuf<64> buffer;
		buffer.stream() << "BoltConnection[" << nextID() << "] ";
		myTag = buffer.str();
	}

	logInfo.mwGet = config.getBooleanValue(BOLT_HIDE_MWGET, true);
}


BoltConnection::~BoltConnection()
{
}


const char* BoltConnection::getLibraryRootName()
{
	return GMSEC_MW_VERSION_STRING;
}


void BoltConnection::getOptions(Options& options)
{
	Config& config = getExternal().getConfig();

	const char* name  = 0;
	const char* value = 0;

	bool hasPair = config.getFirst(name, value);

	const char prefix[] = "MW-";
	size_t presize = sizeof(prefix) - 1;

	while (hasPair)
	{
		std::string tmp(name);

		if (tmp.length() > presize && tmp.substr(0, presize) == prefix)
		{
			std::string key(tmp.substr(presize));

			GMSEC_DEBUG << "getOptions: key=" << key.c_str() << " value=" << value;

			options.add(Property::createString(key, value));
		}

		hasPair = config.getNext(name, value);
	}
}


const char* BoltConnection::getLibraryVersion()
{
	if (libVersion.empty())
	{
		std::ostringstream oss;
		oss << "Bolt " << GMSEC_VERSION_NUMBER;
		libVersion = oss.str();
	}

	return libVersion.c_str();
}


void BoltConnection::onEvent(bolt::State state)
{
	switch (state)
	{
		case STATE_UNCONNECTED:
		{
			Status status(NO_ERROR_CLASS, CONNECTION_LOST, "Connection disconnected");
			GMSEC_INFO << status.get();
			getExternal().dispatchEvent(gmsec::api5::Connection::Event::CONNECTION_BROKEN_EVENT, status);
		}
		break;

		case STATE_CONNECTED:
		{
			Status status(NO_ERROR_CLASS, CONNECTION_CONNECTED, "Connection established");
			GMSEC_INFO << status.get();
			getExternal().dispatchEvent(gmsec::api5::Connection::Event::CONNECTION_SUCCESSFUL_EVENT, status);
		}
		break;

		case STATE_RECONNECTING:
		{
			Status status1(CONNECTION_ERROR, CONNECTION_LOST, "Connection broken");
			GMSEC_WARNING << status1.get();
			getExternal().dispatchEvent(gmsec::api5::Connection::Event::CONNECTION_BROKEN_EVENT, status1);

			Status status2(CONNECTION_ERROR, CONNECTION_RECONNECT, "Attempting to reconnect");
			GMSEC_WARNING << status2.get();
			getExternal().dispatchEvent(gmsec::api5::Connection::Event::CONNECTION_RECONNECT_EVENT, status2);
		}
		break;

		default:
			// we don't care
			break;
	}
}


std::string BoltConnection::generateID()
{
	++counter;
	Rawbuf<256> buffer;
	buffer.stream() << getExternal().getID() << '_' << counter;
	return buffer.str();
}


static void storeProperties(const ValueMap &header, Meta &meta)
{
	ValueMap::Iterator i;
	header.reset(i);

	const Value *v = 0;
	while ((v = i.next()) != 0)
	{
		std::string id = i.getID();

		if (v->isString())
		{
			std::string value;
			v->getString(value);
			meta.add(Property::createString(id, value));
		}
		else if (v->isBoolean())
		{
			bool value;
			v->getBoolean(value);
			meta.add(Property::createFlag(id, value));
		}
		else if (v->isInteger())
		{
			i32 value;
			v->getI32(value);
			meta.add(Property::createInteger(id, value));
		}
		else if (v->isReal())
		{
			f64 value;
			v->getF64(value);
			meta.add(Property::createReal(id, value));
		}
	}
}


void BoltConnection::makePacket(const Message& message, SharedPacket& packet, PacketType type, const std::string* useID)
{
	DataBuffer data;
	ValueMap header;

	Status result = getExternal().getPolicy().package(const_cast<Message&>(message), data, header);

	if (result.hasError())
	{
		throw GmsecException(result);
	}

	const char* topic = message.getSubject();

	packet.reset(new Packet(type));

	Meta meta;
	storeProperties(header, meta);

	string id(useID ? *useID : generateID());
	meta.add(Property::createID(id));
	meta.add(Property::createTopic(topic));
	packet->setMeta(meta);

	// ick, can ByteBuffer be changed to DataBuffer?
	ByteBuffer content((bolt::u8 *) data.get(), static_cast<bolt::i32>(data.size()), false);
	packet->setBody(content);
}


static Message::Kind mapTypeToKind(PacketType type)
{
	switch (type)
	{
		case PACKET_PUBLISH: return Message::Kind::PUBLISH;
		case PACKET_REQUEST: return Message::Kind::REQUEST;
		case PACKET_REPLY  : return Message::Kind::REPLY;

		default:
			throw GmsecException(MIDDLEWARE_ERROR, UNKNOWN_MSG_TYPE, "Unable to decode Message kind");
	}
}


void parseProperties(const Meta& meta, Message::Kind kind, Message*& message)
{
	ValueMap details;
	bool     haveSubject = false;

	for (Meta::ConstIterator i = meta.iterator(); i != meta.end(); ++i)
	{
		const char*     id = i->first.c_str();
		const Property* p  = i->second;

		std::string svalue(p->asString());

		StdUniquePtr<Field> field;

		switch (p->getType())
		{
		case Property::TYPE_STRING:
			details.setString(id, svalue);
			field.reset( new StringField(id, svalue.c_str(), true) );
			break;
		case Property::TYPE_FLAG:
			details.setBoolean(id, p->asFlag());
			field.reset( new BooleanField(id, p->asFlag(), true) );
			break;
		case Property::TYPE_I32:
			details.setI32(id, p->asInteger());
			field.reset( new I32Field(id, p->asInteger(), true) );
			break;
		case Property::TYPE_F64:
			details.setF64(id, p->asReal());
			field.reset( new F64Field(id, p->asReal(), true) );
			break;
		case Property::TYPE_TOPIC:
			details.setString(tag::TOPIC, svalue);
			MessageBuddy::getInternal(*message).setSubject(svalue.c_str());
			haveSubject = true;
			break;
		case Property::TYPE_ID:
			details.setString(tag::ID, svalue);
			break;
		case Property::TYPE_CORR_ID:
			details.setString(tag::CORR_ID, svalue);
			break;
		case Property::TYPE_REPLY_TO:
			details.setString(tag::REPLY_TO, svalue);
			break;
		default:
			GMSEC_WARNING << "parseProperties: " << id << " has unexpected type " << p->getType();
			break;
		}

		if (message && field.get())
		{
			MessageBuddy::getInternal(*message).addField(*(field.release()), false);
		}
	}

	if (message)
	{
		if (!haveSubject)
		{
			delete message;
			message = 0;
		}
		else
		{
			MessageBuddy::getInternal(*message).getDetails().cloneValues(details);

			MessageBuddy::getInternal(*message).setKind(kind);
		}
	}
}


void BoltConnection::fromPacket(SharedPacket& packet, Message*& message)
{
	Message::Kind kind = mapTypeToKind(packet->getHeader()->getType());

	const ByteBuffer* body = packet->getBody();
	const Meta*       meta = packet->getMeta();

	message = NULL;

	if (meta)
	{
		message = MessageFactoryBuddy::getInternal(getExternal().getMessageFactory()).newMessage();
		parseProperties(*meta, kind, message);
	}

	if (message)
	{
		ValueMap& details = MessageBuddy::getInternal(*message).getDetails();

		if (body)
		{
			// ick, can bolt::ByteBuffer be changed to DataBuffer?
			DataBuffer data((DataBuffer::data_t*) body->raw(), body->size(), false);

			Status result = getExternal().getPolicy().unpackage(*message, data, details);

			if (result.hasError())
			{
				delete message;
				message = 0;

				throw GmsecException(result);
			}
		}

		MessageFactoryBuddy::getInternal(getExternal().getMessageFactory()).addMessageTemplate(*message);
		MessageFactoryBuddy::getInternal(getExternal().getMessageFactory()).identifyTrackingFields(*message);
	}
}


void BoltConnection::handleReply(SharedPacket& packet)
{
	Message* message = 0;

	fromPacket(packet, message);

	if (!message)
	{
		GMSEC_WARNING << "BoltConnection.handleReply: bad packet";
		return;
	}

	if (!getExternal().onReply(message))
	{
		handlePacket(packet);
	}
}


void BoltConnection::handlePacket(SharedPacket &packet)
{
	// no-op
}


void BoltConnection::mwPublishAux(const Message& message, SharedPacket& packet)
{
	Message::Kind kind = message.getKind();

	PacketType type;

	switch (kind)
	{
	case Message::Kind::PUBLISH:
		type = PACKET_PUBLISH;
		break;
	case Message::Kind::REPLY:
		type = PACKET_REPLY;
		break;
	default:
		GMSEC_WARNING << "This method should not be used to publish a request message";
		return;
	}

	makePacket(message, packet, type);
}


void BoltConnection::mwRequestAux(Message& message, SharedPacket& packet, const std::string& uniqueID)
{
	makePacket(message, packet, PACKET_REQUEST, &uniqueID);

	Meta *meta = packet->getMeta();
	meta->add(Property::createCorrID(uniqueID));
}


SingleConnection::SingleConnection(const Config& config)
	: BoltConnection(config)
{
}


SingleConnection::~SingleConnection()
{
	delete myReplyHandler;
	myReplyHandler = 0;
}


void SingleConnection::setServers(const list<Server> &in)
{
	if (in.size() == 1)
	{
		server = in.front();
	}
	else
	{
		throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Expecting single server");
	}
}


void SingleConnection::mwConnect()
{
	if (!connection.get())
	{
		Options options;
		getOptions(options);

		options.add(Property::createFlag(opt::REPLY_EXPOSE, true));

		myReplyHandler = new MyPacketHandler(*this, true);

		connection.reset(new bolt::Connection(server, options));
		connection->registerEventListener(this);
		connection->setReplyHandler(myReplyHandler);
	}

	int    maxRetries     = getExternal().getMaxConnectionRetries();
	int    retryInterval  = getExternal().getConnectionRetryInterval();
	int    triesRemaining = maxRetries;
	Result result;

	do
	{
		result = connection->connect();

		if (result.isError() && result.getCode() == 4)
		{
			result.clear();
			break;
		}
		else if (result.isError() && (maxRetries == -1 || --triesRemaining > 0))
		{
			GMSEC_WARNING << "Connection failed; retrying..." << " [tries remaining: " << triesRemaining << "]";
			TimeUtil::millisleep(retryInterval);
		}
	} while (result.isError() && (maxRetries == -1 || triesRemaining >= 0));

	if (result.isError())
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}

	std::ostringstream endpoint;
	endpoint << connection->getServer().host << ":" << connection->getServer().port;

	getExternal().setConnectionEndpoint(endpoint.str());
}


void SingleConnection::mwDisconnect()
{
	connection->setReplyHandler(0);

	Result result = connection->disconnect();

	if (!result.isValid())
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}
}


void SingleConnection::mwSubscribe(const char* subject, const Config& config)
{
	Result result = connection->subscribe(subject);

	if (!result.isValid())
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}
}


void SingleConnection::mwUnsubscribe(const char* subject)
{
	Result result = connection->unsubscribe(subject);

	if (!result.isValid())
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}
}


void SingleConnection::mwPublish(const Message& message, const Config& config)
{
	SharedPacket packet;

	mwPublishAux(const_cast<Message&>(message), packet);

	int    maxRetries     = getExternal().getMaxConnectionRetries();
	int    retryInterval  = getExternal().getConnectionRetryInterval();
	int    triesRemaining = maxRetries;
	Result result;

	do
	{
		result = connection->put(packet);

		if (result.isError() && (maxRetries == -1 || --triesRemaining > 0))
		{
			GMSEC_WARNING << "Publish failed; retrying... [tries remaining: " << triesRemaining << "]";

			TimeUtil::millisleep(retryInterval);
		}
	} while (result.isError() && (maxRetries == -1 || triesRemaining >= 0));

	if (!result.isValid())
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}
}


void SingleConnection::mwRequest(const Message& message, const std::string& uniqueID)
{
	SharedPacket packet;

	mwRequestAux(const_cast<Message&>(message), packet, uniqueID);

	Result result = connection->put(packet);

	if (!result.isValid())
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}
}


void SingleConnection::mwReceive(Message*& message, GMSEC_I32 timeout_ms)
{
	message = 0;

	if (timeout_ms < 0)
	{
		timeout_ms = 24 * 3600 * 1000;
	}

	SharedPacket packet;

	Result result = connection->get(packet, timeout_ms);

	if (result.isError() && result.getCode() != CODE_TIMEOUT)
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}

	if (result.getCode() != CODE_TIMEOUT)
	{
		fromPacket(packet, message);
	}
}


std::string SingleConnection::mwGetUniqueID()
{
	return generateID();
}


const char* SingleConnection::getMWInfo()
{
	if (mwInfo.empty())
	{
		std::ostringstream strm;
		strm << getLibraryRootName();
		mwInfo = strm.str();
	}

	return mwInfo.c_str();
}


MultipleConnection::MultipleConnection(const Config& config)
	: BoltConnection(config)
{
}


MultipleConnection::~MultipleConnection()
{
	for (ConnIt i = connections.begin(); i != connections.end(); ++i)
	{
		bolt::Connection* connection = *i;
		delete connection;
	}

	connections.clear();

	delete myDefaultHandler;
	myDefaultHandler = 0;

	delete myReplyHandler;
	myReplyHandler = 0;
}


void MultipleConnection::setServers(const list<Server>& in)
{
	if (in.size() > 1)
	{
		servers = in;
	}
	else
	{
		throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Expecting at least two servers");
	}
}


void MultipleConnection::mwConnect()
{
	if (connections.size() == 0)
	{
		// perform initialization
		Options options;
		options.add(Property::createFlag(opt::MULTI_SERVER, true));
		getOptions(options);

		options.add(Property::createFlag(opt::REPLY_EXPOSE, true));

		myReplyHandler = new MyPacketHandler(*this, true);

		myDefaultHandler = new MyPacketHandler(*this, false);

		for (ServerIt i = servers.begin(); i != servers.end(); ++i)
		{
			bolt::Connection* connection = new bolt::Connection(*i, options);
			connection->registerEventListener(this);
			connection->setDefaultHandler(myDefaultHandler);
			connection->setReplyHandler(myReplyHandler);

			connections.push_back(connection);
		}
	}

	std::ostringstream endpoints;

	int    maxRetries     = getExternal().getMaxConnectionRetries();
	int    retryInterval  = getExternal().getConnectionRetryInterval();
	int    triesRemaining = maxRetries;
	Status status;

	do
	{
		MultiStatus multi("connect");
		int success = 0;

		endpoints.str("");

		for (ConnIt i = connections.begin(); i != connections.end(); ++i)
		{
			bolt::Connection* connection = *i;

			Result result = connection->connect();

			if (result.isError() && !StringUtil::stringEquals(result.getText().c_str(), "already connected"))
			{
				multi.update(result);
			}
			else
			{
				++success;

				const Server s = connection->getServer();

				endpoints << (endpoints.str().empty() ? "" : ",") << s.host << ":" << s.port;
			}
		}

		status = multi.get();

		if (success > 0)
		{
			status.reset();
		}

		if (status.hasError() && (maxRetries == -1 || --triesRemaining > 0))
		{
			GMSEC_WARNING << "Connection failed; retrying..." << " [tries remaining: " << triesRemaining << "]";
			TimeUtil::millisleep(retryInterval);
		}
	} while (status.hasError() && (maxRetries == -1 || triesRemaining >= 0));

	// if we were unable to connect to any server, should we reset
	// all of the connections?
	if (status.hasError()) // unable to connect to any server
	{
		throw GmsecException(status);
	}

	getExternal().setConnectionEndpoint(endpoints.str());
}


void MultipleConnection::mwDisconnect()
{
	MultiStatus multi("disconnect");

	for (ConnIt i = connections.begin(); i != connections.end(); ++i)
	{
		bolt::Connection *connection = *i;

		connection->setReplyHandler(0);

		Result result = connection->disconnect();
		multi.update(result);
	}

	Status status = multi.get();

	if (status.hasError())
	{
		throw GmsecException(status);
	}
}


void MultipleConnection::mwSubscribe(const char* subject, const Config& config)
{
	MultiStatus multi("subscribe");

	for (ConnIt i = connections.begin(); i != connections.end(); ++i)
	{
		bolt::Connection *connection = *i;
		Result result = connection->subscribe(subject);
		multi.update(result);
	}

	Status status = multi.get();

	if (status.hasError())
	{
		throw GmsecException(status);
	}
}


void MultipleConnection::mwUnsubscribe(const char* subject)
{
	MultiStatus multi("unsubscribe");

	for (ConnIt i = connections.begin(); i != connections.end(); ++i)
	{
		bolt::Connection *connection = *i;
		Result result = connection->unsubscribe(subject);
		multi.update(result);
	}

	Status status = multi.get();

	if (status.hasError())
	{
		throw GmsecException(status);
	}
}



void MultipleConnection::mwPublish(const Message& message, const Config& config)
{
	SharedPacket packet;

	mwPublishAux(message, packet);

	MultiStatus multi("put/publish");

	for (ConnIt i = connections.begin(); i != connections.end(); ++i)
	{
		bolt::Connection *connection = *i;
		Result result = connection->put(packet);
		multi.update(result);
	}

	Status status = multi.get();

	if (status.hasError())
	{
		throw GmsecException(status);
	}
}


void MultipleConnection::mwRequest(const Message& message, const std::string& uniqueID)
{
	SharedPacket packet;

	mwRequestAux(const_cast<Message&>(message), packet, uniqueID);

	MultiStatus multi("put/request");

	for (ConnIt i = connections.begin(); i != connections.end(); ++i)
	{
		bolt::Connection *connection = *i;
		Result result = connection->put(packet);
		multi.update(result);
	}

	Status status = multi.get();

	if (status.hasError())
	{
		throw GmsecException(status);
	}
}


std::string MultipleConnection::mwGetUniqueID()
{
	return generateID();
}


const char* MultipleConnection::getMWInfo()
{
	if (mwInfo.empty())
	{
		std::ostringstream strm;
		strm << getLibraryRootName();
		mwInfo = strm.str();
	}

	return mwInfo.c_str();
}


class TestGetPacket : public Test
{
public:
	TestGetPacket(list<SharedPacket> &q, SharedPacket &out)
		: queue(q), packet(out) { }

	Code apply() {
		if (queue.size() > 0) {
			packet = queue.front();
			queue.pop_front();
			return SUCCESS;
		}
		return CODE_FALSE;
	}

private:
	list<SharedPacket> &queue;
	SharedPacket &packet;
};


void MultipleConnection::mwReceive(Message*& message, GMSEC_I32 timeout_ms)
{
	if (timeout_ms < 0)
	{
		timeout_ms = 24 * 3600 * 1000;
	}

	SharedPacket packet;
	TestGetPacket task(incoming, packet);

	task.describe(myTag + task.info());

	Result result = bolt::util::await(task, condvar, timeout_ms);

	if (result.isError() && result.getCode() != CODE_TIMEOUT)
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}

	if (result.getCode() != CODE_TIMEOUT)
	{
		fromPacket(packet, message);
	}
}


void MultipleConnection::handlePacket(SharedPacket &packet)
{
	AutoMutex hold(condvar.getMutex());

	const Meta *meta = packet->getMeta();
	if (!meta)
	{
		GMSEC_WARNING << "BoltConnection.handlePacket: bad packet: no metadata";
		return;
	}

	std::string id = meta->getID();

	bool isUnique = filter.update(id.c_str());

	if (!isUnique)
	{
		return;
	}

	Message* message = 0;
	fromPacket(packet, message);

	delete message;

	incoming.push_back(packet);
	condvar.broadcast(Condition::USER);
}


} // namespace gmsec_bolt
