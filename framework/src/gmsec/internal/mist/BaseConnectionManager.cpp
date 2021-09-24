/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file BaseConnectionManager.cpp
 *
 *  @brief This file contains a class for the services portion of MIST.
 *
**/

#include <gmsec/internal/mist/BaseConnectionManager.h>

#include <gmsec/mist/ConnectionManagerCallback.h>
#include <gmsec/mist/ConnectionManagerErrorCallback.h>
#include <gmsec/mist/ConnectionManagerReplyCallback.h>
#include <gmsec/mist/MIST_errors.h>

#include <gmsec/internal/API4_Adapter.h>
#include <gmsec/internal/mist/Context.h>
#include <gmsec/internal/mist/ConnMgrCallbacks.h>
#include <gmsec/internal/mist/ConnMgrCallbackCache.h>

#include <gmsec/Connection.h>
#include <gmsec/ConnectionFactory.h>
#include <gmsec/Field.h>
#include <gmsec/Message.h>

#include <gmsec/internal/strutil.h>

#include <gmsec/util/Log.h>
#include <gmsec/util/sysutil.h>
#include <gmsec/util/timeutil.h>

#include <gmsec/util/sysutil.h>
#include <gmsec/util/Mutex.h>
#include <gmsec/util/Atomics.h>
#include <gmsec/util/timeutil.h>

#include <gmsec4/mist/Specification.h>

#include <gmsec4/Exception.h>

#include <algorithm>
#include <string>
#include <vector>

#ifdef __sun
#include <string.h>
#endif


namespace gmsec
{

namespace mist
{

namespace internal
{

BaseConnectionManager::BaseConnectionManager(Config &cfg, unsigned int version, bool validate)
	: specVersion(version),
	specification(0),
	config(cfg),
	validateMessages(validate),
	hbService(0),
	standardFieldsAllMessages(0),
	countStandardFieldsAllMessages(0),
	standardHeartbeatFields(0),
	countStandardHeartbeatFields(0),
	standardLogFields(0),
	countStandardLogFields(0),
	connection(0)
{
	// Determine if the spec version is defined within the Config object
	const char *specVersionStr = 0;
	Status status = cfg.GetValue("GMSEC-SPECIFICATION-VERSION", specVersionStr);

	if (!status.IsError())
	{
		// Get the configured value
		int specVersionInt = 0;
		util::str2int(specVersionInt, specVersionStr);

		specVersion = specVersionInt;
	}

	// Verify specification version is valid; if not, default to current version.
	if (!CheckValidSpec(specVersion))
	{
		LOG_WARNING << "Invalid message specification version supplied, defaulting to GMSEC_ISD_CURRENT";
		specVersion = ConnectionManager::GMSEC_ISD_CURRENT;
	}


	// Determine whether or not to validate messages from the Config object
	const char *validateStr = 0;
	status = cfg.GetValue("GMSEC-MSG-CONTENT-VALIDATE", validateStr);

	if (!status.IsError())
	{
		validateMessages = util::stringEqualsIgnoreCase(validateStr, "true");
	}
}

BaseConnectionManager::~BaseConnectionManager()
{
	(void) StopHeartbeatService();


	delete specification;


	for (size_t i = 0; i < countStandardFieldsAllMessages; ++i)
	{
		delete standardFieldsAllMessages[i];
	}
	delete [] standardFieldsAllMessages;
	standardFieldsAllMessages = 0;


	for (size_t i = 0; i < countStandardHeartbeatFields; ++i)
	{
		delete standardHeartbeatFields[i];
	}
	delete [] standardHeartbeatFields;
	standardHeartbeatFields = 0;


	for (size_t i = 0; i < countStandardLogFields; ++i)
	{
		delete standardLogFields[i];
	}
	delete [] standardLogFields;
	standardLogFields = 0;


	Cleanup();

	ConnMgrCallbackCache::getCache().destroyConnMgrCallbacks(this);
}

Status BaseConnectionManager::SetStandardFields(Field* standardFields[], size_t fieldCount)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}

