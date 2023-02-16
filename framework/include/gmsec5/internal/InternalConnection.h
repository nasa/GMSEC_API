/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalConnection.h
 */

#ifndef GMSEC_API5_INTERNAL_CONNECTION_H
#define GMSEC_API5_INTERNAL_CONNECTION_H

#include <gmsec5/internal/ActiveSubscriptions.h>
#include <gmsec5/internal/CallbackAdapter.h>
#include <gmsec5/internal/AutoDispatcher.h>
#include <gmsec5/internal/RequestSpecs.h>
#include <gmsec5/internal/TicketMutex.h>
#include <gmsec5/internal/TrackingDetails.h>

#include <gmsec5/internal/PerformanceLogger.h>

#include <gmsec5/Config.h>
#include <gmsec5/Connection.h>
#include <gmsec5/MessageFactory.h>
#include <gmsec5/Specification.h>

#include <gmsec5/secure/Policy.h>

#include <gmsec5/util/Atomics.h>
#include <gmsec5/util/List.h>
#include <gmsec5/util/Mutex.h>
#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/StdThread.h>
#include <gmsec5/util/StdUniquePtr.h>
#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>

#include <list>
#include <map>
#include <queue>
#include <string>


namespace gmsec
{
namespace api5
{
	// Forward declarations(s)
	class Callback;
	class EventCallback;
	class ReplyCallback;
	class Field;
	class Message;
	class SubscriptionInfo;

namespace internal
{
	// Forward declarations(s)
	class AsyncPublisher;
	class CallbackLookup;
	class ConnectionInterface;
	class ExclusionFilter;
	class MessageAggregationToolkit;
	class RequestShared;



// Internal Message Field for handling request/reply operations.
// This definition should not be defined in ConfigOptions.h!
//
const char* const GMSEC_REPLY_UNIQUE_ID_FIELD = "__GMSEC-REPLY-UNIQUE-ID__";


// Internal Message Field for conveying schema id with the sent message
// This definition should not be defined in ConfigOptions.h!
//
const char* const GMSEC_SCHEMA_ID_FIELD = "__GMSEC-SCHEMA-ID__";


/**
 * @struct MessagePublishTask
 * @brief This struct collects a Message and Config for asynchronous publishing
 */
struct MessagePublishTask
{
	Message* msg;
	Config*  config;
};

/**
 * @class InternalConnection
 */
class GMSEC_API InternalConnection
{
public:
	static const GMSEC_I32 GMSEC_MIN_TIMEOUT_ms     = 10;
	static const GMSEC_I32 GMSEC_REQUEST_PUBLISH_ms = 10000;
	static const GMSEC_I32 DEFAULT_REPUBLISH_ms     = 60000;
	static const GMSEC_I32 GMSEC_MIN_REPUBLISH_ms   = 100;


	/**
	 * @enum ConnectionState
	 * @brief Possible states for the Connection object.
	 */
	enum ConnectionState
	{
		NOT_CONNECTED = 0,      ///< Not connected to middleware server.
		CONNECTED     = 1,      ///< Connected to middleware server.
		RECONNECTING  = 2       ///< Connection to middleware server is broken; reattempting to connect.
	};



	InternalConnection(gmsec::api5::Connection* parent, const Config& config);

	InternalConnection(gmsec::api5::Connection* parent, const Config& config, MessageFactory& factory);

	~InternalConnection();

	static const char* CALL_TYPE getAPIVersion();

	const char* CALL_TYPE getLibraryName() const;

	const char* CALL_TYPE getLibraryVersion() const;

	void CALL_TYPE connect(); 

	void CALL_TYPE disconnect();

	gmsec::api5::internal::InternalConnection::ConnectionState CALL_TYPE getState() const;

	MessageFactory& CALL_TYPE getMessageFactory() const;

	void CALL_TYPE registerEventCallback(Connection::Event event, EventCallback* cb);

	SubscriptionInfo* CALL_TYPE subscribe(const char *subject, Callback* cb = NULL);

	SubscriptionInfo* CALL_TYPE subscribe(const char* subject, const Config& config, Callback* cb = NULL);

	void CALL_TYPE unsubscribe(SubscriptionInfo*& info);

	bool CALL_TYPE startAutoDispatch();

	bool CALL_TYPE stopAutoDispatch(bool waitForCompletion = true);

	void CALL_TYPE publish(const Message& msg); 

	void CALL_TYPE publish(const Message& msg, const Config& mwConfig);

