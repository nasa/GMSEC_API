/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include "ConnMgrCallback.h"

#include "MBFastMessage.h"

using namespace gmsec::api;

using namespace gmsec_messagebus;


ConnMgrCallback::ConnMgrCallback(ConnectionMgr* connMgr)
	:
	m_connMgr(connMgr)
{
}


void ConnMgrCallback::onMessage(const Message& msg)
{
	m_connMgr->sendMsgToClient(new MBFastMessage(new Message(msg), m_connMgr));
}
