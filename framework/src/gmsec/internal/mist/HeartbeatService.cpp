/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file HeartBeatService.cpp
 *
 *  @brief This file contains a class for the HeartBeatService portion of MIST.
 *
**/

#include <gmsec/internal/mist/HeartbeatService.h>

#include <gmsec/mist/MIST_errors.h>

#include <gmsec/Config.h>
#include <gmsec/Connection.h>
#include <gmsec/ConnectionFactory.h>
#include <gmsec/Message.h>

#include <gmsec/util/Atomics.h>
#include <gmsec/util/Log.h>
#include <gmsec/util/Mutex.h>
#include <gmsec/util/sysutil.h>
#include <gmsec/util/timeutil.h>

#include <string>


struct ActionInfo
{
	ActionInfo(double interval)
		: last_s(gmsec::util::getTime_s(0)),
		  interval_s(interval)
	{
	}

	bool tryNow()
	{
		bool flag = false;
		double now_s = gmsec::util::getTime_s(0);
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
namespace mist
{
namespace internal
{

HeartbeatService::HeartbeatService(Config& config, Message* msgTemplate)
	: m_config(config),
	  m_connection(0),
	  m_msg(0),
	  m_msgTemplate(msgTemplate),
	  m_pubInterval(30),
	  m_startupLatch(1),
	  m_shutdownLatch(1)
{
	Field pubRate;
	Status result = m_msgTemplate->GetField("PUB-RATE", pubRate);

	if (!result.IsError())
	{
		//TODO (MEH): In the long term we should determine the PUB-RATE from
		//the schema.  In the immediate term, and for the conceivable future,
		//it is going to stay as a GMSEC_I16.
		GMSEC_I16 temp;
		pubRate.GetValue(temp);

		m_pubInterval = temp;
	}

	m_alive.set(false);
}


HeartbeatService::~HeartbeatService()
{
	m_alive.set(false);

	if (m_connection)
	{
		teardownService();
	}
}


Status HeartbeatService::ChangeComponentStatus(const Field& componentStatus)
{
	gmsec::util::AutoMutex lock(m_msgMutex);
	Field& field = const_cast<Field&>(componentStatus);
	return m_msg->AddField(field);
}


Status HeartbeatService::ChangeComponentInfo(const Field& componentInfo)
{
	gmsec::util::AutoMutex lock(m_msgMutex);
	Field& field = const_cast<Field&>(componentInfo);
	return m_msg->AddField(field);
}


Status HeartbeatService::ChangeCPUUtil(const Field& cpuUtil)
{
	gmsec::util::AutoMutex lock(m_msgMutex);
	Field& field = const_cast<Field&>(cpuUtil);
	return m_msg->AddField(field);
}


Status HeartbeatService::ChangeCPUMemory(const Field& cpuMemory)
{	
	gmsec::util::AutoMutex lock(m_msgMutex);
	Field& field = const_cast<Field&>(cpuMemory);
	return m_msg->AddField(field);
}


void HeartbeatService::start(gmsec::util::gshared_ptr<HeartbeatService> shared)
{
	shared->run();
}


bool HeartbeatService::stop(unsigned int timeout_ms)
{
	if (!m_alive.get())
	{
		LOG_WARNING << "HeartbeatService::stop: not running";
		return false;
	}

	m_alive.set(false);

	return m_shutdownLatch.await(timeout_ms);
}


bool HeartbeatService::isRunning()
{
	return m_alive.get();
}


bool HeartbeatService::awaitStart(unsigned int timeout_ms)
{
	return m_startupLatch.await(timeout_ms);
}


void HeartbeatService::run()
{
	Status status = setupService();

	if (status.IsError())
	{
		LOG_ERROR << "Unable to setup HeartbeatService service -- " << status.Get();
	}
	else
	{
		m_alive.set(true);

		m_startupLatch.countDown();

		ActionInfo publish(m_pubInterval);

		// Flag that will be used to indicate that we should publish
		// first heartbeat message immediately upon starting the
		// service.
		bool firstTime = true;

		while (m_alive.get())
		{
			bool publishMsg = false;

			if (firstTime)
			{
				publishMsg = true;
				firstTime  = false;
			}
			else
			{
				publishMsg = publish.tryNow();
			}

			if (publishMsg)
			{
				gmsec::util::AutoMutex lock(m_msgMutex);

				status = m_connection->Publish(m_msg);

				if (status.IsError())
				{
					LOG_ERROR << "HeartbeatService: Error publishing message -- " << status.Get();
				}
				else
				{
					LOG_INFO << "HeartbeatService published C2CX-HB message.";
				}
			}
			gmsec::util::millisleep(200);
		}
	}

	status = teardownService();

	if (status.IsError())
	{
		LOG_ERROR << "Unable to teardown HeartbeatService connection -- " << status.Get();
	}

	m_shutdownLatch.countDown();
}


Status HeartbeatService::setupService()
{
	Status status = ConnectionFactory::Create(&m_config, m_connection);

	if (status.IsError())
	{
		LOG_ERROR << "HeartbeatService is unable to create a connection: " << status.Get();
	}
	else
	{
		status = m_connection->Connect();

		if (status.IsError())
		{
			LOG_ERROR << "HeartbeatService is unable to start: " << status.Get();
		}
		else
		{
			status = m_connection->CloneMessage(m_msgTemplate, m_msg);

			if (status.IsError())
			{
				LOG_ERROR << "HeartbeatService is unable to clone message template: " << status.Get();
			}
		}
	}

	return status;
}


Status HeartbeatService::teardownService()
{
	Status status;

	if (m_connection)
	{
		if (m_msg)
		{
			m_connection->DestroyMessage(m_msg);
			m_msg = 0;
		}

		(void) m_connection->Disconnect();
		status = ConnectionFactory::Destroy(m_connection);
		m_connection = 0;
	}

	return status;
}

}
}
}
