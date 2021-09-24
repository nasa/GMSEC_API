/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalConnection.h
 *
 *  @brief This file contains the opaque wrapper for all connections. Instructions
 *	are passed through this interface to the "real" connection.
 */

#ifndef gmsec_InternalConnection_h
#define gmsec_InternalConnection_h


#include <gmsec_defs.h>

#include <gmsec/internal/ci_less.h>

#include <gmsec/Status.h>

#include <gmsec/util/String.h>

#include <gmsec4/util/Mutex.h>
#include <gmsec4/util/wdllexp.h>

#include <list>
#include <map>
#include <string>


namespace gmsec
{

class Config;
class Connection;
class Message;
class Callback;
class ErrorCallback;
class ReplyCallback;

namespace api
{
class Connection;
class SubscriptionInfo;
}

namespace internal
{
class GarbageCollector;


class GMSEC_API InternalConnection
{
public:
	InternalConnection(gmsec::api::Connection* p);


	~InternalConnection();


	void registerParent(Connection* parent);


	const char* CALL_TYPE GetLibraryRootName();


	const char* CALL_TYPE GetLibraryVersion();


	char* CALL_TYPE GetMWINFO(char* infoBuffer);


	Status CALL_TYPE GetMWINFO(gmsec::util::String& info);


	Status CALL_TYPE Connect();


	Status CALL_TYPE Disconnect();


	bool CALL_TYPE IsConnected();


	Status CALL_TYPE RegisterErrorCallback(const char* event, ErrorCallback* ecb);


	Status CALL_TYPE Subscribe(const char* subject);


	Status CALL_TYPE Subscribe(const char* subject, Callback* cb);


	Status CALL_TYPE Subscribe(const char* subject, const Config& config, Callback* cb = NULL);


	Status CALL_TYPE UnSubscribe(const char* subject, Callback* cb = NULL);


	Status CALL_TYPE CreateMessage(Message*& msg);


	Status CALL_TYPE CreateMessage(const char* subject, GMSEC_MSG_KIND msgKind, Message*& msg);


	Status CALL_TYPE CreateMessage(const char* subject, GMSEC_MSG_KIND msgKind, Message*& msg, Config* config);


	Status CALL_TYPE ConvertMessage(Message* in, Message*& out);


	Status CALL_TYPE ConvertCleanup(Message* in, Message* out);


	Status CALL_TYPE CloneMessage(Message* in, Message*& out);


	Status CALL_TYPE DestroyMessage(Message* msg);


	Status CALL_TYPE Publish(Message* msg, const Config& config);


	Status CALL_TYPE Request(Message* request, GMSEC_I32 timeout, Callback* cb, GMSEC_I32 republish_ms = 0);


	Status CALL_TYPE Request(Message* request, GMSEC_I32 timeout, ReplyCallback* rcb, GMSEC_I32 republish_ms = 0);


	Status CALL_TYPE Request(Message* request, GMSEC_I32 timeout, Message*& reply, GMSEC_I32 republish_ms = 0);


	Status CALL_TYPE Reply(Message* request,Message* reply);


	Status CALL_TYPE StartAutoDispatch();


	Status CALL_TYPE StopAutoDispatch(bool waitForComplete);


	Status CALL_TYPE GetNextMsg(Message*& msg, GMSEC_I32 timeout);


	Status CALL_TYPE DispatchMsg(Message* msg);


	Status CALL_TYPE GetLastDispatcherStatus();


	Status CALL_TYPE ExcludeSubject(const char* subject);


	Status CALL_TYPE RemoveExcludedSubject(const char* subject);


	const char* CALL_TYPE GetName();


	void CALL_TYPE SetName(const char* name);


	const char* CALL_TYPE GetConnectionGUI();


	GMSEC_U64 CALL_TYPE GetPublishQueueMessageCount();


	Status CALL_TYPE DispatchError(const char* name, Message* msg, Status* status);


	/*
	 ** these are for C API support ONLY
	 */
	Status CALL_TYPE RegisterErrorCallback(const char* event, GMSEC_ERROR_CALLBACK* ecb);
	Status CALL_TYPE Subscribe(const char* subject, GMSEC_CALLBACK* cb);
	Status CALL_TYPE UnSubscribe(const char* subject, GMSEC_CALLBACK* cb);
	Status CALL_TYPE Request(Message* request, GMSEC_I32 timeout, GMSEC_CALLBACK* cb);
	Status CALL_TYPE Request(Message* request, GMSEC_I32 timeout, GMSEC_REPLY_CALLBACK* cb, GMSEC_ERROR_CALLBACK* er);

private:
	// Declared, but not implemented.
	InternalConnection(const InternalConnection&);
	InternalConnection& operator=(const InternalConnection&);

	struct Details
	{
		Callback* cb;
		gmsec::api::SubscriptionInfo* info;
	};
	class SubscriptionDetails
	{
	public:
		SubscriptionDetails();
		~SubscriptionDetails();

		void addDetails(Callback* cb, gmsec::api::SubscriptionInfo* info);

		std::list<Details*> getDetails(const char* subject, Callback* cb);

		bool hasMoreDetails(const char* subject);

		bool hasDetails(const char* subject, Callback* cb);

	private:
		std::list<Details*> infoDetails;
	};

	typedef std::map<std::string, SubscriptionDetails*> Subscriptions;

	typedef std::map<std::string, ErrorCallback*, ci_less> ErrorCallbackMap;

	gmsec::api::Connection* m_adapter;
	gmsec::Connection*      m_parent;
	gmsec::api::util::Mutex m_subscribeMutex;
	Subscriptions           m_subscriptions;
	ErrorCallbackMap        m_errorCallbacks;
	GarbageCollector*       m_collector;
};

} //end namespace internal
} //end namespace gmsec

#endif
