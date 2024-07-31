/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file CMSConnection.cpp
 *
 * Contains the implementation for the ActiveMQ/Artemis implementation of ConnectionInterface.
 */

#include "CMSConnection.h"

#include "CMSDestination.h"

#include <gmsec5/ConfigOptions.h>

#include <gmsec5/internal/InternalConnection.h>
#include <gmsec5/internal/InternalMessageFactory.h>
#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/Middleware.h>
#include <gmsec5/internal/Rawbuf.h>
#include <gmsec5/internal/StringUtil.h>

#include <gmsec5/Connection.h>
#include <gmsec5/GmsecException.h>

#include <gmsec5/util/Log.h>

#include <activemq/commands/BrokerInfo.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/library/ActiveMQCPP.h>

#include <decaf/lang/System.h>
#include <decaf/lang/Long.h>
#include <decaf/util/Random.h>

#include <sstream>
#include <string>
#include <vector>

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


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;

using namespace gmsec_amqcms;


static Field::Type lookupFieldType(const char* fieldTypeStr)
{
	Field::Type type;

	bool error = false;

	switch (fieldTypeStr[0])
	{
	case 'B':
		if (StringUtil::stringEquals(fieldTypeStr, "BOOL") || StringUtil::stringEquals(fieldTypeStr, "BOOLEAN"))
		{
			type = Field::Type::BOOL;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "BIN"))
		{
			type = Field::Type::BINARY;
		}
		else
		{
			error = true;
		}
		break;

	case 'C':
		if (StringUtil::stringEquals(fieldTypeStr, "CHAR"))
		{
			type = Field::Type::CHAR;
		}
		else
		{
			error = true;
		}
		break;

	case 'D':
		if (StringUtil::stringEquals(fieldTypeStr, "DOUBLE"))
		{
			type = Field::Type::F64;
		}
		else
		{
			error = true;
		}
		break;

	case 'F':
		if (StringUtil::stringEquals(fieldTypeStr, "F32") || StringUtil::stringEquals(fieldTypeStr, "FLOAT"))
		{
			type = Field::Type::F32;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "F64"))
		{
			type = Field::Type::F64;
		}
		else
		{
			error = true;
		}
		break;

	case 'I':
		if (StringUtil::stringEquals(fieldTypeStr, "I16"))
		{
			type = Field::Type::I16;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "I32"))
		{
			type = Field::Type::I32;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "I64"))
		{
			type = Field::Type::I64;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "I8"))
		{
			type = Field::Type::I8;
		}
		else
		{
			error = true;
		}
		break;

	case 'L':
		if (StringUtil::stringEquals(fieldTypeStr, "LONG"))
		{
			type = Field::Type::I32;
		}
		else
		{
			error = true;
		}
		break;

	case 'S':
		if (StringUtil::stringEquals(fieldTypeStr, "STRING"))
		{
			type = Field::Type::STRING;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "SHORT"))
		{
			type = Field::Type::I16;
		}
		else
		{
			error = true;
		}
		break;

	case 'U':
		if (StringUtil::stringEquals(fieldTypeStr, "U16") || StringUtil::stringEquals(fieldTypeStr, "USHORT"))
		{
			type = Field::Type::U16;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "U32") || StringUtil::stringEquals(fieldTypeStr, "ULONG"))
		{
			type = Field::Type::U32;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "U64"))
		{
			type = Field::Type::U64;
		}
		else if (StringUtil::stringEquals(fieldTypeStr, "U8"))
		{
			type = Field::Type::U8;
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
		throw GmsecException(FIELD_ERROR, UNKNOWN_FIELD_TYPE, oss.str().c_str());
	}

	return type;
}


