/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalResourceGenerator.cpp
 *
 * @brief Utility for starting a service that periodically publishes
 * Resource Messages to the GMSEC Bus.
 */


#include <gmsec5/internal/InternalResourceGenerator.h>

#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/ResourceInfoCollector.h>
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


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


InternalResourceGenerator::InternalResourceGenerator(const Config& config, GMSEC_U16 pubRate, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval)
	: m_config(config),
	  m_conn(config),
	  m_rsrcMsg(m_conn.getMessageFactory().createMessage("RSRC")),
	  m_pubRate(pubRate),
	  m_sampleInterval(sampleInterval),
	  m_averageInterval(averageInterval),
	  m_counter(1),
	  m_publishAction(new ActionInfo(static_cast<long>(m_pubRate))),
	  m_sampleAction(new ActionInfo(static_cast<long>(m_sampleInterval))),
	  m_alive(false),
	  m_startupLatch(0),
	  m_shutdownLatch(0),
	  m_service(0)
{
	if (sampleInterval < 1)
	{
		throw GmsecException(RESOURCE_GENERATOR_ERROR, RESOURCE_INFO_SAMPLING_ERROR,
		                     "A sample rate of zero was specified");
	}
	if (averageInterval < sampleInterval)
	{
		throw GmsecException(RESOURCE_GENERATOR_ERROR, RESOURCE_INFO_SAMPLING_ERROR,
		                     "A moving average interval less than sampling interval was specified");
	}

	m_rsrcMsg.setFieldValue("PUB-RATE", m_pubRate);

	m_rsrcMsg.addField("OPER-SYS", ResourceInfoCollector::getOSVersion().c_str());
}



InternalResourceGenerator::InternalResourceGenerator(const Config& config, GMSEC_U16 pubRate, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval,
                                                     const gmsec::api5::util::List<Field*>& fields)
	: m_config(config),
	  m_conn(config),
	  m_rsrcMsg(m_conn.getMessageFactory().createMessage("RSRC")),
	  m_pubRate(pubRate),
	  m_sampleInterval(sampleInterval),
	  m_averageInterval(averageInterval),
	  m_counter(1),
	  m_publishAction(new ActionInfo(static_cast<long>(m_pubRate))),
	  m_sampleAction(new ActionInfo(static_cast<long>(m_sampleInterval))),
	  m_alive(false),
	  m_startupLatch(0),
	  m_shutdownLatch(0),
	  m_service(0)
{
	if (sampleInterval < 1)
	{
		throw GmsecException(RESOURCE_GENERATOR_ERROR, RESOURCE_INFO_SAMPLING_ERROR,
		                     "A sample rate of zero was specified");
	}
	if (averageInterval < sampleInterval)
	{
		throw GmsecException(RESOURCE_GENERATOR_ERROR, RESOURCE_INFO_SAMPLING_ERROR,
		                     "A moving average interval less than sampling interval was specified");
	}

	m_rsrcMsg.setFieldValue("PUB-RATE", m_pubRate);

	m_rsrcMsg.addField("OPER-SYS", ResourceInfoCollector::getOSVersion().c_str());

	for (List<Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
	{
		const Field* field = *it;

		if (field != NULL)
		{
			m_rsrcMsg.addField(*field);
		}
	}
}


InternalResourceGenerator::~InternalResourceGenerator()
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

	ResourceInfoCollector::reset();
}


bool InternalResourceGenerator::start()
{
	if (m_alive.get())
	{
		GMSEC_WARNING << "The ResourceGenerator is already running!";
		return false;
	}

	if (validateMessage())
	{
		Status status = m_rsrcMsg.isCompliant();
		if (status.hasError())
		{
			throw GmsecException(status);
		}
	}

	setupService();

	m_startupLatch.reset(new CountDownLatch(1));
	m_shutdownLatch.reset(new CountDownLatch(1));
	m_service.reset(new StdThread(&InternalResourceGenerator::startThread, this));

	m_service->start();

	return m_startupLatch->await(3000);
}


bool InternalResourceGenerator::stop()
{
	if (!m_service.get())
	{
		GMSEC_WARNING << "ResourceGenerator was never started!";
		return false;
	}

	if (!m_alive.get())
	{
		GMSEC_WARNING << "ResourceGenerator is no longer running";
		return false;
	}

	m_alive.set(false);

	return m_shutdownLatch->await(static_cast<int>(m_pubRate <= 3 ? 3000 : m_pubRate * 1000));
}


bool InternalResourceGenerator::isRunning()
{
	return m_alive.get();
}


