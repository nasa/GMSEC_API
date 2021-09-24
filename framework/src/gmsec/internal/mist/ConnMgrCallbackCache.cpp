/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */




#include <gmsec/internal/mist/ConnMgrCallbackCache.h>

#include <gmsec/Callback.h>
#include <gmsec/ErrorCallback.h>
#include <gmsec/ReplyCallback.h>

#include <gmsec/mist/ConnectionManagerCallback.h>
#include <gmsec/mist/ConnectionManagerErrorCallback.h>
#include <gmsec/mist/ConnectionManagerReplyCallback.h>

#include <gmsec/internal/mist/ConnMgrCallbacks.h>


namespace gmsec
{
namespace mist
{
namespace internal
{

ConnMgrCallbackCache& ConnMgrCallbackCache::getCache()
{
	static ConnMgrCallbackCache cache;
	return cache;
}


void ConnMgrCallbackCache::putCallback(Callback* callback, ConnectionManagerCallback* connMgrCallback)
{
	gmsec::util::AutoMutex lock(m_mutex);

	if (m_callbackRegistry.find(callback) == m_callbackRegistry.end())
	{
		m_callbackRegistry[callback] = connMgrCallback;
	}
	else
	{
		// TODO:  Issue warning that callback is already registered
	}
}


void ConnMgrCallbackCache::putErrorCallback(ErrorCallback* callback, ConnectionManagerErrorCallback* connMgrCallback)
{
	gmsec::util::AutoMutex lock(m_mutex);

	if (m_errorCallbackRegistry.find(callback) == m_errorCallbackRegistry.end())
	{
		m_errorCallbackRegistry[callback] = connMgrCallback;
	}
	else
	{
		// TODO:  Issue warning that callback is already registered
	}
}


void ConnMgrCallbackCache::putReplyCallback(ReplyCallback* callback, ConnectionManagerReplyCallback* connMgrCallback)
{
	gmsec::util::AutoMutex lock(m_mutex);

	if (m_replyCallbackRegistry.find(callback) == m_replyCallbackRegistry.end())
	{
		m_replyCallbackRegistry[callback] = connMgrCallback;
	}
	else
	{
		// TODO:  Issue warning that callback is already registered
	}
}


ConnectionManagerCallback* ConnMgrCallbackCache::getCallback(Callback* callback)
{
	gmsec::util::AutoMutex lock(m_mutex);

	CallbackRegistry::iterator it = m_callbackRegistry.find(callback);

	ConnectionManagerCallback* cmCallback = 0;

	if (it != m_callbackRegistry.end())
	{
		cmCallback = it->second;
	}

	return cmCallback;
}


ConnectionManagerErrorCallback* ConnMgrCallbackCache::getErrorCallback(ErrorCallback* callback)
{
	gmsec::util::AutoMutex lock(m_mutex);

	ErrorCallbackRegistry::iterator it = m_errorCallbackRegistry.find(callback);

	ConnectionManagerErrorCallback* cmCallback = 0;

	if (it != m_errorCallbackRegistry.end())
	{
		cmCallback = it->second;
	}

	return cmCallback;
}


ConnectionManagerReplyCallback* ConnMgrCallbackCache::getReplyCallback(ReplyCallback* callback)
{
	gmsec::util::AutoMutex lock(m_mutex);

	ReplyCallbackRegistry::iterator it = m_replyCallbackRegistry.find(callback);

	ConnectionManagerReplyCallback* cmCallback = 0;

	if (it != m_replyCallbackRegistry.end())
	{
		cmCallback = it->second;
	}

	return cmCallback;
}


Callback* ConnMgrCallbackCache::getAndRemoveCallback(ConnectionManagerCallback* connMgrCallback)
{
	gmsec::util::AutoMutex lock(m_mutex);

	Callback* cb = 0;

	CallbackRegistry::iterator it = m_callbackRegistry.begin();

	for (; it != m_callbackRegistry.end(); ++it)
	{
		if (it->second == connMgrCallback)
		{
			cb = it->first;
			break;
		}
	}

	if (it != m_callbackRegistry.end())
	{
		m_callbackRegistry.erase(it);
	}

	return cb;
}


ErrorCallback* ConnMgrCallbackCache::getAndRemoveErrorCallback(ConnectionManagerErrorCallback* connMgrCallback)
{
	gmsec::util::AutoMutex lock(m_mutex);

	ErrorCallback* cb = 0;

	ErrorCallbackRegistry::iterator it = m_errorCallbackRegistry.begin();

	for (; it != m_errorCallbackRegistry.end(); ++it)
	{
		if (it->second == connMgrCallback)
		{
			cb = it->first;
			break;
		}
	}

	if (it != m_errorCallbackRegistry.end())
	{
		m_errorCallbackRegistry.erase(it);
	}

	return cb;
}


ReplyCallback* ConnMgrCallbackCache::getAndRemoveReplyCallback(ConnectionManagerReplyCallback* connMgrCallback)
{
	gmsec::util::AutoMutex lock(m_mutex);

	ReplyCallback* cb = 0;

	ReplyCallbackRegistry::iterator it = m_replyCallbackRegistry.begin();

	for (; it != m_replyCallbackRegistry.end(); ++it)
	{
		if (it->second == connMgrCallback)
		{
			cb = it->first;
			break;
		}
	}

	if (it != m_replyCallbackRegistry.end())
	{
		m_replyCallbackRegistry.erase(it);
	}

	return cb;
}


void ConnMgrCallbackCache::destroyConnMgrCallbacks(BaseConnectionManager* baseConnMgr)
{
	gmsec::util::AutoMutex lock(m_mutex);

	repeatCallbackFind:
	for (CallbackRegistry::iterator it = m_callbackRegistry.begin(); it != m_callbackRegistry.end(); ++it)
	{
		CMCallback* callback = dynamic_cast<CMCallback*>(it->first);

		if (callback && callback->getManager() == baseConnMgr)
		{
			delete callback;
			m_callbackRegistry.erase(it);
			goto repeatCallbackFind;
		}
	}

	repeatErrorFind:
	for (ErrorCallbackRegistry::iterator it = m_errorCallbackRegistry.begin(); it != m_errorCallbackRegistry.end(); ++it)
	{
		CMErrorCallback* callback = dynamic_cast<CMErrorCallback*>(it->first);

		if (callback && callback->getManager() == baseConnMgr)
		{
			delete callback;
			m_errorCallbackRegistry.erase(it);
			goto repeatErrorFind;
		}
	}

	repeatReplyFind:
	for (ReplyCallbackRegistry::iterator it = m_replyCallbackRegistry.begin(); it != m_replyCallbackRegistry.end(); ++it)
	{
		CMReplyCallback* callback = dynamic_cast<CMReplyCallback*>(it->first);

		if (callback && callback->getManager() == baseConnMgr)
		{
			delete callback;
			m_replyCallbackRegistry.erase(it);
			goto repeatReplyFind;
		}
	}
}


ConnMgrCallbackCache::ConnMgrCallbackCache()
	: m_callbackRegistry(),
	  m_errorCallbackRegistry(),
	  m_replyCallbackRegistry()
{
}

}
}
}
