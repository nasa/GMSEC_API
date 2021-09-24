/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file BaseConnectionManager.h
 *
 *  @brief The BaseConnectionManager is designed to provide several high-level management
 *  functions for operation of a GMSEC connection object. These management functions 
 *  include the application of message content validation, the operation of a
 *  standard service for producing heartbeats, and a set of functions which streamline 
 *  log message production.
 *
**/

#ifndef gmsec_BaseConnectionManager_h
#define gmsec_BaseConnectionManager_h


#include <gmsec/mist/ConnectionManager.h>

#include <gmsec/internal/mist/HeartbeatService.h>

#include <gmsec/Config.h>

#include <gmsec/util/Mutex.h>
#include <gmsec/util/StdThread.h>

#include <gmsec4/util/StdUniquePtr.h>


namespace gmsec
{
	class Connection;
	class Field;
	class Message;
	class Callback;
	class ErrorCallback;
	class ReplyCallback;

	namespace api
	{
		namespace mist
		{
			class Specification;
		}
	}

namespace mist
{
	class ConnectionManagerCallback;
	class ConnectionManagerErrorCallback;
	class ConnectionManagerReplyCallback;

namespace internal
{
	class HeartbeatService;

class GMSEC_API BaseConnectionManager
{
	public:
		BaseConnectionManager(Config &cfg, unsigned int version = ConnectionManager::GMSEC_ISD_CURRENT, bool validate = false);

		~BaseConnectionManager();

		Status CALL_TYPE SetStandardFields(Field* standardFields[], size_t fieldCount);

		Status CALL_TYPE Initialize();

		Status CALL_TYPE Cleanup();

		Status CALL_TYPE RegisterErrorCallback(const char *event, ConnectionManagerErrorCallback *cb);

		Status CALL_TYPE Subscribe(const char *subject);

		Status CALL_TYPE Subscribe(const char *subject, ConnectionManagerCallback *cb);

		Status CALL_TYPE Subscribe(const char *subject, const Config &config);

		Status CALL_TYPE UnSubscribe(const char *subject);

		Status CALL_TYPE UnSubscribe(const char *subject, ConnectionManagerCallback *cb);

		Status CALL_TYPE CreateMessage(const char* subject, GMSEC_MSG_KIND kind, Message *&msg);

		Status CALL_TYPE ConvertMessage(Message *in, Message *&out);

		Status CALL_TYPE ConvertCleanup(Message *in, Message *out);

		Status CALL_TYPE CloneMessage(Message *in, Message *&out);

		Status CALL_TYPE DestroyMessage(Message *msg);

		Status CALL_TYPE Publish(Message *msg);

		Status CALL_TYPE Publish(Message *msg, const Config &config);

		Status CALL_TYPE Request(Message *request, GMSEC_I32 timeout, ConnectionManagerReplyCallback *cb, GMSEC_I32 republish_ms = 0);

		Status CALL_TYPE Request(Message *request, GMSEC_I32 timeout, Message *&reply, GMSEC_I32 republish_ms = 0);

		Status CALL_TYPE Reply(Message *request, Message *reply);

		Status CALL_TYPE StartAutoDispatch();

		Status CALL_TYPE StopAutoDispatch();

		Status CALL_TYPE StopAutoDispatch(bool waitForComplete);

		Status CALL_TYPE GetNextMsg(Message *&msg, GMSEC_I32 timeout);

		Status CALL_TYPE GetHeartbeatMessage(Field * standardFields[], size_t fieldCount, Message *&msg, const char * subject);

		Status CALL_TYPE DispatchMsg(Message *msg);

		Status CALL_TYPE GetLastDispatcherStatus();

		Status CALL_TYPE ExcludeSubject(const char* subject);

		Status CALL_TYPE RemoveExcludedSubject(const char* subject);

		Status CALL_TYPE StartHeartbeatService(Field * standardFields[], size_t fieldCount, const char * subject);

		Status CALL_TYPE ChangeComponentStatus(const Field &componentStatus);

		Status CALL_TYPE ChangeComponentInfo(const Field &componentInfo);

		Status CALL_TYPE ChangeCPUMemory(const Field &cpuMemory);

		Status CALL_TYPE ChangeCPUUtil(const Field &cpuUtil);
		
		Status CALL_TYPE StopHeartbeatService();

		Status CALL_TYPE GetLogMessage(Field * standardFields[], size_t fieldCount, Message *&msg, const char * subject);

		Status CALL_TYPE SetHeartbeatDefaults(Field * standardFields[], size_t fieldCount, const char * subject);

		Status CALL_TYPE SetLoggingDefaults(Field * standardFields[], size_t fieldCount, const char * subject);

		Status CALL_TYPE QuickLog(const char* msg, const Field &severity);

	private:
		// not implemented
		BaseConnectionManager &operator=(const ConnectionManager &);
		BaseConnectionManager(const ConnectionManager &);
		BaseConnectionManager();

		Status GetMessageWithNewFields(const char * subject, Field * standardFields[], size_t fieldCount, Message *&msg);

		// helper functions
		bool CheckValidSpec(unsigned int specVersionInt);
		Status LookupAndValidate(Message *message);

		unsigned int specVersion;
		gmsec::api::mist::Specification* specification;
		Config config;

		bool validateMessages;

		gmsec::api::util::StdUniquePtr<gmsec::util::StdThread> hbThread;
		gmsec::util::gshared_ptr<HeartbeatService> hbService;

		Field** standardFieldsAllMessages;
		size_t countStandardFieldsAllMessages;

		Field** standardHeartbeatFields;
		std::string heartbeatSubject;
		size_t countStandardHeartbeatFields;

		Field** standardLogFields;
		std::string logSubject;
		size_t countStandardLogFields;

		util::Mutex cmLock;
		Connection *connection;
	};
}
}
}

#endif
