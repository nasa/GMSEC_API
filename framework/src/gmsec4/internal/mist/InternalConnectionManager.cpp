/*
 * Copyright 2007-2016 United States Government as represented by the
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
**/

#include <gmsec4/internal/mist/InternalConnectionManager.h>
#include <gmsec4/internal/mist/ResourceInfoGenerator.h>
#include <gmsec4/internal/mist/SpecificationBuddy.h>

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4/internal/mist/ConnMgrCallbacks.h>
#include <gmsec4/internal/mist/ConnMgrCallbackCache.h>
#include <gmsec4/mist/Specification.h>

#include <gmsec4/mist/ConnectionManager.h>
#include <gmsec4/mist/ConnectionManagerCallback.h>
#include <gmsec4/mist/ConnectionManagerEventCallback.h>
#include <gmsec4/mist/ConnectionManagerReplyCallback.h>
#include <gmsec4/mist/SubscriptionInfo.h>
#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/util/DataList.h>

#include <gmsec4/Errors.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>
#include <gmsec4/SubscriptionInfo.h>

#include <gmsec4/util/Log.h>

#include <gmsec4/internal/StringUtil.h>

#include <sstream>

using namespace gmsec::api::util;

using gmsec::api::Field;
using gmsec::api::util::DataList;

//static const char* LIBRARY_VERSION_STRING   = "LIBRARY-VERSION";

namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{


InternalConnectionManager::InternalConnectionManager(gmsec::api::mist::ConnectionManager* parent, const Config& cfg, bool validate, unsigned int version)
	: m_config(cfg),
	  m_connection(0),
	  m_specification(0),
	  m_standardHeartbeatFields(),
	  m_standardLogFields(),
	  m_heartbeatSubject(),
	  m_logSubject(),
	  m_subscriptions(),
	  m_hbService(0),
	  m_parent(parent),
	  m_callbackAdapter(new MistCallbackAdapter),
	  m_messagePopulator(NULL)
{
	// Determine whether or not to validate messages from the Config object
	const char* validateValue = m_config.getValue("GMSEC-MSG-CONTENT-VALIDATE");

	if (validateValue)
	{
		m_validate = StringUtil::stringEqualsIgnoreCase(validateValue, "true");
	}
	else
	{
		//if a config value was never supplied default to programmatic definition (which defaults to true)
		m_validate = validate;
	}

	// Check if the specification version is provided within the supplied configuration
	if (m_config.getValue("GMSEC-SPECIFICATION-VERSION", NULL))
	{
		// Yep, found it!

		m_specification = new Specification(m_config);
	}
	else
	{
		// Nope, not found.  Let's try to use the given version number; if that fails, then default to using GMSEC_ISD_CURRENT
		try
		{
			std::ostringstream oss;
			oss << version;

			Config tmpConfig = m_config;
			tmpConfig.addValue("GMSEC-SPECIFICATION-VERSION", oss.str().c_str());

			m_specification = new Specification(tmpConfig);
		}
		catch (...)
		{
			GMSEC_WARNING << "Invalid specification version supplied to ConnectionManager, defaulting to GMSEC_ISD_CURRENT";

			std::ostringstream oss;
			oss << GMSEC_ISD_CURRENT;

			Config tmpConfig = m_config;
			tmpConfig.addValue("GMSEC-SPECIFICATION-VERSION", oss.str().c_str());

			m_specification = new Specification(tmpConfig);
		}
	}

	m_messagePopulator = new MessagePopulator(m_specification->getVersion());

	m_resourceMessageCounter = 0;
}


InternalConnectionManager::~InternalConnectionManager()
{
	try {
		stopHeartbeatService();
	}
	catch (...) {
		// we could end up here if the HBS is not running; no big deal.
	}

	stopResourceMessageService();

	MessagePopulator::destroyFields(m_standardHeartbeatFields);
	MessagePopulator::destroyFields(m_standardLogFields);

	try {
		cleanup();
		delete m_callbackAdapter;
	}
	catch (Exception& e) {
		GMSEC_ERROR << e.what();
	}

	delete m_messagePopulator;
	delete m_specification;
}


void InternalConnectionManager::initialize()
{
	if (m_connection)
	{
		GMSEC_WARNING << "The ConnectionManager has already been initialized!";
		return;
	}

	m_connection = Connection::create(m_config);

	m_connection->connect();
}


void InternalConnectionManager::cleanup()
{
	if (m_connection)
	{
		// We need to disconnect before doing anything else;
		// this ensures that the auto-dispatcher, if running, is stopped so
		// that no more messages are dispatched to our callbacks.
		// Unfortunately, we have no guarantee that the auto-dispatcher will
		// stop 'immediately', thus we may still be at risk destroying our callbacks.
		m_connection->disconnect();

		ConnMgrCallbackCache::getCache().destroyConnMgrCallbacks(m_parent);

		for (SubscriptionList::iterator it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it)
		{
			delete *it;
		}
		m_subscriptions.clear();

		Connection::destroy(m_connection);
	}
}


const char* InternalConnectionManager::getLibraryVersion() const
{
	if (!m_connection)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}
 
	return m_connection->getLibraryVersion(); 
}


