/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalConnectionManager.cpp
 *
 *  @brief The InternalConnectionManager is designed to provide several high-level management
 *  functions for operation of a GMSEC connection object. These management functions 
 *  include the application of message content validation, the operation of a
 *  standard service for producing heartbeats, and a set of functions which streamline 
 *  log message production.
 *
 */

#include <gmsec4/internal/mist/InternalConnectionManager.h>

#include <gmsec4/internal/mist/ConnMgrCallbacks.h>
#include <gmsec4/internal/mist/ConnMgrCallbackCache.h>
#include <gmsec4/internal/mist/InternalSpecification.h>
#include <gmsec4/internal/mist/ResourceInfoGenerator.h>
#include <gmsec4/internal/mist/SpecificationBuddy.h>

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/mist/message/MistMessage.h>

#include <gmsec4/mist/ConnectionManager.h>
#include <gmsec4/mist/ConnectionManagerCallback.h>
#include <gmsec4/mist/ConnectionManagerEventCallback.h>
#include <gmsec4/mist/ConnectionManagerReplyCallback.h>
#include <gmsec4/mist/Specification.h>
#include <gmsec4/mist/SubscriptionInfo.h>
#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/ConfigOptions.h>
#include <gmsec4/Errors.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>
#include <gmsec4/SubscriptionInfo.h>

#include <gmsec4/util/DataList.h>
#include <gmsec4/util/Log.h>
#include <gmsec4/util/StdUniquePtr.h>

#include <limits>
#include <sstream>

#ifdef WIN32
	#ifdef max
	#undef max
	#endif
#endif

