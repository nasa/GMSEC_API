/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Disable the Visual Studio warning C4355
//   ('this' : used in base member initializer list)
//   Found in the Connection(Server s, const Options &o) constructor
#ifdef WIN32
#pragma warning(disable: 4355)
#endif

#include "ConnectionMgr.h"

#include "ConnMgrCallback.h"
#include "MBConnection.h"
#include "MBFastMessage.h"
#include "MBResourceData.h"
#include "MBService.h"
#include "MBWire.h"

#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/StdUniquePtr.h>
#include <gmsec4/util/TimeUtil.h>


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;

using namespace gmsec_messagebus;


static const int IS_DEBUG = 0;


ConnectionMgr::ConnectionMgr(TCPSocketServer* sock, MBService* mbService)
	:
	m_sock(sock),
	m_mbService(mbService),
	m_callback(new ConnMgrCallback(this)),
	m_subscriptions(),
	m_debug(1),
	m_sockLock(),
	m_isLoopingMsgs(true),
	m_msgQueue(),
	m_msgQueueMutex(),
	m_isRunning(false)
{
	// this needs to be set to one to avoid a race condition in
	// MBService::manageSassions() caused by clients connecting near to
	// the same time and deleting thread handles before the thread has
	// a chance to start
	m_isRunning.set(true);
}


ConnectionMgr::~ConnectionMgr()
{
	delete m_sock;
	delete m_callback;
}


void ConnectionMgr::run()
{
	Status status;

	char*           inBuff = NULL;
	int             buffLen = 0;
	char            code;
	char            acksub[] = { CMD_ACKSUB, 0 };
	bool            disconnect = false;
	MBResourceData* MBRsrcDataCollector;
	GMSEC_U32       aMessage = 1;

	m_sock->setDebug(IS_DEBUG);

	// Ensure the client does not continue until the server starts the
	// ConnectionMgr thread and the ConnectionWriter thread.
	if (!m_connWriterThread.get())
	{
		m_connWriterShared.reset(new ConnectionWriter(this));

		m_connWriterThread.reset(new StdThread(&runConnWriterThread, m_connWriterShared));

		m_connWriterThread->start();

		while (!m_connWriterShared->isRunning())
		{
			TimeUtil::millisleep(30);
		}
	}
	
	// now ack the connection to the client that this manager will handle.
	status = m_sock->write("GG", 3);

	// loop untill the disconnect
	// command is issued
	while (!disconnect)
	{
		// read a command from the client
		while (buffLen == 0)
		{
			status = m_sock->read(inBuff, buffLen);

			if (IS_DEBUG && !(buffLen == 1 && inBuff[0] == CMD_OK))
			{
				char code = inBuff ? inBuff[0] : 'Z';
				GMSEC_INFO << "ConnectionMgr:" << this << ": read command=" << code << " length=" << buffLen;
			}

			if (status.getCode() != NO_ERROR_CODE)
			{
				doDisconnect();
				disconnect = true;
				if (inBuff)
				{
					delete [] inBuff;
					inBuff = NULL;
				}
				break;
			}

			StdThread::yield();
		}

		// Get the pointer to the MBResourceData collector object.
		MBRsrcDataCollector = m_mbService->getMBResourceDataInstance();

		// Bump up the 'bytes received' and "msgs received' values for the
		// message just read in.
		//
		MBRsrcDataCollector->setBytesRcvd((GMSEC_U32) buffLen);
		MBRsrcDataCollector->setMsgsRcvd(aMessage);

		code = (buffLen > 0 && inBuff) ? inBuff[0] : '\0';

		if (disconnect)
		{
			continue;
		}

		// process the command from the client
		switch (code)
		{
		case CMD_OK:
			// okay message (do nothing)
			break;

		case CMD_EXIT:
			// hand exit to MBServer who decides whether to ignore it
			m_mbService->stop();
			break;

		case CMD_SUB:
			// subscribe to a new subject
			{
				std::string topic(inBuff + 1, buffLen - 1);
				doSubscribe(topic.c_str());
				m_sock->write(acksub, StringUtil::stringLength(acksub) + 1);
			}
			break;

		case CMD_DISC:
			// disconnect
			doDisconnect();
			disconnect = true;
			break;

		case CMD_PUB:
			// publish a new message
			{
				Message* msg = NULL;
				Config   msgConfig;

				MBWire::deserialize(inBuff, buffLen, msg, msgConfig);

				if (msg == NULL)
				{
					GMSEC_WARNING << "Failed to deserialize message";
				}
				else
				{
					if (StringUtil::stringEquals(msg->getSubject(), MB_RESOURCE_DATA_REQUEST_SUBJECT))
					{
						// We got a request for MBServer resource data,
						// so call doResourceMessage() to retrieve this data and send
						// message.
#if 0
						doResourceMessage(*msg);
#endif
					}
					else
					{
						StdUniquePtr<MBFastMessage> fastMsg(new MBFastMessage(msg, this));

						delete [] inBuff;
						inBuff = 0;

						doPublish(*fastMsg.get());
					}
				}
			}
			break;

		case CMD_USUB:
			// unsubscribe from a subject
			{
				std::string topic(inBuff + 1, buffLen - 1);
				doUnsubscribe(topic.c_str());
			}
			break;

		case CMD_NLOOP:
			m_isLoopingMsgs = false;
			break;

		default:
			// sommething else
			GMSEC_WARNING << "ConnectionMgr:" << this << ": unknown command asInt=" << int(code);
		}

		// clean the incomming buffer
		// and prepare for the next one
		buffLen = 0;
		if (inBuff)
		{
			delete [] inBuff;
			inBuff = NULL;
		}
	}

	// close all connections to the client and
	// free up any memory used for managing
	// the connection
	//
	m_sock->write(NULL, 0); // this line forces the blocking read on the client to exit
	m_sock->disconnect();

	if (m_connWriterThread.get())
	{
		m_connWriterShared->shutdown();
		m_connWriterShared.reset();
		m_connWriterThread->join();
		m_connWriterThread.reset();
	}

	delete m_sock;
	m_sock = NULL;

	m_isRunning.set(false);
}


