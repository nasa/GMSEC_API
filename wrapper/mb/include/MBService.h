/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef MB_SERVICE_H
#define MB_SERVICE_H

#include "TCPSocketServer.h"

#include <gmsec5/internal/CallbackLookup.h>

#include <gmsec5/Callback.h>
#include <gmsec5/Config.h>
#include <gmsec5/Message.h>
#include <gmsec5/MessageFactory.h>

#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/Mutex.h>
#include <gmsec5/util/StdThread.h>

#include <list>


namespace gmsec_messagebus
{

class ConnectionMgr;
class MBResourceData;


/**
 * @brief This is the class that takes care of handing incomming connections for the MB Server
 */
class MBService
{
public:
	/**
	 * @fn MBService(int port, const gmsec::api5::Config& config)
	 * constructor used to create a new service on a specific port
	 */
	MBService(int port, const gmsec::api5::Config& config);


	/**
	 * @fn ~MBService()
	 * destructor
	 */
	~MBService();


	/**
	 * @fn run()
	 * main function for the MBService class and
	 * takes care of accepting new connections
	 * as well as starting new connection managers
	 */
	void run();


	/** @fn Stop()
	 */
	void stop();


	/**
	 * @fn startNewMgr(TCPSocketServer* newSocket)
	 * start a new connection manager with the provided socket
	 */
	void startNewMgr(TCPSocketServer* newSocket);


	/**
	 * @fn addCallback(const char* subject, gmsec::api5::Callback* cb)
	 * add a callback to the message dispatcher
	 */
	void addCallback(const char* subject, gmsec::api5::Callback* cb);


	/**
	 * @fn removeCallback(const char* subject, gmsec::api5::Callback* cb)
	 * remove a callback from the message dispatcher
	 */
	void removeCallback(const char* subject, gmsec::api5::Callback* cb);


	/**
	 * @fn dispatchMsg(const gmsec::api5::Message& msg)
	 * dispatch a message through the message dispatcher
	 */
	void dispatchMsg(const gmsec::api5::Message& msg);


	/**
	 * @fn getCallbackCount(const char* subject)
	 * this method gets the number of callbacks subscribed to this subject
	 */
	size_t getCallbackCount(const char* subject);


	/**
	 * @fn getClientCount()
	 * Returns the number of clients connected to the MBServer
	 */
	int getClientCount();


	/**
	 * @fn getMBResourceDataInstance()
	 * Returns a pointer to the MBResourceData object that was
	 * instantiated by MBService for the collection of resource data.
	 */
	MBResourceData* getMBResourceDataInstance();


	gmsec::api5::util::Mutex& getCallbackLookupMutex();


	void setExitable(bool flag);


private:
	// Defined, but not implemented
	MBService(const MBService&);
	MBService& operator=(const MBService&);


	typedef gmsec::api5::util::StdSharedPtr<ConnectionMgr> SharedConnectionMgr;
	typedef gmsec::api5::util::StdThread*                  ConnectionMgrThread;

	struct ManagedConnectionMgr
	{
		SharedConnectionMgr shared;
		ConnectionMgrThread thread;
	};

	typedef std::list<ManagedConnectionMgr*>              ConnectionMgrList;


	/**
	 * @fn manageSessions(ConnectionMgr* connMgr);
	 * cleans any session that have ended and adds
	 * the a new manager to a master list of managers
	 */
	void manageSessions(ConnectionMgr* connMgr);


	static void runConnMgr(SharedConnectionMgr shared);


	// Member data
	gmsec::api5::util::Mutex               m_callLkpLock;     // mutex for the callbackLookup
	TCPSocketServer                        m_sock;            // socket performing accept operations
	int                                    m_port;            // port for the server to use
	gmsec::api5::MessageFactory            m_msgFactory;      // factory for creating messages
	gmsec::api5::internal::CallbackLookup* m_callbackLookup;  // lookup with subscription and routing information

	MBResourceData*                        MBR_instance;      // Pointer to MBResourceData instance.

	bool                                   m_allowExit;

	ConnectionMgrList                      m_connMgrs;        // owned connection managers
};

}  // end namespace gmsec_messagebus

#endif
