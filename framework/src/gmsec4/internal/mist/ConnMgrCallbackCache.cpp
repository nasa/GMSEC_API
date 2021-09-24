/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#include <gmsec4/internal/mist/ConnMgrCallbackCache.h>

#include <gmsec4/internal/mist/ConnMgrCallbacks.h>
#include <gmsec4/internal/mist/InternalConnectionManager.h>

#include <gmsec4/mist/ConnectionManagerCallback.h>
#include <gmsec4/mist/ConnectionManagerEventCallback.h>
#include <gmsec4/mist/ConnectionManagerReplyCallback.h>
#include <gmsec4/mist/SubscriptionInfo.h>

#include <gmsec4/Callback.h>
#include <gmsec4/EventCallback.h>
#include <gmsec4/ReplyCallback.h>
#include <gmsec4/Exception.h>


namespace gmsec
{
namespace api
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


void ConnMgrCallbackCache::putCallback(SubscriptionInfo* info, CMCallback* callback)
{
	gmsec::api::util::AutoMutex lock(m_mutex);

	m_callbackRegistry[info] = callback;
}


void ConnMgrCallbackCache::putEventCallback(EventCallback* callback, ConnectionManagerEventCallback* connMgrCallback)
{
	gmsec::api::util::AutoMutex lock(m_mutex);

	m_eventCallbackRegistry[callback] = connMgrCallback;
}


void ConnMgrCallbackCache::putReplyCallback(ReplyCallback* callback, ConnectionManagerReplyCallback* connMgrCallback)
{
	gmsec::api::util::AutoMutex lock(m_mutex);

	m_replyCallbackRegistry[callback] = connMgrCallback;
}


CMCallback* ConnMgrCallbackCache::getAndRemoveCallback(SubscriptionInfo* info)
{
	gmsec::api::util::AutoMutex lock(m_mutex);

	CMCallback* cb = 0;

	CallbackRegistry::iterator it = m_callbackRegistry.find(info);

	if (it != m_callbackRegistry.end())
	{
		cb = it->second;
		m_callbackRegistry.erase(it);
	}

	return cb;
}


EventCallback* ConnMgrCallbackCache::getAndRemoveEventCallback(ConnectionManagerEventCallback* connMgrCallback)
{
	gmsec::api::util::AutoMutex lock(m_mutex);

	EventCallback* cb = 0;

	EventCallbackRegistry::iterator it = m_eventCallbackRegistry.begin();

	for (; it != m_eventCallbackRegistry.end(); ++it)
	{
		if (it->second == connMgrCallback)
		{
			cb = it->first;
			break;
		}
	}

	if (it != m_eventCallbackRegistry.end())
	{
		m_eventCallbackRegistry.erase(it);
	}

	return cb;
}


ReplyCallback* ConnMgrCallbackCache::getAndRemoveReplyCallback(ConnectionManagerReplyCallback* connMgrCallback)
{
	gmsec::api::util::AutoMutex lock(m_mutex);

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


void ConnMgrCallbackCache::destroyConnMgrCallbacks(ConnectionManager* manager)
{
	gmsec::api::util::AutoMutex lock(m_mutex);

	// Clear out Callbacks associated with the manager
	{
#if 0
		bool done = false;
		while (!done)
		{
			CallbackRegistry::iterator it = m_callbackRegistry.begin();
			bool modified = false;

			while (it != m_callbackRegistry.end() && !modified)
			{
				CMCallback* callback = it->second;

				if (callback && callback->getManager() == manager)
				{
					SubscriptionInfo* sinfo = it->first;

					// Unsubscribing modifies m_callbackRegistry, so need to "start all over
					// again" in iterating to delete callbacks, which is done as part of
					// ConnectionManager::unsubscribe() in addition to other things.
					//
					manager->unsubscribe(sinfo);
					modified = true;
				}
				else
				{
					++it;
				}
			}

			done = (it == m_callbackRegistry.end() || m_callbackRegistry.size() == 0);
		}
#else
		// The ConnectionManager's underlying Connection object takes care of
		// cancelling any outstanding subscriptions that are in place when the
		// connection is disconnected.  Thus all we need to do below is delete
		// the CMCallback objects (which serve as a bridge between MIST and the
		// core API) that are associated with the ConnectionManager that is being
		// destroyed.
		CallbackRegistry::iterator it = m_callbackRegistry.begin();

		while (it != m_callbackRegistry.end())
		{
			CMCallback* callback = it->second;

			if (callback && callback->getManager() == manager)
			{
				delete callback;
				m_callbackRegistry.erase(it++);
			}
			else
			{
				++it;
			}
		}
#endif
	}

	// Clear out EventCallbacks associated with the manager
	{
		EventCallbackRegistry::iterator it = m_eventCallbackRegistry.begin();
		while (it != m_eventCallbackRegistry.end())
		{
			CMEventCallback* callback = dynamic_cast<CMEventCallback*>(it->first);

			if (callback && callback->getManager() == manager)
			{
				delete callback;
				m_eventCallbackRegistry.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}

	// Clear out ReplyCallbacks associated with the manager
	{
		ReplyCallbackRegistry::iterator it = m_replyCallbackRegistry.begin();
		while (it != m_replyCallbackRegistry.end())
		{
			CMReplyCallback* callback = dynamic_cast<CMReplyCallback*>(it->first);

			if (callback && callback->getManager() == manager)
			{
				delete callback;
				m_replyCallbackRegistry.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}
}


ConnMgrCallbackCache::ConnMgrCallbackCache()
	: m_callbackRegistry(),
	  m_eventCallbackRegistry(),
	  m_replyCallbackRegistry()
{
}

} // namespace internal
} // namespace mist
} // namespace api
} // namespace gmsec
