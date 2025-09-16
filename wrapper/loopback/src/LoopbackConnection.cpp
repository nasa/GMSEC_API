/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file LoopbackConnection.cpp
 * @brief This file provides functionality for implementing a Loopback wrapper.
 */

#include <LoopbackConnection.h>

#include <gmsec_loopback.h>

#include <gmsec5/internal/InternalConnection.h>
#include <gmsec5/internal/InternalMessageFactory.h>
#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/internal/Subject.h>
#include <gmsec5/internal/SystemUtil.h>

#include <gmsec5/Status.h>

#include <gmsec5/util/Log.h>
#include <gmsec5/util/TimeUtil.h>

#include <sstream>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


namespace gmsec_loopback
{

const char* const MW_SIM_CONNECT_FAILURE      = "MW-SIM-CONNECT-FAILURE";
const char* const MW_SIM_SUBSCRIBE_FAILURE    = "MW-SIM-SUBSCRIBE-FAILURE";
const char* const MW_SIM_UNSUBSCRIBE_FAILURE  = "MW-SIM-UNSUBSCRIBE-FAILURE";
const char* const MW_SIM_PUBLISH_FAILURE      = "MW-SIM-PUBLISH-FAILURE";
const char* const MW_SIM_REQUEST_FAILURE      = "MW-SIM-REQUEST-FAILURE";
const char* const MW_SIM_REPLY_FAILURE        = "MW-SIM-REPLY-FAILURE";
const char* const MW_SIM_RECEIVE_FAILURE      = "MW-SIM-RECEIVE-FAILURE";
const char* const MW_SIM_PUBLISH_DELAY_MS     = "MW-SIM-PUBLISH-DELAY-MS";

const int DEFAULT_PUBLISH_DELAY_MS            = 1;


LoopbackConnection::Subscriptions     LoopbackConnection::s_subscriptions;
LoopbackConnection::SubscriptionMutex LoopbackConnection::s_mutex;
LoopbackConnection::Queue             LoopbackConnection::s_queue(Queue::MAX_QUEUE_SIZE);


LoopbackConnection::LoopbackConnection(const Config& config)
	: m_mwInfo(),
	  m_uniquecounter(0),
	  m_simConnectFailure(false),
	  m_simSubscribeFailure(false),
	  m_simUnsubscribeFailure(false),
	  m_simPublishFailure(false),
	  m_simRequestFailure(false),
	  m_simReplyFailure(false),
	  m_simReceiveFailure(false)
{
	m_simConnectFailure     = config.getBooleanValue(MW_SIM_CONNECT_FAILURE, false);
	m_simSubscribeFailure   = config.getBooleanValue(MW_SIM_SUBSCRIBE_FAILURE, false);
	m_simUnsubscribeFailure = config.getBooleanValue(MW_SIM_UNSUBSCRIBE_FAILURE, false);
	m_simPublishFailure     = config.getBooleanValue(MW_SIM_PUBLISH_FAILURE, false);
	m_simRequestFailure     = config.getBooleanValue(MW_SIM_REQUEST_FAILURE, false);
	m_simReplyFailure       = config.getBooleanValue(MW_SIM_REPLY_FAILURE, false);
	m_simReceiveFailure     = config.getBooleanValue(MW_SIM_RECEIVE_FAILURE, false);
	m_publishDelay          = config.getIntegerValue(MW_SIM_PUBLISH_DELAY_MS, DEFAULT_PUBLISH_DELAY_MS);

	if (m_publishDelay < 1)
	{
		GMSEC_WARNING << "Loopback publish delay cannot be less than 1; defaulting to " << DEFAULT_PUBLISH_DELAY_MS << " ms";
		m_publishDelay = DEFAULT_PUBLISH_DELAY_MS;
	}
}


LoopbackConnection::~LoopbackConnection()
{
}


const char* LoopbackConnection::getLibraryVersion()
{
	return LOOPBACK_VERSION_STRING;
}


const char* LoopbackConnection::getMWInfo()
{
	if (m_mwInfo.empty())
	{
		m_mwInfo = getLibraryRootName();
	}

	return m_mwInfo.c_str();
}


void LoopbackConnection::mwConnect()
{
	if (m_simConnectFailure)
	{
		throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Unable to connect");
	}

	Status status(NO_ERROR_CLASS, CONNECTION_CONNECTED, "Connection established");
	getExternal().dispatchEvent(gmsec::api5::Connection::Event::CONNECTION_SUCCESSFUL_EVENT, status);

	getExternal().setConnectionEndpoint("loopback");
}


void LoopbackConnection::mwDisconnect()
{
removeSub:
	for (Subscriptions::iterator it = s_subscriptions.begin(); it != s_subscriptions.end(); ++it)
	{
		if (it->first == getExternal().getName())
		{
			s_subscriptions.erase(it);
			goto removeSub;
		}
	}

	std::vector<MessageOwner*> notOurs;
	while (!s_queue.empty())
	{
		MessageOwner* msgOwner = s_queue.take();
		if (msgOwner->ownerID == getExternal().getName())
		{
			delete msgOwner->msg;
			delete msgOwner;
		}
		else
		{
			notOurs.push_back(msgOwner);
		}
	}
	for (size_t i = 0; i < notOurs.size(); ++i)
	{
		s_queue.put(notOurs[i]);
	}

	if (!m_simConnectFailure)
	{
		Status status(NO_ERROR_CLASS, CONNECTION_LOST, "Connection disconnected");
		getExternal().dispatchEvent(gmsec::api5::Connection::Event::CONNECTION_BROKEN_EVENT, status);
	}
}


void LoopbackConnection::mwSubscribe(const char* subject, const Config& config)
{
	AutoTicket lock(s_mutex);

	if (m_simSubscribeFailure)
	{
		throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Subscribe failed");
	}

	bool found = false;

	for (Subscriptions::iterator it = s_subscriptions.begin(); it != s_subscriptions.end() && !found; ++it)
	{
		found = (it->first == getExternal().getName() && it->second == subject);
	}

	if (!found)
	{
		s_subscriptions.insert(std::pair<std::string, std::string>(getExternal().getName(), subject));
	}
}


void LoopbackConnection::mwUnsubscribe(const char* subject)
{
	AutoTicket lock(s_mutex);

	if (m_simUnsubscribeFailure)
	{
		throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Unsubscribe failed");
	}

	for (Subscriptions::iterator it = s_subscriptions.begin(); it != s_subscriptions.end(); ++it)
	{
		if (it->first == getExternal().getName() && it->second == subject)
		{
			s_subscriptions.erase(it);
			break;
		}
	}
}


void LoopbackConnection::mwPublish(const Message& msg, const Config& config)
{
	if (m_simPublishFailure || config.getBooleanValue(MW_SIM_PUBLISH_FAILURE, false))
	{
		throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Publish failed");
	}
	if (m_simRequestFailure || config.getBooleanValue(MW_SIM_REQUEST_FAILURE, false))
	{
		throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Request failed");
	}
	if (m_simReplyFailure || config.getBooleanValue(MW_SIM_REPLY_FAILURE, false))
	{
		throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Reply failed");
	}

	if (s_queue.remainingCapacity() == 0)
	{
		throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Queue is full");
	}

	// Only enqueue a message if there is an existing subcriber
	const char* subject = msg.getSubject();

	if (haveSubscriber(subject))
	{
		// Clone message
		Message* clone = new Message(msg);

		// Reset the need to check for tracking fields
		MessageBuddy::getInternal(*clone).getDetails().setBoolean(GMSEC_CHECK_TRACKING_FIELDS, false);

		MessageOwner* msgOwner = new MessageOwner();
		msgOwner->ownerID = getExternal().getName();
		msgOwner->msg = clone;

		s_queue.put(msgOwner);
	}

	// Simulate a network delay
	TimeUtil::millisleep(static_cast<unsigned int>(m_publishDelay));
}


void LoopbackConnection::mwRequest(const Message& request, const std::string& id)
{
	mwPublish(request, getExternal().getConfig());
}


void LoopbackConnection::mwReceive(Message*& msg, GMSEC_I32 timeout)
{
	if (m_simReceiveFailure)
	{
		throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Receive failed");
	}

	MessageOwner* msgOwner = NULL;

	if (timeout == GMSEC_WAIT_FOREVER)
	{
		msgOwner = s_queue.take(); 
	}
	else
	{
		s_queue.poll(timeout, msgOwner);
	}

	if (msgOwner)
	{
		if (isSubscribed(getExternal().getName(), msgOwner->msg->getSubject()))
		{
			msg = msgOwner->msg;
		}
		else
		{
			delete msgOwner->msg;
		}
		delete msgOwner;
	}

	if (msg != NULL)
	{
		if (msg->getKind() == Message::Kind::REPLY)
		{
			getExternal().onReply(new Message(*msg));
		}
	}
}


std::string LoopbackConnection::mwGetUniqueID()
{
	std::ostringstream strm;
	strm << getExternal().getID() << "_" << SystemUtil::getProcessID() << "_" << ++m_uniquecounter;
	return strm.str();
}


bool LoopbackConnection::haveSubscriber(const std::string& subject)
{
	AutoTicket lock(s_mutex);

	bool matches = false;
	for (Subscriptions::const_iterator it = s_subscriptions.begin(); (it != s_subscriptions.end()) && !matches; ++it)
	{
		matches = Subject::doesSubjectMatchPattern(subject, it->second);
	}
	return matches;
}


bool LoopbackConnection::isSubscribed(const std::string& ownerID, const std::string& subject)
{
	AutoTicket lock(s_mutex);

	bool subscribed = false;
	for (Subscriptions::const_iterator it = s_subscriptions.begin(); (it != s_subscriptions.end()) && !subscribed; ++it)
	{
		subscribed = (it->first == getExternal().getName()) && Subject::doesSubjectMatchPattern(subject, it->second);
	}
	return subscribed;
}

} // end namespace gmsec_loopback