using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{


const char* InternalConnectionManager::getAPIVersion()
{
	return Connection::getAPIVersion();
}


InternalConnectionManager::InternalConnectionManager(gmsec::api::mist::ConnectionManager* parent, const Config& cfg, bool validate, unsigned int version)
	: m_config(cfg),
	  m_connection(0),
	  m_validate((validate ? VALIDATE_SEND : VALIDATE_NONE)),
	  m_specification(0),
	  m_heartbeatServiceSubject(),
	  m_heartbeatServiceFields(),
	  m_logSubject(),
	  m_standardLogFields(),
	  m_msgIdCounter(0),
	  m_requestIdCounter(0),
	  m_resourceMessageCounter(0),
	  m_subscriptions(),
	  m_messagePopulator(0),
	  m_parent(parent),
	  m_callbackAdapter(new MistCallbackAdapter()),
	  m_hbService(0),
	  m_rsrcService(0),
	  m_ceeMessageValidator(0)
{
	// Determine whether or not to validate messages from the Config object
	const char* value = m_config.getValue(GMSEC_MSG_CONTENT_VALIDATE);
	if (value)
	{
		m_validate = (StringUtil::stringEqualsIgnoreCase(value, "true") ? VALIDATE_SEND : VALIDATE_NONE);
	}

	value = m_config.getValue(GMSEC_MSG_CONTENT_VALIDATE_ALL);
	if (value)
	{
		m_validate = (StringUtil::stringEqualsIgnoreCase(value, "true") ? VALIDATE_ALL : VALIDATE_NONE);
	}

	if (m_validate == VALIDATE_NONE)
	{
		value = m_config.getValue(GMSEC_MSG_CONTENT_VALIDATE_RECV);
		if (value)
		{
			m_validate = (StringUtil::stringEqualsIgnoreCase(value, "true") ? VALIDATE_RECV : VALIDATE_NONE);
		}

		value = m_config.getValue(GMSEC_MSG_CONTENT_VALIDATE_SEND);
		if (value)
		{
			if (StringUtil::stringEqualsIgnoreCase(value, "true") && m_validate == VALIDATE_RECV)
			{
				m_validate = VALIDATE_ALL;
			}
			else
			{
				m_validate = (StringUtil::stringEqualsIgnoreCase(value, "true") ? VALIDATE_SEND : VALIDATE_NONE);
			}
		}
	}

	// Check if the specification version is provided within the supplied configuration
	if (m_config.getValue(GMSEC_MESSAGE_SPEC_VERSION, NULL))
	{
		// Yep, found it!
		m_specification.reset(new Specification(m_config));
	}
	else
	{
		// Nope, not found.  Let's try to use the given version number; if that fails, then default to using GMSEC_ISD_CURRENT
		try
		{
			std::ostringstream oss;
			oss << version;

			m_config.addValue("GMSEC-SPECIFICATION-VERSION", oss.str().c_str());

			m_specification.reset(new Specification(m_config));
		}
		catch (...)
		{
			GMSEC_WARNING << "Invalid specification version supplied to ConnectionManager, defaulting to GMSEC_ISD_CURRENT";

			std::ostringstream oss;
			oss << GMSEC_ISD_CURRENT;

			m_config.addValue("GMSEC-SPECIFICATION-VERSION", oss.str().c_str());

			m_specification.reset(new Specification(m_config));
		}
	}

	m_messagePopulator.reset(new MessagePopulator(m_specification->getVersion()));

	// Finally, attempt to create the connection object.
	m_connection.reset(Connection::create(m_config));
}


InternalConnectionManager::~InternalConnectionManager()
{
	stopHeartbeatService(false);
	stopResourceMessageService();

	MessagePopulator::destroyFields(m_heartbeatServiceFields);
	MessagePopulator::destroyFields(m_standardLogFields);

	try {
		cleanup();
	}
	catch (Exception& e) {
		GMSEC_ERROR << e.what();
	}

	if (m_connection.get())
	{
		Connection* conn = m_connection.release();
		Connection::destroy(conn);
	}
}


void InternalConnectionManager::initialize()
{
	if (m_connection->getState() == Connection::CONNECTED)
	{
		GMSEC_WARNING << "The ConnectionManager has already been initialized!";
		return;
	}

	m_connection->connect();
}


void InternalConnectionManager::cleanup()
{
	// We need to disconnect before doing anything else;
	// this ensures that the auto-dispatcher, if running, is stopped so
	// that no more messages are dispatched to our callbacks.
	// Unfortunately, we have no guarantee that the auto-dispatcher will
	// stop 'immediately', thus we may still be at risk destroying our callbacks.

	// Gracefully handle any Exceptions thrown by the call to disconnect()
	Exception error(NO_ERROR_CLASS, NO_ERROR_CODE, "");

	if (m_connection->getState() == Connection::CONNECTED)
	{
		try
		{
			m_connection->disconnect();
		}
		catch (const Exception& e)
		{
			error = e;
		}
	}

	ConnMgrCallbackCache::getCache().destroyConnMgrCallbacks(m_parent);

	for (SubscriptionList::iterator it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it)
	{
		delete *it;
	}
	m_subscriptions.clear();

	if (error.getErrorClass() != NO_ERROR_CLASS)
	{
		throw error;
	}

	// Kludge: We need to destroy the Connection object now (and then recreate)
	// when we are working with the generic_jms middleware wrapper.
	//
	const char* mwID = m_config.getValue("mw-id");
	if (!mwID)
	{
		mwID = m_config.getValue("connectionType");
	}
	if (mwID && std::string(mwID).find("generic_jms") != std::string::npos)
	{
		Connection* conn = m_connection.release();
		Connection::destroy(conn);
		m_connection.reset(Connection::create(m_config));
	}
}


Connection::ConnectionState InternalConnectionManager::getState() const
{
	return m_connection->getState();
}


const char* InternalConnectionManager::getLibraryRootName() const
{
	return m_connection->getLibraryRootName();
}


const char* InternalConnectionManager::getLibraryVersion() const
{
	return m_connection->getLibraryVersion(); 
}


Specification& InternalConnectionManager::getSpecification() const
{
	return *m_specification;
}


void InternalConnectionManager::setStandardFields(const DataList<Field*>& standardFields)
{
	m_messagePopulator->setStandardFields(standardFields);
}


const DataList<Field*>& InternalConnectionManager::getStandardFields() const
{
	return m_messagePopulator->getStandardFields();
}


void InternalConnectionManager::addStandardFields(Message& msg) const
{
	m_messagePopulator->addStandardFields(msg);
}


void InternalConnectionManager::registerMessageValidator(MessageValidator* validator)
{
	if (!validator)
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "MessageValidator cannot be null.");
	}

	getSpecification().registerMessageValidator(validator);
}


void InternalConnectionManager::registerEventCallback(Connection::ConnectionEvent event, ConnectionManagerEventCallback* cb)
{
	if (!cb)
	{
		throw Exception(MIST_ERROR, INVALID_CALLBACK, "The ConnectionManagerEventCallback cannot be null.");
	}

	CMEventCallback* callback = new CMEventCallback(m_parent, cb);

	ConnMgrCallbackCache::getCache().putEventCallback(callback ,cb);

	m_connection->registerEventCallback(event, callback);
}


SubscriptionInfo* InternalConnectionManager::subscribe(const char *subject, ConnectionManagerCallback* cb)
{
	Config config;  // define empty config

	return subscribe(subject, config, cb);
}


SubscriptionInfo* InternalConnectionManager::subscribe(const char* subject, const Config& config, ConnectionManagerCallback* cb)
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}

	CMCallback* callback = 0;

	if (cb)
	{
		callback = new CMCallback(m_parent, cb, validateOnRecv());
	}

	gmsec::api::SubscriptionInfo* info     = m_connection->subscribe(subject, config, callback);
	SubscriptionInfo*             mistInfo = new SubscriptionInfo(info, cb);

	// Add SubscriptionInfo to our internal list
	{
		AutoMutex hold(m_cmLock);

		m_subscriptions.push_back(mistInfo);
	}

	if (callback)
	{
		ConnMgrCallbackCache::getCache().putCallback(mistInfo, callback);
	}

	return mistInfo;
}


