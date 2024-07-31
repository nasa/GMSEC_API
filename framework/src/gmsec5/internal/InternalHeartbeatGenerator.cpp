/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalHeartbeatGenerator.cpp
 *
 * @brief This file contains a class for starting a Heartbeat message generator.
 */

#include <gmsec5/internal/InternalHeartbeatGenerator.h>

#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5/ConfigOptions.h>
#include <gmsec5/MessageFactory.h>
#include <gmsec5/Specification.h>
#include <gmsec5/Status.h>

#include <gmsec5/field/Field.h>

#include <gmsec5/util/Log.h>
#include <gmsec5/util/TimeUtil.h>

#include <string>
#include <limits>

#ifdef WIN32
	#ifdef max
	#undef max
	#endif
#endif


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


InternalHeartbeatGenerator::InternalHeartbeatGenerator(const Config& config, const GMSEC_U16 hbPubRate)
	: m_config(config),
	  m_conn(config),
	  m_hbMsg(m_conn.getMessageFactory().createMessage("HB")),
	  m_pubRate(hbPubRate),
	  m_counter(1),
	  m_publishAction(new ActionInfo(static_cast<long>(m_pubRate))),
	  m_alive(false),
	  m_startupLatch(0),
	  m_shutdownLatch(0),
	  m_service(0)
{
	m_hbMsg.setFieldValue("PUB-RATE", m_pubRate);
}


InternalHeartbeatGenerator::InternalHeartbeatGenerator(const Config& config, const GMSEC_U16 hbPubRate, const List<Field*>& fields)
	: m_config(config),
	  m_conn(config),
	  m_hbMsg(m_conn.getMessageFactory().createMessage("HB")),
	  m_pubRate(hbPubRate),
	  m_counter(1),
	  m_publishAction(new ActionInfo(static_cast<long>(m_pubRate))),
	  m_alive(false),
	  m_startupLatch(0),
	  m_shutdownLatch(0),
	  m_service(0)
{
	m_hbMsg.addField("PUB-RATE", m_pubRate);

	for (List<Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
	{
		const Field* field = *it;

		if (field != NULL)
		{
			m_hbMsg.addField(*field);
		}
	}
}


InternalHeartbeatGenerator::~InternalHeartbeatGenerator()
{
	if (m_alive.get())
	{
		m_alive.set(false);

		(void) m_shutdownLatch->await(10000);
	}

	try {
		teardownService();
	}
	catch (const GmsecException& e) {
		GMSEC_WARNING << e.what();
	}
}


bool InternalHeartbeatGenerator::start()
{
	if (m_alive.get())
	{
		GMSEC_WARNING << "The Heartbeat Generator is already running!";
		return false;
	}

	if (validateMessage())
	{
		Status status = m_hbMsg.isCompliant();
		if (status.hasError())
		{
			throw GmsecException(status);
		}
	}

	setupService();

	m_startupLatch.reset(new CountDownLatch(1));
	m_shutdownLatch.reset(new CountDownLatch(1));
	m_service.reset(new StdThread(&InternalHeartbeatGenerator::startThread, this));

	m_service->start();

	return m_startupLatch->await(3000);
}


bool InternalHeartbeatGenerator::stop()
{
	if (!m_service.get())
	{
		GMSEC_WARNING << "Heartbeat Generator was never started!";
		return false;
	}

	if (!m_alive.get())
	{
		GMSEC_WARNING << "Heartbeat Generator is no longer running";
		return false;
	}

	m_alive.set(false);

	return m_shutdownLatch->await(static_cast<int>(m_pubRate <= 3 ? 3000 : m_pubRate * 1000));
}


bool InternalHeartbeatGenerator::isRunning()
{
	return m_alive.get();
}


void InternalHeartbeatGenerator::changePublishRate(GMSEC_U16 pubRate)
{
	(void) setField(U16Field("PUB-RATE", pubRate));
}


bool InternalHeartbeatGenerator::setField(const Field& field)
{
	AutoMutex lock(m_msgMutex);

	bool fieldOverwritten = m_hbMsg.hasField(field.getName());

	if (StringUtil::stringEquals(field.getName(), "PUB-RATE") ||
	    StringUtil::stringEquals(field.getName(), "COUNTER"))
	{
		GMSEC_I64 value = field.getI64Value();

		if (value >= 0)
		{
			m_hbMsg.setFieldValue(field.getName(), value);

			if (StringUtil::stringEquals(field.getName(), "PUB-RATE"))
			{
				m_pubRate = static_cast<GMSEC_U16>(value);

				m_publishAction->setInterval(static_cast<long>(m_pubRate));
			}
			else
			{
				m_counter = static_cast<GMSEC_U16>(value);
			}
		}
		else
		{
			std::ostringstream oss;
			oss << "Setting " << field.getName() << " to less than zero is not permitted";
			throw GmsecException(HEARTBEAT_GENERATOR_ERROR, VALUE_OUT_OF_RANGE, oss.str().c_str());
		}
	}
	else
	{
		m_hbMsg.addField(field);
	}

	return fieldOverwritten;
}


bool InternalHeartbeatGenerator::setField(const char* fieldName, GMSEC_I64 fieldValue)
{
	if (StringUtil::stringEquals(fieldName, "PUB-RATE") ||
	    StringUtil::stringEquals(fieldName, "COUNTER"))
	{
		return setField(I64Field(fieldName, fieldValue));
	}

	AutoMutex lock(m_msgMutex);

	bool fieldOverwritten = m_hbMsg.hasField(fieldName);

	m_hbMsg.setFieldValue(fieldName, fieldValue);

	return fieldOverwritten;
}


bool InternalHeartbeatGenerator::setField(const char* fieldName, GMSEC_F64 fieldValue)
{
	if (StringUtil::stringEquals(fieldName, "PUB-RATE") ||
	    StringUtil::stringEquals(fieldName, "COUNTER"))
	{
		return setField(F64Field(fieldName, fieldValue));
	}

	AutoMutex lock(m_msgMutex);

	bool fieldOverwritten = m_hbMsg.hasField(fieldName);

	m_hbMsg.setFieldValue(fieldName, fieldValue);

	return fieldOverwritten;
}


bool InternalHeartbeatGenerator::setField(const char* fieldName, const char* fieldValue)
{
	if (StringUtil::stringEquals(fieldName, "PUB-RATE") ||
	    StringUtil::stringEquals(fieldName, "COUNTER"))
	{
		return setField(StringField(fieldName, fieldValue));
	}

	AutoMutex lock(m_msgMutex);

	bool fieldOverwritten = m_hbMsg.hasField(fieldName);

	m_hbMsg.setFieldValue(fieldName, fieldValue);

	return fieldOverwritten;
}


void InternalHeartbeatGenerator::startThread(InternalHeartbeatGenerator* instance)
{
	instance->run();
}


void InternalHeartbeatGenerator::run()
{
	m_alive.set(true);

	m_startupLatch->countDown();

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
			AutoMutex lock(m_msgMutex);

			try
			{
				if (m_conn.getMessageFactory().getSpecification().getVersion() == GMSEC_MSG_SPEC_2014_00)
				{
					m_counter %= static_cast<GMSEC_U16>(std::numeric_limits<GMSEC_I16>::max() + 1);
				}

				if (m_counter == 0) ++m_counter;

				m_hbMsg.setFieldValue("COUNTER", static_cast<GMSEC_U32>(m_counter));

				if (m_conn.getMessageFactory().getSpecification().getVersion() == GMSEC_MSG_SPEC_2014_00)
				{
					std::ostringstream oss;
					oss << "GMSEC-HB-MSG-" << m_counter;
					m_hbMsg.addField("MSG-ID", oss.str().c_str()); //Pre-2016 ISDs require this field
				}

				++m_counter;

				m_conn.publish(m_hbMsg);

				GMSEC_VERBOSE << "Heartbeat Generator published HB message";
			}
			catch (const GmsecException& e)
			{
				GMSEC_ERROR << "Heartbeat Generator: Error publishing message -- " << e.what();
			}
		}

		// Small delay so as to not pound the CPU.
		if (!publishMsg)
		{
			TimeUtil::millisleep((m_pubRate > 0 ? 1000 : 100));
		}
	}

	m_shutdownLatch->countDown();
}


