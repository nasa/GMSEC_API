/*
 * Copyright 2007-2016 United States Government as represented by the
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
#include <gmsec4/util/wdllexp.h>

#include <gmsec4/secure/Policy.h>

#include <string>
#include <list>
#include <map>
#include <memory>
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
	static const GMSEC_I32 MIN_TIMEOUT_ms       = 10;
	static const GMSEC_I32 REQUEST_PUBLISH_ms   = 10000;
	static const GMSEC_I32 REPUBLISH_NEVER      = -1;
	static const GMSEC_I32 DEFAULT_REPUBLISH_ms = 60000;
	static const GMSEC_I32 MIN_REPUBLISH_ms     = 100;


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


	virtual void CALL_TYPE publish(const Message& msg, const Config& config);


	virtual void CALL_TYPE request(const Message& request, GMSEC_I32 timeout, ReplyCallback* cb, GMSEC_I32 republish_ms = 0);


	virtual Message* CALL_TYPE request(const Message& request, GMSEC_I32 timeout, GMSEC_I32 republish_ms = 0);


	virtual void CALL_TYPE cancelRequest(ReplyCallback* cb);


	virtual void CALL_TYPE reply(const Message& request, const Message& reply);


	virtual Message* CALL_TYPE receive(GMSEC_I32 timeout = -1);


	virtual void CALL_TYPE dispatch(const Message& msg);


	 virtual void CALL_TYPE release(Message*& msg);


	virtual void excludeSubject(const char *subject);


	virtual void removeExcludedSubject(const char *subject);


	virtual const char* CALL_TYPE getName() const;


	virtual void CALL_TYPE setName(const char* name);


	virtual const char* CALL_TYPE getID() const;


	virtual const char* CALL_TYPE getMWInfo() const;


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


	GMSEC_U32 CALL_TYPE getMessageCounter() const;


	void CALL_TYPE dispatchEvent(Connection::ConnectionEvent event, const Status& status);


	gmsec::api::secure::Policy& CALL_TYPE getPolicy();


	RequestSpecs CALL_TYPE getRequestSpecs() const;


	virtual bool CALL_TYPE onReply(Message* reply);


	void replyEvent(ReplyCallback* rcb, const Status& status, Connection::ConnectionEvent event);


	void replyCallback(ReplyCallback* rcb, const Message& request, const Message& reply);


	void issueRequestToMW(const Message& request, std::string& id);


	static void CALL_TYPE updateReplySubject(Message*& reply, bool warn = false);


	const TrackingDetails& getTracking();


	void autoDispatch();


	void insertTrackingFields(Message& msg);
	void removeTrackingFields(Message& msg);

	void usingAPI3x();


private:
	void getNextMsg(Message*& msg, GMSEC_I32 timeout);
	void dispatchMsgToCallbacks(const Message& msg, std::list<Callback*>& callbacks);

	bool checkExistingSubscription(const char* subject, Callback* cb = NULL) const;
	void unsubscribeAux(SubscriptionInfo*& info);

	void initializeTracking();
	void initializeRequest();

	void resolveRequestTimeout(GMSEC_I32& timeout_ms);
	void resolveRepublishInterval(GMSEC_I32& republish_ms);

	void startRequestThread();
	void stopRequestThread();

	void startPerfLoggerThread();
	void stopPerfLoggerThread();

	void startMsgAggregationToolkitThread();
	void stopMsgAggregationToolkitThread();

	gmsec::api::util::TicketMutex& getReadMutex();
	gmsec::api::util::TicketMutex& getWriteMutex();
	gmsec::api::util::TicketMutex& getEventMutex();

	// defined, but not implemented
	InternalConnection();
	InternalConnection(const InternalConnection&);
	InternalConnection &operator=(const InternalConnection&);

	// useful definitions
	typedef std::list<SubscriptionInfo*>                SubscriptionInfoList;
	typedef std::map<std::string, SubscriptionDetails*> Subscriptions;

	typedef std::map<Connection::ConnectionEvent, EventCallback*> EventCallbackMap;

	typedef std::auto_ptr<gmsec::api::util::StdThread>    RequestThread;
	typedef gmsec::api::util::StdSharedPtr<RequestShared> RequestSharedData;

	typedef std::auto_ptr<gmsec::api::util::StdThread>                     PerfLoggerThread;
	typedef gmsec::api::util::StdSharedPtr<performance::PerformanceLogger> PerfLoggerSharedData;

	typedef gmsec::api::util::BoundedQueue<MessagePublishTask> AsynchronousQueue;
	typedef std::auto_ptr<gmsec::api::util::StdThread>         AsynchronousThread;
	typedef gmsec::api::util::StdSharedPtr<AsyncPublisher>     AsynchronousPubService;

	typedef std::auto_ptr<gmsec::api::util::StdThread>                MsgAggregationToolkitThread;
	typedef gmsec::api::util::StdSharedPtr<MessageAggregationToolkit> MsgAggregationToolkitShared;


	// Member data
	gmsec::api::util::TicketMutex           m_readMutex;
	gmsec::api::util::TicketMutex           m_writeMutex;
	gmsec::api::util::TicketMutex           m_eventMutex;

	Config                                  m_config;
	ConnectionInterface*                    m_connIf;
	RequestSpecs                            m_requestSpecs;
	TrackingDetails                         m_tracking;
	gmsec::api::secure::Policy              m_policy;
	GMSEC_U32                               m_connectionID;
	GMSEC_U32                               m_messageCounter;
	gmsec::api::Connection::ConnectionState m_state;
	gmsec::api::util::AtomicBoolean         m_connectionAlive;
	std::string                             m_connName;
	std::string                             m_connID;
	std::string                             m_userName;
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

	bool                                    m_usingAPI3x;

	friend class InternalAutoDispatcher;
};

} //namespace internal
} //namespace api
} //namespace gmsec

#endif