static Message::Kind lookupMessageKind(const char* msgTypeStr)
{
	Message::Kind kind;

	if (StringUtil::stringEqualsIgnoreCase(msgTypeStr, CMS_TYPE_PUBLISH))
	{
		kind = Message::Kind::PUBLISH;
	}
	else if (StringUtil::stringEqualsIgnoreCase(msgTypeStr, CMS_TYPE_REQUEST))
	{
		kind = Message::Kind::REQUEST;
	}
	else if (StringUtil::stringEqualsIgnoreCase(msgTypeStr, CMS_TYPE_REPLY))
	{
		kind = Message::Kind::REPLY;
	}
	else
	{
		std::ostringstream oss;
		oss << "Unknown message kind: " << msgTypeStr;
		GMSEC_VERBOSE << oss.str().c_str();
		throw GmsecException(MSG_ERROR, UNKNOWN_MSG_TYPE, oss.str().c_str());
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
	TopicListener(CMSConnection* connection, CMSConnection::MessageQueue& q)
			: connection(connection), queue(q), logStackTrace(false) { }

	~TopicListener() throw () { }

	virtual void onMessage(const cms::Message* cmsMsg) throw ()
	{
		try
		{
			Message* tmp;
			connection->unload(cmsMsg, tmp);

			StdUniquePtr<Message> gmsecMsg;
			gmsecMsg.reset(tmp);

			if (gmsecMsg.get() != NULL)
			{
				if (gmsecMsg->getKind() == Message::Kind::REPLY)
				{
					connection->getExternal().onReply(new Message(*gmsecMsg.get()));
				}

				queue.put(gmsecMsg.release());
			}
		}
		catch (const GmsecException& e)
		{
			GMSEC_WARNING << "ActiveMQ TopicListener: " << e.what();
		}
		catch (const cms::CMSException& e)
		{
			std::string extra;
			if (logStackTrace)
			{
				extra = "\n" + e.getStackTraceString();
			}
			GMSEC_WARNING << "ActiveMQ TopicListener: " << e.what() << extra.c_str();
		}
		catch (const decaf::lang::Exception& e)
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

	CMSConnection*               connection;
	CMSConnection::MessageQueue& queue;
	bool                         logStackTrace;
};


gmsec_amqcms::SubscriptionInfo::SubscriptionInfo(cms::MessageConsumer* c)
	: consumer(c),
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
	catch (const cms::CMSException& e)
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
		connection->getExternal().dispatchEvent(Connection::Event::CONNECTION_BROKEN_EVENT, status1);

		if (failoverEvents)
		{
			Status status2(CONNECTION_ERROR, CONNECTION_RECONNECT, "Attempting to reconnect");
			GMSEC_WARNING << status2.get();
			connection->getExternal().dispatchEvent(Connection::Event::CONNECTION_RECONNECT_EVENT, status2);
		}
	}
}


void CMSTransportListener::transportResumed()
{
	if (connection)
	{
		Status status(NO_ERROR_CLASS, CONNECTION_CONNECTED, "Connection established");
		GMSEC_INFO << status.get();
		connection->getExternal().dispatchEvent(Connection::Event::CONNECTION_SUCCESSFUL_EVENT, status);
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
		connection->getExternal().dispatchEvent(Connection::Event::CONNECTION_EXCEPTION_EVENT, status);
	}
}


class CMSMiddleware : public Middleware
{
public:
	CMSMiddleware()
	{
		activemq::library::ActiveMQCPP::initializeLibrary();
	}

	~CMSMiddleware()
	{
		try
		{
			activemq::library::ActiveMQCPP::shutdownLibrary();
		}
		catch (const decaf::lang::Exception& e)
		{
			GMSEC_ERROR << "Unable to finalize ActiveMQ CPP library: " << e.what();
		}
	}
};


static Mutex &getClassMutex()
{
	static Mutex mutex;
	return mutex;
}