void ConnectionMgr::shutdown()
{
}


void ConnectionMgr::sendMsgToClient(MBFastMessage* msg)
{
	AutoMutex hold(m_msgQueueMutex);

	m_msgQueue.push(msg);
}


void ConnectionMgr::doResourceMessage(const Message& request)
{
	try
	{
		std::string requestSubject = request.getSubject();
		std::string routingSubject = request.getStringValue(MB_MY_SUBJECT_FIELD_NAME);

		size_t      dirPos         = requestSubject.find("REQ");
		std::string replySubject   = requestSubject.replace(dirPos, 3, "RESP");

		// Get the pointer to the MBResourceData object.
		MBResourceData* MBRsrcDataCollector = m_mbService->getMBResourceDataInstance();

		// Get the resource data to be placed into the reply message that
		// gets sent back to System Agent.
		//
		GMSEC_U32 total_bytes_rcvd = MBRsrcDataCollector->getBytesRcvd();
		GMSEC_U32 total_bytes_sent = MBRsrcDataCollector->getBytesSent();
		GMSEC_U32 total_msgs_rcvd  = MBRsrcDataCollector->getMsgsRcvd();
		GMSEC_U32 total_msgs_sent  = MBRsrcDataCollector->getMsgsSent();

		Message* rsrcMsg = new Message(replySubject.c_str(), Message::REPLY);

		char currentTime[GMSEC_TIME_BUFSIZE];
		TimeUtil::formatTime(TimeUtil::getCurrentTime(), currentTime);

		rsrcMsg->addField("BYTES-RECEIVED", total_bytes_rcvd);
		rsrcMsg->addField("BYTES-SENT",     total_bytes_sent);
		rsrcMsg->addField("MSGS-RECEIVED",  total_msgs_rcvd);
		rsrcMsg->addField("MSGS-SENT",      total_msgs_sent);

		rsrcMsg->addField("COMPONENT", "MBServer");
		rsrcMsg->addField(MB_MY_SUBJECT_FIELD_NAME, routingSubject.c_str());
		rsrcMsg->addField("PUBLISH-TIME", currentTime);
		rsrcMsg->addField(request.getStringField("REPLY-UNIQUE-ID"));
		rsrcMsg->addField("RESPONSE-STATUS", (short) 3);

		// Put the reply message in the message queue for sending.
		sendMsgToClient(new MBFastMessage(rsrcMsg, this));
	}
	catch (const Exception& e)
	{
		GMSEC_WARNING << "Unable to reply with resource message; reason: " << e.what();
	}
}


void ConnectionMgr::doSubscribe(const char* subject)
{
	if (subject == NULL)
	{
		GMSEC_WARNING << "[Attempt to subscribe to a null subject.]";
		return;
	}

	GMSEC_DEBUG << "ConnectionMgr:" << this << ": subscribe " << subject;

	m_subscriptions.push_back(subject);

	// add the subscription to the dispatcher using
	// the callback for this manager
	AutoMutex hold(m_mbService->getCallbackLookupMutex());
	m_mbService->addCallback(subject, m_callback);
}


