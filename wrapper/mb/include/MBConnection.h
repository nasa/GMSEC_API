/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_MB_CONNECTION_H
#define GMSEC_MB_CONNECTION_H

#include "gmsec_mb.h"

#include "MBReaderThread.h"
#include "TCPSocketClientArray.h"

#include <gmsec5/internal/ConnectionInterface.h>

#include <gmsec5/Message.h>

#include <gmsec5/util/CountDownLatch.h>
#include <gmsec5/util/Mutex.h>
#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/StdThread.h>
#include <gmsec5/util/StdUniquePtr.h>

#include <string>

#ifdef WIN32
#include <process.h>
#endif

#define MB_MY_SUBJECT_FIELD_NAME	"MB-MY-SUBJECT-FIELD-FOR-ROUTING"


namespace gmsec_messagebus
{

class MBReaderThread;


/** @brief This class implements the ConnectionInterface abstract base class to support "Magic Bus" middleware.
**/
class GMSEC_MB_API MBConnection : public gmsec::api5::internal::ConnectionInterface
{
public:
	/**
	 * @fn MBConnection(const gmsec::api5::Config& cfg)
	 * Standard constructor that requires a config
	 */
	MBConnection(const gmsec::api5::Config& cfg);


	/**
	 * @fn ~MBConnection()
	 * Destructor
	 */
	virtual ~MBConnection();


	/**
	 * @fn getLibraryRootName()
	 * Retrieve the root library name
	 */
	virtual const char* getLibraryRootName();


	/**
	 * @fn getLibraryVersion()
	 * Retrieve the version of the underlying middleware libraries
	 */
	virtual const char* getLibraryVersion();


	/**
	 * @fn
	 * Returns string containing middleware information.
	 */
	virtual const char* getMWInfo();



	/**
	 * @fn mwConnect()
	 * Establish connection with the mb server
	 */
	virtual void mwConnect();


	/**
	 * @fn Disconnect()
	 * End connection with the mb server
	 */
	virtual void mwDisconnect();


	/**
	 * @fn mwSubscribe(const char* subject, const gmsec::Config &config)
	 * Subscribe to a subject without a callback
	 */
	virtual void mwSubscribe(const char* subject, const gmsec::api5::Config& config);


	/**
	 * @fn UnSubscribe(const char *subject)
	 * Unsubscribe from a subject
	 */
	virtual void mwUnsubscribe(const char* subject);


	/**
	 * @fn mwPublish(const gmsec::api5::Message& msg, const gmsec::api5::Config& config)
	 * publish a message
	 */
	virtual void mwPublish(const gmsec::api5::Message& msg, const gmsec::api5::Config& config);


	/**
	 * @fn mwRequest(const gmsec::api5::Message& request)
	 * send a request message
	 */
	virtual void mwRequest(const gmsec::api5::Message& request, std::string& id);


	/**
	 * @fn mwReceive(gmsec::api5::Message*& msg, GMSEC_I32 timeout)
	 * pull the next message off the inbound queue
	 */
	virtual void mwReceive(gmsec::api5::Message*& msg, GMSEC_I32 timeout);


	void handleReply(gmsec::api5::Message* reply);


	void ackSubscription();


private:
	typedef gmsec::api5::util::StdUniquePtr<TCPSocketClientArray>         ClientArray;
	typedef gmsec::api5::util::StdUniquePtr<gmsec::api5::util::StdThread> ReaderThread;
	typedef gmsec::api5::util::StdSharedPtr<MBReaderThread>               ReaderThreadShared;

	static void runReaderThread(ReaderThreadShared shared);
	static void stopReaderThread(ReaderThreadShared shared);

	std::string generateUniqueId();

	ClientArray                                 m_sock;
	long                                        m_port;
	std::string                                 m_server;
	std::string                                 m_mwinfo;
	bool                                        m_isLoopingMsgs;
	bool                                        m_startServer;
	gmsec::api5::util::Mutex                    m_subscribeMutex;
	volatile gmsec::api5::util::CountDownLatch* m_subscribeLatch;
	int                                         m_subscribeTimeout_ms;

	ReaderThread                                m_readerThread;
	ReaderThreadShared                          m_readerThreadShared;

	GMSEC_U32                                   m_messageCounter;
};

}  // end namespace gmsec_messagebus

#endif  // MB_CONNECTION_H