CMSConnection::CMSConnection(const Config& config)
	: m_cmsFactory(),
	  m_connection(),
	  m_session(),
	  m_requestReplyDestination(),
	  m_requestReplyProducer(),
	  m_replyConsumer(),
	  m_subscriptions(),
	  m_queue(MessageQueue::MAX_QUEUE_SIZE),
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
			try
			{
				Middleware::addMiddleware(ACTIVEMQVERSIONSTRING, new CMSMiddleware());
				initialized = true;
			}
			catch (const decaf::lang::Exception& e)
			{
				GMSEC_ERROR << "Unable to initialize ActiveMQCPP library : " << e.what();
				throw GmsecException(MIDDLEWARE_ERROR, INVALID_CONNECTION, e.what());
			}
		}
	}

	// Deduce the URI
	const char* defaultURI = "tcp://127.0.0.1:61616";

	m_brokerURI = config.getValue(GMSEC_MIDDLEWARE_SERVER, "");

	if (m_brokerURI.empty())
	{
		// URI not provided; check legacy configuration option
		m_brokerURI = config.getValue(GMSEC_SERVER, defaultURI);
	}

	// Check if failover support is being requested
	std::string serverValue = StringUtil::stringToLower(m_brokerURI);
	size_t pos = serverValue.find("failover");
	m_reportFailoverEvent = (pos != std::string::npos);

	// Try to get the clientID for the cms::Connection
	m_connClientId = config.getValue(AMQ_CONNECTION_CLIENT_ID, m_connClientId.c_str());

	m_username = config.getValue(AMQ_USERNAME, m_username.c_str());
	m_password = config.getValue(AMQ_PASSWORD, m_password.c_str());

	m_keystore           = config.getValue(AMQ_SSL_KEYSTORE, m_keystore.c_str());
	m_keystorePassword   = config.getValue(AMQ_SSL_KEYSTORE_PW, m_keystorePassword.c_str());
	m_truststore         = config.getValue(AMQ_SSL_TRUSTSTORE, m_truststore.c_str());
	m_truststorePassword = config.getValue(AMQ_SSL_TRUSTSTORE_PW, m_truststorePassword.c_str());

	// Check if a user is using legacy-style spelling of keystore and truststore password configuration options
	if (!m_keystore.empty() && m_keystorePassword.empty())
	{
		m_keystorePassword = config.getValue("mw-keystorepassword", m_keystorePassword.c_str());
	}
	if (!m_truststore.empty() && m_truststorePassword.empty())
	{
		m_truststorePassword = config.getValue("mw-truststorepassword", m_truststorePassword.c_str());
	}

	std::string filterToggle = config.getValue(AMQ_FILTER_DUPLICATE_MSGS, "");
	m_useFilter = filterToggle.empty() || StringUtil::stringEqualsIgnoreCase(filterToggle.c_str(), "yes");

	if (!m_keystore.empty() && !m_keystorePassword.empty())
	{
		decaf::lang::System::setProperty("decaf.net.ssl.keyStore", m_keystore);
		decaf::lang::System::setProperty("decaf.net.ssl.keyStorePassword", m_keystorePassword);
	}

	if (!m_truststore.empty())
	{
		decaf::lang::System::setProperty("decaf.net.ssl.trustStore", m_truststore);
		if (!m_truststorePassword.empty())
		{
			decaf::lang::System::setProperty("decaf.net.ssl.trustStorePassword", m_truststorePassword);
		}
	}
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

	try
	{
		if (m_replyConsumer.get())
		{
			m_replyConsumer->stop();
			delete m_replyConsumer->getMessageListener();
			m_replyConsumer.reset();
		}

		if (m_session.get())
		{
			m_session->stop();
			m_session->close();
			m_session.reset();
		}

		m_requestReplyDestination.reset();
		m_requestReplyProducer.reset();

		if (m_connection.get())
		{
			m_connection->stop();
			m_connection->close();
			m_connection.reset();
		}

		m_exceptionListener.reset();
		m_transportListener.reset();

		for (std::map<std::string, gmsec_amqcms::SubscriptionInfo*>::iterator i = m_subscriptions.begin(); i != m_subscriptions.end(); ++i)
		{
			delete i->second;
		}
		m_subscriptions.clear();
	}
	catch (const cms::CMSException& e)
	{
		e.printStackTrace();
	}

	// Destroy messages remaining in the queue
	while (!m_queue.empty())
	{
		Message* msg = m_queue.take();
		delete msg;
	}

	m_transportListener.release();
	m_exceptionListener.release();

	Status tmpStatus(NO_ERROR_CLASS, CONNECTION_LOST, "Connection disconnected");
	GMSEC_INFO << tmpStatus.get();
	getExternal().dispatchEvent(Connection::Event::CONNECTION_BROKEN_EVENT, tmpStatus);
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


