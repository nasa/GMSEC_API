/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file RequestThread.cpp
 *
 *  This file contains the Dispatcher class for Request messages.
 */

#include <gmsec5/internal/RequestThread.h>

#include <gmsec5/internal/ConnectionInterface.h>
#include <gmsec5/internal/InternalConnection.h>
#include <gmsec5/internal/InternalMessage.h>
#include <gmsec5/internal/InternalStatus.h>

#include <gmsec5/Connection.h>
#include <gmsec5/Message.h>

#include <gmsec5/util/Condition.h>
#include <gmsec5/util/Log.h>
#include <gmsec5/util/Mutex.h>
#include <gmsec5/util/TimeUtil.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


void gmsec::api5::internal::runRequestThread(StdSharedPtr<RequestShared> shared)
{
	shared->run();
}


RequestShared::RequestShared(InternalConnection* connection)
	: m_connection(connection)
{
}


RequestShared::~RequestShared()
{
}


void RequestShared::shutdown()
{
	AutoMutex hold(m_condition.getMutex());
	m_connection = 0;
	m_condition.broadcast(QUIT);
}


void RequestShared::run()
{
	AutoMutex hold(m_condition.getMutex());

	bool running = true;

	m_condition.broadcast(MANAGED);

	while (running)
	{
		int wait_ms = waitLimit_ms();

		GMSEC_DEBUG << "RequestShared::run waiting " << wait_ms;

		int reason = m_condition.wait(wait_ms);

		if (reason == Condition::TIMEOUT)
		{
			// do not care
			GMSEC_DEBUG << "RequestShared::run reason TIMEOUT";
		}
		else if (reason == QUIT)
		{
			GMSEC_DEBUG << "RequestShared::run reason QUIT";
			running = false;
		}
		else if (reason == RECEIVE_REPLY)
		{
			GMSEC_DEBUG << "RequestShared::run reason RECEIVE_REPLY";
			deliverReplies();
		}
		else if (reason == DELIVER_REPLY)
		{
			GMSEC_DEBUG << "RequestShared::run reason DELIVER_REPLY";
			// Request(request, timeout, reply) waits for this
		}
		else if (reason == REQUEST)
		{
			GMSEC_DEBUG << "RequestShared::run reason REQUEST";
			// a new request arrived, so need to reset
		}
		else
		{
			GMSEC_DEBUG << "RequestShared::run unexpected reason " << reason;
		}

		if (running && m_connection != NULL)
		{
			GMSEC_DEBUG << "RequestShared::run calling sendRequests()";
			sendRequests();
		}
	}
}


bool RequestShared::add(StdSharedPtr<PendingRequest>& pending)
{
	GMSEC_VERBOSE << "RequestShared::add " << pending.get();

	AutoMutex hold(m_condition.getMutex());

	m_pending.push_back(pending);
	m_condition.broadcast(REQUEST);

	return true;
}


static void incrementIfTrue(std::list<StdSharedPtr<PendingRequest> >::iterator& i, bool flag)
{
	if (flag)
	{
		++i;
	}
}


