/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalHeartbeatGenerator.h
 *
 *  @brief This file contains a class for starting a Heartbeat message generator.
 */

#ifndef GMSEC_API_INTERNAL_HEARTBEAT_GENERATOR_H
#define GMSEC_API_INTERNAL_HEARTBEAT_GENERATOR_H

#include <gmsec4/mist/message/MistMessage.h>

#include <gmsec4/mist/ConnectionManager.h>

#include <gmsec4/Config.h>

#include <gmsec4/util/Atomics.h>
#include <gmsec4/util/CountDownLatch.h>
#include <gmsec4/util/DataList.h>
#include <gmsec4/util/Mutex.h>
#include <gmsec4/util/StdThread.h>
#include <gmsec4/util/StdUniquePtr.h>

#include <gmsec4_defs.h>


namespace gmsec
{
namespace api
{
// Forward declaration(s)
class Field;

namespace internal
{

class InternalHeartbeatGenerator
{
public:
	InternalHeartbeatGenerator(const Config& config, const char* hbMsgSubject, const GMSEC_U16 hbPubRate);

	InternalHeartbeatGenerator(const Config& config, const char* hbMsgSubject, const GMSEC_U16 hbPubRate, const gmsec::api::util::DataList<Field*>& fields);

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
		double                          last_s;
		gmsec::api::util::AtomicLong    interval_s;
		gmsec::api::util::AtomicBoolean actNow;
	};

	typedef gmsec::api::util::StdThread      Service;
	typedef gmsec::api::util::CountDownLatch CountdownLatch;
	typedef gmsec::api::util::StdUniquePtr<gmsec::api::mist::message::MistMessage> HBMessage;

	Config                                         m_config;
	gmsec::api::mist::ConnectionManager            m_connMgr;
	HBMessage                                      m_hbMsg;
	GMSEC_I64                                      m_pubRate;
	GMSEC_U16                                      m_counter;

	gmsec::api::util::StdUniquePtr<ActionInfo>     m_publishAction;

	gmsec::api::util::AtomicBoolean                m_alive;
	gmsec::api::util::Mutex                        m_msgMutex;

	gmsec::api::util::StdUniquePtr<CountdownLatch> m_startupLatch;
	gmsec::api::util::StdUniquePtr<CountdownLatch> m_shutdownLatch;

	gmsec::api::util::StdUniquePtr<Service>        m_service;
};

}  // namespace internal
}  // namespace api
}  // namespace gmsec

#endif
