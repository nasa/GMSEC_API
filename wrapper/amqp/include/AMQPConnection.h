/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec4_defs.h>

#include <gmsec4/internal/ConnectionInterface.h>
#include <gmsec4/internal/Encoder.h>
#include <gmsec4/internal/TicketMutex.h>
#include <gmsec4/internal/UniqueFilter.h>

#include <gmsec4/Config.h>
#include <gmsec4/Message.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/CountDownLatch.h>
#include <gmsec4/util/StdSharedPtr.h>
#include <gmsec4/util/StdThread.h>
#include <gmsec4/util/StdUniquePtr.h>

#include <proton/messenger.h>

#include <queue>
#include <string>


namespace gmsec_amqp
{

// Forward declaration(s)
class SubscriptionInfo;
class AMQPConnection;


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


struct AMQPSubscription
{
	std::string     addr;
	std::string     subject;
	AMQPConnection* conn;

	AMQPSubscription(const std::string& address, const std::string& subscriptionSubject, AMQPConnection* connection)
		: addr(address),
		  subject(subscriptionSubject),
		  conn(connection)
	{}
};


/**
 * @class AMQPConnection
 * This class provides an AMQPConnection implementation of the
 * BaseConnection abstract class.
 */
class GMSEC_AMQP_API AMQPConnection : public gmsec::api::internal::ConnectionInterface
{
public:
	enum Reasons
	{
		QUIT = gmsec::api::util::Condition::USER + 1,
		GOT_MESSAGE,
		LISTENING,
		UNABLE_TO_SUBSCRIBE,
		MESSENGER_ERROR,
		COULDNT_GET_MESSAGE
	};


	gmsec::api::util::Condition queueCondition;
	gmsec::api::util::Condition subscribeCondition;


	/**
	 * @fn AMQPConnection(const gmsec::api::Config& cfg)
	 * @brief Standard constructor that requires a config
	 */
	AMQPConnection(const gmsec::api::Config& cfg);


	virtual ~AMQPConnection();


	/**
	 * @fn getLibraryRootName()
	 * @brief Retrieve the root library name
	 */
	virtual const char* CALL_TYPE getLibraryRootName()
	{
		return "gmsec_amqp";
	}


	/**
	 * @fn getLibraryVersion()
	 * @brief Retrieve the version of the underlying middleware libraries
	 */
	virtual const char* CALL_TYPE getLibraryVersion();


	/**
	 * @fn getMWInfo()
	 * @brief Returns string containing middleware information.
	 */
	virtual const char* getMWInfo();


	/**
	 * @fn mwConnect()
	 * @brief Establish connection to the server.
	 */
	virtual void CALL_TYPE mwConnect();


	/**
	 * @fn mwDisconnect()
	 * @brief End connection to the server.
	 */
	virtual void CALL_TYPE mwDisconnect();


	/**
	 * @fn mwSubscribe(const char* subject, const gmsec::api::Config& config)
	 * @brief Subscribe to a subject without a callback
	 */
	virtual void CALL_TYPE mwSubscribe(const char* subject, const gmsec::api::Config& config);


	/**
	 * @fn mwUnsubscribe(const char* subject)
	 * @brief Unsubscribe from a subject
	 */
	virtual void CALL_TYPE mwUnsubscribe(const char* subject);


	/**
	 * @fn mwPublish(const gmsec::api::Message& msg, const gmsec::api::Config& config)
	 * @brief Send the message to the middleware.
	 */
	virtual void CALL_TYPE mwPublish(const gmsec::api::Message& msg, const gmsec::api::Config& config);


	/**
	 * @fn mwRequest(const gmsec::api::Message& request, std::string& id);
	 * @brief Send a request message to the middleware.
	 */
	virtual void CALL_TYPE mwRequest(const gmsec::api::Message& request, std::string& id);


	/**
	 * @fn mwReply(const gmsec::api::Message& request, const gmsec::api::Message& reply)
	 * @brief Send a reply message to the middleware.
	 */
	virtual void CALL_TYPE mwReply(const gmsec::api::Message& request, const gmsec::api::Message& reply);


