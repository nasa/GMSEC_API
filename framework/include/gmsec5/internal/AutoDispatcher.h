/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file AutoDispatcher.h
 *
 * @brief This file contains the definition for the AutoDispatcher class.
 */

#ifndef GMSEC_API5_INTERNAL_AUTO_DISPATCHER_H
#define GMSEC_API5_INTERNAL_AUTO_DISPATCHER_H

#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/StdThread.h>
#include <gmsec5/util/StdUniquePtr.h>
#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	namespace internal
	{
	class TicketMutex;
	}

namespace internal
{
	// Forward declaration(s)
	class InternalConnection;
	class DispatchShared;


class GMSEC_API AutoDispatcher
{
public:
	AutoDispatcher(InternalConnection* connection, gmsec::api5::internal::TicketMutex& readMutex, gmsec::api5::internal::TicketMutex& writeMutex);


	~AutoDispatcher();


	bool CALL_TYPE start();


	bool CALL_TYPE stop(bool waitForCompletion);


	bool CALL_TYPE isRunning();


private:
	typedef gmsec::api5::util::StdUniquePtr<gmsec::api5::util::StdThread> DispatcherThread;
	typedef gmsec::api5::util::StdSharedPtr<DispatchShared>               DispatcherSharedPtr;


	InternalConnection*                  m_connection;
	gmsec::api5::internal::TicketMutex&  m_readMutex;
	gmsec::api5::internal::TicketMutex&  m_writeMutex;
	gmsec::api5::util::AtomicBoolean     m_dispatcherRunning;
	DispatcherThread                     m_dispatcher;
	DispatcherSharedPtr                  m_dispatchShared;
};

}  // end namespace internal
}  // end namespace api5
}  // end namespace gmsec

#endif
