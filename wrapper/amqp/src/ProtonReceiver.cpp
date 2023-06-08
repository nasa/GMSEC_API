/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include "ProtonReceiver.h"
#include "ProtonException.h"
#include "AMQPConnection.h"

#include <gmsec5/internal/InternalConnection.h>
#include <gmsec5/internal/InternalMessageFactory.h>

#include <gmsec5/Status.h>

#include <gmsec5/util/Log.h>

#include <proton/connection_options.hpp>
#include <proton/receiver_options.hpp>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;
using namespace gmsec_amqp;


ProtonReceiver::ProtonReceiver(const AMQPSubscription& amqpSub)
	: m_amqpSub(amqpSub),
	  m_container(new proton::container()),
	  m_stopReceiving(false),
	  m_workQueue(0),
	  m_closed(false)
{
	proton::connection_options co(*this);

	if (!m_amqpSub.username.empty()) co.user(m_amqpSub.username);
	if (!m_amqpSub.password.empty()) co.password(m_amqpSub.password);

	// TODO: Support SSL options

	// TODO: Support SASL options
	co.sasl_enabled(false);

	m_containerThread = std::thread([&]() { m_container->run(); });

	m_container->open_receiver(m_amqpSub.fullAddr, proton::receiver_options().credit_window(0), co);
}


ProtonReceiver::~ProtonReceiver()
{
	m_containerThread.join();
}


void ProtonReceiver::on_message(proton::delivery& d, proton::message& msg) OVERRIDE
{
	// Proton automatically reduces credit by 1 before calling on_message
	std::lock_guard<std::mutex> l(m_lock);
	m_buffer.push(msg);
	m_can_receive.notify_all();
}


void ProtonReceiver::on_receiver_open(proton::receiver& r) OVERRIDE
{
	m_receiver = r;

	{
		std::lock_guard<std::mutex> l(m_lock);
		m_workQueue = &m_receiver.work_queue();
		m_receiver.add_credit( ProtonReceiver::MAX_BUFFER ); // Buffer is empty, initial credit is the limit
	}

	{
		AutoMutex hold( m_amqpSub.amqpConn->m_subscribeCondition.getMutex() );
		m_amqpSub.amqpConn->m_subscribeCondition.signal(AMQPConnection::LISTENING);
	}
}


void ProtonReceiver::receive_done() OVERRIDE
{
	m_receiver.add_credit(1);
}


void ProtonReceiver::on_error(const proton::error_condition& info) OVERRIDE
{
	Status error(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, info.what().c_str());

	GMSEC_ERROR << error.get();

	if (!m_workQueue)
	{
		AutoMutex hold( m_amqpSub.amqpConn->m_subscribeCondition.getMutex() );
		m_amqpSub.amqpConn->m_subscribeCondition.signal(AMQPConnection::PROTON_ERROR);
	}
	else
	{
		AMQPConnection::MsgSubscriptionResult* result = new AMQPConnection::MsgSubscriptionResult(error, NULL);

		AutoMutex hold(m_amqpSub.amqpConn->m_msgQueueCondition.getMutex());
		m_amqpSub.amqpConn->m_recvQueue.push(result);
		m_amqpSub.amqpConn->m_msgQueueCondition.signal(AMQPConnection::PROTON_ERROR);
	}
}


void ProtonReceiver::stopReceiving(bool flag)
{
	m_stopReceiving.set(flag);

	if (m_stopReceiving.get())
	{
		close();
	}
}


bool ProtonReceiver::stopReceiving() const
{
	return const_cast<ProtonReceiver*>(this)->m_stopReceiving.get();
}


void ProtonReceiver::run(StdSharedPtr<ProtonReceiver> receiver)
{
	try
	{
		while (!receiver->stopReceiving())
		{
			auto msg = receiver->receive();

			Message* gmsecMsg = receiver->unpackMessage(msg);

			if (gmsecMsg)
			{
				receiver->enqueueResult(Status(), gmsecMsg);
			}
		}
	}
	catch (const ProtonException&)
	{
		// We're done
	}
}


void ProtonReceiver::close()
{
	std::lock_guard<std::mutex> l(m_lock);

	if (!m_closed)
	{
		m_closed = true;
		m_can_receive.notify_all();
		if (m_workQueue)
		{
			m_workQueue->add([this]() { this->m_receiver.connection().close(); });
		}
	}
}