	if (standardFields == NULL)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "Array of standard Fields is NULL");
		return result;
	}

	if (standardFieldsAllMessages != 0)
	{
		for (size_t i = 0; i < countStandardFieldsAllMessages; i++)
		{
			delete standardFieldsAllMessages[i];
		}

		delete [] standardFieldsAllMessages;

		standardFieldsAllMessages = 0;
		countStandardFieldsAllMessages = 0;
	}		

	if (fieldCount > 0)
	{
		standardFieldsAllMessages = new Field*[fieldCount];

		for (size_t i = 0; i < fieldCount; ++i)
		{
			if (standardFields[i])
			{
				GMSEC_TYPE type;
				Status status = standardFields[i]->GetType(type);

				if (!status.IsError() && type != GMSEC_TYPE_UNSET)
				{
					standardFieldsAllMessages[ countStandardFieldsAllMessages++ ] = new Field(*standardFields[i]);
				}
			}
			else
			{
				LOG_WARNING << "Field within array of standard fields at index " << i << " is NULL";
			}
		}
	}
	
	return result;
}

Status BaseConnectionManager::Initialize()
{
	Status result = ConnectionFactory::Create(&config, connection);
	if(result.IsError())
	{
		return result;
	}

	result = connection->Connect();
	if(result.IsError())
	{
		ConnectionFactory::Destroy(connection);
		connection = NULL;
		return result;
	}

	// Future consideration: Allow users to specify a templates base directory via either:
	// a) Users construct a Specification to pass to the ConnectionManager
	// b) Users pass a base directory via const char * to ConnectionManager

	try
	{
		specification = new gmsec::api::mist::Specification(gmsec::internal::API4_Adapter::API3ConfigToAPI4(&config));
	}
	catch (gmsec::api::Exception& e)
	{
		result = gmsec::internal::API4_Adapter::API4ExceptionToAPI3(e);
	}

	if(result.IsError())
	{
		ConnectionFactory::Destroy(connection);
		connection = NULL;
	}

	return result;
}

Status BaseConnectionManager::Cleanup()
{
	Status result;

	if (connection)
	{
		result = connection->Disconnect();

		if(result.IsError())
		{
			LOG_ERROR << "Error disconnecting\n";
		}
	
		result = ConnectionFactory::Destroy(connection);
		connection = 0;
	}

	return result;
}

Status BaseConnectionManager::RegisterErrorCallback(const char *event, ConnectionManagerErrorCallback *cb)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!event)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The event string is null.");
		return result;
	}
	if (!cb)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The callback is null.");
		return result;
	}

	CMErrorCallback* callback = new CMErrorCallback(this);

	ConnMgrCallbackCache::getCache().putErrorCallback(callback, cb);

	return connection->RegisterErrorCallback(event, callback);
}

Status BaseConnectionManager::Subscribe(const char *subject)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!subject)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The subject string is null.");
		return result;
	}

	return connection->Subscribe(subject);
}

Status BaseConnectionManager::Subscribe(const char *subject, ConnectionManagerCallback *cb)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!subject)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The subject string is null.");
		return result;
	}
	if (!cb)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The callback is null.");
		return result;
	}

	CMCallback* callback = new CMCallback(this);

	ConnMgrCallbackCache::getCache().putCallback(callback, cb);

	return connection->Subscribe(subject, callback);
}

Status BaseConnectionManager::Subscribe(const char *subject, const Config &cfg)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!subject)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The subject string is null.");
		return result;
	}

	return connection->Subscribe(subject, cfg);
}

Status BaseConnectionManager::UnSubscribe(const char *subject)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!subject)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The subject string is null.");
		return result;
	}

	return connection->UnSubscribe(subject);
}

Status BaseConnectionManager::UnSubscribe(const char *subject, ConnectionManagerCallback *cb)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!subject)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The subject string is null.");
		return result;
	}
	if (!cb)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The callback is null.");
		return result;
	}

	CMCallback* cmCallback = dynamic_cast<CMCallback*>(ConnMgrCallbackCache::getCache().getAndRemoveCallback(cb));

	if (!cmCallback)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The callback is null.");
		return result;
	}

	result = connection->UnSubscribe(subject, cmCallback);

	delete cmCallback;

	return result;
}

Status BaseConnectionManager::CreateMessage(const char* subject, GMSEC_MSG_KIND kind, Message *&msg)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!subject)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The subject string is null.");
		return result;
	}

	util::AutoMutex hold(cmLock);

	result = connection->CreateMessage(subject, kind, msg);

	if(result.IsError())
	{
		return result;
	}

	for(size_t i = 0; i < countStandardFieldsAllMessages; i++)
	{
		result = msg->AddField(*standardFieldsAllMessages[i]);
		if(result.IsError())
		{
			return result;
		}
	}

	return result;
}

