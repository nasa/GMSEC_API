/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file HeartbeatService.h
 *
 *  @brief This file contains the HeartbeatService class, used in conjunction with
 *         ConnectionManager.h
 *
**/

#ifndef GMSEC_MIST_HEARTBEAT_SERVICE_H
#define GMSEC_MIST_HEARTBEAT_SERVICE_H

#include <gmsec4/Config.h>
#include <gmsec4/Message.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/Atomics.h>
#include <gmsec4/util/CountDownLatch.h>
#include <gmsec4/util/StdSharedPtr.h>
#include <gmsec4/util/Mutex.h>


namespace gmsec
{
namespace api
{
// Forward declaration(s)
class Connection;
class Field;

namespace mist
{
namespace internal
{


class GMSEC_API HeartbeatService
{
public:
	// Constructor for heartbeat service.
	HeartbeatService(const Config& config, const Message& msgTemplate);


	// Destructor turns off thread operations
	~HeartbeatService();


	// This function changes the status being published. Under the current spec,
	// values are a short between 0 and 4. An error status is returned if the supplied
	// value falls out of specification.
	Status changeComponentStatus(const Field& componentStatus);


	// This function changes the component info field, which is  a short type containing
	// component specific and user defined information about the component. An error status 
	// is returned if the supplied value falls out of specification.
	Status changeComponentInfo(const Field& componentStatus);


	// This function changes the current CPU utilization field, which is a float
	// containing the current percentage utilization. An error status 
	// is returned if the supplied value falls out of specification.
	Status changeCPUUtil(const Field& cpuUtil);


	// This function changes the current RAM utilization field, which is a float
	// containing the current percentage utilization. An error status 
	// is returned if the supplied value falls out of specification.
	Status changeCPUMemory(const Field& cpuMemory);


	// This function starts the HeartbeatService thread; the thread
	// will begin publishing C2CX messages at a rate as determined by
	// PUB-RATE in the C2CX message.
	static void start(gmsec::api::util::StdSharedPtr<HeartbeatService> shared);


	// This function can be used by main-thread to await a period of time
	// (in milliseconds) to get confirmation that the HeartbeatService
	// thread has indeed started.
	bool awaitStart(unsigned int timeout_ms);


	// This function stops the HeartbeatService thread; the thread
	// will cease publishing C2CX messages.
	bool stop(unsigned int timeout_ms);


	// This function returns the state of whether the HeartbeatService
	// is running or not.  A return value of true indicates that it is
	// running; a value of false indicates that it is not running.
	// It is recommended that the main-thread starting the HeartbeatService
	// thread use await() to allow for delays in start up before attempting
	// to rely on this method.
	bool isRunning();

private:
	// The main processing function for the HeartbeatService.
	void run();

	Status setupService();
	Status teardownService();

	// Defined, but not implemented
	HeartbeatService();
	HeartbeatService(const HeartbeatService&);

	Config              m_config;
	Connection*         m_connection;
	Message             m_msg;

	GMSEC_I32           m_pubInterval;
	GMSEC_U16			m_counter;

	gmsec::api::util::AtomicBoolean m_alive;
	gmsec::api::util::Mutex         m_msgMutex;

	gmsec::api::util::CountDownLatch m_startupLatch;
	gmsec::api::util::CountDownLatch m_shutdownLatch;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
