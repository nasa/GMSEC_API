/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file ConnectionManager.cpp
 *
 *  @brief The ConnectionManager is designed to provide several high-level management
 *  functions for operation of a GMSEC connection object. These management functions 
 *  include the application of message content validation, the operation of a
 *  standard service for producing heartbeats, and a set of functions which streamline 
 *  log message production.
 *
**/

// Disable the Visual Studio warning C4355
//   ('this' : used in base member initializer list)
//   Found in the Connection(Server s, const Options &o) constructor
#ifdef WIN32
#pragma warning(disable: 4355)
#endif


#include <gmsec4/mist/ConnectionManager.h>

#include <gmsec4/Config.h>
#include <gmsec4/Fields.h>
#include <gmsec4/Message.h>
#include <gmsec4/SubscriptionInfo.h>

#include <gmsec4/mist/ConnectionManagerCallback.h>
#include <gmsec4/mist/ConnectionManagerEventCallback.h>
#include <gmsec4/mist/ConnectionManagerReplyCallback.h>
#include <gmsec4/mist/SubscriptionInfo.h>
#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/util/DataList.h>

#include <gmsec4/internal/mist/InternalConnectionManager.h>
#include <gmsec4/mist/Specification.h>


namespace gmsec
{
namespace api
{
namespace mist
{


const char* ConnectionManager::getAPIVersion()
{
	return internal::InternalConnectionManager::getAPIVersion();
}


ConnectionManager::ConnectionManager(const Config& cfg, bool validate, unsigned int version)
	: m_internal(new internal::InternalConnectionManager(this, cfg, validate, version))
{
}

ConnectionManager::~ConnectionManager()
{
	delete m_internal;
	m_internal = 0;
}


void ConnectionManager::initialize()
{
	m_internal->initialize();
}


void ConnectionManager::cleanup()
{
	m_internal->cleanup();
}


Connection::ConnectionState ConnectionManager::getState() const
{
	return m_internal->getState();
}


const char* ConnectionManager::getLibraryRootName() const
{
	return m_internal->getLibraryRootName();
}


const char* ConnectionManager::getLibraryVersion() const
{
	return m_internal->getLibraryVersion();
}


Specification& ConnectionManager::getSpecification() const
{
	return m_internal->getSpecification();
}


void ConnectionManager::setStandardFields(const gmsec::api::util::DataList<Field*>& standardFields)
{
	m_internal->setStandardFields(standardFields);
}


const gmsec::api::util::DataList<Field*>& ConnectionManager::getStandardFields() const
{
	return m_internal->getStandardFields();
}


void ConnectionManager::addStandardFields(Message& msg) const
{
	m_internal->addStandardFields(msg);
}


void ConnectionManager::registerMessageValidator(MessageValidator* validator)
{
	m_internal->registerMessageValidator(validator);
}


void ConnectionManager::registerEventCallback(Connection::ConnectionEvent event, ConnectionManagerEventCallback* cb)
{
	m_internal->registerEventCallback(event, cb);
}


SubscriptionInfo* ConnectionManager::subscribe(const char* subject, ConnectionManagerCallback* cb)
{
	return m_internal->subscribe(subject, cb);
}


SubscriptionInfo* ConnectionManager::subscribe(const char* subject, const Config& config, ConnectionManagerCallback* cb)
{
	return m_internal->subscribe(subject, config, cb);
}


void ConnectionManager::unsubscribe(SubscriptionInfo*& info)
{
	m_internal->unsubscribe(info);
}


void ConnectionManager::publish(const Message& msg)
{
	m_internal->publish(msg);
}


void ConnectionManager::publish(const Message& msg, const Config &mwConfig)
{
	m_internal->publish(msg, mwConfig);
}


void ConnectionManager::request(const Message& request, GMSEC_I32 timeout, ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms)
{
	m_internal->request(request, timeout, cb, republish_ms);
}


Message* ConnectionManager::request(const Message& request, GMSEC_I32 timeout, GMSEC_I32 republish_ms)
{
	return m_internal->request(request, timeout, republish_ms);
}


void ConnectionManager::cancelRequest(ConnectionManagerReplyCallback* cb)
{
	m_internal->cancelRequest(cb);
}


void ConnectionManager::reply(const Message& request, const Message& reply)
{
	m_internal->reply(request, reply);
}


void ConnectionManager::dispatch(const Message& msg)
{
	m_internal->dispatch(msg);
}


Message* ConnectionManager::receive(GMSEC_I32 timeout)
{
	return m_internal->receive(timeout);
}


void ConnectionManager::release(Message*& msg)
{
	m_internal->release(msg);
}


bool ConnectionManager::startAutoDispatch()
{
	return m_internal->startAutoDispatch();
}


bool ConnectionManager::stopAutoDispatch(bool waitForComplete)
{
	return m_internal->stopAutoDispatch(waitForComplete);
}


void ConnectionManager::excludeSubject(const char* subject)
{
	return m_internal->excludeSubject(subject);
}


void ConnectionManager::removeExcludedSubject(const char* subject)
{
	return m_internal->removeExcludedSubject(subject);
}


const char* ConnectionManager::getName() const
{
	return m_internal->getName();
}


void ConnectionManager::setName(const char* name)
{
	m_internal->setName(name);
}


const char* ConnectionManager::getID() const
{
	return m_internal->getID();
}


const char* ConnectionManager::getMWInfo() const
{
	return m_internal->getMWInfo();
}


const char* ConnectionManager::getConnectionEndpoint() const
{
	return m_internal->getConnectionEndpoint();
}


GMSEC_U64 ConnectionManager::getPublishQueueMessageCount() const
{
	return m_internal->getPublishQueueMessageCount();
}


Message ConnectionManager::createHeartbeatMessage(const char* subject, const gmsec::api::util::DataList<Field*>& heartbeatFields) 
{
	return m_internal->createHeartbeatMessage(subject, heartbeatFields);
}


void ConnectionManager::startHeartbeatService(const char* subject, const gmsec::api::util::DataList<Field*>& heartbeatFields)
{
	m_internal->startHeartbeatService(subject, heartbeatFields);
}


void ConnectionManager::stopHeartbeatService()
{
	m_internal->stopHeartbeatService();
}


// deprecated
Status ConnectionManager::changeComponentStatus(const Field& componentStatus)
{
	return m_internal->setHeartbeatServiceField(componentStatus);
}


// deprecated
Status ConnectionManager::changeComponentInfo(const Field& componentInfo)
{
	return m_internal->setHeartbeatServiceField(componentInfo);
}


// deprecated
Status ConnectionManager::changeCPUMemory(const Field& cpuMemory)
{
	return m_internal->setHeartbeatServiceField(cpuMemory);
}


// deprecated
Status ConnectionManager::changeCPUUtil(const Field& cpuUtil)
{
	return m_internal->setHeartbeatServiceField(cpuUtil);
}


Status ConnectionManager::setHeartbeatServiceField(const Field& field)
{
	return m_internal->setHeartbeatServiceField(field);
}


Message ConnectionManager::createLogMessage(const char* subject, const gmsec::api::util::DataList<Field*>& logFields)
{
	return m_internal->createLogMessage(subject, logFields);
}


void ConnectionManager::setLoggingDefaults(const char* subject, const gmsec::api::util::DataList<Field*>& logFields)
{
	m_internal->setLoggingDefaults(subject, logFields);
}


void ConnectionManager::publishLog(const char* logMessage, const Field& severity)
{
	m_internal->publishLog(logMessage, severity);
}


void ConnectionManager::requestSimpleService(const char * subject, const char * opName, const Field& opNumber, const gmsec::api::util::DataList<Field*>& fields, const gmsec::api::util::DataList<ServiceParam*>& sParams)
{
	m_internal->requestSimpleService(subject, opName, opNumber, fields, sParams);
}


void ConnectionManager::requestSimpleService(const char * subject, const char * opName, const Field& opNumber, const gmsec::api::util::DataList<Field*>& fields, const gmsec::api::util::DataList<ServiceParam*>& sParams, GMSEC_I32 timeout, ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms)
{
	m_internal->requestSimpleService(subject, opName, opNumber, fields, sParams, timeout, cb, republish_ms);
}


Message* ConnectionManager::requestSimpleService(const char * subject, const char * opName, const Field& opNumber, const gmsec::api::util::DataList<Field*>& fields, const gmsec::api::util::DataList<ServiceParam*>& sParams, GMSEC_I32 timeout, GMSEC_I32 republish_ms)
{
	return m_internal->requestSimpleService(subject, opName, opNumber, fields, sParams, timeout, republish_ms);
}


void ConnectionManager::acknowledgeSimpleService(const char * subject, const Message &request, ResponseStatus::Response ssResponse, const gmsec::api::util::DataList<Field*>& fields)
{
	m_internal->acknowledgeSimpleService(subject, request, ssResponse, fields);
}


void ConnectionManager::publishResourceMessage(const char* subject, size_t sampleInterval, size_t averageInterval)
{
	m_internal->publishResourceMessage(subject, sampleInterval, averageInterval);
}


Message ConnectionManager::createResourceMessage(const char* subject, size_t sampleInterval, size_t averageInterval)
{
	return m_internal->createResourceMessage(subject, sampleInterval, averageInterval);
}


void ConnectionManager::startResourceMessageService(const char* subject, size_t intervalSeconds, size_t sampleInterval, size_t averageInterval)
{
	m_internal->startResourceMessageService(subject, intervalSeconds, sampleInterval, averageInterval);
}


bool ConnectionManager::stopResourceMessageService()
{
	return m_internal->stopResourceMessageService();
}


void ConnectionManager::requestDirective(const char * subject, const Field& directiveString, const gmsec::api::util::DataList<Field*>& fields)
{
	m_internal->requestDirective(subject, directiveString, fields);
}


void ConnectionManager::requestDirective(const char * subject, const Field& directiveString, const gmsec::api::util::DataList<Field*>& fields, GMSEC_I32 timeout, ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms)
{
	m_internal->requestDirective(subject, directiveString, fields, timeout, cb, republish_ms);
}


Message* ConnectionManager::requestDirective(const char * subject, const Field& directiveString, const gmsec::api::util::DataList<Field*>& fields, GMSEC_I32 timeout, GMSEC_I32 republish_ms)
{
	return m_internal->requestDirective(subject, directiveString, fields, timeout, republish_ms);
}


void ConnectionManager::acknowledgeDirectiveRequest(const char * subject, const Message &request, ResponseStatus::Response ssResponse, const gmsec::api::util::DataList<Field*>& fields)
{
	m_internal->acknowledgeDirectiveRequest(subject, request, ssResponse, fields);
}


void ConnectionManager::shutdownAllMiddlewares()
{
	internal::InternalConnectionManager::shutdownAllMiddlewares();
}


void ConnectionManager::registerMessageValidator(GMSEC_MessageValidator* validator)
{
    m_internal->registerMessageValidator(validator);
}


void ConnectionManager::registerEventCallback(Connection::ConnectionEvent event, GMSEC_ConnectionMgrEventCallback* ecb)
{
	m_internal->registerEventCallback(event, ecb);
}


SubscriptionInfo* ConnectionManager::subscribe(const char* subject, const Config& cfg, GMSEC_ConnectionMgrCallback* cb)
{
	return m_internal->subscribe(subject, cfg, cb);
}


void ConnectionManager::request(const Message& req, GMSEC_I32 timeout, GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb, GMSEC_I32 republish_ms)
{
	m_internal->request(req, timeout, rcb, ecb, republish_ms);
}


void ConnectionManager::cancelRequest(GMSEC_ConnectionMgrReplyCallback* rcb)
{
	m_internal->cancelRequest(rcb);
}


void ConnectionManager::requestDirective(const char* subject, const Field& directiveString,
        const gmsec::api::util::DataList<Field*>& fields, GMSEC_I32 timeout,
        GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb,
		GMSEC_I32 republish_ms)
{
	m_internal->requestDirective(subject, directiveString, fields, timeout, rcb, ecb, republish_ms);
}


void ConnectionManager::requestSimpleService(const char* subject, const char* opName,
        const Field& opNumber, const gmsec::api::util::DataList<Field*>& fields,
        const gmsec::api::util::DataList<ServiceParam*>& params, GMSEC_I32 timeout,
        GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb,
		GMSEC_I32 republish_ms)
{
	m_internal->requestSimpleService(subject, opName, opNumber, fields, params, timeout, rcb, ecb, republish_ms);
}

}  //namespace mist
}  //namespace api
}  //namespace gmsec
