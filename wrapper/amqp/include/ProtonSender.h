/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef AMQP_PROTON_SENDER_H
#define AMQP_PROTON_SENDER_H

#include "AMQPConnection.h"

#include <gmsec5/internal/Value.h>

#include <gmsec5/Message.h>
#include <gmsec5/Status.h>

#include <proton/connection.hpp>
#include <proton/container.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/types.hpp>
#include <proton/work_queue.hpp>

#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>

#ifndef OVERRIDE
	#define OVERRIDE
#endif

namespace gmsec_amqp
{

class AMQPConnection;

class ProtonSender : public proton::messaging_handler
{
public:
	ProtonSender(const std::string& url,
	             const std::string& username,
	             const std::string& password,
	             AMQPConnection* conn);

	virtual ~ProtonSender();

	void send(const gmsec::api5::Message& gmsecMsg);

	void close();

private:
	proton::work_queue* work_queue();

	void on_sender_open(proton::sender& s) OVERRIDE;

	void on_sendable(proton::sender& s) OVERRIDE;

	void do_send(const proton::message& m);

	void on_error(const proton::error_condition& e) OVERRIDE;

	proton::message prepareMessage(const gmsec::api5::Message& gmsecMsg);

	gmsec::api5::Status storeProperty(const gmsec::api5::internal::Value& value, const std::string& name, proton::message::property_map& props);

	gmsec::api5::Status storeProperties(gmsec::api5::internal::ValueMap& meta, proton::message::property_map& props);


	std::string                                        m_url;
	gmsec::api5::Message                               m_msg;
	AMQPConnection*                                    m_conn;

	gmsec::api5::util::StdUniquePtr<proton::container> m_container;
	std::thread                                        m_containerThread;

	proton::sender                                     m_sender;

	std::mutex                                         m_lock;
	proton::work_queue*                                m_workQueue;
	std::condition_variable                            m_senderReady;
	int                                                m_queued;
	int                                                m_credit;
};

}

#endif
