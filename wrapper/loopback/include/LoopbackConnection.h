/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file LoopbackConnection.h
 * @brief This file provides a template for implementing a Loopback wrapper.
 */

#ifndef LOOPBACK_CONNECTION_H
#define LOOPBACK_CONNECTION_H

#include "gmsec_loopback.h"

#include <gmsec5/internal/ConnectionInterface.h>
#include <gmsec5/internal/TicketMutex.h>

#include <gmsec5/Config.h>
#include <gmsec5/Message.h>

#include <gmsec5/util/BoundedQueue.h>

#include <map>
#include <string>


namespace gmsec_loopback
{

/**
 * @class LoopbackConnection
 * This class provides a LoopbackConnection implementation of the
 * ConnectionInterface abstract class.
 */
class GMSEC_LOOPBACK_API LoopbackConnection : public gmsec::api5::internal::ConnectionInterface
{
public:
	/**
	 * @fn LoopbackConnection(const gmsec::api5::Config& cfg)
	 * @brief Standard constructor that requires a config
	 */
	LoopbackConnection(const gmsec::api5::Config& config);


	virtual ~LoopbackConnection();


	/**
	 * @fn getLibraryRootName()
	 * @brief Retrieve the root library name
	 */
	virtual const char* getLibraryRootName()
	{
		return "gmsec_loopback";
	}


	/**
	 * @fn getLibraryVersion()
	 * @brief Retrieve the version of the underlying middleware libraries
	 */
	virtual const char* getLibraryVersion();


	/**
	 * @fn getMWInfo()
	 * @brief Returns string containing middleware information.
	 */
	virtual const char* getMWInfo();


	/**
	 * @fn mwConnect()
	 * @brief Establish connection to the server.
	 */
	virtual void mwConnect();


	/**
	 * @fn mwDisconnect()
	 * @brief End connection to the server.
	 */
	virtual void mwDisconnect();


	/**
	 * @fn mwSubscribe(const char* subject, const gmsec::api5::Config& config)
	 * @brief Subscribe to a subject without a callback
	 */
	virtual void mwSubscribe(const char* subject, const gmsec::api5::Config& config);


	/**
	 * @fn mwUnsubscribe(const char* subject)
	 * @brief Unsubscribe from a subject
	 */
	virtual void mwUnsubscribe(const char* subject);


	/**
	 * @fn mwPublish(const gmsec::api5::Message& msg, const gmsec::api5::Config& config)
	 * @brief Send the message to the middleware.
	 */
	virtual void mwPublish(const gmsec::api5::Message& msg, const gmsec::api5::Config& config);


	/**
	 * @fn mwRequest(const gmsec::api5::Message& request, const std::string& id)
	 * @brief Send a request message to the middleware.
	 */
	virtual void mwRequest(const gmsec::api5::Message& request, const std::string& id);


	/**
	 * @fn mwReceive(gmsec::api5::Message*& msg, GMSEC_I32 timeout)
	 * @brief pull the next message off the inbound queue
	 */
	virtual void mwReceive(gmsec::api5::Message*& msg, GMSEC_I32 timeout);


	/**
	 * @fn std::string getUniqueID()
	 *
	 * @brief Returns a unique identifier string to be included within a request messages.
	 *
	 * @return A string.
	 */
	virtual std::string mwGetUniqueID();
	

	struct MessageOwner
	{
		std::string ownerID;
		gmsec::api5::Message* msg;
	};

	typedef std::multimap<std::string, std::string>        Subscriptions;     // ownerID, topic
	typedef gmsec::api5::internal::TicketMutex             SubscriptionMutex;
	typedef gmsec::api5::util::BoundedQueue<MessageOwner*> Queue;


private:
	bool haveSubscriber(const std::string& subject);
	bool isSubscribed(const std::string& ownerID, const std::string& subject);


	std::string       m_mwInfo;
	int               m_uniquecounter;
	bool              m_simConnectFailure;
	bool              m_simSubscribeFailure;
	bool              m_simUnsubscribeFailure;
	bool              m_simPublishFailure;
	bool              m_simRequestFailure;
	bool              m_simReplyFailure;
	bool              m_simReceiveFailure;
	int               m_publishDelay;

	static Subscriptions     s_subscriptions;
	static SubscriptionMutex s_mutex;
	static Queue             s_queue;
};

}  // end namespace gmsec_loopback

#endif
