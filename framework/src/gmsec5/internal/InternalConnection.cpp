/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file InternalConnection.cpp
 */

#include <gmsec5/internal/InternalConnection.h>

#include <gmsec5/internal/AsyncPublisher.h>
#include <gmsec5/internal/AutoDispatcher.h>
#include <gmsec5/internal/CallbackAdapter.h>
#include <gmsec5/internal/CallbackLookup.h>
#include <gmsec5/internal/ConnectionCallback.h>
#include <gmsec5/internal/ConnectionCallbackCache.h>
#include <gmsec5/internal/ConnectionFactory.h>
#include <gmsec5/internal/ConnectionInterface.h>
#include <gmsec5/internal/DynamicFactory.h>
#include <gmsec5/internal/ExclusionFilter.h>
#include <gmsec5/internal/InternalMessageFactory.h>
#include <gmsec5/internal/InternalSubscriptionInfo.h>
#include <gmsec5/internal/MessageAggregationToolkit.h>
#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/Middleware.h>
#include <gmsec5/internal/RequestThread.h>
#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/internal/SystemUtil.h>
#include <gmsec5/internal/TicketMutex.h>

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5/Config.h>
#include <gmsec5/Callback.h>
#include <gmsec5/EventCallback.h>
#include <gmsec5/ReplyCallback.h>
#include <gmsec5/SubscriptionInfo.h>

#include <gmsec5/util/Log.h>

#include <exception>
#include <sstream>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


static int DEFAULT_MAX_CONN_RETRIES    = 0;
static int DEFAULT_CONN_RETRY_INTERVAL = 5000;   // 5 seconds

static int DEFAULT_ASYNC_QUEUE_DEPTH   = 1000;
static int DEFAULT_ASYNC_TEARDOWN_WAIT = 1000;   // 1 second


// Connection instance counter
static GMSEC_U32 s_instanceCount = 0;

ActiveSubscriptions InternalConnection::s_activeSubscriptions;


InternalConnection::InternalConnection(Connection* parent, const Config& config)
	: m_parent(parent),
	  m_config(config),
	  m_connIf(ConnectionFactory::create(config)),
	  m_validate(VALIDATE_NONE),
	  m_msgFactory(0),
	  m_msgFactoryOwned(true),
	  m_requestSpecs(),
	  m_removeTrackingFields(false),
	  m_tracking(),
	  m_connectionID(0),
	  m_msgConfig(),
	  m_messageCounter(0),
	  m_state(InternalConnection::NOT_CONNECTED),
	  m_connectionAlive(false),
	  m_connName(),
	  m_connID(),
	  m_hostName(),
	  m_lowercaseUserName(),
	  m_uppercaseUserName(),
	  m_processID(0),
	  m_mwInfo(),
	  m_autoDispatcher(this, m_readMutex, m_writeMutex),
	  m_defaultRepublish_ms(DEFAULT_REPUBLISH_ms),
	  m_requestPublish_ms(GMSEC_REQUEST_PUBLISH_ms),
	  m_subscriptions(),
	  m_exclusionFilter(new ExclusionFilter()),
	  m_callbackLookup(new CallbackLookup()),
	  m_eventCallbacks(),
	  m_usePerfLogger(false),
	  m_perfLoggerFilename(),
	  m_useGlobalAsyncPublish(false),
	  m_asyncQueueDepth(DEFAULT_ASYNC_QUEUE_DEPTH),
	  m_asyncTeardownWait(DEFAULT_ASYNC_TEARDOWN_WAIT),
	  m_asyncQueue(0),
	  m_callbackAdapter(0),
	  m_connectionEndpoint(),
	  m_maxConnRetries(DEFAULT_MAX_CONN_RETRIES),
	  m_connRetryInterval(DEFAULT_CONN_RETRY_INTERVAL),
	  m_supportLegacyAPI(false)
{
	initMessageFactory();
	initConnection();
}


InternalConnection::InternalConnection(Connection* parent, const Config& config, MessageFactory& factory)
	: m_parent(parent),
	  m_config(config),
	  m_connIf(ConnectionFactory::create(config)),
	  m_validate(VALIDATE_NONE),
	  m_msgFactory(&factory),
	  m_msgFactoryOwned(false),
	  m_requestSpecs(),
	  m_removeTrackingFields(false),
	  m_tracking(),
	  m_connectionID(0),
	  m_msgConfig(),
	  m_messageCounter(0),
	  m_state(InternalConnection::NOT_CONNECTED),
	  m_connectionAlive(false),
	  m_connName(),
	  m_connID(),
	  m_hostName(),
	  m_lowercaseUserName(),
	  m_uppercaseUserName(),
	  m_processID(0),
	  m_mwInfo(),
	  m_autoDispatcher(this, m_readMutex, m_writeMutex),
	  m_defaultRepublish_ms(DEFAULT_REPUBLISH_ms),
	  m_requestPublish_ms(GMSEC_REQUEST_PUBLISH_ms),
	  m_subscriptions(),
	  m_exclusionFilter(new ExclusionFilter()),
	  m_callbackLookup(new CallbackLookup()),
	  m_eventCallbacks(),
	  m_usePerfLogger(false),
	  m_perfLoggerFilename(),
	  m_useGlobalAsyncPublish(false),
	  m_asyncQueueDepth(DEFAULT_ASYNC_QUEUE_DEPTH),
	  m_asyncTeardownWait(DEFAULT_ASYNC_TEARDOWN_WAIT),
	  m_asyncQueue(0),
	  m_callbackAdapter(0),
	  m_connectionEndpoint(),
	  m_maxConnRetries(DEFAULT_MAX_CONN_RETRIES),
	  m_connRetryInterval(DEFAULT_CONN_RETRY_INTERVAL),
	  m_supportLegacyAPI(false)
{
	initMessageFactory();
	initConnection();
}


InternalConnection::~InternalConnection()
{
	GmsecException error(NO_ERROR_CLASS, NO_ERROR_CODE, "");

	m_connectionAlive.set(false);

	try {
		disconnect();
	}
	catch (const GmsecException& e) {
		error = e;
	}

	// Remove any lingering subscriptions
	for (SubscriptionInfoList::iterator it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it)
	{
		SubscriptionInfo* info = *it;

		s_activeSubscriptions.removeTopic(info->getSubjectPattern());

		Callback*           callback     = ConnectionCallbackCache::getCache().getAndRemoveCallback(info);
		ConnectionCallback* connCallback = dynamic_cast<ConnectionCallback*>(callback);

		if (connCallback != NULL)
		{
			delete connCallback;
		}

		delete info;
	}
	m_subscriptions.clear();

	ConnectionCallbackCache::getCache().destroyCallbacks(m_parent);

	delete m_asyncQueue;
	delete m_callbackAdapter;
	delete m_callbackLookup;
	delete m_exclusionFilter;

	if (!m_msgFactoryOwned)
	{
		(void) m_msgFactory.release();
	}

	delete m_connIf;

	if (error.getErrorClass() != NO_ERROR_CLASS)
	{
		GMSEC_ERROR << error.what();
	}
}


const char* InternalConnection::getAPIVersion()
{
	return GMSEC_VERSION;
}


void InternalConnection::connect()
{
	if (getState() == InternalConnection::CONNECTED)
	{
		GMSEC_WARNING << "The Connection has already been initialized!";
		return;
	}

	connectAux();
}


void InternalConnection::disconnect()
{
	// We need to disconnect before doing anything else;
	// this ensures that the auto-dispatcher, if running, is stopped so
	// that no more messages are dispatched to our callbacks.
	// Unfortunately, we have no guarantee that the auto-dispatcher will
	// stop 'immediately', thus we may still be at risk destroying our callbacks.

	// Gracefully handle any exceptions thrown by the call to disconnect()
	if (getState() == InternalConnection::CONNECTED)
	{
		try {
			disconnectAux();
		}
		catch (const GmsecException& e) {
			GMSEC_WARNING << e.what();
		}
	}
}


InternalConnection::ConnectionState InternalConnection::getState() const
{
	return m_state;
}


const char* InternalConnection::getLibraryName() const
{
	return m_connIf->getLibraryRootName();
}


const char* InternalConnection::getLibraryVersion() const
{
	return m_connIf->getLibraryVersion();
}


MessageFactory& InternalConnection::getMessageFactory() const
{
	return *m_msgFactory.get();
}


void InternalConnection::registerEventCallback(Connection::Event event, EventCallback* cb)
{
	if (!cb)
	{
		throw GmsecException(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "EventCallback cannot be NULL");
	}

	AutoTicket holdEvent(getEventMutex());

	m_eventCallbacks[event] = cb;
}


SubscriptionInfo* InternalConnection::subscribe(const char *subject, Callback* cb)
{
	Config emptyCfg;

	return subscribe(subject, emptyCfg, cb);
}


