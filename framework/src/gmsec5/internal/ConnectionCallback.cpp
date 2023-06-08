/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/ConnectionCallback.h>

#include <gmsec5/internal/InternalConnection.h>

#include <gmsec5/Connection.h>
#include <gmsec5/Message.h>
#include <gmsec5/Status.h>

#include <gmsec5/util/Log.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


ConnectionCallback::ConnectionCallback(Connection* conn, Callback* userCallback, bool doValidate)
	: m_conn(conn),
	  m_userCallback(userCallback),
	  m_doValidate(doValidate)
{
}


void ConnectionCallback::onMessage(Connection& conn, const Message& msg)
{
	if (doValidate())
	{
		Status status = msg.isCompliant();

		if (status.hasError())
		{
			GMSEC_WARNING << "Received non-compliant message; reason:\n" << status.get();

			ConnectionBuddy::getInternal(conn).dispatchEvent(Connection::Event::INVALID_MESSAGE_EVENT, status);

			return;   // do not call user's callback
		}
	}

	m_userCallback->onMessage(conn, msg);
}
