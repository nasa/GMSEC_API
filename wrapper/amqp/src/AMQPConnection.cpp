/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** @file AMQPConnection.cpp
 *  @brief This file provides functionality for implementing an AMQP wrapper.
 */

#include <AMQPConnection.h>

#include <gmsec_amqp.h>
#include <AMQPSubscription.h>
#include <ProtonReceiver.h>
#include <ProtonSender.h>

#include <gmsec5/internal/InternalConnection.h>
#include <gmsec5/internal/InternalMessageFactory.h>
#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/Rawbuf.h>
#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/internal/SystemUtil.h>

#include <gmsec5/util/Log.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <cstring>   // for strlen()


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


// Constants
#define AMQP_REPLY      "AMQP_REPLY_ADDR"
#define OVERRIDE


namespace gmsec_amqp
{

AMQPConnection::AMQPConnection(const Config& cfg)
	: m_uniquecounter(0),
	  m_requestCounter(0),
	  m_sslConfig("", "", "", "")
{
	std::string server;
	std::string filterToggle;
	
	// Deduce the server connection info
	const char* defaultServer = "127.0.0.1:5672";

	server = cfg.getValue(GMSEC_MIDDLEWARE_SERVER, "");

	if (server.empty())
	{
		server = cfg.getValue(GMSEC_SERVER, defaultServer);
	}

	// Get other configurable values (if available) from the config object.
	m_username         = cfg.getValue("mw-username", "");
	m_password         = cfg.getValue("mw-password", "");

	m_sslConfig.tStore = cfg.getValue("mw-truststore", "");
	m_sslConfig.pKey   = cfg.getValue("mw-privateKey", "");
	m_sslConfig.kPass  = cfg.getValue("mw-keyPassword", "");
	m_sslConfig.cert   = cfg.getValue("mw-certificate", "");

	filterToggle       = cfg.getValue("mw-filter-dups", ""); 

	std::vector<std::string> parts = StringUtil::split(server, ":");
	if (parts.size() == 1)
	{
		m_hostname = parts[0];
		m_port     = "5672";
	}
	else if (parts.size() == 2)
	{
		m_hostname = parts[0];
		m_port     = parts[1];
	}
	else
	{
		throw GmsecException(MIDDLEWARE_ERROR, INVALID_CONFIG_VALUE, "Illegal server configuration value");
	}

	m_useFilter = filterToggle.empty()
	              || StringUtil::stringEqualsIgnoreCase(filterToggle.c_str(), "true")
	              || StringUtil::stringEqualsIgnoreCase(filterToggle.c_str(), "yes");
}


AMQPConnection::~AMQPConnection()
{
	for (Subscriptions::iterator it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it)
	{
		SharedReceiver receiver = it->second;

		receiver->stopReceiving(true);

		receiver.reset();
	}
	m_subscriptions.clear();

	while (!m_recvQueue.empty())
	{
		MsgSubscriptionResult* result = m_recvQueue.front();
		m_recvQueue.pop();

		if (result->message)
		{
			delete result->message;
		}

		delete result;
	}
}


const char* AMQPConnection::getLibraryVersion()
{
	return AMQP_VERSION_STRING;
}


const char* AMQPConnection::getMWInfo()
{
	if (m_mwInfo.empty())
	{
		m_mwInfo = getLibraryRootName();
	}

	return m_mwInfo.c_str();
}


void AMQPConnection::mwConnect()
{
	// Initialize request specs
	//m_specs = getExternal().getRequestSpecs();

	getExternal().setConnectionEndpoint(m_hostname + ":" + m_port);

	Status status(NO_ERROR_CLASS, CONNECTION_CONNECTED, "Connection established");
	getExternal().dispatchEvent(gmsec::api5::Connection::Event::CONNECTION_SUCCESSFUL_EVENT, status);
}


void AMQPConnection::mwDisconnect()
{
	for (Subscriptions::iterator it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it)
	{
		SharedReceiver receiver = it->second;

		receiver->stopReceiving(true);

		receiver.reset();
	}
	m_subscriptions.clear();

	Status status(NO_ERROR_CLASS, CONNECTION_LOST, "Connection disconnected");
	getExternal().dispatchEvent(gmsec::api5::Connection::Event::CONNECTION_BROKEN_EVENT, status);
}


void AMQPConnection::mwSubscribe(const char* subject, const Config& config)
{
	if (m_subscriptions.count( fixSubject(subject) ) > 0)
	{
		return;
	}

	// Start the subscription thread
	const std::string fullAddress = assembleAddress( fixSubject(subject) );

	AMQPSubscription amqpSubscription(fullAddress, m_username, m_password, this);

	// Need to launch the ProtonReceiver as a separate thread so that is does not block us from dequeing messages.
	SharedReceiver receiver( new ProtonReceiver(amqpSubscription) );
	StdUniquePtr<StdThread> thread( new StdThread(&ProtonReceiver::run, receiver) );
	thread->start();

	// Allow subscription thread to initialize before proceeding
	AutoMutex lock( m_subscribeCondition.getMutex() );

	int reason = m_subscribeCondition.wait(5000);

	if (reason == AMQPConnection::PROTON_ERROR)
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, reason, "Unable to connect");
	}
	else if (reason == AMQPConnection::LISTENING)
	{
		m_subscriptions[ fixSubject(subject) ] = receiver;
		GMSEC_DEBUG << "[Subscribed successfully: " << subject << "]";
	}
	else
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, reason, "Unable to subscribe");
	}
}


