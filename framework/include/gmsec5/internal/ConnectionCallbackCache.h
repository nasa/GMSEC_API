/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_CONNECTION_CALLBACK_CACHE_H
#define GMSEC_API5_INTERNAL_CONNECTION_CALLBACK_CACHE_H

#include <gmsec5/util/Mutex.h>

#include <map>

namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Callback;
	class Connection;
	class SubscriptionInfo;

namespace internal
{
	// Forward declaration(s)
	class InternalConnection;
	class ConnectionCallback;


class ConnectionCallbackCache
{
public:
	static ConnectionCallbackCache& getCache();

	void putCallback(SubscriptionInfo* info, ConnectionCallback* callback);

	ConnectionCallback* getAndRemoveCallback(SubscriptionInfo* info);

	void destroyCallbacks(Connection* conn);

private:
	ConnectionCallbackCache();

	typedef std::map<SubscriptionInfo*, ConnectionCallback*> CallbackRegistry;

	CallbackRegistry m_callbackRegistry;

	gmsec::api5::util::Mutex m_mutex;
};

}  //namespace internal
}  //namespace api5
}  //namespace gmsec

#endif