SubscriptionInfo* InternalConnection::subscribe(const char* subject0, const Config& config, Callback* cb)
{
	if (getState() != InternalConnection::CONNECTED)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been initialized");
	}

	if (subject0 == NULL || *subject0 == '\0')
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Subject is NULL or is empty string");
	}

	std::string subject = subject0;
	size_t pos = subject.find(ARTEMIS_MQ_TOPIC_DELIMITER);
	if (pos != std::string::npos)
	{
		// We have a subject with a queue name
		subject = subject.substr(0, pos);
	}

	if (!getPolicy().isValidSubscription(subject.c_str()))
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Subject is invalid");
	}

	// We only allow for only one registration of a subscription topic when a
	// callback is not provided.
	//
	// When a callback is provided, it must NOT be the same that is already
	// in use for a particular subscription topic (i.e. the callback must be
	// unique).
	SubscriptionStatus status = checkSubscription(subject.c_str(), cb);

	switch (status)
	{
	case IS_NEW:
		m_connIf->mwSubscribe(subject0, config);
		break;
	case NOT_NEW:
		// Already subscribed with middleware.
		break;
	case ERR_IS_DUPLICATE:
		throw GmsecException(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Duplicate subscription");
		break;
	case ERR_IS_DUPLICATE_USING_CALLBACK:
		throw GmsecException(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Duplicate subscription using same callback");
		break;
	}

	AutoTicket holdRead(getReadMutex());

	ConnectionCallback* connCallback = 0;

	if (cb)
	{
		// We need to conjure a proxy Callback that can be used to validate incoming messages.
		// The user supplied callback will be called using the proxy callback, except when
		// validation is enabled and a received message is not compliant.
		connCallback = new ConnectionCallback(m_parent, cb, validateOnRecv());

		m_callbackLookup->addCallback(subject.c_str(), connCallback);
	}

	GMSEC_VERBOSE << "Successfully subscribed to " << subject0;

	s_activeSubscriptions.addTopic(subject0);

	SubscriptionInfo* info = new SubscriptionInfo(new InternalSubscriptionInfo(m_parent, subject0, config, connCallback));

	m_subscriptions.push_front(info);

	if (connCallback)
	{
		ConnectionCallbackCache::getCache().putCallback(info, connCallback);
	}

	return info;
}


void InternalConnection::unsubscribe(SubscriptionInfo*& info)
{
	if (getState() != InternalConnection::CONNECTED)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been initialized");
	}

	if (!info)
	{
		throw GmsecException(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Cannot unsubscribe using NULL SubscriptionInfo object.");
	}

	if (m_parent != info->getInternal()->getConnection())
	{
		throw GmsecException(CONNECTION_ERROR, OTHER_ERROR_CODE, "SubscriptionInfo object is not associated with this Connection");
	}

	AutoTicket holdRead(getReadMutex());

	SubscriptionInfoList::iterator it = m_subscriptions.begin(); 
	for (; it != m_subscriptions.end(); ++it)
	{
		if (*it == info)
		{
			break;
		}
	}
	if (it == m_subscriptions.end())
	{
		// Odd; we did not find the SubscriptionInfo registered in our internal table.
#ifdef GMSEC_USING_VS2008
		holdRead.leave();
#endif

		throw GmsecException(CONNECTION_ERROR, OTHER_ERROR_CODE, "The given SubscriptionInfo object is not registered with this Connection");
	}
	m_subscriptions.erase(it);

	try
	{
		unsubscribeAux(info);

		delete info;
		info = 0;
	}
	catch (const GmsecException& e)
	{
#ifdef GMSEC_USING_VS2008
		holdRead.leave();
#endif
		throw e;
	}
}


bool InternalConnection::startAutoDispatch()
{
	if (getState() != InternalConnection::CONNECTED)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been initialized");
	}

	return m_autoDispatcher.start();
}


bool InternalConnection::stopAutoDispatch(bool waitForCompletion)
{
	if (getState() != InternalConnection::CONNECTED)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been initialized");
	}

	return m_autoDispatcher.stop(waitForCompletion);
}


void InternalConnection::publish(const Message& msg)
{
	Config emptyCfg;
	publish(msg, emptyCfg);
}


void InternalConnection::publish(const Message& msg, const Config& mwConfig)
{
	if (getState() != InternalConnection::CONNECTED)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been initialized");
	}

	if (msg.getKind() != Message::Kind::PUBLISH)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_MSG, "Cannot publish message with non-PUBLISH message kind.");
	}

	if (!validateOnSend() && *msg.getSubject() == 0)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_MSG, "Message contains an empty subject. Use Message::setSubject");
	}

	// Do not validate aggregated (binned) messages (currently no message template is available that fits the message).
	if (validateOnSend())
	{
		bool checkMsg = true;

		if (MessageAggregationToolkit::isAggregatedMsg(&msg))
		{
			checkMsg = false;
		}

		if (checkMsg)
		{
			if (m_removeTrackingFields || msg.getConfig().getBooleanValue(GMSEC_REMOVE_TRACKING_FIELDS, false))
			{
				removeTrackingFields(const_cast<Message&>(msg));
			}

			MessageBuddy::getInternal(msg).getDetails().setBoolean(GMSEC_CHECK_TRACKING_FIELDS, true);
			Status status = msg.isCompliant();

			if (status.hasError())
			{
				throw GmsecException(status);
			}
		}
	}

	insertTrackingFields(const_cast<Message&>(msg));

	bool msgWasAggregated = false;

	if (m_msgAggregationToolkitThread.get())
	{
		// Check if an error has previously occurred with an attempt to publish an aggregated message.
		Status result = m_msgAggregationToolkitShared->getPublishStatus();

		if (result.hasError())
		{
			throw GmsecException(result);
		}

		msgWasAggregated = m_msgAggregationToolkitShared->addMessage(msg, mwConfig);
	}

	GmsecException result(NO_ERROR_CLASS, NO_ERROR_CODE, "");

	if (!msgWasAggregated)
	{
		if (m_useGlobalAsyncPublish || mwConfig.getBooleanValue(GMSEC_ASYNC_PUBLISH, false))
		{
			if (!m_asyncPubService.get())
			{
				m_asyncPubService.reset(new AsyncPublisher(m_connIf, m_asyncQueue));
				m_asyncPubThread.reset(new StdThread(&AsyncPublisher::start, m_asyncPubService));
				m_asyncPubThread->start();
			}

			MessagePublishTask mpt;
			mpt.msg    = new Message(msg);
			mpt.config = new Config(mwConfig);

			m_asyncQueue->put(mpt);
		}
		else
		{
			AutoTicket hold(getWriteMutex());

			try {
				m_connIf->mwPublish(msg, mwConfig);
			}
			catch (const GmsecException& e) {
				result = e;
			}
		}
	}

	removeTrackingFields(const_cast<Message&>(msg));

	if (result.getErrorClass() != NO_ERROR_CLASS)
	{
		throw result;
	}
}


void InternalConnection::request(const Message& request, GMSEC_I32 timeout, ReplyCallback* cb, GMSEC_I32 republish_ms)
{
	if (getState() != InternalConnection::CONNECTED)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been initialized");
	}

	if (request.getKind() != Message::Kind::REQUEST)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_MSG, "Cannot issue request with non-REQUEST kind message.");
	}

	if (!cb)
	{
		throw GmsecException(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "ReplyCallback cannot be NULL");
	}

	if (!validateOnSend() && *request.getSubject() == 0)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_MSG, "Request message contains an empty subject");
	}

	if (validateOnSend())
	{
		if (m_removeTrackingFields || request.getConfig().getBooleanValue(GMSEC_REMOVE_TRACKING_FIELDS, false))
		{
			removeTrackingFields(const_cast<Message&>(request));
		}

		MessageBuddy::getInternal(request).getDetails().setBoolean(GMSEC_CHECK_TRACKING_FIELDS, true);
		Status status = request.isCompliant();

		if (status.hasError())
		{
			throw GmsecException(status);
		}
	}

	// Do we need to set up a subscription to receive a response?
	if (getRequestSpecs().subscribeForResponse)
	{
		setupSubscriptionForResponse(request);
	}

	StdSharedPtr<PendingRequest> pending;

	// We make a clone of the user's request message so that the API can manage the lifeline of such.
	// The user is responsible for managing the lifeline of their request message.
	Message* requestClone = new Message(request);

	startRequestThread();

	resolveRequestTimeout(timeout);
	resolveRepublishInterval(republish_ms);

	pending.reset(new PendingRequest(requestClone, timeout, republish_ms));

	pending->replyCallback = cb;
	pending->multiResponse = m_requestSpecs.multiResponse;

	m_requestShared->add(pending);

	if (!pending->sentLatch.await(m_requestPublish_ms))
	{
		GMSEC_WARNING << "Request(req, timeout, callback): initial send has not completed";
	}
}