Status BaseConnectionManager::ConvertMessage(Message *in, Message *&out)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!in)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The in-Message object is null.");
		return result;
	}

	return connection->ConvertMessage(in, out);
}

Status BaseConnectionManager::ConvertCleanup(Message *in, Message *out)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!in)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The in-Message object is null.");
		return result;
	}
	if (!out)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The out-Message object is null.");
		return result;
	}

	return connection->ConvertCleanup(in, out);
}

Status BaseConnectionManager::CloneMessage(Message *in, Message *&out)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!in)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The in-Message object is null.");
		return result;
	}

	return connection->CloneMessage(in, out);
}

Status BaseConnectionManager::DestroyMessage(Message *msg)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!msg)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The Message object is null.");
		return result;
	}

	return connection->DestroyMessage(msg);
}

Status BaseConnectionManager::Publish(Message* msg)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!msg)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The Message object is null.");
		return result;
	}

	util::AutoMutex hold(cmLock);

	// Validate the message before it is sent out
	if (validateMessages)
	{
		result = LookupAndValidate(msg);

		if (result.IsError())
		{
			const char * msgXml = 0;
			msg->ToXML(msgXml);
			LOG_ERROR << "Unable to publish message due to validation failure: \n" << msgXml;
			return result;
		}
	}

	result = connection->Publish(msg);

	return result;
}

Status BaseConnectionManager::Publish(Message *msg, const Config &cfg)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!msg)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The Message object is null.");
		return result;
	}

	util::AutoMutex hold(cmLock);

	// Validate the message before it is sent out
	if (validateMessages)
	{
		result = LookupAndValidate(msg);

		if (result.IsError())
		{
			const char * msgXml = 0;
			msg->ToXML(msgXml);
			LOG_ERROR << "Unable to publish message: \n" << msgXml;
			return result;
		}
	}

	result = connection->Publish(msg, cfg);

	return result;
}

Status BaseConnectionManager::Request(Message *request, GMSEC_I32 timeout, ConnectionManagerReplyCallback *cb, GMSEC_I32 republish_ms)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!request)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The request message is null.");
		return result;
	}
	if (!cb)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The callback is null.");
		return result;
	}

	util::AutoMutex hold(cmLock);

	// Validate the message before it is sent out
	if (validateMessages)
	{
		result = LookupAndValidate(request);

		if (result.IsError())
		{
			const char * msgXml = 0;
			request->ToXML(msgXml);
			LOG_ERROR << "Unable to publish request message: \n" << msgXml;
			return result;
		}
	}

	CMReplyCallback* callback = new CMReplyCallback(this);

	ConnMgrCallbackCache::getCache().putReplyCallback(callback, cb);

	result = connection->Request(request, timeout, callback, republish_ms);

	return result;
}

Status BaseConnectionManager::Request(Message *request, GMSEC_I32 timeout, Message *&reply, GMSEC_I32 republish_ms)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!request)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The request message is null.");
		return result;
	}

	util::AutoMutex hold(cmLock);

	// Validate the message before it is sent out
	if (validateMessages)
	{
		result = LookupAndValidate(request);

		if (result.IsError())
		{
			const char * msgXml = 0;
			request->ToXML(msgXml);
			LOG_ERROR << "Unable to publish request message: \n" << msgXml;
			return result;
		}
	}

	result = connection->Request(request, timeout, reply, republish_ms);

	return result;
}

Status BaseConnectionManager::Reply(Message *request, Message *reply)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!request)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The request message is null.");
		return result;
	}
	if (!reply)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The reply message is null.");
		return result;
	}

	util::AutoMutex hold(cmLock);

	// Validate the message before it is sent out
	if (validateMessages)
	{
		result = LookupAndValidate(reply);

		if (result.IsError())
		{
			const char * msgXml = 0;
			reply->ToXML(msgXml);
			LOG_ERROR << "Unable to publish reply message: \n" << msgXml;
			return result;
		}
	}

	result = connection->Reply(request, reply);

	return result;
}

Status BaseConnectionManager::StartAutoDispatch()
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}

	return connection->StartAutoDispatch();
}

