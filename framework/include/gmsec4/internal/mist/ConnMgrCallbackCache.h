/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#ifndef CONN_MGR_CALLBACK_CACHE_H
#define CONN_MGR_CALLBACK_CACHE_H

#include <gmsec4/util/Mutex.h>

#include <map>

namespace gmsec
{
namespace api
{
	class Callback;
	class EventCallback;
	class ReplyCallback;

namespace mist
{
	class ConnectionManager;
	class ConnectionManagerCallback;
	class ConnectionManagerEventCallback;
	class ConnectionManagerReplyCallback;
	class SubscriptionInfo;

namespace internal
{
	class InternalConnectionManager;
	class CMCallback;


class ConnMgrCallbackCache
{
public:
	static ConnMgrCallbackCache& getCache();

	void putCallback(SubscriptionInfo* info, CMCallback* callback);
	void putEventCallback(EventCallback* callback, ConnectionManagerEventCallback* connMgrCallback);
	void putReplyCallback(ReplyCallback* callback, ConnectionManagerReplyCallback* connMgrCallback);

	CMCallback*    getAndRemoveCallback(SubscriptionInfo* info);
	EventCallback* getAndRemoveEventCallback(ConnectionManagerEventCallback* connMgrCallback);
	ReplyCallback* getAndRemoveReplyCallback(ConnectionManagerReplyCallback* connMgrCallback);

	void destroyConnMgrCallbacks(ConnectionManager* manager);

private:
	ConnMgrCallbackCache();

	typedef std::map<SubscriptionInfo*, CMCallback*>                  CallbackRegistry;
	typedef std::map<EventCallback*, ConnectionManagerEventCallback*> EventCallbackRegistry;
	typedef std::map<ReplyCallback*, ConnectionManagerReplyCallback*> ReplyCallbackRegistry;

	CallbackRegistry      m_callbackRegistry;
	EventCallbackRegistry m_eventCallbackRegistry;
	ReplyCallbackRegistry m_replyCallbackRegistry;

	gmsec::api::util::Mutex m_mutex;
};

}  //namespace internal
}  //namespace mist
}  //namespace api
}  //namespace gmsec

#endif
