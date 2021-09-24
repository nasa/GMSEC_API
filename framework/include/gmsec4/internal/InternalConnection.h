/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalConnection.h
 *
 *  @brief This file contains the opaque wrapper for middleware connections.
 *
 *
**/

#ifndef GMSEC_API_INTERNAL_CONNECTION_H
#define GMSEC_API_INTERNAL_CONNECTION_H

#include <gmsec4_defs.h>

#include <gmsec4/internal/ActiveSubscriptions.h>
#include <gmsec4/internal/CallbackAdapter.h>
#include <gmsec4/internal/InternalAutoDispatcher.h>
#include <gmsec4/internal/RequestSpecs.h>
#include <gmsec4/internal/TicketMutex.h>
#include <gmsec4/internal/TrackingDetails.h>

#include <gmsec4/internal/performance/PerformanceLogger.h>

#include <gmsec4/Config.h>
#include <gmsec4/Connection.h>
#include <gmsec4/Errors.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/Atomics.h>
#include <gmsec4/util/BoundedQueue.h>
#include <gmsec4/util/StdSharedPtr.h>
#include <gmsec4/util/StdThread.h>
#include <gmsec4/util/StdUniquePtr.h>
#include <gmsec4/util/wdllexp.h>

#include <gmsec4/secure/Policy.h>

#include <string>
#include <list>
#include <map>
#include <queue>


namespace gmsec
{
// Forward declaration for legacy Connection class
class Connection;

namespace api
{
// Forward declaration(s)
class Callback;
class EventCallback;
class Message;
class ReplyCallback;
class SubscriptionInfo;

namespace internal
{
// Forward declaration(s)
class AsyncPublisher;
class CallbackLookup;
class ConnectionInterface;
class ExclusionFilter;
class MessageAggregationToolkit;
class RequestShared;
class SubscriptionDetails;


// Internal Message Field for handling request/reply operations.
// This definition should not be defines in ConfigOptions.h!
//
const char* const GMSEC_REPLY_UNIQUE_ID_FIELD = "__GMSEC-REPLY-UNIQUE-ID__";


/**
 * @struct MessagePublishTask
 * @brief This struct collects a Message and Config for asynchronous publishing
 */
struct MessagePublishTask
{
	Message* msg;
	Config*  config;
};


class GMSEC_API InternalConnection
{
public:
	static const GMSEC_I32 GMSEC_MIN_TIMEOUT_ms     = 10;
	static const GMSEC_I32 GMSEC_REQUEST_PUBLISH_ms = 10000;
	static const GMSEC_I32 DEFAULT_REPUBLISH_ms     = 60000;
	static const GMSEC_I32 GMSEC_MIN_REPUBLISH_ms   = 100;


	static const char* CALL_TYPE getAPIVersion();


	InternalConnection(const Config& config, ConnectionInterface* connIf);


	virtual ~InternalConnection();


	virtual void CALL_TYPE connect();


	virtual void CALL_TYPE disconnect(); 


	virtual gmsec::api::Connection::ConnectionState CALL_TYPE getState() const;


	virtual const char* CALL_TYPE getLibraryRootName() const;


	virtual const char* CALL_TYPE getLibraryVersion() const;


	virtual void CALL_TYPE registerEventCallback(gmsec::api::Connection::ConnectionEvent event, EventCallback* cb);


	virtual SubscriptionInfo* CALL_TYPE subscribe(const char* subject, Callback* cb = NULL);


	virtual SubscriptionInfo* CALL_TYPE subscribe(const char* subject, const Config& config, Callback* cb = NULL);


	virtual void CALL_TYPE unsubscribe(SubscriptionInfo*& info);


	virtual bool CALL_TYPE startAutoDispatch();


	virtual bool CALL_TYPE stopAutoDispatch(bool waitForCompletion = true);


	virtual void CALL_TYPE publish(const Message& msg); 


	virtual void CALL_TYPE publish(const Message& msg, const Config& mwConfig);


	virtual void CALL_TYPE request(const Message& request, GMSEC_I32 timeout, ReplyCallback* cb, GMSEC_I32 republish_ms = 0);


	virtual Message* CALL_TYPE request(const Message& request, GMSEC_I32 timeout, GMSEC_I32 republish_ms = 0);


