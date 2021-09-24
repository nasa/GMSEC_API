/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file WebSConnection.cpp
 *
 *  @author Matt Handy
 *  @date November 16, 2009
 *
 *  @brief This file contains the implementation for the WebSphere MQ Connection.
 *
 */

#include "WebSConnection.h"

#include <gmsec4/ConfigOptions.h>

#include <gmsec4/internal/ConnectionInterface.h>
#include <gmsec4/internal/InternalConnection.h>
#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/SystemUtil.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/StdUniquePtr.h>

#include <sstream>


using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::internal;


namespace gmsec_websphere
{
static int DEFAULT_ASYNC_STATUS_MESSAGE_INTERVAL = 100;

static int DEFAULT_MAX_CONN_RETRIES    = 1000;
static int DEFAULT_CONN_RETRY_INTERVAL = 5000;  // 5 seconds
static int DEFAULT_MESSAGE_WAIT_PERIOD = 2000;  // 2 seconds

static int MQINQMP_SPACE               = 1000;
static int MIN_MQINQMP_SPACE           = sizeof(GMSEC_F64);

static int DEBUG_PROP                  = 0;

const char* const DEFAULT_CHANNEL      = "SYSTEM.DEF.SVRCONN";
const char* const WEBSPHERE_SUBJECT    = "GMSEC_SUBJECT_WEBSPHERE";
const char* const WEBSPHERE_KIND       = "GMSEC_KIND_WEBSPHERE";
const char* const DEFAULT_SUBJECT      = "BOGUS.TOPIC";

const char* const MW_PROP_PATTERN      = "gmsec.%";
const char* const MW_PROP_PREFIX       = "gmsec.";
const char* const MW_PROP_SUBJECT      = "SUBJECT";
const char* const MW_PROP_KIND         = "KIND";


// Function prototypes for locally defined helper functions
static std::string idFromMW(const std::string& in);
static std::string idToMW(const std::string& in);
static bool        toMQString(MQCHARV &out, const char* p, int count);
static bool        toMQString(MQCHARV &out, const char* in);
static std::string idFromMW(const MQCHARV& in);
static Message*    parseProperties(ValueMap& meta, MQHCONN hcon, MQHMSG hmsg, const Config& msgConfig);
static bool        getMetaFromFields(Message& message);
static void        storeProperty(const Value& value, const std::string& id0, MQHCONN hcon, MQHMSG hmsg);
static void        storeProperties(const ValueMap &in, MQHCONN hcon, MQHMSG hmsg);


MQLONG EventHandler   (MQHCONN   hConn,
					   MQMD    * pMsgDesc,
					   MQGMO   * pGetMsgOpts,
					   MQBYTE  * Buffer,
					   MQCBC   * pContext)
{
	WebSConnection* conn = reinterpret_cast<WebSConnection*>(pContext->CallbackArea);

	switch(pContext->Reason)
	{
	case MQRC_CONNECTION_BROKEN:
		{
			Status status(CONNECTION_ERROR, CONNECTION_LOST, "Connection broken");
			GMSEC_WARNING << status.get();

			if (conn)
			{
				conn->getExternal().dispatchEvent(Connection::CONNECTION_BROKEN_EVENT, status);
			}
		}
		break;

	case MQRC_RECONNECTING:
		{
			std::ostringstream oss;
			oss << "Attempting to reconnect (delay: " << int(pContext->ReconnectDelay) << "ms)";

			Status status(CONNECTION_ERROR, CONNECTION_RECONNECT, oss.str().c_str());
			GMSEC_WARNING << status.get();

			if (conn)
			{
				conn->getExternal().dispatchEvent(Connection::CONNECTION_RECONNECT_EVENT, status);
			}
		}
		break;

	case MQRC_RECONNECTED:
		{
			Status status(NO_ERROR_CLASS, CONNECTION_CONNECTED, "Connection established");
			GMSEC_INFO << status.get();

			if (conn)
			{
				conn->getExternal().dispatchEvent(Connection::CONNECTION_SUCCESSFUL_EVENT, status);
			}
		}
		break;

	case MQRC_RECONNECT_FAILED:
		{
			Status status(CONNECTION_ERROR, CONNECTION_LOST, "Re-connect has failed");
			GMSEC_WARNING << status.get();

			if (conn)
			{
				conn->getExternal().dispatchEvent(Connection::CONNECTION_EXCEPTION_EVENT, status);
			}
		}
		break;

	default:
		{
			std::ostringstream errmsg;
			errmsg << "Unexpected event occurred; reason = " << int(pContext->Reason) << ")";

			Status status(CONNECTION_ERROR, OTHER_ERROR_CODE, errmsg.str().c_str());
			GMSEC_WARNING << status.get();

			if (conn)
			{
				conn->getExternal().dispatchEvent(Connection::CONNECTION_EXCEPTION_EVENT, status);
			}
		}
		break;
	}

	pContext->ReconnectDelay = 500;

	return 0;
}


static void mwConfig(const Config& config, const char* key, std::string& out, bool requirePrefix = false)
{
	std::string prefixedKey = std::string("MW-") + key;

	const char* value = config.getValue(prefixedKey.c_str());

	if (value != 0)
	{
		out = value;
	}
	else if (!requirePrefix)
	{
		value = config.getValue(key);

		if (value != 0)
		{
			out = value;
		}
	}
}


SubscriptionInfo::SubscriptionInfo(const std::string& t, Condition& subscribe, WebSConnection* connection, MQHCONN exconn, bool replies)
	:
	doneWithLife(false),
	connected(false),
	deathlatch(new CountDownLatch(1)),
	topic(t),
	mqconnection(exconn),
	sub(subscribe),
	conn(connection),
	currBufLen(4000),
	buffer(new char[currBufLen]),
	qTopic(0),
	forReplies(replies),
	Hobj(MQHO_NONE),
	Hsub(MQHO_NONE),
	Reason(0),
	CompCode(0),
	hmsg(MQHM_UNUSABLE_HMSG)
{
}


SubscriptionInfo::~SubscriptionInfo()
{
	this->shutdown();
}


bool SubscriptionInfo::initSubscriptionWithConn(MQGMO& gmo, MQSD& sd, MQMD& md)
{
	MQCBD cbd = {MQCBD_DEFAULT};
	cbd.CallbackType = MQCBT_EVENT_HANDLER;
	cbd.CallbackFunction = (void *) EventHandler;

	MQCB(mqconnection, MQOP_REGISTER, &cbd, MQHO_UNUSABLE_HOBJ, NULL, NULL, &CompCode, &Reason);

	connected = true;

	sd.Options = MQSO_CREATE | MQSO_NON_DURABLE | MQSO_FAIL_IF_QUIESCING | MQSO_MANAGED;

	qTopic = StringUtil::stringNew(topic.c_str());
	if (!qTopic)
	{
		GMSEC_WARNING << "Unable to create new topic string.";
		return false;
	}
	sd.ObjectString.VSPtr    = qTopic;
	sd.ObjectString.VSLength = (MQLONG) StringUtil::stringLength(qTopic);

	MQSUB(mqconnection, &sd, &Hobj, &Hsub, &CompCode, &Reason);                   

	{
		AutoMutex hold(sub.getMutex());
		if (CompCode == MQCC_FAILED)
		{
			GMSEC_ERROR << "[MQSUB ended with reason code: " << Reason << "]";
			sub.signal(WebSConnection::UNABLE_TO_SUBSCRIBE);
			deathlatch->countDown();
			delete[] qTopic;
			return false;
		}
		else
		{
			sub.signal(WebSConnection::GOT_MESSAGE);
		}
	}

	gmo.WaitInterval = conn->getMessageWaitPeriod();   //Need to come back from waiting on the topic periodically
	gmo.Options = MQGMO_WAIT | MQGMO_NO_SYNCPOINT;

	{
		MQCMHO cmho = { MQCMHO_DEFAULT };
		MQCRTMH(mqconnection, &cmho, &hmsg, &CompCode, &Reason);
		if (CompCode == MQCC_OK)
		{
			gmo.Options += MQGMO_PROPERTIES_IN_HANDLE;
			gmo.MsgHandle = hmsg;
			gmo.Version = MQGMO_VERSION_4;
		}
		else
		{
			GMSEC_WARNING << "MQCRTMH: code=" << CompCode << " reason=" << Reason;
		}
	}

	return true;
}


MQLONG SubscriptionInfo::getReasonCode() const
{
	return Reason;
}


void SubscriptionInfo::run()
{
	MQGMO gmo = {MQGMO_DEFAULT};
	MQSD  sd  = {MQSD_DEFAULT};    
	MQMD  md  = {MQMD_DEFAULT};

	if (!initSubscriptionWithConn(gmo, sd, md))
	{
		return;
	}

	while (!doneWithLife)
	{
		StdThread::yield(); // to avoid starving other threads

		//Message and correlation ids set on return, reset to none so as not to retrieve
		//same message twice.
		StringUtil::copyBytes(md.MsgId, MQMI_NONE, sizeof(md.MsgId));
		StringUtil::copyBytes(md.CorrelId, MQCI_NONE, sizeof(md.CorrelId));

		md.Encoding       = MQENC_NATIVE;
		md.CodedCharSetId = MQCCSI_Q_MGR;

		MQLONG messlen = 1;    // message length received

		MQGET(mqconnection, Hobj, &md, &gmo, currBufLen, buffer, &messlen, &CompCode, &Reason);

		if (Reason == MQRC_TRUNCATED_MSG_FAILED && messlen > currBufLen)
		{
			currBufLen = messlen + 100;
			delete [] buffer;
			buffer = new char[currBufLen];
			continue;
		}

		if (doneWithLife)
		{
			continue;
		}

		if (Reason == MQRC_HCONN_ERROR) //Break loop, connection terminated
		{
			GMSEC_WARNING << "SubscriptionInfo - connection terminated.";
			break;
		}

		if (Reason == MQRC_CONNECTION_BROKEN)
		{
			int  maxRetries     = conn->getMaxConnectionRetries();
			int  triesRemaining = maxRetries;
			bool alive          = false;

			//Connection and all object handles now invalidated, need to re-establish
			//connection broker, but only if configured to do so.
			while (!alive && (maxRetries == -1 || --triesRemaining >= 0))
			{
				mqconnection = 0;

				try
				{
					WebSConnection::createLiveConnection(mqconnection, conn->getQueueManager(),
						conn->getHostname(), conn->getChannel(), conn->getCipherSpec(), conn->getKeyReposStem());

					if (mqconnection)
					{
						Status tmpStatus(NO_ERROR_CLASS, CONNECTION_CONNECTED, "Connection established");
						GMSEC_INFO << tmpStatus.get();
						conn->getExternal().dispatchEvent(Connection::CONNECTION_SUCCESSFUL_EVENT, tmpStatus);

						alive = initSubscriptionWithConn(gmo, sd, md);
					}
				}
				catch (const Exception& e)
				{
					GMSEC_ERROR << "SubscriptionInfo - " << e.what();
				}

				if (!alive)
				{
					TimeUtil::millisleep(conn->getConnectionRetryInterval());

					std::ostringstream oss;
					oss << "SubscriptionInfo - Attempting to reconnect.";
					if (maxRetries != -1)
					{
						oss << " Tries remaining: " << triesRemaining;
					}

					GMSEC_INFO << oss.str().c_str();
				}
			}

			if (!alive)
			{
				if (maxRetries > 0)
				{
					GMSEC_ERROR << "SubscriptionInfo - Unable to re-establish connection to server.";
					conn->enqueueResult( Status(CONNECTION_ERROR, CONNECTION_LOST, "Unable to re-establish connection to server") );
				}
				doneWithLife = true;
			}

			// Return to top of loop to acquire new status (Reason) for the connection,
			// unless we are done-with-life, in which case we will just simply exit.
			continue;
		}

		if (CompCode != MQCC_FAILED)
		{
			if (Reason == MQRC_NO_MSG_AVAILABLE)
			{
				//No msg in timeout, continue loop
			}
			else
			{
				DataBuffer alias((GMSEC_U8*) buffer, messlen, false);

				(void) conn->fromMW(alias, mqconnection, hmsg, forReplies);
			}
		}
	}

	if (hmsg != MQHM_UNUSABLE_HMSG)
	{
		MQDMHO dmho = { MQDMHO_DEFAULT };
		MQDLTMH(mqconnection, &hmsg, &dmho, &CompCode, &Reason);
		if (CompCode != MQCC_OK)
		{
			GMSEC_WARNING << "MQDLTMH: code=" << CompCode << " reason=" << Reason;
		}
	}

	deathlatch->countDown();

	delete [] qTopic;
}


void SubscriptionInfo::shutdown()
{
	if (connected)
	{
		doneWithLife = true;

		deathlatch->await();

		MQLONG C_options = MQCO_NONE;

		MQCLOSE(mqconnection, &Hsub, C_options, &CompCode, &Reason);

		MQCLOSE(mqconnection, &Hobj, C_options, &CompCode, &Reason);

		MQDISC(&mqconnection, &CompCode, &Reason);

		delete deathlatch;
		delete [] buffer;

		connected = false;
	}
}


// Make GMSEC compliant string into WebSphere
// compliant string
std::string fixSubject(const char* str)
{
	std::string subject = str;
	// Replace . with /
	size_t i = 1;
	while (true)
	{
		i = subject.find(".",i);
		if (i != std::string::npos)
		{
			subject.replace(i,1,"/");
		}
		else
		{
			break;
		}
	}

	// Replace + with #
	i = 1;
	while (true)
	{
		i = subject.find("+",i);
		if (i != std::string::npos)
		{
			subject.replace(i,1,"#");
		}
		else
		{
			break;
		}
	}

	// Replace > with +/#
	i = 1;
	while (true)
	{
		i = subject.find(">",i);
		if (i != std::string::npos)
		{
			subject.replace(i,1,"+/#");
		}
		else
		{
			break;
		}
	}

	// Replace * with +
	i = 1;
	while (true)
	{
		i = subject.find("*",i);
		if (i != std::string::npos)
		{
			subject.replace(i,1,"+");
		}
		else
		{
			break;
		}
	}

	return subject.c_str();
}


// vice-versa
std::string UnfixSubject(const char* str)
{
	std::string subject;

	if (NULL == str)
	{
		return subject;
	}

	subject = str;

	// Replace / with .
	size_t i = 1;
	while (true)
	{
		i = subject.find("/",i);
		if (i != std::string::npos)
		{
			subject.replace(i,1,".");
		}
		else
		{
			break;
		}
	}

	// Replace +.# with >
	i = 1;
	while (true)
	{
		i = subject.find("+.#",i);
		if (i != std::string::npos)
		{
			subject.replace(i,3,">");
		}
		else
		{
			break;
		}
	}

	// Replace + with *
	i = 1;
	while (true)
	{
		i = subject.find("+",i);
		if (i != std::string::npos)
		{
			subject.replace(i,1,"*");
		}
		else
		{
			break;
		}
	}

	// Replace # with +
	i = 1;
	while (true)
	{
		i = subject.find("#",i);
		if (i != std::string::npos)
		{
			subject.replace(i,1,"+");
		}
		else
		{
			break;
		}
	}

	return subject;
}


WebSConnection::WebSConnection(const Config& config)
	:
	uniquecounter(0),
	requestCounter(0),
	checkAsyncStatusMessageInterval(DEFAULT_ASYNC_STATUS_MESSAGE_INTERVAL),
	messageCounter(0),
	messageWaitPeriod(DEFAULT_MESSAGE_WAIT_PERIOD),
	sigMismatchFlag(false),
	maxConnectionRetries(DEFAULT_MAX_CONN_RETRIES),
	connectionRetryInterval(DEFAULT_CONN_RETRY_INTERVAL),
	Hcon(0)
{
	std::string filterToggle;
	std::string asyncPublish;
	std::string checkAsyncStatus;

	// Try to get the needed values out of the config object.
	mwConfig(config, "server", hostname);
	mwConfig(config, "filter-dups", filterToggle);

	mwConfig(config, "channel", channel, true);
	mwConfig(config, "queuemanager", qmanager, true);
	mwConfig(config, "keystore", pKeyReposStem, true);
	mwConfig(config, "cipher", pCipherSpec, true);
	mwConfig(config, OPT_ASYNC_PUBLISH, asyncPublish);
	mwConfig(config, OPT_ASYNC_STATUS_CHECK, checkAsyncStatus);

	int tmpint;
	std::string tmp;

	mwConfig(config, OPT_ASYNC_STATUS_CHECK_MESSAGE_INTERVAL, tmp);
	if (tmp.length())
	{
		if (StringUtil::str2int(tmpint, tmp.c_str()) == StringUtil::STR2NUM_SUCCESS)
		{
			if (tmpint >= -1)
			{
				checkAsyncStatusMessageInterval = tmpint;
			}
			else
			{
				GMSEC_WARNING << "ignoring invalid value for " << OPT_ASYNC_STATUS_CHECK_MESSAGE_INTERVAL << ": " << tmp.c_str();
			}
		}
		else
		{
			GMSEC_WARNING << "ignoring non-numeric value for " << OPT_ASYNC_STATUS_CHECK_MESSAGE_INTERVAL << ": " << tmp.c_str();
		}
	}

	messageWaitPeriod = config.getIntegerValue(OPT_MESSAGE_WAIT_PERIOD, messageWaitPeriod);
	messageWaitPeriod = (messageWaitPeriod < 100 ? 100 : messageWaitPeriod);

	tmp = "";
	mwConfig(config, "MQINQMP-SPACE", tmp, true);
	GMSEC_I32 space = 0;
	if (tmp.length())
	{
		if (StringUtil::stringParseI32(tmp.c_str(), space) && space >= MIN_MQINQMP_SPACE)
		{
			MQINQMP_SPACE = space;
		}
		else
		{
			GMSEC_INFO << "ignoring invalid MQINQMP-SPACE: " << tmp.c_str();
		}
	}

	tmp = "";
	mwConfig(config, "DEBUG-PROP", tmp, true);
	DEBUG_PROP = StringUtil::stringIsTrue(tmp.c_str());

	tmp = "";
	mwConfig(config, OPT_MAX_CONN_RETRIES, tmp, false);
	if (tmp.length() > 0)
	{
		if (StringUtil::str2int(tmpint, tmp.c_str()) == StringUtil::STR2NUM_SUCCESS)
		{
			if (tmpint >= -1)
			{
				maxConnectionRetries = tmpint;
			}
			else
			{
				GMSEC_WARNING << "ignoring invalid value for " << OPT_MAX_CONN_RETRIES << ": " << tmp.c_str();
			}
		}
		else
		{
			GMSEC_WARNING << "ignoring invalid " << OPT_MAX_CONN_RETRIES << ": " << tmp.c_str();
		}
	}

	tmp = "";
	mwConfig(config, OPT_CONN_RETRY_INTERVAL, tmp, false);
	if (tmp.length() > 0)
	{
		if (StringUtil::str2int(tmpint, tmp.c_str()) == StringUtil::STR2NUM_SUCCESS)
		{
			if (tmpint >= 0)
			{
				connectionRetryInterval = tmpint;
			}
			else
			{
				GMSEC_WARNING << "ignoring invalid value for " << OPT_CONN_RETRY_INTERVAL << ": " << tmp.c_str();
			}
		}
		else
		{
			GMSEC_WARNING << "ignoring invalid " << OPT_CONN_RETRY_INTERVAL << ": " << tmp.c_str();
		}
	}

	//Default maximum number of handles to 100
	maxTopicHandles = 100;
	tmp = "";
	mwConfig(config, OPT_MAX_TOPIC_HANDLES, tmp, false);
	if (!tmp.empty())
	{
		bool error = false;

		try
		{
			int handles = StringUtil::getValue<int>(tmp.c_str());

			if (handles <= 0)
			{
				error = true;
			}
			else
			{
				maxTopicHandles = (size_t) handles;
			}
		}
		catch (...)
		{
			error = true;
		}

		if (error)
		{
			GMSEC_WARNING << "Ignoring invalid value for " << OPT_MAX_TOPIC_HANDLES << ": " << tmp.c_str();
		}
	}

	useFilter = filterToggle.empty() || StringUtil::stringEqualsIgnoreCase(filterToggle.c_str(), "yes");
	connAsyncPublish = StringUtil::stringEqualsIgnoreCase(asyncPublish.c_str(), "true");
	connCheckAsyncStatus = StringUtil::stringEqualsIgnoreCase(checkAsyncStatus.c_str(), "true");
}


WebSConnection::~WebSConnection()
{
	for (Subscriptions::iterator it = subscriptions.begin(); it != subscriptions.end(); ++it)
	{
		SharedSubscriptionInfo subInfo = it->second;

		subInfo->shutdown();

		subInfo.reset();
	}
	subscriptions.clear();

	while (!queue.empty())
	{
		MsgSubscriptionResult* msgResult = queue.front();

		if (msgResult->message)
		{
			delete msgResult->message;
		}

		queue.pop();

		delete msgResult;
	}
}


const char* WebSConnection::getLibraryVersion()
{
	return WEBSPHERE_VERSION_STRING;
}


const char* WebSConnection::getMWInfo()
{
	if (mwInfo.empty())
	{
		std::ostringstream strm;
		strm << getLibraryRootName();
		mwInfo = strm.str();
	}
	return mwInfo.c_str();
}


void WebSConnection::mwConnect()
{
	// Initialize request specifications
	requestSpecs = getExternal().getRequestSpecs();

	Hcon = 0;
	WebSConnection::createLiveConnection(Hcon, qmanager, hostname, channel, pCipherSpec, pKeyReposStem);

	Status tmpStatus(NO_ERROR_CLASS, CONNECTION_CONNECTED, "Connection established");
	GMSEC_INFO << tmpStatus.get();
	getExternal().dispatchEvent(Connection::CONNECTION_SUCCESSFUL_EVENT, tmpStatus);

	if (requestSpecs.requestReplyEnabled && requestSpecs.useSubjectMapping)
	{
		MQHCONN RequestConn;
		WebSConnection::createLiveConnection(RequestConn, qmanager, hostname, channel, pCipherSpec, pKeyReposStem);

		std::string wsSubject = fixSubject(requestSpecs.replySubject.c_str());

		if (!requestThread.get())
		{
			requestSubscriptionInfo.reset(new SubscriptionInfo(wsSubject, subscribeCondition, this, RequestConn, true));

			requestThread.reset(new StdThread(&runSubscriptionThread, requestSubscriptionInfo));

			AutoMutex subMutex(subscribeCondition.getMutex());

			GMSEC_DEBUG << "[" << this << "] is starting the Request Subscription Thread";

			requestThread->start();

			if (subscribeCondition.wait() == WebSConnection::UNABLE_TO_SUBSCRIBE)
			{
				throw Exception(MIDDLEWARE_ERROR, INITIALIZATION_ERROR, WebSConnection::UNABLE_TO_SUBSCRIBE, "Unable to start reply listener");
			}
		}
	}

	MQCBD    cbd = {MQCBD_DEFAULT};   /* Callback Descriptor */
	MQLONG   CompCode;                
	MQLONG   CReason;

	cbd.CallbackType     = MQCBT_EVENT_HANDLER;
	cbd.CallbackFunction = (void *) EventHandler;
	cbd.CallbackArea     = this;    // provided so that we can call dispatchEvent()

	MQCB(Hcon, MQOP_REGISTER, &cbd, MQHO_UNUSABLE_HOBJ, NULL, NULL, &CompCode, &CReason);

	if (CompCode == MQCC_FAILED)
	{
		std::ostringstream ss;
		ss << "MQCB ended with reason code " << CReason << ".";
		GMSEC_WARNING << ss.str().c_str();
	}

	getExternal().setConnectionEndpoint(hostname);
}


void WebSConnection::mwDisconnect()
{
	MQLONG   CompCode;
	MQLONG   Reason;

	if (requestSubscriptionInfo.get())
	{
		GMSEC_DEBUG << "[" << this << "] is stopping the Subscription Thread";

		requestSubscriptionInfo->shutdown();
		requestSubscriptionInfo.reset();
	}

	flushTopicHandles();

	for (Subscriptions::iterator it = subscriptions.begin(); it != subscriptions.end(); ++it)
	{
		SharedSubscriptionInfo subInfo = it->second;

		subInfo->shutdown();

		subInfo.reset();
	}
	subscriptions.clear();

	MQDISC(&Hcon, &CompCode, &Reason);

	if (Reason != MQRC_NONE)
	{
		std::ostringstream ss;
		ss << "MQDISC ended with reason code " << Reason << ".";
		GMSEC_WARNING << ss.str().c_str();
	}

	Status tmpStatus(NO_ERROR_CLASS, CONNECTION_LOST, "Connection disconnected");
	GMSEC_INFO << tmpStatus.get();
	getExternal().dispatchEvent(Connection::CONNECTION_BROKEN_EVENT, tmpStatus);
}


void WebSConnection::mwSubscribe(const char* subject, const Config& config)
{
	Status result;

	std::string fixedSub = fixSubject(subject);

	if (subscriptions.count(fixedSub) > 0)
	{
		//Already subscribed, don't do anything
		return;
	}

	MQHCONN  SubConn = 0;
	WebSConnection::createLiveConnection(SubConn, qmanager, hostname, channel, pCipherSpec, pKeyReposStem);

	bool allowReplies = false;
	if (!requestSpecs.useSubjectMapping)
	{
		allowReplies = requestSpecs.legacy || requestSpecs.exposeReplies;
	}

	bool ableToSubscribe = true;

	SharedSubscriptionInfo subInfo(new SubscriptionInfo(fixedSub, subscribeCondition, this, SubConn, allowReplies));

	SubscriptionThread subThread(new StdThread(&runSubscriptionThread, subInfo));

	AutoMutex subMutex(subscribeCondition.getMutex());

	subThread->start();

	if (subscribeCondition.wait() == WebSConnection::UNABLE_TO_SUBSCRIBE)
	{
		ableToSubscribe = false;
	}
	else
	{
		subscriptions[fixedSub] = subInfo;
		GMSEC_DEBUG << "[Subscribed successfully: " << subject << "]";
	}

	if (!ableToSubscribe)
	{
		// Unable to set up subscription
		throw Exception(MIDDLEWARE_ERROR, INITIALIZATION_ERROR, subInfo->getReasonCode(), "Unable to subscribe");
	}
}


void WebSConnection::mwUnsubscribe(const char* subject)
{
	std::string fixedSub = fixSubject(subject);

	AutoMutex subMutex(subscribeCondition.getMutex());

	Subscriptions::iterator it = subscriptions.find(fixedSub);

	if (it == subscriptions.end())
	{
		throw Exception(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Not subscribed to subject");
	}

	SharedSubscriptionInfo subInfo = it->second;

	subInfo->shutdown();

	subInfo.reset();

	subscriptions.erase(fixedSub);

	GMSEC_DEBUG << "[Unsubscribed successfully: " << subject << "]";
}


void WebSConnection::mwPublish(const Message& msg, const Config& config)
{
	std::string asyncPublish;
	mwConfig(config, OPT_ASYNC_PUBLISH, asyncPublish);
	bool pubAsyncPublish = (StringUtil::stringEqualsIgnoreCase(asyncPublish.c_str(), "true"));

	std::string checkAsyncStatus;
	mwConfig(config, OPT_ASYNC_STATUS_CHECK, checkAsyncStatus);
	bool pubCheckAsyncStatus = (StringUtil::stringEqualsIgnoreCase(checkAsyncStatus.c_str(), "true"));

	ValueMap             meta;
	Message::MessageKind kind    = msg.getKind();
	const char*          subject = msg.getSubject();

	meta.setI32(MW_PROP_KIND, kind);
	meta.setString(MW_PROP_SUBJECT, subject);

	DataBuffer buffer;

	Status result = getExternal().getPolicy().package(const_cast<Message&>(msg), buffer, meta);

	if (result.isError())
	{
		throw Exception(result);
	}

	std::string fixedSub = fixSubject(subject);

	MQHOBJ objHandle;
	MQMD   msgDesc   = {MQMD_DEFAULT};
	MQHMSG msgHandle = MQHM_UNUSABLE_HMSG;
	MQPMO  msgOpts   = {MQPMO_DEFAULT};

	if (pubAsyncPublish || connAsyncPublish)
	{
		// Turn on Asynchronous Put Response (Middleware-level Asynchronous Publish)
		msgOpts.Options |= MQPMO_ASYNC_RESPONSE;
	}

	int maxRetries     = getMaxConnectionRetries();
	int triesRemaining = maxRetries;
	do
	{
		if (!result.isError())
		{
			if (publishers.size() >= maxTopicHandles)
			{
				flushTopicHandles();
			}

			// Step 1: Reference object handle, if it exists.
			if (publishers.count(fixedSub) == 0)
			{
				// If object handle does not exist.
				try
				{
					openObjectHandle(fixedSub, msgDesc, objHandle);

					publishers[fixedSub] = objHandle;
				}
				catch (const Exception& e)
				{
					result = Status(e);
				}
			}
			else
			{
				// reference existing object handle.
				objHandle = publishers[fixedSub];
			}

			if (!result.isError())
			{
				try
				{
					// Step 2: Create Message Handle
					createMsgHandle(meta, msgHandle, msgOpts);

					// Step 3: Publish the Message
					publishMessage(objHandle, msgDesc, msgOpts, buffer);
				}
				catch (const Exception& e)
				{
					result = Status(e);
				}
			}
		}

		if (result.isError())
		{
			GMSEC_WARNING << "mwPublish: " << result.get();

			MQLONG reason = result.getCustomCode();

			// If we're configured to retry, and we have hit one of the specific errors we can handle,
			// attempt to reconnect. Otherwise, we're done... throw an exception.
			if ((maxRetries == -1 || maxRetries > 0) &&
			    (  reason == MQRC_CONNECTION_BROKEN   // 2009
			    || reason == MQRC_HCONN_ERROR         // 2018
			    || reason == MQRC_Q_MGR_NOT_AVAILABLE // 2059
			    || reason == MQRC_Q_MGR_QUIESCING     // 2161
			    || reason == MQRC_HOST_NOT_AVAILABLE  // 2538
			    || reason == MQRC_RECONNECT_FAILED))  // 2548
			{
				TimeUtil::millisleep(connectionRetryInterval);

				std::ostringstream oss;
				oss << "mwPublish - Attempting to reconnect.";
				if (triesRemaining > 0)
				{
					oss << " Tries remaining: " << triesRemaining;
				}
				GMSEC_WARNING << oss.str().c_str();

				result = reconnect();
			}
			else
			{
				throw Exception(result);
			}
		}
	} while (result.isError() && (maxRetries == -1 || --triesRemaining >= 0));

	if (result.isError() && triesRemaining <= 0)
	{
		GMSEC_ERROR << "mwPublish - Unable to re-establish connection to server; result = " << result.get();

		throw Exception(result);
	}
	else
	{
		if (msgHandle != MQHM_UNUSABLE_HMSG)
		{
			MQLONG compCode = MQCC_OK;
			MQLONG reason   = MQRC_NONE;
			MQDMHO dmho     = { MQDMHO_DEFAULT };

			MQDLTMH(Hcon, &msgHandle, &dmho, &compCode, &reason);

			if (compCode != MQCC_OK)
			{
				GMSEC_WARNING << "mwPublish: MQDLTMH=" << (void*) msgHandle << " code=" << compCode << " reason=" << reason;
			}
		}
	}

	if (pubCheckAsyncStatus || (connCheckAsyncStatus && (++messageCounter > (checkAsyncStatusMessageInterval - 1))))
	{
		// Check and log the status of previous Asynchronous Put calls
		checkAsyncPublishStatus();

		// Reset the message counter
		messageCounter = 0;
	}
}


void WebSConnection::mwRequest(const Message& request, std::string& id)
{
	std::string RequestID;

	id = generateUniqueId(++requestCounter);

	MessageBuddy::getInternal(request).addField(GMSEC_REPLY_UNIQUE_ID_FIELD, id.c_str());

	if (!requestSpecs.useSubjectMapping)
	{
		MessageBuddy::getInternal(request).getDetails().setBoolean(GMSEC_REQ_RESP_BEHAVIOR, true);
	}

	mwPublish(request, getExternal().getConfig());

	GMSEC_DEBUG << "[Request sent successfully: "<< request.getSubject() << "]";
}


void WebSConnection::mwReply(const Message& request, const Message& reply)
{
	std::string uniqueID  = getExternal().getReplyUniqueID(request);

	if (uniqueID.empty())
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Request does not contain Unique ID field");
	}

	MessageBuddy::getInternal(reply).addField(GMSEC_REPLY_UNIQUE_ID_FIELD, uniqueID.c_str());
	MessageBuddy::getInternal(reply).setSubject(uniqueID.substr(0, uniqueID.rfind("_")).c_str());

	mwPublish(reply, getExternal().getConfig());

	MessageBuddy::getInternal(reply).clearField(GMSEC_REPLY_UNIQUE_ID_FIELD);

	GMSEC_DEBUG << "[Reply sent successfully: " << reply.getSubject() <<"]";
}


void WebSConnection::mwReceive(Message*& msg, GMSEC_I32 timeout)
{
	double start_s;

	// initialize message to NULL
	msg = NULL;

	if (timeout != GMSEC_WAIT_FOREVER)
	{
		start_s = TimeUtil::getCurrentTime_s();
	}

	bool done  = false;
	bool first = true; // if timeout > 0, ensure that a wait occurs

	while (!done)
	{
		AutoMutex inner(queueCondition.getMutex());
		int reason = 0;

		if (!queue.empty())
		{
			// have a status/message, don't need to wait
			reason = GOT_MESSAGE;
		}
		else if (timeout == 0)
		{
			// poll for message; no wait
			done = true;
		}
		else if (timeout == GMSEC_WAIT_FOREVER)
		{
			reason = queueCondition.wait();
		}
		else
		{
			// timeout > 0
			double remaining_ms = timeout - 1000 * (TimeUtil::getCurrentTime_s() - start_s);

			if (first || remaining_ms > 0)
			{
				int wait_ms = int(remaining_ms);
				if (wait_ms < 1)
				{
					wait_ms = 1;
				}
				reason = queueCondition.wait(wait_ms);
			}
			else
			{
				// timeout (no error)
				done = true;
			}
		}

		first = false;
		if (sigMismatchFlag)
		{
			GMSEC_DEBUG << "Signature mismatch";
			sigMismatchFlag = false;
			continue;
		}
		if (reason == GOT_MESSAGE)
		{
			if (!queue.empty())
			{
				MsgSubscriptionResult* msgResult = queue.front();

				msg = msgResult->message;
				Status result = msgResult->status;

				queue.pop();
				delete msgResult;

				if (result.isError())
				{
					throw Exception(result);
				}

				if (msg != NULL)
				{
					const StringField* uniqueID = dynamic_cast<const StringField*>(msg->getField("UNIQUE-ID"));

					if (useFilter && uniqueID)
					{
						const char* value = uniqueID->getValue();

						if (uniqueFilter.update(value))
						{
							// found a unique message
							done = true;
						}
					}
					else
					{
						// Either tracking is turned off on the publisher
						// (No UNIQUE-ID field), or filtering is turned off
						// on the subscriber, so return the message.
						done = true; 
					}

//DMW
#if 0
					if (!requestSpecs.useSubjectMapping)
					{
						ValueMap& meta = MessageBuddy::getInternal(*msg).getDetails();

						meta.setBoolean(GMSEC_REQ_RESP_BEHAVIOR, true);
					}

					if (msg->getKind() == Message::REQUEST)
					{
						try
						{
							getExternal().setReplyUniqueID(*msg, msg->getStringValue(GMSEC_REPLY_UNIQUE_ID_FIELD));

							msg->clearField(GMSEC_REPLY_UNIQUE_ID_FIELD);
						}
						catch (...)
						{
							GMSEC_WARNING << "GMSEC_REPLY_UNIQUE_ID_FIELD field is missing!";
						}
					}
#endif
				}
			}
		}
	}

	if (msg != NULL)
	{
		GMSEC_DEBUG << "[Received published message: " << msg->getSubject() << "]";
	}
}


bool WebSConnection::fromMW(const DataBuffer& buffer, MQHCONN hcon, MQHMSG hmsg, bool forReplies)
{
	StdUniquePtr<Message> msg;
	ValueMap              meta;

	try
	{
		msg.reset(parseProperties(meta, hcon, hmsg, getExternal().getMessageConfig()));
	}
	catch (const Exception& e)
	{
		GMSEC_WARNING << "Unable to extract meta-data from message -- " << e.what();
		enqueueResult(Status(e));
		return false;
	}

	Status result = getExternal().getPolicy().unpackage(*msg.get(), buffer, meta);

	if (result.isError())
	{
		if (result.getCode() == INVALID_SIGNATURE)
		{
			sigMismatchFlag = true;
			AutoMutex hold(queueCondition.getMutex());
			queueCondition.signal(GOT_MESSAGE);
		}

		GMSEC_WARNING << "Unable to unpackage message";
		enqueueResult(result);
		return false;
	}

	const char* subject = msg->getSubject();

	if (StringUtil::stringEquals(subject, DEFAULT_SUBJECT))
	{
		getMetaFromFields(*msg.get());
	}

	Message::MessageKind kind = msg->getKind();

	bool enqueue = true;

	if (kind == Message::REPLY)
	{
		// the legacy behavior was to expose subscribed replies
		if (requestSpecs.legacy)
		{
			enqueue = !forReplies;
		}
		else
		{
			enqueue = requestSpecs.exposeReplies;
		}

		Message* tmp = msg.get();
		getExternal().updateReplySubject(tmp);

		if (forReplies)
		{
			Message* reply = msg.release();

			// make a copy of the message if it is being passed to onReply
			// and placed on the queue
			if (enqueue)
			{
				msg.reset(new Message(*reply));
			}

			if (reply)
			{
				getExternal().onReply(reply);
			}
		}
	}
	else if (kind == Message::REQUEST)
	{
		if (requestSpecs.useSubjectMapping)
		{
			try
			{
				getExternal().setReplyUniqueID(*msg, msg->getStringValue(GMSEC_REPLY_UNIQUE_ID_FIELD));

				msg->clearField(GMSEC_REPLY_UNIQUE_ID_FIELD);
			}
			catch (...)
			{
				GMSEC_WARNING << "GMSEC_REPLY_UNIQUE_ID_FIELD field is missing!";
			}
		}
		else
		{
			ValueMap& meta = MessageBuddy::getInternal(*msg.get()).getDetails();

			meta.setBoolean(GMSEC_REQ_RESP_BEHAVIOR, true);
		}
	}

	if (enqueue)
	{
		enqueueResult(Status(), msg.release());
	}

	return true;
}


void WebSConnection::enqueueResult(const Status& status, Message* message)
{
	AutoMutex hold(queueCondition.getMutex());
	queue.push(new MsgSubscriptionResult(status, message));
	queueCondition.signal(GOT_MESSAGE);
}


void WebSConnection::createLiveConnection(MQHCONN& subConn,
                                          const std::string& qmanager,
                                          const std::string& hostname, 
                                          const std::string& channel,
                                          const std::string& pCipherSpec,
                                          const std::string& pKeyReposStem)
{
	// MQCONNX options
	MQCNO ConnectOptions = {MQCNO_DEFAULT}; 
	ConnectOptions.Options = MQCNO_HANDLE_SHARE_BLOCK | MQCNO_RECONNECT_Q_MGR;

	// Client connection channel definition
	MQCD ClientConn = {MQCD_CLIENT_CONN_DEFAULT}; 

	// SSL connection options
	MQSCO SslConnOptions = {MQSCO_DEFAULT};                         
 
	// Values used for MQI calls once the client has connected
	MQCHAR QMName[MQ_Q_MGR_NAME_LENGTH]; // name of connection q manager
	MQLONG CompCode;                     // completion code
	MQLONG CReason;                      // reason code for MQCONNX
 
	if (!qmanager.empty()) 
	{ 
		StringUtil::stringCopy(QMName, qmanager.c_str(), MQ_Q_MGR_NAME_LENGTH); 
	} 
	else 
	{ 
		QMName[0] = '\0';  // default
	} 
 
	StringUtil::stringCopy(ClientConn.ConnectionName, hostname.c_str(), MQ_CONN_NAME_LENGTH); 
 
	std::string argChannelName(channel);

	if (channel.empty())
	{
		argChannelName = DEFAULT_CHANNEL;
	}
 
	StringUtil::stringCopy(ClientConn.ChannelName, argChannelName.c_str(), MQ_CHANNEL_NAME_LENGTH); 
 
	ClientConn.MaxMsgLength = 104857600;

	// Point the MQCNO to the client connection definition
	ConnectOptions.ClientConnPtr = &ClientConn; 
 
	if (!pCipherSpec.empty())
	{
		// SSL requires MQCD version 7 or later
		ClientConn.Version = MQCD_VERSION_7;

		// The MQCD must contain the SSL CipherSpec string
		StringUtil::stringCopy(ClientConn.SSLCipherSpec, pCipherSpec.c_str(), MQ_SSL_CIPHER_SPEC_LENGTH);
	}
 
	if (!pKeyReposStem.empty())
	{
		// The MQSCO can contain a key repository location
		StringUtil::stringCopy(SslConnOptions.KeyRepository, pKeyReposStem.c_str(), MQ_SSL_KEY_REPOSITORY_LENGTH);

		// Point the MQCNO to the SSL configuration options
		ConnectOptions.SSLConfigPtr = &SslConnOptions;
	}
   
	// We must specify MQCNO version 4 to ensure that both the client
	// connection pointer and SSL configuration options are used 
	ConnectOptions.Version = MQCNO_VERSION_4;
	MQCONNX(QMName, &ConnectOptions, &subConn, &CompCode, &CReason);
	
	if (CompCode == MQCC_FAILED)
	{
		std::ostringstream ss;
		ss << "MQCONNX ended with reason code " << CReason << ".";
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, CReason, ss.str().c_str());
	}
}


void WebSConnection::runSubscriptionThread(SharedSubscriptionInfo shared)
{
	shared->run();
}


std::string WebSConnection::generateUniqueId(long id)
{
	std::ostringstream strm;
	strm << getExternal().getID() << "_" << id;
	return strm.str();
}


void WebSConnection::openObjectHandle(const std::string& subject,
                                      MQMD& md,
                                      MQHOBJ& objHandle)
{
	char* qTopic = StringUtil::stringNew(subject.c_str());

	MQOD od = {MQOD_DEFAULT};    // Object description

	od.ObjectString.VSPtr    = qTopic;
	od.ObjectString.VSLength = (MQLONG) StringUtil::stringLength(qTopic);
	od.ObjectType            = MQOT_TOPIC;
	od.Version               = MQOD_VERSION_4;

	StringUtil::copyBytes(md.Format, MQFMT_STRING, (size_t) MQ_FORMAT_LENGTH);

	MQLONG compCode = MQCC_OK;
	MQLONG reason   = MQRC_NONE;

	MQOPEN(Hcon,                                 // connection handle
	       &od,                                  // object desc for topic
           MQOO_OUTPUT | MQOO_FAIL_IF_QUIESCING, // open options
	       &objHandle,                           // object handle
	       &compCode,                            // MQOPEN completion code
	       &reason);                             // reason code

	StringUtil::stringDestroy(qTopic);

	if (compCode != MQCC_OK)
	{
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, reason, "MQOPEN internal error");
	}
}


void WebSConnection::createMsgHandle(ValueMap& meta, MQHMSG& msgHandle, MQPMO& msgOptions)
{
	MQLONG compCode = MQCC_OK;
	MQLONG reason   = MQRC_NONE;

	MQCMHO cmho = { MQCMHO_DEFAULT };
	cmho.Options = MQCMHO_VALIDATE;

	MQCRTMH(Hcon, &cmho, &msgHandle, &compCode, &reason);

	if (compCode == MQCC_OK)
	{
		#if TEST_PROPS
			meta.setBoolean("T", true);
			meta.setBoolean("F", false);
			meta.setI32("I888", 888);
			string q;
			static char c[] = "0123456789";
			for (int i = 0; i < 66; ++i)
			{
				q.push_back(c[i%10]);
			}
			meta.setString("S66", q);
		#endif

		storeProperties(meta, Hcon, msgHandle);

		msgOptions.OriginalMsgHandle = msgHandle;
		msgOptions.Version           = MQPMO_VERSION_3;
	}
	else
	{
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, reason, "MQCRTMH internal error");
	}
}


void WebSConnection::checkAsyncPublishStatus()
{
	Status status;

	MQSTS sts       = {MQSTS_DEFAULT};
	MQLONG compCode = MQCC_OK;
	MQLONG reason   = MQRC_NONE;

	// Check for errors and warnings in previous async MQPUT calls and log output using GMSEC Logger
	MQSTAT(Hcon,                    // connection handle
	       MQSTAT_TYPE_ASYNC_ERROR, // status type
	       &sts,                    // status information
	       &compCode,               // completion code
	       &reason);                // reason code

	std::ostringstream strm;

	// For statistical reasons, track the number of successful publishes
	strm << "Status of asynchronous publish operations since last check:\n";

	if (sts.PutSuccessCount > 0)
	{
		strm << "Successes: " << sts.PutSuccessCount;
	}

	if (sts.PutWarningCount > 0)
	{
		if (sts.PutSuccessCount > 0)
		{
			strm << ", ";
		}

		if (sts.PutFailureCount == 0)
		{
			strm << "Warnings [Reason Code of first Warning: " << sts.Reason << "]: " << sts.PutWarningCount;
		}
		else
		{
			strm << "Warnings: " << sts.PutWarningCount;
		}
	}

	if (sts.PutFailureCount > 0)
	{
		if (sts.PutSuccessCount > 0 || sts.PutWarningCount > 0)
		{
			strm << ", ";
		}

		strm << "Failures [Reason Code of first Failure: " << sts.Reason << "]: " << sts.PutFailureCount;
	}

	// If there were failures or warnings, package up the status as an error with the Reason code
	if (sts.CompCode == MQCC_FAILED || sts.CompCode == MQCC_WARNING)
	{
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, strm.str().c_str(), sts.Reason);
	}
	// If there were no warnings or errors, package up the status with the successful output string
	else
	{
		status.set(NO_ERROR_CLASS, NO_ERROR_CODE, strm.str().c_str());
	}

	getExternal().dispatchEvent(Connection::WSMQ_ASYNC_STATUS_CHECK_EVENT, status);
}