proton::message ProtonReceiver::receive()
{
	std::unique_lock<std::mutex> l(m_lock);
	// Wait for buffered messages
	while (!m_closed && (!m_workQueue || m_buffer.empty()))
	{
		m_can_receive.wait(l);
	}

	if (m_closed) throw ProtonException("ProtonReceiver closed");

	proton::message m = std::move(m_buffer.front());
	m_buffer.pop();

	// Add a lambda to the work queue to call receive_done().
	// This will tell the handler to add more credit.
	m_workQueue->add([=]() { this->receive_done(); });

	return m;
}


void ProtonReceiver::enqueueResult(const Status& status, Message* msg)
{
	AMQPConnection::MsgSubscriptionResult* result = new AMQPConnection::MsgSubscriptionResult(status, msg);

	AutoMutex hold(m_amqpSub.amqpConn->m_msgQueueCondition.getMutex());
	m_amqpSub.amqpConn->m_recvQueue.push(result);
	m_amqpSub.amqpConn->m_msgQueueCondition.signal(AMQPConnection::GOT_MESSAGE);
}


Message* ProtonReceiver::unpackMessage(const proton::message& msg)
{
	// Get message subject, body and properties
	const std::string subject = msg.subject();

	// Put message contents into a GMSEC message
	proton::scalar sdata = msg.properties().get("DATA");
	std::string data = proton::coerce<std::string>(sdata);
	DataBuffer buffer(reinterpret_cast<const GMSEC_U8*>(data.c_str()), data.length(), false);
	ValueMap   meta;

	StdUniquePtr<Message> gmsecMsg;
	gmsecMsg.reset( MessageFactoryBuddy::getInternal(m_amqpSub.amqpConn->getExternal().getMessageFactory()).newMessage() );

	gmsecMsg->setSubject(subject.c_str());
	gmsecMsg->setConfig(m_amqpSub.amqpConn->getExternal().getMessageConfig());

	parseProperties(*gmsecMsg.get(), meta, msg.properties());

	Status result = m_amqpSub.amqpConn->getExternal().getPolicy().unpackage(*gmsecMsg.get(), buffer, meta);

	if (result.hasError())
	{
		GMSEC_WARNING << "AMQP: " << result.get();
		return NULL;
	}

	MessageFactoryBuddy::getInternal(m_amqpSub.amqpConn->getExternal().getMessageFactory()).addMessageTemplate(*gmsecMsg.get());

	// Check if REPLY kind
	if (gmsecMsg->getKind() == Message::Kind::REPLY)
	{
		Message* reply = gmsecMsg.release();

		gmsecMsg.reset( new Message(*reply) );

		m_amqpSub.amqpConn->getExternal().onReply(reply);
	}

	return gmsecMsg.release();
}


void ProtonReceiver::parseProperties(Message& message, ValueMap& meta, const proton::message::property_map& props)
{
	typedef std::map<std::string, proton::scalar> PropertyMap;

	PropertyMap properties;
	proton::get(props, properties);
	
	for (PropertyMap::const_iterator it = properties.begin(); it != properties.end(); ++it)
	{
		std::string name = it->first;

		if (name == "DATA")
		{
			continue;
		}

		StdUniquePtr<Field> field;

		if (!field.get())
		{
			try {
				std::string value = proton::get<std::string>(it->second);

				meta.setString(name.c_str(), value.c_str());

				field.reset(new StringField(name.c_str(), value.c_str(), true));
			}
			catch (...) {
			}
		}

		if (!field.get())
		{
			try {
				GMSEC_I32 value = proton::get<GMSEC_I32>(it->second);

				meta.setI32(name.c_str(), value);

				field.reset(new I32Field(name.c_str(), value, true));
			}
			catch (...) {
			}
		}

		if (!field.get())
		{
			try {
				bool value = proton::get<bool>(it->second);

				meta.setBoolean(name.c_str(), value);

				field.reset(new BooleanField(name.c_str(), value, true));
			}
			catch (...) {
			}
		}

		if (!field.get())
		{
			try {
				GMSEC_F64 value = proton::get<GMSEC_F64>(it->second);

				meta.setF64(name.c_str(), value);

				field.reset(new F64Field(name.c_str(), value, true));
			}
			catch (...) {
			}
		}

		if (!field.get())
		{
			GMSEC_WARNING << "AMQP parseProperties: unhandled property type for item: " << name.c_str();
		}
		
		if (field.get())
		{
			message.addField(*field.get());
		}
	}
}
