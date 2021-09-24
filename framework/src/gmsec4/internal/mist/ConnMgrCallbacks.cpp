/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#include <gmsec4/internal/mist/ConnMgrCallbacks.h>

#include <gmsec4/internal/mist/ConnMgrCallbackCache.h>

#include <gmsec4/internal/ConnectionBuddy.h>

#include <gmsec4/mist/ConnectionManager.h>
#include <gmsec4/mist/ConnectionManagerCallback.h>
#include <gmsec4/mist/ConnectionManagerEventCallback.h>
#include <gmsec4/mist/ConnectionManagerReplyCallback.h>
#include <gmsec4/mist/Specification.h>

#include <gmsec4/Message.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


//
// CMCallback -- supports ConnectionManagerCallback
//
CMCallback::CMCallback(ConnectionManager* manager, ConnectionManagerCallback* cb, bool doValidate)
	: m_mgr(manager),
	  m_cb(cb),
	  m_doValidate(doValidate)
{
}


void CMCallback::onMessage(Connection& conn, const Message& msg)
{
	if (m_doValidate)
	{
		try
		{
			m_mgr->getSpecification().validateMessage(msg);
		}
		catch (const Exception& e)
		{
			GMSEC_WARNING << "Received invalid message; reason:\n" << e.what();

			ConnectionBuddy::getInternal(conn).dispatchEvent(Connection::INVALID_MESSAGE_EVENT, Status(e));

			return;
		}
	}

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


void CMEventCallback::onEvent(Connection& conn, const Status& status, Connection::ConnectionEvent event)
{
	m_cb->onEvent(*m_mgr, status, event);
}


//
// CMReplyCallback -- supports ConnectionManagerReplyCallback
//
CMReplyCallback::CMReplyCallback(ConnectionManager* manager, ConnectionManagerReplyCallback* cb, bool doValidate)
	: m_mgr(manager),
	  m_cb(cb),
	  m_doValidate(doValidate)
{
}


void CMReplyCallback::onReply(Connection& conn, const Message& req, const Message& rep)
{
	if (m_doValidate)
	{
		try
		{
			m_mgr->getSpecification().validateMessage(rep);
		}
		catch (const Exception& e)
		{
			GMSEC_WARNING << "Received invalid response message; reason:\n" << e.what();

			ConnectionBuddy::getInternal(conn).dispatchEvent(Connection::INVALID_MESSAGE_EVENT, Status(e));

			return;
		}
	}

	m_cb->onReply(*m_mgr, req, rep);
}


void CMReplyCallback::onEvent(Connection& conn, const Status& status, Connection::ConnectionEvent event)
{
	m_cb->onEvent(*m_mgr, status, event);
}
