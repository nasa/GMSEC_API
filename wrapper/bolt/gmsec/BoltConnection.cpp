/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec_bolt.h>

#include <BoltConnection.h>

#include <gmsec4/ConfigOptions.h>

#include <gmsec4/internal/InternalConnection.h>
#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/Rawbuf.h>

#include <gmsec4/Connection.h>
#include <gmsec4/Errors.h>

#include <gmsec4/util/Buffer.h>
#include <gmsec4/util/Condition.h>
#include <gmsec4/util/Log.h>
#include <gmsec4/util/Mutex.h>

#include <gmsec_version.h>

#include <sstream>
#include <iostream>


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;
using namespace bolt;

using std::list;


namespace gmsec_bolt {


static const int DEBUG_PREPARE = 0;
static const int DEBUG_UNLOAD = 0;
static const int DEBUG_REPLY = 0;
static const int DEBUG_MULTI = 0;
static const int DEBUG_PARSE = 0;


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
		if (DEBUG_REPLY)
		{
			GMSEC_DEBUG << "MyPacketHandler.onPacket() reply=" << reply;
			packet->put(std::cout);
		}

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
			getExternal().dispatchEvent(gmsec::api::Connection::CONNECTION_BROKEN_EVENT, status);
		}
		break;

		case STATE_CONNECTED:
		{
			Status status(NO_ERROR_CLASS, CONNECTION_CONNECTED, "Connection established");
			GMSEC_INFO << status.get();
			getExternal().dispatchEvent(gmsec::api::Connection::CONNECTION_SUCCESSFUL_EVENT, status);
		}
		break;

		case STATE_RECONNECTING:
		{
			Status status1(CONNECTION_ERROR, CONNECTION_LOST, "Connection broken");
			GMSEC_WARNING << status1.get();
			getExternal().dispatchEvent(gmsec::api::Connection::CONNECTION_BROKEN_EVENT, status1);

			Status status2(CONNECTION_ERROR, CONNECTION_RECONNECT, "Attempting to reconnect");
			GMSEC_WARNING << status2.get();
			getExternal().dispatchEvent(gmsec::api::Connection::CONNECTION_RECONNECT_EVENT, status2);
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
			bool value = false;
			v->getBoolean(value);
			meta.add(Property::createFlag(id, value));
		}
		else if (v->isInteger())
		{
			i32 value = 0;
			v->getI32(value);
			meta.add(Property::createInteger(id, value));
		}
		else if (v->isReal())
		{
			f64 value = 0;
			v->getF64(value);
			meta.add(Property::createReal(id, value));
		}
	}
}