	virtual void CALL_TYPE cancelRequest(ReplyCallback* cb);


	virtual void CALL_TYPE reply(const Message& request, const Message& reply);


	virtual Message* CALL_TYPE receive(GMSEC_I32 timeout = GMSEC_WAIT_FOREVER);


	virtual void CALL_TYPE dispatch(const Message& msg);


	 virtual void CALL_TYPE release(Message*& msg);


	virtual void excludeSubject(const char *subject);


	virtual void removeExcludedSubject(const char *subject);


	virtual const char* CALL_TYPE getName() const;


	virtual void CALL_TYPE setName(const char* name);


	virtual const char* CALL_TYPE getID() const;


	virtual const char* CALL_TYPE getMWInfo() const;


	const char* CALL_TYPE getConnectionEndpoint() const;


	virtual GMSEC_U64 CALL_TYPE getPublishQueueMessageCount() const;


	static void CALL_TYPE shutdownAllMiddlewares();


	static void CALL_TYPE shutdownMiddleware(const char* name);


	// This method is needed in support of using legacy callbacks
	// (see GMSECadapter)
	virtual void CALL_TYPE registerParent(gmsec::api::Connection* parent);


	/* For C-binding only! */
	virtual void CALL_TYPE registerEventCallback(Connection::ConnectionEvent event, GMSEC_EventCallback* ecb);


	/* For C-binding only! */
	virtual SubscriptionInfo* CALL_TYPE subscribe(const char* subject, const Config& cfg, GMSEC_Callback* cb);


	/* For C-binding only! */
	virtual void CALL_TYPE request(const Message& req, GMSEC_I32 timeout, GMSEC_ReplyCallback* rcb, GMSEC_EventCallback* ecb, GMSEC_I32 republish_ms);


	/* For C-binding only! */
	virtual void CALL_TYPE cancelRequest(GMSEC_ReplyCallback* rcb);


	//
	// Internal methods typically used by middleware wrappers (i.e. ConnectionInterface subclasses)
	//

	Config& CALL_TYPE getConfig();


	Config& CALL_TYPE getMessageConfig();


	void CALL_TYPE dispatchEvent(Connection::ConnectionEvent event, const Status& status);


	gmsec::api::secure::Policy& CALL_TYPE getPolicy();


	RequestSpecs CALL_TYPE getRequestSpecs() const;


	void setReplyUniqueID(Message& msg, const std::string& uniqueID);


	std::string getReplyUniqueID(const Message& msg);


	virtual bool CALL_TYPE onReply(Message* reply);


	void replyEvent(ReplyCallback* rcb, const Status& status, Connection::ConnectionEvent event);


	void replyCallback(ReplyCallback* rcb, const Message& request, const Message& reply);


	void issueRequestToMW(const Message& request, std::string& id);


	static void CALL_TYPE updateReplySubject(Message*& reply, bool warn = false);


	const TrackingDetails& getTracking();


	void autoDispatch();


	void insertTrackingFields(Message& msg);
	void removeTrackingFields(Message& msg);

	void setConnectionEndpoint(const std::string& endpoint);

	unsigned int getSpecVersion() const;

	int getSpecLevel() const;

	void usingAPI3x();


private:
	void getNextMsg(Message*& msg, GMSEC_I32 timeout);
	void dispatchMsgToCallbacks(const Message& msg, std::list<Callback*>& callbacks);

	bool checkExistingSubscription(const char* subject, Callback* cb = NULL) const;
	void unsubscribeAux(SubscriptionInfo*& info);

	void initializeRequest();

	void resolveRequestTimeout(GMSEC_I32& timeout_ms);
	void resolveRepublishInterval(GMSEC_I32& republish_ms);

	void startRequestThread();
	void stopRequestThread();

	void startPerfLoggerThread();
	void stopPerfLoggerThread();

	void startMsgAggregationToolkitThread();
	void stopMsgAggregationToolkitThread();

	GMSEC_U32 getNextMessageCounter();

	gmsec::api::util::TicketMutex& getReadMutex();
	gmsec::api::util::TicketMutex& getWriteMutex();
	gmsec::api::util::TicketMutex& getEventMutex();
	gmsec::api::util::TicketMutex& getCounterMutex();

