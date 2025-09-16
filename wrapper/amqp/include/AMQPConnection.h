/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* @file AMQPConnection.h
 * This file provides a template for implementing a AMQP wrapper.
 */

#ifndef AMQP_CONNECTION_H
#define AMQP_CONNECTION_H

#include "gmsec_amqp.h"
#include "SafeQueue.h"

#include <gmsec5_defs.h>

#include <gmsec5/internal/ConnectionInterface.h>
#include <gmsec5/internal/Encoder.h>
#include <gmsec5/internal/TicketMutex.h>
#include <gmsec5/internal/UniqueFilter.h>

#include <gmsec5/Config.h>
#include <gmsec5/Message.h>
#include <gmsec5/Status.h>

#include <gmsec5/util/CountDownLatch.h>
#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/StdThread.h>
#include <gmsec5/util/StdUniquePtr.h>

#include <queue>
#include <string>


namespace gmsec_amqp
{

// Forward declaration(s)
class AMQPConnection;
class ProtonReceiver;


struct SSLConfiguration
{
	std::string tStore;
	std::string pKey;
	std::string kPass;
	std::string cert;

	SSLConfiguration(const std::string& trustStore, const std::string& privateKey, const std::string& keyPassword, const std::string& certificate)
		: tStore(trustStore),
		  pKey(privateKey),
		  kPass(keyPassword),
		  cert(certificate)
	{}
};


/**
 * @class AMQPConnection
 * This class provides an AMQPConnection implementation of the
 * BaseConnection abstract class.
 */
class GMSEC_AMQP_API AMQPConnection : public gmsec::api5::internal::ConnectionInterface
{
public:
	enum Reasons
	{
		QUIT = gmsec::api5::util::Condition::USER + 1,
		OKAY_TO_SEND,
		GOT_MESSAGE,
		LISTENING,
		UNABLE_TO_SUBSCRIBE,
		PROTON_ERROR,
		COULDNT_GET_MESSAGE
	};


	gmsec::api5::util::Condition m_msgQueueCondition;
	gmsec::api5::util::Condition m_publishCondition;
	gmsec::api5::util::Condition m_subscribeCondition;


	/**
	 * @fn AMQPConnection(const gmsec::api5::Config& cfg)
	 * @brief Standard constructor that requires a config
	 */
	AMQPConnection(const gmsec::api5::Config& cfg);


	virtual ~AMQPConnection();


	/**
	 * @fn getLibraryRootName()
	 * @brief Retrieve the root library name
	 */
	virtual const char* getLibraryRootName()
	{
		return "gmsec_amqp";
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
	 * @fn mwRequest(const gmsec::api5::Message& request)
	 * @brief Send a request message to the middleware.
	 */
	virtual void mwRequest(const gmsec::api5::Message& request, const std::string& uniqueID);


	/**
	 * @fn mwReceive(gmsec::api5::Message*& msg, GMSEC_I32 timeout)
	 * @brief pull the next message off the inbound queue
	 */
	virtual void mwReceive(gmsec::api5::Message*& msg, GMSEC_I32 timeout);


	/**
	 * @fn mwGetUniqueID()
	 * @brief Return a unique identifier string for request messages
	 */
	virtual std::string mwGetUniqueID();


	struct GMSEC_AMQP_API MsgSubscriptionResult
	{
		gmsec::api5::Status   status;
		gmsec::api5::Message* message;

		MsgSubscriptionResult(const gmsec::api5::Status& status, gmsec::api5::Message* msg)
			: status(status),
			  message(msg)
		{}
	};


private:
	typedef gmsec::api5::util::StdSharedPtr<ProtonReceiver> SharedReceiver;
	typedef std::map<std::string, SharedReceiver>           Subscriptions;


	std::string fixSubject(const std::string& subject);

	std::string unfixSubject(const std::string& subject);

	std::string assembleAddress(const std::string& subject);

	std::string generateUniqueId(long id);

	// configuration
	gmsec::api5::internal::UniqueFilter    m_uniqueFilter;
	int                                    m_uniquecounter;
	std::string                            m_mwInfo;
	long                                   m_requestCounter;
	SafeQueue<MsgSubscriptionResult*>      m_recvQueue;

	Subscriptions                          m_subscriptions;

	gmsec::api5::internal::RequestSpecs    m_specs;

	std::string                            m_hostname;
	std::string                            m_port;
	std::string                            m_username;
	std::string                            m_password;
	bool                                   m_useFilter;
	SSLConfiguration                       m_sslConfig;

	gmsec::api5::internal::TicketMutex     m_mutex;

	friend class ProtonReceiver;
};

}  // end namespace gmsec_amqp

#endif
