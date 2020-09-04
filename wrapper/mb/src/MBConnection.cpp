/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "MBConnection.h"
#include "MBWire.h"

#include <gmsec4/internal/InternalConnection.h>
#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/SystemUtil.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/TimeUtil.h>

#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;
using namespace std;

using namespace gmsec_messagebus;


enum /* Connect CustomCodes */
{
	CONNECT_NO_ERROR,
	CONNECT_SERVER_NOT_FOUND,
	CONNECT_MISSING_ACK,
	CONNECT_NO_LOOP_FAILED,
	CONNECT_SUBSCRIBE_RESPONSE_FAILED,
	CONNECT_CUSTOM_END
};

// this constructor is used to instantiate a new
// connection with the given configuration
MBConnection::MBConnection(const Config& config)
	:
	m_sock(NULL),
	m_port(9000),
	m_server(),
	m_mwinfo(),
	m_reqSpecs(),
	m_isLoopingMsgs(true),
	m_startServer(false),
	m_subscribeLatch(NULL),
	m_subscribeTimeout_ms(3000),
	m_readerThread(NULL),
	m_readerThreadShared(NULL)
{

#ifndef WIN32
	signal(SIGPIPE, SIG_IGN);
#endif

	const char* server = config.getValue("server");
	const char* port   = config.getValue("port");

	if (server != NULL)
	{
		m_server = server;
	}
	if (port != NULL)
	{
		m_port = atoi(port);
	}

	// get the value for looping messages
	m_isLoopingMsgs = config.getBooleanValue("loopback", true);
	m_startServer   = config.getBooleanValue("autostart", false);

	if (config.getBooleanValue("compress", false))
	{
		MBWire::setCompress(true);
	}
}


MBConnection::~MBConnection()
{
	if (m_sock)
	{
		delete m_sock;
		m_sock = NULL;
	}
}


const char* MBConnection::getLibraryRootName()
{
	return "gmsec_mb";
}


const char* MBConnection::getLibraryVersion()
{
	return "GMSEC MB Client v"GMSEC_VERSION_NUMBER" [" __DATE__ "]";
}


const char* MBConnection::getMWInfo()
{
	if (m_mwinfo.empty())
	{
		std::ostringstream strm;
		strm << getLibraryRootName() << ": ";
		if (m_sock)
		{
			if (m_sock->getCurrentServer().length() > 0)
			{
		    	strm << m_sock->getCurrentServer();
			}
			else
			{
				std::string host;
				SystemUtil::getHostName(host);
				strm << host;
			}
		}
		else
		{
			strm << " <no client ID... no connection>";
		}

		m_mwinfo = strm.str();
	}

	return m_mwinfo.c_str();
}