	// defined, but not implemented
	InternalConnection();
	InternalConnection(const InternalConnection&);
	InternalConnection &operator=(const InternalConnection&);

	// useful definitions
	typedef std::list<SubscriptionInfo*>                SubscriptionInfoList;
	typedef std::map<std::string, SubscriptionDetails*> Subscriptions;

	typedef std::map<Connection::ConnectionEvent, EventCallback*> EventCallbackMap;

	typedef gmsec::api::util::StdUniquePtr<gmsec::api::util::StdThread> RequestThread;
	typedef gmsec::api::util::StdSharedPtr<RequestShared>               RequestSharedData;

	typedef gmsec::api::util::StdUniquePtr<gmsec::api::util::StdThread>    PerfLoggerThread;
	typedef gmsec::api::util::StdSharedPtr<performance::PerformanceLogger> PerfLoggerSharedData;

	typedef gmsec::api::util::BoundedQueue<MessagePublishTask>          AsynchronousQueue;
	typedef gmsec::api::util::StdUniquePtr<gmsec::api::util::StdThread> AsynchronousThread;
	typedef gmsec::api::util::StdSharedPtr<AsyncPublisher>              AsynchronousPubService;

	typedef gmsec::api::util::StdUniquePtr<gmsec::api::util::StdThread> MsgAggregationToolkitThread;
	typedef gmsec::api::util::StdSharedPtr<MessageAggregationToolkit>   MsgAggregationToolkitShared;


	// Member data
	gmsec::api::util::TicketMutex           m_readMutex;
	gmsec::api::util::TicketMutex           m_writeMutex;
	gmsec::api::util::TicketMutex           m_eventMutex;
	gmsec::api::util::TicketMutex           m_counterMutex;

	Config                                  m_config;
	ConnectionInterface*                    m_connIf;
	RequestSpecs                            m_requestSpecs;
	TrackingDetails                         m_tracking;
	gmsec::api::secure::Policy              m_policy;
	GMSEC_U32                               m_connectionID;
	Config                                  m_msgConfig;
	GMSEC_U32                               m_messageCounter;
	gmsec::api::Connection::ConnectionState m_state;
	gmsec::api::util::AtomicBoolean         m_connectionAlive;
	std::string                             m_connName;
	std::string                             m_connID;
	std::string                             m_hostName;
	std::string                             m_lowercaseUserName;
	std::string                             m_uppercaseUserName;
	int                                     m_processID;
	mutable std::string                     m_mwInfo;
	InternalAutoDispatcher                  m_autoDispatcher;
	GMSEC_I32                               m_defaultRepublish_ms;
	GMSEC_I32                               m_requestPublish_ms;
	SubscriptionInfoList                    m_subscriptions;
	Subscriptions                           m_subscriptionRegistry;
	ExclusionFilter*                        m_exclusionFilter;

	CallbackLookup*                         m_callbackLookup;
	EventCallbackMap                        m_eventCallbacks;

	RequestThread                           m_requestThread;
	RequestSharedData                       m_requestShared;

	PerfLoggerThread                        m_perfLoggerThread;
	PerfLoggerSharedData                    m_perfLoggerShared;
	bool                                    m_usePerfLogger;
	std::string                             m_perfLoggerFilename;

	bool                                    m_useGlobalAsyncPublish;
	size_t                                  m_asyncQueueDepth;
	unsigned int                            m_asyncTeardownWait;
	AsynchronousQueue*                      m_asyncQueue;
	AsynchronousThread                      m_asyncPubThread;
	AsynchronousPubService                  m_asyncPubService;

	MsgAggregationToolkitThread             m_msgAggregationToolkitThread;
	MsgAggregationToolkitShared             m_msgAggregationToolkitShared;

	// Will be used for support of legacy Callbacks
	CallbackAdapter*                        m_callbackAdapter;

	gmsec::api::Connection*                 m_parent;

	std::string                             m_connectionEndpoint;

	unsigned int                            m_specVersion;
	int                                     m_specLevel;

	bool                                    m_usingAPI3x;

	static ActiveSubscriptions              s_activeSubscriptions;

	friend class InternalAutoDispatcher;
};

} //namespace internal
} //namespace api
} //namespace gmsec

#endif
