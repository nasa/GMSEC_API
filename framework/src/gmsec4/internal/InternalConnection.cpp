/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalConnection.cpp
 */

// Disable the Visual Studio warning C4355
//   ('this' : used in base member initializer list)
#ifdef WIN32
#pragma warning(disable: 4355)
#endif

#include <gmsec4/internal/InternalConnection.h>

#include <gmsec4/internal/AsyncPublisher.h>
#include <gmsec4/internal/CallbackLookup.h>
#include <gmsec4/internal/ConnectionInterface.h>
#include <gmsec4/internal/DynamicFactory.h>
#include <gmsec4/internal/ExclusionFilter.h>
#include <gmsec4/internal/InternalAutoDispatcher.h>
#include <gmsec4/internal/InternalStatus.h>
#include <gmsec4/internal/MessageAggregationToolkit.h>
#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/Middleware.h>
#include <gmsec4/internal/RequestThread.h>
#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/SubscriptionDetails.h>
#include <gmsec4/internal/SystemUtil.h>
#include <gmsec4/internal/Value.h>

#include <gmsec4/internal/performance/PerformanceLogger.h>

#include <gmsec4/Callback.h>
#include <gmsec4/Config.h>
#include <gmsec4/Connection.h>
#include <gmsec4/EventCallback.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>
#include <gmsec4/Message.h>
#include <gmsec4/ReplyCallback.h>
#include <gmsec4/SubscriptionInfo.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/TimeUtil.h>

#include <gmsec_version.h>

#include <memory>
#include <sstream>


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;


// Connection instance counter
static GMSEC_U32 s_instanceCount = 0;


const char* InternalConnection::getAPIVersion()
{
	return GMSEC_VERSION;
}


InternalConnection::InternalConnection(const Config& config, ConnectionInterface* connIf)
	: m_config(config),
	  m_connIf(connIf),
	  m_requestSpecs(),
	  m_tracking(),
	  m_connectionID(0),
	  m_messageCounter(0),
	  m_state(Connection::NOT_CONNECTED),
	  m_connectionAlive(false),
	  m_connName(),
	  m_connID(),
	  m_userName(),
	  m_mwInfo(),
	  m_autoDispatcher(this, m_readMutex, m_writeMutex),
	  m_defaultRepublish_ms(DEFAULT_REPUBLISH_ms),
	  m_requestPublish_ms(REQUEST_PUBLISH_ms),
	  m_subscriptions(),
	  m_subscriptionRegistry(),
	  m_exclusionFilter(new ExclusionFilter()),
	  m_callbackLookup(new CallbackLookup()),
	  m_eventCallbacks(),
	  m_usePerfLogger(false),
	  m_perfLoggerFilename(),
	  m_useGlobalAsyncPublish(false),
	  m_asyncQueueDepth(1000),
	  m_asyncTeardownWait(1000),
	  m_asyncQueue(0),
	  m_callbackAdapter(0),
	  m_parent(0),
	  m_usingAPI3x(false)
{
	// Establish connection ID
	m_connectionID = ++s_instanceCount;


	// Generate Connection ID
	int pid = SystemUtil::getProcessID();

	std::string user;
	int code = SystemUtil::getUserName(user);
	if (code || user.empty())
	{
		GMSEC_WARNING << "Error acquiring user name [code = " << code << "]";
	}

	std::string host;
	code = SystemUtil::getHostName(host);
	if (code || host.empty())
	{
		GMSEC_WARNING << "Error acquiring host name [code = " << code << "]";
		host = "unknown";
	}

	std::ostringstream oss;
	oss << "GMSEC_" << StringUtil::stringToUpper(host) << "_";
	{
		// apply timestamp <seconds:x><subseconds:04x>
		double tmp    = TimeUtil::getCurrentTime_s();
		unsigned sec  = unsigned(tmp);
		unsigned subs = unsigned((tmp - sec) * 65536);
		char buffer[32];

		StringUtil::stringFormat(buffer, sizeof(buffer), "%X%04X", sec, subs);
		oss << buffer;
	}
	oss << '_' << pid << '_' << m_connectionID;

	m_connID   = oss.str();
	m_userName = user;


	// Set up policy
	Status status = DynamicFactory::initialize(getPolicy(), m_config);

	if (status.isError())
	{
		GMSEC_WARNING << status.get();
	}

	initializeTracking();
	initializeRequest();


	// Check if performance logger is being enabled
	const char* perfLoggerFilename = m_config.getValue(OPT_LOG_PERFORMANCE);

	if (perfLoggerFilename)
	{
		m_usePerfLogger      = true;
		m_perfLoggerFilename = perfLoggerFilename;
	}


	// Check if asynchronous publishing is being enabled
	m_useGlobalAsyncPublish        = m_config.getBooleanValue(GMSEC_ASYNC_PUBLISH, false);
	const char* asyncQueueDepthCfg = m_config.getValue(GMSEC_ASYNC_PUBLISH_QUEUE_DEPTH);
	const char* asyncTeardownCfg   = m_config.getValue(GMSEC_ASYNC_PUBLISH_TEARDOWN_WAIT);

	if (asyncQueueDepthCfg)
	{
		GMSEC_I32 depth;

		if (!StringUtil::stringParseI32(asyncQueueDepthCfg, depth))
		{
			GMSEC_WARNING << "Invalid value for " << GMSEC_ASYNC_PUBLISH_QUEUE_DEPTH
				<< ", defaulting to 1000. Value given is: " << asyncQueueDepthCfg;

			depth = 1000;
		}

		m_asyncQueueDepth = depth;
	}

	if (asyncTeardownCfg)
	{
		GMSEC_I32 waitTime;

		if (!StringUtil::stringParseI32(asyncTeardownCfg, waitTime))
		{
			GMSEC_WARNING << "Invalid value for " << GMSEC_ASYNC_PUBLISH_TEARDOWN_WAIT
				<< ", defaulting to 1000. Value given is: " << asyncTeardownCfg;

			waitTime = 1000;
		}

		m_asyncTeardownWait = waitTime;
	}

	// Client may elect to use asynchronous publishing via the configuration passed to
	// publish().  For such reason, we instantiate the message queue now.
	m_asyncQueue = new BoundedQueue<MessagePublishTask>(m_asyncQueueDepth);


	// Check if request/reply functionality is being disabled
	const char* tmpValue = config.getValue(GMSEC_DISABLE_RR);
	if (!tmpValue)
	{
		m_requestSpecs.requestReplyEnabled = true;
	}
	else
	{
		m_requestSpecs.requestReplyEnabled = StringUtil::stringEqualsIgnoreCase(tmpValue, "false");
	}


	m_msgAggregationToolkitShared.reset(new MessageAggregationToolkit(this));

	// Check if this Connection object will perform Message aggregation
	if (config.getBooleanValue(GMSEC_USE_MSG_BINS, false))
	{
		startMsgAggregationToolkitThread();
	}


	// Instantiate C-binding callback adapter factory.
	m_callbackAdapter = new CallbackAdapter();

	m_connIf->setExternal(this);

	m_connectionAlive.set(true);
}