void RequestShared::sendRequests()
{
	double time_s = TimeUtil::getCurrentTime_s();

	bool inc;

	// Lock the mutex to prevent the shutdown of RequestShared thread and the addition
	// of new requests while we are here in the middle of processing existing pending
	// requests.
	AutoMutex hold(m_condition.getMutex());

	for (std::list<StdSharedPtr<PendingRequest> >::iterator i = m_pending.begin();
		i != m_pending.end();
		incrementIfTrue(i, inc))
	{
		StdSharedPtr<PendingRequest> tmp(*i);

		PendingRequest* pending = tmp.get();

		inc = true;

		if (pending->sentOnce)
		{
			if (!pending->isDone && time_s > pending->expireTime_s)
			{
				GMSEC_DEBUG << "sendRequests: " << pending->id.c_str() << " has timed out";

				pending->isDone = (pending->republish_ms == GMSEC_REQUEST_REPUBLISH_NEVER);

				if (pending->replyCallback)
				{
					// Dealing with an Asynchronous Request

					Status status(CONNECTION_ERROR, TIMEOUT_OCCURRED, "Request timed out");

					m_connection->replyEvent(pending->replyCallback, status, Connection::Event::REQUEST_TIMEOUT_EVENT);

					m_connection->dispatchEvent(Connection::Event::REQUEST_TIMEOUT_EVENT, status);

					if (!pending->isDone)
					{
						pending->expireTime_s = time_s + pending->republish_ms / 1000;
					}
				}
			}

			if (pending->isDone)
			{
				inc = false;
				GMSEC_DEBUG << "sendRequests: " << pending->id.c_str() << " isDone- erasing";
				i = m_pending.erase(i);
				continue;
			}

			// no need to (re)publish this request
			if (!pending->needToSend)
			{
				continue;
			}

			// not yet time to republish this request
			if (time_s < pending->publishTime_s)
			{
				continue;
			}
		}

		// need to send it
		std::string id;
		try
		{
			m_connection->issueRequestToMW(*(pending->request), id);
		}
		catch (const GmsecException& e)
		{
			GMSEC_WARNING << "RequestShared::sendRequests(): Unable to issue Request message: " << e.what();
			// Remove the invalid pending Request
			i = m_pending.erase(i);
			continue;
		}

		GMSEC_VERBOSE << "auxRequests: got ID " << id.c_str();

		if (!pending->sentOnce)
		{
			if (findPending(id))
			{
				GMSEC_WARNING << "RequestShared::sendRequests: Duplicate request ID : " << id.c_str();
			}

			pending->sentLatch.countDown();
			pending->sentOnce = true;
		}

		pending->id = id;

		// update what time we think it is
		time_s = TimeUtil::getCurrentTime_s();

		if (pending->republish_ms > 0)
		{
			pending->publishTime_s = time_s + pending->republish_ms / 1000.0;
		}
		else
		{
			pending->needToSend = false;
		}
	}
}


PendingRequest* RequestShared::findPending(const std::string& id)
{
	PendingRequest* found = NULL;

	for (std::list<StdSharedPtr<PendingRequest> >::iterator i = m_pending.begin(); i != m_pending.end(); ++i)
	{
		StdSharedPtr<PendingRequest> tmp(*i);

		PendingRequest* pending = tmp.get();

		if (pending->id == id)
		{
			found = pending;
			break;
		}
	}

	return found;
}


void RequestShared::onReply(Message* reply)
{
	AutoMutex hold(m_condition.getMutex());

	m_replies.push_back(reply);

	m_condition.broadcast(RECEIVE_REPLY);
}


void RequestShared::deliverReplies()
{
	for (std::list<Message*>::iterator i = m_replies.begin(); i != m_replies.end(); ++i)
	{
		Message* reply = *i;
		deliverReply(reply);
	}

	m_replies.clear();
}


static bool allowMoreResponses(PendingRequest* pending, Message* reply)
{
	if (pending->isDone)
	{
		return false;
	}
	if (!pending->multiResponse)
	{
		return false;
	}

	const char RESPONSE_STATUS[] = "RESPONSE-STATUS";

	const Field* field = reply->getField(RESPONSE_STATUS);

	if (!field)
	{
		return false;
	}

	GMSEC_I32 responseStatus = 0;

	Field::Type type = field->getType();

	if (type == Field::Type::I16)
	{
		responseStatus = static_cast<GMSEC_I32>( dynamic_cast<const I16Field*>(field)->getValue() );
	}
	else
	{
		GMSEC_VERBOSE << RESPONSE_STATUS << " field has type " << static_cast<int>(type);
		return false;
	}

	return (responseStatus == static_cast<GMSEC_I32>(Message::ResponseStatus::ACKNOWLEDGEMENT) || responseStatus == static_cast<GMSEC_I32>(Message::ResponseStatus::WORKING_KEEP_ALIVE));
}


void RequestShared::deliverReply(Message* reply)
{
	std::string replyID;

	if (reply->hasField(GMSEC_REPLY_UNIQUE_ID_FIELD))
	{
		replyID = reply->getStringValue(GMSEC_REPLY_UNIQUE_ID_FIELD);

		reply->clearField(GMSEC_REPLY_UNIQUE_ID_FIELD);
	}
	else
	{
		GMSEC_WARNING << "RequestShared::deliverReply: no " << GMSEC_REPLY_UNIQUE_ID_FIELD << " in reply message";
		delete reply;
		return;
	}

	PendingRequest* pending = findPending(replyID);

	if (!pending)
	{
		GMSEC_DEBUG << "RequestShared::deliverReply: nothing pending for " << replyID.c_str();
		delete reply;
		return;
	}

	bool allowMore = allowMoreResponses(pending, reply);

	if (pending->isDone)
	{
		GMSEC_DEBUG << "RequestShared::deliverReply: " << replyID.c_str() << " already isDone";
	}
	else if (pending->replyCallback)
	{
		m_connection->replyCallback(pending->replyCallback, *pending->request, *reply);

		delete reply;
	}
	else
	{
		pending->reply = reply;

		// Since this is called by the main loop, the mutex will still be locked.
		m_condition.broadcast(DELIVER_REPLY);
	}

	if (!allowMore)
	{
		pending->isDone = true;
	}
}


int RequestShared::waitLimit_ms()
{
	const int DEFAULT_DELAY_ms = 60 * 1000;   // 1 minute
	const int MINIMUM_DELAY_ms = 10;          // 0.01 seconds;

	int limit_ms = DEFAULT_DELAY_ms;

	double time_s = TimeUtil::getCurrentTime_s();

	for (std::list<StdSharedPtr<PendingRequest> >::iterator i = m_pending.begin(); i != m_pending.end(); ++i)
	{
		StdSharedPtr<PendingRequest> tmp(*i);

		PendingRequest* pending = tmp.get();

		if (pending->isDone)
		{
			continue;
		}

		if (!pending->neverExpire)
		{
			// update the limit with pending expiration
			double expire_s  = pending->expireTime_s - time_s;
			int    expire_ms = int(1000 * expire_s);

			if (expire_ms < limit_ms)
			{
				limit_ms = expire_ms;
			}
		}

		if (pending->needToSend)
		{
			// update the limit with pending publish time
			double publish_s  = pending->publishTime_s - time_s;
			int    publish_ms = int(1000 * publish_s);

			if (publish_ms < limit_ms)
			{
				limit_ms = publish_ms;
			}
		}
	}

	if (limit_ms < MINIMUM_DELAY_ms)
	{
		limit_ms = MINIMUM_DELAY_ms;
	}

	return limit_ms;
}



PendingRequest::PendingRequest(Message* request, GMSEC_I32 timeout_ms, GMSEC_I32 localRepublish_ms)
	: request(request),
	  reply(NULL),
	  needToSend(true),
	  neverExpire(false),
	  publishTime_s(0),
	  expireTime_s(0),
	  republish_ms(localRepublish_ms),
	  replyCallback(NULL),
	  sentOnce(false),
	  isDone(false),
	  multiResponse(false),
	  sentLatch(1)
{
	double time_s = TimeUtil::getCurrentTime_s();

	publishTime_s = time_s;

	if (timeout_ms > 0)
	{
		expireTime_s = time_s + timeout_ms / 1000.0;
	}
	else
	{
		neverExpire = true;
		expireTime_s = 1e100;
	}
}


PendingRequest::~PendingRequest()
{
	if (request)
	{
		delete request;
		request = 0;
	}
}