void WebSConnection::publishMessage(MQHOBJ& objHandle, MQMD& msgDesc, MQPMO& msgOpts, DataBuffer& buffer)
{
	MQLONG compCode = MQCC_OK;
	MQLONG reason   = MQRC_NONE;

	MQPUT(Hcon,              // connection handle
	      objHandle,         // object handle
	      &msgDesc,          // message descriptor
	      &msgOpts,          // default options (datagram)
	      buffer.size(),     // message length
	      buffer.raw(),      // message buffer
	      &compCode,         // completion code
	      &reason);          // reason code

	if (compCode != MQCC_OK)
	{
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, reason, "MQPUT internal error");
	}
}


Status WebSConnection::reconnect()
{
	Status status;

	publishers.clear();

	Hcon = 0;

	try
	{
		WebSConnection::createLiveConnection(Hcon, qmanager, hostname, channel, pCipherSpec, pKeyReposStem);

		MQLONG compCode;
		MQLONG reason;
		MQCBD  cbd = {MQCBD_DEFAULT};

		cbd.CallbackType     = MQCBT_EVENT_HANDLER;
		cbd.CallbackFunction = (void *) EventHandler;

		MQCB(Hcon, MQOP_REGISTER, &cbd, MQHO_UNUSABLE_HOBJ, NULL, NULL, &compCode, &reason);

		if (compCode == MQCC_FAILED)
		{
			GMSEC_WARNING << "reconnect: MQCB ended with reason code " << reason << ".";
		}
	}
	catch (const Exception& e)
	{
		status = Status(e);
	}

	return status;
}


