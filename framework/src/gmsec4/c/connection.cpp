/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file connection.cpp
 *
 *  @brief This file contains functions for the management of a connection object.
 */


#include <gmsec4/c/connection.h>

#include <gmsec4/Config.h>
#include <gmsec4/Connection.h>
#include <gmsec4/Exception.h>

using namespace gmsec::api;



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
			conn = reinterpret_cast<GMSEC_Connection>(Connection::create(*cfg));
		}
		catch (Exception& e)
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

		Connection::destroy(c);

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
		catch (Exception& e)
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
		try
		{
			c->disconnect();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_ConnectionState CALL_TYPE connectionGetState(const GMSEC_Connection conn, GMSEC_Status status)
{
	GMSEC_ConnectionState state = GMSEC_NOT_CONNECTED;
	Status                result;

	const Connection* c = reinterpret_cast<const Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		int tmp = c->getState();

		state = static_cast<GMSEC_ConnectionState>(tmp);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return state;
}


const char* CALL_TYPE connectionGetLibraryRootName(const GMSEC_Connection conn, GMSEC_Status status)
{
	const char* libName = NULL;
	Status      result;

	const Connection* c = reinterpret_cast<const Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		try
		{
			libName = c->getLibraryRootName();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return libName;
}


const char* CALL_TYPE connectionGetLibraryVersion(const GMSEC_Connection conn, GMSEC_Status status)
{
	const char* libVersion = NULL;
	Status      result;

	const Connection* c = reinterpret_cast<const Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else
	{
		try
		{
			libVersion = c->getLibraryVersion();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return libVersion;
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
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "EventCallback is NULL");
	}
	else
	{
		try
		{
			int tmp = event;

			Connection::ConnectionEvent eventKind = static_cast<Connection::ConnectionEvent>(tmp);

			c->registerEventCallback(eventKind, cb);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_SubscriptionInfo CALL_TYPE connectionSubscribe(GMSEC_Connection conn, const char* subject, GMSEC_Status status)
{
	GMSEC_SubscriptionInfo sinfo = { NULL, GMSEC_FALSE };
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
			gmsec::api::SubscriptionInfo* info = c->subscribe(subject);

			sinfo.info     = info;
			sinfo.fromMIST = GMSEC_FALSE;
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return sinfo;
}


GMSEC_SubscriptionInfo CALL_TYPE connectionSubscribeWithCallback(GMSEC_Connection conn, const char* subject, GMSEC_Callback* cb, GMSEC_Status status)
{
	GMSEC_SubscriptionInfo sinfo = { NULL, GMSEC_FALSE };
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
			Config cfg;  // define empty config

			gmsec::api::SubscriptionInfo* info = c->subscribe(subject, cfg, cb);

			sinfo.info     = info;
			sinfo.fromMIST = GMSEC_FALSE;
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return sinfo;
}


GMSEC_SubscriptionInfo CALL_TYPE connectionSubscribeWithConfig(GMSEC_Connection conn, const char* subject, GMSEC_Config config, GMSEC_Status status)
{
	GMSEC_SubscriptionInfo sinfo = { NULL, GMSEC_FALSE };
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
			gmsec::api::SubscriptionInfo* info = c->subscribe(subject, *cfg);

			sinfo.info     = info;
			sinfo.fromMIST = GMSEC_FALSE;
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return sinfo;
}


GMSEC_SubscriptionInfo CALL_TYPE connectionSubscribeWithConfigAndCallback(GMSEC_Connection conn, const char* subject, GMSEC_Config config, GMSEC_Callback* cb, GMSEC_Status status)
{
	GMSEC_SubscriptionInfo sinfo = { NULL, GMSEC_FALSE };
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
			gmsec::api::SubscriptionInfo* info = c->subscribe(subject, *cfg, cb);

			sinfo.info     = info;
			sinfo.fromMIST = GMSEC_FALSE;
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return sinfo;
}


void CALL_TYPE connectionUnsubscribe(GMSEC_Connection conn, GMSEC_SubscriptionInfo* sinfo, GMSEC_Status status)
{
	Status result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else if (!sinfo || !sinfo->info)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Reference to SubscriptionInfo handle is NULL");
	}
	else if (sinfo->fromMIST == GMSEC_TRUE)
	{
		result = Status(CONNECTION_ERROR, OTHER_ERROR_CODE, "SubscriptionInfo originated from ConnectionManager");
	}
	else
	{
		try
		{
			gmsec::api::SubscriptionInfo* i = reinterpret_cast<gmsec::api::SubscriptionInfo*>(sinfo->info);

			c->unsubscribe(i);

			sinfo->info = NULL;
		}
		catch (Exception& e)
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
	GMSEC_BOOL started = GMSEC_FALSE;
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
			started = (c->startAutoDispatch() ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
	GMSEC_BOOL stopped = GMSEC_FALSE;
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
			stopped = (c->stopAutoDispatch() ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
			c->publish(*m);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
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
		catch (Exception& e)
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
	else
	{
		try
		{
			c->request(*req, timeout, rcb, ecb, republish_ms);
		}
		catch (Exception& e)
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
		catch (Exception& e)
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


void CALL_TYPE connectionCancelRequest(GMSEC_Connection conn, GMSEC_ReplyCallback* cb, GMSEC_Status status)
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
			c->cancelRequest(cb);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
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
		catch (Exception& e)
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
		catch (Exception& e)
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
		catch (Exception& e)
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


void CALL_TYPE connectionRelease(GMSEC_Connection conn, GMSEC_Message* msg, GMSEC_Status status)
{
	Status result;

	Connection* c = reinterpret_cast<Connection*>(conn);

	if (!c)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Connection handle is NULL");
	}
	else if (!msg || !*msg)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Reference to Message handle is NULL");
	}
	else
	{
		Message* m = reinterpret_cast<Message*>(*msg);

		try
		{
			c->release(m);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}

		*msg = NULL;
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
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
		catch (Exception& e)
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
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


const char* CALL_TYPE connectionGetName(const GMSEC_Connection conn, GMSEC_Status status)
{
	const char* name = NULL;
	Status      result;

	const Connection* c = reinterpret_cast<const Connection*>(conn);

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
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


const char* CALL_TYPE connectionGetID(const GMSEC_Connection conn, GMSEC_Status status)
{
	const char* id = NULL;
	Status      result;

	const Connection* c = reinterpret_cast<const Connection*>(conn);

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


const char* CALL_TYPE connectionGetMWInfo(const GMSEC_Connection conn, GMSEC_Status status)
{
	const char* info = NULL;
	Status      result;

	const Connection* c = reinterpret_cast<const Connection*>(conn);

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


const char* CALL_TYPE connectionGetConnectionEndpoint(const GMSEC_Connection conn, GMSEC_Status status)
{
	const char* endpoint = NULL;
	Status      result;

	const Connection* c = reinterpret_cast<const Connection*>(conn);

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


GMSEC_U64 CALL_TYPE connectionGetPublishQueueMessageCount(const GMSEC_Connection conn, GMSEC_Status status)
{
	GMSEC_U64 count = 0;
	Status    result;

	const Connection* c = reinterpret_cast<const Connection*>(conn);

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