Specification& InternalConnectionManager::getSpecification() const
{
	if (!m_connection)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	return *m_specification;
}


void InternalConnectionManager::setStandardFields(const DataList<Field*>& standardFields)
{
	m_messagePopulator->setStandardFields(standardFields);
}


void InternalConnectionManager::addStandardFields(Message& msg) const
{
	m_messagePopulator->addStandardFields(msg);
}


void InternalConnectionManager::registerEventCallback(Connection::ConnectionEvent event, ConnectionManagerEventCallback* cb)
{
	if (!m_connection)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}
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
	if (!m_connection)
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
		callback = new CMCallback(m_parent, cb);
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
	if (!m_connection)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}
	if (!mistInfo)
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The SubscriptionInfo object is null.");
	}
	if (m_connection != mistInfo->getInfo()->getConnection())
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


void InternalConnectionManager::publish(const Message& msg, const Config& config)
{
	if (!m_connection)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}
	if (msg.getKind() != Message::PUBLISH)
	{
		throw Exception(MIST_ERROR, INVALID_MSG, "Cannot issue request with non-PUBLISH kind message.");
	}

	AutoMutex hold(m_cmLock);

	// If validation is enabled, check the message before it is sent out
	if (m_validate)
	{
		m_specification->validateMessage(const_cast<Message&>(msg));
	}

	m_connection->publish(msg, config);
}


void InternalConnectionManager::request(const Message& request, GMSEC_I32 timeout, ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms)
{
	if (!m_connection)
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

	AutoMutex hold(m_cmLock);

	// Validate the message before it is sent out
	if (m_validate)
	{
		m_specification->validateMessage(const_cast<Message&>(request));
	}

	CMReplyCallback* callback = new CMReplyCallback(m_parent, cb);

	ConnMgrCallbackCache::getCache().putReplyCallback(callback, cb);

	m_connection->request(request, timeout, callback, republish_ms);

	//note: The CMReplyCallback will be deallocated later.
}


Message* InternalConnectionManager::request(const Message& request, GMSEC_I32 timeout, GMSEC_I32 republish_ms)
{
	if (!m_connection)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}
	if (request.getKind() != Message::REQUEST)
	{
		throw Exception(MIST_ERROR, INVALID_MSG, "Cannot issue request with non-REQUEST kind message.");
	}

	return m_connection->request(request, timeout, republish_ms);
}


void InternalConnectionManager::cancelRequest(ConnectionManagerReplyCallback* cb)
{
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
	if (!m_connection)
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

	m_connection->reply(request, reply);
}


