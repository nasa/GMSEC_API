/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <bolt/Shared.h>
#include <bolt/Connection.h>
#include <bolt/Log.h>
#include <bolt/util.h>

#include <gmsec4/util/Atomics.h>

#include <gmsec4/internal/Rawbuf.h>

#include <iostream>


namespace bolt {


static const int DEBUG_WRITE = 0;
static const int DEBUG_PUT = 0;


static i32 getID()
{
	static i32 counter = 0;
	i32 id = ++counter;
	return id;
}


Shared::Shared(Connection *connection, const Options &options)
	:
	fOptions(options),
	fState(STATE_UNKNOWN),
	fIoCounter(0),
	fTraps(0),
	fMode(CONNECT_STARTING),
	fConnection(connection),
	fCondition(fMutex),
	fDefaultHandler(0),
	fReplyHandler(0),
	fWarnAlways(true),
	fNullRead_ms(0),
	fNullReadLog_ms(5000)
{
	fTag = "Shared[" + util::toString(getID()) + "] ";

	fReplyTo = fOptions.getString(opt::REPLY_TO, "");
	fReplyExpose = fOptions.getFlag(opt::REPLY_EXPOSE, false);

	bool debug = fOptions.getFlag(opt::GDEBUG, false);
	bool multi = fOptions.getFlag(opt::MULTI_SERVER, false);
	fWarnAlways = debug || !multi;

	fSocket.setOptions(fOptions);
}


Shared::~Shared()
{
}


string Shared::tag(const char *info) const
{
	gmsec::api::util::Rawbuf<256> buffer;
	if (!info)
		info = "Shared";
	buffer.stream() << info << ':' << fTag;
	return buffer.str();
}


void Shared::setState(State state, bool notifyListeners)
{
	AutoMutex hold(fCondition.getMutex());
	if (state != fState)
	{
		fState = state;
		fCondition.broadcast(SIGNAL_STATE);

		if (notifyListeners && fConnection)
		{
			fConnection->notifyEventListeners(state);
		}
	}
}


State Shared::getState()
{
	return (State) gmsec::api::util::syncAddAndFetch(&fState, 0);
}


Condition &Shared::getCondition()
{
	return fCondition;
}


Result Shared::connect()
{
	Result result;

	AutoMutex hold(getMutex());

GMSEC_VERBOSE << fTag.c_str() << "connect";

	State state = _getState();

	if (state == STATE_CONNECTED)
	{
		result.set(ERR_STATE, "already connected");
	}
	else if (state == STATE_MANAGED || state == STATE_UNCONNECTED || state == STATE_RECONNECTING)
	{
		gmsec::api::util::syncAddAndFetch(&fIoCounter, 1);

		if (!fConnection)
		{
			result.set(ERR_STATE, "null connection");
		}
		else
		{
			Server server = fConnection->getServer();

			result = fSocket.connect(server.host.c_str(), server.port);
			if (result.isError())
			{
				if (state == STATE_UNCONNECTED || state == STATE_MANAGED)
				{
					setState(STATE_RECONNECTING);
				}
			}
			else
			{
				GMSEC_DEBUG << fTag.c_str() << "broadcasting CONNECTED";
				bool notifyListeners = (state == STATE_RECONNECTING);
				fMode = CONNECT_STARTING;
				setState(STATE_CONNECTED, notifyListeners);
			}
		}
	}
	else
	{
		result.set(ERR_STATE, "invalid state " + util::toString(state));
	}

	if (result.isError())
	{
		GMSEC_VERBOSE << fTag.c_str() << "connect failed " << result;
	}

	return result;
}


Result Shared::disconnect()
{
	Result result;

	AutoMutex hold(getMutex());

GMSEC_VERBOSE << fTag.c_str() << "disconnect";

	State state = _getState();

	if (state == STATE_UNCONNECTED)
	{
		result.set(ERR_STATE, "not connected");
	}
	else if (state == STATE_CONNECTED || state == STATE_RECONNECTING)
	{
		gmsec::api::util::syncAddAndFetch(&fTraps, 1);

		if (!fConnection)
		{
			result.set(ERR_STATE, "null connection");
		}
		else
		{
			result = fSocket.disconnect();
			if (state == STATE_CONNECTED)
			{
				GMSEC_DEBUG << fTag.c_str() << "broadcasting UNCONNECTED";
				setState(STATE_UNCONNECTED);
			}
		}
	}
	else
	{
		result.set(ERR_STATE, "invalid state " + util::toString(state));
	}

	if (result.isError() && fWarnAlways)
	{
		GMSEC_WARNING << fTag.c_str() << "disconnect failed " << result;
	}

	return result;
}


Result Shared::shutdown()
{
	Result result;

GMSEC_VERBOSE << fTag.c_str() << "shutdown";
	AutoMutex hold(getMutex());

	fConnection = 0;
	State state = _getState();

	if (state == STATE_FINISHED)
	{
		result.set(ERR_STATE, "already shutdown");
	}
	else
	{
		if (state == STATE_CONNECTED || state == STATE_RECONNECTING)
		{
			fSocket.disconnect();
		}

		setState(STATE_FINISHED);
	}

	if (result.isError())
	{
		GMSEC_WARNING << fTag.c_str() << "shutdown failed " << result;
	}

	return result;
}


Result Shared::write(const ByteBuffer &buffer)
{
	Result result;

	int position = 0;
	const int max_no_progress = 3;
	int no_progress = 0;

	while (position < buffer.size() && getState() == STATE_CONNECTED)
	{
		int pending = buffer.size() - position;
if (DEBUG_WRITE)
{
	GMSEC_DEBUG << fTag.c_str() << "write size=" << buffer.size()
			<< " position=" << position << " pending=" << pending;
}

		int written = write(buffer.raw() + position, pending);

		if (written < 0)
		{
			result = ioError("write(ByteBuffer)");
		}
		else if (written > 0)
		{
			position += written;
			no_progress = 0;
		}
		else
		{
			++no_progress;
			GMSEC_WARNING << fTag.c_str() << "write(ByteBuffer) wrote nothing " << no_progress << " time(s)";
			if (no_progress == max_no_progress)
			{
				result = ioError("giving up on write; assuming bad socket");
			}
		}
	}

	return result;
}


int Shared::write(const u8 *raw, int count)
{
	int written = fSocket.write(reinterpret_cast<const char *>(raw), count);
	if (written <= 0)
	{
		GMSEC_WARNING << fTag.c_str() << "write written=" << written;
	}

	return written;
}


Result Shared::put(const SharedPacket &packet)
{
	Result result;

	if (fState == STATE_CONNECTED)
	{
		ByteBuffer buffer;
		packet->put(buffer);
		result = write(buffer);

if (DEBUG_PUT)
{
	GMSEC_INFO << fTag.c_str() << "put wrote";
	packet->put(std::cout);
}
	}
	else
	{
		result.set(ERR_STATE, "Shared.put: invalid state " + util::toString(_getState()));
	}

	if (result.isError() && fWarnAlways)
	{
		GMSEC_VERBOSE << fTag.c_str() << "put failed " << result;
	}

	return result;
}


int Shared::read(u8 *raw, int count)
{
	int read = 0;

	if (fState == STATE_CONNECTED)
	{
		const int timeout_ms = 100;
		const int delta_ms = 1000;
		read = fSocket.doRead(reinterpret_cast<char*>(raw), count, timeout_ms);
		if (read > 0)
		{
			fNullRead_ms = 0;
			fNullReadLog_ms = 5000;
		}
		else
		{
			fNullRead_ms += timeout_ms;
			if (fNullRead_ms >= fNullReadLog_ms)
			{
				fNullReadLog_ms = fNullRead_ms + delta_ms;
				GMSEC_WARNING << "inactivity: nothing read for " << fNullRead_ms << " [ms]";
			}

			if (fNullRead_ms >= 10000)
			{
				ioError("extended inactivity- assuming broken connection");
			}
		}
	}
	else
	{
		read = -ERR_STATE;
		if (fWarnAlways)
		{
			GMSEC_WARNING << fTag.c_str() << "read: not connected";
		}
	}

	return read;
}


Result Shared::await(int timeout_ms, Test &test)
{
	test.describe(fTag + test.info());
	Result result = util::await(test, fCondition, timeout_ms);
	return result;
}


Code Shared::isAcknowledged(const string &id)
{
	Code code = CODE_FALSE;
	if (fState == STATE_CONNECTED)
	{
		// <algorithm> find(qAcks.begin(), qAcks.end(), id)
		for (std::list<string>::iterator i = qAcks.begin(); i != qAcks.end(); ++i)
			if (*i == id)
			{
				code = SUCCESS;
				break;
			}
	}
	else
		code = CODE_ABORT;

	return code;
}


class TestInput : public Test
{
public:
	TestInput(Shared *s, SharedPacket &out)
		: shared(s), packet(out) { }