void InternalConnectionManager::unsubscribe(SubscriptionInfo*& mistInfo)
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}
	if (!mistInfo)
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The SubscriptionInfo object is null.");
	}
	if (m_connection.get() != mistInfo->getInfo()->getConnection())
	{
		throw Exception(MIST_ERROR, OTHER_ERROR_CODE,
			"The given SubscriptionInfo object is not associated with this ConnectionManager");
	}

	// Look up the original callback (if any) that was setup with the subscription; destroy if found.
	CMCallback* callback = ConnMgrCallbackCache::getCache().getAndRemoveCallback(mistInfo);

	gmsec::api::SubscriptionInfo* info = mistInfo->getInfo();
	m_connection->unsubscribe(info);

	if (callback)
	{
		delete callback;
	}

	// Remove SubscriptionInfo from our internal list
	{
		AutoMutex hold(m_cmLock);

		for (SubscriptionList::iterator it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it)
		{
			if (*it == mistInfo)
			{
				m_subscriptions.erase(it);
				break;
			}
		}
	}

	delete mistInfo;
	mistInfo = 0;
}


void InternalConnectionManager::publish(const Message& msg)
{
	Config emptyCfg;
	publish(msg, emptyCfg);
}


void InternalConnectionManager::publish(const Message& msg, const Config& mwConfig)
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}
	if (msg.getKind() != Message::PUBLISH)
	{
		throw Exception(MIST_ERROR, INVALID_MSG, "Cannot publish message with non-PUBLISH message kind.");
	}

	// If validation is enabled, check the message before it is sent out
	if (validateOnSend())
	{
		MessageBuddy::getInternal(msg).getDetails().setBoolean(GMSEC_MSG_BEING_SENT, true);
		getSpecification().validateMessage(const_cast<Message&>(msg));
	}

	m_connection->publish(msg, mwConfig);
}


void InternalConnectionManager::request(const Message& request, GMSEC_I32 timeout, ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms)
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}
	if (request.getKind() != Message::REQUEST)
	{
		throw Exception(MIST_ERROR, INVALID_MSG, "Cannot issue request with non-REQUEST kind message.");
	}
	if (!cb)
	{
		throw Exception(MIST_ERROR, INVALID_CALLBACK, "The ConnectionManagerReplyCallback is null.");
	}

	// Validate the message before it is sent out
	if (validateOnSend())
	{
		MessageBuddy::getInternal(request).getDetails().setBoolean(GMSEC_MSG_BEING_SENT, true);
		getSpecification().validateMessage(const_cast<Message&>(request));
	}

	CMReplyCallback* callback = new CMReplyCallback(m_parent, cb, validateOnRecv());

	ConnMgrCallbackCache::getCache().putReplyCallback(callback, cb);

	m_connection->request(request, timeout, callback, republish_ms);

	//note: The CMReplyCallback will be deallocated later.
}


Message* InternalConnectionManager::request(const Message& request, GMSEC_I32 timeout, GMSEC_I32 republish_ms)
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}
	if (request.getKind() != Message::REQUEST)
	{
		throw Exception(MIST_ERROR, INVALID_MSG, "Cannot issue request with non-REQUEST kind message.");
	}

	// Validate the message before it is sent out
	if (validateOnSend())
	{
		MessageBuddy::getInternal(request).getDetails().setBoolean(GMSEC_MSG_BEING_SENT, true);
		getSpecification().validateMessage(const_cast<Message&>(request));
	}

	return m_connection->request(request, timeout, republish_ms);
}


void InternalConnectionManager::cancelRequest(ConnectionManagerReplyCallback* cb)
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}
	if (!cb)
	{
		throw Exception(MIST_ERROR, INVALID_CALLBACK, "The ConnectionManagerReplyCallback is null.");
	}

	ReplyCallback* callback = ConnMgrCallbackCache::getCache().getAndRemoveReplyCallback(cb);

	if (!callback)
	{
		throw Exception(MIST_ERROR, INVALID_CALLBACK, "The ConnectionManagerReplyCallback is not registered with any pending request.");
	}

	m_connection->cancelRequest(callback);
}


void InternalConnectionManager::reply(const Message& request, const Message& reply)
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}
	if (request.getKind() != Message::REQUEST)
	{
		throw Exception(MIST_ERROR, INVALID_MSG, "Cannot issue reply with non-REQUEST kind message.");
	}
	if (reply.getKind() != Message::REPLY)
	{
		throw Exception(MIST_ERROR, INVALID_MSG, "Cannot issue reply with non-REPLY kind message.");
	}

	// Validate the message before it is sent out
	if (validateOnSend())
	{
		MessageBuddy::getInternal(reply).getDetails().setBoolean(GMSEC_MSG_BEING_SENT, true);
		getSpecification().validateMessage(const_cast<Message&>(reply));
	}

	m_connection->reply(request, reply);
}


