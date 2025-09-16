/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_MB_CCONNECTION_MGR_H
#define GMSEC_MB_CCONNECTION_MGR_H

#include "ConnectionWriter.h"
#include "TCPSocketServer.h"


#include <gmsec5/Message.h>
#include <gmsec5/MessageFactory.h>

#include <gmsec5/util/Atomics.h>
#include <gmsec5/util/Mutex.h>
#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/StdThread.h>
#include <gmsec5/util/StdUniquePtr.h>

#include <list>
#include <string>
#include <queue>


namespace gmsec_messagebus
{

class MBService;
class ConnMgrCallback;
class MBFastMessage;


// This is the subject that is used by the SystemAgent to request
// resource data from the MBServer.
//
#define MB_RESOURCE_DATA_REQUEST_SUBJECT         "GMSEC.REQ.DIR.MB.RSRC"


/**  @brief This class handles communications for a particular client
 **/
class ConnectionMgr
{
public:
	/**
	 * @fn CConnectionMgr(TCPSocketServer *sock, CMBService *mbService, gmsec::api5::MessageFactory& msgFactory)
	 * @brief Constructor for the ConnectionMgr class
	 * @param sock - a pointer to a socket instance returned from a call to accept.
	 * @param mbService - the parent service instance of the manager
	 * @param msgFactory - the message factory to use when creating GMSEC messages
	 */
	ConnectionMgr(TCPSocketServer* sock, MBService* mbService, gmsec::api5::MessageFactory& msgFactory);


	/**
	 * @fn ~CConnectionMgr();
	 * default desstructor
	 */
	~ConnectionMgr();


	/**
	 * @fn run();
	 * this function gets started when then start method in the
	 * ConnectionMgr class is invoked.  It is the main function
	 * for the new execution thread.
	 */
	void run();


	/**
	 * @fn Stop();
	 */
	void shutdown();


	/**
	 * @fn SendMsgToClient(MBFastMessage* msg);
	 * this function gets called indirectly by the
	 * message dispatcher and is used to send
	 * the message to the client that this
	 * manager handles
	 */
	void sendMsgToClient(MBFastMessage* msg);


	/**
	 * @fn sendNextMsgToClient()
	 * sends the next queued message to the client
	 */
	bool sendNextMsgToClient();


	/**
	 * @fn isRunning()
	 * returns running status of this manager
	 */
	bool isRunning();


private:
	// Defined, but not implemented
	ConnectionMgr(const ConnectionMgr&);
	ConnectionMgr& operator=(const ConnectionMgr&);


	/**
	 * @fn doResourceMessage(const gmsec::api5::Message& request)
	 * send the resource data to the requesting component
	 */
	void doResourceMessage(const gmsec::api5::Message& request);


	/**
	 * @fn doSubscribe(const char* subject);
	 * send the subscribe request to the message dispatcher
	 */
	void doSubscribe(const char* subject);


	/**
	 * @fn doUnsubscribe(const char* subject)
	 * remove the subscription entry from the message dispatcher and maintains the list of current subscriptions
	 */
	void doUnsubscribe(const char* subject);


	/**
	 * @fn doDisconnect();
	 * prepare the dispatcher and the manager for terminating the connection to the client
	 */
	void doDisconnect();


	/**
	 * @fn doPublish(const MBFastmessage& msg)
	 * send a message to their designated clients
	 */
	void doPublish(const MBFastMessage& msg);


	typedef std::queue<MBFastMessage*>                                    MessageQueue;
	typedef gmsec::api5::util::StdUniquePtr<gmsec::api5::util::StdThread> ConnWriterThread;
	typedef gmsec::api5::util::StdSharedPtr<ConnectionWriter>             ConnWriterShared;


	// Member data
	TCPSocketServer*                 m_sock;          // socket used for communications
	MBService*                       m_mbService;     // parent service that started this manager
	gmsec::api5::MessageFactory&     m_msgFactory;    // factory to use when creating GMSEC messages
	ConnMgrCallback*                 m_callback;      // unique callback instance used to identify this manager with the callback dispatcher
	std::list<std::string>           m_subscriptions; // list of current subscriptions
	int                              m_debug;         // debug flag
	gmsec::api5::util::Mutex         m_sockLock;      // socket mutex
	bool                             m_isLoopingMsgs; // used to spacify if clients can get messages they publish

	MessageQueue                     m_msgQueue;      // queue of messages that need to be sent to the client
	gmsec::api5::util::Mutex         m_msgQueueMutex; // mutex for the messageQueue
	gmsec::api5::util::AtomicBoolean m_isRunning;     // flag indicating if this manager is running

	ConnWriterThread                 m_connWriterThread;
	ConnWriterShared                 m_connWriterShared;

	static void runConnWriterThread(ConnWriterShared shared);
};

}  // end namespace gmsec_messagebus

#endif
