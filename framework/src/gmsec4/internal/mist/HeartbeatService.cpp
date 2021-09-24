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

#include <gmsec4/internal/mist/HeartbeatService.h>

#include <gmsec4/mist/ConnectionManager.h>
#include <gmsec4/mist/Specification.h>

#include <gmsec4/Config.h>
#include <gmsec4/Errors.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>
#include <gmsec4/Message.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/Atomics.h>
#include <gmsec4/util/Log.h>
#include <gmsec4/util/Mutex.h>
#include <gmsec4/util/TimeUtil.h>

#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/SystemUtil.h>

#include <string>
#include <limits>

#ifdef WIN32
	#ifdef max
	#undef max
	#endif
#endif


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

HeartbeatService::HeartbeatService(const Config& config, const Message& msgTemplate)
	: m_config(config),
	  m_connMgr(0),
	  m_msg(msgTemplate),
	  m_pubInterval(30),
	  m_counter(1),
	  m_publishAction(0),
	  m_startupLatch(1),
	  m_shutdownLatch(1)
{
	const Field* pubRate = m_msg.getField("PUB-RATE");
	if (pubRate)
	{
		m_pubInterval = (GMSEC_U16) pubRate->getUnsignedIntegerValue();
	}

	const Field* counter = m_msg.getField("COUNTER");
	if (counter)
	{
		m_counter = (GMSEC_U16) counter->getUnsignedIntegerValue();
	}

	m_alive.set(false);
}


HeartbeatService::~HeartbeatService()
{
	m_alive.set(false);

	if (m_connMgr)
	{
		teardownService();
		delete m_connMgr;
	}
}


void HeartbeatService::setField(const Field& field)
{
	gmsec::api::util::AutoMutex lock(m_msgMutex);

	if (gmsec::api::util::StringUtil::stringEquals(field.getName(), "PUB-RATE"))
	{
		GMSEC_I64 rate = field.getIntegerValue();

		if (rate >= 0)
		{
			m_msg.addField(field);

			m_pubInterval = (GMSEC_U16) rate;

			m_publishAction->setInterval(m_pubInterval);
		}
		else
		{
			throw Exception(MIST_ERROR, VALUE_OUT_OF_RANGE, "Setting HeartbeatService PUB-RATE to less than zero is not permitted");
		}
	}
	else
	{
		m_msg.addField(field);
	}
}


void HeartbeatService::start(gmsec::api::util::StdSharedPtr<HeartbeatService> shared)
{
	shared->run();
}


bool HeartbeatService::stop(unsigned int timeout_ms)
{
	if (!m_alive.get())
	{
		GMSEC_WARNING << "HeartbeatService is not, or is no longer, running";
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

	if (status.isError())
	{
		GMSEC_ERROR << "Unable to setup HeartbeatService service -- " << status.get();
	}
	else
	{
		m_alive.set(true);

		m_startupLatch.countDown();

	  	m_publishAction.reset(new ActionInfo(m_pubInterval));

		// Flag that will be used to indicate that we should publish
		// first heartbeat message immediately upon starting the
		// service.
		bool firstTime = true;

		while (m_alive.get())
		{
			bool publishMsg = m_publishAction->tryNow();

			if (firstTime)
			{
				publishMsg = true;
				firstTime  = false;
			}

			if (publishMsg)
			{
				gmsec::api::util::AutoMutex lock(m_msgMutex);

				try
				{
					const Field* field = m_msg.getField("COUNTER");

					if (field != NULL)
					{
						if (field->getType() == Field::I16_TYPE)
						{
							m_counter %= GMSEC_U32(std::numeric_limits<GMSEC_I16>::max() + 1);

							if (m_counter == 0) ++m_counter;

							m_msg.addField("COUNTER", GMSEC_I16(m_counter));
						}
						else if (field->getType() == Field::U16_TYPE)
						{
							if (m_counter == 0) ++m_counter;

							m_msg.addField("COUNTER", m_counter);
						}
					}

					if (m_connMgr->getSpecification().getVersion() == GMSEC_ISD_2014_00)
					{
						std::ostringstream oss;
						oss << "GMSEC-C2CX-HB-" << m_counter;
						m_msg.addField("MSG-ID", oss.str().c_str()); //Pre-2016 ISDs require this field
					}

					m_connMgr->publish(m_msg);

					GMSEC_VERBOSE << "HeartbeatService published C2CX-HB message.";

					++m_counter;
				}
				catch (Exception& e)
				{
					GMSEC_ERROR << "HeartbeatService: Error publishing message -- " << e.what();
				}
			}

			// Delay a bit (less than a second) so that in case the PUB-RATE is
			// zero, we do not hammer the CPU.
			gmsec::api::util::TimeUtil::millisleep(500);
		}
	}

	status = teardownService();

	if (status.isError())
	{
		GMSEC_ERROR << "Unable to teardown HeartbeatService connection -- " << status.get();
	}

	m_shutdownLatch.countDown();
}


Status HeartbeatService::setupService()
{
	Status status;

	try
	{
		m_connMgr = new ConnectionManager(m_config);
		m_connMgr->initialize();
	}
	catch (const Exception& e)
	{
		GMSEC_ERROR << "HeartbeatService is unable to create a ConnectionManager: " << e.what();
		status.set(e.getErrorClass(), e.getErrorCode(), e.getErrorMessage());
	}

	return status;
}


Status HeartbeatService::teardownService()
{
	Status status;

	if (m_connMgr)
	{
		try
		{
			m_connMgr->cleanup();
		}
		catch (const Exception& e)
		{
			status.set(e.getErrorClass(), e.getErrorCode(), e.getErrorMessage());
		}
	}

	return status;
}


HeartbeatService::ActionInfo::ActionInfo(double interval)
	: last_s(gmsec::api::util::TimeUtil::getCurrentTime_s(0)),
	  interval_s(interval),
	  actNow(false)
{
}


bool HeartbeatService::ActionInfo::tryNow()
{
	bool flag = false;

	if (interval_s > 0)
	{
		double now_s = gmsec::api::util::TimeUtil::getCurrentTime_s(0);
		if (now_s - last_s >= interval_s)
		{
			flag = true;
			last_s = now_s;
		}
	}

	if (flag == false)
	{
		flag   = actNow;
		actNow = false;
	}

	return flag;
}


void HeartbeatService::ActionInfo::setInterval(double interval)
{
	interval_s = interval;

	if (interval == 0)
	{
		actNow = true;
	}
}


} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec
