/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#include <gmsec4/internal/mist/ConnMgrCallbacks.h>

#include <gmsec4/mist/ConnectionManager.h>
#include <gmsec4/mist/ConnectionManagerCallback.h>
#include <gmsec4/mist/ConnectionManagerEventCallback.h>
#include <gmsec4/mist/ConnectionManagerReplyCallback.h>

#include <gmsec4/internal/mist/ConnMgrCallbackCache.h>

#include <gmsec4/Message.h>
#include <gmsec4/Status.h>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

//
// CMCallback -- supports ConnectionManagerCallback
//
CMCallback::CMCallback(ConnectionManager* manager, ConnectionManagerCallback* cb)
	: m_mgr(manager),
	  m_cb(cb)
{
}


ConnectionManager* CMCallback::getManager() const
{
	return m_mgr;
}


ConnectionManagerCallback* CMCallback::getCallback() const
{
	return m_cb;
}


void CMCallback::onMessage(Connection& conn, const Message& msg)
{
	m_cb->onMessage(*m_mgr, msg);
}


//
// CMEventCallback -- supports ConnectionManagerEventCallback
//
CMEventCallback::CMEventCallback(ConnectionManager* manager, ConnectionManagerEventCallback* cb)
	: m_mgr(manager),
	  m_cb(cb)
{
}


ConnectionManager* CMEventCallback::getManager() const
{
	return m_mgr;
}


ConnectionManagerEventCallback* CMEventCallback::getCallback() const
{
	return m_cb;
}


void CMEventCallback::onEvent(Connection& conn, const Status& status, Connection::ConnectionEvent event)
{
	m_cb->onEvent(*m_mgr, status, event);
}


//
// CMReplyCallback -- supports ConnectionManagerReplyCallback
//
CMReplyCallback::CMReplyCallback(ConnectionManager* manager, ConnectionManagerReplyCallback* cb)
	: m_mgr(manager),
	  m_cb(cb)
{
}


ConnectionManager* CMReplyCallback::getManager() const
{
	return m_mgr;
}


ConnectionManagerReplyCallback* CMReplyCallback::getCallback() const
{
	return m_cb;
}


void CMReplyCallback::onReply(Connection& conn, const Message& req, const Message& rep)
{
	m_cb->onReply(*m_mgr, req, rep);
}


void CMReplyCallback::onEvent(Connection& conn, const Status& status, Connection::ConnectionEvent event)
{
	m_cb->onEvent(*m_mgr, status, event);
}

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec
