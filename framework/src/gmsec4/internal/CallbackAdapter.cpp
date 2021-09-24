/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file CallbackAdapter.cpp
 *
 * @brief This file contains functions for the management of C++ callbacks that are
 * created in support of C function callbacks.
 */


#include <gmsec4/internal/CallbackAdapter.h>

#include <gmsec4/Callback.h>
#include <gmsec4/Connection.h>
#include <gmsec4/EventCallback.h>
#include <gmsec4/Message.h>
#include <gmsec4/ReplyCallback.h>


namespace gmsec
{
namespace api
{
namespace internal
{

// C_CallbackAdapter
//
class C_CallbackAdapter : public gmsec::api::Callback
{
public:
	C_CallbackAdapter(GMSEC_Callback* cb) : cb(cb) {}

	void CALL_TYPE onMessage(Connection& conn, const Message& msg)
	{
		Message& m = const_cast<Message&>(msg);
 
		(*cb)(&conn, &m);
	}

private:
	GMSEC_Callback* cb;
};


// C_EventCallbackAdapter
//
class C_EventCallbackAdapter : public gmsec::api::EventCallback
{
public:
	C_EventCallbackAdapter(GMSEC_EventCallback* ecb) : ecb(ecb) {}

	void CALL_TYPE onEvent(Connection& conn, const Status& status, Connection::ConnectionEvent event)
	{
		Status& s = const_cast<Status&>(status);
		int cEvent = event;

		(*ecb)(&conn, &s, static_cast<GMSEC_ConnectionEvent>(cEvent));
	}

private:
	GMSEC_EventCallback* ecb;
};


// C_ReplyCallbackAdapter
//
class C_ReplyCallbackAdapter : public gmsec::api::ReplyCallback
{
public:
	C_ReplyCallbackAdapter(GMSEC_ReplyCallback* rcb, GMSEC_EventCallback* ecb) : rcb(rcb), ecb(ecb) {}

	void CALL_TYPE onReply(Connection& conn, const Message& request, const Message& reply)
	{
		Message& req = const_cast<Message&>(request);
		Message& rep = const_cast<Message&>(reply);

		(*rcb)(&conn, &req, &rep);
	}

	void CALL_TYPE onEvent(Connection& conn, const Status& status, Connection::ConnectionEvent event)
	{
		Status& s = const_cast<Status&>(status);
		int cEvent = event;

		(*ecb)(&conn, &s, static_cast<GMSEC_ConnectionEvent>(cEvent));
	}

private:
	GMSEC_ReplyCallback* rcb;
	GMSEC_EventCallback* ecb;
};

} // end namespace internal
} // end namespace api
} // end namespace gmsec


using namespace gmsec::api;
using namespace gmsec::api::internal;


CallbackAdapter::CallbackAdapter()
	: m_callbacks(),
	  m_eventCallbacks(),
	  m_replyCallbacks()
{
}


CallbackAdapter::~CallbackAdapter()
{
	for (Callbacks::iterator it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
	for (EventCallbacks::iterator it = m_eventCallbacks.begin(); it != m_eventCallbacks.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
	for (ReplyCallbacks::iterator it = m_replyCallbacks.begin(); it != m_replyCallbacks.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
}


Callback* CallbackAdapter::createCallback(GMSEC_Callback* cb)
{
	C_CallbackAdapter* callback = dynamic_cast<C_CallbackAdapter*>(lookupCallback(cb));

	if (!callback)
	{
		callback = new C_CallbackAdapter(cb);
		m_callbacks[cb] = callback;
	}

	return callback;
}


EventCallback* CallbackAdapter::createEventCallback(GMSEC_EventCallback* ecb)
{
	C_EventCallbackAdapter* callback = dynamic_cast<C_EventCallbackAdapter*>(lookupEventCallback(ecb));

	if (!callback)
	{
		callback = new C_EventCallbackAdapter(ecb);
		m_eventCallbacks[ecb] = callback;
	}

	return callback;
}


ReplyCallback* CallbackAdapter::createReplyCallback(GMSEC_ReplyCallback* rcb, GMSEC_EventCallback* ecb)
{
	C_ReplyCallbackAdapter* callback = dynamic_cast<C_ReplyCallbackAdapter*>(lookupReplyCallback(rcb));

	if (!callback)
	{
		callback = new C_ReplyCallbackAdapter(rcb, ecb);
		m_replyCallbacks[rcb] = callback;
	}

	return callback;
}


void CallbackAdapter::deleteCallback(GMSEC_Callback* cb)
{
	C_CallbackAdapter* callback = dynamic_cast<C_CallbackAdapter*>(lookupCallback(cb));

	if (callback)
	{
		m_callbacks[cb] = NULL;

		delete callback;
	}
}


void CallbackAdapter::deleteEventCallback(GMSEC_EventCallback* ecb)
{
	C_EventCallbackAdapter* callback = dynamic_cast<C_EventCallbackAdapter*>(lookupEventCallback(ecb));

	if (callback)
	{
		m_eventCallbacks[ecb] = NULL;

		delete callback;
	}
}


void CallbackAdapter::deleteReplyCallback(GMSEC_ReplyCallback* rcb)
{
	C_ReplyCallbackAdapter* callback = dynamic_cast<C_ReplyCallbackAdapter*>(lookupReplyCallback(rcb));

	if (callback)
	{
		m_replyCallbacks[rcb] = NULL;

		delete callback;
	}
}


Callback* CallbackAdapter::lookupCallback(GMSEC_Callback* cb)
{
	return m_callbacks[cb];
}


EventCallback* CallbackAdapter::lookupEventCallback(GMSEC_EventCallback* ecb)
{
	return m_eventCallbacks[ecb];
}


ReplyCallback* CallbackAdapter::lookupReplyCallback(GMSEC_ReplyCallback* rcb)
{
	return m_replyCallbacks[rcb];
}