void InternalHeartbeatGenerator::setupService()
{
	m_conn.connect();
}


void InternalHeartbeatGenerator::teardownService()
{
	m_conn.disconnect();
}


bool InternalHeartbeatGenerator::validateMessage()
{
	bool validate = StringUtil::stringIsTrue(m_config.getValue(GMSEC_MSG_CONTENT_VALIDATE, NULL));

	if (!validate)
	{
		validate = StringUtil::stringIsTrue(m_config.getValue(GMSEC_MSG_CONTENT_VALIDATE_ALL, NULL));
	}
	if (!validate)
	{
		validate = StringUtil::stringIsTrue(m_config.getValue(GMSEC_MSG_CONTENT_VALIDATE_SEND, NULL));
	}

	return validate;
}


InternalHeartbeatGenerator::ActionInfo::ActionInfo(long interval)
	: last_s(TimeUtil::getCurrentTime_s(0)),
	  interval_s(interval),
	  actNow(false)
{
}


bool InternalHeartbeatGenerator::ActionInfo::tryNow()
{
	bool flag = false;

	if (interval_s.get() > 0)
	{
		double now_s = TimeUtil::getCurrentTime_s(0);

		if ((now_s - last_s) >= interval_s.doubleValue())
		{
			flag = true;
			last_s = now_s;
		}
	}

	if (flag == false)
	{
		flag = actNow.get();
		actNow.set(false);
	}

	return flag;
}


void InternalHeartbeatGenerator::ActionInfo::setInterval(long interval)
{
	interval_s.set(interval);

	if (interval == 0)
	{
		actNow.set(true);
	}
}