Message* InternalConnection::request(const Message& request, GMSEC_I32 timeout, GMSEC_I32 republish_ms)
{
	if (getState() != InternalConnection::CONNECTED)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been initialized");
	}

	if (request.getKind() != Message::Kind::REQUEST)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_MSG, "Cannot issue request with non-REQUEST kind message.");
	}

	if (!validateOnSend() && *request.getSubject() == 0)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_MSG, "Request message contains an empty subject");
	}

	if (validateOnSend())
	{
		if (m_removeTrackingFields || request.getConfig().getBooleanValue(GMSEC_REMOVE_TRACKING_FIELDS, false))
		{
			removeTrackingFields(const_cast<Message&>(request));
		}

		MessageBuddy::getInternal(request).getDetails().setBoolean(GMSEC_CHECK_TRACKING_FIELDS, true);
		Status status = request.isCompliant();

		if (status.hasError())
		{
			throw GmsecException(status);
		}
	}

	// Do we need to set up a subscription to receive a response?
	if (getRequestSpecs().subscribeForResponse)
	{
		setupSubscriptionForResponse(request);
	}

	Message* reply = NULL;

	StdSharedPtr<PendingRequest> pending;

	// We make a clone of the user's request message so that the API can manage the lifeline of such.
	// The user is responsible for managing the lifeline of their request message.
	Message* requestClone = new Message(request);

	startRequestThread();

	resolveRequestTimeout(timeout);
	resolveRepublishInterval(republish_ms);

	pending.reset(new PendingRequest(requestClone, timeout, republish_ms));

	{
		Condition& condition = m_requestShared->getCondition();
		AutoMutex hold(condition.getMutex());

		m_requestShared->add(pending);

		const int DEFAULT_WAIT_ms = 10000; // check back every 10 seconds
		const int MIN_WAIT_ms     = 10;
		int       wait_ms         = DEFAULT_WAIT_ms;

		if (timeout >= 0)
		{
			wait_ms = (timeout > MIN_WAIT_ms ? timeout : MIN_WAIT_ms);
		}

		Status result;

		while (!result.hasError())
		{
			// minimize holding condition / mutex
			int reason = condition.wait(wait_ms);

			if (reason == RequestShared::DELIVER_REPLY)
			{
				if (pending->reply)
				{
					reply = pending->reply;

					// note that this request is no longer active
					pending->expireTime_s = TimeUtil::getCurrentTime_s() - 1;
					break;
				}
			}
			else if (reason == RequestShared::QUIT)
			{
				result.set(CONNECTION_ERROR, REQUEST_DISPATCH_FAILURE, "Request interrupted with QUIT");
				GMSEC_ERROR << result.get();
			}
			else if (reason == Condition::TIMEOUT)
			{
				if (timeout == GMSEC_WAIT_FOREVER)
				{
					GMSEC_DEBUG << "Request(req, timeout, reply): inconsequential timeout [GMSEC_WAIT_FOREVER]";
				}
				else if (TimeUtil::getCurrentTime_s() > pending->expireTime_s)
				{
					Status localResult(CONNECTION_ERROR, TIMEOUT_OCCURRED, "Request timed out");

					// Check if a request timeout callback is associated with the connection.
					dispatchEvent(Connection::Event::REQUEST_TIMEOUT_EVENT, localResult);

					if (republish_ms == GMSEC_REQUEST_REPUBLISH_NEVER)
					{
						result = localResult;
					}

					if (republish_ms > 0)
					{
						pending->expireTime_s += (republish_ms / 1000.0);
					}
				}
			}
		}
	}

	if (!pending->sentOnce)
	{
		if (!pending->sentLatch.await(m_requestPublish_ms))
		{
			GMSEC_WARNING << "Request(req, timeout, reply): initial send has not completed";
		}
	}

	return reply;
}


void InternalConnection::reply(const Message& request, const Message& reply)
{
	if (getState() != InternalConnection::CONNECTED)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been initialized");
	}

	if (request.getKind() != Message::Kind::REQUEST)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_MSG, "Cannot issue reply with non-REQUEST kind message.");
	}

	if (reply.getKind() != Message::Kind::REPLY)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_MSG, "Cannot issue reply with non-REPLY kind message.");
	}

	if (!validateOnSend() && *reply.getSubject() == 0)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_MSG, "Reply message contains an empty subject. Use Message::setSubject");
	}

	if (validateOnSend())
	{
		if (m_removeTrackingFields || reply.getConfig().getBooleanValue(GMSEC_REMOVE_TRACKING_FIELDS, false))
		{
			removeTrackingFields(const_cast<Message&>(reply));
		}

		MessageBuddy::getInternal(reply).getDetails().setBoolean(GMSEC_CHECK_TRACKING_FIELDS, true);
		Status status = reply.isCompliant();

		if (status.hasError())
		{
			throw GmsecException(status);
		}
	}

	ValueMap& meta = MessageBuddy::getInternal(request).getDetails();

	std::string id;
	if (meta.getString(GMSEC_REPLY_UNIQUE_ID_FIELD, id).hasError())
	{
		meta.getString(LEGACY_GMSEC_REPLY_UNIQUE_ID_FIELD, id);
	}

	if (id.empty())
	{
		GMSEC_WARNING << "Request Message does not contain " << GMSEC_REPLY_UNIQUE_ID_FIELD << " in meta";
	}
	else
	{
		const char* replyUniqueIdField = (m_supportLegacyAPI ? LEGACY_GMSEC_REPLY_UNIQUE_ID_FIELD : GMSEC_REPLY_UNIQUE_ID_FIELD);

		MessageBuddy::getInternal(reply).addField(replyUniqueIdField, id.c_str());
	}

	insertTrackingFields(const_cast<Message&>(reply));

	GmsecException result(NO_ERROR_CLASS, NO_ERROR_CODE, "");

	AutoTicket hold(getWriteMutex());

	// Publish the reply message
	Config config = Config();

	try {
		m_connIf->mwPublish(reply, config);
	}
	catch (const GmsecException& e) {
		result = e;
	}

	removeTrackingFields(const_cast<Message&>(reply));

	MessageBuddy::getInternal(reply).clearField(GMSEC_REPLY_UNIQUE_ID_FIELD);
	if (m_supportLegacyAPI)
	{
		MessageBuddy::getInternal(reply).clearField(LEGACY_GMSEC_REPLY_UNIQUE_ID_FIELD);
	}

	if (result.getErrorClass() != NO_ERROR_CLASS)
	{
		throw result;
	}
}


Message* InternalConnection::receive(GMSEC_I32 timeout)
{
	if (getState() != InternalConnection::CONNECTED)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been initialized");
	}

	if (m_autoDispatcher.isRunning())
	{
		throw GmsecException(CONNECTION_ERROR, AUTODISPATCH_EXCLUSIVE,
			"This method cannot be called when the Auto-Dispatcher is in use");
	}

	AutoTicket hold(getReadMutex());

	Message* msg = 0;

	getNextMsg(msg, timeout);

	if (msg && validateOnRecv() && !MessageAggregationToolkit::isAggregatedMsg(msg))
	{
		Status status = msg->isCompliant();

		if (status.hasError())
		{
			delete msg;

			throw GmsecException(status);
		}
	}

	return msg;
}


void InternalConnection::dispatch(const Message& msg)
{
	if (getState() != InternalConnection::CONNECTED)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been initialized");
	}

	if (m_autoDispatcher.isRunning())
	{
		throw GmsecException(CONNECTION_ERROR, AUTODISPATCH_EXCLUSIVE,
			"This method cannot be called when the Auto-Dispatcher is in use");
	}

	if (*msg.getSubject() == 0)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_MSG, "Cannot publish a message with an empty subject. Use Message::setSubject");
	}

	if (validateOnSend())
	{
		MessageBuddy::getInternal(msg).getDetails().setBoolean(GMSEC_CHECK_TRACKING_FIELDS, true);
		Status status = msg.isCompliant();

		if (status.hasError())
		{
			throw GmsecException(status);
		}
	}

	AutoTicket hold(getReadMutex());

	std::list<Callback*> callbacks;

	const char* subject = msg.getSubject();

	m_callbackLookup->collectCallbacks(subject, callbacks);

	if (callbacks.size() > 0)
	{
		dispatchMsgToCallbacks(msg, callbacks);
	}
	else
	{
		GMSEC_VERBOSE << "No callback(s) associated with message subject " << subject;
	}
}


void InternalConnection::excludeSubject(const char* subject)
{
	if (getState() != InternalConnection::CONNECTED)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been initialized");
	}

	if (subject == NULL || std::string(subject).empty())
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Subject is NULL or is empty-string");
	}

	if (!getPolicy().isValidSubscription(subject))
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Subject is invalid");
	}

	m_exclusionFilter->excludeSubject(subject);
}


