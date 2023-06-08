/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/ConnectionCallbackCache.h>

#include <gmsec5/internal/InternalConnection.h>
#include <gmsec5/internal/ConnectionCallback.h>

#include <gmsec5/Callback.h>
#include <gmsec5/Connection.h>
#include <gmsec5/SubscriptionInfo.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


ConnectionCallbackCache& ConnectionCallbackCache::getCache()
{
	static ConnectionCallbackCache cache;
	return cache;
}


void ConnectionCallbackCache::putCallback(SubscriptionInfo* info, ConnectionCallback* callback)
{
	AutoMutex lock(m_mutex);

	m_callbackRegistry[ info ] = callback;
}


ConnectionCallback* ConnectionCallbackCache::getAndRemoveCallback(SubscriptionInfo* info)
{
	AutoMutex lock(m_mutex);

	ConnectionCallback* cb = 0;

	CallbackRegistry::iterator it = m_callbackRegistry.find(info);

	if (it != m_callbackRegistry.end())
	{
		cb = it->second;
		m_callbackRegistry.erase(it);
	}

	return cb;
}


void ConnectionCallbackCache::destroyCallbacks(Connection* conn)
{
	// The Connection object takes care of cancelling any outstanding
	// subscriptions that are in place when the connection is disconnected.
	// All we need to do below is delete the ConnectionCallback objects
	// that are associated with the Connection that is being destroyed.

	AutoMutex lock(m_mutex);

reset_iter:
	for (CallbackRegistry::iterator it = m_callbackRegistry.begin(); it != m_callbackRegistry.end(); ++it)
	{
		ConnectionCallback* callback = it->second;

		if (callback && callback->getConnection() == conn)
		{
			delete callback;
			m_callbackRegistry.erase(it);
			goto reset_iter;
		}
	}
}


ConnectionCallbackCache::ConnectionCallbackCache()
	: m_callbackRegistry()
{
}
