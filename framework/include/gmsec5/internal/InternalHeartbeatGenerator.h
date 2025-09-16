/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalHeartbeatGenerator.h
 *
 * @brief This file contains a class for starting a Heartbeat message generator.
 */

#ifndef GMSEC_API5_INTERNAL_HEARTBEAT_GENERATOR_H
#define GMSEC_API5_INTERNAL_HEARTBEAT_GENERATOR_H

#include <gmsec5/Message.h>

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

namespace internal
{

class InternalHeartbeatGenerator
{
public:
	InternalHeartbeatGenerator(const Config& config, const GMSEC_U16 hbPubRate);

	InternalHeartbeatGenerator(const Config& config, const GMSEC_U16 hbPubRate, const gmsec::api5::util::List<Field*>& fields);

	~InternalHeartbeatGenerator();

	bool start();

	bool stop();

	bool isRunning();

	void changePublishRate(GMSEC_U16 pubRate);

	bool setField(const Field& field);

	bool setField(const char* fieldName, GMSEC_I64 fieldValue);

	bool setField(const char* fieldName, GMSEC_F64 fieldValue);

	bool setField(const char* fieldName, const char* fieldValue);

private:
	// Defined, but not implemented
	InternalHeartbeatGenerator();
	InternalHeartbeatGenerator(const InternalHeartbeatGenerator&);

	// The main processing function for the HeartbeatService.
	static void startThread(InternalHeartbeatGenerator* instance);

	void run();

	void setupService();
	void teardownService();

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
	Message                                          m_hbMsg;
	GMSEC_U16                                        m_pubRate;
	GMSEC_U16                                        m_counter;

	gmsec::api5::util::StdUniquePtr<ActionInfo>      m_publishAction;

	gmsec::api5::util::AtomicBoolean                 m_alive;
	gmsec::api5::util::Mutex                         m_msgMutex;

	gmsec::api5::util::StdUniquePtr<CountdownLatch>  m_startupLatch;
	gmsec::api5::util::StdUniquePtr<CountdownLatch>  m_shutdownLatch;

	gmsec::api5::util::StdUniquePtr<Service>         m_service;
};

}  // namespace internal
}  // namespace api5
}  // namespace gmsec

#endif