void InternalConnectionManager::dispatch(const Message& msg)
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	// If validation is enabled, check the message before it is sent out
	if (validateOnSend())
	{
		MessageBuddy::getInternal(msg).getDetails().setBoolean(GMSEC_MSG_BEING_SENT, true);
		getSpecification().validateMessage(const_cast<Message&>(msg));
	}

	m_connection->dispatch(msg);
}


Message* InternalConnectionManager::receive(GMSEC_I32 timeout)
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	StdUniquePtr<Message> msg(m_connection->receive(timeout));

	if (msg.get() != NULL && validateOnRecv())
	{
		getSpecification().validateMessage(*(msg.get()));
	}

	return msg.release();
}


void InternalConnectionManager::release(Message*& msg)
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	m_connection->release(msg);
}


bool InternalConnectionManager::startAutoDispatch()
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	return m_connection->startAutoDispatch();
}


bool InternalConnectionManager::stopAutoDispatch(bool waitForCompletion)
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	return m_connection->stopAutoDispatch(waitForCompletion);
}


void InternalConnectionManager::excludeSubject(const char* subject)
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	return m_connection->excludeSubject(subject);
}


void InternalConnectionManager::removeExcludedSubject(const char* subject)
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	return m_connection->removeExcludedSubject(subject);
}


const char* InternalConnectionManager::getName() const
{
	return m_connection->getName();
}


void InternalConnectionManager::setName(const char* name)
{
	m_connection->setName(name);
}


const char* InternalConnectionManager::getID() const
{
	return m_connection->getID();
}


const char* InternalConnectionManager::getMWInfo() const
{
	return m_connection->getMWInfo();
}


const char* InternalConnectionManager::getConnectionEndpoint() const
{
	return m_connection->getConnectionEndpoint();
}


GMSEC_U64 InternalConnectionManager::getPublishQueueMessageCount() const
{
	return m_connection->getPublishQueueMessageCount();
}


Message InternalConnectionManager::createHeartbeatMessage(const char* subject, const DataList<Field*>& heartbeatFields)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty");
	}

	// Create message with the standard heartbeat message fields
	message::MistMessage msg(subject, "HB", getSpecification());

	for (DataList<Field*>::const_iterator it = heartbeatFields.begin(); it != heartbeatFields.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			msg.addField(*field);
		}
	}

	m_messagePopulator->addStandardFields(msg);

	if (msg.getField("MSG-ID") == NULL && getSpecification().getVersion() <= GMSEC_ISD_2014_00)
	{
		std::ostringstream oss;
		oss << "GMSEC-HB-" << m_msgIdCounter.incrementAndGet();
		msg.addField("MSG-ID", oss.str().c_str()); //Pre-2016 ISDs require this field
	}

	return msg;
}


void InternalConnectionManager::startHeartbeatService(const char* subject, const DataList<Field*>& heartbeatFields)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty");
	}
	if (m_hbService.get() && m_hbService.get()->isRunning())
	{
		throw Exception(MIST_ERROR, HEARTBEAT_SERVICE_IS_RUNNING, "Heartbeat Service is already running");
	}

	// Create heartbeat message; this message will be owned by HeartbeatService.
	Message msg = createHeartbeatMessage(subject, heartbeatFields);

	if (validateOnSend())
	{
		getSpecification().validateMessage(msg);
	}

	// Store the heartbeat service subject and fields that were given;
	// these can be used later if setHeartbeatServiceField() is called.
	storeHeartbeatServiceDetails(subject, heartbeatFields);

	// Start the Heartbeat Service
	m_hbService.reset(new HeartbeatService(m_config, msg));
	m_hbThread.reset(new StdThread(&HeartbeatService::start, m_hbService));
	m_hbThread->start();

	// Wait until the Heartbeat Service thread actually starts
	// running before allowing the thread which called this
	// function, presumably a "user-controlled" thread, to move on.
	//
	// Regardless of whether we receive confirmation that the service
	// has started or not, we need to destroy the CXC2 HB message template
	// that was provided to the service.
	//
	if (!m_hbService->awaitStart(10000))
	{
		throw Exception(MIST_ERROR, HEARTBEAT_SERVICE_NOT_RUNNING,
			"Heartbeat Service timed-out when attempting to start");
	}
	else
	{
		GMSEC_INFO << "Heartbeat Service started";
	}
}


void InternalConnectionManager::stopHeartbeatService(bool throwExceptionOnError)
{
	if (m_hbService.get() && m_hbService.get()->isRunning())
	{
		const unsigned int timeout_ms = 3000;

		if (!m_hbService->stop(timeout_ms))
		{
			GMSEC_WARNING << "Heartbeat Service is not responding in timely manner to shutdown request";
		}
		else
		{
			m_hbService.reset();
			m_hbThread->join();
			m_hbThread.reset();
			GMSEC_INFO << "Heartbeat Service has been stopped";
		}
	}
	else if (throwExceptionOnError)
	{
		throw Exception(MIST_ERROR, HEARTBEAT_SERVICE_NOT_RUNNING, "Heartbeat Service is not running!");
	}
}