InternalConnection::~InternalConnection()
{
	m_connectionAlive.set(false);

	// Remove any lingering subscriptions
	for (SubscriptionInfoList::iterator it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it)
	{
		delete *it;
	}
	m_subscriptions.clear();

	for (Subscriptions::iterator it = m_subscriptionRegistry.begin(); it != m_subscriptionRegistry.end(); ++it)
	{
		delete it->second;
	}
	m_subscriptionRegistry.clear();

	if (m_state == Connection::CONNECTED)
	{
		disconnect();
	}

	delete m_callbackAdapter;
	delete m_callbackLookup;
	delete m_exclusionFilter;
	delete m_asyncQueue;

	delete m_connIf;
}


void InternalConnection::connect()
{
	if (m_state == Connection::CONNECTED)
	{
		GMSEC_WARNING << "A connection has already been established!";
		throw Exception(CONNECTION_ERROR, CONNECTION_CONNECTED, "A connection has already been established!");
	}

	m_connIf->mwConnect();

	m_state = Connection::CONNECTED;

	Status status = getPolicy().authenticate(*m_parent);

	if (status.isError())
	{
		m_connIf->mwDisconnect();
		throw Exception(status);
	}

	if (m_usePerfLogger)
	{
		startPerfLoggerThread();
	}

	if (m_subscriptionRegistry.size() > 0)
	{
		for (Subscriptions::iterator it = m_subscriptionRegistry.begin(); it != m_subscriptionRegistry.end(); ++it)
		{
			SubscriptionDetails* subInfo = it->second;

			subInfo->resetIter();

			while (subInfo->hasNext())
			{
				const Details* details = subInfo->next();

				m_connIf->mwSubscribe(it->first.c_str(), details->config);
			}
		}
	}
}


void InternalConnection::disconnect()
{
	if (m_state == Connection::CONNECTED)
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

		(void) m_connIf->mwDisconnect();

		if (m_usePerfLogger)
		{
			stopPerfLoggerThread();
		}

		m_state = Connection::NOT_CONNECTED;
	}
}


gmsec::api::Connection::ConnectionState InternalConnection::getState() const
{
	return m_state;
}


const char* InternalConnection::getLibraryRootName() const
{
	return m_connIf->getLibraryRootName();
}


const char* InternalConnection::getLibraryVersion() const
{
	return m_connIf->getLibraryVersion();
}


void InternalConnection::registerEventCallback(Connection::ConnectionEvent event, EventCallback* cb)
{
	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
	}

	if (!cb)
	{
		throw Exception(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "EventCallback cannot be NULL");
	}

	AutoTicket holdEvent(getEventMutex());

	m_eventCallbacks[event] = cb;
}


SubscriptionInfo* InternalConnection::subscribe(const char* subject, Callback* cb)
{
	Config config;  // define empty config

	return subscribe(subject, config, cb);
}