	void CALL_TYPE request(const Message& request, GMSEC_I32 timeout, ReplyCallback* cb, GMSEC_I32 republish_ms = 0);

	Message* CALL_TYPE request(const Message& request, GMSEC_I32 timeout, GMSEC_I32 republish_ms = 0);

	void CALL_TYPE reply(const Message& request, const Message& reply);

	Message* CALL_TYPE receive(GMSEC_I32 timeout = GMSEC_WAIT_FOREVER);

	void CALL_TYPE dispatch(const Message& msg);

	void CALL_TYPE excludeSubject(const char *subject);

	void CALL_TYPE removeExcludedSubject(const char *subject);

	const char* CALL_TYPE getName() const;

	void CALL_TYPE setName(const char* name);

	const char* CALL_TYPE getID() const;

	const char* CALL_TYPE getMWInfo() const;

    const char* CALL_TYPE getConnectionEndpoint() const;

	GMSEC_U64 CALL_TYPE getPublishQueueMessageCount() const;

	static void shutdownAllMiddlewares();

	static void shutdownMiddleware(const char* name);


	/* For C-binding only! */
	void registerEventCallback(Connection::Event event, GMSEC_EventCallback* ecb);

	/* For C-binding only! */
	SubscriptionInfo* subscribe(const char* subject, const Config& cfg, GMSEC_Callback* cb);

	/* For C-binding only! */
	void request(const Message& req, GMSEC_I32 timeout, GMSEC_ReplyCallback* rcb, GMSEC_EventCallback* ecb, GMSEC_I32 republish_ms);


	//
	// Internal methods typically used by middleware wrappers (i.e. ConnectionInterface subclasses)
	//

	Config& getConfig();

	const Config& getMessageConfig() const;

	void dispatchEvent(Connection::Event event, const Status& status);

	gmsec::api5::secure::Policy& getPolicy();

	RequestSpecs getRequestSpecs() const;

	void setReplyUniqueID(Message& msg, const std::string& uniqueID);

	std::string getReplyUniqueID(const Message& msg);

	bool onReply(Message* reply);

	void replyEvent(ReplyCallback* rcb, const Status& status, Connection::Event event);

	void replyCallback(ReplyCallback* rcb, const Message& request, const Message& reply);

	void issueRequestToMW(const Message& request, std::string& id);

	const TrackingDetails& getTracking();

	void autoDispatch();

	void insertTrackingFields(Message& msg);

	void removeTrackingFields(Message& msg);

	void setConnectionEndpoint(const std::string& endpoint);

	unsigned int getSpecVersion() const;

	Specification::SchemaLevel getSpecSchemaLevel() const;

	int getMaxConnectionRetries() const;
	int getConnectionRetryInterval() const;

	// helper functions
	enum ValidateLevel
	{
		VALIDATE_NONE,
		VALIDATE_RECV,
		VALIDATE_SEND,
		VALIDATE_ALL
	};

	inline bool validateOnSend() const { return m_validate == VALIDATE_ALL || m_validate == VALIDATE_SEND; }
	inline bool validateOnRecv() const { return m_validate == VALIDATE_ALL || m_validate == VALIDATE_RECV; }

private:
	// Defined, but not implemented
	InternalConnection();
	InternalConnection(const InternalConnection&);
	InternalConnection&operator=(const InternalConnection&);


	void initConnection();
	void initMessageFactory();

	void connectAux();
	void disconnectAux();

	void getNextMsg(Message*& msg, GMSEC_I32 timeout);
	void dispatchMsgToCallbacks(const Message& msg, std::list<Callback*>& callbacks);

	enum SubscriptionStatus { IS_NEW, NOT_NEW, ERR_IS_DUPLICATE, ERR_IS_DUPLICATE_USING_CALLBACK };

	SubscriptionStatus checkSubscription(const char* subject, Callback* cb = NULL);
	bool haveRemainingSubscription(SubscriptionInfo*& info);
	void unsubscribeAux(SubscriptionInfo*& info);

	void initializeRequest();
	void setupSubscriptionForResponse(const Message& request);

	void resolveRequestTimeout(GMSEC_I32& timeout_ms);
	void resolveRepublishInterval(GMSEC_I32& republish_ms);

	void startRequestThread();
	void stopRequestThread();

	void startPerfLoggerThread();
	void stopPerfLoggerThread();

	void startMsgAggregationToolkitThread();
	void stopMsgAggregationToolkitThread();

	GMSEC_U32 getNextMessageCounter();

