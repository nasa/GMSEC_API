/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include "MBReaderThread.h"

#include "MBWire.h"

#include <gmsec5/internal/InternalConnection.h>
#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/internal/SystemUtil.h>

#include <gmsec5/util/Log.h>

#ifdef WIN32
#include <windows.h>
#endif


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;

using namespace gmsec_messagebus;


MBReaderThread::MBReaderThread(TCPSocketClientArray* sock)
	:
	m_sock(sock),
	m_connection(0),
	m_isRunning(false),
	m_stopRunning(false),
	m_connWasDropped(false)
{
}


MBReaderThread::~MBReaderThread()
{
	m_connection = 0;

	while (!m_msgQueue.empty())
	{
		Message* msg = m_msgQueue.front();

		if (msg)
		{
			delete msg;
		}

		m_msgQueue.pop();
	}
}



// this function contains the main reading loop
// in the thread
void MBReaderThread::run()
{
	Status status;

	m_isRunning.set(true);

	GMSEC_DEBUG << "ReaderThread started.  Entering main loop.";

	char* buffer = 0;
	GMSEC_I32 bufferSize = 0;

	bool hadConnError = false;

	// run until told to stop by calling prepare disconnect
	while (!m_stopRunning.get())
	{
		if (buffer != 0)
		{
			delete [] buffer;
			buffer = 0;
		}

		status = m_sock->read(buffer, bufferSize);

		if (status.hasError())
		{
			if (!m_stopRunning.get())
			{
				if (!hadConnError)
				{
					GMSEC_WARNING << "Read error; has the MBServer broker died?";
				}

				hadConnError = true;

				TimeUtil::millisleep(1000);
			}
			continue;
		}

		if (hadConnError)
		{
			hadConnError = false;

			GMSEC_INFO << "Connection to server has resumed";
		}

		if (bufferSize == 2 && buffer[0] == CMD_ACKSUB)
		{
			m_connection->ackSubscription();
			continue;
		}

		if (bufferSize == 3 && buffer[0] == CMD_ACKCONN && buffer[1] == CMD_ACKCONN)
		{
			continue; //Connect acknowledgement received from backup server, don't deserialize
		}

		// parse the message
		MessageFactory& msgFactory = m_connection->getExternal().getMessageFactory();
		const Config&   msgConfig  = m_connection->getExternal().getMessageConfig();
		Message*        gmsecMsg   = MBWire::deserialize(buffer, bufferSize, msgFactory, msgConfig);

		if (gmsecMsg == NULL)
		{
			GMSEC_WARNING << "Error deserializing message.  Discarding.";
			continue;
		}

		if (StringUtil::stringEquals(gmsecMsg->getSubject(), "\"hb"))
		{
			delete gmsecMsg;

			continue;
		}

		// add the message to reply queue or the message queue accordingly
		if (gmsecMsg->getKind() == Message::Kind::REPLY)
		{
			Message* replyCopy = new Message(*gmsecMsg);

			m_connection->handleReply(replyCopy);
		}

		pushMsg(gmsecMsg);
	}

	m_isRunning.set(false);
}


void MBReaderThread::shutdown()
{
	// tell the main loop to stop
	m_stopRunning.set(true);

	while (m_isRunning.get())
	{
		TimeUtil::millisleep(10);
	}

	m_connection = 0;

	GMSEC_DEBUG << "ReaderThread stopped successfully.";
}


Message* MBReaderThread::getQueuedMsg(long timeout, double endTime)
{
	Status status;

	Message* retMsg = NULL;

	// lock the queue to protect from
	// errors since the will be called from a different
	// thread
	if (timeout == GMSEC_NO_WAIT)
    {
		// get the first message if there is one
        AutoMutex hold(m_queueMutex);

        if (!m_msgQueue.empty())
        {
			retMsg = m_msgQueue.front();
			m_msgQueue.pop();
        }
    }
    else
    {
		// wait at most the specified amount of time
        // and return a message if there is one
        while (retMsg == NULL && (timeout == GMSEC_WAIT_FOREVER || ((TimeUtil::getCurrentTime_s() <= endTime)))) 
        {
            bool sleep = false;
            {
                AutoMutex hold(m_queueMutex);

                if (!m_msgQueue.empty())
                {
                    // get the message
                    retMsg = m_msgQueue.front();
                    m_msgQueue.pop();
                }
                else
                {
                    // wait some time but free the
                    // queue while it's not being used
                    sleep = true;
                }
            }

            if (sleep)
			{
                TimeUtil::millisleep(1);
			}

            //  see if the server is still up and running
            if (m_connWasDropped.get())
			{
                break;
			}
        }
	}

	return retMsg;
}


bool MBReaderThread::getConnWasDropped()
{
	return m_connWasDropped.get();
}


bool MBReaderThread::getIsRunning()
{
	return m_isRunning.get();
}


void MBReaderThread::setConnection(MBConnection* conn)
{
	m_connection = conn;
}


void MBReaderThread::pushMsg(Message* msg)
{
	AutoMutex hold(m_queueMutex);

	m_msgQueue.push(msg);
}


void MBReaderThread::pushReply(Message* msg)
{
	if (m_connection)
	{
		m_connection->handleReply(msg);
	}
	else
	{
		GMSEC_INFO << "pushReply: connection gone";
	}
}
