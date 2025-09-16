/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalResourceGenerator.h
 *
 * @brief Utility for starting a service that periodically publishes
 * Resource Messages to the GMSEC Bus.
 */

#ifndef GMSEC_API5_INTERNAL_RESOURCE_GENERATOR_H
#define GMSEC_API5_INTERNAL_RESOURCE_GENERATOR_H

#include <gmsec5/Config.h>
#include <gmsec5/Connection.h>
#include <gmsec5/Message.h>

#include <gmsec5/util/Atomics.h>
#include <gmsec5/util/CountDownLatch.h>
#include <gmsec5/util/List.h>
#include <gmsec5/util/Mutex.h>
#include <gmsec5/util/StdThread.h>
#include <gmsec5/util/StdUniquePtr.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Field;
	class MessageFactory;

namespace internal
{

class InternalResourceGenerator
{
public:
	InternalResourceGenerator(const Config& config, GMSEC_U16 pubRate, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval);

	InternalResourceGenerator(const Config& config, GMSEC_U16 pubRate, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval,
	                          const gmsec::api5::util::List<Field*>& fields);

	~InternalResourceGenerator();

	bool start();

	bool stop();

	bool isRunning();

	bool setField(const Field& field);

	static Message createResourceMessage(MessageFactory& factory, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval);

private:
	// Defined, but not implemented
	InternalResourceGenerator();
	InternalResourceGenerator(const InternalResourceGenerator&);

	// The main processing function for the Resource Generator Service.
	static void startThread(InternalResourceGenerator* instance);

	void run();

	void setupService();
	void teardownService();

	static void collectResources(Message& msg, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval);

	bool validateMessage();

	struct ActionInfo
	{
		ActionInfo(long interval);

		bool tryNow();

		void setInterval(long interval);

	private:
		double                           last_s;
		gmsec::api5::util::AtomicLong    interval_s;
		gmsec::api5::util::AtomicBoolean actNow;
	};

	typedef gmsec::api5::util::StdThread             Service;
	typedef gmsec::api5::util::CountDownLatch        CountdownLatch;

	Config                                           m_config;
	Connection                                       m_conn;
	Message                                          m_rsrcMsg;
	GMSEC_U16                                        m_pubRate;
	GMSEC_U16                                        m_sampleInterval;
	GMSEC_U16                                        m_averageInterval;
	GMSEC_U16                                        m_counter;

	gmsec::api5::util::StdUniquePtr<ActionInfo>      m_publishAction;
	gmsec::api5::util::StdUniquePtr<ActionInfo>      m_sampleAction;

	gmsec::api5::util::AtomicBoolean                 m_alive;
	gmsec::api5::util::Mutex                         m_msgMutex;

	gmsec::api5::util::StdUniquePtr<CountdownLatch>  m_startupLatch;
	gmsec::api5::util::StdUniquePtr<CountdownLatch>  m_shutdownLatch;

	gmsec::api5::util::StdUniquePtr<Service>         m_service;
};

} // end namespace internal
} // end namespace api5
} // end namespace gmsec

#endif
