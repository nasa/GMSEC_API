/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file CMSConnection.cpp
 *
 *  This file contains the implementation for the ActiveMQ Connection.
 */

#include "CMSConnection.h"

#include "CMSDestination.h"

#include <gmsec4/ConfigOptions.h>

#include <gmsec4/internal/InternalConnection.h>
#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/Rawbuf.h>
#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/util/Log.h>

#include <gmsec4/Connection.h>
#include <gmsec4/Exception.h>

#include <activemq/commands/BrokerInfo.h>
#include <activemq/core/ActiveMQConnection.h>
#include <decaf/lang/System.h>
#include <decaf/lang/Long.h>
#include <decaf/util/Random.h>

#ifndef ACTIVEMQ_CMS_V2
#include <gmsec4/internal/Middleware.h>

#include <activemq/library/ActiveMQCPP.h>
#endif

#include <sstream>

#define CMS_TYPE_PUBLISH  "PUBLISH"
#define CMS_TYPE_REQUEST  "REQUEST"
#define CMS_TYPE_REPLY    "REPLY"
#define CMS_TYPE_UNSET    "UNSET"

/**
 * @def GMSEC_PROP_TYPES
 * Since CMS does not provide a way to determine property
 * types, this property stores the encoding
 */
#define GMSEC_PROP_TYPES  "GTYPES"

/**
 * @def GMSEC_SUBJECT_PROPERTY
 * The CMS message property used for the GMSEC message subject.
 */
#define GMSEC_SUBJECT_PROPERTY "GMSEC-SUBJECT"


static const bool DEBUG_UNLOAD = false;


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;

using namespace gmsec_amqcms;


static Field::FieldType lookupFieldType(const char* fieldTypeStr)
{
	Field::FieldType type;

	bool error = false;

	switch (fieldTypeStr[0])
	{
	case 'B':
		if (StringUtil::stringEquals(fieldTypeStr, "BOOL") || StringUtil::stringEquals(fieldTypeStr, "BOOLEAN"))
		{
			type = Field::BOOL_TYPE;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "BIN"))
		{
			type = Field::BIN_TYPE;
		}
		else
		{
			error = true;
		}
		break;

	case 'C':
		if (StringUtil::stringEquals(fieldTypeStr, "CHAR"))
		{
			type = Field::CHAR_TYPE;
		}
		else
		{
			error = true;
		}
		break;

	case 'D':
		if (StringUtil::stringEquals(fieldTypeStr, "DOUBLE"))
		{
			type = Field::F64_TYPE;
		}
		else
		{
			error = true;
		}
		break;

	case 'F':
		if (StringUtil::stringEquals(fieldTypeStr, "F32") || StringUtil::stringEquals(fieldTypeStr, "FLOAT"))
		{
			type = Field::F32_TYPE;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "F64"))
		{
			type = Field::F64_TYPE;
		}
		else
		{
			error = true;
		}
		break;

	case 'I':
		if (StringUtil::stringEquals(fieldTypeStr, "I16"))
		{
			type = Field::I16_TYPE;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "I32"))
		{
			type = Field::I32_TYPE;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "I64"))
		{
			type = Field::I64_TYPE;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "I8"))
		{
			type = Field::I8_TYPE;
		}
		else
		{
			error = true;
		}
		break;

	case 'L':
		if (StringUtil::stringEquals(fieldTypeStr, "LONG"))
		{
			type = Field::I32_TYPE;
		}
		else
		{
			error = true;
		}
		break;

	case 'S':
		if (StringUtil::stringEquals(fieldTypeStr, "STRING"))
		{
			type = Field::STRING_TYPE;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "SHORT"))
		{
			type = Field::I16_TYPE;
		}
		else
		{
			error = true;
		}
		break;

	case 'U':
		if (StringUtil::stringEquals(fieldTypeStr, "U16") || StringUtil::stringEquals(fieldTypeStr, "USHORT"))
		{
			type = Field::U16_TYPE;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "U32") || StringUtil::stringEquals(fieldTypeStr, "ULONG"))
		{
			type = Field::U32_TYPE;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "U64"))
		{
			type = Field::U64_TYPE;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "U8"))
		{
			type = Field::U8_TYPE;
		}
		else
		{
			error = true;
		}
		break;

	default:
		error = true;
		break;
    }

	if (error)
	{
		std::ostringstream oss;
		oss << "Unknown Field Type: " << fieldTypeStr;
		GMSEC_VERBOSE << oss.str().c_str();
		throw Exception(FIELD_ERROR, UNKNOWN_FIELD_TYPE, oss.str().c_str());
	}

	return type;
}


static Message::MessageKind lookupMessageKind(const char* msgTypeStr)
{
	Message::MessageKind kind;

	if (StringUtil::stringEqualsIgnoreCase(msgTypeStr, CMS_TYPE_PUBLISH))
	{
		kind = Message::PUBLISH;
	}
	else if (StringUtil::stringEqualsIgnoreCase(msgTypeStr, CMS_TYPE_REQUEST))
	{
		kind = Message::REQUEST;
	}
	else if (StringUtil::stringEqualsIgnoreCase(msgTypeStr, CMS_TYPE_REPLY))
	{
		kind = Message::REPLY;
	}
	else
	{
		std::ostringstream oss;
		oss << "Unknown message kind: " << msgTypeStr;
		GMSEC_VERBOSE << oss.str().c_str();
		throw Exception(MSG_ERROR, UNKNOWN_MSG_TYPE, oss.str().c_str());
	}

	return kind;
}


/**
* @brief the generic listener for all subscriptions.
* As each message arrives, it is placed on the (synchronized) queue.
*/

class TopicListener : public cms::MessageListener
{
public:
	TopicListener(CMSConnection* connection, MessageQueue &q, const RequestSpecs& specs)
			: connection(connection), queue(q), requestSpecs(specs), logStackTrace(false) { }

	~TopicListener() throw () { }

