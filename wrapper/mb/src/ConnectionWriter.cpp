/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "ConnectionWriter.h"

#include "ConnectionMgr.h"

#include <gmsec4/util/TimeUtil.h>


using namespace gmsec::api::util;

using namespace gmsec_messagebus;


ConnectionWriter::ConnectionWriter(ConnectionMgr* connectionMgr)
	:
	m_connectionMgr(connectionMgr),
	m_isRunning(false),
	m_keepRunning(false)
{
}


ConnectionWriter::~ConnectionWriter()
{
}


bool ConnectionWriter::isRunning()
{
	return m_isRunning.get();
}


void ConnectionWriter::run()
{
	if (m_connectionMgr == NULL)
	{
		return;
	}

	m_keepRunning.set(true);
	m_isRunning.set(true);

	while (m_keepRunning.get())
	{
		bool sent = m_connectionMgr->sendNextMsgToClient();

		if (!sent)
		{
			TimeUtil::millisleep(10);
		}
	}

	m_isRunning.set(false);
}


void ConnectionWriter::shutdown()
{
	m_keepRunning.set(false);
}