void InternalConnection::removeExcludedSubject(const char* subject)
{
	if (getState() != InternalConnection::CONNECTED)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been initialized");
	}

	if (subject == NULL || std::string(subject).empty())
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Subject is NULL or is empty-string");
	}

	if (!getPolicy().isValidSubscription(subject))
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Subject is invalid");
	}

	m_exclusionFilter->removeExcludedSubject(subject);
}


const char* InternalConnection::getName() const
{
	return m_connName.c_str();
}


void InternalConnection::setName(const char* name)
{
	if (name != NULL)
	{
		m_connName = name;
	}
}


const char* InternalConnection::getID() const
{
	return m_connID.c_str();
}


const char* InternalConnection::getMWInfo() const
{
	if (m_connIf)
	{
		m_mwInfo = m_connIf->getMWInfo();
	}

	if (m_mwInfo.empty())
	{
		m_mwInfo = "<not available>";
	}

	return m_mwInfo.c_str();
}


const char* InternalConnection::getConnectionEndpoint() const
{
	return (m_connectionEndpoint.empty() ? "unknown" : m_connectionEndpoint.c_str());
}


GMSEC_U64 InternalConnection::getPublishQueueMessageCount() const
{
	return (m_asyncQueue ? (GMSEC_U64) m_asyncQueue->queuedElements() : 0);
}


void InternalConnection::shutdownAllMiddlewares()
{
	gmsec::api5::internal::Middleware::shutdownAll();
}


void InternalConnection::shutdownMiddleware(const char* name)
{
	gmsec::api5::internal::Middleware::shutdown(name);
}


void InternalConnection::registerEventCallback(Connection::Event event, GMSEC_EventCallback* ecb)
{
	EventCallback* callback = m_callbackAdapter->createEventCallback(ecb);

	registerEventCallback(event, callback);
}


SubscriptionInfo* InternalConnection::subscribe(const char* subject, const Config& cfg, GMSEC_Callback* cb)
{
	Callback* callback = m_callbackAdapter->createCallback(cb);

	SubscriptionInfo* info = subscribe(subject, cfg, callback);

	info->getInternal()->setCallbackFunction(reinterpret_cast<void*>(cb));

	return info;
}


void InternalConnection::request(const Message& req, GMSEC_I32 timeout, GMSEC_ReplyCallback* rcb, GMSEC_EventCallback* ecb, GMSEC_I32 republish_ms)
{
	ReplyCallback* callback = m_callbackAdapter->createReplyCallback(rcb, ecb);

	request(req, timeout, callback, republish_ms);
}


Config& InternalConnection::getConfig()
{
	return m_config;
}


const Config& InternalConnection::getMessageConfig() const
{
	return MessageFactoryBuddy::getInternal(*m_msgFactory.get()).getMessageConfig();
}


void InternalConnection::dispatchEvent(Connection::Event event, const Status& status)
{
	AutoTicket lock(getEventMutex());

	EventCallbackMap::iterator it = m_eventCallbacks.find(event);

	if (it == m_eventCallbacks.end())
	{
		it = m_eventCallbacks.find(Connection::Event::ALL_EVENTS);
	}

	if (it != m_eventCallbacks.end() && it->second != NULL)
	{
		try
		{
			it->second->onEvent(*m_parent, status, event);
		}
		catch (const GmsecException& e)
		{
			GMSEC_ERROR   << "An uncaught exception from an EventCallback was handled by the GMSEC API";
			GMSEC_VERBOSE << "Exception: " << e.what();
		}
		catch (const std::exception& e)
		{
			GMSEC_ERROR   << "An uncaught exception from an EventCallback was handled by the GMSEC API";
			GMSEC_VERBOSE << "Exception: " << e.what();
		}
	}
}


gmsec::api5::secure::Policy& InternalConnection::getPolicy()
{
	return m_policy;
}


RequestSpecs InternalConnection::getRequestSpecs() const
{
	return m_requestSpecs;
}


void InternalConnection::setReplyUniqueID(Message& msg, const std::string& uniqueID)
{
	ValueMap& details = MessageBuddy::getInternal(msg).getDetails();

	details.setString(GMSEC_REPLY_UNIQUE_ID_FIELD, uniqueID);
	if (m_supportLegacyAPI)
	{
		details.setString(LEGACY_GMSEC_REPLY_UNIQUE_ID_FIELD, uniqueID);
	}
}


std::string InternalConnection::getReplyUniqueID(const Message& msg)
{
	std::string uniqueID;

	ValueMap& details = MessageBuddy::getInternal(msg).getDetails();

	details.getString(GMSEC_REPLY_UNIQUE_ID_FIELD, uniqueID);

	if (m_supportLegacyAPI && uniqueID.empty())
	{
		details.getString(LEGACY_GMSEC_REPLY_UNIQUE_ID_FIELD, uniqueID);
	}

	return uniqueID;
}


bool InternalConnection::onReply(Message* reply)
{
	bool replyDelivered = true;

	if (m_supportLegacyAPI && reply->hasField(LEGACY_GMSEC_REPLY_UNIQUE_ID_FIELD))
	{
        MessageBuddy::getInternal(*reply).addField(GMSEC_REPLY_UNIQUE_ID_FIELD, reply->getStringValue(LEGACY_GMSEC_REPLY_UNIQUE_ID_FIELD));

        MessageBuddy::getInternal(*reply).clearField(LEGACY_GMSEC_REPLY_UNIQUE_ID_FIELD);
	}

	StdSharedPtr<RequestShared> tmp(m_requestShared);

	if (tmp.get())
	{
		tmp->onReply(reply);
	}
	else
	{
		delete reply;

		replyDelivered = false;
	}

	return replyDelivered;
}


void InternalConnection::replyEvent(ReplyCallback* rcb, const Status& status, Connection::Event event)
{
	AutoTicket hold(getEventMutex());

	if (rcb)
	{
		try
		{
			rcb->onEvent(*m_parent, status, event);
		}
		catch (const GmsecException& e)
		{
			GMSEC_ERROR   << "An uncaught exception from an EventCallback was handled by the GMSEC API";
			GMSEC_VERBOSE << "Exception: " << e.what();
		}
		catch (const std::exception& e)
		{
			GMSEC_ERROR   << "An uncaught exception from an EventCallback was handled by the GMSEC API";
			GMSEC_VERBOSE << "Exception: " << e.what();
		}
	}
}


void InternalConnection::replyCallback(ReplyCallback* rcb, const Message& request, const Message& reply)
{
	if (rcb)
	{
		try
		{
			rcb->onReply(*m_parent, request, reply);
		}
		catch (const GmsecException& e)
		{
			GMSEC_ERROR   << "An uncaught exception from a ReplyCallback was handled by the GMSEC API";
			GMSEC_VERBOSE << "Exception: " << e.what();
		}
		catch (const std::exception& e)
		{
			GMSEC_ERROR   << "An uncaught exception from a ReplyCallback was handled by the GMSEC API";
			GMSEC_VERBOSE << "Exception: " << e.what();
		}
	}
}


void InternalConnection::issueRequestToMW(const Message& request, std::string& id)
{
	Message requestCopy(request);

	insertTrackingFields(requestCopy);

	AutoTicket hold(getWriteMutex());

	// We do not need to catch m/w exception here since the tracking fields are
	// added to a copy of the request message, not the original request message.
	id = m_connIf->mwGetUniqueID();

	const char* replyUniqueIdField = (m_supportLegacyAPI ? LEGACY_GMSEC_REPLY_UNIQUE_ID_FIELD : GMSEC_REPLY_UNIQUE_ID_FIELD);

	MessageBuddy::getInternal(requestCopy).addField(replyUniqueIdField, id.c_str());

	m_connIf->mwRequest(requestCopy, id);
}


const TrackingDetails& InternalConnection::getTracking()
{
	return m_tracking;
}


void InternalConnection::autoDispatch()
{
	if (!m_autoDispatcher.isRunning())
	{
		throw GmsecException(OTHER_ERROR, OTHER_ERROR_CODE, "Auto-dispatcher is not running");
	}

	// lock the InternalConnection
	AutoTicket holdRead(getReadMutex());

	static const int AUTO_DISPATCH_ms = 250;

	// Get the next message
	StdUniquePtr<Message> msg;

	try
	{
		Message* tmp = NULL;

		getNextMsg(tmp, AUTO_DISPATCH_ms);

		msg.reset(tmp);

		if (msg.get())
		{
			std::list<Callback*> callbacks;

			m_callbackLookup->collectCallbacks(msg->getSubject(), callbacks);

			dispatchMsgToCallbacks(*msg, callbacks);
		}
	}
	catch (const GmsecException& e)
	{
		dispatchEvent(Connection::Event::DISPATCHER_ERROR_EVENT, Status(e));
	}

	if (msg.get())
	{
		Message* tmp = msg.release();
		Message::destroy(tmp);
	}
}


