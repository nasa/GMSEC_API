/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef AMQP_PROTON_RECEIVER_H
#define AMQP_PROTON_RECEIVER_H

#include "AMQPConnection.h"
#include "AMQPSubscription.h"
#include "SafeQueue.h"

#include <gmsec5/internal/Value.h>

#include <gmsec5/Message.h>
#include <gmsec5/util/Condition.h>
#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/StdUniquePtr.h>

#include <proton/connection.hpp>
#include <proton/container.hpp>
#include <proton/delivery.hpp>
#include <proton/error_condition.hpp>
#include <proton/message.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/types.hpp>
#include <proton/work_queue.hpp>

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#ifndef OVERRIDE
	#define OVERRIDE
#endif

namespace gmsec_amqp
{

class ProtonReceiver : public proton::messaging_handler
{
public:
	static const size_t MAX_BUFFER = 100;


	ProtonReceiver(const AMQPSubscription& amqpSub);

	virtual ~ProtonReceiver();

	void on_message(proton::delivery& d, proton::message& msg) OVERRIDE;

	void on_receiver_open(proton::receiver& r) OVERRIDE;

	void receive_done() OVERRIDE;

	void on_error(const proton::error_condition& info) OVERRIDE;

	void stopReceiving(bool flag);

	bool stopReceiving() const;

	static void run(gmsec::api5::util::StdSharedPtr<ProtonReceiver> receiver);

private:
	void close();

	proton::message receive();

	void enqueueResult(const gmsec::api5::Status& result, gmsec::api5::Message* msg);

	gmsec::api5::Message* unpackMessage(const proton::message& msg);

	void parseProperties(gmsec::api5::Message& message, gmsec::api5::internal::ValueMap& meta, const proton::message::property_map& props);


	AMQPSubscription                                   m_amqpSub;

	gmsec::api5::util::StdUniquePtr<proton::container> m_container;
	std::thread                                        m_containerThread;

	gmsec::api5::util::AtomicBoolean                   m_stopReceiving;
	proton::receiver                                   m_receiver;

	std::mutex                                         m_lock;
	proton::work_queue*                                m_workQueue;
	std::queue<proton::message>                        m_buffer;
	std::condition_variable                            m_can_receive;
	bool                                               m_closed;
};

}

#endif