	gmsec::api5::internal::TicketMutex& getReadMutex();
	gmsec::api5::internal::TicketMutex& getWriteMutex();
	gmsec::api5::internal::TicketMutex& getEventMutex();
	gmsec::api5::internal::TicketMutex& getCounterMutex();


	// useful definitions
	typedef gmsec::api5::util::StdUniquePtr<MessageFactory>                     MsgFactory;

	typedef std::list<SubscriptionInfo*>                                        SubscriptionInfoList;

	typedef std::map<Connection::Event, EventCallback*>                         EventCallbackMap;

	typedef gmsec::api5::util::StdUniquePtr<gmsec::api5::util::StdThread>       RequestThread;
	typedef gmsec::api5::util::StdSharedPtr<RequestShared>                      RequestSharedData;

	typedef gmsec::api5::util::StdUniquePtr<gmsec::api5::util::StdThread>       PerfLoggerThread;
	typedef gmsec::api5::util::StdSharedPtr<PerformanceLogger>                  PerfLoggerSharedData;

	typedef gmsec::api5::util::BoundedQueue<MessagePublishTask>                 AsynchronousQueue;
	typedef gmsec::api5::util::StdUniquePtr<gmsec::api5::util::StdThread>       AsynchronousThread;
	typedef gmsec::api5::util::StdSharedPtr<AsyncPublisher>                     AsynchronousPubService;

	typedef gmsec::api5::util::StdUniquePtr<gmsec::api5::util::StdThread>       MsgAggregationToolkitThread;
	typedef gmsec::api5::util::StdSharedPtr<MessageAggregationToolkit>          MsgAggregationToolkitShared;


	//
	// Member data
	//
	gmsec::api5::Connection*                 m_parent;    // The Connection object that owns us
	Config                                   m_config;
	ConnectionInterface*                     m_connIf;
	ValidateLevel                            m_validate;
	MsgFactory                               m_msgFactory;
	bool                                     m_msgFactoryOwned;
	RequestSpecs                             m_requestSpecs;
	bool                                     m_removeTrackingFields;
	TrackingDetails                          m_tracking;
	gmsec::api5::secure::Policy              m_policy;
	GMSEC_U32                                m_connectionID;
	Config                                   m_msgConfig;
	GMSEC_U32                                m_messageCounter;
	ConnectionState                          m_state;
	gmsec::api5::util::AtomicBoolean         m_connectionAlive;
	std::string                              m_connName;
	std::string                              m_connID;
	std::string                              m_hostName;
	std::string                              m_lowercaseUserName;
	std::string                              m_uppercaseUserName;
	int                                      m_processID;
	mutable std::string                      m_mwInfo;
	AutoDispatcher                           m_autoDispatcher;
	GMSEC_I32                                m_defaultRepublish_ms;
	GMSEC_I32                                m_requestPublish_ms;
	SubscriptionInfoList                     m_subscriptions;
	ExclusionFilter*                         m_exclusionFilter;

	CallbackLookup*                          m_callbackLookup;
	EventCallbackMap                         m_eventCallbacks;

	RequestThread                            m_requestThread;
	RequestSharedData                        m_requestShared;

	PerfLoggerThread                         m_perfLoggerThread;
	PerfLoggerSharedData                     m_perfLoggerShared;
	bool                                     m_usePerfLogger;
	std::string                              m_perfLoggerFilename;

	bool                                     m_useGlobalAsyncPublish;
	size_t                                   m_asyncQueueDepth;
	int                                      m_asyncTeardownWait;
	AsynchronousQueue*                       m_asyncQueue;
	AsynchronousThread                       m_asyncPubThread;
	AsynchronousPubService                   m_asyncPubService;

	MsgAggregationToolkitThread              m_msgAggregationToolkitThread;
	MsgAggregationToolkitShared              m_msgAggregationToolkitShared;

	CallbackAdapter*                         m_callbackAdapter;

	std::string                              m_connectionEndpoint;

	gmsec::api5::internal::TicketMutex       m_readMutex;
	gmsec::api5::internal::TicketMutex       m_writeMutex;
	gmsec::api5::internal::TicketMutex       m_eventMutex;
	gmsec::api5::internal::TicketMutex       m_counterMutex;

	int                                      m_maxConnRetries;
	int                                      m_connRetryInterval;

	static ActiveSubscriptions               s_activeSubscriptions;

	friend class AutoDispatcher;
};


class ConnectionBuddy
{
public:
	static InternalConnection& getInternal(const Connection& conn)
	{
		return *(conn.m_internal);
	}
};

}  //namespace internal
}  //namespace api5
}  //namespace gmsec

#endif