	virtual void onMessage(const cms::Message *message) throw ()
	{
		try
		{
			std::string          msgType = message->getCMSType();
			Message::MessageKind msgKind;

			try
			{
				msgKind = lookupMessageKind(msgType.c_str());
			}
			catch (Exception& e)
			{
				GMSEC_DEBUG << e.what();
				GMSEC_DEBUG << "Ignoring native ActiveMQ message on " << getInfo(message).c_str();
				return;
			}


			if (msgKind == Message::REPLY && (!requestSpecs.useSubjectMapping && requestSpecs.exposeReplies))
			{
				(void) connection->handleCmsReply(message, logStackTrace);
			}

			if (msgKind == Message::REPLY && requestSpecs.exposeReplies == false)
			{
				return;
			}

			decaf::util::concurrent::Lock lock(&queue);
			queue.push(message->clone());
			queue.notifyAll();
		}
		catch (cms::CMSException &e)
		{
			std::string extra;
			if (logStackTrace)
			{
				extra = "\n" + e.getStackTraceString();
			}
			GMSEC_WARNING << "ActiveMQ TopicListener: " << e.what() << extra.c_str();
		}
		catch (decaf::lang::Exception &e)
		{
			std::string extra;
			if (logStackTrace)
			{
				extra = "\n" + e.getStackTraceString();
			}
			GMSEC_WARNING << "ActiveMQ TopicListener: " << e.what() << extra.c_str();
		}
	}

	void setStackTrace (bool state)
	{
		logStackTrace = state;
	}

private:

	std::string getInfo(const cms::Message *m)
	{
		const cms::Destination *destination = m->getCMSDestination();
		int type = destination->getDestinationType();
		Rawbuf<256> buffer;
		if (type == cms::Destination::TOPIC || type == cms::Destination::TEMPORARY_TOPIC)
		{
			buffer.stream() << (type == cms::Destination::TOPIC ? "topic" : "temporary topic");
			const cms::Topic *tmp = dynamic_cast<const cms::Topic*>(destination);
			if (tmp)
			{
				buffer.stream() << ": " << tmp->getTopicName();
			}
		}
		else if (type == cms::Destination::QUEUE || type == cms::Destination::TEMPORARY_QUEUE)
		{
			const cms::Queue *tmp = dynamic_cast<const cms::Queue*>(destination);
			buffer.stream() << (type == cms::Destination::QUEUE ? "queue" : "temporary queue");
			if (tmp)
			{
				buffer.stream() << ": " << tmp->getQueueName();
			}
		}
		else
		{
			buffer.stream() << "type=" << type;
		}
		return buffer.str();
	}

	CMSConnection* connection;
	MessageQueue&  queue;
	RequestSpecs   requestSpecs;
	bool           logStackTrace;
};


/**
* @brief the generic listener for replies.
*/

class ReplyListener : public cms::MessageListener
{
public:
	ReplyListener(CMSConnection *conn)
			: connection(conn), logStackTrace(false) { }
	~ReplyListener() throw () { }

	virtual void onMessage(const cms::Message *message) throw ()
	{
		if (connection)
		{
			(void) connection->handleCmsReply(message, logStackTrace);
		}
	}

	void setStackTrace (bool state)
	{
		logStackTrace = state;
	}

private:
	CMSConnection *connection;
	bool logStackTrace;
};




/**
* @fn mwConfig (Config *config, const char *key, string &out, bool requirePrefix)
* @param config The gmsec::Config
* @param key The configuration parameter name
* @param out The configuration parameter value (output)
* @param requirePrefix If true, parameter name must be prefixed with MW-
* @brief Retrieve a configuration parameter value.
*/

enum PrefixConfig
{
	PREFIX_OPTIONAL,
	PREFIX_REQUIRED,
	PREFIX_NONE
};


static void mwConfig(const Config& config, const char* key, std::string& out, PrefixConfig prefix = PREFIX_OPTIONAL)
{
	const char* value = 0;

	if (prefix != PREFIX_NONE)
	{
		std::string prefixedKey = std::string("MW-") + key;
		value = config.getValue(prefixedKey.c_str());
	}

	if (value != 0)
	{
		out = value;
	}
	else if (prefix != PREFIX_REQUIRED)
	{
		value = config.getValue(key);
		if (value != 0)
		{
			out = value;
		}
	}
}


gmsec_amqcms::SubscriptionInfo::SubscriptionInfo(cms::MessageConsumer *c)
		:
		consumer(c),
		extra(0)
{
}


gmsec_amqcms::SubscriptionInfo::~SubscriptionInfo()
{
	try
	{
		consumer->stop();
		delete consumer->getMessageListener();
		delete consumer;
		delete extra;
	}
	catch (cms::CMSException &e)
	{
		e.printStackTrace();
	}
}


void gmsec_amqcms::SubscriptionInfo::setExtra(SubscriptionInfo *info)
{
	extra = info;
}


CMSTransportListener::CMSTransportListener(CMSConnection* conn, bool issueFailoverEvents)
	: connection(conn),
	  failoverEvents(issueFailoverEvents)
{
}


CMSTransportListener::~CMSTransportListener()
{
	connection = 0;
}


void CMSTransportListener::onCommand(const decaf::lang::Pointer<activemq::commands::Command> command)
{
	if (command->isBrokerInfo())
	{
		const activemq::commands::BrokerInfo* brokerInfo = dynamic_cast<const activemq::commands::BrokerInfo*>(command.get());

		if (brokerInfo != NULL)
		{
			GMSEC_VERBOSE << "Connected to: " << brokerInfo->getBrokerURL().c_str();

			connection->getExternal().setConnectionEndpoint(brokerInfo->getBrokerURL());
		}
	}
}


void CMSTransportListener::transportInterrupted()
{
	if (connection)
	{
		Status status1(CONNECTION_ERROR, CONNECTION_LOST, "Connection broken");
		GMSEC_WARNING << status1.get();
		connection->getExternal().dispatchEvent(Connection::CONNECTION_BROKEN_EVENT, status1);

		if (failoverEvents)
		{
			Status status2(CONNECTION_ERROR, CONNECTION_RECONNECT, "Attempting to reconnect");
			GMSEC_WARNING << status2.get();
			connection->getExternal().dispatchEvent(Connection::CONNECTION_RECONNECT_EVENT, status2);
		}
	}
}


