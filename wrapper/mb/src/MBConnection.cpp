/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "MBConnection.h"
#include "MBWire.h"

#include <gmsec5/ConfigOptions.h>

#include <gmsec5/internal/InternalConnection.h>
#include <gmsec5/internal/InternalMessageFactory.h>
#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/internal/SystemUtil.h>

#include <gmsec5/util/Log.h>
#include <gmsec5/util/TimeUtil.h>

#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;
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


static const char* DEFAULT_SERVER = "localhost";
static const int   DEFAULT_PORT   = 9000;


// this constructor is used to instantiate a new
// connection with the given configuration
MBConnection::MBConnection(const Config& config)
	:
	m_sock(NULL),
	m_port(DEFAULT_PORT),
	m_server(),
	m_mwinfo(),
	m_isLoopingMsgs(true),
	m_startServer(false),
	m_subscribeLatch(NULL),
	m_subscribeTimeout_ms(3000),
	m_readerThread(NULL),
	m_readerThreadShared(NULL),
	m_messageCounter(0)
{

#ifndef WIN32
	signal(SIGPIPE, SIG_IGN);
#endif

	// Deduce the server
	m_server = config.getValue(GMSEC_MIDDLEWARE_SERVER, "");

	if (m_server.empty())
	{
		// Server not provided; check legacy configuration option
		m_server = config.getValue(GMSEC_SERVER, DEFAULT_SERVER);
	}

	// Deduce the port (deprecated??)
	m_port = config.getIntegerValue("port", DEFAULT_PORT);

	// get the value for looping messages
	m_isLoopingMsgs = config.getBooleanValue(MB_LOOPBACK, true);
	m_startServer   = config.getBooleanValue(MB_AUTOSTART, false);

	if (config.getBooleanValue("compress", false))
	{
		MBWire::setCompress(true);
	}
}


MBConnection::~MBConnection()
{
}


const char* MBConnection::getLibraryRootName()
{
	return "gmsec_mb";
}


const char* MBConnection::getLibraryVersion()
{
	return "GMSEC MB Client v" GMSEC_VERSION_NUMBER " [" __DATE__ "]";
}


const char* MBConnection::getMWInfo()
{
	if (m_mwinfo.empty())
	{
		m_mwinfo = getLibraryRootName();
	}

	return m_mwinfo.c_str();
}


void MBConnection::mwConnect()
{
	// create socket and reader thread
	m_sock.reset(new TCPSocketClientArray(getExternal().getMessageFactory()));
	m_sock->setConfig(getExternal().getConfig());

	if (!m_readerThread.get())
	{
		m_readerThreadShared.reset(new MBReaderThread(m_sock.get()));

		m_readerThreadShared->setConnection(this);

		m_readerThread.reset(new StdThread(&runReaderThread, m_readerThreadShared));
	}

	// connect to the specifyied server or to
	// localhost if no server was specified
	int    maxRetries     = getExternal().getMaxConnectionRetries();
	int    retryInterval  = getExternal().getConnectionRetryInterval();
	int    triesRemaining = maxRetries;
	Status status;

	do
	{
		status = m_sock->connect(m_server, m_port);

		if (status.hasError() && (maxRetries == -1 || --triesRemaining > 0))
		{
			GMSEC_WARNING << "Connection failed; retrying... [tries remaining: " << triesRemaining << "]";
			TimeUtil::millisleep(retryInterval);
		}
	} while (status.hasError() && (maxRetries == -1 || triesRemaining >= 0));

	if (status.hasError())
	{
		// Return server not found error
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, CONNECT_SERVER_NOT_FOUND, "No Message Bus Server found");
	}

	m_sock->setDebug(false);

	// This while loop ensure the MBServer is up and running before continuing on
	//
	char* inBuff   = NULL;
	GMSEC_I32 inBufLen = 0;

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
	while (status.hasError() && (status.getCode() == TIMEOUT_OCCURRED || status.getCode() == NO_ERROR_CODE));

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

	if (status.hasError())
	{
		status.setCustomCode(CONNECT_MISSING_ACK);
		goto cleanup;
	}

	GMSEC_DEBUG << "Connected to server(s) " << m_sock->getCurrentServer().c_str();
	GMSEC_INFO << "Connection established";

	getExternal().setConnectionEndpoint(m_sock->getCurrentServer());

	// look at the loopingMsgs flag and
	// send tell the server if we don't want
	// to loop traffic
	if (m_isLoopingMsgs == false)
	{
		char cmd[1];
		cmd[0] = CMD_NLOOP;
		status = m_sock->write(cmd, 1);
		if (status.hasError())
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

cleanup:
	if (status.hasError())
	{
		throw GmsecException(status);
	}
}


