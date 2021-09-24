/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */




#include <gmsec/internal/mist/ConnMgrCallbacks.h>

#include <gmsec/Connection.h>
#include <gmsec/Message.h>
#include <gmsec/Status.h>

#include <gmsec/mist/ConnectionManager.h>
#include <gmsec/mist/ConnectionManagerCallback.h>
#include <gmsec/mist/ConnectionManagerErrorCallback.h>
#include <gmsec/mist/ConnectionManagerReplyCallback.h>

#include <gmsec/internal/mist/ConnMgrCallbackCache.h>


namespace gmsec
{
namespace mist
{
namespace internal
{

//
// CMCallback -- supports ConnectionManagerCallback
//
CMCallback::CMCallback(BaseConnectionManager* manager)
	: m_manager(manager)
{
}


BaseConnectionManager* CMCallback::getManager() const
{
	return m_manager;
}


void CMCallback::OnMessage(Connection* conn, Message* msg)
{
	// We need to call the user app's ConnectionManagerCallback

	ConnectionManagerCallback* callback = ConnMgrCallbackCache::getCache().getCallback(this);

	if (callback)
	{
		ConnectionManager connMgr(m_manager);

		callback->OnMessage(&connMgr, msg);
	}
}


//
// CMErrorCallback -- supports ConnectionManagerErrorCallback
//
CMErrorCallback::CMErrorCallback(BaseConnectionManager* manager)
	: m_manager(manager)
{
}


BaseConnectionManager* CMErrorCallback::getManager() const
{
	return m_manager;
}


void CMErrorCallback::OnError(Connection* conn, Message* msg, Status* status, const char* event)
{
	// We need to call the user app's ConnectionManagerErrorCallback

	ConnectionManagerErrorCallback* callback = ConnMgrCallbackCache::getCache().getErrorCallback(this);

	if (callback)
	{
		ConnectionManager connMgr(m_manager);

		callback->OnError(&connMgr, msg, status, event);
	}
}


//
// CMReplyCallback -- supports ConnectionManagerReplyCallback
//
CMReplyCallback::CMReplyCallback(BaseConnectionManager* manager)
	: m_manager(manager)
{
}


BaseConnectionManager* CMReplyCallback::getManager() const
{
	return m_manager;
}


void CMReplyCallback::OnReply(Connection* conn, Message* req, Message* rep)
{
	// We need to call the user app's ConnectionManagerReplyCallback

	ConnectionManagerReplyCallback* callback = ConnMgrCallbackCache::getCache().getReplyCallback(this);

	if (callback)
	{
		ConnectionManager connMgr(m_manager);

		callback->OnReply(&connMgr, req, rep);

		(void) ConnMgrCallbackCache::getCache().getAndRemoveReplyCallback(callback);

		// Is this legal?  We're done with this CMReplyCallback object, but we cannot
		// delete it elsewhere since we cannot predict when we're done with it.
		// delete this;
	}
}


void CMReplyCallback::OnError(Connection* conn, Message* req, Status* status, const char* event)
{
	// We need to call the user app's ConnectionManagerReplyCallback

	ConnectionManagerReplyCallback* callback = ConnMgrCallbackCache::getCache().getReplyCallback(this);

	if (callback)
	{
		ConnectionManager connMgr(m_manager);

		callback->OnError(&connMgr, req, status, event);
	}
}

}
}
}