void CMSTransportListener::transportResumed()
{
	if (connection)
	{
		Status status(NO_ERROR_CLASS, CONNECTION_CONNECTED, "Connection established");
		GMSEC_INFO << status.get();
		connection->getExternal().dispatchEvent(Connection::CONNECTION_SUCCESSFUL_EVENT, status);
	}
}


CMSExceptionListener::CMSExceptionListener(CMSConnection* conn)
	: connection(conn)
{
}


CMSExceptionListener::~CMSExceptionListener()
{
	connection = 0;
}


void CMSExceptionListener::onException(const cms::CMSException& e)
{
	if (connection)
	{
		Status status(CONNECTION_ERROR, OTHER_ERROR_CODE, e.what());
		GMSEC_ERROR << status.get();
		connection->getExternal().dispatchEvent(Connection::CONNECTION_EXCEPTION_EVENT, status);
	}
}


#ifndef ACTIVEMQ_CMS_V2

class CMSMiddleware : public Middleware
{
public:
	CMSMiddleware()
	{
		activemq::library::ActiveMQCPP::initializeLibrary();

		//GMSEC_INFO << "Initialized ActiveMQ CMS";
	}

	~CMSMiddleware()
	{
		//GMSEC_INFO << "Finalizing ActiveMQ CMS";
		try
		{
			activemq::library::ActiveMQCPP::shutdownLibrary();
		}
		catch (const decaf::lang::Exception &e)
		{
			GMSEC_ERROR << "Unable to finalize ActiveMQ CPP library: " << e.what();
		}
	}
};

#endif /* ACTIVEMQ_CMS_V2 */



static Mutex &getClassMutex()
{
	static Mutex mutex;
	return mutex;
}


CMSConnection::CMSConnection(const Config& config)
	: m_cmsFactory(),
	  m_connection(),
	  m_publishSession(),
	  m_requestReplyDestination(),
	  m_requestReplyProducer(),
	  m_replyConsumer(),
	  m_subscriptions(),
	  m_queue(),
	  m_uniqueFilter(),
	  m_brokerURI(),
	  m_connClientId(),
	  m_username(),
	  m_password(),
	  m_keystore(),
	  m_keystorePassword(),
	  m_truststore(),
	  m_truststorePassword(),
	  m_useFilter(true),
	  m_reportFailoverEvent(false),
	  m_requestSpecs(),
	  m_mwinfo(),
	  m_transportListener(0),
	  m_exceptionListener(0)
{
	static bool initialized = false;

	if (!initialized)
	{
		AutoMutex hold(getClassMutex());
		if (!initialized)
		{
#if ACTIVEMQ_CMS_V3
			try
			{
				Middleware::addMiddleware(ACTIVEMQVERSIONSTRING, new CMSMiddleware());
				initialized = true;
			}
			catch (decaf::lang::Exception &e)
			{
				GMSEC_ERROR << "Unable to initialize ActiveMQCPP library : " << e.what();
				throw Exception(MIDDLEWARE_ERROR, INVALID_CONNECTION, e.what());
			}
#else /* ACTIVEMQ_CMS_V3 */
			/* ActiveMQ CMS 2 did not require this initialization */
			initialized = true;
#endif /* ACTIVEMQ_CMS_V3 */
		}
	}

	// set default broker
	m_brokerURI = "tcp://127.0.0.1:61616";

	std::string filterToggle;

	// Try to get the needed values out of the config object.
	mwConfig(config, "server", m_brokerURI);

	// Try to get the clientID for the cms::Connection
	mwConfig(config, "connection-client-id", m_connClientId);

	// SEC-* parameters are processed first
	mwConfig(config, "SEC-AUTH-NAME", m_username, PREFIX_NONE);
	// First try the generic password
	mwConfig(config, "SEC-COMMON-KEY", m_password, PREFIX_NONE);
	if (!m_password.empty())
	{
		m_keystorePassword   = m_password;
		m_truststorePassword = m_password;
	}

	mwConfig(config, "SEC-AUTH-KEY", m_password, PREFIX_NONE);
	mwConfig(config, "SEC-SSL-KEYSTORE", m_keystore, PREFIX_NONE);
	mwConfig(config, "SEC-SSL-KEYSTORE-KEY", m_keystorePassword, PREFIX_NONE);
	mwConfig(config, "SEC-SSL-TRUSTSTORE", m_truststore, PREFIX_NONE);
	mwConfig(config, "SEC-SSL-TRUSTSTORE-KEY", m_truststorePassword, PREFIX_NONE);

	mwConfig(config, "username", m_username);
	mwConfig(config, "password", m_password);
	mwConfig(config, "keystore", m_keystore);
	mwConfig(config, "keystorePassword", m_keystorePassword);
	mwConfig(config, "truststore", m_truststore);
	mwConfig(config, "truststorePassword", m_truststorePassword);
	mwConfig(config, "filter-dups", filterToggle);

	// Check if failover support is being requested
	const char* value = config.getValue("server");
	if (value)
	{
		std::string serverValue = StringUtil::stringToLower(value);
		size_t pos = serverValue.find("failover");
		m_reportFailoverEvent = (pos != std::string::npos);
	}

	m_useFilter = filterToggle.empty() || StringUtil::stringEqualsIgnoreCase(filterToggle.c_str(), "yes");

#if ACTIVEMQ_CMS_V3
	// Only available in ActiveMQ CMS 3
	if (!m_keystore.empty() && !m_keystorePassword.empty())
	{
		decaf::lang::System::setProperty("decaf.net.ssl.keyStore", m_keystore);
		decaf::lang::System::setProperty("decaf.net.ssl.keyStorePassword", m_keystorePassword);
	}

	if (!m_truststore.empty() && !m_truststorePassword.empty())
	{
		decaf::lang::System::setProperty("decaf.net.ssl.trustStore", m_truststore);
		decaf::lang::System::setProperty("decaf.net.ssl.trustStorePassword", m_truststorePassword);
	}
#endif
}



CMSConnection::~CMSConnection()
{
	cleanup();
}