static void handleCmsException(const cms::CMSException& e)
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
		throw GmsecException(POLICY_ERROR, USER_ACCESS_INVALID, message.c_str());
	}
	else
	{
		std::ostringstream oss;
		oss << "Error connecting to middleware broker [" << message << "]";
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, INVALID_CONNECTION, oss.str().c_str());
	}
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
			if (!m_reportFailoverEvent)
			{
				int  maxRetries     = getExternal().getMaxConnectionRetries();
				int  retryInterval  = getExternal().getConnectionRetryInterval();
				int  triesRemaining = maxRetries;
				bool haveError      = false;

				do
				{
					try
					{
						m_connection.reset(m_cmsFactory->createConnection());
						haveError = false;
					}
					catch (const cms::CMSException& e)
					{
						haveError = true;

						if (maxRetries == -1 || --triesRemaining > 0)
						{
							GMSEC_WARNING << "Failed to connect; will retry... [tries remaining: " << triesRemaining << "]";

							TimeUtil::millisleep(retryInterval);
						}
						else
						{
							handleCmsException(e);
						}
					}
				} while (haveError && (maxRetries == -1 || triesRemaining >= 0));
			}
			else
			{
				GMSEC_INFO << "Waiting to connect...";

				m_connection.reset(m_cmsFactory->createConnection());
			}
		}

		// Set the clientID (Needed for durable subscribers to persist between Connections)
		if (!m_connClientId.empty())
		{
			m_connection->setClientID(m_connClientId);
		}

		m_exceptionListener.reset(new CMSExceptionListener(this));
		m_connection->setExceptionListener(m_exceptionListener.get());

		activemq::core::ActiveMQConnection* amqConnection = dynamic_cast<activemq::core::ActiveMQConnection*>(m_connection.get());
		if (amqConnection)
		{
			m_transportListener.reset(new CMSTransportListener(this, m_reportFailoverEvent));
			amqConnection->addTransportListener(m_transportListener.get());
		}

		m_ackMode = getExternal().getConfig().getIntegerValue(AMQ_ACK_MODE, static_cast<int>(cms::Session::AUTO_ACKNOWLEDGE));

		if (m_ackMode < static_cast<int>(cms::Session::AUTO_ACKNOWLEDGE) || m_ackMode > static_cast<int>(cms::Session::INDIVIDUAL_ACKNOWLEDGE))
		{
			GMSEC_WARNING << "Illegal value given for " << AMQ_ACK_MODE << "; defaulting to AUTO_ACKNOWLEDGE (0)";
			m_ackMode = static_cast<int>(cms::Session::AUTO_ACKNOWLEDGE);
		}

		m_session.reset(m_connection->createSession( static_cast<cms::Session::AcknowledgeMode>(m_ackMode) ));

		m_connection->start();

		if (m_transportListener.get())
		{
			m_transportListener->transportResumed();
		}
	}
	catch (const cms::CMSException& e)
	{
		handleCmsException(e);
	}
}


void CMSConnection::mwDisconnect()
{
	try
	{
		cleanup();
	}
	catch (const cms::CMSException& e)
	{
		GMSEC_ERROR << "Error disconnecting";
	}
}