SubscriptionInfo* InternalConnection::subscribe(const char* subject, const Config& config, Callback* cb)
{
	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
	}

	if (subject == NULL || std::string(subject).empty())
	{
		throw Exception(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Subject is NULL or is empty string");
	}

	if (!getPolicy().isValidSubscription(subject))
	{
		throw Exception(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Subject is invalid");
	}

	AutoTicket holdRead(getReadMutex());

	// We only allow for only one registration of a subscription topic when a
	// callback is not provided.
	//
	// When a callback is provided, it must not be the same that is already
	// in use for a particular subscription topic (i.e. the callback must be
	// unique).
	try
	{
		if (checkExistingSubscription(subject, cb))
		{
			// New subscription
			m_connIf->mwSubscribe(subject, config);
		}
	}
	catch (Exception& e)
	{
#ifdef GMSEC_USING_VS2008
		holdRead.leave();
#endif

		throw e;
	}

	if (cb)
	{
		m_callbackLookup->addCallback(subject, cb);
	}

	Subscriptions::iterator it = m_subscriptionRegistry.find(subject);

	if (it == m_subscriptionRegistry.end())
	{
		// new subscription
		SubscriptionDetails* details = new SubscriptionDetails();
		details->addDetails(cb, config);
		m_subscriptionRegistry[subject] = details;
	}
	else
	{
		it->second->addDetails(cb, config);
	}

	GMSEC_DEBUG << "Successfully subscribed to " << subject;

	SubscriptionInfo* info = new SubscriptionInfo(m_parent, subject, cb);

	m_subscriptions.push_front(info);

	return info;
}


void InternalConnection::unsubscribe(SubscriptionInfo*& info)
{
	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
	}

	if (!info)
	{
		throw Exception(CONNECTION_ERROR, UNINITIALIZED_OBJECT,
		                "Cannot unsubscribe using NULL SubscriptionInfo object.");
	}

	if (m_parent != info->getConnection())
	{
		throw Exception(CONNECTION_ERROR, OTHER_ERROR_CODE,
		                "SubscriptionInfo object is not associated with this Connection");
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

		throw Exception(CONNECTION_ERROR, OTHER_ERROR_CODE,
		                "The given SubscriptionInfo object is not registered with this Connection");
	}
	m_subscriptions.erase(it);

	try
	{
		unsubscribeAux(info);

		delete info;
		info = 0;
	}
	catch (const Exception& e)
	{
#ifdef GMSEC_USING_VS2008
		holdRead.leave();
#endif
		throw e;
	}
}


bool InternalConnection::startAutoDispatch()
{
	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
	}

	return m_autoDispatcher.start();
}


bool InternalConnection::stopAutoDispatch(bool waitForCompletion)
{
	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
	}

	return m_autoDispatcher.stop(waitForCompletion);
}


void InternalConnection::publish(const Message& msg)
{
	Config emptyCfg;
	publish(msg, emptyCfg);
}


void InternalConnection::publish(const Message& msg, const Config& config)
{
	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
	}

	if (msg.getKind() != Message::PUBLISH)
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Cannot publish non-PUBLISH kind message.");
	}

	if (!getPolicy().isValidSubject(msg.getSubject()))
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Message has invalid subject.");
	}

	insertTrackingFields(const_cast<Message&>(msg));

	bool msgWasAggregated = false;

	if (m_msgAggregationToolkitThread.get())
	{
		// Check if an error has previously occurred with an attempt to publish an aggregated message.
		Status result = m_msgAggregationToolkitShared->getPublishStatus();

		if (result.isError())
		{
			throw Exception(result);
		}

		msgWasAggregated = m_msgAggregationToolkitShared->addMessage(msg, config);
	}

	if (!msgWasAggregated)
	{
		if (m_useGlobalAsyncPublish || config.getBooleanValue(GMSEC_ASYNC_PUBLISH, false))
		{
			if (!m_asyncPubService.get())
			{
				m_asyncPubService.reset(new AsyncPublisher(m_connIf, m_asyncQueue));
				m_asyncPubThread.reset(new StdThread(&AsyncPublisher::start, m_asyncPubService));
				m_asyncPubThread->start();
			}

			MessagePublishTask mpt;
			mpt.msg    = new Message(msg);
			mpt.config = new Config(config);

			m_asyncQueue->put(mpt);
		}
		else
		{
			m_connIf->mwPublish(msg, config);
		}
	}

	removeTrackingFields(const_cast<Message&>(msg));
}