/*
    Remove all of the active subscriptions and messages which have been
    received.  Close Sessions/Connection.
*/
void CMSConnection::cleanup()
{
	if (m_connection.get() == NULL)
	{
		return;
	}

//DMW -- The transport listener will be set to a 'do nothing' listener by the ActiveMQ CMS library when
//       connection->close() is called below.
//
//       Because of such, it is conceivable that we do not have to remove our registered transport listener.
//
//       On occasion, some of our Unit Tests misfire (fail) in the following frame:
//       C  [libactivemq-cpp.so.18+0xb463af]  activemq::transport::TransportFilter::onCommand(decaf::lang::Pointer<activemq::commands::Command, decaf::util::concurrent::atomic::AtomicRefCounter>)+0xf
//
#if 0
	if (m_transportListener)
	{
		activemq::core::ActiveMQConnection* amqConnection = dynamic_cast<activemq::core::ActiveMQConnection*>(m_connection.get());

		if (amqConnection)
		{
			amqConnection->removeTransportListener(m_transportListener);
		}
	}
	if (m_exceptionListener)
	{
		m_connection->setExceptionListener(NULL);
	}
#endif

	try
	{
		if (m_replyConsumer.get())
		{
			m_replyConsumer->stop();
			delete m_replyConsumer->getMessageListener();
			m_replyConsumer.reset();
		}

		if (m_publishSession.get())
		{
			m_publishSession->stop();
			m_publishSession->close();
			m_publishSession.reset();
		}

		m_requestReplyDestination.reset();
		m_requestReplyProducer.reset();

		if (m_connection.get())
		{
			m_connection->stop();
			m_connection->close();
			m_connection.reset();
		}

		for (std::map<std::string, gmsec_amqcms::SubscriptionInfo*>::iterator i = m_subscriptions.begin(); i != m_subscriptions.end(); ++i)
		{
			delete i->second;
		}
		m_subscriptions.clear();
	}
	catch (cms::CMSException &e)
	{
		e.printStackTrace();
	}

	// Destroy messages remaining in the queue
	{
		decaf::util::concurrent::Lock lock(&m_queue);

		cms::Message* cmsMessage;
		while ((cmsMessage = m_queue.pop()) != 0)
		{
			delete cmsMessage;
		}
	}

	delete m_transportListener;
	m_transportListener = 0;

	delete m_exceptionListener;
	m_exceptionListener = 0;

	Status tmpStatus(NO_ERROR_CLASS, CONNECTION_LOST, "Connection disconnected");
	GMSEC_INFO << tmpStatus.get();
	getExternal().dispatchEvent(Connection::CONNECTION_BROKEN_EVENT, tmpStatus);
}


const char* CMSConnection::getLibraryVersion()
{
	return ACTIVEMQVERSIONSTRING;
}


const char* CMSConnection::getMWInfo()
{
	if (m_mwinfo.empty())
	{
		std::ostringstream strm;

		if (m_connection.get() != NULL)
		{
#ifdef WIN32
			// The CMS::Connection::getClientID() returns an std::string from the
			// ActiveMQ C++ library; the returned string may not be able to be
			// deallocated by this middleware wrapper library when operating under
			// Windoze, so we avoid calling it, and instead, use m_connClientId (if
			// available).
			//
			strm << getLibraryRootName();

			if (!m_connClientId.empty())
			{
				strm << ": " << m_connClientId.c_str();
			}
#else
			strm << getLibraryRootName() << ": " << m_connection->getClientID();
#endif
		}
		else
		{
			strm << getLibraryRootName() << ": <no client ID... no connection>";
		}

		m_mwinfo = strm.str();
	}

	return m_mwinfo.c_str();
}


void CMSConnection::mwConnect()
{
	// Initialize the request specs
	m_requestSpecs = getExternal().getRequestSpecs();

	try
	{
		// ensure we are connected to broker

		if (!m_cmsFactory.get())
		{
			m_cmsFactory.reset(cms::ConnectionFactory::createCMSConnectionFactory(m_brokerURI));
		}

		if (!m_username.empty() && !m_password.empty())
		{
			m_connection.reset(m_cmsFactory->createConnection(m_username, m_password));
		}
		else
		{
			m_connection.reset(m_cmsFactory->createConnection());
		}

		// Set the clientID (Needed for durable subscribers to persist between Connections)
		if (!m_connClientId.empty())
		{
			m_connection->setClientID(m_connClientId);
		}

		m_exceptionListener = new CMSExceptionListener(this);
		m_connection->setExceptionListener(m_exceptionListener);

		activemq::core::ActiveMQConnection* amqConnection = dynamic_cast<activemq::core::ActiveMQConnection*>(m_connection.get());
		if (amqConnection)
		{
			m_transportListener = new CMSTransportListener(this, m_reportFailoverEvent);
			amqConnection->addTransportListener(m_transportListener);
		}

		m_publishSession.reset(m_connection->createSession());

		if (m_requestSpecs.requestReplyEnabled && m_requestSpecs.useSubjectMapping)
		{
			m_requestReplyDestination.reset(m_publishSession->createTopic(m_requestSpecs.replySubject));
			m_requestReplyProducer.reset(m_publishSession->createProducer(m_requestReplyDestination.get()));
			m_replyConsumer.reset(m_publishSession->createConsumer(m_requestReplyDestination.get()));
			m_replyConsumer->setMessageListener(new ReplyListener(this));
		}

		m_connection->start();

		if (m_transportListener)
		{
			m_transportListener->transportResumed();
		}
	}
	catch (cms::CMSException &e)
	{
		// check if this is an invalid access exception
		std::string message = e.what();  // call on method that does NOT return an std::string!!!

		size_t p = message.find("Message: User name or password is invalid.");
		if (p != std::string::npos) 
		{
			// skip the "Message: "
			size_t from = p + 9;

			// drop after "Cause: "
			size_t to = message.find("Cause: ");
			if (to == std::string::npos)
			{
				to = message.length();
			}

			message = message.substr(from, to - from);
			GMSEC_VERBOSE << message.c_str();
			throw Exception(POLICY_ERROR, USER_ACCESS_INVALID, message.c_str());
		}
		else
		{
			std::ostringstream oss;
			oss << "Error connecting to middleware broker [" << message << "]";
			GMSEC_VERBOSE << message.c_str();
			throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, INVALID_CONNECTION, oss.str().c_str());
		}
	}
}