void ConnectionMgr::doUnsubscribe(const char* subject)
{
	if (subject == NULL)
	{
		GMSEC_WARNING << "[Attempt to unsubscribe from a null subject.]";
		return;
	}

	GMSEC_DEBUG << "ConnectionMgr:" << this << ": unsubscribing " << subject;

	// remove the subscription from the dispatcher
	{
		AutoMutex hold(m_mbService->getCallbackLookupMutex());
		m_mbService->removeCallback(subject, m_callback);
	}

	// remove the entry from the subscription list
	for (std::list<std::string>::iterator it = m_subscriptions.begin(); it != m_subscriptions.end();)
	{
		if (*it == subject)
		{
			it = m_subscriptions.erase(it);
		}
		else
		{
			++it;
		}
	}
}


void ConnectionMgr::doDisconnect()
{
	GMSEC_DEBUG << "ConnectionMgr:" << this << ": disconnecting";


	// enter a critical section and
	// remove all subscriptions from the
	// message dispatcher
	//
	{
		AutoMutex hold(m_mbService->getCallbackLookupMutex());
	
		for (std::list<std::string>::iterator it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it)
		{
			m_mbService->removeCallback((*it).c_str(), m_callback);
		}
		m_subscriptions.clear();
	}

	// cleanup the content of the message queue
	AutoMutex hold(m_msgQueueMutex);
	
	while (!m_msgQueue.empty())
	{
		MBFastMessage* msg = m_msgQueue.front();
		m_msgQueue.pop();

		if (msg != NULL)
		{
			MBFastMessage::destroy(msg);
		}
	}
}


void ConnectionMgr::doPublish(const MBFastMessage& msg)
{
	const char* subject = msg.getSubject();

	// get the list of callbacks that need to get the message
	AutoMutex hold(m_mbService->getCallbackLookupMutex());
	size_t callbackCount = m_mbService->getCallbackCount(subject);

	if (IS_DEBUG)
	{
		GMSEC_DEBUG << "ConnectionMgr:" << this << ": doPub:" << subject << " callbackCount=" << callbackCount;
	}

	// if there are no callbacks expecting this message
	// delete it and leave the function
	if (callbackCount == 0)
	{
		return;
	}

	m_mbService->dispatchMsg(*msg.get());
}


bool ConnectionMgr::sendNextMsgToClient()
{
	MBFastMessage*  sndMsg = NULL;
	bool            ignoreThisMsg = false;
	MBResourceData* MBRsrcDataCollector;
	GMSEC_U32       aMessage = 1;

	// get the first message if there is one
	{
		AutoMutex hold(m_msgQueueMutex);

		if (!m_msgQueue.empty())
		{
			sndMsg = m_msgQueue.front();
			m_msgQueue.pop();
		}
	}

	if (sndMsg == NULL)
	{
		return false;
	}

	// Check the loppingMsgs flag to
	// determine if this message should
	// be sent to this client or not.
	// When the looping flag is set to
	// 0 messages will not be sent back to the
	// client that originated them
	// even if they are subscribed to
	// the subject.
	//
	if (m_isLoopingMsgs)
	{
		ignoreThisMsg = false;
	}
	else if (sndMsg->getCreatorObject() != this)
	{
		ignoreThisMsg = false;
	}
	else
	{
		ignoreThisMsg = true;
	}

	if (ignoreThisMsg == false)
	{
		char*  buffer  = 0;
		size_t bufSize = 0;

		MBWire::serialize(*sndMsg->get(), buffer, bufSize);

		if (buffer && bufSize > 0)
		{
			m_sock->write(buffer, bufSize);

			MBRsrcDataCollector = m_mbService->getMBResourceDataInstance();
			MBRsrcDataCollector->setBytesSent((GMSEC_U32) bufSize);
			MBRsrcDataCollector->setMsgsSent(aMessage);

			delete [] buffer;
			buffer = 0;
		}
	}

	// check if the message is ready to be deleted
	// and if so delete it
	MBFastMessage::destroy(sndMsg);

	return true;
}


bool ConnectionMgr::isRunning()
{
	return m_isRunning.get();
}


void ConnectionMgr::runConnWriterThread(ConnWriterShared shared)
{
	shared->run();
}