void AMQPConnection::mwUnsubscribe(const char* subject)
{
	std::string fixedSub = fixSubject(subject);

	AutoMutex subMutex(m_subscribeCondition.getMutex());

	Subscriptions::iterator it = m_subscriptions.find(fixedSub);

	if (it == m_subscriptions.end())
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Not subscribed to subject");
	}

	SharedReceiver receiver = it->second;

	receiver->stopReceiving(true);

	receiver.reset();

	m_subscribeCondition.wait(100);

	m_subscriptions.erase(fixedSub);

	GMSEC_DEBUG << "[Unsubscribed successfully: " << subject << "]";
}


void AMQPConnection::mwPublish(const Message& msg, const Config& config)
{
	std::string url = assembleAddress( msg.getSubject() );

	//DMW TODO: Consider caching the ProtonSender object so it can be
	//          reused for messages containing the same subject.

	StdUniquePtr<ProtonSender> sender( new ProtonSender(url, m_username, m_password, this) );

	// Check to ensure we can send a message
	AutoMutex lock( m_publishCondition.getMutex() );

	int reason = m_publishCondition.wait(5000);

	if (reason == AMQPConnection::PROTON_ERROR)
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, reason, "Unable to connect");
	}

	sender->send(msg);
	sender->close();
	sender.reset();
}


void AMQPConnection::mwRequest(const Message& request, std::string& id)
{
	{
		AutoTicket lock(m_mutex);
		id = generateUniqueId( ++m_requestCounter );
	}

	MessageBuddy::getInternal(request).addField(GMSEC_REPLY_UNIQUE_ID_FIELD, id.c_str());

	// Send request for reply (subscription thread was started on connection)
	mwPublish(request, getExternal().getConfig());
}


