
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





// Disable the Visual Studio warning C4355
//   ('this' : used in base member initializer list)
//   Found in the Connection(Server s, const Options &o) constructor
#ifdef WIN32
#pragma warning(disable: 4355)
#endif

#include <bolt/Connection.h>
#include <bolt/EventListener.h>
#include <bolt/Log.h>
#include <bolt/util.h>

#include <gmsec4/internal/Rawbuf.h>
#include <gmsec4/internal/Subject.h>

#include <list>


namespace bolt {


class TestManaged : public Test
{
public:
	TestManaged(Shared *s) : shared(s) { }
	Code apply() {
		State s = shared->getState();
GMSEC_DEBUG << shared->tag(0).c_str() << "TestManaged.apply: state=" << util::toString(s).c_str();
		if (s == STATE_MANAGED)
			return SUCCESS;
		else if (s == STATE_UNKNOWN)
			return CODE_FALSE;
		return CODE_ABORT;
	}
private:
	Shared *shared;
};



class TestDispatching : public Test
{
public:
	TestDispatching(Shared *s) : shared(s) { }
	Code apply() {
		if (shared->getMode() == CONNECT_DISPATCHING)
			return SUCCESS;
		return CODE_FALSE;
	}
private:
	Shared *shared;
};


Connection::Connection(Server s, const Options &o)
	:
	options(o),
	server(s),
	shared(new Shared(this, o)),
	manager(&runManager, shared),
	reader(&runReader, shared),
	counter(0),
	lastState(STATE_UNKNOWN)
{
	fTag = shared->tag("Connection");

	{
		string tmp(fTag + ":manager");
		manager.setName(tmp.c_str());
		manager.start();
	}

	TestManaged test(shared.get());
	test.setVerbosity(options.getI32("LOG-TestManaged", logWARNING));
	const int timeout_ms = options.getI32(opt::MANAGE_TIMEOUT_ms, 3000);
	Result result = shared->await(timeout_ms, test);
	if (result.isValid())
	{
		string tmp(fTag + ":reader");
		reader.setName(tmp.c_str());
		reader.start();
	}
	else
	{
		GMSEC_WARNING << fTag.c_str() << "unable to manage connection: " << result;
	}

	{
		string tmp = options.getString(opt::REPLY_TO, "");
		if (tmp.size())
			replyTopic = tmp;
	}
}


Connection::~Connection()
{
	shared->shutdown();
}


Server Connection::getServer() const
{
	return server;
}


Result Connection::connect()
{
	Result result = shared->connect();
	if (result.isValid())
	{
		TestDispatching dispatching(shared.get());
		dispatching.setVerbosity(options.getI32("LOG-TestDispatching", logWARNING));
		const int timeout_ms = 10000;
		result = shared->await(timeout_ms, dispatching);
	}

	if (result.isValid())
	{
		notifyEventListeners(shared->getState());
	}

	return result;
}


Result Connection::disconnect()
{
	subscriptions.clear();
	Result result = shared->disconnect();
	notifyEventListeners(shared->getState());
	return result;
}


Result Connection::put(SharedPacket packet)
{
	Result result = shared->put(packet);
	return result;
}


Result Connection::subscribe(const string &pattern)
{
	// save the subscription- if the server responds with an error,
	// then remove it [TODO]
	subscriptions.push_back(pattern);

	State state = shared->getState();
	if (state != STATE_CONNECTED)
	{
		// Result result(ERR_STATE, "no connection");
		Result result;
		return result;
	}

	std::list<string> patterns;
	patterns.push_back(pattern);

	Result result = _subscribe(patterns, true);

	return result;
}


void Connection::setDefaultHandler(PacketHandler *handler)
{
	shared->setDefaultHandler(handler);
}


void Connection::setReplyHandler(PacketHandler *handler)
{
	shared->setReplyHandler(handler);
}


Result Connection::_subscribe(const std::list<string> &patterns, bool await)
{
	if (patterns.size() == 0)
	{
		// Result result(ERR_STATE, "no subscription pattern");
		Result result;
		return result;
	}

	SharedPacket packet(new Packet(PACKET_SUBSCRIBE));

	Meta meta;
	string id = generateID();
	meta.setID(id);

	std::list<string>::const_iterator i = patterns.begin();
	meta.setTopic(*i++);

	for (i32 index = 2; i != patterns.end(); ++i, ++index)
	{
		string key("TOPIC-" + util::toString(index));
		meta.add(Property::createString(key, *i));
	}

	packet->setMeta(meta);
#ifdef DEBUG_SUBSCRIBE
GMSEC_INFO << fTag.c_str() << "_subscribe:" << shared.get() << ": putting";
packet->put(std::cout);
#endif

	Result result = shared->put(packet);

	if (result.isValid() && await)
	{
		TestAcknowledged ack(shared.get(), id);
		const int timeout_ms = 10000;
		GMSEC_DEBUG << fTag.c_str() << "_subscribe: awaiting ack ID=" << id.c_str();
		ack.setVerbosity(options.getI32("LOG-TestAck", logWARNING));
		result = shared->await(timeout_ms, ack);
	}

	return result;
}


Result Connection::unsubscribe(const string &pattern)
{
	bool found = false;
	// <algorithm> find
	for (std::list<string>::iterator i = subscriptions.begin();
			i != subscriptions.end(); ++i)
		if (*i == pattern)
		{
			found = true;
			subscriptions.erase(i);
			break;
		}

	if (!found)
	{
		GMSEC_WARNING << fTag.c_str() << "unsubscribe: not subscribed to " << pattern.c_str();
	}

	State state = shared->getState();
	if (state != STATE_CONNECTED)
	{
		Result result(ERR_STATE, "no connection");
		return result;
	}

	SharedPacket packet(new Packet(PACKET_UNSUBSCRIBE));

	Meta meta;
	string id = generateID();
	meta.setID(id);
	meta.setTopic(pattern);
	packet->setMeta(meta);

	Result result = shared->put(packet);

#if 0
	if (result.isValid())
	{
		TestAcknowledged ack(shared.get(), id);
		const int timeout_ms = 10000;
		result = shared->await(timeout_ms, ack);
	}
#endif

	return result;
}


Result Connection::get(SharedPacket &packet, int timeout_ms)
{
	packet.reset();
	Result result = shared->getInput(packet, timeout_ms);
	return result;
}


string Connection::generateID()
{
	gmsec::api::util::Rawbuf<64> buffer;
	buffer.stream() << ++counter;
	return buffer.str();
}


bool Connection::resubscribe()
{
	std::list<string> patterns;
	patterns = subscriptions;

	if (replyTopic.size())
		patterns.push_back(replyTopic);

	if (patterns.size() > 0)
	{
		_subscribe(patterns, false);
		return true;
	}
	else
	{
		GMSEC_DEBUG << fTag.c_str() << "resubscribe: no subscriptions";
		return false;
	}
}


Result Connection::ping()
{
	SharedPacket packet(new Packet(PACKET_ECHO));

	Meta meta;
	string id = generateID();
	meta.setID(id);

	packet->setMeta(meta);

	Result result = shared->put(packet);

	return result;
}


bool Connection::isSubscribed(const string &topic)
{
	std::vector<string> elements;
	if (!gmsec::api::util::Subject::getElements(topic, elements))
	{
		GMSEC_WARNING << "getSubscriptionFlags: invalid topic " << topic.c_str();
		return false;
	}

	for (std::list<string>::iterator i = subscriptions.begin();
			i != subscriptions.end();
			++i)
	{
		if (gmsec::api::util::Subject::doesSubjectMatchPattern(elements, *i))
		{
			return true;
		}
	}

	return false;
}


void Connection::registerEventListener(EventListener* listener)
{
	if (listener)
	{
		listeners.push_back(listener);
	}
}


void Connection::notifyEventListeners(State state)
{
	if (lastState != state)
	{
		lastState = state;

		for (std::list<EventListener*>::iterator i = listeners.begin();
		     i != listeners.end();
		     ++i)
		{
			EventListener* listener = *i;
			listener->onEvent(state);
		}
	}
}


} // namespace bolt

