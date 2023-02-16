/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_MB_CONN_MGR_CALLBACK_H
#define GMSEC_MB_CONN_MGR_CALLBACK_H

#include "ConnectionMgr.h"

#include <gmsec5/internal/CallbackLookup.h>

#include <gmsec5/Message.h>

#include <gmsec5/util/wdllexp.h>


namespace gmsec_messagebus
{

class ConnectionMgr;

/**
 * @brief This class implements the callback for sending messages to the appropriate clients.
 */
class ConnMgrCallback : public gmsec::api5::internal::MBCallback
{
public:
	/**
	 * @fn ConnMgrCallback(ConnectionMgr* connMgr)
	 * default constructor
	 * the connMgr is the parent connMgr that
	 * this callback services
	 */
	ConnMgrCallback(ConnectionMgr* connMgr);


	virtual void CALL_TYPE onMessage(gmsec::api5::Connection& conn, const gmsec::api5::Message& msg) {}

	virtual void onMessage(const gmsec::api5::Message& msg);


private:
	ConnectionMgr* m_connMgr;
};

}  // end namespace gmsec_messagebus

#endif