void InternalConnectionManager::storeHeartbeatServiceDetails(const char* subject, const DataList<Field*>& heartbeatFields)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}
	if (m_hbService.get() && m_hbService.get()->isRunning())
	{
		throw Exception(MIST_ERROR, HEARTBEAT_SERVICE_IS_RUNNING, "HeartbeatService is already running.");
	}

	MessagePopulator::destroyFields(m_heartbeatServiceFields);

	m_heartbeatServiceSubject = subject;

	for (DataList<Field*>::const_iterator it = heartbeatFields.begin(); it != heartbeatFields.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			m_heartbeatServiceFields.push_back(gmsec::api::internal::InternalField::makeFieldCopy(*field));
		}
	}
}


Status InternalConnectionManager::setHeartbeatServiceField(const Field& field)
{
	Status result;

	if (m_hbService.get() != NULL)
	{
		if (validateOnSend())
		{
			Message msg = createHeartbeatMessage(m_heartbeatServiceSubject.c_str(), m_heartbeatServiceFields);

			msg.addField(field);

			try
			{
				getSpecification().validateMessage(msg);
			}
			catch (const Exception& e)
			{
				result = e;
			}
		}

		if (!result.isError())
		{
			try
			{
				m_hbService->setField(field);
			}
			catch (const Exception& e)
			{
				result = e;
			}
		}
	}
	else
	{
		result.set(MIST_ERROR, HEARTBEAT_SERVICE_NOT_RUNNING, "HeartbeatService is not running.");
	}

	return result;
}


Message InternalConnectionManager::createLogMessage(const char* subject, const DataList<Field*>& logFields)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}

	// Create message with the standard log message fields
	message::MistMessage msg(subject, "LOG", getSpecification());

	// Add user provided fields specific to this message
	for (DataList<Field*>::const_iterator it = logFields.begin(); it != logFields.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			msg.addField(*field);
		}
	}

	// Add fields specific to all log messages
	for (DataList<Field*>::const_iterator it = m_standardLogFields.begin(); it != m_standardLogFields.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			msg.addField(*field);
		}
	}

	// Add fields specific to all messages
	m_messagePopulator->addStandardFields(msg);

	// If the message already contains the EVENT-TIME field, then do NOT overwrite it;
	// otherwise add the EVENT-TIME field with the current time.
	if (!msg.hasField("EVENT-TIME"))
	{
		char eventTime[GMSEC_TIME_BUFSIZE] = {0};
		GMSEC_TimeSpec theTime = TimeUtil::getCurrentTime();
		TimeUtil::formatTime(theTime, eventTime);

		msg.addField("EVENT-TIME", const_cast<const char*>(eventTime));
	}

	return msg;
}


void InternalConnectionManager::setLoggingDefaults(const char* subject, const DataList<Field*>& logFields)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}

	m_logSubject = subject;

	MessagePopulator::destroyFields(m_standardLogFields);

	for (DataList<Field*>::const_iterator it = logFields.begin(); it != logFields.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			m_standardLogFields.push_back(gmsec::api::internal::InternalField::makeFieldCopy(*field));
		}
	}
}


void InternalConnectionManager::publishLog(const char* logMessage, const Field& severity)
{
	if (m_connection->getState() != Connection::CONNECTED)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}
	if (!logMessage || std::string(logMessage).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The log message string is null, or empty.");
	}

	// Create a log message
	DataList<Field*> emptyList;
	Message msg = createLogMessage(m_logSubject.c_str(), emptyList);

	std::string tmpSubject = msg.getSubject();
	std::ostringstream oss;
	oss << tmpSubject;
	if (msg.hasField("SUBCLASS"))
	{
		oss << "." << msg.getStringValue("SUBCLASS");
	}
	else
	{
		oss << ".FILL";
	}
	if (msg.hasField("OCCURRENCE-TYPE"))
	{
		oss << "." << msg.getStringValue("OCCURRENCE-TYPE");
	}
	else
	{
		oss << ".FILL";
	}
	oss << "." << severity.getStringValue();

	tmpSubject = StringConverter::instance().convertString(oss.str());

	msg.setSubject(tmpSubject.c_str());

	msg.addField(severity);
	msg.addField("MSG-TEXT", logMessage);

	publish(msg);
}


void InternalConnectionManager::destroyFields(FieldList& flist)
{
	for (FieldList::iterator it = flist.begin(); it != flist.end(); ++it)
	{
		delete *it;
	}

	flist.clear();
}


void InternalConnectionManager::requestSimpleService(const char * subject, const char * opName, const Field& opNumber, const gmsec::api::util::DataList<Field*>& fields, const gmsec::api::util::DataList<ServiceParam*>& sParams)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}

	Message msg(subject, gmsec::api::Message::PUBLISH);

	msg.addField("RESPONSE", false);

	m_messagePopulator->addStandardFields(msg);
	m_messagePopulator->populateSimpleServiceMessage(msg, opName, opNumber, fields, sParams);

	publish(msg);
}


