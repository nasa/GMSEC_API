/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Connection.cpp
 *
 *  @brief This file contains the opaque wrapper for middleware connections.
 *
 *
**/

#include <gmsec4/Connection.h>
#include <gmsec4/Callback.h>
#include <gmsec4/Config.h>
#include <gmsec4/EventCallback.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>
#include <gmsec4/ReplyCallback.h>

#include <gmsec4/internal/ConnectionFactory.h>
#include <gmsec4/internal/InternalConnection.h>

#include <gmsec4/util/Log.h>


using namespace gmsec::api;


const char* Connection::getAPIVersion()
{
	return internal::InternalConnection::getAPIVersion();
}


Connection* Connection::create(const Config& cfg)
{
	return internal::ConnectionFactory::create(cfg);
}


void Connection::destroy(Connection*& conn)
{
	internal::ConnectionFactory::destroy(conn);
}


Connection::Connection(gmsec::api::internal::InternalConnection* iConn)
	: m_internal(iConn)
{
}


Connection::~Connection()
{
	delete m_internal;
}


void Connection::connect()
{
	m_internal->connect();
}


void Connection::disconnect()
{
	m_internal->disconnect();
}


gmsec::api::Connection::ConnectionState Connection::getState() const
{
	return m_internal->getState();
}


const char* Connection::getLibraryRootName() const
{
	return m_internal->getLibraryRootName();
}


const char* Connection::getLibraryVersion() const
{
	return m_internal->getLibraryVersion();
}


void Connection::registerEventCallback(ConnectionEvent event, EventCallback* cb)
{
	m_internal->registerEventCallback(event, cb);
}


SubscriptionInfo* Connection::subscribe(const char* subject, Callback* cb)
{
	return m_internal->subscribe(subject, cb);
}


SubscriptionInfo* Connection::subscribe(const char* subject, const Config& config, Callback* cb)
{
	return m_internal->subscribe(subject, config, cb);
}


void Connection::unsubscribe(SubscriptionInfo*& info)
{
	m_internal->unsubscribe(info);
}


bool Connection::startAutoDispatch()
{
	return m_internal->startAutoDispatch();
}


bool Connection::stopAutoDispatch(bool waitForCompletion)
{
	return m_internal->stopAutoDispatch(waitForCompletion);
}


void Connection::publish(const Message& msg)
{
	m_internal->publish(msg);
}


void Connection::publish(const Message& msg, const Config& mwConfig)
{
	m_internal->publish(msg, mwConfig);
}


void Connection::request(const Message& request, GMSEC_I32 timeout, ReplyCallback* cb, GMSEC_I32 republish_ms)
{
	m_internal->request(request, timeout, cb, republish_ms);
}


Message* Connection::request(const Message& request, GMSEC_I32 timeout, GMSEC_I32 republish_ms)
{
	return m_internal->request(request, timeout, republish_ms);
}


void Connection::cancelRequest(ReplyCallback* cb)
{
	m_internal->cancelRequest(cb);
}


void Connection::reply(const Message& request, const Message& reply)
{
	m_internal->reply(request, reply);
}


Message* Connection::receive(GMSEC_I32 timeout)
{
	return m_internal->receive(timeout);
}


void Connection::dispatch(const Message& msg)
{
	m_internal->dispatch(msg);
}


void Connection::release(Message*& msg)
{
	m_internal->release(msg);
}


void Connection::excludeSubject(const char* subject)
{
	return m_internal->excludeSubject(subject);
}


void Connection::removeExcludedSubject(const char* subject)
{
	return m_internal->removeExcludedSubject(subject);
}


const char* Connection::getName() const
{
	return m_internal->getName();
}


void Connection::setName(const char* name)
{
	m_internal->setName(name);
}


const char* Connection::getID() const
{
	return m_internal->getID();
}


const char* Connection::getMWInfo() const
{
	return m_internal->getMWInfo();
}


const char* Connection::getConnectionEndpoint() const
{
	return m_internal->getConnectionEndpoint();
}


GMSEC_U64 Connection::getPublishQueueMessageCount() const
{
	return m_internal->getPublishQueueMessageCount();
}


void Connection::shutdownAllMiddlewares()
{
	internal::InternalConnection::shutdownAllMiddlewares();
}


void Connection::shutdownMiddleware(const char* name)
{
	internal::InternalConnection::shutdownMiddleware(name);
}


void Connection::registerEventCallback(ConnectionEvent event, GMSEC_EventCallback* ecb)
{
	m_internal->registerEventCallback(event, ecb);
}


SubscriptionInfo* Connection::subscribe(const char* subject, const Config& cfg, GMSEC_Callback* cb)
{
	return m_internal->subscribe(subject, cfg, cb);
}


void Connection::request(const Message& req, GMSEC_I32 timeout, GMSEC_ReplyCallback* rcb, GMSEC_EventCallback* ecb, GMSEC_I32 republish_ms)
{
	m_internal->request(req, timeout, rcb, ecb, republish_ms);
}


void Connection::cancelRequest(GMSEC_ReplyCallback* rcb)
{
	m_internal->cancelRequest(rcb);
}