void InternalConnectionManager::dispatch(const Message& msg)
{
	if (!m_connection)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	m_connection->dispatch(msg);
}


Message* InternalConnectionManager::receive(GMSEC_I32 timeout)
{
	if (!m_connection)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	return m_connection->receive(timeout);
}


void InternalConnectionManager::release(Message*& msg)
{
	if (!m_connection)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	m_connection->release(msg);
}


bool InternalConnectionManager::startAutoDispatch()
{
	if (!m_connection)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	return m_connection->startAutoDispatch();
}


bool InternalConnectionManager::stopAutoDispatch(bool waitForCompletion)
{
	if (!m_connection)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	return m_connection->stopAutoDispatch(waitForCompletion);
}


void InternalConnectionManager::excludeSubject(const char* subject)
{
	if (!m_connection)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	return m_connection->excludeSubject(subject);
}


void InternalConnectionManager::removeExcludedSubject(const char* subject)
{
	if (!m_connection)
	{
		throw Exception(MIST_ERROR, CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
	}

	return m_connection->removeExcludedSubject(subject);
}


Message InternalConnectionManager::createHeartbeatMessage(const char* subject, const DataList<Field*>& heartbeatFields)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}

	// Create message with the standard heartbeat message fields
	Message msg(subject, Message::PUBLISH);

	m_messagePopulator->addStandardFields(msg);
	m_messagePopulator->populateHeartbeatMessage(msg, heartbeatFields, m_standardHeartbeatFields);

	return msg;
}


void InternalConnectionManager::startHeartbeatService(const char* subject, const DataList<Field*>& heartbeatFields)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}
	if (m_hbService.get() && m_hbService.get()->isRunning())
	{
		throw Exception(MIST_ERROR, HEARTBEAT_SERVICE_IS_RUNNING, "HeartbeatService is already running.");
	}

	// Create heartbeat message; this message will be owned by HeartbeatService.
	Message msg = createHeartbeatMessage(subject, heartbeatFields);

	if (m_validate)
	{
		m_specification->validateMessage(msg);
	}

	// Set the standard fields for a heartbeat message
	setHeartbeatDefaults(subject, heartbeatFields);

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
			"Heartbeat Service timed-out when attempting to start.");
	}
}


void InternalConnectionManager::stopHeartbeatService()
{
	if (m_hbService.get() != NULL)
	{
		const unsigned int timeout_ms = 3000;

		if (!m_hbService->stop(timeout_ms))
		{
			GMSEC_WARNING << "Heartbeat Service is not responding in timely manner to shutdown request.";
		}
		else
		{
			m_hbService.reset();
			m_hbThread->join();
			m_hbThread.reset();
		}
	}
	else
	{
		throw Exception(MIST_ERROR, HEARTBEAT_SERVICE_NOT_RUNNING, "HeartbeatService is not running!");
	}
}


void InternalConnectionManager::setHeartbeatDefaults(const char* subject, const DataList<Field*>& heartbeatFields)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}
	if (m_hbService.get() && m_hbService.get()->isRunning())
	{
		throw Exception(MIST_ERROR, HEARTBEAT_SERVICE_IS_RUNNING, "HeartbeatService is already running.");
	}

	MessagePopulator::destroyFields(m_standardHeartbeatFields);

	m_heartbeatSubject = subject;

	for (DataList<Field*>::const_iterator it = heartbeatFields.begin(); it != heartbeatFields.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			m_standardHeartbeatFields.push_back(gmsec::api::internal::InternalField::makeFieldCopy(*field));
		}
	}
}


Status InternalConnectionManager::changeComponentStatus(const Field& componentStatus)
{
	Status result;

	if (m_hbService.get() != NULL)
	{
		if (m_validate)
		{
			Message msg = createHeartbeatMessage(m_heartbeatSubject.c_str(), m_standardHeartbeatFields);

			msg.addField(componentStatus);

			try
			{
				m_specification->validateMessage(msg);
			}
			catch(Exception &e)
			{
				result = e;
			}

			if (!result.isError())
			{
				result = m_hbService->changeComponentStatus(componentStatus);
			}
		}
	}
	else
	{
		result.set(MIST_ERROR, HEARTBEAT_SERVICE_NOT_RUNNING, "HeartbeatService is not running.");
	}

	return result;
}