bool InternalResourceGenerator::setField(const Field& field)
{
	AutoMutex lock(m_msgMutex);

	bool fieldOverwritten = m_rsrcMsg.hasField(field.getName());

	if (StringUtil::stringEquals(field.getName(), "PUB-RATE") ||
	    StringUtil::stringEquals(field.getName(), "COUNTER"))
	{
		GMSEC_I64 value = field.getI64Value();

		if (value >= 0)
		{
			m_rsrcMsg.setFieldValue(field.getName(), value);

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
		m_rsrcMsg.addField(field);
	}

	return fieldOverwritten;
}


Message InternalResourceGenerator::createResourceMessage(MessageFactory& factory, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval)
{
	if (sampleInterval < 1)
	{
		throw GmsecException(RESOURCE_GENERATOR_ERROR, RESOURCE_INFO_SAMPLING_ERROR,
		                     "A sample rate of zero was specified");
	}
	if (averageInterval < sampleInterval)
	{
		throw GmsecException(RESOURCE_GENERATOR_ERROR, RESOURCE_INFO_SAMPLING_ERROR,
		                     "A moving average interval less than sampling interval was specified");
	}

	Message rsrcMsg = factory.createMessage("RSRC");

	collectResources(rsrcMsg, sampleInterval, averageInterval);

	rsrcMsg.addField("OPER-SYS", ResourceInfoCollector::getOSVersion().c_str());

	return rsrcMsg;
}


void InternalResourceGenerator::startThread(InternalResourceGenerator* instance)
{
	instance->run();
}


void InternalResourceGenerator::run()
{
	m_alive.set(true);

	m_startupLatch->countDown();

	// Flag that will be used to indicate that we should publish
	// first heartbeat message immediately upon starting the
	// service.
	bool firstTime = true;

	Message sampleMsg;

	while (m_alive.get())
	{
		bool publishMsg = m_publishAction->tryNow();
		bool sample     = m_sampleAction->tryNow();

		if (firstTime)
		{
			publishMsg = true;
			sample     = false;
			firstTime  = false;
		}

		if (publishMsg)
		{
			AutoMutex lock(m_msgMutex);

			// Collect updated resource info into the RSRC message
			collectResources(m_rsrcMsg, m_sampleInterval, m_averageInterval);

			if (m_counter == 0) ++m_counter;

			m_rsrcMsg.setFieldValue("COUNTER", static_cast<GMSEC_U32>(m_counter));

			++m_counter;

			try
			{
				m_conn.publish(m_rsrcMsg);
			}
			catch (const GmsecException& e)
			{
				GMSEC_ERROR << "ResourceGenerator: Error publishing message -- " << e.what();
			}
		}
		else if (sample)
		{
			// Collect updated resource info into a temporary message
			collectResources(sampleMsg, m_sampleInterval, m_averageInterval);
		}

		// Small delay so as to not pound the CPU.
		if (!publishMsg && !sample)
		{
			TimeUtil::millisleep((m_pubRate > 0 ? 1000 : 100));
		}
	}

	m_shutdownLatch->countDown();
}


void InternalResourceGenerator::setupService()
{
	m_conn.connect();
}


void InternalResourceGenerator::teardownService()
{
	m_conn.disconnect();
}


void InternalResourceGenerator::collectResources(Message& msg, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval)
{
	const unsigned int specVersion   = msg.getVersion();
	const size_t       movingSamples = averageInterval / sampleInterval;

	try {
		ResourceInfoCollector::addCPUStats(msg, specVersion, movingSamples);
	}
	catch (const GmsecException& e) {
		GMSEC_WARNING << "ResourceGenerator: " << e.getErrorMessage();
	}

	try {
		ResourceInfoCollector::addMainMemoryStats(msg, specVersion, movingSamples);
	}
	catch (const GmsecException& e) {
		GMSEC_WARNING << "ResourceGenerator: " << e.getErrorMessage();
	}

	try {
		ResourceInfoCollector::addDiskStats(msg, specVersion, movingSamples);
	}
	catch (const GmsecException& e) {
		GMSEC_WARNING << "ResourceGenerator: " << e.getErrorMessage();
	}

	try {
		ResourceInfoCollector::addNetworkStats(msg, specVersion, movingSamples);
	}
	catch (const GmsecException& e) {
		GMSEC_WARNING << "ResourceGenerator: " << e.getErrorMessage();
	}
}


bool InternalResourceGenerator::validateMessage()
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


InternalResourceGenerator::ActionInfo::ActionInfo(long interval)
	: last_s(TimeUtil::getCurrentTime_s(0)),
	  interval_s(interval),
	  actNow(false)
{
}

bool InternalResourceGenerator::ActionInfo::tryNow()
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


void InternalResourceGenerator::ActionInfo::setInterval(long interval)
{
	interval_s.set(interval);

	if (interval == 0)
	{
		actNow.set(true);
	}
}