Status BaseConnectionManager::StopAutoDispatch()
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}

	return connection->StopAutoDispatch();
}

Status BaseConnectionManager::StopAutoDispatch(bool waitForComplete)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}

	return connection->StopAutoDispatch(waitForComplete);
}

Status BaseConnectionManager::GetNextMsg(Message *&msg, GMSEC_I32 timeout)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}

	util::AutoMutex hold(cmLock);

	return connection->GetNextMsg(msg, timeout);
}

Status BaseConnectionManager::DispatchMsg(Message *msg)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!msg)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The message is null.");
		return result;
	}

	util::AutoMutex hold(cmLock);

	return connection->DispatchMsg(msg);
}

Status BaseConnectionManager::GetLastDispatcherStatus()
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}

	return connection->GetLastDispatcherStatus();
}

Status BaseConnectionManager::ExcludeSubject(const char* subject)
{
	return connection->ExcludeSubject(subject);
}

Status BaseConnectionManager::RemoveExcludedSubject(const char* subject)
{
	return connection->RemoveExcludedSubject(subject);
}

Status BaseConnectionManager::GetHeartbeatMessage(Field * standardFields[], size_t fieldCount, Message *&msg, const char * subject)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!subject)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The subject string is null.");
		return result;
	}

	// Add heartbeat message fields
	result = GetMessageWithNewFields(subject, standardHeartbeatFields, countStandardHeartbeatFields, msg);

	if(result.IsError())
	{
		return result;
	}

	//MEH - Next block appears redundant - however either the heartbeat standard fields are set
	//and the array to this function is empty, there are no heartbeat standard fields, or 
	//heartbeat standard fields are set and the user is amending/overwriting that list.
	if (standardFields != NULL && fieldCount > 0)
	{
		for(size_t i = 0; i < fieldCount;i++)
		{
			if (standardFields[i])
			{
				GMSEC_TYPE type;
				Status status = standardFields[i]->GetType(type);

				if (!status.IsError() && type != GMSEC_TYPE_UNSET)
				{
					msg->AddField(*standardFields[i]);
				}
			}
		}
	}

	//TODO (MEH): This should be modified to validate schema version down the road in case identifiers for HB message change
	Field typeField;
	typeField.SetName("MESSAGE-TYPE");
	typeField.SetValue("MSG");
	msg->AddField(typeField);

	Field subtypeField;
	subtypeField.SetName("MESSAGE-SUBTYPE");
	subtypeField.SetValue("C2CX");
	msg->AddField(subtypeField);

	Field c2cxSubtypeField;
	c2cxSubtypeField.SetName("C2CX-SUBTYPE");
	c2cxSubtypeField.SetValue("HB");
	msg->AddField(c2cxSubtypeField);
	
	return result;
}

Status BaseConnectionManager::GetMessageWithNewFields(const char * subject, Field * standardFields[], size_t fieldCount, Message *&msg)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}

	//TODO: check subject against standard template
	result = CreateMessage(subject, GMSEC_MSG_PUBLISH, msg);
	if(result.IsError())
	{
		return result;
	}

	for(size_t i = 0; i < fieldCount; i++)
	{
		//TODO: check against standard template
			//result.Set(standard template error)
			//return result;
		//const char * tempName;
		//standardFields[i]->GetName(tempName);

		if (standardFields[i])
		{
			GMSEC_TYPE type;
			Status status = standardFields[i]->GetType(type);

			if (!status.IsError() && type != GMSEC_TYPE_UNSET)
			{
				result = msg->AddField(*standardFields[i]);

				if (result.IsError())
				{
					return result;
				}
			}
		}

	}

	return result;
}

