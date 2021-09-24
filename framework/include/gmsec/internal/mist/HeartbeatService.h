/*
 * Copyright 2007-2021 United States Government as represented by the
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

#ifndef gmsec_HeartbeatService_h
#define gmsec_HeartbeatService_h

#include <gmsec/Config.h>
#include <gmsec/Status.h>

#include <gmsec/util/Atomics.h>
#include <gmsec/util/CountDownLatch.h>
#include <gmsec/util/gshared_ptr.h>
#include <gmsec/util/Mutex.h>


namespace gmsec
{

class Config;
class Field;
class Message;

namespace mist
{

namespace internal
{
class BaseConnectionManager;


class GMSEC_API HeartbeatService
{
public:
	// Constructor for heartbeat service.
	HeartbeatService(Config& config, Message* msgTemplate);


	// Destructor turns off thread operations
	~HeartbeatService();


	// This function changes the status being published. Under the current spec,
	// values are a short between 0 and 4. An error status is returned if the supplied
	// value falls out of specification.
	Status ChangeComponentStatus(const Field& componentStatus);


	// This function changes the component info field, which is  a short type containing
	// component specific and user defined information about the component. An error status 
	// is returned if the supplied value falls out of specification.
	Status ChangeComponentInfo(const Field& componentStatus);


	// This function changes the current CPU utilization field, which is a float
	// containing the current percentage utilization. An error status 
	// is returned if the supplied value falls out of specification.
	Status ChangeCPUUtil(const Field& cpuUtil);


	// This function changes the current RAM utilization field, which is a float
	// containing the current percentage utilization. An error status 
	// is returned if the supplied value falls out of specification.
	Status ChangeCPUMemory(const Field& cpuMemory);


	// This function starts the HeartbeatService thread; the thread
	// will begin publishing C2CX messages at a rate as determined by
	// PUB-RATE in the C2CX message.
	static void start(gmsec::util::gshared_ptr<HeartbeatService> shared);


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

	HeartbeatService();
	HeartbeatService(const HeartbeatService&);

	Config              m_config;
	Connection*         m_connection;
	Message*            m_msg;
	Message*            m_msgTemplate;  // C2CX message template that we do NOT own!

	GMSEC_I32           m_pubInterval;

	util::AtomicBoolean m_alive;
	util::Mutex         m_msgMutex;

	util::CountDownLatch m_startupLatch;
	util::CountDownLatch m_shutdownLatch;
};

}
}
}

#endif