void InternalConnectionManager::requestSimpleService(const char * subject, const char * opName, const Field& opNumber, const gmsec::api::util::DataList<Field*>& fields, const gmsec::api::util::DataList<ServiceParam*>& sParams, GMSEC_I32 timeout, ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}

	Message msg(subject, gmsec::api::Message::REQUEST);

	msg.addField("RESPONSE", true);

	m_messagePopulator->addStandardFields(msg);
	m_messagePopulator->populateSimpleServiceMessage(msg, opName, opNumber, fields, sParams);

	request(msg, timeout, cb, republish_ms);
}


Message* InternalConnectionManager::requestSimpleService(const char * subject, const char * opName, const Field& opNumber, const gmsec::api::util::DataList<Field*>& fields, const gmsec::api::util::DataList<ServiceParam*>& sParams, GMSEC_I32 timeout, GMSEC_I32 republish_ms)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}

	Message msg(subject, gmsec::api::Message::REQUEST);

	msg.addField("RESPONSE", true);

	m_messagePopulator->addStandardFields(msg);
	m_messagePopulator->populateSimpleServiceMessage(msg, opName, opNumber, fields, sParams);

	return request(msg, timeout, republish_ms);
}


void InternalConnectionManager::acknowledgeSimpleService(const char * subject, const Message &request, ResponseStatus::Response ssResponse, const gmsec::api::util::DataList<Field*>& fields)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}

	//TODO: is this subject just going to be overwritten?
	Message msg(subject, gmsec::api::Message::REPLY);

	m_messagePopulator->addStandardFields(msg);
	m_messagePopulator->populateSimpleServiceAck(msg, ssResponse, fields);

	reply(request, msg);
}


Message InternalConnectionManager::createResourceMessage(const char* subject, size_t sampleInterval, size_t averageInterval)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}
	if (sampleInterval == 0)
	{
		throw Exception(MIST_ERROR,
		                RESOURCE_INFO_SAMPLING_ERROR,
		                "InternalConnectionManager::createResourceMessage():  A sample rate of zero was used.");
	}
	else if (averageInterval < sampleInterval)
	{
		throw Exception(MIST_ERROR,
		                RESOURCE_INFO_SAMPLING_ERROR,
		                "InternalConnectionManager::createResourceMessage():  A moving average interval less than the sampling interval was used.");
	}

	// Create message with the standard resource message fields
	message::MistMessage msg(subject, "RSRC", getSpecification());


	std::string osVersion = ResourceInfoGenerator::getOSVersion();
	msg.addField("OPER-SYS", osVersion.c_str());

	try
	{ 
		ResourceInfoGenerator::addCPUStats(msg, getSpecification().getVersion(), averageInterval/sampleInterval);
	}
	catch (const Exception& e)
	{
		GMSEC_ERROR << "ResourceInfoGenerator:: " << e.getErrorMessage();
	}

	try
	{
		ResourceInfoGenerator::addMainMemoryStats(msg, getSpecification().getVersion(), averageInterval/sampleInterval);
	}
	catch (const Exception& e)
	{
		GMSEC_ERROR << "ResourceInfoGenerator:: " << e.getErrorMessage();
	}

	try
	{
		ResourceInfoGenerator::addDiskStats(msg, getSpecification().getVersion(), averageInterval/sampleInterval);
	}
	catch (const Exception& e)
	{
		GMSEC_ERROR << "ResourceInfoGenerator:: " << e.getErrorMessage();
	}

	try
	{
		ResourceInfoGenerator::addNetworkStats(msg, sampleInterval, averageInterval/sampleInterval);
	}
	catch (const Exception& e)
	{
		GMSEC_ERROR << "ResourceInfoGenerator:: " << e.getErrorMessage();
	}

	m_messagePopulator->addStandardFields(msg);

	return msg;
}


//deprecated
void InternalConnectionManager::publishResourceMessage(const char* subject, size_t sampleInterval, size_t averageInterval)
{
	const size_t defaultPubRate = 0;
	publishResourceMessage(subject, defaultPubRate, sampleInterval, averageInterval);
}


