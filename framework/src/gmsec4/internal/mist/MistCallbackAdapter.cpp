/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file MistCallbackAdapter.cpp
 *
 * @brief This file contains functions for the management of C++ callbacks that are
 * created in support of C function callbacks.
 */

#include <gmsec4/internal/mist/MistCallbackAdapter.h>

#include <gmsec4/Message.h>

#include <gmsec4/mist/ConnectionManager.h>
#include <gmsec4/mist/ConnectionManagerCallback.h>
#include <gmsec4/mist/ConnectionManagerEventCallback.h>
#include <gmsec4/mist/ConnectionManagerReplyCallback.h>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{
// C_ConnMgrCallbackAdapter
//
class C_ConnMgrCallbackAdapter : public ConnectionManagerCallback
{
public:
	C_ConnMgrCallbackAdapter(GMSEC_ConnectionMgrCallback* cb) : cb(cb) {}

	void CALL_TYPE onMessage(ConnectionManager& connMgr, const Message& msg)
	{
		Message& m = const_cast<Message&>(msg);

		(*cb)(&connMgr, &m);
	}

private:
	GMSEC_ConnectionMgrCallback* cb;
};


// C_ConnMgrEventCallbackAdapter
//
class C_ConnMgrEventCallbackAdapter : public ConnectionManagerEventCallback
{
public:
	C_ConnMgrEventCallbackAdapter(GMSEC_ConnectionMgrEventCallback* ecb) : ecb(ecb) {}

	void CALL_TYPE onEvent(ConnectionManager& connMgr, const Status& status, Connection::ConnectionEvent event)
	{
		Status& s = const_cast<Status&>(status);
		int cEvent = event;

		(*ecb)(&connMgr, &s, static_cast<GMSEC_ConnectionEvent>(cEvent));
	}

private:
	GMSEC_ConnectionMgrEventCallback* ecb;
};


// C_ConnMgrReplyCallbackAdapter
//
class C_ConnMgrReplyCallbackAdapter : public ConnectionManagerReplyCallback
{
public:
	C_ConnMgrReplyCallbackAdapter(GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb) : rcb(rcb), ecb(ecb) {}

	void CALL_TYPE onReply(ConnectionManager& connMgr, const Message& request, const Message& reply)
	{
		Message& req = const_cast<Message&>(request);
		Message& rep = const_cast<Message&>(reply);

		(*rcb)(&connMgr, &req, &rep);
	}

	void CALL_TYPE onEvent(ConnectionManager& connMgr, const Status& status, Connection::ConnectionEvent event)
	{
		Status& s = const_cast<Status&>(status);
		int cEvent = event;

		(*ecb)(&connMgr, &s, static_cast<GMSEC_ConnectionEvent>(cEvent));
	}

private:
	GMSEC_ConnectionMgrReplyCallback* rcb;
	GMSEC_ConnectionMgrEventCallback* ecb;
};

} // end namespace internal
} // end namespace mist
} // end namespace api
} // end namespace gmsec



using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


MistCallbackAdapter::MistCallbackAdapter()
	: m_callbacks(),
	  m_eventCallbacks(),
	  m_replyCallbacks()
{
}


MistCallbackAdapter::~MistCallbackAdapter()
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


ConnectionManagerCallback* MistCallbackAdapter::createCallback(GMSEC_ConnectionMgrCallback* cb)
{
	C_ConnMgrCallbackAdapter* callback = dynamic_cast<C_ConnMgrCallbackAdapter*>(lookupCallback(cb));

	if (!callback)
	{
		callback = new C_ConnMgrCallbackAdapter(cb);
		m_callbacks[cb] = callback;
	}

	return callback;
}


ConnectionManagerEventCallback* MistCallbackAdapter::createEventCallback(GMSEC_ConnectionMgrEventCallback* ecb)
{
	C_ConnMgrEventCallbackAdapter* callback = dynamic_cast<C_ConnMgrEventCallbackAdapter*>(lookupEventCallback(ecb));

	if (!callback)
	{
		callback = new C_ConnMgrEventCallbackAdapter(ecb);
		m_eventCallbacks[ecb] = callback;
	}

	return callback;
}


ConnectionManagerReplyCallback* MistCallbackAdapter::createReplyCallback(GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb)
{
	C_ConnMgrReplyCallbackAdapter* callback = dynamic_cast<C_ConnMgrReplyCallbackAdapter*>(lookupReplyCallback(rcb));

	if (!callback)
	{
		callback = new C_ConnMgrReplyCallbackAdapter(rcb, ecb);
		m_replyCallbacks[rcb] = callback;
	}

	return callback;
}


void MistCallbackAdapter::deleteCallback(GMSEC_ConnectionMgrCallback* cb)
{
	C_ConnMgrCallbackAdapter* callback = dynamic_cast<C_ConnMgrCallbackAdapter*>(lookupCallback(cb));

	if (callback)
	{
		m_callbacks[cb] = NULL;

		delete callback;
	}
}


void MistCallbackAdapter::deleteEventCallback(GMSEC_ConnectionMgrEventCallback* ecb)
{
	C_ConnMgrEventCallbackAdapter* callback = dynamic_cast<C_ConnMgrEventCallbackAdapter*>(lookupEventCallback(ecb));

	if (callback)
	{
		m_eventCallbacks[ecb] = NULL;

	delete callback;
	}
}


void MistCallbackAdapter::deleteReplyCallback(GMSEC_ConnectionMgrReplyCallback* rcb)
{
	C_ConnMgrReplyCallbackAdapter* callback = dynamic_cast<C_ConnMgrReplyCallbackAdapter*>(lookupReplyCallback(rcb));

	if (callback)
	{
		m_replyCallbacks[rcb] = NULL;

	delete callback;
	}
}


ConnectionManagerCallback* MistCallbackAdapter::lookupCallback(GMSEC_ConnectionMgrCallback* cb)
{
	return m_callbacks[cb];
}


ConnectionManagerEventCallback* MistCallbackAdapter::lookupEventCallback(GMSEC_ConnectionMgrEventCallback* ecb)
{
	return m_eventCallbacks[ecb];
}


ConnectionManagerReplyCallback* MistCallbackAdapter::lookupReplyCallback(GMSEC_ConnectionMgrReplyCallback* rcb)
{
	return m_replyCallbacks[rcb];
}