void MBConnection::mwDisconnect()
{
	// send the disconnect command to the server
	char tBuff[2] = { CMD_DISC, 0 };

	Status status = m_sock->write(tBuff, sizeof(tBuff));

	if (status.hasError())
	{
		GMSEC_DEBUG << "Disconnect request to server failed.";
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
	if (m_sock.get())
	{
		m_sock.reset(NULL);
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

	GMSEC_I32 space = static_cast<GMSEC_I32>( StringUtil::stringLength(subject) )+ 2;
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

	if (status.hasError())
	{
		throw GmsecException(status);
	}
}


void MBConnection::mwUnsubscribe(const char* subject)
{
	GMSEC_I32 space = static_cast<GMSEC_I32>( StringUtil::stringLength(subject) ) + 2;
	char* outCmd = new char[space];

	// send the unsubscribe command and the subject
	// to unsubscribe from
	StringUtil::stringFormat(outCmd, space, "%c%s", CMD_USUB, subject);
	Status status = m_sock->write(outCmd, space);
	m_sock->removeConfig(outCmd);
	delete []outCmd;

	if (status.hasError())
	{
		throw GmsecException(status);
	}
}


void MBConnection::mwPublish(const Message& msg, const Config& config)
{
	// prepare the command string and send it
	char*  buffer     = 0;
	GMSEC_I32 bufferSize = 0;

	MBWire::serialize(msg, buffer, bufferSize);

	std::string uniqueID = generateUniqueId();

	Status status = m_sock->write(buffer, bufferSize, uniqueID.c_str());

	delete [] buffer;

	if (status.hasError())
	{
		throw GmsecException(status);
	}
}


void MBConnection::mwRequest(const Message& request, const std::string& id)
{
	mwPublish(request, getExternal().getConfig());
}


void MBConnection::mwReceive(Message*& msg, GMSEC_I32 timeout)
{
	double endTime = 0.0;

	msg = NULL;

	// if the connection is not there then report the error to the user
	if (m_readerThread.get() == NULL || m_readerThreadShared->getConnWasDropped())
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_CONNECTION, "Connect to server host was lost");
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
	msg = m_readerThreadShared->getQueuedMsg(timeout, endTime);

	if (msg == NULL)
	{
		if (m_readerThread.get() == NULL || m_readerThreadShared->getConnWasDropped())
		{
			getExternal().disconnect();

			throw GmsecException(CONNECTION_ERROR, CONNECTION_LOST, "Connection to server was lost");
		}
	}
	else
	{
		MessageFactoryBuddy::getInternal(getExternal().getMessageFactory()).addMessageTemplate(*msg);
		MessageFactoryBuddy::getInternal(getExternal().getMessageFactory()).identifyTrackingFields(*msg);
	}
}


std::string MBConnection::mwGetUniqueID()
{
	return generateUniqueId();
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


std::string MBConnection::generateUniqueId()
{
	GMSEC_U32 counter = ++m_messageCounter;

	std::ostringstream uniqueId;
	uniqueId << getExternal().getID() << "." << counter;
	return uniqueId.str();
}