void InternalConnectionManager::publishResourceMessage(const char* subject, size_t pubRate, size_t sampleInterval, size_t averageInterval)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "publishResourceMessage():  The subject string is null, or is empty.");
	}

	Message rsrcMsg = createResourceMessage(subject, sampleInterval, averageInterval);

	if (getSpecification().getVersion() < GMSEC_ISD_2016_00)
	{
		rsrcMsg.addField("COUNTER", (GMSEC_I16) m_resourceMessageCounter.incrementAndGet());

		if (pubRate > 0)
		{
			rsrcMsg.addField("PUB-RATE", (GMSEC_I16) pubRate);
		}
	}
	else
	{
		rsrcMsg.addField("COUNTER", (GMSEC_U16) m_resourceMessageCounter.incrementAndGet());
		if (pubRate > 0)
		{
			rsrcMsg.addField("PUB-RATE", (GMSEC_U16) pubRate);
		}
	}

	if (getSpecification().getVersion() <= GMSEC_ISD_2014_00)
	{
		std::ostringstream oss;
		oss << "GMSEC-RESOURCE-MESSAGE-" << m_msgIdCounter.get();
		rsrcMsg.addField("MSG-ID", oss.str().c_str()); //Pre-2016 ISDs require this field
	}

	publish(rsrcMsg);
}


void InternalConnectionManager::startResourceMessageService(const char* subject, size_t intervalSeconds, size_t sampleInterval, size_t averageInterval)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}
	if (m_rsrcService.get() && m_rsrcService.get()->isRunning())
	{
		throw Exception(MIST_ERROR, RESOURCE_SERVICE_IS_RUNNING, "ResourceService is already running.");
	}

	// Kludge alert: We need to inform the 'other' ConnectionManager, which is used by the ResourceService,
	// of the desired pub-rate for Resource Messages. The pub-rate setting will be used within createResourceMessage().
	m_rsrcService.reset(new ResourceService(m_config, m_messagePopulator->getStandardFields(), subject, intervalSeconds, sampleInterval, averageInterval));
	m_rsrcThread.reset(new StdThread(&ResourceService::start, m_rsrcService));
	m_rsrcThread->start();

	if (!m_rsrcService->awaitStart(10000))
	{
		throw Exception(MIST_ERROR, RESOURCE_SERVICE_NOT_RUNNING,
			"Resource Service timed-out when attempting to start.");
	}
	else
	{
		GMSEC_INFO << "Resource Service started.";
	}
}


bool InternalConnectionManager::stopResourceMessageService()
{
	if (m_rsrcService.get() != NULL)
	{
		const unsigned int timeout_ms = 3000;

		if (!m_rsrcService->stop(timeout_ms))
		{
			GMSEC_WARNING << "Resource Service is not responding in timely manner to shutdown request.";
		}
		else
		{
			m_rsrcService.reset();
			m_rsrcThread->join();
			m_rsrcThread.reset();
			GMSEC_INFO << "Resource Service stopped by user.";
		}

		return true;
	}

	return false;
}


void InternalConnectionManager::requestDirective(const char * subject, const Field& directiveString, const gmsec::api::util::DataList<Field*>& fields)
{
	message::MistMessage msg(subject, "REQ.DIR", getSpecification());

	msg.addField("RESPONSE", false);   // No response is being requested, nor expected.

	msg.addField(directiveString);

	for (DataList<Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			msg.addField(*field);
		}
	}

	m_messagePopulator->addStandardFields(msg);

	if (getSpecification().getVersion() == GMSEC_ISD_2014_00 && !msg.hasField("MSG-ID"))
	{
		std::ostringstream oss;
		oss << "GMSEC-REQUEST-MESSAGE-" << m_msgIdCounter.get();
		msg.addField("MSG-ID", oss.str().c_str());
	}
	else if (getSpecification().getVersion() >= GMSEC_ISD_2019_00 && !msg.hasField("REQUEST-ID"))
	{
		msg.setValue("REQUEST-ID", (GMSEC_I64) m_requestIdCounter.incrementAndGet());
	}

	Message* reply = request(msg, 10, -1);

	if (reply)
	{
		GMSEC_WARNING << "An unexpected response was received request directive: " << subject;

		release(reply);
	}
}


void InternalConnectionManager::requestDirective(const char * subject, const Field& directiveString, const gmsec::api::util::DataList<Field*>& fields, GMSEC_I32 timeout, ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms)
{
	message::MistMessage msg(subject, "REQ.DIR", getSpecification());

	msg.addField("RESPONSE", true);

	msg.addField(directiveString);

	for (DataList<Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			msg.addField(*field);
		}
	}

	m_messagePopulator->addStandardFields(msg);

	if (getSpecification().getVersion() == GMSEC_ISD_2014_00 && !msg.hasField("MSG-ID"))
	{
		std::ostringstream oss;
		oss << "GMSEC-REQUEST-MESSAGE-" << m_msgIdCounter.get();
		msg.addField("MSG-ID", oss.str().c_str());
	}
	else if (getSpecification().getVersion() >= GMSEC_ISD_2019_00 && !msg.hasField("REQUEST-ID"))
	{
		msg.setValue("REQUEST-ID", (GMSEC_I64) m_requestIdCounter.incrementAndGet());
	}

	request(msg, timeout, cb, republish_ms);
}