void CMSConnection::mwDisconnect()
{
	try
	{
		cleanup();
	}
	catch (cms::CMSException &e)
	{
		GMSEC_ERROR << "Error disconnecting";
	}
}


gmsec_amqcms::SubscriptionInfo* CMSConnection::makeSubscriptionInfo(const std::string& in, const Config& config)
{
	GMSEC_VERBOSE << "makeSubscriptionInfo(" << in.c_str() << ')';

	StdUniquePtr<cms::Topic>           topic(m_publishSession->createTopic(in));
	StdUniquePtr<cms::MessageConsumer> consumer;

	if (config.getBooleanValue(AMQ_DURABLE_SUBSCRIBE, false))
	{
		const char* clientId    = config.getValue(AMQ_DURABLE_CLIENT_ID);
		const char* msgSelector = config.getValue(AMQ_DURABLE_MSG_SELECTOR);

		if (!clientId)
		{
			GMSEC_ERROR << "No client ID specified for durable subscription";
			return NULL;
		}
		// Message selector is not required, if not present, populate with empty string
		if (!msgSelector)
		{
			msgSelector = "";
		}

		consumer.reset(m_publishSession->createDurableConsumer(topic.get(), clientId, msgSelector));
	}
	else
	{
		consumer.reset(m_publishSession->createConsumer(topic.get()));
	}

	TopicListener* listener = new TopicListener(this, m_queue, m_requestSpecs);
	consumer->setMessageListener(listener);

	gmsec_amqcms::SubscriptionInfo* info = new gmsec_amqcms::SubscriptionInfo(consumer.release());
	return info;
}


void CMSConnection::mwSubscribe(const char *subject0, const Config &config)
{
	std::string subject(subject0);
	std::map<std::string, gmsec_amqcms::SubscriptionInfo*>::const_iterator i = m_subscriptions.find(subject);
	if (i != m_subscriptions.end())
	{
		GMSEC_VERBOSE << "Duplicate subscription";
		throw Exception(MIDDLEWARE_ERROR, INVALID_SUBJECT_NAME, "Duplicate subscription");
	}

	try
	{
		// Setup Subscription(s)
		// Need to turn X.> into subscriptions to X.* and X.*.>
		// string cmsExtra;
		size_t p = subject.length() - 1;
		if (subject[p] == '>')
		{
			// cmsExtra = subject.replace(p, 1, "*");
			subject = subject.replace(p, 1, "*.>");
		}
		else if (subject[p] == '+')
		{
			subject = subject.replace(p, 1, ">");
		}
		gmsec_amqcms::SubscriptionInfo* info = makeSubscriptionInfo(subject, config);
		if (info != NULL)
		{
			m_subscriptions[subject0] = info;
		}
		else
		{
			GMSEC_VERBOSE << "Invalid subscription configuration arguments";
			throw Exception(MIDDLEWARE_ERROR, INVALID_CONFIG_VALUE, "Invalid subscription configuration arguments");
		}

		// if (cmsExtra.length() > 0)
		// {
		// 	info->setExtra(makeSubscriptionInfo(cmsExtra, config));
		// }
	}
	catch (cms::CMSException &e)
	{
		std::string message(e.what());
		// check if this is an authorization exception
		// check if this is an unauthorized exception
		size_t p = message.find(" is not authorized to read from:");
		if (p != std::string::npos)
		{
			size_t from = message.find("Message: ");
			// skip the "Message: "
			if (from == std::string::npos)
			{
				from = 0;
			}
			else
			{
				from += 9;
			}

			size_t to = message.find("Exception Class");
			if (to == std::string::npos)
			{
				to = message.length();
			}

			message = message.substr(from, to - from);
			GMSEC_VERBOSE << message.c_str();
			throw Exception(POLICY_ERROR, SUBSCRIBE_NOT_AUTHORIZED, message.c_str());
		}
		else
		{
			GMSEC_VERBOSE << message.c_str();
			throw Exception(MIDDLEWARE_ERROR, INVALID_CONNECTION, message.c_str());
		}
	}
}