void MBConnection::mwConnect()
{
	// Initialize our request specs
	m_reqSpecs = getExternal().getRequestSpecs();

	// create socket and reader thread
	m_sock = new TCPSocketClientArray();
	m_sock->setConfig(getExternal().getConfig());

	if (!m_readerThread.get())
	{
		m_readerThreadShared.reset(new MBReaderThread(m_sock));

		m_readerThreadShared->setConnection(this, m_reqSpecs);

		m_readerThread.reset(new StdThread(&runReaderThread, m_readerThreadShared));
	}

	// connect to the specifyied server or to
	// localhost if no server was specified
	Status status;
	if (m_server.length() > 0)
	{
		status = m_sock->connect(m_port, (char*)m_server.c_str());
	}
	else
	{
		status = m_sock->connect(m_port);
	}

	if (status.isError())
	{
		// Return server not found error
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, CONNECT_SERVER_NOT_FOUND, "No Message Bus Server found");
	}

	m_sock->setDebug(false);

	// This while loop ensure the MBServer is up and running before continuing on
	//
	char* inBuff   = NULL;
	int   inBufLen = 0;

	// do the first read to synchronize with the server
	do
	{
		if (inBuff)
		{
			delete [] inBuff;
			inBuff = NULL;
		}
		status = m_sock->read(inBuff, inBufLen);
	}
	while (status.isError() && (status.getCode() == TIMEOUT_OCCURRED || status.getCode() == NO_ERROR_CODE));

	if (inBufLen > 0)
	{
		GMSEC_DEBUG << "Ack from server msg=\"" << inBuff <<"\".";
	}
	else
	{
		GMSEC_WARNING << "Ack from server length=" << inBufLen << " status=" << status.get();
	}

	delete [] inBuff;
	inBuff = NULL;

	if (status.isError())
	{
		status.setCustomCode(CONNECT_MISSING_ACK);
		goto cleanup;
	}

	GMSEC_DEBUG << "Connected to server "
			    << ((m_server=="") ? "localhost" : m_server.c_str())
			    << " on port " << m_port << ".";

	GMSEC_INFO << "Connection established";

	// look at the loopingMsgs flag and
	// send tell the server if we don't want
	// to loop traffic
	if (m_isLoopingMsgs == 0)
	{
		char cmd[1];
		cmd[0] = CMD_NLOOP;
		status = m_sock->write(cmd, 1);
		if (status.isError())
		{
			status.setCustomCode(CONNECT_NO_LOOP_FAILED);
			goto cleanup;
		}
	}

	if (m_readerThread.get())
	{
		m_readerThread->start();

		while (!m_readerThreadShared->getIsRunning())
		{
			TimeUtil::millisleep(10);
		}
	}

	// Unless Request-Reply functionality is enabled, there is no need
	// to subscribe to the REPLY-TO subject associated with this
	// connection.
	if (m_reqSpecs.requestReplyEnabled)
	{
		mwSubscribe(m_reqSpecs.replySubject.c_str(), getExternal().getConfig());
	}

cleanup:
	if (status.isError())
	{
		throw Exception(status);
	}
}


void MBConnection::mwDisconnect()
{
	// send the disconnect command to the server
	char tBuff[2] = { CMD_DISC, 0 };

	Status status = m_sock->write(tBuff, sizeof(tBuff));

	if (status.isError())
	{
		GMSEC_WARNING << "Disconnect request to server failed.";
	}

	// tell the reader thread to disconnect
	// and disconnect the socket
	if (m_readerThread.get())
	{
		m_readerThreadShared->shutdown();

		m_readerThreadShared.reset();
		m_readerThread->join();
		m_readerThread.reset();
	}
	m_sock->disconnect();

	GMSEC_DEBUG << "Successful disconnect from server "
	            << ((m_server.empty()) ? "localhost" : m_server.c_str())
	            << " on port " << m_port << ".";

	// clean up
	if (m_sock)
	{
		delete m_sock;
		m_sock = NULL;
	}

	GMSEC_INFO << "Connection disconnected";
}


void MBConnection::mwSubscribe(const char* subject, const Config& config)
{
	// set up the subscription latch
	{
		AutoMutex hold(m_subscribeMutex);
		if (m_subscribeLatch)
		{
			GMSEC_WARNING << "MBConnection::Subscribe: subscribeLatch already exists";
			delete m_subscribeLatch;
		}
		m_subscribeLatch = new CountDownLatch(1);
	}

	int space = StringUtil::stringLength(subject) + 2;
	char* outCmd = new char[space];

	// send the subscribe command and the subject to
	// subscribe to
	StringUtil::stringFormat(outCmd, space, "%c%s", CMD_SUB, subject);
	Status status = m_sock->write(outCmd, space);
	m_sock->addConfig(outCmd);
	delete []outCmd;

	// If there is some weird error causing the latch not to be
	// hit, yet the server is ok, subscription won't take more than
	// a second. Anyway, we don't want to hang here forever.
	((CountDownLatch*) m_subscribeLatch)->await(m_subscribeTimeout_ms);
	{
		AutoMutex hold(m_subscribeMutex);
		delete m_subscribeLatch;
		m_subscribeLatch = 0;
	}

	if (status.isError())
	{
		throw Exception(status);
	}
}


