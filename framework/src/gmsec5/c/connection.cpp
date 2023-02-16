/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file connection.cpp
 *
 *  @brief This file contains functions for the management of a connection object.
 */


#include <gmsec5/c/connection.h>
#include <gmsec5/c/config.h>

#include <gmsec5/Config.h>
#include <gmsec5/Connection.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/MessageFactory.h>
#include <gmsec5/Status.h>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


GMSEC_Connection CALL_TYPE connectionCreate(GMSEC_Config config, GMSEC_Status status)
{
	GMSEC_Connection conn = NULL;
	Status           result;

	Config* cfg = reinterpret_cast<Config*>(config);

	if (!cfg)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else
	{
		try
		{
			conn = reinterpret_cast<GMSEC_Connection>(new Connection(*cfg));
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return conn;
}


GMSEC_Connection CALL_TYPE connectionCreateWithFactory(GMSEC_Config config, GMSEC_MessageFactory factory, GMSEC_Status status)
{
	GMSEC_Connection conn = NULL;
	Status           result;

	Config*         c = reinterpret_cast<Config*>(config);
	MessageFactory* f = reinterpret_cast<MessageFactory*>(factory);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else if (!f)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "MessageFactory handle is NULL");
	}
	else
	{
		try
		{
			conn = reinterpret_cast<GMSEC_Connection>(new Connection(*c, *f));
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return conn;
}


void CALL_TYPE connectionDestroy(GMSEC_Connection* conn)
{
	if (conn && *conn)
	{
		Connection* c = reinterpret_cast<Connection*>(*conn);

		delete c;

		*conn = NULL;
	}
}


const char* CALL_TYPE connectionGetAPIVersion(void)
{
	return Connection::getAPIVersion();
}


void CALL_TYPE connectionConnect(GMSEC_Connection conn, GMSEC_Status status)
{
	Status result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		try
		{
			c->connect();
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE connectionDisconnect(GMSEC_Connection conn, GMSEC_Status status)
{
	Status result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		c->disconnect();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


const char* CALL_TYPE connectionGetLibraryName(GMSEC_Connection conn, GMSEC_Status status)
{
	const char* libName = NULL;
	Status      result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		libName = c->getLibraryName();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return libName;
}


const char* CALL_TYPE connectionGetLibraryVersion(GMSEC_Connection conn, GMSEC_Status status)
{
	const char* libVersion = NULL;
	Status      result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		libVersion = c->getLibraryVersion();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return libVersion;
}


GMSEC_MessageFactory CALL_TYPE connectionGetMessageFactory(GMSEC_Connection conn, GMSEC_Status status)
{
	GMSEC_MessageFactory factory = NULL;
	Status result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		factory = reinterpret_cast<GMSEC_MessageFactory>(&c->getMessageFactory());
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return factory;
}


void CALL_TYPE connectionRegisterEventCallback(GMSEC_Connection conn, GMSEC_ConnectionEvent event, GMSEC_EventCallback* cb, GMSEC_Status status)
{
	Status result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else if (!cb)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "EventCallback function is NULL");
	}
	else
	{
		int tmp = static_cast<int>(event);

		Connection::Event eventKind = static_cast<Connection::Event>(tmp);

		c->registerEventCallback(eventKind, cb);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_SubscriptionInfo CALL_TYPE connectionSubscribe(GMSEC_Connection conn, const char* subject, GMSEC_Status status)
{
	GMSEC_SubscriptionInfo info = NULL;
	Status                 result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		try
		{
			info = reinterpret_cast<GMSEC_SubscriptionInfo>(c->subscribe(subject));
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return info;
}


GMSEC_SubscriptionInfo CALL_TYPE connectionSubscribeWithCallback(GMSEC_Connection conn, const char* subject, GMSEC_Callback* cb, GMSEC_Status status)
{
	GMSEC_SubscriptionInfo info = NULL;
	Status                 result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else if (!cb)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Callback function is NULL");
	}
	else
	{
		try
		{
			Config cfg;  // define empty config

			info = reinterpret_cast<GMSEC_SubscriptionInfo>(c->subscribe(subject, cfg, cb));
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return info;
}


GMSEC_SubscriptionInfo CALL_TYPE connectionSubscribeWithConfig(GMSEC_Connection conn, const char* subject, GMSEC_Config config, GMSEC_Status status)
{
	GMSEC_SubscriptionInfo info = NULL;
	Status                 result;

	Connection* c   = reinterpret_cast<Connection*>(conn);
	Config*     cfg = reinterpret_cast<Config*>(config);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else if (!cfg)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else
	{
		try
		{
			info = reinterpret_cast<GMSEC_SubscriptionInfo>(c->subscribe(subject, *cfg));
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return info;
}


GMSEC_SubscriptionInfo CALL_TYPE connectionSubscribeWithConfigAndCallback(GMSEC_Connection conn, const char* subject, GMSEC_Config config, GMSEC_Callback* cb, GMSEC_Status status)
{
	GMSEC_SubscriptionInfo info = NULL;
	Status                 result;

	Connection* c   = reinterpret_cast<Connection*>(conn);
	Config*     cfg = reinterpret_cast<Config*>(config);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else if (!cfg)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else if (!cb)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Callback function is NULL");
	}
	else
	{
		try
		{
			info = reinterpret_cast<GMSEC_SubscriptionInfo>(c->subscribe(subject, *cfg, cb));
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return info;
}


void CALL_TYPE connectionUnsubscribe(GMSEC_Connection conn, GMSEC_SubscriptionInfo* info, GMSEC_Status status)
{
	Status result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else if (!info)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Reference to SubscriptionInfo handle is NULL");
	}
	else
	{
		try
		{
			SubscriptionInfo* i = reinterpret_cast<SubscriptionInfo*>(*info);

			c->unsubscribe(i);

			*info = NULL;
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE connectionStartAutoDispatch(GMSEC_Connection conn, GMSEC_Status status)
{
	GMSEC_BOOL started = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		try
		{
			started = (c->startAutoDispatch() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return started;
}


GMSEC_BOOL CALL_TYPE connectionStopAutoDispatch(GMSEC_Connection conn, GMSEC_Status status)
{
	GMSEC_BOOL stopped = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		try
		{
			stopped = (c->stopAutoDispatch() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return stopped;
}


void CALL_TYPE connectionPublish(GMSEC_Connection conn, GMSEC_Message msg, GMSEC_Status status)
{
	GMSEC_Config mwConfig = configCreate();  // empty config

	connectionPublishWithConfig(conn, msg, mwConfig, status);

	configDestroy(&mwConfig);
}


void CALL_TYPE connectionPublishWithConfig(GMSEC_Connection conn, GMSEC_Message msg, GMSEC_Config mwConfig, GMSEC_Status status)
{
	Status result;

	Connection* c   = reinterpret_cast<Connection*>(conn);
	Message*    m   = reinterpret_cast<Message*>(msg);
	Config*     cfg = reinterpret_cast<Config*>(mwConfig);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else if (!m)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else if (!cfg)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else
	{
		try
		{
			c->publish(*m, *cfg);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE connectionRequestWithCallback(GMSEC_Connection conn, GMSEC_Message request, GMSEC_I32 timeout, GMSEC_ReplyCallback* rcb, GMSEC_EventCallback* ecb, GMSEC_I32 republish_ms, GMSEC_Status status)
{
	Status result;

	Connection* c   = reinterpret_cast<Connection*>(conn);
	Message*    req = reinterpret_cast<Message*>(request);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else if (!req)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Request Message handle is NULL");
	}
	else if (!rcb)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "ReplyCallback function is NULL");
	}
	else if (!ecb)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "EventCallback function is NULL");
	}
	else
	{
		try
		{
			c->request(*req, timeout, rcb, ecb, republish_ms);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_Message CALL_TYPE connectionRequest(GMSEC_Connection conn, GMSEC_Message request, GMSEC_I32 timeout, GMSEC_I32 republish_ms, GMSEC_Status status)
{
	GMSEC_Message reply = NULL;
	Status        result;

	Connection* c   = reinterpret_cast<Connection*>(conn);
	Message*    req = reinterpret_cast<Message*>(request);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else if (!req)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Request Message handle is NULL");
	}
	else
	{
		try
		{
			reply = reinterpret_cast<GMSEC_Message>(c->request(*req, timeout, republish_ms));
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return reply;
}


void CALL_TYPE connectionReply(GMSEC_Connection conn, GMSEC_Message request, GMSEC_Message reply, GMSEC_Status status)
{
	Status result;

	Connection* c   = reinterpret_cast<Connection*>(conn);
	Message*    req = reinterpret_cast<Message*>(request);
	Message*    rep = reinterpret_cast<Message*>(reply);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else if (!req)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Request Message handle is NULL");
	}
	else if (!rep)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Reply Message handle is NULL");
	}
	else
	{
		try
		{
			c->reply(*req, *rep);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE connectionDispatch(GMSEC_Connection conn, GMSEC_Message msg, GMSEC_Status status)
{
	Status result;

	Connection* c = reinterpret_cast<Connection*>(conn);
	Message*    m = reinterpret_cast<Message*>(msg);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else if (!m)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			c->dispatch(*m);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_Message CALL_TYPE connectionReceive(GMSEC_Connection conn, GMSEC_I32 timeout, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		try
		{
			msg = reinterpret_cast<GMSEC_Message>(c->receive(timeout));
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


void CALL_TYPE connectionExcludeSubject(GMSEC_Connection conn, const char* subject, GMSEC_Status status)
{
	Status result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		try
		{
			c->excludeSubject(subject);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE connectionRemoveExcludedSubject(GMSEC_Connection conn, const char* subject, GMSEC_Status status)
{
	Status result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		try
		{
			c->removeExcludedSubject(subject);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


const char* CALL_TYPE connectionGetName(GMSEC_Connection conn, GMSEC_Status status)
{
	const char* name = NULL;
	Status      result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		name = c->getName();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return name;
}


void CALL_TYPE connectionSetName(GMSEC_Connection conn, const char* name, GMSEC_Status status)
{
	Status result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		try
		{
			c->setName(name);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


const char* CALL_TYPE connectionGetID(GMSEC_Connection conn, GMSEC_Status status)
{
	const char* id = NULL;
	Status      result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		id = c->getID();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return id;
}


const char* CALL_TYPE connectionGetMWInfo(GMSEC_Connection conn, GMSEC_Status status)
{
	const char* info = NULL;
	Status      result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		info = c->getMWInfo();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return info;
}


const char* CALL_TYPE connectionGetConnectionEndpoint(GMSEC_Connection conn, GMSEC_Status status)
{
	const char* endpoint = NULL;
	Status      result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		endpoint = c->getConnectionEndpoint();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return endpoint;
}


GMSEC_U64 CALL_TYPE connectionGetPublishQueueMessageCount(GMSEC_Connection conn, GMSEC_Status status)
{
	GMSEC_U64 count = 0;
	Status    result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		count = c->getPublishQueueMessageCount();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return count;
}


void CALL_TYPE connectionShutdownAllMiddlewares(void)
{
	Connection::shutdownAllMiddlewares();
}


void CALL_TYPE connectionShutdownMiddleware(const char* name)
{
	Connection::shutdownMiddleware(name);
}