Status BaseConnectionManager::StartHeartbeatService(Field * standardFields[], size_t fieldCount, const char * subject)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!subject)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The subject string is null.");
		return result;
	}

	if (hbService.get() && hbService.get()->isRunning())
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_HEARTBEAT_SERVICE_IS_RUNNING, "HeartbeatService is already running.");
	}
	else
	{
		// This message will be owned by HeartbeatService and destroyed when it is
		Message* c2cxMessage = 0;
		result = GetHeartbeatMessage(standardFields, fieldCount, c2cxMessage, subject);
		
		if (result.IsError())
		{
			return result;
		}

		if (validateMessages)
		{
			result = LookupAndValidate(c2cxMessage);

			if (result.IsError())
			{
				DestroyMessage(c2cxMessage);
				return result;
			}
		}

		// Set the standard fields for a heartbeat message
		SetHeartbeatDefaults(standardFields, fieldCount, subject);

		hbService.reset(new HeartbeatService(config, c2cxMessage));
		hbThread.reset(new gmsec::util::StdThread(&HeartbeatService::start, hbService));
		heartbeatSubject = subject;
		hbThread->start();

		// Wait until the hearbeat service thread actually starts
		// running before allowing the thread which calls 
		// this function BaseConnectionManager::StartHeartbeatService(),
		// presumably some sort of "user-controlled" thread, to move on.
		//
		// Regardless of whether we receive confirmation that the service
		// has started or not, we need to destroy the CXC2 HB message template
		// that was provided to the service.
		//
		if (!hbService->awaitStart(10000))
		{
			result.Set(GMSEC_GENERAL_MIST_ERROR,
			           GMSEC_HEARTBEAT_SERVICE_NOT_RUNNING,
			           "Timed out when initializing.");

			LOG_ERROR << "BaseConnectionManager::StartHeartbeatService : " << result.Get();
		}

		if (connection->DestroyMessage(c2cxMessage).IsError())
		{
			LOG_ERROR << "BaseConnectionManager::StartHeartbeatService : unable to destroy temporary C2CX message.";
		}
	}

	return result;
}

Status BaseConnectionManager::ChangeComponentStatus(const Field &componentStatus)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}

	if(hbService.get() != NULL)
	{
		if (validateMessages)
		{
			Message *msg;
			result = GetHeartbeatMessage(standardHeartbeatFields, countStandardHeartbeatFields, msg, heartbeatSubject.c_str());
			if (result.IsError())
			{
				return result;
			}

			Field& field = const_cast<Field&>(componentStatus);
			result = msg->AddField(field);

			if (result.IsError())
			{
				DestroyMessage(msg);
				return result;
			}

			result = LookupAndValidate(msg);

			if (result.IsError())
			{
				DestroyMessage(msg);
				return result;
			}

			DestroyMessage(msg);
		}

		result = hbService->ChangeComponentStatus(componentStatus);
	}
	else
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_HEARTBEAT_SERVICE_NOT_RUNNING, "HeartbeatService is not running.");
	}

	return result;
}

Status BaseConnectionManager::ChangeComponentInfo(const Field &componentInfo)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}

	if(hbService.get() != NULL)
	{
		if (validateMessages)
		{
			Message *msg;
			result = GetHeartbeatMessage(standardHeartbeatFields, countStandardHeartbeatFields, msg, heartbeatSubject.c_str());
			if (result.IsError())
			{
				return result;
			}

			Field& field = const_cast<Field&>(componentInfo);
			result = msg->AddField(field);

			if (result.IsError())
			{
				DestroyMessage(msg);
				return result;
			}

			result = LookupAndValidate(msg);

			if (result.IsError())
			{
				DestroyMessage(msg);
				return result;
			}

			DestroyMessage(msg);
		}

		result = hbService->ChangeComponentInfo(componentInfo);
	}
	else
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_HEARTBEAT_SERVICE_NOT_RUNNING, "HeartbeatService is not running.");
	}

	return result;
}

Status BaseConnectionManager::ChangeCPUUtil(const Field &cpuUtil)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}

	if(hbService.get() != NULL)
	{
		if (validateMessages)
		{
			Message *msg;
			result = GetHeartbeatMessage(standardHeartbeatFields, countStandardHeartbeatFields, msg, heartbeatSubject.c_str());
			if (result.IsError())
			{
				return result;
			}

			Field& field = const_cast<Field&>(cpuUtil);
			result = msg->AddField(field);

			if (result.IsError())
			{
				DestroyMessage(msg);
				return result;
			}

			result = LookupAndValidate(msg);

			if (result.IsError())
			{
				DestroyMessage(msg);
				return result;
			}

			DestroyMessage(msg);
		}

		result = hbService->ChangeCPUUtil(cpuUtil);
	}
	else
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_HEARTBEAT_SERVICE_NOT_RUNNING, "HeartbeatService is not running.");
	}

	return result;
}