void BoltConnection::makePacket(const Message& message, SharedPacket& packet, PacketType type, std::string* useID)
{
	DataBuffer data;
	ValueMap header;

	Status result = getExternal().getPolicy().package(const_cast<Message&>(message), data, header);

	if (result.isError())
	{
		throw Exception(result);
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
	ByteBuffer content((bolt::u8 *) data.get(), data.size(), false);
	packet->setBody(content);

	if (DEBUG_PREPARE)
	{
		GMSEC_DEBUG << "BoltConnection.makePacket:";
		packet->put(std::cout);
	}
}


static Message::MessageKind mapTypeToKind(PacketType type)
{
	switch (type)
	{
		case PACKET_PUBLISH: return Message::PUBLISH;
		case PACKET_REQUEST: return Message::REQUEST;
		case PACKET_REPLY  : return Message::REPLY;

		default:
			throw Exception(MIDDLEWARE_ERROR, UNKNOWN_MSG_TYPE, "Unable to decode Message kind");
	}
}


static Message* parseProperties(const Meta& meta, Message::MessageKind kind, const Config& msgConfig)
{
	Message* message = new Message("BOGUS.TOPIC", kind, msgConfig);
	ValueMap details;
	bool     haveSubject = false;

	for (Meta::ConstIterator i = meta.iterator(); i != meta.end(); ++i)
	{
		const char*     id = i->first.c_str();
		const Property* p  = i->second;

		std::string svalue(p->asString());

		Field* field = NULL;

		switch (p->getType())
		{
		case Property::TYPE_STRING:
			details.setString(id, svalue);
			field = new StringField(id, svalue.c_str());
			field->isHeader(true);
			break;
		case Property::TYPE_FLAG:
			details.setBoolean(id, p->asFlag());
			field = new BooleanField(id, p->asFlag());
			field->isHeader(true);
			break;
		case Property::TYPE_I32:
			details.setI32(id, p->asInteger());
			field = new I32Field(id, p->asInteger());
			field->isHeader(true);
			break;
		case Property::TYPE_F64:
			details.setF64(id, p->asReal());
			field = new F64Field(id, p->asReal());
			field->isHeader(true);
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

		if (field)
		{
			if (message)
			{
				message->addField(*field);
			}

			delete field;
		}

		if (DEBUG_PARSE)
		{
			GMSEC_INFO << "parseProperties: " << id << " => " << svalue.c_str();
		}
	}

	if (message)
	{
		MessageBuddy::getInternal(*message).getDetails().cloneValues(details);
	}

	if (!haveSubject)
	{
		delete message;
		message = 0;
	}

	return message;
}


void BoltConnection::fromPacket(SharedPacket& packet, Message*& message)
{
	if (DEBUG_UNLOAD)
	{
		GMSEC_INFO << "BoltConnection.fromPacket:";
		packet->put(std::cout);
	}

	Message::MessageKind kind = mapTypeToKind(packet->getHeader()->getType());

	const ByteBuffer* body = packet->getBody();
	const Meta*       meta = packet->getMeta();

	message = NULL;

	if (meta)
	{
		message = parseProperties(*meta, kind, getExternal().getMessageConfig());
	}

	if (message)
	{
		ValueMap& details = MessageBuddy::getInternal(*message).getDetails();

		if (body)
		{
			// ick, can bolt::ByteBuffer be changed to DataBuffer?
			DataBuffer data((DataBuffer::data_t*) body->raw(), body->size(), false);

			Status result = getExternal().getPolicy().unpackage(*message, data, details);

			if (result.isError())
			{
				delete message;
				message = 0;

				throw Exception(result);
			}
		}

		if (kind == Message::REPLY)
		{
			getExternal().updateReplySubject(message);
		}

		if (kind == Message::REQUEST && (meta != NULL && meta->getReplyTo().empty()))
		{
			details.setBoolean(GMSEC_REQ_RESP_BEHAVIOR, true);

			try
			{
				getExternal().setReplyUniqueID(*message, message->getStringValue(GMSEC_REPLY_UNIQUE_ID_FIELD));

				// Remove GMSEC_REPLY_UNIQUE_ID_FIELD from the message.
				message->clearField(GMSEC_REPLY_UNIQUE_ID_FIELD);
			}
			catch (...)
			{
				GMSEC_WARNING << "GMSEC_REPLY_UNIQUE_ID field is missing!";
			}
		}
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

	if (!getExternal().onReply(message) && getExternal().getRequestSpecs().exposeReplies)
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
	Message::MessageKind kind = message.getKind();

	if (kind == Message::REPLY)
	{
		RequestSpecs specs = getExternal().getRequestSpecs();

		if (specs.useSubjectMapping)
		{
			// For backward compatibility.
			makePacket(message, packet, PACKET_PUBLISH);
		}
		else
		{
			makePacket(message, packet, PACKET_REPLY);

			InternalMessage& intMsg = MessageBuddy::getInternal(message);

			bool reqResp = false;

			intMsg.getDetails().getBoolean(GMSEC_REQ_RESP_BEHAVIOR, reqResp, &reqResp);

			if (reqResp)
			{
				Meta *meta = packet.get()->getMeta();
				meta->add(Property::createFlag(GMSEC_REQ_RESP_BEHAVIOR, true));
			}
		}
	}
	else if (kind == Message::REQUEST)
	{
		std::string uniqueID = generateID();

		const_cast<Message&>(message).addField(GMSEC_REPLY_UNIQUE_ID_FIELD, uniqueID.c_str());

		makePacket(message, packet, PACKET_REQUEST);
	}
	else
	{
		makePacket(message, packet, PACKET_PUBLISH);
	}
}


void BoltConnection::mwRequestAux(Message& message, SharedPacket& packet, std::string& uniqueID)
{
	uniqueID = generateID();

	message.addField(GMSEC_REPLY_UNIQUE_ID_FIELD, uniqueID.c_str());

	RequestSpecs specs = getExternal().getRequestSpecs();

	if (!specs.useSubjectMapping)
	{
		MessageBuddy::getInternal(message).getDetails().setBoolean(GMSEC_REQ_RESP_BEHAVIOR, true);
	}

	makePacket(message, packet, PACKET_REQUEST, &uniqueID);

	Meta *meta = packet->getMeta();
	meta->add(Property::createCorrID(uniqueID));

	if (specs.useSubjectMapping)
	{
		meta->add(Property::createReplyTo(myReplyTo));
	}
}


void BoltConnection::mwReplyAux(Message& request, Message& reply, SharedPacket& packet)
{
	// Get the Request's Unique ID, and put it into a field in the Reply
	std::string corrID(getExternal().getReplyUniqueID(request));
	std::string reqReplyTo = "<NOT FOUND>";

	MessageBuddy::getInternal(request).getDetails().getString(tag::REPLY_TO, reqReplyTo);

	if (reqReplyTo == "<NOT FOUND>")
	{
		GMSEC_WARNING << "get(REPLY_TO) == <NOT FOUND>";
	}

	GMSEC_DEBUG << "mwReply: corrID=" << corrID.c_str() << " reqReplyTo=" << reqReplyTo.c_str();

	reply.addField(GMSEC_REPLY_UNIQUE_ID_FIELD, getExternal().getReplyUniqueID(request).c_str());

	makePacket(reply, packet, PACKET_REPLY);

	Meta* meta = packet->getMeta();
	meta->add(Property::createTopic(reqReplyTo));
	meta->add(Property::createCorrID(corrID));
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
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Expecting single server");
	}
}


void SingleConnection::mwConnect()
{
	RequestSpecs specs = getExternal().getRequestSpecs();

	if (!connection.get())
	{
		Options options;
		getOptions(options);

		if (specs.requestReplyEnabled)
		{
			if (specs.useSubjectMapping)
			{
				myReplyTo = specs.replySubject;
				options.add(Property::createString(opt::REPLY_TO, myReplyTo));
			}

			options.add(Property::createFlag(opt::REPLY_EXPOSE, specs.exposeReplies));
		}

		connection.reset(new bolt::Connection(server, options));
		connection->registerEventListener(this);
		myReplyHandler = new MyPacketHandler(*this, true);
	}

	Result result = connection->connect();

	if (!result.isValid())
	{
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}

	std::ostringstream endpoint;
	endpoint << connection->getServer().host << ":" << connection->getServer().port;

	getExternal().setConnectionEndpoint(endpoint.str());

	if (specs.requestReplyEnabled)
	{
		connection->setReplyHandler(myReplyHandler);
	}
}


void SingleConnection::mwDisconnect()
{
	connection->setReplyHandler(0);

	Result result = connection->disconnect();

	if (!result.isValid())
	{
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}
}


void SingleConnection::mwSubscribe(const char* subject, const Config& config)
{
	Result result = connection->subscribe(subject);

	if (!result.isValid())
	{
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}
}


void SingleConnection::mwUnsubscribe(const char* subject)
{
	Result result = connection->unsubscribe(subject);

	if (!result.isValid())
	{
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}
}


void SingleConnection::mwPublish(const Message& message, const Config& config)
{
	SharedPacket packet;

	mwPublishAux(const_cast<Message&>(message), packet);

	Result result = connection->put(packet);

	if (!result.isValid())
	{
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}
}


void SingleConnection::mwRequest(const Message& message, std::string& id)
{
	SharedPacket packet;

	mwRequestAux(const_cast<Message&>(message), packet, id);

	Result result = connection->put(packet);

	if (!result.isValid())
	{
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}
}


void SingleConnection::mwReply(const Message& request, const Message& reply)
{
	SharedPacket packet;

	mwReplyAux(const_cast<Message&>(request), const_cast<Message&>(reply), packet);

	Result result = connection->put(packet);

	if (!result.isValid())
	{
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
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
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}

	if (result.getCode() != CODE_TIMEOUT)
	{
		fromPacket(packet, message);
	}
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
	filter.setDebug(config.getBooleanValue(BOLT_LOG_FILTER, false));
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
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Expecting at least two servers");
	}
}


void MultipleConnection::mwConnect()
{
	MultiStatus multi("connect");

	if (connections.size() == 0)
	{
		// perform initialization
		Options options;
		options.add(Property::createFlag(opt::MULTI_SERVER, true));
		getOptions(options);

		RequestSpecs specs = getExternal().getRequestSpecs();

		if (specs.useSubjectMapping)
		{
			myReplyTo = specs.replySubject;
			options.add(Property::createString(opt::REPLY_TO, myReplyTo));
		}

		options.add(Property::createFlag(opt::REPLY_EXPOSE, specs.exposeReplies));

		for (ServerIt i = servers.begin(); i != servers.end(); ++i)
		{
			bolt::Connection* connection = new bolt::Connection(*i, options);
			connections.push_back(connection);

			connection->registerEventListener(this);
		}

		myDefaultHandler = new MyPacketHandler(*this, false);
		myReplyHandler = new MyPacketHandler(*this, true);
	}

	std::ostringstream endpoints;

	for (ConnIt i = connections.begin(); i != connections.end(); ++i)
	{
		bolt::Connection *connection = *i;
		Result result = connection->connect();
		connection->setDefaultHandler(myDefaultHandler);
		connection->setReplyHandler(myReplyHandler);

		multi.update(result);

		if (!result.isError())
		{
			const Server s = connection->getServer();

			endpoints << (endpoints.str().empty() ? "" : ",") << s.host << ":" << s.port;
		}
	}

	Status status = multi.get();

	// if we were unable to connect to any server, should we reset
	// all of the connections?
	if (status.isError()) // unable to connect to any server
	{
		throw Exception(status);
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

	if (status.isError())
	{
		throw Exception(status);
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

	if (status.isError())
	{
		throw Exception(status);
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

	if (status.isError())
	{
		throw Exception(status);
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

	if (status.isError())
	{
		throw Exception(status);
	}
}


void MultipleConnection::mwRequest(const Message& message, std::string& id)
{
	SharedPacket packet;

	mwRequestAux(const_cast<Message&>(message), packet, id);

	MultiStatus multi("put/request");

	for (ConnIt i = connections.begin(); i != connections.end(); ++i)
	{
		bolt::Connection *connection = *i;
		Result result = connection->put(packet);
		multi.update(result);
	}

	Status status = multi.get();

	if (status.isError())
	{
		throw Exception(status);
	}
}


void MultipleConnection::mwReply(const Message& request, const Message& reply)
{
	SharedPacket packet;

	mwReplyAux(const_cast<Message&>(request), const_cast<Message&>(reply), packet);

	MultiStatus multi("put/reply");

	for (ConnIt i = connections.begin(); i != connections.end(); ++i)
	{
		bolt::Connection *connection = *i;
		Result result = connection->put(packet);
		multi.update(result);
	}

	Status status = multi.get();

	if (status.isError())
	{
		throw Exception(status);
	}
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
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, (GMSEC_I32) result.getCode(), result.getText().c_str());
	}

	if (result.getCode() != CODE_TIMEOUT)
	{
		fromPacket(packet, message);
	}
}


void MultipleConnection::handlePacket(SharedPacket &packet)
{
	AutoMutex hold(condvar.getMutex());

	if (DEBUG_MULTI)
	{
		GMSEC_INFO << "MultipleConnection.handlePacket";
		packet->put(std::cout);
	}

	const Meta *meta = packet->getMeta();
	if (!meta)
	{
		GMSEC_WARNING << "BoltConnection.handlePacket: bad packet: no metadata";
		return;
	}

	std::string id = meta->getID();

	bool isUnique = filter.update(id.c_str());
	if (DEBUG_MULTI)
	{
		GMSEC_DEBUG << "\tisUnique(" << id.c_str() << ") => " << (isUnique ? "true" : "false");
	}

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