	/**
	 * @fn mwReceive(gmsec::api::Message*& msg, GMSEC_I32 timeout)
	 * @brief pull the next message off the inbound queue
	 */
	virtual void CALL_TYPE mwReceive(gmsec::api::Message*& msg, GMSEC_I32 timeout);
	

	std::string fixSubject(const char* str);


	std::string unfixSubject(const char* str);


	void handleMessage(pn_message_t* message, bool replies);


	void enqueueResult(const gmsec::api::Status& status, gmsec::api::Message* message);


	bool getAMQPURIConfig();


	std::string assembleAddress(const char* address, const char* subject);


	void publishTestMessage(const char* subject);


private:
	typedef gmsec::api::util::StdUniquePtr<gmsec::api::util::StdThread> SubscriptionThread;
	typedef gmsec::api::util::StdSharedPtr<SubscriptionInfo>            SharedSubscriptionInfo;
	typedef std::map<std::string, SharedSubscriptionInfo>               Subscriptions;


	struct GMSEC_AMQP_API MsgSubscriptionResult
	{
		gmsec::api::Status   status;
		gmsec::api::Message* message;

		MsgSubscriptionResult(const gmsec::api::Status& status, gmsec::api::Message* msg)
			: status(status),
			  message(msg)
		{}
	};


	std::string generateUniqueId(long id);

	static void runSubscriptionThread(SharedSubscriptionInfo shared);


	// configuration
	const char*                               address;
	gmsec::api::internal::UniqueFilter        uniqueFilter;
	int                                       uniquecounter;
	std::string                               mwInfo;
	long                                      requestCounter;
	std::queue<MsgSubscriptionResult*>        queue;
	bool                                      sigMismatchFlag;
	pn_messenger_t*                           pubMessenger;
	std::string                               uriScheme;
	std::string                               vhost;
	std::string                               wcard1;
	std::string                               wcard2;
	std::string                               wcard3;
	std::string                               wcard4;
	bool                                      isApollo;
	std::string                               configFile;

	SubscriptionThread                        requestThread;

	Subscriptions                             subscriptions;

	gmsec::api::internal::RequestSpecs        specs;

	// C2 Stuff
	std::string                               username;
	std::string                               password;
	std::string                               hostname;
	bool                                      useFilter;
	SSLConfiguration                          sslConfig;

	gmsec::api::util::TicketMutex             m_mutex;
};


/**
 * @class SubscriptionInfo
 *	
 * @brief This class represents a thread that forms a subscription, listens to it,
 * and posts messages to the appropriate queue within a connection object.
 */
class SubscriptionInfo
{
public:
	/**
	 * @fn SubscriptionInfo(xms::Destination topic, xms::MessageConsumer consumer)
	 *
	 * @brief Constructor
	 */
	SubscriptionInfo(const AMQPSubscription& amqpSubscription,
	                 gmsec::api::util::CountDownLatch* cdlatch,
	                 const SSLConfiguration& sslConfig,
	                 bool replies);

	
	/**
	 * @fn ~SubscriptionInfo()
	 *
	 * @brief Basic destructor
	 */
	~SubscriptionInfo();
	

	/**
	 * @fn run()
	 *
	 * @brief As this is a thread, run starts the thread
	 */
	void CALL_TYPE run();


	/**
	 * @fn shutdown()
	 *
	 * @brief Stops the thread
	 */
	void CALL_TYPE shutdown();

	
	/**
	 * @fn getReasonCode()
	 *
	 * @brief Returns the stored reason code
	 */
	long getReasonCode() const;


private:
	bool initSubscriptionWithConn();

	bool                              doneWithLife;
	bool                              connected;
	gmsec::api::util::CountDownLatch* deathLatch;
	AMQPSubscription                  amqpSub;
	pn_messenger_t*                   threadMessenger;
	bool                              isReply;
	gmsec::api::util::CountDownLatch* listeningLatch;
	SSLConfiguration                  ssl;
};

}  // end namespace gmsec_amqp

#endif
