/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef MB_CONNECTION_WRITER_H
#define MB_CONNECTION_WRITER_H


#include <gmsec4/util/Atomics.h>


namespace gmsec_messagebus
{

// Forward declaration(s)
class ConnectionMgr;


/**
 * @brief This file contains the declaration for the ConnectionWriter class used by the Magic Bus
 */
class ConnectionWriter
{
public:
	ConnectionWriter(ConnectionMgr* connectionMgr);

	~ConnectionWriter();

	void run();

	void shutdown();

	bool isRunning();


private:
	ConnectionMgr*                  m_connectionMgr;
	gmsec::api::util::AtomicBoolean m_isRunning;
	gmsec::api::util::AtomicBoolean m_keepRunning;
};

}  // end namespace gmsec_messagebus

#endif
