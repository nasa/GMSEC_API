/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
* @file ZMQConnection.h
* Holds the declaration of ZeroMQ subclass of gmsec::internal::BaseConnection.
*/

#ifndef GMSEC_ZMQ_MESSAGE_LISTENER_H
#define GMSEC_ZMQ_MESSAGE_LISTENER_H

#include <gmsec5/internal/ExclusionFilter.h>

#include <gmsec5/util/CountDownLatch.h>

#include <string>


namespace gmsec_zeromq
{
class ZMQConnection;


/**	@class ZMQMessageListener
 *	
 *	@brief This class represents a thread that forms a subscription, listens to it,
 *	and posts messages to the appropriate queue within a connection object.
 */
class ZMQMessageListener
{
public:
	/**
	 * @fn ZMQMessageListener(ZMQConnection* zmqConnection, void** subSocket, bool replies)
 	 *
	 * @brief Constructor
	 */
	ZMQMessageListener(ZMQConnection* zmqConnection, void** subSocket);

	
	/**
	 * @fn ~ZMQMessageListener()
 	 *
	 * @brief Basic destructor
	 */
	virtual ~ZMQMessageListener();

	
	/**
	 * @fn run()
 	 *
	 * @brief Start the ZMQMessageListener thread
	 */
	virtual void run();

	
	/**
	 * @fn shutdown()
 	 *
	 * @brief Instruct the thread to break out of the run loop and stop
	 */
	virtual void shutdown();


	/**
	 * @fn Subscribe(const std::string& subject)
	 *
	 * @brief Adds the subject to the list of subscriptions
	 *
	 * @param subject - The subject with which to subscribe
	 */
	void subscribe(const std::string& subject);

	
	/**
	 * @fn Unsubscribe(const std::string& subject)
	 *
	 * @brief Removes the subject from the list of subscriptions
	 *
	 * @param subject - The subject with which to unsubscribe
	 */
	void unsubscribe(const std::string& subject);


private:
	// Defined, but not implemented
	ZMQMessageListener(const ZMQMessageListener&);
	ZMQMessageListener& operator=(const ZMQMessageListener&);


	/**
	 * @fn moreToReceive()
	 *
	 * @brief Helper function to check if there are more ZeroMQ message frames to receive 
	 */
	bool moreToReceive();


	/**
	 * @fn recvMessage(const char* subject)
	 *
	 * @brief Helper function to process an incoming GMSEC Message from the ZMQ Socket
	 */
	void recvMessage(const char* subject);


	// ZeroMQ Object Pointers (Refers to ZMQConnection Objects)
	ZMQConnection*                          m_connectionPtr;
	void*                                   m_subSocketPtr;

	// GMSEC Configuration
	// Note: The ExclusionFilter is logically inverted to act as an InclusionFilter
	gmsec::api5::internal::ExclusionFilter* m_inclusionFilter;
	bool                                    m_doneWithLife;
	gmsec::api5::util::CountDownLatch*      m_deathLatch;
};


}  // end namespace gmsec_zeromq

#endif
