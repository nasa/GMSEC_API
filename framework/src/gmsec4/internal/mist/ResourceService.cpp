/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file ResourceService.cpp
 *
 *  @brief This file contains a class for the ResourceService portion of MIST.
 *
**/

#include <gmsec4/internal/mist/ResourceService.h>

#include <gmsec4/internal/mist/InternalConnectionManager.h>

#include <gmsec4/Config.h>
#include <gmsec4/mist/ConnectionManager.h>
#include <gmsec4/Errors.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>
#include <gmsec4/Message.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/Atomics.h>
#include <gmsec4/util/Log.h>
#include <gmsec4/util/Mutex.h>
#include <gmsec4/util/TimeUtil.h>

#include <gmsec4/internal/SystemUtil.h>

#include <string>


using namespace gmsec::api::util;


struct ActionInfo
{
	ActionInfo(double interval)
		: last_s(TimeUtil::getCurrentTime_s(0)),
		  interval_s(interval)
	{
	}

	bool tryNow()
	{
		bool flag = false;
		double now_s = TimeUtil::getCurrentTime_s(0);
		if (now_s - last_s >= interval_s)
		{
			flag = true;
			last_s = now_s;
		}
		return flag;
	}

private:
	double last_s;
	double interval_s;
};


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

ResourceService::ResourceService(const Config& config, 
								 const gmsec::api::util::DataList<Field*>& standardFields,
								 std::string subject, 
								 size_t pubInterval, 
								 size_t sampleInterval, 
								 size_t averageInterval)
	: m_config(config),
	  m_connMgr(new ConnectionManager(config)),
	  m_subject(subject),
	  m_pubInterval(pubInterval),
	  m_sampleInterval(sampleInterval),
	  m_averageInterval(averageInterval),
	  m_startupLatch(1),
	  m_shutdownLatch(1)
{
	m_alive.set(false);
	m_connMgr->setStandardFields(standardFields);
}

ResourceService::~ResourceService()
{
	m_alive.set(false);

	teardownService();
}


void ResourceService::start(StdSharedPtr<ResourceService> shared)
{
	shared->run();
}


bool ResourceService::stop(unsigned int timeout_ms)
{
	if (!m_alive.get())
	{
		GMSEC_WARNING << "ResourceService is not, or is no longer, running";
		return false;
	}

	m_alive.set(false);

	return m_shutdownLatch.await(timeout_ms);
}


bool ResourceService::isRunning()
{
	return m_alive.get();
}


bool ResourceService::awaitStart(unsigned int timeout_ms)
{
	return m_startupLatch.await(timeout_ms);
}


void ResourceService::run()
{
	Status status = setupService();

	if (status.isError())
	{
		GMSEC_ERROR << "Unable to setup ResourceService service -- " << status.get();
	}
	else
	{
		m_alive.set(true);

		m_startupLatch.countDown();
		
		ActionInfo publish((double) m_pubInterval);
		ActionInfo sample((double) m_sampleInterval);

		InternalConnectionManager& intConnMgr = ConnectionManagerBuddy::getInternal(*m_connMgr);

		// Flag that will be used to indicate that we should publish
		// first resource message immediately upon starting the
		// service.
		bool firstTime = true;

		while (m_alive.get())
		{
			bool publishMsg = publish.tryNow();
			bool sampleMsg  = sample.tryNow();

			if (firstTime)
			{
				publishMsg = true;
				firstTime  = false;
			}

			if (publishMsg)
			{
				try
				{
					intConnMgr.publishResourceMessage(m_subject.c_str(), m_pubInterval, m_sampleInterval, m_averageInterval);
					GMSEC_VERBOSE << "ResourceService published C2CX-RSRC message.";
				}
				catch (const Exception& e)
				{
					GMSEC_ERROR << "ResourceService: Error publishing message -- " << e.what();
				}
			}
			else if (sampleMsg)
			{
				//The ResourceInfoGenerator keeps a running queue of samples from each message created. In the event
				//that the desired sample interval is smaller than the publish interval, this sample clause will execute
				//to create a message, thus triggering the sample logic. 
				//
				//TODO: For efficiency, break message population and stat generation into different steps. However,
				// this would require breaking the current encapsulation of ResourceInfoGenerator within ConnectionManager.
				//The savings from this change are minimal, and the work is deferred. 
				try
				{
					(void) intConnMgr.createResourceMessage(m_subject.c_str(), m_sampleInterval, m_averageInterval);
					GMSEC_VERBOSE << "ResourceService sampled C2CX-RSRC message.";
				}
				catch (const Exception& e)
				{
					GMSEC_ERROR << "ResourceService: Error sampling C2CX-RSRC message -- " << e.what();
				}
			}

			if (m_pubInterval == 0)
			{
				break;
			}

			TimeUtil::millisleep(200);
		}
	}

	status = teardownService();

	if (status.isError())
	{
		GMSEC_ERROR << "Unable to teardown ResourceService connection -- " << status.get();
	}

	m_shutdownLatch.countDown();
}


Status ResourceService::setupService()
{
	Status status;

	try
	{
		m_connMgr->initialize();
	}
	catch (Exception& e)
	{
		GMSEC_ERROR << "ResourceService is unable to create a connection: " << e.what();
		status.set(e.getErrorClass(), e.getErrorCode(), e.getErrorMessage());
	}

	return status;
}


Status ResourceService::teardownService()
{
	Status status;

	if (m_connMgr)
	{
		try
		{
			m_connMgr->cleanup();
		}
		catch (Exception& e)
		{
			status.set(e.getErrorClass(), e.getErrorCode(), e.getErrorMessage());
		}

		delete m_connMgr;
		m_connMgr = NULL;
	}

	return status;
}

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec
