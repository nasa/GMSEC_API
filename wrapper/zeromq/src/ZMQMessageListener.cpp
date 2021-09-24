/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/*
 * @file ZMQConnection.h
 * Holds the declaration of ZeroMQ subclass of internal::BaseConnection.
 */

#include "ZMQMessageListener.h"

#include "ZMQConnection.h"

#include "ZMQEnums.h"

#include <gmsec4/util/StdThread.h>

#include <zmq.h>

#include <vector>
#include <cstdlib>
#include <cstring>


using namespace gmsec_zeromq;


ZMQMessageListener::ZMQMessageListener(ZMQConnection* zmqConnection, void** subSocket, bool reply)
	: m_connectionPtr(zmqConnection),
	  m_subSocketPtr(*subSocket),
	  m_inclusionFilter(new gmsec::api::internal::ExclusionFilter()),
	  m_isReply(reply),
	  m_doneWithLife(false),
	  m_deathLatch(new gmsec::api::util::CountDownLatch(1))
{
}


ZMQMessageListener::~ZMQMessageListener()
{
	m_deathLatch->await();

	if (m_deathLatch)
	{
		delete m_deathLatch;
		m_deathLatch = 0;
	}

	if (m_inclusionFilter)
	{
		delete m_inclusionFilter;
	}

	// Ensure that the ZMQConnection pointer no longer points to the connection
	// Note: The ZMQConnection will be cleaned up elsewhere in the code
	m_connectionPtr = 0;
}


void ZMQMessageListener::run()
{
	zmq_pollitem_t pollItem[1];
	pollItem[0].socket = m_subSocketPtr;
	pollItem[0].events = ZMQ_POLLIN;

	while (!m_doneWithLife)
	{
		// Avoid starving other threads
		gmsec::api::util::StdThread::yield();

		// Poll the socket to see if any messages have been received
		int result = zmq_poll(pollItem, 1, 1000);

		// Return to the top if this was just a timeout
		if (result == -1 && errno == ENOTSOCK)
		{
			continue;
		}
		
		// Retrieve the SUBJECT of the message from the ZeroMQ subscription socket
		zmq_msg_t message;
		zmq_msg_init(&message);
		int size = zmq_msg_recv(&message, m_subSocketPtr, ZMQ_DONTWAIT);
		
		if (size == -1)
		{
			zmq_msg_close(&message);
			continue;
		}

		// Extract the subject from the ZeroMQ message
		std::vector<char> header(size + 1);
		memcpy(header.data(), zmq_msg_data(&message), size);
		header[size] = 0;

		// Clean up the zmq_msg_t before moving on
		zmq_msg_close(&message);

		// Otherwise, process the message received
		recvMessage(header.data());
	}

	m_deathLatch->countDown();
}


void ZMQMessageListener::shutdown()
{
	m_doneWithLife = true;
}


bool ZMQMessageListener::moreToReceive()
{
	// Ensure that there is more to the message that is to be received
	int more;
	size_t more_size = sizeof(more);
	zmq_getsockopt(m_subSocketPtr, ZMQ_RCVMORE, &more, &more_size);

	return more;
}


void ZMQMessageListener::recvMessage(const char* subject)
{
	// If the message was sent without telling the socket to expect more, stop
	if (!moreToReceive())
	{
		return;
	}

	// Retrieve and extract the GMSEC message meta details
	zmq_msg_t metaMessage;
	zmq_msg_init(&metaMessage);
	int metaSize = zmq_msg_recv(&metaMessage, m_subSocketPtr, ZMQ_DONTWAIT);

	// If the message was sent without telling the socket to expect more, stop
	if (!moreToReceive())
	{
		return;
	}

	// Retrieve and extract the encoded GMSEC message
	zmq_msg_t gmsecMessage;
	zmq_msg_init(&gmsecMessage);
	int gmsecSize = zmq_msg_recv(&gmsecMessage, m_subSocketPtr, ZMQ_DONTWAIT);

	// Check to see if this message should be accepted
	// Note: ExclusionFilter logic is inverted to act as an Inclusion filter
	if (m_inclusionFilter->isSubjectExcluded(std::string(subject)))
	{
		// Decode and add the message to the queue
		m_connectionPtr->handleMessage(&gmsecMessage, gmsecSize, &metaMessage, metaSize, subject, m_isReply);
	}

	// Clean up
	zmq_msg_close(&metaMessage);
	zmq_msg_close(&gmsecMessage);
}


void ZMQMessageListener::subscribe(const std::string& subject)
{
	m_inclusionFilter->excludeSubject(subject.c_str());
}


void ZMQMessageListener::unsubscribe(const std::string& subject)
{
	m_inclusionFilter->removeExcludedSubject(subject.c_str());
}
