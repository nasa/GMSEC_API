/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef MB_CONN_MGR_CALLBACK_H
#define MB_CONN_MGR_CALLBACK_H

#include "ConnectionMgr.h"

#include <gmsec4/internal/CallbackLookup.h>

#include <gmsec4/Message.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec_messagebus
{

class ConnectionMgr;

/**
 * @brief This class implements the callback for sending messages to the appropriate clients.
 */
class ConnMgrCallback : public gmsec::api::internal::MBCallback
{
public:
	/**
	 * @fn ConnMgrCallback(ConnectionMgr* connMgr)
	 * default constructor
	 * the connMgr is the parent connMgr that
	 * this callback services
	 */
	ConnMgrCallback(ConnectionMgr* connMgr);


	virtual void CALL_TYPE onMessage(gmsec::api::Connection& conn, const gmsec::api::Message& msg) {}

	virtual void onMessage(const gmsec::api::Message& msg);


private:
	ConnectionMgr* m_connMgr;
};

}  // end namespace gmsec_messagebus

#endif
