/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include "ProtonSender.h"
#include "ProtonException.h"

#include <gmsec5/internal/InternalConnection.h>

#include <gmsec5/util/TimeUtil.h>
#include <gmsec5/util/Log.h>

#include <proton/connection_options.hpp>

#include <cstdlib>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;
using namespace gmsec_amqp;


ProtonSender::ProtonSender(const std::string& url,
                           const std::string& username,
                           const std::string& password,
                           AMQPConnection* conn)
	: m_url(url),
	  m_conn(conn),
	  m_container(new proton::container()),
	  m_workQueue(0),
	  m_queued(0),
	  m_credit(0)
{
	proton::connection_options co(*this);

	if (!username.empty()) co.user(username);
	if (!password.empty()) co.password(password);

	// TODO: Support SSL options

	// TODO: Support SASL options
	co.sasl_enabled(false);

	m_containerThread = std::thread([&]() { m_container->run(); });

	m_container->open_sender(m_url, co);
}


ProtonSender::~ProtonSender()
{
	m_containerThread.join();
}


void ProtonSender::send(const Message& gmsecMsg)
{
	proton::message msg = prepareMessage(gmsecMsg);

	{
		std::unique_lock<std::mutex> l(m_lock);
		// Don't queue up more messages than we have credit for
		while (!m_workQueue || m_queued >= m_credit) m_senderReady.wait(l);
		++m_queued;
	}
	m_workQueue->add([=]() { this->do_send(msg); }); // work queue is thread safe
}


void ProtonSender::close()
{
	work_queue()->add([=]() { m_sender.connection().close(); });
}


proton::work_queue* ProtonSender::work_queue()
{
	// Wait till work queue and sender are initialized.
	std::unique_lock<std::mutex> l(m_lock);
	while (!m_workQueue) m_senderReady.wait(l);
	return m_workQueue;
}


void ProtonSender::on_sender_open(proton::sender& s) OVERRIDE
{
	// Make sure sender and work queue are set atomically
	std::lock_guard<std::mutex> l(m_lock);
	m_sender = s;
	m_workQueue = &s.work_queue();

	AutoMutex lock( m_conn->m_publishCondition.getMutex() );
	m_conn->m_publishCondition.signal(AMQPConnection::OKAY_TO_SEND);
}


void ProtonSender::on_sendable(proton::sender& s) OVERRIDE
{
	std::lock_guard<std::mutex> l(m_lock);
	m_credit = s.credit();
	m_senderReady.notify_all(); // Notify senders we have credit
}


void ProtonSender::do_send(const proton::message& m)
{
	m_sender.send(m);
	std::lock_guard<std::mutex> l(m_lock);
	--m_queued;                     // work item was consumed from the work queue
	m_credit = m_sender.credit();   // update credit
	m_senderReady.notify_all();     // Notify senders we have space on queue
}


void ProtonSender::on_error(const proton::error_condition& e) OVERRIDE
{
	GMSEC_ERROR << e.what().c_str();

	AutoMutex lock( m_conn->m_publishCondition.getMutex() );
	m_conn->m_publishCondition.signal(AMQPConnection::PROTON_ERROR);
}


proton::message ProtonSender::prepareMessage(const Message& gmsecMsg)
{
	ValueMap meta;

	DataBuffer buffer;
	Status status = m_conn->getExternal().getPolicy().package(const_cast<Message&>(gmsecMsg), buffer, meta);

	proton::message msg;

	if (status.hasError())
	{
		GMSEC_WARNING << "AMQP: Unable to prepare Qpid Proton Message; reason: " << status.get();
		return msg;
	}

	msg.subject(gmsecMsg.getSubject());

	storeProperties(meta, msg.properties());

	std::string str(reinterpret_cast<const char*>(buffer.get()), buffer.size());

	msg.properties().put("DATA", str);

	return msg;
}


Status ProtonSender::storeProperty(const Value& value, const std::string& name, proton::message::property_map& props)
{
	Status status;

	if (value.isString())
	{
		std::string x;
		status = value.getString(x);
		if (!status.hasError())
		{
			props.put(name, x);
		}
	}
	else if (value.isInteger())
	{
		GMSEC_I32 x = 0;
		status = value.getI32(x);
		if (!status.hasError())
		{
			props.put(name, x);
		}
	}
	else if (value.isBoolean())
	{
		bool x = false;
		status = value.getBoolean(x);
		if (!status.hasError())
		{
			props.put(name, x);
		}
	}
	else if (value.isReal())
	{
		GMSEC_F64 x = 0;
		status = value.getF64(x);
		if (!status.hasError())
		{
			props.put(name, x);
		}
	}
	else
	{
		status.set(FIELD_ERROR, OTHER_ERROR_CODE, "Unable to convert type to AMQP property");
	}

	return status;
}


Status ProtonSender::storeProperties(ValueMap& meta, proton::message::property_map& props)
{
	Status status;
	
	ValueMap::Iterator i;
	meta.reset(i);
	
	while (!status.hasError())
	{
		const Value* value = i.next();

		if (!value) break;

		status = storeProperty(*value, i.getID(), props);
	}
	
	return status;
}