Status BaseConnectionManager::ChangeCPUMemory(const Field &cpuMemory)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}

	if(hbService.get() != NULL)
	{
		if (validateMessages)
		{
			Message *msg;
			result = GetHeartbeatMessage(standardHeartbeatFields, countStandardHeartbeatFields, msg, heartbeatSubject.c_str());
			if (result.IsError())
			{
				return result;
			}

			Field& field = const_cast<Field&>(cpuMemory);
			result = msg->AddField(field);

			if (result.IsError())
			{
				DestroyMessage(msg);
				return result;
			}

			result = LookupAndValidate(msg);

			if (result.IsError())
			{
				DestroyMessage(msg);
				return result;
			}

			DestroyMessage(msg);
		}

		result = hbService->ChangeCPUMemory(cpuMemory);
	}
	else
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_HEARTBEAT_SERVICE_NOT_RUNNING, "HeartbeatService is not running.");
	}

	return result;
}
		
Status BaseConnectionManager::StopHeartbeatService()
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}

	if (hbService.get() != NULL)
	{
		const unsigned int timeout_ms = 3000;

		if (!hbService->stop(timeout_ms))
		{
			result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_HEARTBEAT_SERVICE_FAILURE, "Stop timed out");
			LOG_WARNING << "BaseConnectionManager::StopHeartbeatService: " << result.Get();
		}
		else
		{
			hbService.reset();
			hbThread->join();
			hbThread.reset();
		}
	}
	else
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_HEARTBEAT_SERVICE_NOT_RUNNING, "HeartbeatService is not running.");
	}

	return result;
}

Status BaseConnectionManager::GetLogMessage(Field * standardFields[], size_t fieldCount, Message *&msg, const char *subject)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!subject)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The subject string is null.");
		return result;
	}

	// Add log message fields
	result = GetMessageWithNewFields(subject, standardLogFields, countStandardLogFields, msg);

	if(result.IsError())
	{
		return result;
	}

	//MEH - Next block appears redundant - however either the log standard fields are set
	//and the array to this function is empty, there are no log standard fields, or 
	//log standard fields are set and the user is amending/overwriting that list.
	for(size_t i = 0; i < fieldCount;i++)
	{
		if (standardFields[i])
		{
			GMSEC_TYPE type;
			Status status = standardFields[i]->GetType(type);

			if (!status.IsError() && type != GMSEC_TYPE_UNSET)
			{
				msg->AddField(*standardFields[i]);
			}
		}
	}

	//TODO (MEH): This should be modified to validate schema version down the road in case identifiers for LOG message change
	Field typeField;
	typeField.SetName("MESSAGE-TYPE");
	typeField.SetValue("MSG");
	msg->AddField(typeField);

	Field subtypeField;
	subtypeField.SetName("MESSAGE-SUBTYPE");
	subtypeField.SetValue("LOG");
	msg->AddField(subtypeField);

	char tempBuffer[256];
	double t = gmsec::util::getTime_s();
	gmsec::util::formatTime_s(t, tempBuffer);
	Field eventTime("EVENT-TIME", (GMSEC_STR) tempBuffer);
	result = msg->AddField(eventTime);

	return result;
}

Status BaseConnectionManager::SetHeartbeatDefaults(Field* standardFields[], size_t fieldCount, const char * subject)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!subject)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The subject string is null.");
		return result;
	}

	// Disallow setting the default heartbeat fields if the HeartbeatService has started
	if (hbService.get() && hbService.get()->isRunning())
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_HEARTBEAT_SERVICE_IS_RUNNING, "HeartbeatService is already running.");
		return result;
	}

	if(standardHeartbeatFields != 0)
	{
		for(size_t i = 0; i < countStandardHeartbeatFields; i++)
		{
			delete standardHeartbeatFields[i];
		}

		delete [] standardHeartbeatFields;

		standardHeartbeatFields = 0;
		countStandardHeartbeatFields = 0;
	}	

	if (standardFields != NULL && fieldCount > 0)
	{
		standardHeartbeatFields = new Field*[fieldCount];

		for (size_t i = 0; i < fieldCount; ++i)
		{
			if (standardFields[i])
			{
				GMSEC_TYPE type;
				Status status = standardFields[i]->GetType(type);

				if (!status.IsError() && type != GMSEC_TYPE_UNSET)
				{
					standardHeartbeatFields[ countStandardHeartbeatFields++ ] = new Field(*standardFields[i]);
				}
			}
			else
			{
				LOG_WARNING << "Field within array of standard heartbeat fields at index " << i << " is NULL";
			}
		}
	}

	heartbeatSubject = subject;

	return result;
}