void MBConnection::mwUnsubscribe(const char* subject)
{
	int space = StringUtil::stringLength(subject) + 2;
	char* outCmd = new char[space];

	// send the unsubscribe command and the subject
	// to unsubscribe from
	StringUtil::stringFormat(outCmd, space, "%c%s", CMD_USUB, subject);
	Status status = m_sock->write(outCmd, space);
	m_sock->removeConfig(outCmd);
	delete []outCmd;

	if (status.isError())
	{
		throw Exception(status);
	}
}


void MBConnection::mwPublish(const Message& msg, const Config& config)
{
	// prepare the command string and send it
	char*  buffer     = 0;
	size_t bufferSize = 0;

	MBWire::serialize(msg, buffer, bufferSize);

	std::ostringstream strm;
	strm << getExternal().getID() << "_" << getExternal().getMessageCounter();

	Status status = m_sock->write(buffer, bufferSize, strm.str().c_str());

	delete [] buffer;

	if (status.isError())
	{
		throw Exception(status);
	}
}


void MBConnection::mwRequest(const Message& request, std::string& id)
{
	long thisID = getExternal().getMessageCounter() + 1;

	std::ostringstream os;
	os << thisID;
	id = os.str();

	// Add an id for identifying the reply
	MessageBuddy::getInternal(request).addField(REPLY_UNIQUE_ID_FIELD, id.c_str());

	// Add a field with the subject to publish the reply to
	MessageBuddy::getInternal(request).addField(MB_MY_SUBJECT_FIELD_NAME, m_reqSpecs.replySubject.c_str());

	mwPublish(request, getExternal().getConfig());
}


void MBConnection::mwReply(const Message& request, const Message& reply)
{
	const StringField* uniqueID  = dynamic_cast<const StringField*>(request.getField(REPLY_UNIQUE_ID_FIELD));
	const StringField* mySubject = dynamic_cast<const StringField*>(request.getField(MB_MY_SUBJECT_FIELD_NAME));

	if (!uniqueID)
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Request does not contain unique ID field");
	}
	if (!mySubject)
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Request does not contain unique subject field");
	}

	// set the UNIQUE ID and the ROUTING SUBJECT within the reply message
	MessageBuddy::getInternal(reply).addField(*uniqueID);
	MessageBuddy::getInternal(reply).setSubject(mySubject->getValue());

	mwPublish(reply, getExternal().getConfig());
}


void MBConnection::mwReceive(Message*& msg, GMSEC_I32 timeout)
{
	double endTime = 0.0;

	msg = NULL;

	// if the connection is not there then report the error to the user
	if (m_readerThread.get() == NULL || m_readerThreadShared->getConnWasDropped())
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connect to server host was lost");
	}

	if ((timeout != GMSEC_WAIT_FOREVER) && (timeout != GMSEC_NO_WAIT))
	{
		// Get current time in seconds, then add the 'timeout' in
		// milliseconds and finally convert back to seconds for
		// desired end time for this transaction.
		//
		endTime = TimeUtil::getCurrentTime_s() + timeout / 1000.0;
	}


	// get the next message from the imcomming message queue
	// the queue gets populated by the mbReaderThread instance
	Message* rcvdMsg = m_readerThreadShared->getQueuedMsg(timeout, endTime);

	if (rcvdMsg == NULL)
	{
		if (m_readerThread.get() == NULL || m_readerThreadShared->getConnWasDropped())
		{
			mwDisconnect();

			throw Exception(CONNECTION_ERROR, INVALID_CONNECTION , "Connection to server host was lost");
		}
	}
	else
	{
		msg = rcvdMsg;

		if (msg->getKind() == Message::REPLY)
		{
			getExternal().updateReplySubject(msg);
		}
	}
}


void MBConnection::handleReply(Message* reply)
{
	getExternal().onReply(reply);
}


void MBConnection::ackSubscription()
{
	AutoMutex hold(m_subscribeMutex);
	if (m_subscribeLatch)
	{
		((CountDownLatch*) m_subscribeLatch)->countDown();
	}
	else
	{
		GMSEC_VERBOSE << "MBConnection::ackSubscription: already gave up";
	}
}


void MBConnection::runReaderThread(ReaderThreadShared shared)
{
	shared->run();
}