Status InternalConnectionManager::changeComponentInfo(const Field& componentInfo)
{
	Status result;

	if (m_hbService.get() != NULL)
	{
		if (m_validate)
		{
			
			Message msg = createHeartbeatMessage(m_heartbeatSubject.c_str(), m_standardHeartbeatFields);
			
			msg.addField(componentInfo);

			try
			{
				m_specification->validateMessage(msg);
			}
			catch(Exception &e)
			{
				result = e;
			}

			if (!result.isError())
			{
				result = m_hbService->changeComponentInfo(componentInfo);
			}
		}
	}
	else
	{
		result.set(MIST_ERROR, HEARTBEAT_SERVICE_NOT_RUNNING, "HeartbeatService is not running.");
	}

	return result;
}


Status InternalConnectionManager::changeCPUMemory(const Field& cpuMemory)
{
	Status result;

	if (m_hbService.get() != NULL)
	{
		if (m_validate)
		{

			Message msg = createHeartbeatMessage(m_heartbeatSubject.c_str(), m_standardHeartbeatFields);

			msg.addField(cpuMemory);

			try
			{
				m_specification->validateMessage(msg);
			}
			catch(Exception &e)
			{
				result = e;
			}

			if (!result.isError())
			{
				result = m_hbService->changeCPUMemory(cpuMemory);
			}
		}
	}
	else
	{
		result.set(MIST_ERROR, HEARTBEAT_SERVICE_NOT_RUNNING, "HeartbeatService is not running.");
	}

	return result;
}


