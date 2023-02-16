/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file CMSConnection.h
 * Holds the declaration of ActiveMQ subclass of gmsec::api::internal::ConnectionInterface.
 */

#ifndef CMS_CONNECTION_H
#define CMS_CONNECTION_H

#include "gmsec_activemq.h"

#include <gmsec5/internal/ConnectionInterface.h>
#include <gmsec5/internal/MiddlewareInfo.h>
#include <gmsec5/internal/UniqueFilter.h>

#include <gmsec5/Config.h>
#include <gmsec5/Message.h>

#include <gmsec5/util/StdUniquePtr.h>
#include <gmsec5/util/StdSharedPtr.h>

#include <cms/Connection.h>
#include <cms/ConnectionFactory.h>
#include <cms/DeliveryMode.h>
#include <cms/Message.h>
#include <cms/MessageConsumer.h>
#include <cms/Session.h>
#include <cms/ExceptionListener.h>

#include <activemq/transport/DefaultTransportListener.h>
#include <decaf/lang/Pointer.h>

#if ACTIVEMQ_CMS_V2
#include <decaf/util/Queue.h>
typedef decaf::util::Queue<cms::Message*> MessageQueue;
#else
#include <decaf/util/StlQueue.h>
typedef decaf::util::StlQueue<cms::Message*> MessageQueue;
#endif


namespace gmsec_amqcms
{


/**
 * @class SubscriptionInfo
 * Active MQ specific class for managing subscription details.
 */
class SubscriptionInfo
{
public:
	/**
	* @brief Constructor- assumes ownership of the consumer,
	* and the consumer's MessageListener.
	*/
	SubscriptionInfo(cms::MessageConsumer *consumer);

	/**
	* @brief Destructor- destroys consumer, and consumer's
	* MessageListener.
	*/
	~SubscriptionInfo();

	void setExtra(SubscriptionInfo *info);

private:
	// not implemented
	SubscriptionInfo (const SubscriptionInfo &);
	SubscriptionInfo &operator=(const SubscriptionInfo &);

	cms::MessageConsumer *consumer;
	SubscriptionInfo *extra;
};


// Forward declaration
class CMSConnection;


/**
 * @class CMSTransportListener
 * Callback that is used to dispatch CMS Transport events.
 */
class CMSTransportListener : public activemq::transport::DefaultTransportListener
{
public:
	CMSTransportListener(CMSConnection* conn, bool issueFailoverEvents);

	virtual ~CMSTransportListener();

	virtual void onCommand(const decaf::lang::Pointer<activemq::commands::Command> command AMQCPP_UNUSED);
	virtual void transportInterrupted();
	virtual void transportResumed();

private:
	CMSConnection* connection;
	bool failoverEvents;
};


/**
 * @class CMSExceptionListener
 * Callback that is used to dispatch CMS Exception errors.
 */
class CMSExceptionListener : public cms::ExceptionListener
{
public:
	CMSExceptionListener(CMSConnection* conn);

	virtual ~CMSExceptionListener();

	virtual void onException(const cms::CMSException& e);

private:
	CMSConnection* connection;
};


/**
 * @class CMSConnection
 * Active MQ / CMS implementation of GMSEC ConnectionInterface.
 */
class GMSEC_ACTIVEMQ_API CMSConnection : public gmsec::api5::internal::ConnectionInterface
{
private:
	gmsec::api5::util::StdUniquePtr<cms::ConnectionFactory> m_cmsFactory;
	gmsec::api5::util::StdUniquePtr<cms::Connection>        m_connection;
	gmsec::api5::util::StdUniquePtr<cms::Session>           m_session;
	gmsec::api5::util::StdUniquePtr<cms::Destination>       m_requestReplyDestination;
	gmsec::api5::util::StdUniquePtr<cms::MessageProducer>   m_requestReplyProducer;
	gmsec::api5::util::StdUniquePtr<cms::MessageConsumer>   m_replyConsumer;
	std::map<std::string, SubscriptionInfo*>                m_subscriptions;
	MessageQueue                                            m_queue;
	gmsec::api5::internal::UniqueFilter                     m_uniqueFilter;