	Code apply() {
		Code code = shared->getInput(packet);
		if (packet.get())
			return SUCCESS;
		else
			return code;
	}
private:
	Shared *shared;
	SharedPacket &packet;
};


Result Shared::getInput(SharedPacket &packet, int timeout_ms)
{
	Result result;
	Code code = CODE_FALSE;

	if (timeout_ms > 0)
	{
		TestInput test(this, packet);
		test.setVerbosity(fOptions.getI32("LOG-TestInput", logNONE));
		result = await(timeout_ms, test);
	}
	else
	{
		AutoMutex hold(getMutex());
		if (qIncoming.size() > 0)
		{
			packet = qIncoming.front();
			qIncoming.pop_front();
			code = SUCCESS;
		}

		if (code != SUCCESS)
		{
			result.set(CODE_TIMEOUT, "getInput: nothing pending");
		}
	}

	return result;
}


Code Shared::getInput(SharedPacket &packet)
{
	Code code = CODE_FALSE;

	if (qIncoming.size() > 0)
	{
		packet = qIncoming.front();
		qIncoming.pop_front();
		code = SUCCESS;
	}

	return code;
}


static void logProblems(const SharedPacket &packet)
{
}


static bool canUseDefaultHandler(PacketType type)
{
	if (type == PACKET_PUBLISH)
		return true;
	if (type == PACKET_REQUEST)
		return true;
	return false;
}


void Shared::handle(const SharedPacket &packet)
{
	AutoMutex hold(getMutex());

	ConnectState mode = getMode();
	const Header * header = packet->getHeader();
	PacketType type = header->getType();
	const Meta *meta = packet->getMeta();

	GMSEC_DEBUG << fTag.c_str() << "handle: mode=" << util::toString(mode).c_str() << " packet=" << util::toString(type).c_str();

	if (header->hasProblems())
		logProblems(packet);

	if (type == PACKET_ECHO)
	{
GMSEC_DEBUG << fTag.c_str() << "handle: ECHO ID=" << (meta ? meta->getID().c_str() : "null");
	}
	else if (mode == CONNECT_DISPATCHING)
	{
		bool enqueue = false;
		if (type == PACKET_ACK)
		{
			if (meta)
			{
GMSEC_DEBUG << fTag.c_str() << "handle: ACK ID=" << meta->getID().c_str();
				qAcks.push_back(meta->getID());
				fCondition.broadcast(SIGNAL_INPUT);
			}
			else
			{
				GMSEC_DEBUG << fTag.c_str() << "handle: ACK missing ID";
			}
		}
		else if (fDefaultHandler && canUseDefaultHandler(type))
		{
			fDefaultHandler->onPacket(packet);
		}
		else if (type == PACKET_REPLY)
		{
			// determine if we need to pass the reply through
			// the message and/or reply queue

			// if in the future we allow replies on arbitrary topics, then
			// instead of checking the topic, this will check whether the
			// reply is in response to a request from this connection (and
			// isSubscribed will be unnecessary)
			string topic = meta ? meta->getTopic() : "";

			if ((topic == fReplyTo) || (fReplyTo.empty() && fReplyExpose))
			{
				if (fReplyHandler)
					fReplyHandler->onPacket(packet);
			}

			enqueue = fReplyExpose;
		}
		else
		{
			enqueue = true;
		}

		if (enqueue)
		{
			qIncoming.push_back(packet);
			fCondition.broadcast(SIGNAL_INPUT);
		}
	}
	else
	{
		// still setting up

		if (mode == CONNECT_STARTING && type == PACKET_WELCOME)
		{
			welcome(packet);
		}
		else if (mode == CONNECT_NEGOTIATING && type == PACKET_ACK)
		{
			if (fConnection->resubscribe())
			{
				setMode(CONNECT_RESUBSCRIBING);
			}
			else
			{
				setMode(CONNECT_DISPATCHING);
			}
			fCondition.broadcast(SIGNAL_STATE);
		}
		// else if (mode == CONNECT_RESUBSCRIBING && (type == PACKET_ACK || type == PACKET_ECHO))
		else if (mode == CONNECT_RESUBSCRIBING && type == PACKET_ACK)
		{
			setMode(CONNECT_DISPATCHING);
			fCondition.broadcast(SIGNAL_STATE);
		}
		else
			GMSEC_WARNING << fTag.c_str() << "handle: invalid mode=" << util::toString(mode).c_str() << " packet=" << util::toString(type).c_str();
	}
}



void Shared::welcome(const SharedPacket &in)
{
	setMode(CONNECT_WELCOMED);
	const Meta *meta = in->getMeta();
	if (!meta)
	{
		GMSEC_WARNING << fTag.c_str() << "welcome: missing meta";
		return;
	}

	const Property *p = meta->get("VERSION");
	if (p)
	{
		string version = p->asString();
		GMSEC_VERBOSE << fTag.c_str() << "welcome: VERSION=" << version.c_str();
	}
	else
		GMSEC_WARNING << fTag.c_str() << "welcome: missing VERSION";

	setMode(CONNECT_NEGOTIATING);
	SharedPacket out(new Packet(PACKET_NEGOTIATE));
	Result result = put(out);
	GMSEC_DEBUG << "Shared.welcome: put=" << result;
}


void Shared::setMode(ConnectState m)
{
	AutoMutex hold(getMutex());
	fMode = m;
	GMSEC_DEBUG << fTag.c_str() << "setMode: " << util::toString(m).c_str();
}


void Shared::setDefaultHandler(PacketHandler *handler)
{
	AutoMutex hold(getMutex());
	fDefaultHandler = handler;
}


void Shared::setReplyHandler(PacketHandler *handler)
{
	AutoMutex hold(getMutex());
	fReplyHandler = handler;
}


Options Shared::getOptions() const
{
	return fOptions;
}


void Shared::ping()
{
	if (fMode != CONNECT_DISPATCHING)
	{
		GMSEC_DEBUG << fTag.c_str() << "ping: but mode is " << util::toString(fMode).c_str();
		return;
	}

	SharedPacket out(new Packet(PACKET_ECHO));
	Meta meta;
	gmsec::api::util::Rawbuf<64> buffer;
	static unsigned counter = 0;
	++counter;
	buffer.stream() << this << ':' << counter;
	meta.add(Property::createID(buffer.str()));
	out->setMeta(meta);
	Result result = put(out);
	GMSEC_DEBUG << "Shared.ping: put=" << result;
}


Result Shared::ioError(const string &s)
{
	Result result(ERR_IO, s);

	GMSEC_VERBOSE << fTag.c_str() << s.c_str();

	gmsec::api::util::syncAddAndFetch(&fIoCounter, 1);

	if (fState == STATE_CONNECTED)
	{
		GMSEC_VERBOSE << "changing state from CONNECTED to RECONNECTING";
		fSocket.disconnect();
		setState(STATE_RECONNECTING, true);
	}

	return result;
}


i32 Shared::getIoCounter(i32 &traps)
{
	traps = gmsec::api::util::syncAddAndFetch(&fTraps, 0);
	return gmsec::api::util::syncAddAndFetch(&fIoCounter, 0);
}


} // namespace bolt