void InternalConnection::initConnection()
{
	// Establish connection ID
	m_connectionID = ++s_instanceCount;

	// Generate Connection ID
	m_processID = SystemUtil::getProcessID();

	int code = SystemUtil::getUserName(m_lowercaseUserName);
	if (code || m_lowercaseUserName.empty())
	{
		GMSEC_WARNING << "Error acquiring user name [code = " << code << "]";
		m_lowercaseUserName = "unknown";
	}
	m_uppercaseUserName = StringUtil::stringToUpper(m_lowercaseUserName);

	code = SystemUtil::getHostName(m_hostName);
	if (code || m_hostName.empty())
	{
		GMSEC_WARNING << "Error acquiring host name [code = " << code << "]";
		m_hostName = "unknown";
	}
	m_hostName = StringUtil::stringToUpper(m_hostName);

	// Set default connection name
	std::ostringstream oss;
	oss << "Connection " << m_connectionID;
	m_connName = oss.str();
	oss.str("");

	// Set default connection ID
	oss << "GMSEC_" << m_hostName << "_";
	{
		// apply timestamp <seconds:x><subseconds:04x>
		double tmp    = TimeUtil::getCurrentTime_s();
		unsigned sec  = unsigned(tmp);
		unsigned subs = unsigned((tmp - sec) * 65536);
		char buffer[32];

		StringUtil::stringFormat(buffer, sizeof(buffer), "%X%04X", sec, subs);
		oss << buffer;
	}
	oss << '_' << m_processID << '_' << m_connectionID;

	m_connID = oss.str();


	// Set up policy
	Status status = DynamicFactory::initialize(getPolicy(), m_config);

	if (status.hasError())
	{
		throw GmsecException(status);
	}

	m_removeTrackingFields = m_config.getBooleanValue(GMSEC_REMOVE_TRACKING_FIELDS, false);

	m_tracking = TrackingDetails::initialize(m_config);

	initializeRequest();


	// Check if performance logger is being enabled
	const char* perfLoggerFilename = m_config.getValue(GMSEC_LOG_PERFORMANCE);

	if (perfLoggerFilename)
	{
		m_usePerfLogger      = true;
		m_perfLoggerFilename = perfLoggerFilename;
	}


	// Check if asynchronous publishing is being enabled
	m_useGlobalAsyncPublish = m_config.getBooleanValue(GMSEC_ASYNC_PUBLISH, false);
	m_asyncQueueDepth       = m_config.getIntegerValue(GMSEC_ASYNC_PUBLISH_QUEUE_DEPTH, DEFAULT_ASYNC_QUEUE_DEPTH);
	m_asyncTeardownWait     = m_config.getIntegerValue(GMSEC_ASYNC_PUBLISH_TEARDOWN_WAIT, DEFAULT_ASYNC_TEARDOWN_WAIT);

	if (m_asyncQueueDepth < static_cast<int>(BoundedQueue<MessagePublishTask>::MIN_QUEUE_SIZE))
	{
		GMSEC_WARNING << "Invalid value for " << GMSEC_ASYNC_PUBLISH_QUEUE_DEPTH
		              << ", defaulting to " << DEFAULT_ASYNC_QUEUE_DEPTH;

		m_asyncQueueDepth = DEFAULT_ASYNC_QUEUE_DEPTH;
	}
	else if (m_asyncQueueDepth > static_cast<int>(BoundedQueue<MessagePublishTask>::MAX_QUEUE_SIZE))
	{
		GMSEC_WARNING << "Value for " << GMSEC_ASYNC_PUBLISH_QUEUE_DEPTH
		              << " exceeds maximum limit, defaulting to " << BoundedQueue<MessagePublishTask>::MAX_QUEUE_SIZE;

		m_asyncQueueDepth = static_cast<int>(BoundedQueue<MessagePublishTask>::MAX_QUEUE_SIZE);
	}

	if (m_asyncTeardownWait <= 0)
	{
		GMSEC_WARNING << "Invalid value for " << GMSEC_ASYNC_PUBLISH_TEARDOWN_WAIT
		              << ", defaulting to " << DEFAULT_ASYNC_TEARDOWN_WAIT;

		m_asyncTeardownWait = DEFAULT_ASYNC_TEARDOWN_WAIT;
	}

	// Client may elect to use asynchronous publishing via the configuration passed to
	// publish(). For such reason, we instantiate the message queue now.
	m_asyncQueue = new BoundedQueue<MessagePublishTask>( static_cast<size_t>(m_asyncQueueDepth) );


	// Always create an instance of MAT (it may be needed by a subscriber)
	m_msgAggregationToolkitShared.reset(new MessageAggregationToolkit(this, m_config));

	// Check if this Connection object will perform Message aggregation
	if (m_config.getBooleanValue(GMSEC_USE_MSG_BINS, false))
	{
		startMsgAggregationToolkitThread();
	}


	// Check if options related to connection retries and interval are provided; use default values
	// if not provided.
	m_maxConnRetries    = m_config.getIntegerValue(OPT_MAX_CONN_RETRIES, DEFAULT_MAX_CONN_RETRIES);
	m_connRetryInterval = m_config.getIntegerValue(OPT_CONN_RETRY_INTERVAL, DEFAULT_CONN_RETRY_INTERVAL);

	if (m_maxConnRetries < -1)
	{
		m_maxConnRetries = -1;
	}
	if (m_connRetryInterval < 0)
	{
		m_connRetryInterval = 0;
	}


	// Determine whether to validate messages, and to what extent (by default, validation is not performed)
	m_validate = m_config.getBooleanValue(GMSEC_MSG_CONTENT_VALIDATE,     false) ? VALIDATE_SEND : m_validate;
	m_validate = m_config.getBooleanValue(GMSEC_MSG_CONTENT_VALIDATE_ALL, false) ? VALIDATE_ALL  : m_validate;

	if (m_validate == VALIDATE_NONE)
	{
		m_validate = m_config.getBooleanValue(GMSEC_MSG_CONTENT_VALIDATE_RECV, false) ? VALIDATE_RECV : m_validate;

		if (m_config.getBooleanValue(GMSEC_MSG_CONTENT_VALIDATE_SEND, false) && m_validate == VALIDATE_RECV)
		{
			m_validate = VALIDATE_ALL;
		}
		else if (m_config.getBooleanValue(GMSEC_MSG_CONTENT_VALIDATE_SEND, false))
		{
			m_validate = VALIDATE_SEND;
		}
	}

	// Should we support comm with a legacy API?
	m_supportLegacyAPI = m_config.getBooleanValue(GMSEC_SUPPORT_LEGACY_API, false);

	// Instantiate C-binding callback adapter factory.
	m_callbackAdapter = new CallbackAdapter();

	m_connIf->setExternal(this);

	m_connectionAlive.set(true);
}


void InternalConnection::initMessageFactory()
{
	// Check if we already have a message factory; if not, then instantiate one.
	if (m_msgFactory.get() == NULL)
	{
		m_msgFactory.reset( new MessageFactory(m_config) );
	}
}


void InternalConnection::connectAux()
{
	if (getState() == InternalConnection::CONNECTED)
	{
		GMSEC_WARNING << "A connection to the bus has already been established!";
		throw GmsecException(CONNECTION_ERROR, CONNECTION_CONNECTED, "A connection to the bus has already been established!");
	}

	m_connIf->mwConnect();

	m_state = InternalConnection::CONNECTED;

	Status status = getPolicy().authenticate(*m_parent);

	if (status.hasError())
	{
		m_connIf->mwDisconnect();
		m_state = InternalConnection::NOT_CONNECTED;
		throw GmsecException(status);
	}

	if (m_usePerfLogger)
	{
		startPerfLoggerThread();
	}

	for (SubscriptionInfoList::iterator it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it)
	{
		SubscriptionInfo* info = *it;

		m_connIf->mwSubscribe(info->getSubjectPattern(), info->getInternal()->getConfig());
	}
}


void InternalConnection::disconnectAux()
{
	if (getState() == InternalConnection::CONNECTED)
	{
		if (m_autoDispatcher.isRunning())
		{
			m_autoDispatcher.stop(true);
		}

		stopRequestThread();

		if (m_asyncPubService.get())
		{
			m_asyncPubService->stop(m_asyncTeardownWait);
			m_asyncPubService.reset();
			m_asyncPubThread.reset();
		}

		MessagePublishTask mpt;
		while (!m_asyncQueue->empty())
		{
			if (m_asyncQueue->poll(500, mpt))
			{
				delete mpt.msg;
				delete mpt.config;
			}
		}

		stopMsgAggregationToolkitThread();

		GmsecException error(NO_ERROR_CLASS, NO_ERROR_CODE, "");
		try
		{
			m_connIf->mwDisconnect();
		}
		catch (const GmsecException& e)
		{
			error = e;
		}

		if (m_usePerfLogger)
		{
			stopPerfLoggerThread();
		}

		m_state = InternalConnection::NOT_CONNECTED;

		if (error.getErrorClass() != NO_ERROR_CLASS)
		{
			throw error;
		}
	}
}