void CMSConnection::mwUnsubscribe(const char* subject)
{
	std::map<std::string, SubscriptionInfo*>::iterator it = m_subscriptions.find(subject);

	if (it == m_subscriptions.end())
	{
		GMSEC_VERBOSE << "Not subscribed to subject";
		throw Exception(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Not subscribed to subject");
	}

	try
	{
		delete it->second;
		m_subscriptions.erase(it);

		GMSEC_DEBUG << "Unsubscribed from " << subject;
	}
	catch (cms::CMSException &e)
	{
		GMSEC_VERBOSE << e.what();
		throw Exception(MIDDLEWARE_ERROR, INVALID_CONNECTION, e.what());
	}
}


void CMSConnection::mwPublish(const Message& msg, const Config& config)
{
	std::string subject(msg.getSubject());

	try
	{
		// the Destinations/Producers could be cached...
		StdUniquePtr<cms::Destination>     destination(m_publishSession->createTopic(subject));
		StdUniquePtr<cms::MessageProducer> producer(m_publishSession->createProducer(destination.get()));
		StdUniquePtr<cms::Message>         cmsMsg;

		prepare(msg, cmsMsg);

		if (config.getBooleanValue(AMQ_DURABLE_PUBLISH, false))
		{
			int priority = 4;
			long long timeToLive = 30000;

			const char* priorityStr   = config.getValue(AMQ_DURABLE_PRIORITY);
			const char* timeToLiveStr = config.getValue(AMQ_DURABLE_TTL);

			if (priorityStr)
			{
				if (StringUtil::str2int(priority, priorityStr) != StringUtil::STR2NUM_SUCCESS)
				{
					GMSEC_WARNING << "Invalid 'durable-priority' value provided for publish operation. Defaulting to a priority of 4";
				}
			}
			else
			{
				GMSEC_DEBUG << "No priority specified for durable publication, defaulting to 4";
			}

			if (timeToLiveStr)
			{
				if (StringUtil::str2longlong(timeToLive, timeToLiveStr) != StringUtil::STR2NUM_SUCCESS)
				{
					GMSEC_WARNING << "Invalid 'durable-time-to-live' value provided for publish operation. Defaulting to 30000ms";
				}
			}
			else
			{
				GMSEC_DEBUG << "No time to live period specified for durable publication, defaulting to 30000ms";
			}

			producer->send(cmsMsg.get(), cms::DeliveryMode::PERSISTENT, priority, timeToLive);
		}
		else
		{
			producer->send(cmsMsg.get());
		}
	}
	catch (cms::CMSException &e)
	{
		std::string message = e.what();

		// check if this is an unauthorized exception
		size_t p = message.find(" is not authorized to write to:");
		if (p != std::string::npos)
		{
			size_t from = message.find("Message: ");
			// skip the "Message: "
			if (from == std::string::npos)
			{
				from = 0;
			}
			else
			{
				from += 9;
			}

			size_t to = message.find("Exception Class");
			if (to == std::string::npos)
			{
				to = message.length();
			}

			message = message.substr(from, to - from);
			GMSEC_VERBOSE << message.c_str();
			throw Exception(POLICY_ERROR, PUBLISH_NOT_AUTHORIZED, message.c_str());
		}
		else
		{
			GMSEC_VERBOSE << e.what();
			throw Exception(MIDDLEWARE_ERROR, INVALID_MSG, e.what());
		}
	}
}


static std::string generateID()
{
	static decaf::util::Random random(decaf::lang::System::nanoTime());
	AutoMutex hold(getClassMutex());
	std::ostringstream o;
	o << decaf::lang::Long::toHexString(random.nextLong());
	return o.str();
}


void CMSConnection::mwRequest(const Message& request, std::string& id)
{
	std::string subject(request.getSubject());

	id = generateID();

	StdUniquePtr<cms::Message> cmsRequest;

	MessageBuddy::getInternal(request).addField(GMSEC_REPLY_UNIQUE_ID_FIELD, id.c_str());

	try
	{
		if (!m_requestSpecs.useSubjectMapping)
		{
			MessageBuddy::getInternal(request).getDetails().setBoolean(GMSEC_REQ_RESP_BEHAVIOR, true);
		}

		prepare(request, cmsRequest);

		if (m_requestSpecs.useSubjectMapping)
		{
			cmsRequest->setCMSReplyTo(m_requestReplyDestination.get());
		}

		cmsRequest->setCMSType(CMS_TYPE_REQUEST);
		cmsRequest->setCMSCorrelationID(id);

		StdUniquePtr<cms::Destination>     destination(m_publishSession->createTopic(subject));
		StdUniquePtr<cms::MessageProducer> producer(m_publishSession->createProducer(destination.get()));

		producer->send(cmsRequest.get());
	}
	catch (cms::CMSException &e)
	{
		GMSEC_VERBOSE << e.what();
		throw Exception(MIDDLEWARE_ERROR, INVALID_MSG, e.what());
	}
}


void CMSConnection::mwReply(const Message& request, const Message& reply)
{
	// Get the Request's Unique ID, and put it into a field in the Reply
	std::string uniqueID = getExternal().getReplyUniqueID(request);

	if (uniqueID.empty())
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Request does not contain unique ID field");
	}

	MessageBuddy::getInternal(reply).addField(GMSEC_REPLY_UNIQUE_ID_FIELD, uniqueID.c_str());

	try
	{
		StdUniquePtr<cms::Message> cmsReply;

		prepare(reply, cmsReply);

		cmsReply->setCMSType(CMS_TYPE_REPLY);
		cmsReply->setCMSCorrelationID(uniqueID.c_str());

		Value*            value   = MessageBuddy::getInternal(request).getDetails().getOpaqueValue(tag::REPLY_TO);
		CMSDestination*   dest    = dynamic_cast<CMSDestination*>(value);
		cms::Destination* replyTo = (dest ? dest->getReplyTo() : NULL);

		StdUniquePtr<cms::MessageProducer> replyProducer(m_publishSession->createProducer(replyTo));

		replyProducer->send(cmsReply.get());
	}
	catch (cms::CMSException &e)
	{
		GMSEC_VERBOSE << e.what();
		throw Exception(MIDDLEWARE_ERROR, INVALID_CONNECTION, e.what());
	}

	GMSEC_DEBUG << "Reply sent with subject " << reply.getSubject();
}


void CMSConnection::mwReceive(Message*& msg, GMSEC_I32 timeout)
{
	bool done = false;

	// initialize msg
	msg = NULL;

	while (!done)
	{
		// free msg
		if (msg != NULL)
		{
			delete msg;
		}			

		mwReceiveAux(msg, timeout);

		if (!msg)
		{
			// error already noted
			break;
		}

		if (m_useFilter)
		{
			try
			{
				const StringField& idField  = msg->getStringField("UNIQUE-ID");
				const char*        uniqueID = idField.getValue();

				if (m_uniqueFilter.update(uniqueID))
				{
					// found a unique message
					done = true;
				}
			}
			catch (Exception& e)
			{
				// No UNIQUE-ID field (tracking turned off?)
				done = true;
			}
		}
		else
		{
			// Filtering has not been enabled
			done = true;
		}
	}
}


void CMSConnection::mwReceiveAux(Message*& msg, GMSEC_I32 timeout)
{
	double start_s;

	// initialize message to NULL
	msg = NULL;

	if (timeout != GMSEC_WAIT_FOREVER)
	{
		start_s = TimeUtil::getCurrentTime_s();
	}

	bool done = false;
	while (!done)
	{
		try
		{
			decaf::util::concurrent::Lock lock(&m_queue);

			if (!m_queue.empty())
			{
				// have a message, don't need to wait
			}
			else if (timeout == GMSEC_WAIT_FOREVER)
			{
				m_queue.wait();
			}
			else
			{
				double elapsed_ms = 1000 * (TimeUtil::getCurrentTime_s() - start_s);
				long cmsTimeout = (long) (timeout - elapsed_ms);
				if (cmsTimeout > 0)
				{
					m_queue.wait(cmsTimeout);
				}
				else
				{
					// timeout (no error)
					done = true;
				}
			}

			StdUniquePtr<cms::Message> popped(m_queue.pop());
			if (popped.get())
			{
				done = true;
				unload(popped.get(), msg);
			}
		}
		catch (decaf::lang::Exception &e)
		{
			GMSEC_VERBOSE << e.what();
			throw Exception(MIDDLEWARE_ERROR, INVALID_MSG, e.what());
		}
	}
}