Status InternalConnectionManager::changeCPUUtil(const Field& cpuUtil)
{
	Status result;

	if (m_hbService.get() != NULL)
	{
		if (m_validate)
		{

			Message msg = createHeartbeatMessage(m_heartbeatSubject.c_str(), m_standardHeartbeatFields);

			msg.addField(cpuUtil);

			try
			{
				m_specification->validateMessage(msg);
			}
			catch(Exception &e)
			{
				result = e;
			}

			if (!result.isError())
			{
				result = m_hbService->changeCPUUtil(cpuUtil);
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

	// Create message with the standard heartbeat message fields

	Message msg(subject, Message::PUBLISH);

	m_messagePopulator->addStandardFields(msg);
	m_messagePopulator->populateLogMessage(msg, logFields, m_standardLogFields);

	return msg;
}


void InternalConnectionManager::setLoggingDefaults(const char* subject, const DataList<Field*>& logFields)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}

	MessagePopulator::destroyFields(m_standardLogFields);

	m_logSubject = subject;

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
	if (!m_connection)
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

	// Set the MSG-TEXT field and verify it against the template
	//TODO (MEH): use schema to figure out what this field should be instead of
	//hardcoding - ditto for the EVENT-TIME
	msg.addField(severity);
	msg.addField(StringField("MSG-TEXT", logMessage));

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

	Message msg(subject, gmsec::api::Message::PUBLISH);
	m_resourceMessageCounter++;

	m_messagePopulator->addStandardFields(msg);
	m_messagePopulator->populateResourceStaticMembers(msg, m_resourceMessageCounter);

	std::string osVersion = ResourceInfoGenerator::getOSVersion();
	msg.addField("OPER-SYS", osVersion.c_str());

	if(GMSEC_ISD_2014_00 == m_specification->getVersion())
	{
		msg.addField("MSG-ID", "GMSEC-RESOURCE-MESSAGE"); //Pre-2016 ISDs required this version
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

	try
	{ 
		ResourceInfoGenerator::addCPUStats(msg, m_specification->getVersion(), averageInterval/sampleInterval);
	}
	catch (const Exception& excep)
	{
		GMSEC_ERROR << "ResourceInfoGenerator:: " << excep.getErrorMessage();
	}

	try
	{
		ResourceInfoGenerator::addMainMemoryStats(msg, m_specification->getVersion(), averageInterval/sampleInterval);
	}
	catch (const Exception& excep)
	{
		GMSEC_ERROR << "ResourceInfoGenerator:: " << excep.getErrorMessage();
	}

	try
	{
		ResourceInfoGenerator::addDiskStats(msg, m_specification->getVersion(), averageInterval/sampleInterval);
	}
	catch (const Exception& excep)
	{
		GMSEC_ERROR << "ResourceInfoGenerator:: " << excep.getErrorMessage();
	}

	try
	{
		ResourceInfoGenerator::addNetworkStats(msg, m_specification->getVersion(), averageInterval/sampleInterval);
	}
	catch (const Exception& excep)
	{
		GMSEC_ERROR << "ResourceInfoGenerator:: " << excep.getErrorMessage();
	}

	return msg;
}


void InternalConnectionManager::publishResourceMessage(const char* subject, size_t sampleInterval, size_t averageInterval)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "publishResourceMessage():  The subject string is null, or is empty.");
	}

	publish(createResourceMessage(subject, sampleInterval, averageInterval));
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

	// Start the Resource Service
	m_rsrcService.reset(new ResourceService(m_config, m_messagePopulator->getStandardFields(), subject, intervalSeconds, sampleInterval, averageInterval));
	m_rsrcThread.reset(new StdThread(&ResourceService::start, m_rsrcService));
	m_rsrcThread->start();

	if (!m_rsrcService->awaitStart(10000))
	{
		throw Exception(MIST_ERROR, RESOURCE_SERVICE_NOT_RUNNING,
			"Resource Service timed-out when attempting to start.");
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
		}

		return true;
	}

	return false;
}


void InternalConnectionManager::requestDirective(const char * subject, const Field& directiveString, const gmsec::api::util::DataList<Field*>& fields)
{
	Message msg(subject, gmsec::api::Message::PUBLISH);

	msg.addField("RESPONSE", false);   // No response is being requested, nor expected.

	m_messagePopulator->addStandardFields(msg);
	m_messagePopulator->populateDirective(msg, directiveString, fields);

	publish(msg);
}


void InternalConnectionManager::requestDirective(const char * subject, const Field& directiveString, const gmsec::api::util::DataList<Field*>& fields, GMSEC_I32 timeout, ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms)
{
	Message msg(subject, gmsec::api::Message::REQUEST);

	msg.addField("RESPONSE", true);

	m_messagePopulator->addStandardFields(msg);
	m_messagePopulator->populateDirective(msg, directiveString, fields);

	request(msg, timeout, cb, republish_ms);
}


Message* InternalConnectionManager::requestDirective(const char * subject, const Field& directiveString, const gmsec::api::util::DataList<Field*>& fields, GMSEC_I32 timeout, GMSEC_I32 republish_ms)
{
	Message msg(subject, gmsec::api::Message::REQUEST);

	msg.addField("RESPONSE", true);

	m_messagePopulator->addStandardFields(msg);
	m_messagePopulator->populateDirective(msg, directiveString, fields);

	return request(msg, timeout, republish_ms);
}


void InternalConnectionManager::acknowledgeDirectiveRequest(const char * subject, const Message &request, ResponseStatus::Response ssResponse, const gmsec::api::util::DataList<Field*>& fields)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The subject string is null, or is empty.");
	}

	Message msg(subject, gmsec::api::Message::REPLY);

	m_messagePopulator->addStandardFields(msg);
	m_messagePopulator->populateDirectiveAck(msg, ssResponse, fields);

	reply(request, msg);
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