void InternalConnection::getNextMsg(Message*& msg, GMSEC_I32 timeout)
{
	msg = NULL;

	if (getState() != InternalConnection::CONNECTED)
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been initialized");
	}

	if (timeout < 0 && timeout != GMSEC_WAIT_FOREVER)
	{
		timeout = GMSEC_WAIT_FOREVER;
	}

	double beforeOp_s = TimeUtil::getCurrentTime_s();
	bool   done       = false;

	while (!done)
	{
		if (m_msgAggregationToolkitShared->hasNextMsg())
		{
			msg = m_msgAggregationToolkitShared->nextMsg();

			// Attempt to add a message template to the message
			try
			{
				MessageFactoryBuddy::getInternal(getMessageFactory()).addMessageTemplate(*msg);
			}
			catch (...)
			{
				GMSEC_WARNING << "Cannot apply message template to de-aggregated message";
			}
		}
		else
		{
			m_connIf->mwReceive(msg, timeout);
		}

		if (msg != NULL && m_exclusionFilter->checkForExclusion(msg))
		{
			// Clean up the excluded message
			delete msg;
			msg = NULL;

			// If timeout is GMSEC_WAIT_FOREVER, continue waiting for a message
			if (timeout == GMSEC_WAIT_FOREVER)
			{
				continue;
			}
			// Otherwise, if there is still time left on the timeout, continue waiting for a message
			// with a new time calculated from the absolute time recorded before the original
			// mwReceive() call
			else
			{
				double elapsed_ms = 1000 * (TimeUtil::getCurrentTime_s() - beforeOp_s);
				timeout -= GMSEC_I32(elapsed_ms);
				if (timeout > 0)
				{
					continue;
				}
				else
				{
					// Timeout (no error)
					done = true;
				}
			}
		}
		else if (msg != NULL && MessageAggregationToolkit::isAggregatedMsg(msg))
		{
			m_msgAggregationToolkitShared->processAggregatedMsg(msg);

			// If there are messages (and there should be), always return the first one.
			if (m_msgAggregationToolkitShared->hasNextMsg())
			{
				msg = m_msgAggregationToolkitShared->nextMsg();

				// Attempt to add a message template to the message
				try
				{
					MessageFactoryBuddy::getInternal(getMessageFactory()).addMessageTemplate(*msg);
				}
				catch (...)
				{
					GMSEC_WARNING << "Cannot apply message template to de-aggregated message";
				}
			}
		}

		done = true;
	}

	if (msg != NULL && m_usePerfLogger)
	{
		const StringField* pubTime = dynamic_cast<const StringField*>(msg->getField("PUBLISH-TIME"));

		if (pubTime)
		{
			m_perfLoggerShared->dispatchLog(msg->getSubject(), pubTime->getValue());
		}
	}

	if (msg != NULL)
	{
		if (msg->getKind() == Message::Kind::REQUEST)
		{
			std::string uniqueID;

			if (msg->hasField(GMSEC_REPLY_UNIQUE_ID_FIELD))
			{
				uniqueID = msg->getStringValue(GMSEC_REPLY_UNIQUE_ID_FIELD);
			}
			else if (m_supportLegacyAPI && msg->hasField(LEGACY_GMSEC_REPLY_UNIQUE_ID_FIELD))
			{
				uniqueID = msg->getStringValue(LEGACY_GMSEC_REPLY_UNIQUE_ID_FIELD);
			}

			msg->clearField(GMSEC_REPLY_UNIQUE_ID_FIELD);
			msg->clearField(LEGACY_GMSEC_REPLY_UNIQUE_ID_FIELD);

			setReplyUniqueID(*msg, uniqueID);
		}
		else if (msg->getKind() == Message::Kind::REPLY)
		{
			msg->clearField(GMSEC_REPLY_UNIQUE_ID_FIELD);
			msg->clearField(LEGACY_GMSEC_REPLY_UNIQUE_ID_FIELD);
		}
	}
}


void InternalConnection::dispatchMsgToCallbacks(const Message& msg, std::list<Callback*>& callbacks)
{
	for (std::list<Callback*>::iterator i = callbacks.begin(); m_connectionAlive.get() && i != callbacks.end(); ++i)
	{
		Callback* callback = *i;

		try
		{
			callback->onMessage(*m_parent, msg);
		}
		catch (const GmsecException& e)
		{
			GMSEC_ERROR   << "An uncaught exception from a Callback was handled by the GMSEC API";
			GMSEC_VERBOSE << "Exception: " << e.what();
		}
		catch (const std::exception& e)
		{
			GMSEC_ERROR   << "An uncaught exception from a Callback was handled by the GMSEC API";
			GMSEC_VERBOSE << "Exception: " << e.what();
		}
	}
}


InternalConnection::SubscriptionStatus InternalConnection::checkSubscription(const char* subject, Callback* cb)
{
	AutoTicket holdRead(getReadMutex());

	SubscriptionStatus status = IS_NEW;

	for (SubscriptionInfoList::const_iterator it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it)
	{
		const SubscriptionInfo* info = *it;

		bool subjectFound = StringUtil::stringEquals(info->getSubjectPattern(), subject);

		if (subjectFound)
		{
			status = NOT_NEW;

			if (cb == NULL)
			{
				status = ERR_IS_DUPLICATE;
			}
			else
			{
				Callback*           callback     = info->getInternal()->getCallback();
				ConnectionCallback* connCallback = dynamic_cast<ConnectionCallback*>(callback);

				if ((connCallback && connCallback->getUserCallback() == cb) || (callback == cb))
				{
					status = ERR_IS_DUPLICATE_USING_CALLBACK;
				}
			}
		}
	}

	return status;
}


bool InternalConnection::haveRemainingSubscription(SubscriptionInfo*& info)
{
	bool remainingSubscription = false;

	for (SubscriptionInfoList::const_iterator it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it)
	{
		if (StringUtil::stringEquals((*it)->getSubjectPattern(), info->getSubjectPattern()))
		{
			remainingSubscription = true;
			break;
		}
	}

	return remainingSubscription;
}


void InternalConnection::unsubscribeAux(SubscriptionInfo*& info)
{
	if (haveRemainingSubscription(info) == false)
	{
		m_connIf->mwUnsubscribe(info->getSubjectPattern());
	}

	if (info->getInternal()->getCallback() != NULL)
	{
		std::string pattern = info->getSubjectPattern();
		size_t p = pattern.find(ARTEMIS_MQ_TOPIC_DELIMITER);
		if (p != std::string::npos)
		{
			pattern = pattern.substr(0, p);
		}

		m_callbackLookup->removeCallback(pattern.c_str(), info->getInternal()->getCallback());

		Callback*           callback     = ConnectionCallbackCache::getCache().getAndRemoveCallback(info);
		ConnectionCallback* connCallback = dynamic_cast<ConnectionCallback*>(callback);

		if (connCallback != NULL)
		{
			delete connCallback;
		}
	}

	s_activeSubscriptions.removeTopic(info->getSubjectPattern());
}


void InternalConnection::initializeRequest()
{
	m_requestSpecs.multiResponse = m_config.getBooleanValue(OPT_MULTI_RESP, false);
	m_requestSpecs.subscribeForResponse = m_config.getBooleanValue(OPT_SUBSCRIBE_FOR_RESPONSE, true);

	const char* value = m_config.getValue(GMSEC_REQ_REPUBLISH_MS);
	if (value)
	{
		GMSEC_I32 tmp = 0;
		std::istringstream is(value);
		is >> tmp;
		if (is)
		{
			m_defaultRepublish_ms = tmp;
			resolveRepublishInterval(m_defaultRepublish_ms);
		}
	}
}


void InternalConnection::setupSubscriptionForResponse(const Message& request)
{
	// Attempt to set up a subscription so that a response, if requested, can be delivered

	if (request.hasField("RESPONSE") && request.getBooleanValue("RESPONSE"))
	{
		// determine if user configured a reply subject string
		const char* topic = request.getConfig().getValue(GMSEC_REPLY_SUBJECT);

		if (topic)
		{
			if (checkSubscription(topic, NULL) == IS_NEW)
			{
				GMSEC_VERBOSE << "Subscribing for response message using topic: " << topic;
				subscribe(topic, Config(), reinterpret_cast<Callback*>(NULL));
			}
		}
		else
		{
			// attempt to deduce reply subject string
			const char* topic = MessageBuddy::getInternal(request).getResponseTopic();

			if (topic && !std::string(topic).empty())
			{
				if (checkSubscription(topic, NULL) == IS_NEW)
				{
					GMSEC_VERBOSE << "Subscribing for response message using topic: " << topic;
					subscribe(topic, Config(), reinterpret_cast<Callback*>(NULL));
				}
			}
			else
			{
				GMSEC_WARNING << "Cannot deduce response topic; unable to subscribe to receive response message(s)";
			}
		}

	}
}


