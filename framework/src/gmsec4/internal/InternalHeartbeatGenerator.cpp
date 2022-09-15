/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalHeartbeatGenerator.cpp
 *
 *  @brief This file contains a class for starting a Heartbeat message generator.
 */

#include <gmsec4/internal/InternalHeartbeatGenerator.h>

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/field/Field.h>

#include <gmsec4/mist/Specification.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/TimeUtil.h>

#include <string>
#include <limits>

#ifdef WIN32
	#ifdef max
	#undef max
	#endif
#endif


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::util;


InternalHeartbeatGenerator::InternalHeartbeatGenerator(const Config& config, const char* hbMsgSubject, const GMSEC_U16 hbPubRate)
	: m_config(config),
	  m_connMgr(config),
	  m_hbMsg(0),
	  m_pubRate(static_cast<GMSEC_I64>(hbPubRate)),
	  m_counter(1),
	  m_publishAction(new ActionInfo( static_cast<long>(m_pubRate) )),
	  m_alive(false),
	  m_startupLatch(0),
	  m_shutdownLatch(0),
	  m_service(0)
{
	if (m_connMgr.getSpecification().getVersion() >= GMSEC_ISD_2019_00)
	{
		m_hbMsg.reset(new MistMessage(hbMsgSubject, "MSG.HB", m_connMgr.getSpecification()));
	}
	else
	{
		m_hbMsg.reset(new MistMessage(hbMsgSubject, "MSG.C2CX.HB", m_connMgr.getSpecification()));
	}

	m_hbMsg->setValue("PUB-RATE", m_pubRate);

	if (m_connMgr.getSpecification().getVersion() == GMSEC_ISD_2014_00)
	{
		std::ostringstream oss;
		oss << "GMSEC-HB-MSG-" << m_counter;
		m_hbMsg->addField("MSG-ID", oss.str().c_str()); //Pre-2016 ISDs require this field
	}

	if (validateMessage())
	{
		m_connMgr.getSpecification().validateMessage(*m_hbMsg.get());
	}

	setupService();
}


