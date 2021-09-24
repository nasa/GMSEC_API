/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file connection_manager.cpp
 *
 * @brief This file contains functions for the management of a MIST ConnectionManager object.
 * The ConnectionManager wraps the GMSEC Connection object, supplying both normal
 * middleware interaction functions and some additional quality of service. 
 * This is accomplished through standard services for producing Log and 
 * Heartbeat messages, as well as functionality for validating messages. 
 * 
 * If the ConnectionManager is configured to validate messages, all messages sent 
 * from this ConnectionManager via either publish or eequest will be compared
 * to an available schema definition. If a template definition for the message
 * in question is available, it will be validated. The message will not be sent
 * over the middleware if validation fails. Messages without a template
 * definition can be sent as well.
 */


#include <gmsec4/c/mist/connection_manager.h>

#include <gmsec4/mist/ConnectionManager.h>

#include <gmsec4/Config.h>
#include <gmsec4/Connection.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/DataList.h>

#include <cstdlib>   // for malloc() and free()

using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::util;


const char* CALL_TYPE connectionManagerGetAPIVersion()
{
	return ConnectionManager::getAPIVersion();
}


GMSEC_ConnectionMgr CALL_TYPE connectionManagerCreate(const GMSEC_Config config, GMSEC_Status status)
{
	GMSEC_ConnectionMgr connMgr = NULL;
	Status              result;

	const Config* cfg = reinterpret_cast<const Config*>(config);

	if (!cfg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else
	{
		try
		{
			connMgr = reinterpret_cast<GMSEC_ConnectionMgr>(new ConnectionManager(*cfg));
		}
		catch (const Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return connMgr;
}


GMSEC_ConnectionMgr CALL_TYPE connectionManagerCreateUsingValidation(const GMSEC_Config config, GMSEC_BOOL validate, GMSEC_Status status)
{
	GMSEC_ConnectionMgr connMgr = NULL;
	Status              result;

	const Config* cfg = reinterpret_cast<const Config*>(config);

	if (!cfg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else
	{
		try
		{
			connMgr = reinterpret_cast<GMSEC_ConnectionMgr>(new ConnectionManager(*cfg, validate == GMSEC_TRUE));
		}
		catch (const Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return connMgr;
}


GMSEC_ConnectionMgr CALL_TYPE connectionManagerCreateUsingISD(const GMSEC_Config config, GMSEC_BOOL validate, unsigned int version, GMSEC_Status status)
{
	GMSEC_ConnectionMgr connMgr = NULL;
	Status              result;

	const Config* cfg = reinterpret_cast<const Config*>(config);

	if (!cfg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else
	{
		try
		{
			connMgr = reinterpret_cast<GMSEC_ConnectionMgr>(new ConnectionManager(*cfg, validate == GMSEC_TRUE, version));
		}
		catch (const Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return connMgr;
}


void CALL_TYPE connectionManagerDestroy(GMSEC_ConnectionMgr* connMgr)
{
	if (connMgr && *connMgr)
	{
		ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(*connMgr);

		delete mgr;

		*connMgr = NULL;
	}
}


void CALL_TYPE connectionManagerInitialize(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			mgr->initialize();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerCleanup(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			mgr->cleanup();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


GMSEC_ConnectionState CALL_TYPE connectionManagerGetState(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	Status                result;
	GMSEC_ConnectionState state = GMSEC_NOT_CONNECTED;
	ConnectionManager*    mgr   = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		switch (mgr->getState())
		{
			case Connection::NOT_CONNECTED: state = GMSEC_NOT_CONNECTED; break;
			case Connection::CONNECTED:     state = GMSEC_CONNECTED;     break;
			case Connection::RECONNECTING:  state = GMSEC_RECONNECTING;  break;
		}
	}    

	if (status)
	{
		*((Status*) status) = result;
	}

	return state;
}


const char* CALL_TYPE connectionManagerGetLibraryRootName(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	Status             result;
	const char*        rootname = NULL;
	ConnectionManager* mgr      = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		rootname = mgr->getLibraryRootName();
	}    

	if (status)
	{
		*((Status*) status) = result;
	}

	return rootname;
}


const char* CALL_TYPE connectionManagerGetLibraryVersion(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	Status             result;
	const char*        libver = NULL;
	ConnectionManager* mgr    = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		libver = mgr->getLibraryVersion();
	}    

	if (status)
	{
		*((Status*) status) = result;
	}

	return libver;
}


GMSEC_Specification CALL_TYPE connectionManagerGetSpecification(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	GMSEC_Specification spec = NULL;
	Status              result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			Specification& s = mgr->getSpecification();

			spec = reinterpret_cast<GMSEC_Specification>(&s);
		}
		catch (const Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return spec;
}


void CALL_TYPE connectionManagerRegisterMessageValidator(GMSEC_ConnectionMgr connMgr,
                                                         GMSEC_MessageValidator* validator,
                                                         GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (mgr == NULL)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (validator == NULL || *validator == NULL)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MessageValidator function is NULL");
	}
	else
	{
		mgr->registerMessageValidator(validator);
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerSetStandardFields(GMSEC_ConnectionMgr connMgr, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!fields || numFields == 0)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Array of standard Fields is NULL or field count is zero");
	}
	else
	{
		DataList<Field*> standardFields;

		for (size_t i = 0; i < numFields; ++i)
		{
			standardFields.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		try
		{
			mgr->setStandardFields(standardFields);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


GMSEC_Field* CALL_TYPE connectionManagerGetStandardFields(GMSEC_ConnectionMgr connMgr, size_t* numFields, GMSEC_Status status)
{
	GMSEC_Field* fields = NULL;
	Status       result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		const DataList<Field*>& standardFields = mgr->getStandardFields();

		*numFields = standardFields.size();
		fields     = (void**) std::malloc(*numFields * sizeof(GMSEC_Field));

		if (fields == NULL)
		{
			result = Status(MIST_ERROR, OUT_OF_MEMORY, "Unable to allocate memory for GMSEC_Field array");
		}
		else
		{
			int i = 0;
			for (DataList<Field*>::iterator it = standardFields.begin(); it != standardFields.end(); ++it, ++i)
			{
				fields[i] = *it;
			}
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return fields;
}


void CALL_TYPE connectionManagerAddStandardFields(const GMSEC_ConnectionMgr connMgr, GMSEC_Message msg, GMSEC_Status status)
{
	Status result;

	const ConnectionManager* mgr = reinterpret_cast<const ConnectionManager*>(connMgr);
	Message*                 m   = reinterpret_cast<Message*>(msg);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!m)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		mgr->addStandardFields(*m);
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerRegisterEventCallback(GMSEC_ConnectionMgr connMgr, GMSEC_ConnectionEvent event, GMSEC_ConnectionMgrEventCallback* ecb, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			int tmp = event;
			mgr->registerEventCallback(static_cast<Connection::ConnectionEvent>(tmp), ecb);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


GMSEC_SubscriptionInfo CALL_TYPE connectionManagerSubscribe(GMSEC_ConnectionMgr connMgr, const char* subject, GMSEC_Status status)
{
	GMSEC_SubscriptionInfo sinfo = { NULL, GMSEC_FALSE};
	Status                 result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			gmsec::api::mist::SubscriptionInfo* info = mgr->subscribe(subject);

			sinfo.info     = info;
			sinfo.fromMIST = GMSEC_TRUE;
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return sinfo;
}


GMSEC_SubscriptionInfo CALL_TYPE connectionManagerSubscribeWithCallback(GMSEC_ConnectionMgr connMgr, const char* subject, GMSEC_ConnectionMgrCallback* cb, GMSEC_Status status)
{
	GMSEC_SubscriptionInfo sinfo = { NULL, GMSEC_FALSE};
	Status                 result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			Config cfg;  // define empty config

			gmsec::api::mist::SubscriptionInfo* info = mgr->subscribe(subject, cfg, cb);

			sinfo.info     = info;
			sinfo.fromMIST = GMSEC_TRUE;
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return sinfo;
}


GMSEC_SubscriptionInfo CALL_TYPE connectionManagerSubscribeWithConfig(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Config config, GMSEC_Status status)
{
	GMSEC_SubscriptionInfo sinfo = { NULL, GMSEC_FALSE };
	Status                 result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Config*      cfg = reinterpret_cast<Config*>(config);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!cfg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else
	{
		try
		{
			gmsec::api::mist::SubscriptionInfo* info = mgr->subscribe(subject, *cfg);

			sinfo.info     = info;
			sinfo.fromMIST = GMSEC_TRUE;
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return sinfo;
}


GMSEC_SubscriptionInfo CALL_TYPE connectionManagerSubscribeWithConfigAndCallback(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Config config, GMSEC_ConnectionMgrCallback* cb, GMSEC_Status status)
{
	GMSEC_SubscriptionInfo sinfo = { NULL, GMSEC_FALSE };
	Status                 result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Config*      cfg = reinterpret_cast<Config*>(config);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!cfg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else
	{
		try
		{
			gmsec::api::mist::SubscriptionInfo* info = mgr->subscribe(subject, *cfg, cb);

			sinfo.info     = info;
			sinfo.fromMIST = GMSEC_TRUE;
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return sinfo;
}


void CALL_TYPE connectionManagerUnsubscribe(GMSEC_ConnectionMgr connMgr, GMSEC_SubscriptionInfo* sinfo, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!sinfo || !sinfo->info)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Reference to SubscriptionInfo handle is NULL");
	}
	else if (sinfo->fromMIST == GMSEC_FALSE)
	{
		result = Status(MIST_ERROR, OTHER_ERROR_CODE, "SubscriptionInfo did not originate from ConnectionManager");
	}
	else
	{
		try
		{
			gmsec::api::mist::SubscriptionInfo* i = reinterpret_cast<gmsec::api::mist::SubscriptionInfo*>(sinfo->info);

			mgr->unsubscribe(i);

			sinfo->info = NULL;
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


GMSEC_BOOL CALL_TYPE connectionManagerStartAutoDispatch(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	GMSEC_BOOL started = GMSEC_FALSE;
	Status     result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			started = (mgr->startAutoDispatch() ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return started;
}


GMSEC_BOOL CALL_TYPE connectionManagerStopAutoDispatch(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	GMSEC_BOOL stopped = GMSEC_FALSE;
	Status     result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			stopped = (mgr->stopAutoDispatch() ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return stopped;
}


void CALL_TYPE connectionManagerPublish(GMSEC_ConnectionMgr connMgr, const GMSEC_Message msg, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Message*     m   = reinterpret_cast<const Message*>(msg);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!m)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			mgr->publish(*m);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerPublishWithConfig(GMSEC_ConnectionMgr connMgr, const GMSEC_Message msg, const GMSEC_Config mwConfig, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Message*     m   = reinterpret_cast<const Message*>(msg);
	const Config*      cfg = reinterpret_cast<const Config*>(mwConfig);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!m)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else if (!cfg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else
	{
		try
		{
			mgr->publish(*m, *cfg);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerRequestWithCallback(GMSEC_ConnectionMgr connMgr, const GMSEC_Message request, GMSEC_I32 timeout, GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb, GMSEC_I32 republish_ms, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Message*     req = reinterpret_cast<const Message*>(request);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!req)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Request Message handle is NULL");
	}
	else
	{
		try
		{
			mgr->request(*req, timeout, rcb, ecb, republish_ms);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


GMSEC_Message CALL_TYPE connectionManagerRequest(GMSEC_ConnectionMgr connMgr, const GMSEC_Message request, GMSEC_I32 timeout, GMSEC_I32 republish_ms, GMSEC_Status status)
{
	GMSEC_Message reply = NULL;
	Status        result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Message*     req = reinterpret_cast<const Message*>(request);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!req)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Request Message handle is NULL");
	}
	else
	{
		try
		{
			reply = reinterpret_cast<GMSEC_Message>(mgr->request(*req, timeout, republish_ms));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return reply;
}


void CALL_TYPE connectionManagerCancelRequest(GMSEC_ConnectionMgr connMgr, GMSEC_ConnectionMgrReplyCallback* cb, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			mgr->cancelRequest(cb);
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


void CALL_TYPE connectionManagerReply(GMSEC_ConnectionMgr connMgr, const GMSEC_Message request, const GMSEC_Message reply, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Message*     req = reinterpret_cast<const Message*>(request);
	const Message*     rep = reinterpret_cast<const Message*>(reply);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!req)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Request Message handle is NULL");
	}
	else if (!rep)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Reply Message handle is NULL");
	}
	else
	{
		try
		{
			mgr->reply(*req, *rep);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerDispatch(GMSEC_ConnectionMgr connMgr, const GMSEC_Message msg, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Message*     m   = reinterpret_cast<const Message*>(msg);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!msg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			mgr->dispatch(*m);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


GMSEC_Message CALL_TYPE connectionManagerReceive(GMSEC_ConnectionMgr connMgr, GMSEC_I32 timeout, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			msg = reinterpret_cast<GMSEC_Message>(mgr->receive(timeout));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return msg;
}


void CALL_TYPE connectionManagerRelease(GMSEC_ConnectionMgr connMgr, GMSEC_Message* msg, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!msg || !*msg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Reference to Message handle is NULL");
	}
	else
	{
		Message* m = reinterpret_cast<Message*>(*msg);

		try
		{
			mgr->release(m);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerExcludeSubject(GMSEC_ConnectionMgr connMgr, const char* subject, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			mgr->excludeSubject(subject);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerRemoveExcludedSubject(GMSEC_ConnectionMgr connMgr, const char* subject, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			mgr->removeExcludedSubject(subject);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


const char* CALL_TYPE connectionManagerGetName(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	Status             result;
	const char*        name = NULL;
	ConnectionManager* mgr  = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		name = mgr->getName();
	}    

	if (status)
	{
		*((Status*) status) = result;
	}

	return name;
}


void CALL_TYPE connectionManagerSetName(GMSEC_ConnectionMgr connMgr, const char* name, GMSEC_Status status)
{
	Status             result;
	ConnectionManager* mgr  = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			mgr->setName(name);
		}
		catch (const Exception& e)
		{
			result = Status(e);
		}
	}    

	if (status)
	{
		*((Status*) status) = result;
	}
}


const char* CALL_TYPE connectionManagerGetID(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	Status             result;
	const char*        id  = NULL;
	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		id = mgr->getID();
	}    

	if (status)
	{
		*((Status*) status) = result;
	}

	return id;
}


const char* CALL_TYPE connectionManagerGetMWInfo(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	Status             result;
	const char*        info = NULL;
	ConnectionManager* mgr  = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		info = mgr->getMWInfo();
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return info;
}


const char* CALL_TYPE connectionManagerGetConnectionEndpoint(const GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	Status             result;
	const char*        endpoint = NULL;
	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		endpoint = mgr->getConnectionEndpoint();
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return endpoint;
}


GMSEC_U64 CALL_TYPE connectionManagerGetPublishQueueMessageCount(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	Status             result;
	GMSEC_U64          count = 0;
	ConnectionManager* mgr   = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		count = mgr->getPublishQueueMessageCount();
	}    

	if (status)
	{
		*((Status*) status) = result;
	}

	return count;
}


GMSEC_Message CALL_TYPE connectionManagerCreateHeartbeatMessage(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		DataList<Field*> standardFields;

		for (size_t i = 0; i < numFields; ++i)
		{
			standardFields.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		try
		{
			Message hbMsg = mgr->createHeartbeatMessage(subject, standardFields);

			msg = reinterpret_cast<GMSEC_Message>(new Message(hbMsg));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return msg;
}


void CALL_TYPE connectionManagerStartHeartbeatService(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		DataList<Field*> standardFields;

		for (size_t i = 0; i < numFields; ++i)
		{
			standardFields.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		try
		{
			mgr->startHeartbeatService(subject, standardFields);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerStopHeartbeatService(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			mgr->stopHeartbeatService();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


// deprecated
void CALL_TYPE connectionManagerChangeComponentStatus(GMSEC_ConnectionMgr connMgr, const GMSEC_Field componentStatus, GMSEC_Status status)
{
	connectionManagerSetHeartbeatServiceField(connMgr, componentStatus, status);
}


// deprecated
void CALL_TYPE connectionManagerChangeComponentInfo(GMSEC_ConnectionMgr connMgr, const GMSEC_Field componentInfo, GMSEC_Status status)
{
	connectionManagerSetHeartbeatServiceField(connMgr, componentInfo, status);
}


// deprecated
void CALL_TYPE connectionManagerChangeCPUMemory(GMSEC_ConnectionMgr connMgr, const GMSEC_Field cpuMemory, GMSEC_Status status)
{
	connectionManagerSetHeartbeatServiceField(connMgr, cpuMemory, status);
}


// deprecated
void CALL_TYPE connectionManagerChangeCPUUtil(GMSEC_ConnectionMgr connMgr, const GMSEC_Field cpuUtil, GMSEC_Status status)
{
	connectionManagerSetHeartbeatServiceField(connMgr, cpuUtil, status);
}


void CALL_TYPE connectionManagerSetHeartbeatServiceField(GMSEC_ConnectionMgr connMgr, const GMSEC_Field field, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Field*       fld = reinterpret_cast<const Field*>(field);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!fld)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Field handle is NULL");
	}
	else
	{
		result = mgr->setHeartbeatServiceField(*fld);
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


GMSEC_Message CALL_TYPE connectionManagerCreateLogMessage(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		DataList<Field*> standardFields;

		for (size_t i = 0; i < numFields; ++i)
		{
			standardFields.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		try
		{
			Message logMsg = mgr->createLogMessage(subject, standardFields);

			msg = reinterpret_cast<GMSEC_Message>(new Message(logMsg));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return msg;
}


void CALL_TYPE connectionManagerSetLoggingDefaults(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		DataList<Field*> standardFields;

		for (size_t i = 0; i < numFields; ++i)
		{
			standardFields.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		try
		{
			mgr->setLoggingDefaults(subject, standardFields);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerPublishLog(GMSEC_ConnectionMgr connMgr, const char* logMsg, const GMSEC_Field severity, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Field*       fld = reinterpret_cast<const Field*>(severity);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!fld)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Severity Field handle is NULL");
	}
	else
	{
		try
		{
			mgr->publishLog(logMsg, *fld);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerRequestDirectiveNoReply(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field dirString, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Field*       fld = reinterpret_cast<const Field*>(dirString);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!fld)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "DirectiveString Field handle is NULL");
	}
	else
	{
		DataList<Field*> fieldList;
		for (size_t i = 0; i < numFields; ++i)
		{
			fieldList.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		try
		{
			mgr->requestDirective(subject, *fld, fieldList);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerRequestDirectiveWithCallback(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field dirString, const GMSEC_Field fields[], size_t numFields, GMSEC_I32 timeout, GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb, GMSEC_I32 republish_ms, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Field*       fld = reinterpret_cast<const Field*>(dirString);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!fld)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "DirectiveString Field handle is NULL");
	}
	else
	{
		DataList<Field*> fieldList;
		for (size_t i = 0; i < numFields; ++i)
		{
			fieldList.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		try
		{
			mgr->requestDirective(subject, *fld, fieldList, timeout, rcb, ecb, republish_ms);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


GMSEC_Message CALL_TYPE connectionManagerRequestDirective(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field dirString, const GMSEC_Field fields[], size_t numFields, GMSEC_I32 timeout, GMSEC_I32 republish_ms, GMSEC_Status status)
{
	GMSEC_Message reply = NULL;
	Status        result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Field*       fld = reinterpret_cast<const Field*>(dirString);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!fld)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "DirectiveString Field handle is NULL");
	}
	else
	{
		DataList<Field*> fieldList;
		for (size_t i = 0; i < numFields; ++i)
		{
			fieldList.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		try
		{
			reply = reinterpret_cast<GMSEC_Message*>(mgr->requestDirective(subject, *fld, fieldList, timeout, republish_ms));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return reply;
}


void CALL_TYPE connectionManagerAckDirectiveRequest(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Message request, GMSEC_ResponseStatus response, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Message*     req = reinterpret_cast<const Message*>(request);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!req)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Request Message handle is NULL");
	}
	else
	{
		DataList<Field*> fieldList;
		for (size_t i = 0; i < numFields; ++i)
		{
			fieldList.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		try
		{
			int tmp = response;

			mgr->acknowledgeDirectiveRequest(subject, *req, static_cast<ResponseStatus::Response>(tmp), fieldList);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerPublishResourceMessage(GMSEC_ConnectionMgr connMgr, const char* subject, size_t sampleInterval, size_t averageInterval, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			mgr->publishResourceMessage(subject, sampleInterval, averageInterval);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


GMSEC_Message CALL_TYPE connectionManagerCreateResourceMessage(GMSEC_ConnectionMgr connMgr, const char* subject, size_t sampleInterval, size_t averageInterval, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			Message resMsg = mgr->createResourceMessage(subject, sampleInterval, averageInterval);

			msg = reinterpret_cast<GMSEC_Message*>(new Message(resMsg));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return msg;
}


void CALL_TYPE connectionManagerStartResourceMessageService(GMSEC_ConnectionMgr connMgr, const char* subject, size_t intervalSeconds, size_t sampleInterval, size_t averageInterval, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			mgr->startResourceMessageService(subject, intervalSeconds, sampleInterval, averageInterval);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerStopResourceMessageService(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else
	{
		try
		{
			mgr->stopResourceMessageService();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerAckSimpleService(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Message request, GMSEC_ResponseStatus response, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Message*     req = reinterpret_cast<const Message*>(request);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!req)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Request Message handle is NULL");
	}
	else
	{
		DataList<Field*> fieldList;
		for (size_t i = 0; i < numFields; ++i)
		{
			fieldList.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		try
		{
			int tmp = response;

			mgr->acknowledgeSimpleService(subject, *req, static_cast<ResponseStatus::Response>(tmp), fieldList);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerRequestSimpleServiceNoReply(GMSEC_ConnectionMgr connMgr, const char* subject, const char* opName, const GMSEC_Field opNumber, const GMSEC_Field fields[], size_t numFields, const GMSEC_ServiceParam params[], size_t numParams, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Field*       fld = reinterpret_cast<const Field*>(opNumber);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!fld)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "OpNumber Field handle is NULL");
	}
	else
	{
		DataList<Field*> fieldList;
		for (size_t i = 0; i < numFields; ++i)
		{
			fieldList.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		DataList<ServiceParam*> paramList;
		for (size_t i = 0; i < numParams; ++i)
		{
			paramList.push_back(reinterpret_cast<ServiceParam*>(params[i]));
		}

		try
		{
			mgr->requestSimpleService(subject, opName, *fld, fieldList, paramList);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE connectionManagerRequestSimpleServiceWithCallback(GMSEC_ConnectionMgr connMgr, const char* subject, const char* opName, const GMSEC_Field opNumber,
		const GMSEC_Field fields[], size_t numFields,
		const GMSEC_ServiceParam params[], size_t numParams,
		GMSEC_I32 timeout,
		GMSEC_ConnectionMgrReplyCallback* rcb,
		GMSEC_ConnectionMgrEventCallback* ecb,
		GMSEC_I32 republish_ms, GMSEC_Status status)
{
	Status result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Field*       fld = reinterpret_cast<const Field*>(opNumber);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!fld)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "OpNumber Field handle is NULL");
	}
	else
	{
		DataList<Field*> fieldList;
		for (size_t i = 0; i < numFields; ++i)
		{
			fieldList.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		DataList<ServiceParam*> paramList;
		for (size_t i = 0; i < numParams; ++i)
		{
			paramList.push_back(reinterpret_cast<ServiceParam*>(params[i]));
		}

		try
		{
			mgr->requestSimpleService(subject, opName, *fld, fieldList, paramList, timeout, rcb, ecb, republish_ms);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


GMSEC_Message CALL_TYPE connectionManagerRequestSimpleService(GMSEC_ConnectionMgr connMgr, const char* subject, const char* opName, const GMSEC_Field opNumber,
		const GMSEC_Field fields[], size_t numFields,
		const GMSEC_ServiceParam params[], size_t numParams,
		GMSEC_I32 timeout, GMSEC_I32 republish_ms, GMSEC_Status status)
{
	GMSEC_Message reply = NULL;
	Status        result;

	ConnectionManager* mgr = reinterpret_cast<ConnectionManager*>(connMgr);
	const Field*       fld = reinterpret_cast<const Field*>(opNumber);

	if (!mgr)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ConnectionManager handle is NULL");
	}
	else if (!fld)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "OpNumber Field handle is NULL");
	}
	else
	{
		DataList<Field*> fieldList;
		for (size_t i = 0; i < numFields; ++i)
		{
			fieldList.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		DataList<ServiceParam*> paramList;
		for (size_t i = 0; i < numParams; ++i)
		{
			paramList.push_back(reinterpret_cast<ServiceParam*>(params[i]));
		}

		try
		{
			reply = reinterpret_cast<GMSEC_Message*>(mgr->requestSimpleService(subject, opName, *fld, fieldList, paramList, timeout, republish_ms));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return reply;
}


void CALL_TYPE connectionManagerShutdownAllMiddlewares(void)
{
	Connection::shutdownAllMiddlewares();
}


void CALL_TYPE freeFieldArray(GMSEC_Field* array)
{
	std::free(array);
}