static const char* kindToCMSType(Message::MessageKind kind)
{
	const char* cmsType = "VOID";

	switch (kind)
	{
	case Message::PUBLISH:
		return CMS_TYPE_PUBLISH;

	case Message::REQUEST:
		return CMS_TYPE_REQUEST;

	case Message::REPLY:
		return CMS_TYPE_REPLY;
	}

	return cmsType;
}


static Message* parseProperties(const cms::Message& cmsMessage, Message::MessageKind msgKind, const Config& msgConfig, ValueMap& meta)
{
	StdUniquePtr<Message> message(new Message("BOGUS.TOPIC", msgKind, msgConfig));

	std::string subject;

	try
	{
		bool haveSubject = false;

		std::string allTypes;
		std::map<std::string, Field::FieldType> typeMap;

		if (cmsMessage.propertyExists(GMSEC_PROP_TYPES))
		{
			allTypes = cmsMessage.getStringProperty(GMSEC_PROP_TYPES);

			if (DEBUG_UNLOAD)
			{
				GMSEC_DEBUG << "parseProperties: allTypes=" << allTypes.c_str();
			}

			std::vector<std::string> types = StringUtil::split(allTypes, ',');

			for (size_t i = 0; i < types.size(); ++i)
			{
				std::vector<std::string> idType = StringUtil::split(types[i], '=');

				if (idType.size() == 2)
				{
					try
					{
						typeMap[idType[0]] = lookupFieldType(idType[1].c_str());
					}
					catch (Exception& e)
					{
						GMSEC_WARNING << e.what() << " -- invalid idType: " << types[i].c_str();
					}
				}
				else
				{
					GMSEC_WARNING << "invalid idType: " << types[i].c_str();
				}
			}
		}

		std::vector<std::string> properties(cmsMessage.getPropertyNames());

		if (DEBUG_UNLOAD)
		{
			GMSEC_DEBUG << "CMS message has " << properties.size() << " properties";
		}

		for (std::vector<std::string>::iterator i = properties.begin(); i != properties.end(); ++i)
		{
			std::string key(*i);
			std::string value(cmsMessage.getStringProperty(key));

			if (DEBUG_UNLOAD)
			{
				GMSEC_DEBUG << "\t" << key.c_str() << " => " << value.c_str();
			}

			if (key == GMSEC_PROP_TYPES)
			{
				// already handled
			}
			else if (key == GMSEC_SUBJECT_PROPERTY)
			{
				subject     = value;
				haveSubject = true;
			}
			else
			{
				std::map<std::string, Field::FieldType>::iterator found = typeMap.find(key);

				const char* name  = key.c_str();
				Field*      field = NULL;

				if (found == typeMap.end() || found->second == Field::STRING_TYPE)
				{
					field = new StringField(name, value.c_str());

					meta.setString(name, value);
				}
				else if (found->second == Field::I32_TYPE)
				{
					GMSEC_I32 value = cmsMessage.getIntProperty(key);

					field = new I32Field(name, value);

					meta.setI32(name, value);
				}
				else if (found->second == Field::F64_TYPE)
				{
					GMSEC_F64 value = cmsMessage.getDoubleProperty(key);

					field = new F64Field(name, value);

					meta.setF64(name, value);
				}
				else if (found->second == Field::BOOL_TYPE)
				{
					bool value = cmsMessage.getBooleanProperty(key);

					field = new BooleanField(name, value);

					meta.setBoolean(name, value);
				}
				else
				{
					GMSEC_WARNING << "parseProperties: unhandled type " << found->second;

					field = new StringField(name, value.c_str());

					meta.setString(name, value);
				}

				if (field)
				{
					field->isHeader(true);

					message->addField(*field);

					delete field;
				}
			}
		}

		// update the subject
		if (!haveSubject)
		{
			const cms::Topic* topic = dynamic_cast<const cms::Topic*>(cmsMessage.getCMSDestination());
			if (topic)
			{
				subject = topic->getTopicName();
			}
		}
	}
	catch (cms::CMSException& e)
	{
		GMSEC_VERBOSE << e.what();
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, e.what());
	}

	try
	{
		Message* tmp = message.get();

		MessageBuddy::getInternal(*tmp).setSubject(subject.c_str());
	}
	catch (Exception& e)
	{
		message.reset();
		throw e;
	}

	return message.release();
}