InternalHeartbeatGenerator::InternalHeartbeatGenerator(const Config& config, const char* hbMsgSubject, const GMSEC_U16 hbPubRate, const DataList<Field*>& fields)
	: m_config(config),
	  m_connMgr(config),
	  m_hbMsg(0),
	  m_pubRate(static_cast<GMSEC_I64>(hbPubRate)),
	  m_counter(1),
	  m_publishAction(new ActionInfo( static_cast<long>(m_pubRate) )),
	  m_alive(false),
	  m_startupLatch(0),
	  m_shutdownLatch(0),
	  m_service(0)
{
	if (m_connMgr.getSpecification().getVersion() >= GMSEC_ISD_2019_00)
	{
		m_hbMsg.reset(new MistMessage(hbMsgSubject, "MSG.HB", m_connMgr.getSpecification()));
	}
	else
	{
		m_hbMsg.reset(new MistMessage(hbMsgSubject, "MSG.C2CX.HB", m_connMgr.getSpecification()));
	}

	m_hbMsg->setValue("PUB-RATE", m_pubRate);

	for (DataList<Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
	{
		const Field* field = *it;

		if (field != NULL)
		{
			m_hbMsg->addField(*field);
		}
	}

	if (m_connMgr.getSpecification().getVersion() == GMSEC_ISD_2014_00)
	{
		std::ostringstream oss;
		oss << "GMSEC-HB-MSG-" << m_counter;
		m_hbMsg->addField("MSG-ID", oss.str().c_str()); //Pre-2016 ISDs require this field
	}

	if (validateMessage())
	{
		m_connMgr.getSpecification().validateMessage(*m_hbMsg.get());
	}

	setupService();
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
	catch (const Exception& e) {
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

	return m_shutdownLatch->await(static_cast<int>(m_pubRate == 0 ? 3000 : m_pubRate * 1000));
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

	bool fieldOverwritten = (m_hbMsg->getField(field.getName()) != NULL);

	if (StringUtil::stringEquals(field.getName(), "PUB-RATE") ||
	    StringUtil::stringEquals(field.getName(), "COUNTER"))
	{
		GMSEC_I64 value = field.getIntegerValue();

		if (value >= 0)
		{
			m_hbMsg->setValue(field.getName(), value);

			if (StringUtil::stringEquals(field.getName(), "PUB-RATE"))
			{
				m_pubRate = value;

				m_publishAction->setInterval( static_cast<long>(value) );
			}
			else
			{
				m_counter = (GMSEC_U16) value;
			}
		}
		else
		{
			std::ostringstream oss;
			oss << "Setting " << field.getName() << " to less than zero is not permitted";
			throw Exception(HEARTBEAT_GENERATOR_ERROR, VALUE_OUT_OF_RANGE, oss.str().c_str());
		}
	}
	else
	{
		try
		{
			if (validateMessage())
			{
				MistMessage tmp = *m_hbMsg.get();

				tmp.setValue(field.getName(), field.getStringValue());

				m_connMgr.getSpecification().validateMessage(tmp);
			}

			m_hbMsg->setValue(field.getName(), field.getStringValue());
		}
		catch (const Exception& e)
		{
			throw Exception(HEARTBEAT_GENERATOR_ERROR, INVALID_MSG, e.what());
		}
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

	bool fieldOverwritten = (m_hbMsg->getField(fieldName) != NULL);

	try
	{
		if (validateMessage())
		{
			MistMessage tmp = *m_hbMsg.get();

			tmp.setValue(fieldName, fieldValue);

			m_connMgr.getSpecification().validateMessage(tmp);
		}

		m_hbMsg->setValue(fieldName, fieldValue);
	}
	catch (const Exception& e)
	{
		throw Exception(HEARTBEAT_GENERATOR_ERROR, INVALID_MSG, e.what());
	}

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

	bool fieldOverwritten = (m_hbMsg->getField(fieldName) != NULL);

	try
	{
		if (validateMessage())
		{
			MistMessage tmp = *m_hbMsg.get();

			tmp.setValue(fieldName, fieldValue);

			m_connMgr.getSpecification().validateMessage(tmp);
		}

		m_hbMsg->setValue(fieldName, fieldValue);
	}
	catch (const Exception& e)
	{
		throw Exception(HEARTBEAT_GENERATOR_ERROR, INVALID_MSG, e.what());
	}

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

	bool fieldOverwritten = (m_hbMsg->getField(fieldName) != NULL);

	try
	{
		if (validateMessage())
		{
			MistMessage tmp = *m_hbMsg.get();

			tmp.setValue(fieldName, fieldValue);

			m_connMgr.getSpecification().validateMessage(tmp);
		}

		m_hbMsg->setValue(fieldName, fieldValue);
	}
	catch (const Exception& e)
	{
		throw Exception(HEARTBEAT_GENERATOR_ERROR, INVALID_MSG, e.what());
	}

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
				if (m_connMgr.getSpecification().getVersion() == GMSEC_ISD_2014_00)
				{
					m_counter %= GMSEC_U16(std::numeric_limits<GMSEC_I16>::max() + 1);
				}

				if (m_counter == 0) ++m_counter;

				m_hbMsg->setValue("COUNTER", (GMSEC_I64) m_counter);

				if (m_connMgr.getSpecification().getVersion() == GMSEC_ISD_2014_00)
				{
					std::ostringstream oss;
					oss << "GMSEC-HB-MSG-" << m_counter;
					m_hbMsg->addField("MSG-ID", oss.str().c_str()); //Pre-2016 ISDs require this field
				}

				++m_counter;

				m_connMgr.publish(*m_hbMsg.get());

				GMSEC_VERBOSE << "Heartbeat Generator published C2CX HB message";
			}
			catch (const Exception& e)
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
	m_connMgr.initialize();
}


void InternalHeartbeatGenerator::teardownService()
{
	m_connMgr.cleanup();
}


bool InternalHeartbeatGenerator::validateMessage()
{
	bool validate = StringUtil::stringIsTrue(m_config.getValue("gmsec-msg-content-validate", NULL));

	if (!validate)
	{
		validate = StringUtil::stringIsTrue(m_config.getValue("gmsec-msg-content-validate-all", NULL));
	}
	if (!validate)
	{
		validate = StringUtil::stringIsTrue(m_config.getValue("gmsec-msg-content-validate-send", NULL));
	}

	return validate;
}


InternalHeartbeatGenerator::ActionInfo::ActionInfo(long interval)
	: last_s(gmsec::api::util::TimeUtil::getCurrentTime_s(0)),
	  interval_s(interval),
	  actNow(false)
{
}


bool InternalHeartbeatGenerator::ActionInfo::tryNow()
{
	bool flag = false;

	if (interval_s.get() > 0)
	{
		double now_s = gmsec::api::util::TimeUtil::getCurrentTime_s(0);

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
