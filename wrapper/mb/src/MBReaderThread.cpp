/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include "MBReaderThread.h"

#include "MBWire.h"

#include <gmsec4/internal/InternalConnection.h>
#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/SystemUtil.h>

#include <gmsec4/util/Log.h>

#ifdef WIN32
#include <windows.h>
#endif


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;

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
	int   bufferSize = 0;

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

		if (status.isError())
		{
			if (!m_stopRunning.get())
			{
				if (!hadConnError)
				{
					GMSEC_WARNING << "Read error; has the server died?";
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
		Message* gmsecMsg = NULL;

		bool msgDecoded = MBWire::deserialize(buffer, bufferSize, gmsecMsg, m_connection->getExternal().getMessageConfig());

		if (!msgDecoded || gmsecMsg == NULL)
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
		if (gmsecMsg->getKind() == Message::REPLY)
		{
			//TODO: Remove functionality from MB(Server) that generates a Resource Message
#if 0
			bool processReply = (std::string(gmsecMsg->getSubject()).find(m_replySubject) != std::string::npos);

			if (!processReply)
			{
				// Check if we are dealing with an MB Resource Message
				try
				{
					const StringField& mySubjectField = gmsecMsg->getStringField(MB_MY_SUBJECT_FIELD_NAME);

					processReply = (m_replySubject == mySubjectField.getValue());

					gmsecMsg->clearField(MB_MY_SUBJECT_FIELD_NAME);
				}
				catch (const Exception& e)
				{
					processReply = false;
				}
			}
#else
			bool processReply = true;
#endif

			if (processReply)
			{
				pushReply(gmsecMsg);
			}

			if (m_replyExpose)
			{
				Message* replyCopy = new Message(*gmsecMsg);

				pushMsg(replyCopy);
			}
		}
		else
		{
			pushMsg(gmsecMsg);
		}
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


void MBReaderThread::setConnection(MBConnection* conn, const RequestSpecs& specs)
{
	m_connection   = conn;
	m_replyLegacy  = specs.legacy;
	m_replySubject = specs.replySubject;
	m_replyExpose  = specs.exposeReplies;
}


void MBReaderThread::pushMsg(Message* msg)
{
	AutoMutex hold(m_queueMutex);

	if (msg->getKind() == Message::REPLY)
	{
		if (m_connection)
		{
			m_connection->getExternal().updateReplySubject(msg);
		}
		else
		{
			GMSEC_INFO << "pushMsg: connection gone";
		}
	}

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
