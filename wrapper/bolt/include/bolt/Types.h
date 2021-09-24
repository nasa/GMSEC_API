/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef bolt_Types_h
#define bolt_Types_h


#include <gmsec4/util/Condition.h>
#include <gmsec4/util/Mutex.h>

#include <string>


namespace bolt {


const int DEFAULT_PORT = 9100;


typedef unsigned char u8;
typedef GMSEC_I32 i32;
typedef GMSEC_F64 f64;


using std::string;

using gmsec::api::util::AutoMutex;
using gmsec::api::util::Condition;
using gmsec::api::util::Mutex;



enum Code
{
	SUCCESS,
	CODE_FALSE,
	CODE_TIMEOUT,
	CODE_ABORT,
	CODE_END
};



enum PacketType
{
	PACKET_VOID,

	PACKET_WELCOME = 1,
	PACKET_GOODBYE = 2,
	PACKET_ECHO = 3,
	PACKET_ERROR = 4,
	PACKET_ACK = 5,

	PACKET_NEGOTIATE = 10,
	PACKET_STATISTICS = 11,

	PACKET_SUBSCRIBE = 20,
	PACKET_UNSUBSCRIBE = 21,

	PACKET_PUBLISH = 22,

	PACKET_REQUEST = 23,
	PACKET_REPLY = 24,

	PACKET_END
};


enum State
{
	STATE_UNKNOWN,
	STATE_MANAGED,
	STATE_UNCONNECTED,
	STATE_CONNECTED,
	STATE_RECONNECTING,
	STATE_FINISHED,
	STATE_END
};


enum Signal
{
	SIGNAL_NONE = 0,
	SIGNAL_TIMEOUT = Condition::TIMEOUT,
	SIGNAL_STATE = Condition::USER,
	SIGNAL_OUTPUT,
	SIGNAL_INPUT,
	SIGNAL_END
};


enum ConnectState
{
	CONNECT_STARTING,
	CONNECT_WELCOMED,
	CONNECT_NEGOTIATING,
	CONNECT_RESUBSCRIBING,
	CONNECT_DISPATCHING,
	CONNECT_END
};



struct Server
{
	string host;
	int port;
};


enum Error
{
	ERR_NONE = 0,

	// these match Code
	ERR_FALSE,
	ERR_TIMEOUT,
	ERR_ABORT,

	ERR_STATE,
	ERR_BUG,
	ERR_IO,
	ERR_CONNECT,

	ERR_END
};

} // namespace bolt


#include <bolt/counted_ptr.h>


#endif /* bolt_Types_h */