Status BaseConnectionManager::SetLoggingDefaults(Field* standardFields[], size_t fieldCount, const char * subject)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!subject)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The subject string is null.");
		return result;
	}

	if(standardLogFields != 0)
	{
		for(size_t i = 0; i < countStandardLogFields; i++)
		{
			delete standardLogFields[i];
		}

		delete [] standardLogFields;

		standardLogFields = 0;
		countStandardLogFields = 0;
	}	

	if (standardFields != NULL && fieldCount > 0)
	{
		standardLogFields = new Field*[fieldCount];

		for (size_t i = 0; i < fieldCount; ++i)
		{
			if (standardFields[i])
			{
				GMSEC_TYPE type;
				Status status = standardFields[i]->GetType(type);

				if (!status.IsError() && type != GMSEC_TYPE_UNSET)
				{
					standardLogFields[ countStandardLogFields++ ] = new Field(*standardFields[i]);
				}
			}
			else
			{
				LOG_WARNING << "Field within array of standard log fields at index " << i << " is NULL";
			}
		}
	}

	logSubject = subject;

	return result;
}

Status BaseConnectionManager::QuickLog(const char* msg, const Field &severity)
{
	Status result;

	if (!connection)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_CONNECTION_NOT_INITIALIZED, "The ConnectionManager has not been initialized!");
		return result;
	}
	if (!msg)
	{
		result.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "The log message string is null.");
		return result;
	}

	Message* message = 0;
	result = GetLogMessage(NULL, 0, message, logSubject.c_str());

	if(result.IsError())
	{
		if (message != NULL)
		{
			DestroyMessage(message);
		}

		return result;
	}

	Field& field = const_cast<Field&>(severity);
	result = message->AddField(field);

	if(result.IsError())
	{
		DestroyMessage(message);
		return result;
	}

	// Set the MSG-TEXT field and verify it against the template
	//TODO (MEH): use schema to figure out what this field should be instead of
	//hardcoding - ditto for the EVENT-TIME
	Field mText("MSG-TEXT", msg);
	result = message->AddField(mText);

	if(result.IsError())
	{
		DestroyMessage(message);
		return result;
	}

	if (validateMessages)
	{
		result = LookupAndValidate(message);

		if (result.IsError())
		{
			DestroyMessage(message);
			return result;
		}
	}

	result = Publish(message);

	if (result.IsError())
	{
		LOG_ERROR << "BaseConnectionManager::QuickLog : " << result.Get();
	}

	// Clean up the published message
	result = DestroyMessage(message);

	if (result.IsError())
	{
		LOG_ERROR << "BaseConnectionManager::QuickLog : " << result.Get();
	}

	return result;
}


bool BaseConnectionManager::CheckValidSpec(unsigned int specVersionInt)
{
	switch(specVersionInt)
	{
		// Place future versions here
		//case ConnectionManager::GMSEC_ISD_2014_00:
		case ConnectionManager::GMSEC_ISD_CURRENT:
			return true;
		default:
			return false;
	}
}

Status BaseConnectionManager::LookupAndValidate(Message *msg)
{
	Status result;

	// Lookup the message template
	try
	{
		gmsec::api::Message* msg4 = gmsec::internal::API4_Adapter::referenceAPI4Message(msg);

		// Check to see if required fields are included; if not, add them so that validation
		// will not complain.
		static const char* REQUIRED_FIELDS[] = { "MISSION-ID", "FACILITY", "COMPONENT", NULL };

		for (size_t i = 0; REQUIRED_FIELDS[i] != NULL; ++i)
		{
			if (msg4->getField(REQUIRED_FIELDS[i]) == NULL)
			{
				msg4->addField(REQUIRED_FIELDS[i], "UNSET");
			}
		}

		specification->validateMessage(*msg4);
	}
	catch (gmsec::api::Exception& e)
	{
		result = gmsec::internal::API4_Adapter::API4ExceptionToAPI3(e);
	}

	return result;
}

}
}
}