void AMQPConnection::mwReceive(Message*& msg, GMSEC_I32 timeout)
{
	double start_s;
	bool   done = false;
	bool   first = true;

	msg = NULL;

	// Start counter if there is a timeout
	if (timeout != GMSEC_WAIT_FOREVER)
	{
		start_s = TimeUtil::getCurrentTime_s();
	}

	// Until "done" condition is set:
	while (!done)
	{
		int reason = 0;

		// If the queue is empty, either return timeout or wait. Otherwise, set "reason" flag.
		if (m_recvQueue.empty())
		{
			if (timeout == GMSEC_WAIT_FOREVER)
			{
				AutoMutex lock(m_msgQueueCondition.getMutex());
				reason = m_msgQueueCondition.wait();
			}
			else if (timeout == 0)
			{
				done = true;
			}
			else
			{
				// 0 < timeout < ∞
				double remaining_ms = timeout - 1000 * (TimeUtil::getCurrentTime_s() - start_s);

				if (first || remaining_ms > 0)
				{
					int wait_ms = static_cast<int>(remaining_ms);
					if (wait_ms < 1)
					{
						wait_ms = 1;
					}
					AutoMutex lock(m_msgQueueCondition.getMutex());
					reason = m_msgQueueCondition.wait(wait_ms);
				}
				else
				{
					// timeout (do not treat as error)
					done = true;
					break;
				}
			}
		}
		else
		{
			reason = GOT_MESSAGE;
		}

		// If a message is available:
		if (reason == GOT_MESSAGE)
		{
			if (!m_recvQueue.empty())
			{
				// Read it off of the queue and check the result for errors
				MsgSubscriptionResult* result = m_recvQueue.front();

				msg = result->message;
				Status status = result->status;

				m_recvQueue.pop();
				delete result;

				if (status.hasError())
				{
					throw GmsecException(status);
				}

				if (msg != NULL)
				{
					const StringField* id = dynamic_cast<const StringField*>(msg->getField("UNIQUE-ID"));

					if (m_useFilter && id != NULL)
					{
						if (m_uniqueFilter.update(id->getValue()))
						{
							// Found a unique message
							done = true;
						}
					}
					else
					{
						// Either tracking is turned off on the publisher (No UNIQUE-ID field),
						// or filtering is turned off on the subscriber, so return the message.
						done = true;
					}

					// Check subject mapping and handle accordingly.
					ValueMap& meta = MessageBuddy::getInternal(*msg).getDetails();

					if (msg->getKind() == Message::Kind::REQUEST && msg->hasField(GMSEC_REPLY_UNIQUE_ID_FIELD))
					{
						meta.setString(GMSEC_REPLY_UNIQUE_ID_FIELD, msg->getStringValue(GMSEC_REPLY_UNIQUE_ID_FIELD));

						msg->clearField(GMSEC_REPLY_UNIQUE_ID_FIELD);
					}
				}
				else
				{
					done = true;
				}
			}
		}
		else if (reason == PROTON_ERROR)
		{
			throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, AMQPConnection::PROTON_ERROR, "Messenger Error");
		}
		else if (reason == COULDNT_GET_MESSAGE)
		{
			throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, AMQPConnection::COULDNT_GET_MESSAGE, "Could not retrieve message");
		}

		first = false;
	}
}


// Make GMSEC-compliant string into AMQP-compliant string
std::string AMQPConnection::fixSubject(const std::string& subject)
{
	const std::string gmsec_wcard1 = ">";
	const std::string gmsec_wcard2 = "+";

	const std::string rmq_wcard1 = "*.#";
	const std::string rmq_wcard2 = "#";

	std::string rmqSubject = subject;

	size_t i;

	// Replace > with appropriate mapping
	while (true)
	{
		i = rmqSubject.find(gmsec_wcard1);
		if (i != std::string::npos)
		{
			rmqSubject.replace(i, gmsec_wcard1.length(), rmq_wcard1);
		}
		else
		{
			break;
		}
	}

	// Replace + with appropriate mapping
	while (true)
	{
		i = rmqSubject.find(gmsec_wcard2);
		if (i != std::string::npos)
		{
			rmqSubject.replace(i, gmsec_wcard2.length(), rmq_wcard2);
		}
		else
		{
			break;
		}
	}

	return rmqSubject;
}


// Make AMQP-compliant string into GMSEC-compliant string
std::string AMQPConnection::unfixSubject(const std::string& subject)
{
	const std::string gmsec_wcard1 = ">";
	const std::string gmsec_wcard2 = "+";

	const std::string rmq_wcard1 = "*.#";
	const std::string rmq_wcard2 = "#";

	std::string gmsecSubject = subject;

	size_t i;

	// Replace appropriate mapping with >
	while (true)
	{
		i = gmsecSubject.find(rmq_wcard1);
		if (i != std::string::npos)
		{
			gmsecSubject.replace(i, rmq_wcard1.length(), gmsec_wcard1);
		}
		else
		{
			break;
		}
	}
	
	// Replace appropriate mapping with +
	while (true)
	{
		i = gmsecSubject.find(rmq_wcard2);
		if (i != std::string::npos)
		{
			gmsecSubject.replace(i, rmq_wcard2.length(), gmsec_wcard2);
		}
		else
		{
			break;
		}
	}

	return gmsecSubject;
}


std::string AMQPConnection::assembleAddress(const std::string& subject)
{
	return m_hostname + ":" + m_port + "//topic/" + subject;
}


std::string AMQPConnection::generateUniqueId(long id)
{
	std::ostringstream strm;
	strm << getExternal().getID() << "_" << SystemUtil::getProcessID() << "_" << ++m_uniquecounter << "_" << id;
	return strm.str();
}

} // end namespace gmsec_amqp
