/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_MB_CONNECTION_WRITER_H
#define GMSEC_MB_CONNECTION_WRITER_H


#include <gmsec5/util/Atomics.h>


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
	ConnectionMgr*                   m_connectionMgr;
	gmsec::api5::util::AtomicBoolean m_isRunning;
	gmsec::api5::util::AtomicBoolean m_keepRunning;
};

}  // end namespace gmsec_messagebus

#endif