void WebSConnection::flushTopicHandles()
{
	MQLONG   CompCode;
	MQLONG   Reason;
	MQHOBJ obj;

	std::map<std::string, MQHOBJ>::iterator iter;

	for (iter = publishers.begin(); iter != publishers.end(); ++iter)
	{
		obj = iter->second;
		MQCLOSE(Hcon, &obj, MQCO_NONE, &CompCode, &Reason);

		if (Reason != MQRC_NONE)
		{
			GMSEC_ERROR << "Failed to close MQ Publish Handle: " << Reason;
		}
	}

	publishers.clear();
}


/* this toMQString initializes an input MQCHARV to an existing string */
bool toMQString(MQCHARV &out, const char* in)
{
	static MQCHARV init = { MQCHARV_DEFAULT };
	out = init;
	out.VSPtr = (char*) in;
	out.VSLength = StringUtil::stringLength(in);
	return true;
}


/* this toMQString initializes an output MQCHARV to an available buffer */
bool toMQString(MQCHARV &out, const char* p, int count)
{
	static MQCHARV init = { MQCHARV_DEFAULT };
	out = init;
	out.VSPtr = (char*) p;
	out.VSBufSize = count;
	return true;
}


/* idToMW/idFromMW convert GMSEC names to/from MQ message property names
   	map hyphen (-) <-> 'h'
*/
std::string idToMW(const std::string& in)
{
	std::string out = in;

	for (size_t i = in.find_first_of('-', 0); i != std::string::npos; i = in.find_first_of('-', i+1))
	{
		out[i] = 'h';
	}

	return out;
}