void InternalConnection::insertTrackingFields(Message& msg)
{
	const TrackingDetails& connTracking = getTracking();
	const TrackingDetails& msgTracking  = MessageBuddy::getInternal(msg).getTracking();

	const int ON    = MESSAGE_TRACKINGFIELDS_ON;
	const int OFF   = MESSAGE_TRACKINGFIELDS_OFF;
	const int UNSET = MESSAGE_TRACKINGFIELDS_UNSET;

	// Add the Tracking Fields, if enabled
	bool addTracking = (connTracking.get() == ON && (msgTracking.get() == ON || msgTracking.get() == UNSET));

	if ((addTracking || connTracking.getNode() == ON || msgTracking.getNode() == ON) &&
	    (connTracking.getNode() != OFF && msgTracking.getNode() != OFF))
	{
		Field* field = new StringField("NODE", m_hostName.c_str(), true);
		FieldBuddy::getInternal(*field).isTracking(true);
		MessageBuddy::getInternal(msg).addField(*field, false);
	}

	if ((addTracking || connTracking.getProcessId() == ON || msgTracking.getProcessId() == ON) &&
	    (connTracking.getProcessId() != OFF && msgTracking.getProcessId() != OFF))
	{
		Field* field;

		if (getSpecVersion() == GMSEC_MSG_SPEC_2014_00 || getSpecVersion() == GMSEC_MSG_SPEC_2016_00)
		{
			field = new I16Field("PROCESS-ID", (GMSEC_I16) m_processID, true);
		}
		else
		{
			field = new U32Field("PROCESS-ID", (GMSEC_U32) m_processID, true);
		}
		FieldBuddy::getInternal(*field).isTracking(true);
		MessageBuddy::getInternal(msg).addField(*field, false);
	}

	if ((addTracking || connTracking.getUserName() == ON || msgTracking.getUserName() == ON) &&
	    (connTracking.getUserName() != OFF && msgTracking.getUserName() != OFF))
	{
		const char* userName = (getSpecVersion() < GMSEC_MSG_SPEC_2018_00 ? m_lowercaseUserName.c_str() : m_uppercaseUserName.c_str());

		Field* field = new StringField("USER-NAME", userName, true);
		FieldBuddy::getInternal(*field).isTracking(true);
		MessageBuddy::getInternal(msg).addField(*field, false);
	}

	if ((addTracking || connTracking.getConnectionId() == ON || msgTracking.getConnectionId() == ON) &&
	    (connTracking.getConnectionId() != OFF && msgTracking.getConnectionId() != OFF))
	{
		Field* field = new U32Field("CONNECTION-ID", static_cast<GMSEC_U32>(m_connectionID), true);
		FieldBuddy::getInternal(*field).isTracking(true);
		MessageBuddy::getInternal(msg).addField(*field, false);
	}

	if (m_usePerfLogger || ((addTracking || connTracking.getPublishTime() == ON || msgTracking.getPublishTime() == ON) &&
	    ((connTracking.getPublishTime() != OFF) && (msgTracking.getPublishTime() != OFF))))
	{
		GMSEC_TimeSpec ts = TimeUtil::getCurrentTime();
		char           curTime[GMSEC_TIME_BUFSIZE];

		TimeUtil::formatTime(ts, curTime);

		Field* field = new StringField("PUBLISH-TIME", curTime, true);
		FieldBuddy::getInternal(*field).isTracking(true);
		MessageBuddy::getInternal(msg).addField(*field, false);
	}

	if ((addTracking || connTracking.getUniqueId() == ON || msgTracking.getUniqueId() == ON) &&
	    (connTracking.getUniqueId() != OFF && msgTracking.getUniqueId() != OFF))
	{
		GMSEC_U32 counter = getNextMessageCounter();

		std::ostringstream oss;
		oss << m_connID << "_" << counter;

		Field* field = new StringField("UNIQUE-ID", oss.str().c_str(), true);
		FieldBuddy::getInternal(*field).isTracking(true);
		MessageBuddy::getInternal(msg).addField(*field, false);
	}

	if ((addTracking || connTracking.getMwInfo() == ON || msgTracking.getMwInfo() == ON) &&
	    (connTracking.getMwInfo() != OFF && msgTracking.getMwInfo() != OFF))
	{
		Field* field = new StringField("MW-INFO", getMWInfo(), true);
		FieldBuddy::getInternal(*field).isTracking(true);
		MessageBuddy::getInternal(msg).addField(*field, false);
	}

	// We only add NUM-OF-SUBSCRIPTIONS, SUBSCRIPTION.n.SUBJECT-PATTERN, and MW-CONNECTION-ENDPOINT
	// when working with a configuration other than the 2014 or 2016 ISDs. This is to ensure that
	// messages built per those ISD version remain compliant.
	//
	// If the user has not indicated which ISD to work with, then these tracking fields will be
	// included in the message.
	if (getSpecVersion() >= GMSEC_MSG_SPEC_2018_00)
	{
		// We only include subscription subject-pattern field(s) within Heartbeat Messages.
		const Field* c2cxSubtype = NULL;

		if (getSpecVersion() < GMSEC_MSG_SPEC_2019_00)
		{
			c2cxSubtype = msg.getField("C2CX-SUBTYPE");
		}
		else
		{
			c2cxSubtype = msg.getField("MESSAGE-SUBTYPE");
		}

		if (c2cxSubtype != NULL && StringUtil::stringEqualsIgnoreCase(c2cxSubtype->getStringValue(), "HB"))
		{
			if ((addTracking || connTracking.getActiveSubscriptions() == ON || msgTracking.getActiveSubscriptions() == ON) &&
			    (connTracking.getActiveSubscriptions() != OFF && msgTracking.getActiveSubscriptions() != OFF))
			{
				std::set<std::string> activeSubs = s_activeSubscriptions.getTopics();

				Field* numsubs = new U16Field("NUM-OF-SUBSCRIPTIONS", static_cast<GMSEC_U16>(activeSubs.size()));
				FieldBuddy::getInternal(*numsubs).isTracking(true);
				MessageBuddy::getInternal(msg).addField(*numsubs, false);

				if (activeSubs.size() > 0)
				{
					int n = 1;
					for (std::set<std::string>::iterator it = activeSubs.begin(); it != activeSubs.end(); ++it, ++n)
					{
						std::ostringstream fieldName;
						fieldName << "SUBSCRIPTION." << n << ".SUBJECT-PATTERN";

						Field* pattern = new StringField(fieldName.str().c_str(), it->c_str());
						FieldBuddy::getInternal(*pattern).isTracking(true);
						MessageBuddy::getInternal(msg).addField(*pattern, false);
					}
				}
			}

			if ((addTracking || connTracking.getConnectionEndpoint() == ON || msgTracking.getConnectionEndpoint() == ON) &&
			    (connTracking.getConnectionEndpoint() != OFF && msgTracking.getConnectionEndpoint() != OFF))
			{
				const char* endpoint = (m_connectionEndpoint.empty() ? "unknown" : m_connectionEndpoint.c_str());
				const char* fieldName;

				if (getSpecVersion() == GMSEC_MSG_SPEC_2018_00 && getSpecSchemaLevel() == Specification::SchemaLevel::LEVEL_0)
				{
					fieldName = "CONNECTION-ENDPOINT";     // spelling used within 2018 C2MS-draft
				}
				else
				{
					fieldName = "MW-CONNECTION-ENDPOINT";  // official spelling used within 2019 C2MS
				}

				Field* endpnt = new StringField(fieldName, endpoint);
				FieldBuddy::getInternal(*endpnt).isTracking(true);
				MessageBuddy::getInternal(msg).addField(*endpnt, false);
			}
		}
	}
}