gmsec_amqcms::SubscriptionInfo* CMSConnection::makeSubscriptionInfo(const std::string& subject, const Config& config)
{
	GMSEC_VERBOSE << "makeSubscriptionInfo(" << subject.c_str() << ')';

	StdUniquePtr<cms::Destination>     dest;
	StdUniquePtr<cms::MessageConsumer> consumer;

	if (config.getBooleanValue(AMQ_DURABLE_SUBSCRIBE, false))
	{
		dest.reset(m_session->createTopic(subject));

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

		consumer.reset(m_session->createDurableConsumer(dynamic_cast<cms::Topic*>(dest.get()), clientId, msgSelector));
	}
#ifdef GMSEC_ARTEMIS
	else if (subject.find(ARTEMIS_MQ_TOPIC_DELIMITER) != std::string::npos)
	{
		// We have a subscription topic w/ a Fully Qualified Queue Name (FQQN)
		dest.reset(m_session->createQueue(subject));

		consumer.reset(m_session->createConsumer(dest.get()));
	}
#endif
	else
	{
		dest.reset(m_session->createTopic(subject));

		consumer.reset(m_session->createConsumer(dest.get()));
	}

	TopicListener* listener = new TopicListener(this, m_queue);
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
		throw GmsecException(MIDDLEWARE_ERROR, INVALID_SUBJECT_NAME, "Duplicate subscription");
	}

	try
	{
#ifdef GMSEC_ARTEMIS
		size_t p = subject.find(ARTEMIS_MQ_TOPIC_DELIMITER);

		if (p == std::string::npos)
		{
			p = subject.length() - 1;
		}
		else
		{
			--p;
		}
#else
		size_t p = subject.length() - 1;
#endif

		if (subject[p] == '>')
		{
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
			throw GmsecException(MIDDLEWARE_ERROR, INVALID_CONFIG_VALUE, "Invalid subscription configuration arguments");
		}
	}
	catch (const cms::CMSException& e)
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
			throw GmsecException(POLICY_ERROR, SUBSCRIBE_NOT_AUTHORIZED, message.c_str());
		}
		else
		{
			GMSEC_VERBOSE << message.c_str();
			throw GmsecException(MIDDLEWARE_ERROR, INVALID_CONNECTION, message.c_str());
		}
	}
}