void CMSConnection::unload(const cms::Message* cmsMessage, Message*& gmsecMessage)
{
	gmsecMessage = 0;

	StdUniquePtr<Message> msgManager;
	Message::MessageKind  msgKind = lookupMessageKind(cmsMessage->getCMSType().c_str());

	ValueMap meta;

	msgManager.reset(parseProperties(*cmsMessage, msgKind, getExternal().getMessageConfig(), meta));

	// need to copy destination for reply
	if (msgKind == Message::REQUEST)
	{
		const cms::Destination* dest = cmsMessage->getCMSReplyTo();
		if (dest)
		{
			CMSDestination* destination = new CMSDestination(dest->clone());

			MessageBuddy::getInternal(*msgManager.get()).getDetails().setOpaqueValue(tag::REPLY_TO, destination);
		}
	}

#ifdef ACTIVEMQ_USE_MSG_RESET
	// For ActiveMQ C++ 3.7.1 (and possibly for newer releases), we have to reset
	// the incoming bytes-message so we can read it.  Otherwise the message will
	// be marked as write-only, and any attempt to access attributes from it will
	// generate an exception.
	//
	// This workaround also functions for AMQ C++ 3.4.0, however to be consistent
	// with previous releases of the API, we are only applying this workaround for
	// AMQ C++ v3.7.1.
	//
	// On the date this note was written (2013-08-29), it was unknown if the need
	// to reset the message is a functional requirement for AMQ C++ 3.7.1, or a bug.
	//
	cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>(const_cast<cms::Message*>(cmsMessage));

	if (bytesMessage)
	{
		bytesMessage->reset();
	}
#else
	const cms::BytesMessage* bytesMessage = dynamic_cast<const cms::BytesMessage*>(cmsMessage);
#endif

	if (bytesMessage)
	{
		int bytes = bytesMessage->getBodyLength();
		GMSEC_U8* p = (GMSEC_U8 *) bytesMessage->getBodyBytes();
		DataBuffer buffer(p, bytes, true);

		Status result = getExternal().getPolicy().unpackage(*msgManager.get(), buffer, meta);

		if (result.isError())
		{
			throw Exception(result);
		}

		if (msgKind == Message::REPLY)
		{
			Message* tmp = msgManager.get();

			getExternal().updateReplySubject(tmp);
		}
	}
	else
	{
		// we could add support for other CMS message types...
		GMSEC_VERBOSE << "Received cms::Message is not a BytesMessage";
		throw Exception(MIDDLEWARE_ERROR, INVALID_MSG, "Received cms::Message is not a BytesMessage");
	}

	gmsecMessage = msgManager.release();

	if (msgKind == Message::REQUEST && !m_requestSpecs.useSubjectMapping && cmsMessage->getCMSReplyTo() == NULL)
	{
		ValueMap& msgMeta = MessageBuddy::getInternal(*gmsecMessage).getDetails();

		msgMeta.setBoolean(GMSEC_REQ_RESP_BEHAVIOR, true);

		const StringField& field    = gmsecMessage->getStringField(GMSEC_REPLY_UNIQUE_ID_FIELD);
		const char*        uniqueID = field.getValue();

		msgMeta.setString(GMSEC_REPLY_UNIQUE_ID_FIELD, uniqueID);

		gmsecMessage->clearField(GMSEC_REPLY_UNIQUE_ID_FIELD);
	}

	// The following field appears courtesy of Apache Artemis; remove it!
	gmsecMessage->clearField("__AMQ_CID");
}


void CMSConnection::handleCmsReply(const cms::Message* cmsReply, bool logStackTrace)
{
	try
	{
		handleReply(cmsReply);
	}
	catch (cms::CMSException &e)
	{
		std::string extra;
		if (logStackTrace)
		{
			extra = "\n" + e.getStackTraceString();
		}
		GMSEC_WARNING << "ActiveMQ handleCmsReply: " << e.what() << extra.c_str();
	}
	catch (decaf::lang::Exception &e)
	{
		std::string extra;
		if (logStackTrace)
		{
			extra = "\n" + e.getStackTraceString();
		}
		GMSEC_WARNING << "ActiveMQ handleCmsReply: " << e.what() << extra.c_str();
	}
}


void CMSConnection::handleReply(const cms::Message* cmsMessage)
{
	Message* gmsecMessage = 0;

	unload(cmsMessage, gmsecMessage);

	getExternal().onReply(gmsecMessage);
}



static Status storeProperty(const Value& value, const std::string& name, cms::Message& cmsMessage, std::string& types)
{
	Status status;

	std::string type;

	if (value.isString())
	{
		std::string x;
		status = value.getString(x);
		if (!status.isError())
		{
			cmsMessage.setStringProperty(name, x);
		}
	}
	else if (value.isInteger())
	{
		GMSEC_I32 x = 0;
		type = "I32";
		status = value.getI32(x);
		if (!status.isError())
		{
			cmsMessage.setIntProperty(name, x);
		}
	}
	else if (value.isBoolean())
	{
		bool x = false;
		type = "BOOL";
		status = value.getBoolean(x);
		if (!status.isError())
		{
			cmsMessage.setBooleanProperty(name, x);
		}
	}
	else if (value.isReal())
	{
		GMSEC_F64 x = 0;
		type = "F64";
		status = value.getF64(x);
		if (!status.isError())
		{
			cmsMessage.setDoubleProperty(name, x);
		}
	}
	else
	{
		status.set(FIELD_ERROR, OTHER_ERROR_CODE, "Unable to convert type to CMS property");
	}

	if (!status.isError() && (type.length() > 0))
	{
		if (types.length() > 0)
		{
			types += ',';
		}
		types += name + '=' + type;
	}

	return status;
}


static Status storeProperties(ValueMap& header, cms::Message& cmsMessage)
{
	Status status;

	ValueMap::Iterator i;
	header.reset(i);

	std::string types;

	while (!status.isError())
	{
		const Value* value = i.next();

		if (!value)
		{
			break;
		}

		status = storeProperty(*value, i.getID(), cmsMessage, types);
	}

	if (types.length() > 0)
	{
		cmsMessage.setStringProperty(GMSEC_PROP_TYPES, types);
	}

	return status;
}


void CMSConnection::prepare(const Message& msg, StdUniquePtr<cms::Message>& cmsMessage)
{
	DataBuffer buffer;
	ValueMap header;

	Status result = getExternal().getPolicy().package(const_cast<Message&>(msg), buffer, header);

	if (result.isError())
	{
		throw Exception(result);
	}

	std::string subject = msg.getSubject();
	Message::MessageKind kind = msg.getKind();

	header.setString(GMSEC_SUBJECT_PROPERTY, subject.c_str());

	try
	{
		StdUniquePtr<cms::BytesMessage> bytesMessage(m_publishSession->createBytesMessage());
		bytesMessage->setCMSType(kindToCMSType(kind));
		bytesMessage->setBodyBytes(buffer.get(), buffer.size());

		storeProperties(header, *bytesMessage);

		cmsMessage.reset(bytesMessage.release());
	}
	catch (cms::CMSException &e)
	{
		GMSEC_VERBOSE << e.what();
		throw Exception(MSG_ERROR, INVALID_MSG, e.what());
	}
}