void InternalConnection::removeTrackingFields(Message& msg)
{
	const TrackingDetails& connTracking = getTracking();
	const TrackingDetails& msgTracking  = MessageBuddy::getInternal(msg).getTracking();

	const int ON    = MESSAGE_TRACKINGFIELDS_ON;
	const int OFF   = MESSAGE_TRACKINGFIELDS_OFF;
	const int UNSET = MESSAGE_TRACKINGFIELDS_UNSET;

	// Remove the Tracking Fields, if they have been added by the API
	bool addTracking = (connTracking.get() == ON && (msgTracking.get() == ON || msgTracking.get() == UNSET));

	if ((addTracking || connTracking.getNode() == ON || msgTracking.getNode() == ON) &&
	    (connTracking.getNode() != OFF && msgTracking.getNode() != OFF))
	{
		msg.clearField("NODE");
	}
	if ((addTracking || connTracking.getProcessId() == ON || msgTracking.getProcessId() == ON) &&
	    (connTracking.getProcessId() != OFF && msgTracking.getProcessId() != OFF))
	{
		msg.clearField("PROCESS-ID");
	}
	if ((addTracking || connTracking.getUserName() == ON || msgTracking.getUserName() == ON) &&
	    (connTracking.getUserName() != OFF && msgTracking.getUserName() != OFF))
	{
		msg.clearField("USER-NAME");
	}
	if ((addTracking || connTracking.getConnectionId() == ON || msgTracking.getConnectionId() == ON) &&
	    (connTracking.getConnectionId() != OFF && msgTracking.getConnectionId() != OFF))
	{
		msg.clearField("CONNECTION-ID");
	}
	if ((addTracking || connTracking.getPublishTime() == ON || msgTracking.getPublishTime() == ON) &&
	    (connTracking.getPublishTime() != OFF && msgTracking.getPublishTime() != OFF))
	{
		msg.clearField("PUBLISH-TIME");
	}
	if ((addTracking || connTracking.getUniqueId() == ON || msgTracking.getUniqueId() == ON) &&
	    (connTracking.getUniqueId() != OFF && msgTracking.getUniqueId() != OFF))
	{
		msg.clearField("UNIQUE-ID");
	}
	if ((addTracking || connTracking.getMwInfo() == ON || msgTracking.getMwInfo() == ON) &&
	    (connTracking.getMwInfo() != OFF && msgTracking.getMwInfo() != OFF))
	{
		msg.clearField("MW-INFO");
	}

	if (getSpecVersion() >= GMSEC_MSG_SPEC_2018_00)
	{
		const Field* c2cxSubtype = NULL;

		if (getSpecVersion() < GMSEC_MSG_SPEC_2019_00)
		{
			c2cxSubtype = msg.getField("C2CX-SUBTYPE");
		}
		else
		{
			c2cxSubtype = msg.getField("MESSAGE-SUBTYPE");
		}

		if (c2cxSubtype != NULL && StringUtil::stringEqualsIgnoreCase(c2cxSubtype->getStringValue(), "HB"))
		{
			if ((addTracking || connTracking.getActiveSubscriptions() == ON || msgTracking.getActiveSubscriptions() == ON) &&
			    (connTracking.getActiveSubscriptions() != OFF && msgTracking.getActiveSubscriptions() != OFF))
			{
				if (msg.hasField("NUM-OF-SUBSCRIPTIONS"))
				{
					GMSEC_I64 numSubscriptions = msg.getI64Value("NUM-OF-SUBSCRIPTIONS");

					for (GMSEC_I64 n = 1; n <= numSubscriptions; ++n)
					{
						std::ostringstream fieldName;
						fieldName << "SUBSCRIPTION." << n << ".SUBJECT-PATTERN";

						msg.clearField(fieldName.str().c_str());
					}

					msg.clearField("NUM-OF-SUBSCRIPTIONS");
				}
			}

			if ((addTracking || connTracking.getConnectionEndpoint() == ON || msgTracking.getConnectionEndpoint() == ON) &&
		   	    (connTracking.getConnectionEndpoint() != OFF && msgTracking.getConnectionEndpoint() != OFF))
			{
				if (getSpecVersion() == GMSEC_MSG_SPEC_2018_00 && getSpecSchemaLevel() == Specification::SchemaLevel::LEVEL_0)
				{
					msg.clearField("CONNECTION-ENDPOINT");     // spelling used within 2018 C2MS-draft
				}
				else
				{
					msg.clearField("MW-CONNECTION-ENDPOINT");  // official spelling used within 2019 C2MS
				}
			}
		}
	}
}


void InternalConnection::resolveRequestTimeout(GMSEC_I32& timeout_ms)
{
	if (timeout_ms < 0)
	{
		timeout_ms = GMSEC_WAIT_FOREVER;
	}
	else if (timeout_ms < GMSEC_MIN_TIMEOUT_ms)
	{
		timeout_ms = GMSEC_MIN_TIMEOUT_ms;
	}
}


void InternalConnection::resolveRepublishInterval(GMSEC_I32& republish_ms)
{
	if (republish_ms == 0)
	{
		republish_ms = m_defaultRepublish_ms;
	}
	else if (republish_ms < 0)
	{
		republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER;
	}
	else if (republish_ms > 0 && (republish_ms < GMSEC_MIN_REPUBLISH_ms))
	{
		republish_ms = GMSEC_MIN_REPUBLISH_ms;
	}
}


void InternalConnection::startRequestThread()
{
	if (!m_requestShared.get())
	{
		m_requestShared.reset(new RequestShared(this));
		m_requestThread.reset(new StdThread(&runRequestThread, m_requestShared));

		const int timeout_ms = 10000;
		Condition& condition = m_requestShared->getCondition();

		AutoMutex hold(condition.getMutex());

		m_requestThread->start();

		int reason = condition.wait(timeout_ms);

		if (reason != RequestShared::MANAGED)
		{
			GMSEC_WARNING << "[" << this << "] RequestThread is slow starting";
		}
	}
}


void InternalConnection::stopRequestThread()
{
	if (m_requestShared.get())
	{
		GMSEC_DEBUG << "[" << this << "] is stopping the Request Thread";

		m_requestShared->shutdown();
		m_requestShared.reset();
	}
}


void InternalConnection::startPerfLoggerThread()
{
	if (!m_requestShared.get())
	{
		m_perfLoggerShared.reset(new PerformanceLogger(m_perfLoggerFilename.c_str()));
		m_perfLoggerThread.reset(new StdThread(&runPerformanceLoggerThread, m_perfLoggerShared));

		GMSEC_DEBUG << "[" << this << "] is starting the Performance Logger Thread";

		m_perfLoggerThread->start();
	}
}


void InternalConnection::stopPerfLoggerThread()
{
	if (m_perfLoggerShared.get())
	{
		GMSEC_DEBUG << "[" << this << "] is stopping the Performance Logger Thread";

		m_perfLoggerShared->shutdown();
		m_perfLoggerShared.reset();
	}
}


void InternalConnection::startMsgAggregationToolkitThread()
{
	if (!m_msgAggregationToolkitThread.get())
	{
		m_msgAggregationToolkitThread.reset(new StdThread(&MessageAggregationToolkit::runThread, m_msgAggregationToolkitShared));

		GMSEC_DEBUG << "[" << this << "] is starting the Message Aggregation Toolkit Thread";

		const int timeout_ms = 10000;
		Condition& condition = m_msgAggregationToolkitShared->getCondition();

		AutoMutex hold(condition.getMutex());

		m_msgAggregationToolkitThread->start();

		int reason = condition.wait(timeout_ms);

		if (reason != MessageAggregationToolkit::MANAGED)
		{
			GMSEC_WARNING << "[" << this << "] MsgAggregationToolkitThread is slow starting";
		}
	}
}


void InternalConnection::stopMsgAggregationToolkitThread()
{
	if (m_msgAggregationToolkitThread.get())
	{
		GMSEC_DEBUG << "[" << this << "] is stopping the Message Aggregation Toolkit Thread";

		m_msgAggregationToolkitShared->shutdown();

		m_msgAggregationToolkitShared.reset();
	}
}


GMSEC_U32 InternalConnection::getNextMessageCounter()
{
	AutoTicket hold(getCounterMutex());

	return ++m_messageCounter;
}


gmsec::api5::internal::TicketMutex& InternalConnection::getReadMutex()
{
	return m_readMutex;
}


gmsec::api5::internal::TicketMutex& InternalConnection::getWriteMutex()
{
	return m_writeMutex;
}


gmsec::api5::internal::TicketMutex& InternalConnection::getEventMutex()
{
	return m_eventMutex;
}


gmsec::api5::internal::TicketMutex& InternalConnection::getCounterMutex()
{
	return m_counterMutex;
}


void InternalConnection::setConnectionEndpoint(const std::string& endpoint)
{
	m_connectionEndpoint = endpoint;
}


unsigned int InternalConnection::getSpecVersion() const
{
    return m_msgFactory->getSpecification().getVersion();
}


Specification::SchemaLevel InternalConnection::getSpecSchemaLevel() const
{
    return m_msgFactory->getSpecification().getSchemaLevel();
}


int InternalConnection::getMaxConnectionRetries() const
{
	return m_maxConnRetries;
}

int InternalConnection::getConnectionRetryInterval() const
{
	return m_connRetryInterval;
}