void CMSConnection::mwUnsubscribe(const char* subject)
{
	std::map<std::string, SubscriptionInfo*>::iterator it = m_subscriptions.find(subject);

	if (it == m_subscriptions.end())
	{
		GMSEC_VERBOSE << "Not subscribed to subject";
		throw GmsecException(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Not subscribed to subject");
	}

	try
	{
		delete it->second;
		m_subscriptions.erase(it);

		GMSEC_DEBUG << "Unsubscribed from " << subject;
	}
	catch (const cms::CMSException& e)
	{
		GMSEC_VERBOSE << e.what();
		throw GmsecException(MIDDLEWARE_ERROR, INVALID_CONNECTION, e.what());
	}
}


void CMSConnection::mwPublish(const Message& msg, const Config& config)
{
	std::string subject(msg.getSubject());

	try
	{
		// the Destinations/Producers could be cached...
		StdUniquePtr<cms::Destination>     destination(m_session->createTopic(subject));
		StdUniquePtr<cms::MessageProducer> producer(m_session->createProducer(destination.get()));
		StdUniquePtr<cms::Message>         cmsMsg;

		prepare(msg, cmsMsg);

		if (config.getBooleanValue(AMQ_DURABLE_PUBLISH, false))
		{
			int       priority   = 4;
			long long timeToLive = 30000;   // 30 seconds

			priority   = config.getIntegerValue(AMQ_DURABLE_PRIORITY, priority);
			timeToLive = static_cast<long long>( config.getIntegerValue(AMQ_DURABLE_TTL, static_cast<int>(timeToLive)) );

			producer->send(cmsMsg.get(), cms::DeliveryMode::PERSISTENT, priority, timeToLive);
		}
		else
		{
			producer->send(cmsMsg.get());
		}
	}
	catch (const cms::CMSException& e)
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
			throw GmsecException(POLICY_ERROR, PUBLISH_NOT_AUTHORIZED, message.c_str());
		}
		else
		{
			GMSEC_VERBOSE << e.what();
			throw GmsecException(MIDDLEWARE_ERROR, INVALID_MSG, e.what());
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


void CMSConnection::mwRequest(const Message& request, const std::string& uniqueID)
{
	std::string subject(request.getSubject());

	StdUniquePtr<cms::Message> cmsRequest;

	try
	{
		prepare(request, cmsRequest);

		cmsRequest->setCMSType(CMS_TYPE_REQUEST);
		cmsRequest->setCMSCorrelationID(uniqueID);

		StdUniquePtr<cms::Destination>     destination(m_session->createTopic(subject));
		StdUniquePtr<cms::MessageProducer> producer(m_session->createProducer(destination.get()));

		producer->send(cmsRequest.get());
	}
	catch (const cms::CMSException& e)
	{
		GMSEC_VERBOSE << e.what();
		throw GmsecException(MIDDLEWARE_ERROR, INVALID_MSG, e.what());
	}
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
			if (msg->hasField("UNIQUE-ID"))
			{
				const char* uniqueID = msg->getStringValue("UNIQUE-ID");

				if (m_uniqueFilter.update(uniqueID))
				{
					// found a unique message
					done = true;
				}
			}
			else
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
	msg = NULL;

	if (timeout == GMSEC_WAIT_FOREVER)
	{
		msg = m_queue.take();
	}
	else
	{
		m_queue.poll(timeout, msg);
	}
}


void CMSConnection::mwAcknowledge(StdSharedPtr<MiddlewareInfo>& info)
{
	cms::Message* mwMsg = reinterpret_cast<cms::Message*>(info->m_mwMsg);

	if (mwMsg)
	{
		mwMsg->acknowledge();
		delete mwMsg;
	}
}


std::string CMSConnection::mwGetUniqueID()
{
	return generateID();
}


static const char* kindToCMSType(Message::Kind kind)
{
	const char* cmsType = "VOID";

	switch (kind)
	{
	case Message::Kind::PUBLISH:
		return CMS_TYPE_PUBLISH;

	case Message::Kind::REQUEST:
		return CMS_TYPE_REQUEST;

	case Message::Kind::REPLY:
		return CMS_TYPE_REPLY;
	}

	return cmsType;
}


static Message* parseProperties(const cms::Message& cmsMessage,
                                Message::Kind       msgKind,
                                MessageFactory&     factory,
                                const Config&       msgConfig,
                                ValueMap&           meta)
{
	StdUniquePtr<Message> message( MessageFactoryBuddy::getInternal(factory).newMessage() );

	message->setConfig(msgConfig);

	MessageBuddy::getInternal(*message.get()).setKind(msgKind);

	std::string subject;

	try
	{
		bool haveSubject = false;

		std::string allTypes;
		std::map<std::string, Field::Type> typeMap;

		if (cmsMessage.propertyExists(GMSEC_PROP_TYPES))
		{
			allTypes = cmsMessage.getStringProperty(GMSEC_PROP_TYPES);

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
					catch (const GmsecException& e)
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

		for (std::vector<std::string>::iterator i = properties.begin(); i != properties.end(); ++i)
		{
			std::string key(*i);
			std::string value(cmsMessage.getStringProperty(key));

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
				std::map<std::string, Field::Type>::iterator found = typeMap.find(key);

				const char* name  = key.c_str();
				StdUniquePtr<Field> field;

				if (found == typeMap.end() || found->second == Field::Type::STRING)
				{
					field.reset( new StringField(name, value.c_str(), true) );

					meta.setString(name, value);
				}
				else if (found->second == Field::Type::I32)
				{
					GMSEC_I32 value = cmsMessage.getIntProperty(key);

					field.reset( new I32Field(name, value, true) );

					meta.setI32(name, value);
				}
				else if (found->second == Field::Type::F64)
				{
					GMSEC_F64 value = cmsMessage.getDoubleProperty(key);

					field.reset( new F64Field(name, value, true) );

					meta.setF64(name, value);
				}
				else if (found->second == Field::Type::BOOL)
				{
					bool value = cmsMessage.getBooleanProperty(key);

					field.reset( new BooleanField(name, value, true) );

					meta.setBoolean(name, value);
				}
				else
				{
					GMSEC_WARNING << "parseProperties: unhandled type " << static_cast<int>(found->second);

					field.reset( new StringField(name, value.c_str(), true) );

					meta.setString(name, value);
				}

				if (field.get())
				{
					MessageBuddy::getInternal(*message).addField(*field.release(), false);
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
	catch (const cms::CMSException& e)
	{
		GMSEC_VERBOSE << e.what();
		throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, e.what());
	}

	try
	{
		Message* tmp = message.get();

		MessageBuddy::getInternal(*tmp).setSubject(subject.c_str());
	}
	catch (const GmsecException& e)
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
	Message::Kind         msgKind = lookupMessageKind(cmsMessage->getCMSType().c_str());

	ValueMap meta;

	msgManager.reset(parseProperties(*cmsMessage, msgKind, getExternal().getMessageFactory(), getExternal().getMessageConfig(), meta));

	// need to copy destination for reply
	if (msgKind == Message::Kind::REQUEST)
	{
		const cms::Destination* dest = cmsMessage->getCMSReplyTo();
		if (dest)
		{
			CMSDestination* destination = new CMSDestination(dest->clone());

			MessageBuddy::getInternal(*msgManager.get()).getDetails().setOpaqueValue(tag::REPLY_TO, destination);
		}
	}

	cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>(const_cast<cms::Message*>(cmsMessage));

	if (bytesMessage != NULL)
	{
		bytesMessage->reset();

		int bytes = bytesMessage->getBodyLength();
		GMSEC_U8* p = (GMSEC_U8 *) bytesMessage->getBodyBytes();
		DataBuffer buffer(p, bytes, true);

		Status result = getExternal().getPolicy().unpackage(*msgManager.get(), buffer, meta);

		if (result.hasError())
		{
			throw GmsecException(result);
		}
	}
	else
	{
		// we could add support for other CMS message types...
		GMSEC_VERBOSE << "Received cms::Message is not a BytesMessage";
		throw GmsecException(MIDDLEWARE_ERROR, INVALID_MSG, "Received cms::Message is not a BytesMessage");
	}

	gmsecMessage = msgManager.release();

	if (m_ackMode == static_cast<int>(cms::Session::INDIVIDUAL_ACKNOWLEDGE))
	{
		// Hook for allowing a client to acknowledge a CMS message
		StdSharedPtr<MiddlewareInfo> info( new MiddlewareInfo(this, reinterpret_cast<void*>(cmsMessage->clone()), NULL) );
		MessageBuddy::getInternal(*gmsecMessage).setMiddlewareInfo(info);
	}

#ifdef GMSEC_ARTEMIS
	// The following field appears courtesy of Apache Artemis; remove it!
	gmsecMessage->clearField("__AMQ_CID");
#endif

	MessageFactoryBuddy::getInternal(getExternal().getMessageFactory()).addMessageTemplate(*gmsecMessage);
	MessageFactoryBuddy::getInternal(getExternal().getMessageFactory()).identifyTrackingFields(*gmsecMessage);
}


static Status storeProperty(const Value& value, const std::string& name, cms::Message& cmsMessage, std::string& types)
{
	Status status;

	std::string type;

	if (value.isString())
	{
		std::string x;
		status = value.getString(x);
		if (!status.hasError())
		{
			cmsMessage.setStringProperty(name, x);
		}
	}
	else if (value.isInteger())
	{
		GMSEC_I32 x = 0;
		type = "I32";
		status = value.getI32(x);
		if (!status.hasError())
		{
			cmsMessage.setIntProperty(name, x);
		}
	}
	else if (value.isBoolean())
	{
		bool x = false;
		type = "BOOL";
		status = value.getBoolean(x);
		if (!status.hasError())
		{
			cmsMessage.setBooleanProperty(name, x);
		}
	}
	else if (value.isReal())
	{
		GMSEC_F64 x = 0;
		type = "F64";
		status = value.getF64(x);
		if (!status.hasError())
		{
			cmsMessage.setDoubleProperty(name, x);
		}
	}
	else
	{
		status.set(FIELD_ERROR, OTHER_ERROR_CODE, "Unable to convert type to CMS property");
	}

	if (!status.hasError() && (type.length() > 0))
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

	while (!status.hasError())
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

	if (result.hasError())
	{
		throw GmsecException(result);
	}

	std::string   subject = msg.getSubject();
	Message::Kind kind    = msg.getKind();

	header.setString(GMSEC_SUBJECT_PROPERTY, subject.c_str());

	try
	{
		StdUniquePtr<cms::BytesMessage> bytesMessage(m_session->createBytesMessage());
		bytesMessage->setCMSType(kindToCMSType(kind));
		bytesMessage->setBodyBytes(buffer.get(), static_cast<int>(buffer.size()));

		storeProperties(header, *bytesMessage);

		cmsMessage.reset(bytesMessage.release());
	}
	catch (const cms::CMSException& e)
	{
		GMSEC_VERBOSE << e.what();
		throw GmsecException(MSG_ERROR, INVALID_MSG, e.what());
	}
}