	// configuration
	std::string m_brokerURI;
	std::string m_connClientId;
	std::string m_username;
	std::string m_password;
	std::string m_keystore;
	std::string m_keystorePassword;
	std::string m_truststore;
	std::string m_truststorePassword;
	bool        m_useFilter;
	bool        m_reportFailoverEvent;
	int         m_ackMode;

	// Request Specs
	gmsec::api5::internal::RequestSpecs m_requestSpecs;

	// middleware info
	std::string m_mwinfo;

	gmsec::api5::util::StdUniquePtr<CMSTransportListener> m_transportListener;
	gmsec::api5::util::StdUniquePtr<CMSExceptionListener> m_exceptionListener;


	/**
	 * @brief Utility function for releasing resources.
	 */
	void cleanup();


	/**
	 * @brief Prepare a CMS Message for a GMSEC Message.
	 */
	void prepare(const gmsec::api5::Message& msg, gmsec::api5::util::StdUniquePtr<cms::Message>& cmsMsg);


	/**
	 * @brief Handles the conversion of a CMS message to a GMSEC message, and the delivery of such
	 */
	void handleReply(const cms::Message* message);


	/**
	 * @brief Extract a GMSEC Message from a CMS Message.
	 */
	void unload(const cms::Message* cmsMsg, gmsec::api5::Message*& gmsecMsg);


	/**
	 * @brief Actual implementation of mwReceive
	 */
	void mwReceiveAux(gmsec::api5::Message*& msg, GMSEC_I32 timeout);


	SubscriptionInfo* makeSubscriptionInfo(const std::string& in, const gmsec::api5::Config& config);


public:
	/**
	 * @fn CMSConnection(const gmsec::api::Config& config)
	 * @brief Create a CMS Connection with configuration.
	 */
	CMSConnection(const gmsec::api5::Config& config);


	/**
	 * @brief Destroy a CMS Connection.
	 */
	virtual ~CMSConnection();


	/**
	 * @brief Determines the middleware wrapper tag.
	 */
	virtual const char* getLibraryRootName()
	{
		return ACTIVEMQVERSIONSTRING;	// defined in gmsec_activemq.h
	}


	/**
	 * @brief Determines the middleware version string.
	 */
	virtual const char* getLibraryVersion();


	/**
	 * @brief Determine the unique connection identifier.
	 */
	virtual const char* getMWInfo();


	/**
	 * @brief Connect to the middleware server.
	 */
	virtual void mwConnect();


	/**
	 * @brief Disconnect from the middleware server.
	 */
	virtual void mwDisconnect();


	/**
	 * @brief Subscribe to @p subject (which possibly contains wildcards)
	 * using the configuration supplied in the config object.
	 */
	virtual void mwSubscribe(const char *subject, const gmsec::api5::Config &config);


	/**
	 * @brief Unsubscribe from @p subject (which possibly contains wildcards).
	 */
	virtual void mwUnsubscribe(const char* subject);


	/**
	 * @brief Publish message @p msg using @p configuration.
	 */
	virtual void mwPublish(const gmsec::api5::Message& msg, const gmsec::api5::Config& config);


	/**
	 * @brief Send request message.
	 */
	virtual void mwRequest(const gmsec::api5::Message& request, std::string& id);


	/**
	 * @brief Assists in delivering reply messages to the client.
	 */
	void handleCmsReply(const cms::Message* cmsReply, bool logStackTrace);


	/**
	 * @brief Wait up to @p timeout milliseconds for next message.
	 */
	virtual void mwReceive(gmsec::api5::Message*& msg, GMSEC_I32 timeout);


	/**
	 * @brief Acknowledge receipt/processing of underlying CMS message
	 */
	virtual void mwAcknowledge(gmsec::api5::util::StdSharedPtr<gmsec::api5::internal::MiddlewareInfo>& info);
};

}  // end namespace gmsec_amqcms

#endif  // CMS_CONNECTION_H
