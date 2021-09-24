/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef MB_CONNECTION_H
#define MB_CONNECTION_H

#include "gmsec_mb.h"

#include "MBReaderThread.h"
#include "TCPSocketClientArray.h"

#include <gmsec4/internal/ConnectionInterface.h>

#include <gmsec4/Message.h>

#include <gmsec4/util/CountDownLatch.h>
#include <gmsec4/util/Mutex.h>
#include <gmsec4/util/StdSharedPtr.h>
#include <gmsec4/util/StdThread.h>
#include <gmsec4/util/StdUniquePtr.h>

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
class GMSEC_MB_API MBConnection : public gmsec::api::internal::ConnectionInterface
{
public:
	/**
	 * @fn MBConnection(const gmsec::api::Config& cfg)
	 * Standard constructor that requires a config
	 */
	MBConnection(const gmsec::api::Config& cfg);


	/**
	 * @fn ~MBConnection()
	 * Destructor
	 */
	virtual ~MBConnection();


	/**
	 * @fn getLibraryRootName()
	 * Retrieve the root library name
	 */
	virtual const char* CALL_TYPE getLibraryRootName();


	/**
	 * @fn getLibraryVersion()
	 * Retrieve the version of the underlying middleware libraries
	 */
	virtual const char* CALL_TYPE getLibraryVersion();


	/**
	 * @fn
	 * Returns string containing middleware information.
	 */
	virtual const char* CALL_TYPE getMWInfo();



	/**
	 * @fn mwConnect()
	 * Establish connection with the mb server
	 */
	virtual void CALL_TYPE mwConnect();


	/**
	 * @fn Disconnect()
	 * End connection with the mb server
	 */
	virtual void CALL_TYPE mwDisconnect();


	/**
	 * @fn mwSubscribe(const char* subject, const gmsec::Config &config)
	 * Subscribe to a subject without a callback
	 */
	virtual void CALL_TYPE mwSubscribe(const char* subject, const gmsec::api::Config& config);


	/**
	 * @fn UnSubscribe(const char *subject)
	 * Unsubscribe from a subject
	 */
	virtual void CALL_TYPE mwUnsubscribe(const char* subject);


	/**
	 * @fn mwPublish(const gmsec::api::Message& msg, const gmsec::api::Config& config)
	 * publish a message
	 */
	virtual void CALL_TYPE mwPublish(const gmsec::api::Message& msg, const gmsec::api::Config& config);


	/**
	 * @fn mwRequest(const gmsec::api::Message& request, std::string& id)
	 * send a request message
	 */
	virtual void CALL_TYPE mwRequest(const gmsec::api::Message& request, std::string& id);


	/**
	 * @fn mwReply(const gmsec::api::Message& request, const gmsec::api::Message& reply)
	 * send a reply to a request message
	 */
	virtual void CALL_TYPE mwReply(const gmsec::api::Message& request, const gmsec::api::Message& reply);


	/**
	 * @fn mwReceive(gmsec::api::Message*& msg, GMSEC_I32 timeout)
	 * pull the next message off the inbound queue
	 */
	virtual void CALL_TYPE mwReceive(gmsec::api::Message*& msg, GMSEC_I32 timeout);


	void handleReply(gmsec::api::Message* reply);


	void ackSubscription();


private:
	typedef gmsec::api::util::StdUniquePtr<gmsec::api::util::StdThread> ReaderThread;
	typedef gmsec::api::util::StdSharedPtr<MBReaderThread>              ReaderThreadShared;

	static void runReaderThread(ReaderThreadShared shared);
	static void stopReaderThread(ReaderThreadShared shared);

	std::string generateUniqueId();

	TCPSocketClientArray*                      m_sock;
	long                                       m_port;
	std::string                                m_server;
	std::string                                m_mwinfo;
	gmsec::api::internal::RequestSpecs         m_reqSpecs;
	bool                                       m_isLoopingMsgs;
	bool                                       m_startServer;
	gmsec::api::util::Mutex                    m_subscribeMutex;
	volatile gmsec::api::util::CountDownLatch* m_subscribeLatch;
	int                                        m_subscribeTimeout_ms;

	ReaderThread                               m_readerThread;
	ReaderThreadShared                         m_readerThreadShared;

	GMSEC_U32                                  m_messageCounter;
};

}  // end namespace gmsec_messagebus

#endif  // MB_CONNECTION_H
