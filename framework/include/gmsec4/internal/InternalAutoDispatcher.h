/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalAutoDispatcher.h
 *
 * @brief This file contains the definition for the AutoDispatcher class.
 */

#ifndef GMSEC_API_INTERNAL_AUTO_MSG_DISPATCHER_H
#define GMSEC_API_INTERNAL_AUTO_MSG_DISPATCHER_H

#include <gmsec4/util/StdSharedPtr.h>
#include <gmsec4/util/StdThread.h>
#include <gmsec4/util/StdUniquePtr.h>
#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{

namespace util
{
class TicketMutex;
}

namespace internal
{
// Forward declaration(s)
class InternalConnection;
class DispatchShared;


class GMSEC_API InternalAutoDispatcher
{
public:
	InternalAutoDispatcher(InternalConnection* connection, gmsec::api::util::TicketMutex& readMutex, gmsec::api::util::TicketMutex& writeMutex);


	~InternalAutoDispatcher();


	bool CALL_TYPE start();


	bool CALL_TYPE stop(bool waitForCompletion);


	bool CALL_TYPE isRunning();


private:
	typedef gmsec::api::util::StdUniquePtr<gmsec::api::util::StdThread> DispatcherThread;
	typedef gmsec::api::util::StdSharedPtr<DispatchShared>              DispatcherSharedPtr;


	InternalConnection*             m_iConn;
	gmsec::api::util::TicketMutex&  m_readMutex;
	gmsec::api::util::TicketMutex&  m_writeMutex;
	gmsec::api::util::AtomicBoolean m_dispatcherRunning;
	DispatcherThread                m_dispatcher;
	DispatcherSharedPtr             m_dispatchShared;
};

}  // end namespace internal
}  // end namespace api
}  // end namespace gmsec

#endif
