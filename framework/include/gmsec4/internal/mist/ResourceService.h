/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file ResourceService.h
 *
 *  @brief This file contains the ResourceService class, used in conjunction with
 *         ResourceService.h
 *
**/

#ifndef GMSEC_MIST_RESOURCE_SERVICE_H
#define GMSEC_MIST_RESOURCE_SERVICE_H

#include <gmsec4/Config.h>
#include <gmsec4/Message.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/Atomics.h>
#include <gmsec4/util/CountDownLatch.h>
#include <gmsec4/util/StdSharedPtr.h>
#include <gmsec4/util/DataList.h>

#include <string>

namespace gmsec
{
namespace api
{
namespace mist
{
class ConnectionManager;
namespace internal
{


class GMSEC_API ResourceService
{
public:
	// Constructor for heartbeat service.
	ResourceService(const Config& config, 
					const gmsec::api::util::DataList<Field*>& standardFields,
					std::string subject, 
					size_t pubInterval, 
					size_t sampleInterval, 
					size_t averageInterval);

	// Destructor turns off thread operations
	~ResourceService();


	// This function starts the ResourceService thread;
	static void start(gmsec::api::util::StdSharedPtr<ResourceService> shared);


	// This function can be used by main-thread to await a period of time
	// (in milliseconds) to get confirmation that the ResourceService
	// thread has indeed started.
	bool awaitStart(unsigned int timeout_ms);


	// This function stops the ResourceService thread; the thread
	// will cease publishing RSRC messages.
	bool stop(unsigned int timeout_ms);


	// This function returns the state of whether the HeartbeatService
	// is running or not.  A return value of true indicates that it is
	// running; a value of false indicates that it is not running.
	// It is recommended that the main-thread starting the ResourceService
	// thread use await() to allow for delays in start up before attempting
	// to rely on this method.
	bool isRunning();

private:
	// The main processing function for the HeartbeatService.
	void run();

	Status setupService();
	Status teardownService();

	// Defined, but not implemented
	ResourceService();
	ResourceService(const ResourceService&);
	ResourceService& operator=(const ResourceService&);

	Config              m_config;
	ConnectionManager*  m_connMgr;
	std::string         m_subject;
	size_t              m_pubInterval;
	size_t              m_sampleInterval;
	size_t              m_averageInterval;

	gmsec::api::util::AtomicBoolean  m_alive;
	gmsec::api::util::CountDownLatch m_startupLatch;
	gmsec::api::util::CountDownLatch m_shutdownLatch;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
