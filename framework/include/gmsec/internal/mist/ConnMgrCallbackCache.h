/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */




#ifndef CONN_MGR_CALLBACK_CACHE_H
#define CONN_MGR_CALLBACK_CACHE_H

#include <gmsec/util/Mutex.h>

#include <map>

namespace gmsec
{
class Callback;
class ErrorCallback;
class ReplyCallback;

namespace mist
{
class ConnectionManagerCallback;
class ConnectionManagerErrorCallback;
class ConnectionManagerReplyCallback;

namespace internal
{
class BaseConnectionManager;


class ConnMgrCallbackCache
{
public:
	static ConnMgrCallbackCache& getCache();

	void putCallback(Callback* callback, ConnectionManagerCallback* connMgrCallback);
	void putErrorCallback(ErrorCallback* callback, ConnectionManagerErrorCallback* connMgrCallback);
	void putReplyCallback(ReplyCallback* callback, ConnectionManagerReplyCallback* connMgrCallback);

	ConnectionManagerCallback* getCallback(Callback* callback);
	ConnectionManagerErrorCallback* getErrorCallback(ErrorCallback* callback);
	ConnectionManagerReplyCallback* getReplyCallback(ReplyCallback* callback);

	Callback* getAndRemoveCallback(ConnectionManagerCallback* connMgrCallback);
	ErrorCallback* getAndRemoveErrorCallback(ConnectionManagerErrorCallback* connMgrCallback);
	ReplyCallback* getAndRemoveReplyCallback(ConnectionManagerReplyCallback* connMgrCallback);

	void destroyConnMgrCallbacks(BaseConnectionManager* baseConnMgr);

private:
	ConnMgrCallbackCache();

	typedef std::map<Callback*,      ConnectionManagerCallback*>      CallbackRegistry;
	typedef std::map<ErrorCallback*, ConnectionManagerErrorCallback*> ErrorCallbackRegistry;
	typedef std::map<ReplyCallback*, ConnectionManagerReplyCallback*> ReplyCallbackRegistry;

	CallbackRegistry      m_callbackRegistry;
	ErrorCallbackRegistry m_errorCallbackRegistry;
	ReplyCallbackRegistry m_replyCallbackRegistry;

	gmsec::util::Mutex    m_mutex;
};

}
}
}

#endif