void InternalConnection::request(const Message& request, GMSEC_I32 timeout, ReplyCallback* cb, GMSEC_I32 republish_ms)
{
	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
	}

	if (request.getKind() != Message::REQUEST)
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Cannot issue request with non-REQUEST kind message.");
	}

	if (!cb)
	{
		throw Exception(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "ReplyCallback cannot be NULL");
	}

	if (!m_requestSpecs.requestReplyEnabled)
	{
		throw Exception(CONNECTION_ERROR, FEATURE_NOT_SUPPORTED,
			"Request/Reply functionality has been disabled for this Connection");
	}

	if (!getPolicy().isValidSubject(request.getSubject()))
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Message has invalid subject");
	}

	// We make a clone of the user's request message so that the API can manage the lifeline of such.
	// The user is responsible for managing the lifeline of their request message.
	Message* requestClone = new Message(request);

	startRequestThread();

	resolveRequestTimeout(timeout);
	resolveRepublishInterval(republish_ms);

	if (m_requestSpecs.useSubjectMapping == false)
	{
		MessageBuddy::getInternal(*requestClone).getDetails().setBoolean(OPT_REQ_RESP, true);
	}

	insertTrackingFields(*requestClone);

	StdSharedPtr<PendingRequest> pending(new PendingRequest(requestClone, timeout, republish_ms));

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
	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
	}

	if (request.getKind() != Message::REQUEST)
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Cannot issue request with non-REQUEST kind message.");
	}

	if (!m_requestSpecs.requestReplyEnabled)
	{
		throw Exception(CONNECTION_ERROR, FEATURE_NOT_SUPPORTED,
			"Request/Reply functionality has been disabled for this Connection");
	}

	if (!getPolicy().isValidSubject(request.getSubject()))
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Message has invalid subject");
	}

	Message* reply = NULL;

	// We make a clone of the user's request message so that the API can manage the lifeline of such.
	// The user is responsible for managing the lifeline of their request message.
	Message* requestClone = new Message(request);

	startRequestThread();

	resolveRequestTimeout(timeout);
	resolveRepublishInterval(republish_ms);

	if (m_requestSpecs.useSubjectMapping == false)
	{
		MessageBuddy::getInternal(*requestClone).getDetails().setBoolean(OPT_REQ_RESP, true);
	}

	insertTrackingFields(*requestClone);

	StdSharedPtr<PendingRequest> pending(new PendingRequest(requestClone, timeout, republish_ms));

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

		while (!result.isError())
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
				GMSEC_INFO << result.get();
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

					// Kludge to pass the request message to the API 3.x interface.
					// Remove this code when API 3.x has been deprecated.
					StatusBuddy::getInternal(localResult).setRequestMessage(*pending->request);

					// Check if a request timeout callback is associated with the connection.
					dispatchEvent(Connection::REQUEST_TIMEOUT_EVENT, localResult);

					// API 3.x expected immediate return on timeout if the republish_ms period is 0ms.
					// This of course is a bug, but we will allow it to persevere.
					if ((m_usingAPI3x && republish_ms == DEFAULT_REPUBLISH_ms) || (republish_ms == REPUBLISH_NEVER))
					{
						result = localResult;
					}

					pending->expireTime_s += (republish_ms / 1000.0);
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


void InternalConnection::cancelRequest(ReplyCallback* cb)
{
	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
	}

	if (!cb)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CALLBACK, "ReplyCallback cannot be NULL");
	}

	GMSEC_WARNING << "InternalConnection::cancelRequest() is currently not implemented.";
}


void InternalConnection::reply(const Message& request, const Message& reply)
{
	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
	}

	if (request.getKind() != Message::REQUEST)
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Cannot issue reply with non-REQUEST kind message.");
	}

	if (reply.getKind() != Message::REPLY)
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Cannot issue reply with non-REPLY kind message.");
	}

	if (!m_requestSpecs.requestReplyEnabled)
	{
		throw Exception(CONNECTION_ERROR, FEATURE_NOT_SUPPORTED,
			"Request/Reply functionality has been disabled for this Connection");
	}

	if (!getPolicy().isValidSubject(reply.getSubject()))
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Message has invalid subject");
	}

	ValueMap& meta = MessageBuddy::getInternal(request).getDetails();

	std::string id;
	if (!meta.getString(REPLY_UNIQUE_ID_FIELD, id).isError())
	{
		MessageBuddy::getInternal(reply).addField(REPLY_UNIQUE_ID_FIELD, id.c_str());
	}

	bool noSubjectMapping = false;
	meta.getBoolean(OPT_REQ_RESP, noSubjectMapping, &noSubjectMapping);

	insertTrackingFields(const_cast<Message&>(reply));

	if (noSubjectMapping)
	{
		// We do NOT include the Reply Subject within a message field.
		// And we publish the reply message instead of using the traditional
		// middleware reply method.
		Config config = Config();
		m_connIf->mwPublish(reply, config);
	}
	else
	{
		// Add the reply subject as a field of the message being sent.
		MessageBuddy::getInternal(reply).addField(REPLY_SUBJECT_FIELD, reply.getSubject());

		m_connIf->mwReply(request, reply);

		MessageBuddy::getInternal(reply).clearField(REPLY_SUBJECT_FIELD);
	}

	MessageBuddy::getInternal(reply).clearField(REPLY_UNIQUE_ID_FIELD);

	removeTrackingFields(const_cast<Message&>(reply));
}