std::string idFromMW(const std::string& in)
{
	std::string out = in;

	for (size_t i = in.find_first_of('h', 0); i != std::string::npos; i = in.find_first_of('h', i+1))
	{
		out[i] = '-';
	}

	return out;
}


std::string idFromMW(const MQCHARV& in)
{
	std::string tmp;

	if (in.VSOffset != 0)
	{
		GMSEC_WARNING << "idFromMW(MQCHARV&): VSOffset=" << in.VSOffset;
	}

	if (in.VSPtr != 0)
	{
		if (in.VSLength >= 0)
		{
			tmp = std::string((const char*) in.VSPtr, in.VSLength);
		}
	}
	else
	{
		GMSEC_WARNING << "idFromMW(MQCHARV&): VSPtr=null";
	}

	std::string out = idFromMW(tmp);

	return out;
}


Message* parseProperties(ValueMap& meta, MQHCONN hcon, MQHMSG hmsg, const Config& msgConfig)
{
	MQIMPO     ipo   = { MQIMPO_DEFAULT };
	MQPD       mqpd  = { MQPD_DEFAULT };
	MQCHARV    iname = { MQCHARV_DEFAULT };
	MQLONG     length = 0;
	DataBuffer buffer(MQINQMP_SPACE);

	MQLONG code   = 0;
	MQLONG reason = 0;

	if (buffer.get() == 0)
	{
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Failed to allocate memory for buffer");
	}

	char onameraw[256];
	onameraw[0] = 0;
	toMQString(ipo.ReturnedName, onameraw, sizeof(onameraw));

	toMQString(iname, MW_PROP_PATTERN);

	StdUniquePtr<Message> msg(new Message(DEFAULT_SUBJECT, Message::PUBLISH, msgConfig));

	for (bool first = true; true; first = false)
	{
		bool requery = false;

		ipo.Options = (first ? MQIMPO_INQ_FIRST : MQIMPO_INQ_NEXT);

		MQLONG type = MQTYPE_AS_SET;

		MQINQMP(hcon, hmsg, &ipo, &iname,
			&mqpd, &type, buffer.size(), (MQBYTE*) buffer.raw(),
			&length, &code, &reason);

		if (code != MQCC_OK)
		{
			if (DEBUG_PROP)
			{
				GMSEC_VERBOSE << "MQINQMP: code=" << code << " reason=" << reason;
			}

			if (reason == MQRC_PROPERTY_VALUE_TOO_BIG)
			{
				requery = true;

				buffer.resize(length);

				if (buffer.get() == 0)
				{
					throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Failed to re-allocate memory for buffer");
				}
			}
			else if (reason == MQRC_PROPERTY_NOT_AVAILABLE)
			{
				if (DEBUG_PROP)
				{
					GMSEC_VERBOSE << "reached end of properties";
				}
				break;
			}
			else
			{
				throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, reason, "MQINQMP error");
			}
		}

		std::string fullname = idFromMW(ipo.ReturnedName);

		if (!fullname.length())
		{
			GMSEC_WARNING << "parseParameters: unable to get full name";
			continue;
		}

		static const std::string prefix(MW_PROP_PREFIX);
		const char* name = 0;

		if (fullname.find_first_of(prefix) == 0)
		{
			name = fullname.c_str() + prefix.length();
		}

		if (!name)
		{
			if (DEBUG_PROP)
			{
				GMSEC_VERBOSE << "parseParameters: fullname=" << fullname.c_str() << " name=(empty)";
			}
			continue;
		}

		if (DEBUG_PROP)
		{
			GMSEC_VERBOSE << "parseParameters: full=" << fullname.c_str() << " name=" << name << " length=" << length;
		}

		if (type == MQTYPE_BOOLEAN)
		{
			if (length == 4)
			{
				GMSEC_I32 value = 0;
				StringUtil::copyBytes(&value, buffer.get(), length);
				bool flag = value != 0;

				meta.setBoolean(name, flag);

				BooleanField field(name, flag);
				field.isHeader(true);

				msg->addField(field);
			}
			else
			{
				GMSEC_WARNING << "property " << name << " is boolean but length=" << length;
			}
		}
		else if (type == MQTYPE_INT32)
		{
			if (length == 4)
			{
				GMSEC_I32 value = 0;
				StringUtil::copyBytes(&value, buffer.get(), length);

				if (StringUtil::stringEquals(MW_PROP_KIND, name))
				{
					MessageBuddy::getInternal(*msg.get()).setKind(static_cast<Message::MessageKind>(value));
				}
				else
				{
					meta.setI32(name, value);

					I32Field field(name, value);
					field.isHeader(true);

					msg->addField(field);
				}
			}
			else
			{
				GMSEC_WARNING << "property " << name << " is i32 but length=" << length;
			}
		}
		else if (type == MQTYPE_FLOAT64)
		{
			if (length == 8)
			{
				GMSEC_F64 value = 0;
				StringUtil::copyBytes(&value, buffer.get(), length);

				meta.setF64(name, value);

				F64Field field(name, value);
				field.isHeader(true);

				msg->addField(field);
			}
			else
			{
				GMSEC_WARNING << "property " << name << " is f64 but length=" << length;
			}
		}
		else if (type == MQTYPE_STRING)
		{
			if (requery)
			{
				if (DEBUG_PROP)
				{
					GMSEC_VERBOSE << "requerying string parameter name=" << name;
				}
				ipo.Options = MQIMPO_INQ_PROP_UNDER_CURSOR;

				MQINQMP(hcon, hmsg, &ipo, &iname,
					&mqpd, &type, buffer.size(), (MQBYTE*) buffer.raw(),
					&length, &code, &reason);

				if (code)
				{
					GMSEC_WARNING << "MQINQMP(requery): code=" << code << " reason=" << reason;
					continue;
				}

				if ((size_t) length != buffer.size())
				{
					GMSEC_WARNING << "MQINQMP(requery:" << name << "): << length=" << length << " buffer.size=" << buffer.size();
					continue;
				}
			}

			std::string tmp((char*) buffer.raw(), length);
			const char* value = tmp.c_str();

			if (DEBUG_PROP)
			{
				GMSEC_VERBOSE << "got string name=" << name << " value=" << value;
			}

			if (StringUtil::stringEquals(MW_PROP_SUBJECT, name))
			{
				MessageBuddy::getInternal(*msg.get()).setSubject(value);
			}
			else
			{
				meta.setString(name, value);

				StringField field(name, value);
				field.isHeader(true);

				msg->addField(field);
			}
		}
		else
		{
			GMSEC_WARNING << "property " << name << " has unexpected type " << type;
		}
	}

	if (DEBUG_PROP)
	{
		GMSEC_VERBOSE << "leaving parseProperties";
	}

	return msg.release();
}


