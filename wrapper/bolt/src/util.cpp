
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#ifdef __GNUC__
#include <cxxabi.h>
#endif

#include <bolt/util.h>

#include <gmsec4/internal/Rawbuf.h>

namespace bolt {

using std::string;
using gmsec::api::util::Rawbuf;


void util::zeroBytes (void *p, size_t count)
{
	std::memset(p, 0, count);
}


string util::toString (i32 value)
{
	Rawbuf<64> buffer;
	buffer.stream() << value;
	return buffer.str();
}


string util::toString (State s)
{
	if (s >= STATE_UNKNOWN && s < STATE_END)
	{
		static const string nice[] = {
			"UNKNOWN",
			"MANAGED",
			"UNCONNECTED",
			"CONNECTED",
			"RECONNECTING",
			"FINISHED"
		};
		return nice[s];
	}

	Rawbuf<64> buffer;
	buffer.stream() << "State[" << s << ']';
	return buffer.str();
}


string util::toString (Signal s)
{
	if (s >= SIGNAL_NONE && s < SIGNAL_END)
	{
		static const string nice[] = {
			"NONE",
			"TIMEOUT",
			"STATE",
			"ACK",
			"SENT",
			"END"
		};
		return nice[s];
	}

	Rawbuf<64> buffer;
	buffer.stream() << "Signal[" << s << ']';
	return buffer.str();
}


string util::toString (ConnectState s)
{
	if (s >= CONNECT_STARTING && s < CONNECT_END)
	{
		static const string nice[] = {
			"STARTING",
			"WELCOMED",
			"NEGOTIATING",
			"RESUBSCRIBING",
			"DISPATCHING",
			"END"
		};
		return nice[s];
	}

	Rawbuf<64> buffer;
	buffer.stream() << "ConnectState[" << s << ']';
	return buffer.str();
}


string util::toString (PacketType t)
{
	string out;
	switch (t)
	{
	case PACKET_VOID:
		out = "VOID";
		break;
	case PACKET_WELCOME:
		out = "WELCOME";
		break;
	case PACKET_GOODBYE:
		out = "GOODBYE";
		break;
	case PACKET_ECHO:
		out = "ECHO";
		break;
	case PACKET_ERROR:
		out = "ERROR";
		break;
	case PACKET_ACK:
		out = "ACK";
		break;
	case PACKET_NEGOTIATE:
		out = "NEGOTIATE";
		break;
	case PACKET_STATISTICS:
		out = "STATISTICS";
		break;
	case PACKET_SUBSCRIBE:
		out = "SUBSCRIBE";
		break;
	case PACKET_UNSUBSCRIBE:
		out = "UNSUBSCRIBE";
		break;
	case PACKET_PUBLISH:
		out = "PUBLISH";
		break;
	case PACKET_REQUEST:
		out = "REQUEST";
		break;
	case PACKET_REPLY:
		out = "REPLY";
		break;
	default: {
		Rawbuf<64> buffer;
		buffer.stream() << "PacketType[" << t << ']';
		out = buffer.str();
		break;
		}
	}

	return out;
}


string util::toString (Code code)
{
	if (code >= SUCCESS && code < CODE_END)
	{
		static const string nice[] = {
			"SUCCESS",
			"FALSE",
			"TIMEOUT",
			"ABORT",
			"END"
		};
		return nice[code];
	}

	Rawbuf<64> buffer;
	buffer.stream() << "Code[" << code << ']';
	return buffer.str();
}


string util::toString (Error error)
{
	if (error >= ERR_NONE && error < ERR_END)
	{
		static const string nice[] = {
			"OK",
			"FALSE",
			"TIMEOUT",
			"ABORT",
			"STATE",
			"BUG",
			"IO",
			"CONNECT",
			"END"
		};
		return nice[error];
	}

	Rawbuf<64> buffer;
	buffer.stream() << "Error[" << error << ']';
	return buffer.str();
}


void util::set(Result &result, Code code)
{
	if (code == SUCCESS)
		result.clear();
	else
		result.set(code, util::toString(code));
}


#ifdef __GNUC__

string util::getname(const std::type_info &t)
{
	return util::demangle(t.name());
}


string util::demangle(const char *s)
{
	int tmp = 0;
	const char * t = abi::__cxa_demangle(s, 0, 0, &tmp);
	string out(t);
	std::free((void*) t);
	return out;
}

#else

string util::getname(const std::type_info &t)
{
	return t.name();
}

string util::demangle(const char *s)
{
	return s;
}
#endif

} // namespace bolt