Message* InternalConnection::receive(GMSEC_I32 timeout)
{
	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
	}

	if (m_autoDispatcher.isRunning())
	{
		throw Exception(CONNECTION_ERROR, AUTODISPATCH_EXCLUSIVE,
			"This method cannot be called when the Auto-Dispatcher is in use");
	}

	AutoTicket hold(getReadMutex());

	Message* msg = 0;

	getNextMsg(msg, timeout);

	return msg;
}


void InternalConnection::dispatch(const Message& msg)
{
	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
	}

	if (m_autoDispatcher.isRunning())
	{
		throw Exception(CONNECTION_ERROR, AUTODISPATCH_EXCLUSIVE,
			"This method cannot be called when the Auto-Dispatcher is in use");
	}

	AutoTicket hold(getWriteMutex());

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


void InternalConnection::release(Message*& msg)
{
	delete msg;
	msg = 0;
}


void InternalConnection::excludeSubject(const char* subject)
{
	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
	}

	if (subject == NULL || std::string(subject).empty())
	{
		throw Exception(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Subject is NULL or is empty-string");
	}

	if (!getPolicy().isValidSubject(subject))
	{
		throw Exception(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Subject is invalid");
	}

	m_exclusionFilter->excludeSubject(subject);
}


void InternalConnection::removeExcludedSubject(const char* subject)
{
	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
	}

	if (subject == NULL || std::string(subject).empty())
	{
		throw Exception(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Subject is NULL or is empty-string");
	}

	if (!getPolicy().isValidSubject(subject))
	{
		throw Exception(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Subject is invalid");
	}

	m_exclusionFilter->removeExcludedSubject(subject);
}


const char* InternalConnection::getName() const
{
	return m_connName.c_str();
}


void InternalConnection::setName(const char* name)
{
	if (!name)
	{
		throw Exception(CONNECTION_ERROR, OTHER_ERROR_CODE, "Connection name cannot be NULL");
	}

	m_connName = name;
}


const char* InternalConnection::getID() const
{
	return m_connID.c_str();
}


const char* InternalConnection::getMWInfo() const
{
	if (m_connIf && m_state == Connection::CONNECTED)
	{
		m_mwInfo = m_connIf->getMWInfo();
	}

	if (m_mwInfo.empty())
	{
		m_mwInfo = "<not available>";
	}

	return m_mwInfo.c_str();
}


GMSEC_U64 InternalConnection::getPublishQueueMessageCount() const
{
	return (m_asyncQueue ? (GMSEC_U64) m_asyncQueue->queuedElements() : 0);
}


void InternalConnection::shutdownAllMiddlewares()
{
	gmsec::api::internal::Middleware::shutdownAll();
}


void InternalConnection::shutdownMiddleware(const char* name)
{
	gmsec::api::internal::Middleware::shutdown(name);
}


void InternalConnection::registerParent(gmsec::api::Connection* parent)
{
	m_parent = parent;
}


void InternalConnection::registerEventCallback(Connection::ConnectionEvent event, GMSEC_EventCallback* ecb)
{
	EventCallback* callback = m_callbackAdapter->createEventCallback(ecb);

	registerEventCallback(event, callback);
}


SubscriptionInfo* InternalConnection::subscribe(const char* subject, const Config& cfg, GMSEC_Callback* cb)
{
	Callback* callback = m_callbackAdapter->createCallback(cb);

	SubscriptionInfo* info = subscribe(subject, cfg, callback);

	info->setCallbackFunction(reinterpret_cast<void*>(cb));

	return info;
}


void InternalConnection::request(const Message& req, GMSEC_I32 timeout, GMSEC_ReplyCallback* rcb, GMSEC_EventCallback* ecb, GMSEC_I32 republish_ms)
{
	ReplyCallback* callback = m_callbackAdapter->createReplyCallback(rcb, ecb);

	request(req, timeout, callback, republish_ms);
}


void InternalConnection::cancelRequest(GMSEC_ReplyCallback* rcb)
{
	ReplyCallback* callback = m_callbackAdapter->lookupReplyCallback(rcb);

	cancelRequest(callback);

	m_callbackAdapter->deleteReplyCallback(rcb);
}


Config& InternalConnection::getConfig()
{
	return m_config;
}


GMSEC_U32 InternalConnection::getMessageCounter() const
{
	return m_messageCounter;
}


void InternalConnection::dispatchEvent(Connection::ConnectionEvent event, const Status& status)
{
	AutoTicket lock(getEventMutex());

	EventCallbackMap::iterator it = m_eventCallbacks.find(event);

	if (it == m_eventCallbacks.end())
	{
		it = m_eventCallbacks.find(Connection::ALL_EVENTS);
	}

	if (it != m_eventCallbacks.end() && it->second != NULL)
	{
		it->second->onEvent(*m_parent, status, event);
	}
}


gmsec::api::secure::Policy& InternalConnection::getPolicy()
{
	return m_policy;
}


RequestSpecs InternalConnection::getRequestSpecs() const
{
	return m_requestSpecs;
}


bool InternalConnection::onReply(Message* reply)
{
	bool replyDelivered = true;

	StdSharedPtr<RequestShared> tmp(m_requestShared);

	if (tmp.get())
	{
		tmp->onReply(reply);
	}
	else
	{
		// We should never end up here, but just in case, destroy the reply message.
		delete reply;

		replyDelivered = false;

		GMSEC_WARNING << "InternalConnection[" << this << "] RequestShared is NOT running";
	}

	return replyDelivered;
}


void InternalConnection::replyEvent(ReplyCallback* rcb, const Status& status, Connection::ConnectionEvent event)
{
	AutoTicket hold(getEventMutex());

	if (rcb)
	{
		rcb->onEvent(*m_parent, status, event);
	}
}


void InternalConnection::replyCallback(ReplyCallback* rcb, const Message& request, const Message& reply)
{
	if (rcb)
	{
		rcb->onReply(*m_parent, request, reply);
	}
}


void InternalConnection::issueRequestToMW(const Message& request, std::string& id)
{
	Message requestCopy(request);

	insertTrackingFields(requestCopy);

	m_connIf->mwRequest(requestCopy, id);
}


void InternalConnection::updateReplySubject(Message*& reply, bool warn)
{
	// NOTE:  When performing a Request/Reply scenario, the API modifies the subject of
	// -----  the reply message such that it equals the UNIQUE-ID.  This is done so
	// that the middleware knows where to route the message.  Therefore in order to
	// preserve the original reply subject, we need to extract it from within the message
	// where the API puts it to preserve its value.  Once it is extracted it needs to be
	// placed back into the subject of the message.

	const char* problem = 0;

    // Get the REPLY_SUBJECT_FIELD from within the Reply message.
    const Field* field = reply->getField(REPLY_SUBJECT_FIELD);

    if (!field)
    {
        problem = "missing reply subject field";
    }
	else
	{
		const StringField* replySubjectField = dynamic_cast<const StringField*>(field);

		if (replySubjectField)
		{
			const char* replySubject = replySubjectField->getValue();

			MessageBuddy::getInternal(*reply).setSubject(replySubject);

			reply->clearField(REPLY_SUBJECT_FIELD);
		}
		else
		{
        	problem = "reply subject field is not a StringField";
		}
	}

	if (problem && warn)
	{
		GMSEC_WARNING << "updateReplySubject: " << problem;
	}
}


const TrackingDetails& InternalConnection::getTracking()
{
	return m_tracking;
}


void InternalConnection::autoDispatch()
{
	if (!m_autoDispatcher.isRunning())
	{
		throw Exception(OTHER_ERROR, OTHER_ERROR_CODE, "Auto-dispatcher is not running");
	}

	// lock the InternalConnection
	AutoTicket holdRead(getReadMutex());

	static const int AUTO_DISPATCH_ms = 250;

	// Get the next message
	Message* msg = 0;

	getNextMsg(msg, AUTO_DISPATCH_ms);

	if (msg)
	{
		std::list<Callback*> callbacks;

		m_callbackLookup->collectCallbacks(msg->getSubject(), callbacks);

		dispatchMsgToCallbacks(*msg, callbacks);

		delete msg;
	}
}


void InternalConnection::getNextMsg(Message*& msg, GMSEC_I32 timeout)
{
	msg = NULL;

	if (m_state != Connection::CONNECTED)
	{
		throw Exception(CONNECTION_ERROR, INVALID_CONNECTION, "Connection has not been established");
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
		else
		{
			if (msg != NULL && m_msgAggregationToolkitShared->isAggregatedMsg(msg))
			{
				m_msgAggregationToolkitShared->processAggregatedMsg(msg);

				// If there are messages (and there should be), always return the first one.
				if (m_msgAggregationToolkitShared->hasNextMsg())
				{
					msg = m_msgAggregationToolkitShared->nextMsg();
				}
			}

			done = true;
		}
	}

	if (msg && m_usePerfLogger)
	{
		const StringField* pubTime = dynamic_cast<const StringField*>(msg->getField("PUBLISH-TIME"));

		if (pubTime)
		{
			m_perfLoggerShared->dispatchLog(msg->getSubject(), pubTime->getValue());
		}
	}
}


void InternalConnection::dispatchMsgToCallbacks(const Message& msg, std::list<Callback*>& callbacks)
{
	for (std::list<Callback*>::iterator i = callbacks.begin(); m_connectionAlive.get() && i != callbacks.end(); ++i)
	{
		Callback* callback = *i;

		callback->onMessage(*m_parent, msg);
	}
}


bool InternalConnection::checkExistingSubscription(const char* subject, Callback* cb) const
{
	bool newSubscription = true;

	for (SubscriptionInfoList::const_iterator it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it)
	{
		const SubscriptionInfo* info = *it;

		if (StringUtil::stringEqualsIgnoreCase(subject, info->getSubject()))
		{
			newSubscription = false;

			if (cb == NULL)
			{
				throw Exception(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Duplicate subscription");
			}
			else if (cb == info->getCallback())
			{
				throw Exception(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Duplicate subscription using same callback");
			}
		}
	}

	return newSubscription;
}


void InternalConnection::unsubscribeAux(SubscriptionInfo*& info)
{
	Subscriptions::iterator it = m_subscriptionRegistry.find(info->getSubject());

	if (it == m_subscriptionRegistry.end())
	{
		throw Exception(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Not subscribed to subject pattern");
	}

	if (info->getCallback() == NULL)
	{
		// Since no callback, we unsubscribe completely,
		// including any callbacks that may have previously
		// been associated with the subject.
		m_connIf->mwUnsubscribe(info->getSubject());

		SubscriptionDetails* subInfo = it->second;

		subInfo->resetIter();

		while (subInfo->hasNext())
		{
			const Details* details = subInfo->next();

			if (details->callback)
			{
				(void) m_callbackLookup->removeCallback(info->getSubject(), details->callback);
			}
		}

		delete it->second;
		m_subscriptionRegistry.erase(it);
	}
	else
	{
		m_callbackLookup->removeCallback(info->getSubject(), info->getCallback());

		it->second->removeDetails(info->getCallback());

		// if we have no remaining subscriptions, then
		// notify middleware wrapper.
		if (it->second->numDetailsRegistered() == 0)
		{
			m_connIf->mwUnsubscribe(info->getSubject());

			delete it->second;

			m_subscriptionRegistry.erase(it);
		}
	}
}


void InternalConnection::initializeTracking()
{
	// boolean config values for tracking fields
	m_tracking.set(m_config.getBooleanValue("tracking", true));
	m_tracking.setNode(m_config.getBooleanValue("tracking-node", true));
	m_tracking.setProcessId(m_config.getBooleanValue("tracking-process-id", true));
	m_tracking.setUserName(m_config.getBooleanValue("tracking-user-name", true));
	m_tracking.setConnectionId(m_config.getBooleanValue("tracking-connection-id", true));
	m_tracking.setPublishTime(m_config.getBooleanValue("tracking-publish-time", true));
	m_tracking.setUniqueId(m_config.getBooleanValue("tracking-unique-id", true));
	m_tracking.setMwInfo(m_config.getBooleanValue("tracking-mw-info", true));
}


void InternalConnection::initializeRequest()
{
	const char* value = NULL;

	value = m_config.getValue(OPT_REQ_RESP);

	if (value && StringUtil::stringEqualsIgnoreCase(value, "OPEN-RESP"))
	{
		m_requestSpecs.useSubjectMapping = false;
		m_requestSpecs.legacy = false;
		m_requestSpecs.exposeReplies = true;
	}
	else
	{
		m_requestSpecs.useSubjectMapping = true;
	}

	// default reply subject is the connection ID
	std::string subject = getID();

	// allow override through the REPLY-STRING parameter
	// but only if we are using subject mapping.
	value = m_config.getValue(OPT_REPLY_STRING);

	if (value)
	{
		if (m_requestSpecs.useSubjectMapping)
		{
			bool ok = true;
			std::string tmp(value);

			// an asterix in the REPLY-STRING will be replaced by the connection ID
			size_t star = tmp.find('*');
			if (star != std::string::npos)
			{
				size_t next = tmp.find('*', star + 1);
				if (next != std::string::npos)
				{
					ok = false;
					GMSEC_WARNING << "determineReplySubject: invalid input " << tmp.c_str();
				}
			}

			if (star != std::string::npos)
			{
				tmp.replace(star, 1, getID());
			}

			if (ok && !getPolicy().isValidSubject(tmp.c_str()))
			{
				ok = false;
				GMSEC_WARNING << "determineReplySubject: invalid subject " << tmp.c_str();
			}

			if (ok)
			{
				m_requestSpecs.legacy = false;
				subject = tmp;
			}
		}
		else
		{
			GMSEC_WARNING << OPT_REQ_RESP << " and " << OPT_REPLY_STRING
				<< " cannot be specified at the same time; " << OPT_REPLY_STRING
				<< " is being ignored.";
		}
	}

	m_requestSpecs.replySubject = subject;
	GMSEC_VERBOSE << "using reply subject " << subject.c_str();

	value = m_config.getValue(OPT_EXPOSE_RESP);
	if (value)
	{
		m_requestSpecs.legacy = false;
		m_requestSpecs.exposeReplies = StringUtil::stringIsTrue(value);
	}

	m_requestSpecs.multiResponse = m_config.getBooleanValue(OPT_MULTI_RESP, false);

	value = m_config.getValue("republish_ms");
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


void InternalConnection::insertTrackingFields(Message& msg)
{
	// Check if tracking fields were already inserted (e.g. by API 3.x)
	bool trackingInserted = false;
	MessageBuddy::getInternal(msg).getDetails().getBoolean("TRACKING-FIELDS-INSERTED-BY-API3", trackingInserted, &trackingInserted);

	if (trackingInserted)
	{
		return;
	}

	AutoTicket hold(getWriteMutex());

	++m_messageCounter;

	const TrackingDetails& connTracking = getTracking();
	const TrackingDetails& msgTracking  = MessageBuddy::getInternal(msg).getTracking();

	const int ON    = MESSAGE_TRACKINGFIELDS_ON;
	const int OFF   = MESSAGE_TRACKINGFIELDS_OFF;
	const int UNSET = MESSAGE_TRACKINGFIELDS_UNSET;

	GMSEC_DEBUG << "CONN TRACKING:\n" << connTracking.toString();
	GMSEC_DEBUG << "MSG TRACKING:\n"  << msgTracking.toString();

	// Add the Tracking Fields
	if ((connTracking.get() != OFF && msgTracking.get() == UNSET) || msgTracking.get() == ON || m_usePerfLogger)
	{
		if ((connTracking.getNode() != OFF && msgTracking.getNode() == UNSET) || msgTracking.getNode() == ON)
		{
			std::string hostname;
			SystemUtil::getHostName(hostname);
			msg.addField("NODE", hostname.c_str());
		}

		if (((connTracking.getProcessId() != OFF && msgTracking.getProcessId() == UNSET) || msgTracking.getProcessId() == ON))
		{
			msg.addField("PROCESS-ID", (GMSEC_I16) SystemUtil::getProcessID());
		}

		if (((connTracking.getUserName() != OFF && msgTracking.getUserName() == UNSET) || msgTracking.getUserName() == ON))
		{
			msg.addField("USER-NAME", m_userName.c_str());
		}

		if (((connTracking.getConnectionId() != OFF && msgTracking.getConnectionId() == UNSET) || msgTracking.getConnectionId() == ON))
		{
			msg.addField("CONNECTION-ID", (GMSEC_U16) m_connectionID);
		}

		if (((connTracking.getPublishTime() != OFF && msgTracking.getPublishTime() == UNSET) || msgTracking.getPublishTime() == ON || m_usePerfLogger))
		{
			GMSEC_TimeSpec ts = TimeUtil::getCurrentTime();
			char           curTime[GMSEC_TIME_BUFSIZE];

			TimeUtil::formatTime(ts, curTime);

			msg.addField("PUBLISH-TIME", curTime);
		}

		if (((connTracking.getUniqueId() != OFF && msgTracking.getUniqueId() == UNSET) || msgTracking.getUniqueId() == ON))
		{
			std::ostringstream oss;
			oss << m_connID << "_" << m_messageCounter;

			msg.addField("UNIQUE-ID", oss.str().c_str());
		}

		if (((connTracking.getMwInfo() != OFF && msgTracking.getMwInfo() == UNSET) || msgTracking.getMwInfo() == ON))
		{
			msg.addField("MW-INFO", getMWInfo());
		}
	}
}


void InternalConnection::removeTrackingFields(Message& msg)
{
	// We do NOT remove tracking fields when using API 3.x
	bool trackingInserted = false;
	MessageBuddy::getInternal(msg).getDetails().getBoolean("TRACKING-FIELDS-INSERTED-BY-API3", trackingInserted, &trackingInserted);

	if (trackingInserted)
	{
		// Reset meta-details field so that the message will have new tracking fields inserted should the message be used again.
		MessageBuddy::getInternal(msg).getDetails().setBoolean("TRACKING-FIELDS-INSERTED-BY-API3", false);
	}
	else
	{
		msg.clearField("NODE");
		msg.clearField("PROCESS-ID");
		msg.clearField("USER-NAME");
		msg.clearField("CONNECTION-ID");
		msg.clearField("PUBLISH-TIME");
		msg.clearField("UNIQUE-ID");
		msg.clearField("MW-INFO");
	}
}


void InternalConnection::resolveRequestTimeout(GMSEC_I32& timeout_ms)
{
	if (timeout_ms < 0)
	{
		timeout_ms = GMSEC_WAIT_FOREVER;
	}
	else if (timeout_ms < MIN_TIMEOUT_ms)
	{
		timeout_ms = MIN_TIMEOUT_ms;
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
		republish_ms = REPUBLISH_NEVER;
	}
	else if (republish_ms > 0 && (republish_ms < MIN_REPUBLISH_ms))
	{
		republish_ms = MIN_REPUBLISH_ms;
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
		m_perfLoggerShared.reset(new performance::PerformanceLogger(m_perfLoggerFilename.c_str()));
		m_perfLoggerThread.reset(new StdThread(&performance::runPerformanceLoggerThread, m_perfLoggerShared));

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
		m_msgAggregationToolkitShared->configure(m_config);

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


gmsec::api::util::TicketMutex& InternalConnection::getReadMutex()
{
	return m_readMutex;
}


gmsec::api::util::TicketMutex& InternalConnection::getWriteMutex()
{
	return m_writeMutex;
}


gmsec::api::util::TicketMutex& InternalConnection::getEventMutex()
{
	return m_eventMutex;
}


void InternalConnection::usingAPI3x()
{
	m_usingAPI3x = true;
}