bool getMetaFromFields(Message& msg)
{
	static int warned = 0;
	if (!warned)
	{
		warned = 1;
		GMSEC_WARNING << "getMetaFromFields: received message without subject/kind in properties; check publisher version";
	}

	int ok = 0;

	const StringField* webSubject = dynamic_cast<const StringField*>(msg.getField(WEBSPHERE_SUBJECT));
	const I32Field*    webKind    = dynamic_cast<const I32Field*>(msg.getField(WEBSPHERE_KIND));

	if (webSubject)
	{
		MessageBuddy::getInternal(msg).setSubject(webSubject->getValue());

		msg.clearField(WEBSPHERE_SUBJECT);

		++ok;
	}

	if (webKind)
	{
		Message::MessageKind kind = static_cast<Message::MessageKind>(webKind->getValue());

		MessageBuddy::getInternal(msg).setKind(kind);

		msg.clearField(WEBSPHERE_KIND);

		++ok;
	}

	if (ok != 2)
	{
		GMSEC_WARNING << "getMetaFromFields: missing subject/kind";
	}

	return ok == 2;
}


void storeProperty(const Value& value, const std::string& id0, MQHCONN hcon, MQHMSG hmsg)
{
	std::string id = idToMW(id0);

	MQLONG type;
	MQLONG valueLength;
	MQSMPO spo = { MQSMPO_DEFAULT };
	MQPD   pd  = { MQPD_DEFAULT };

	MQLONG code;
	MQLONG reason;

	const void* pvalue = 0;
	GMSEC_I32 iv;
	GMSEC_F64 fv;
	std::string sv;

	if (value.isString())
	{
		value.getString(sv);
		pvalue = sv.c_str();

		type = MQTYPE_STRING;
		valueLength = MQLONG(sv.length());
	}
	else if (value.isBoolean())
	{
		bool tmp = false;
		value.getBoolean(tmp);
		iv = tmp ? 1 : 0;
		pvalue = &iv;

		type = MQTYPE_BOOLEAN;
		valueLength = 4;
	}
	else if (value.isInteger())
	{
		value.getI32(iv);
		pvalue = &iv;

		type = MQTYPE_INT32;
		valueLength = 4;
	}
	else if (value.isReal())
	{
		value.getF64(fv);
		pvalue = &fv;

		type = MQTYPE_FLOAT64;
		valueLength = 8;
	}
	else
	{
		GMSEC_WARNING << "storeProperty: property " << id.c_str() << " has unexpected type";
		return;
	}

	std::string mqid = MW_PROP_PREFIX + id;

	MQCHARV mqname = { MQCHARV_DEFAULT };
	toMQString(mqname, mqid.c_str());

	if (DEBUG_PROP)
	{
		GMSEC_VERBOSE << "storing property " << id.c_str() << " with value length=" << valueLength;
	}
	MQSETMP(hcon, hmsg, &spo, &mqname, &pd, type, valueLength, (MQBYTE*) pvalue, &code, &reason);

	if (code != MQCC_OK)
	{
		GMSEC_WARNING << "storeProperty: property " << id.c_str() << " failed reason=" << reason;
	}
}


void storeProperties(const ValueMap& in, MQHCONN hcon, MQHMSG hmsg)
{
	ValueMap::Iterator i;
	in.reset(i);

	const Value* v = NULL;

	while ((v = i.next()) != NULL)
	{
		storeProperty(*v, i.getID(), hcon, hmsg);
	}
}

} // end namespace gmsec_websphere