Message* InternalConnectionManager::requestDirective(const char * subject, const Field& directiveString, const gmsec::api::util::DataList<Field*>& fields, GMSEC_I32 timeout, GMSEC_I32 republish_ms)
{
	message::MistMessage msg(subject, "REQ.DIR", getSpecification());

	msg.addField("RESPONSE", true);

	msg.addField(directiveString);

	for (DataList<Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			msg.addField(*field);
		}
	}

	m_messagePopulator->addStandardFields(msg);

	if (getSpecification().getVersion() == GMSEC_ISD_2014_00 && !msg.hasField("MSG-ID"))
	{
		std::ostringstream oss;
		oss << "GMSEC-REQUEST-MESSAGE-" << m_msgIdCounter.get();
		msg.addField("MSG-ID", oss.str().c_str());
	}
	else if (getSpecification().getVersion() >= GMSEC_ISD_2019_00 && !msg.hasField("REQUEST-ID"))
	{
		msg.setValue("REQUEST-ID", (GMSEC_I64) m_requestIdCounter.incrementAndGet());
	}

	return request(msg, timeout, republish_ms);
}


void InternalConnectionManager::acknowledgeDirectiveRequest(const char * subject, const Message &request, ResponseStatus::Response ssResponse, const gmsec::api::util::DataList<Field*>& fields)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}

	message::MistMessage msg(subject, "RESP.DIR", getSpecification());

	msg.setValue("RESPONSE-STATUS", (GMSEC_I64) ssResponse);

	for (DataList<Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			msg.addField(*field);
		}
	}

	m_messagePopulator->addStandardFields(msg);

	if (getSpecification().getVersion() == GMSEC_ISD_2014_00 && request.hasField("MSG-ID"))
	{
		msg.addField(*(request.getField("MSG-ID")));
	}
	else if (getSpecification().getVersion() >= GMSEC_ISD_2019_00 && request.hasField("REQUEST-ID"))
	{
		msg.addField(*(request.getField("REQUEST-ID")));
	}

	reply(request, msg);
}


void InternalConnectionManager::shutdownAllMiddlewares()
{
	Connection::shutdownAllMiddlewares();
}


void InternalConnectionManager::registerMessageValidator(GMSEC_MessageValidator* validator)
{
	if (validator == NULL || *validator == NULL)
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "MessageValidator cannot be null.");
	}

	m_ceeMessageValidator.reset(new CustomMessageValidator(validator));

	getSpecification().registerMessageValidator(m_ceeMessageValidator.get());
}


void InternalConnectionManager::registerEventCallback(Connection::ConnectionEvent event, GMSEC_ConnectionMgrEventCallback* ecb)
{
	ConnectionManagerEventCallback* callback = m_callbackAdapter->createEventCallback(ecb);

	registerEventCallback(event, callback);
}


SubscriptionInfo* InternalConnectionManager::subscribe(const char* subject, const Config& cfg, GMSEC_ConnectionMgrCallback* cb)
{
	ConnectionManagerCallback* callback = m_callbackAdapter->createCallback(cb);

	SubscriptionInfo* info = subscribe(subject, cfg, callback);

	info->setCallbackFunction(reinterpret_cast<void*>(cb));

	return info;
}


void InternalConnectionManager::request(const Message& req, GMSEC_I32 timeout, GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb, GMSEC_I32 republish_ms)
{
	ConnectionManagerReplyCallback* callback = m_callbackAdapter->createReplyCallback(rcb, ecb);

	request(req, timeout, callback, republish_ms);
}


void InternalConnectionManager::cancelRequest(GMSEC_ConnectionMgrReplyCallback* rcb)
{
	ConnectionManagerReplyCallback* callback = m_callbackAdapter->lookupReplyCallback(rcb);

	cancelRequest(callback);

	m_callbackAdapter->deleteReplyCallback(rcb);
}


void InternalConnectionManager::requestDirective(const char* subject, const Field& directiveString,
		const gmsec::api::util::DataList<Field*>& fields,
		GMSEC_I32 timeout,
		GMSEC_ConnectionMgrReplyCallback* rcb,
		GMSEC_ConnectionMgrEventCallback* ecb,
		GMSEC_I32 republish_ms)
{
	ConnectionManagerReplyCallback* callback = m_callbackAdapter->createReplyCallback(rcb, ecb);

	requestDirective(subject, directiveString, fields, timeout, callback, republish_ms);
}


void InternalConnectionManager::requestSimpleService(const char * subject, const char * opName, const Field& opNumber,
		const gmsec::api::util::DataList<Field*>& fields,
		const gmsec::api::util::DataList<ServiceParam*>& params,
		GMSEC_I32 timeout,
		GMSEC_ConnectionMgrReplyCallback* rcb,
		GMSEC_ConnectionMgrEventCallback* ecb,
		GMSEC_I32 republish_ms)
{
	ConnectionManagerReplyCallback* callback = m_callbackAdapter->createReplyCallback(rcb, ecb);

	requestSimpleService(subject, opName, opNumber, fields, params, timeout, callback, republish_ms);
}


} // end namespace internal
} // end namespace mist
} // end namespace api
} // end namespace gmsec
