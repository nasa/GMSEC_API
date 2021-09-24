/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** @file AMQPConnection.cpp
 *  @brief This file provides functionality for implementing an AMQP wrapper.
 */

#include <AMQPConnection.h>

#include <gmsec_amqp.h>

#include <gmsec4/internal/FileUtil.h>
#include <gmsec4/internal/InternalConnection.h>
#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/Rawbuf.h>
#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/SystemUtil.h>

#include <gmsec4/util/Log.h>

#include <proton/error.h>
#include <proton/message.h>
#include <proton/session.h>
#include <proton/connection.h>
#include <proton/sasl.h>
#include <proton/delivery.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <cstring>   // for strlen()


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;


// Constants
#define TOPIC_PREFIX    ""
#define AMQP_REPLY      "AMQP_REPLY_ADDR"


namespace gmsec_amqp
{


static Status storeProperties(ValueMap& header, pn_data_t* properties);
static void   parseProperties(Message& message, ValueMap& meta, pn_data_t* properties);


SubscriptionInfo::SubscriptionInfo(const AMQPSubscription& amqpSubscription,
                                   CountDownLatch* cdlatch,
                                   const SSLConfiguration& sslConfig,
                                   bool replies)
	:
	doneWithLife(false),
	connected(false),
	deathLatch(new CountDownLatch(1)),
	amqpSub(amqpSubscription),
	threadMessenger(0),
	isReply(replies),
	listeningLatch(cdlatch),
	ssl(sslConfig)
{
}


SubscriptionInfo::~SubscriptionInfo()
{
	this->shutdown();
}


bool SubscriptionInfo::initSubscriptionWithConn()
{
	// Initialize messenger and incoming window
	threadMessenger = pn_messenger(NULL);

	int amqpResult = pn_messenger_set_incoming_window(threadMessenger, -1);

	if (amqpResult != 0)
	{
		GMSEC_WARNING << "Unable to set incoming window";
		return false;
	}

	pn_messenger_set_trusted_certificates(threadMessenger, ssl.tStore.c_str());
	pn_messenger_set_certificate(threadMessenger, ssl.cert.c_str());
	pn_messenger_set_private_key(threadMessenger, ssl.pKey.c_str());
	pn_messenger_set_password(threadMessenger, ssl.kPass.c_str());

	pn_messenger_set_timeout(threadMessenger, 500);

	// Set messenger's subscription (does not connect to server at this point)
	pn_messenger_subscribe(threadMessenger, amqpSub.addr.c_str());

	bool subscribed = false;
	int  tries = 3;

	for (int i = 0; i < tries && !subscribed; ++i)
	{
		pn_messenger_recv(threadMessenger, -1);

		amqpSub.conn->publishTestMessage(amqpSub.subject.c_str());

		amqpResult = pn_messenger_work(threadMessenger, 1000);

		if (amqpResult == 0)
		{
			GMSEC_WARNING << "Subscription test message not received" << (i < tries ? "; will try again..." : "");
		}
		else if (amqpResult < 0)
		{
			GMSEC_ERROR << "Error receiving subscription test message [" << amqpResult << "]" << (i < tries ? "; will try again..." : "");
		}
		else if (pn_messenger_incoming(threadMessenger) > 0)
		{
			GMSEC_VERBOSE << "Subscribed to: " << amqpSub.subject.c_str();
			subscribed = true;
		}
		else
		{
			GMSEC_WARNING << "No incoming message in queue" << (i < tries ? "; will try again..." : "");
		}
	}

	if (!subscribed)
	{
		GMSEC_WARNING << "Failed to subscribe to: " << amqpSub.subject.c_str();
	}
	
	return subscribed;
}


void SubscriptionInfo::run()
{
	pn_tracker_t tracker;

	// Initialize subscription
	if (initSubscriptionWithConn())
	{
		AutoMutex hold(amqpSub.conn->subscribeCondition.getMutex());
		amqpSub.conn->subscribeCondition.signal(AMQPConnection::LISTENING);
		connected = true;
	}
	else
	{
		AutoMutex hold(amqpSub.conn->subscribeCondition.getMutex());
		amqpSub.conn->subscribeCondition.signal(AMQPConnection::UNABLE_TO_SUBSCRIBE);
		doneWithLife = true;
	}
	
	while (!doneWithLife)
	{
		// Avoid starving other threads
		StdThread::yield();
		
		// Receive any messages from threadMessenger's queue
		int amqpResult = pn_messenger_work(threadMessenger, -1);

		if (amqpResult < 0 && amqpResult != PN_INTR)
		{
			AutoMutex hold(amqpSub.conn->queueCondition.getMutex());
			amqpSub.conn->queueCondition.signal(AMQPConnection::MESSENGER_ERROR);
			deathLatch->countDown();
			break;
		}

		// For all messages on queue, call handleMessage
		while (pn_messenger_incoming(threadMessenger))
		{
			pn_message_t *message = pn_message();

			amqpResult = pn_messenger_get(threadMessenger, message);

			if (amqpResult && amqpResult != PN_INTR)
			{
				AutoMutex hold(amqpSub.conn->queueCondition.getMutex());
				amqpSub.conn->queueCondition.signal(AMQPConnection::COULDNT_GET_MESSAGE);
				deathLatch->countDown();
				break;
			}

			tracker = pn_messenger_incoming_tracker(threadMessenger);
			amqpSub.conn->handleMessage(message, isReply);
			pn_messenger_accept(threadMessenger, tracker, 0);

			// Clean up the proton message once it is no longer needed
			pn_message_free(message);
		}
	}

	deathLatch->countDown();
}


void SubscriptionInfo::shutdown()
{
	if (connected)
	{
		doneWithLife = true;

		pn_messenger_interrupt(threadMessenger);

		deathLatch->await();

		pn_messenger_stop(threadMessenger);
		pn_messenger_free(threadMessenger);

		if (deathLatch)
		{
			delete deathLatch;
			deathLatch = 0;
		}

		if (listeningLatch)
		{
			delete listeningLatch;
			listeningLatch = 0;
		}

		connected = false;
	}
}


static Status storeProperty(const Value& value, const std::string& name, pn_data_t* properties, std::string& types, int num)
{
	Status      status;
	std::string type;

	if (value.isString())
	{
		std::string x;
		status = value.getString(x);
		if (!status.isError())
		{
			pn_data_put_string(properties, pn_bytes(strlen(name.c_str()), name.c_str()));
			pn_data_put_string(properties, pn_bytes(strlen(x.c_str()), x.c_str()));
		}
	}
	else if (value.isInteger())
	{
		GMSEC_I32 x = 0;
		status = value.getI32(x);
		if (!status.isError())
		{
			pn_data_put_string(properties, pn_bytes(strlen(name.c_str()), name.c_str()));
			pn_data_put_int(properties, x);
		}
	}
	else if (value.isBoolean())
	{
		bool x = false;
		status = value.getBoolean(x);
		if (!status.isError())
		{
			pn_data_put_string(properties, pn_bytes(strlen(name.c_str()), name.c_str()));
			pn_data_put_bool(properties, x);
		}
	}
	else if (value.isReal())
	{
		GMSEC_F64 x = 0;
		status = value.getF64(x);
		if (!status.isError())
		{
			pn_data_put_string(properties, pn_bytes(strlen(name.c_str()), name.c_str()));
			pn_data_put_float(properties, x);
		}
	}
	else
	{
		status.set(FIELD_ERROR, OTHER_ERROR_CODE, "Unable to convert type to AMQP property");
	}

	if (!status.isError() && (type.length() > 0))
	{
		if (types.length() > 0)
		{
			types += ',';
		}
		types += name + '=' + type;
	}

	return status;
}


static Status storeProperties(ValueMap& meta, pn_data_t* properties)
{
	Status status;
	
	ValueMap::Iterator i;
	meta.reset(i);
	
	int numEntries = 0;
	std::string types;
	
	pn_data_enter(properties);
	while (!status.isError())
	{
		const Value* value = i.next();
		if (!value) break;
		status = storeProperty(*value, i.getID(), properties, types, numEntries++);
	}
	pn_data_exit(properties);
	
	return status;
}


static void parseProperties(Message& message, ValueMap& meta, pn_data_t* properties)
{
	std::string subject;
	
	pn_data_next(properties);

	int numEntries = pn_data_get_map(properties) / 2;

	pn_data_enter(properties);

	for (int i = 0; i < numEntries; ++i)
	{
		pn_data_next(properties);

		const char* name = (const char*) pn_data_get_string(properties).start;

		pn_data_next(properties);
		
		if (!strcmp(name, "SUBJECT"))
		{
		}
		else if (!strcmp(name, "KIND"))
		{
		}
		else
		{
			StdUniquePtr<Field> field;

			if (pn_data_type(properties) == PN_STRING)
			{
				const char* value = (const char*) pn_data_get_string(properties).start;

				meta.setString(name, value);

				field.reset(new StringField(name, value));
			}
			else if (pn_data_type(properties) == PN_INT)
			{
				GMSEC_I32 value = pn_data_get_int(properties);

				meta.setI32(name, value);

				field.reset(new I32Field(name, value));
			}
			else if (pn_data_type(properties) == PN_BOOL)
			{
				GMSEC_BOOL value = (GMSEC_BOOL) pn_data_get_bool(properties);

				meta.setBoolean(name, value);

				field.reset(new BooleanField(name, value == GMSEC_TRUE));
			}
			else if (pn_data_type(properties) == PN_FLOAT)
			{
				GMSEC_F64 value = pn_data_get_float(properties);

				meta.setF64(name, value);

				field.reset(new F64Field(name, value));
			}
			else
			{
				GMSEC_WARNING << "parseProperties: unhandled type '" << (pn_type_t) pn_data_type(properties) << "'";

				const char* value = (const char*) pn_data_get_string(properties).start;

				meta.setString(name, value);

				field.reset(new StringField(name, value));
			}
		
			if (field.get())
			{
				field->isHeader(true);

				message.addField(*field.get());
			}
		}
	}

	pn_data_exit(properties);
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


AMQPConnection::AMQPConnection(const Config& cfg)
	:
	uniquecounter(0),
	requestCounter(0),
	sigMismatchFlag(false),
	sslConfig("", "", "", "")
{
	std::string filterToggle;
	
	// Try to get the needed values out of the config object.
	mwConfig(cfg, "username", username);
	mwConfig(cfg, "password", password);
	mwConfig(cfg, "server", hostname);
	mwConfig(cfg, "configFile", configFile);

	mwConfig(cfg, "truststore", sslConfig.tStore);
	mwConfig(cfg, "privateKey", sslConfig.pKey);
	mwConfig(cfg, "keyPassword", sslConfig.kPass);
	mwConfig(cfg, "certificate", sslConfig.cert);

	mwConfig(cfg, "filter-dups", filterToggle, true);
	
	useFilter = filterToggle.empty() || StringUtil::stringEqualsIgnoreCase(filterToggle.c_str(), "yes");
}


AMQPConnection::~AMQPConnection()
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

		queue.pop();

		if (msgResult->message)
		{
			delete msgResult->message;
		}

		delete msgResult;
	}
}


const char* AMQPConnection::getLibraryVersion()
{
	return AMQP_VERSION_STRING;
}


const char* AMQPConnection::getMWInfo()
{
	if (mwInfo.empty())
	{
		mwInfo = getLibraryRootName();
	}

	return mwInfo.c_str();
}


void AMQPConnection::mwConnect()
{
	// Initialize request specs
	specs = getExternal().getRequestSpecs();

	isApollo = false;

	if (!getAMQPURIConfig())
	{
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, 1, "Unable to read 'amqp.xml'");
	}

	pubMessenger = pn_messenger(NULL);

	// Test the ability to connect to the given address
	pn_messenger_t* testSubMessenger = pn_messenger(NULL);

	// Set the SSL options
	pn_messenger_set_trusted_certificates(testSubMessenger, sslConfig.tStore.c_str());
	pn_messenger_set_certificate(testSubMessenger, sslConfig.cert.c_str());
	pn_messenger_set_private_key(testSubMessenger, sslConfig.pKey.c_str());
	pn_messenger_set_password(testSubMessenger, sslConfig.kPass.c_str());

	pn_messenger_set_trusted_certificates(pubMessenger, sslConfig.tStore.c_str());
	pn_messenger_set_certificate(pubMessenger, sslConfig.cert.c_str());
	pn_messenger_set_private_key(pubMessenger, sslConfig.pKey.c_str());
	pn_messenger_set_password(pubMessenger, sslConfig.kPass.c_str());

	pn_messenger_set_blocking(testSubMessenger, false);
	std::string fullAddress = assembleAddress(address, "");
	pn_messenger_subscribe(testSubMessenger, fullAddress.c_str());
	pn_messenger_recv(testSubMessenger, -1);
	pn_messenger_set_blocking(testSubMessenger, true);

	bool confirmed = false;
	int  connTestTimeout = 0;
	for (int confTries = 0; confTries < 10 && !confirmed; ++confTries)
	{
		connTestTimeout += 50;

		pn_messenger_set_timeout(testSubMessenger, connTestTimeout);

		int amqpResult = pn_messenger_work(testSubMessenger, connTestTimeout);

		if (amqpResult != PN_TIMEOUT)
		{
			confirmed = true;
		}
	}

	pn_messenger_stop(testSubMessenger);
	pn_messenger_free(testSubMessenger);

	if (!confirmed)
	{
		pn_messenger_free(pubMessenger);

		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, 1, "Connect: Connection timed out");
	}

	// If subject mapping is turned on, set up and start the reply listening thread
	if (specs.requestReplyEnabled && specs.useSubjectMapping)
	{
		fullAddress = assembleAddress(address, fixSubject(specs.replySubject.c_str()).c_str());

		AMQPSubscription listeningSubscription(fullAddress, specs.replySubject, this);

		if (!requestThread.get())
		{
			SharedSubscriptionInfo reqSubInfo(new SubscriptionInfo(listeningSubscription, new CountDownLatch(1), sslConfig, true));

			requestThread.reset(new StdThread(&runSubscriptionThread, reqSubInfo));

			AutoMutex subMutex(subscribeCondition.getMutex());

			GMSEC_DEBUG << "[" << this << "] is starting the Request Subscription Thread";

			requestThread->start();

			int reason = subscribeCondition.wait(10 * 1000);

			if (reason != AMQPConnection::LISTENING)
			{
				throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, AMQPConnection::UNABLE_TO_SUBSCRIBE, "Unable to start reply listener");
			}

			subscriptions[specs.replySubject] = reqSubInfo;
		}
	}

	getExternal().setConnectionEndpoint(hostname);
}


void AMQPConnection::mwDisconnect()
{
	pn_messenger_free(pubMessenger);

	for (Subscriptions::iterator it = subscriptions.begin(); it != subscriptions.end(); ++it)
	{
		SharedSubscriptionInfo subInfo = it->second;

		subInfo->shutdown();

		subInfo.reset();
	}
	subscriptions.clear();
}


void AMQPConnection::mwSubscribe(const char* subject, const Config& config)
{
	if (subscriptions.count(fixSubject(subject)) > 0)
	{
		return;
	}

	bool allowReplies = false;
	if (!specs.useSubjectMapping)
	{
		allowReplies = specs.legacy || specs.exposeReplies;
	}

	bool ableToSubscribe = true;

	// Start the subscription thread
	const std::string fullAddress = assembleAddress(address, fixSubject(subject).c_str());

	AMQPSubscription amqpSubscription(fullAddress, subject, this);

	SharedSubscriptionInfo subInfo(new SubscriptionInfo(amqpSubscription, new CountDownLatch(1), sslConfig, allowReplies));

	SubscriptionThread subThread(new StdThread(&runSubscriptionThread, subInfo));

	AutoMutex subMutex(subscribeCondition.getMutex());

	subThread->start();
		
	// Allow subscription thread to initialize before proceeding
	if (subscribeCondition.wait(5000) != AMQPConnection::LISTENING)
	{
		ableToSubscribe = false;
	}
	else
	{
		subscriptions[fixSubject(subject)] = subInfo;
		GMSEC_DEBUG << "[Subscribed successfully: " << subject << "]";
	}

	if (!ableToSubscribe)
	{
		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, AMQPConnection::UNABLE_TO_SUBSCRIBE, "Unable to subscribe");
	}
}


void AMQPConnection::mwUnsubscribe(const char* subject)
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


void AMQPConnection::mwPublish(const Message& msg, const Config& config)
{
	AutoTicket lock(m_mutex);

	int               amqpResult;
	std::string       fullAddress;

	pn_message_t*     amqpMsg = pn_message();
	pn_data_t*        body;
	pn_data_t*        properties;
	pn_tracker_t      tracker;
	pn_delivery_tag_t dtg;
	ValueMap          meta;

	// Get/Set message kind
	Message::MessageKind kind = msg.getKind();
	meta.setI32("KIND", kind);
	amqpResult = pn_message_set_content_type(amqpMsg, (const char*) (&kind));
	if (amqpResult)
	{
		pn_message_free(amqpMsg);

		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, amqpResult, "Problem setting message kind in AMQP message");
	}

	// Get/Set subject
	const char* subject = msg.getSubject();
	meta.setString("SUBJECT", subject);
	amqpResult = pn_message_set_subject(amqpMsg, subject);
	if (amqpResult)
	{
		pn_message_free(amqpMsg);

		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, amqpResult, "Problem setting subject in AMQP message");
	}

	// Add subject to address and set as message address
	fullAddress = assembleAddress(address, fixSubject(subject).c_str());
	amqpResult = pn_message_set_address(amqpMsg, fullAddress.c_str());
	if (amqpResult)
	{
		pn_message_free(amqpMsg);

		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, amqpResult, "Problem setting AMQP address in AMQP message");
	}

	// Get/Set message body
	DataBuffer buffer;

	Status result = getExternal().getPolicy().package(const_cast<Message&>(msg), buffer, meta);

	if (result.isError())
	{
		pn_message_free(amqpMsg);

		throw Exception(result);
	}

	body = pn_message_body(amqpMsg);

	if (isApollo)
	{
		amqpResult = pn_data_put_binary(body, pn_bytes(buffer.size(), (const char*) buffer.get()));
	}
	else
	{
		amqpResult = pn_data_put_string(body, pn_bytes(buffer.size(), (const char*) buffer.get()));
	}
	if (amqpResult)
	{
		pn_message_free(amqpMsg);

		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, amqpResult, "Problem converting GMSEC data to AMQP data");
	}

	pn_messenger_start(pubMessenger);

	// Get/Set message properties
	properties = pn_message_properties(amqpMsg);
	pn_data_put_map(properties);

	result = storeProperties(meta, properties);
	if (result.isError())
	{
		pn_message_free(amqpMsg);

		throw Exception(result);
	}

	amqpResult = pn_messenger_put(pubMessenger, amqpMsg);
	if (amqpResult)
	{
		pn_message_free(amqpMsg);

		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, amqpResult, "Problem storing AMQP data");
	}

	tracker = pn_messenger_outgoing_tracker(pubMessenger);
	dtg = pn_delivery_tag(pn_messenger_delivery(pubMessenger, tracker));

	// Send
	amqpResult = pn_messenger_send(pubMessenger, -1);
	if (amqpResult)
	{
		pn_message_free(amqpMsg);

		throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, amqpResult, "Problem sending AMQP data");
	}

	pn_message_free(amqpMsg);
	pn_messenger_stop(pubMessenger);
}


void AMQPConnection::mwRequest(const Message& request, std::string& id)
{
	{
		AutoTicket lock(m_mutex);
		id = generateUniqueId(++requestCounter);
	}

	MessageBuddy::getInternal(request).addField(GMSEC_REPLY_UNIQUE_ID_FIELD, id.c_str());

	if (specs.useSubjectMapping)
	{
		MessageBuddy::getInternal(request).addField(AMQP_REPLY, specs.replySubject.c_str());
	}
	else
	{
		MessageBuddy::getInternal(request).getDetails().setBoolean(GMSEC_REQ_RESP_BEHAVIOR, true);
	}

	// Send request for reply (subscription thread was started on connection)
	mwPublish(request, getExternal().getConfig());

	GMSEC_DEBUG << "[Request sent successfully: " << request.getSubject() << "]";
}


void AMQPConnection::mwReply(const Message& request, const Message& reply)
{
	// Get the Request's Unique ID, and put it into a field in the Reply
	std::string        uniqueID  = getExternal().getReplyUniqueID(request);
	const StringField* replyAddr = dynamic_cast<const StringField*>(request.getField(AMQP_REPLY));

	if (uniqueID.empty())
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Request does not contain unique ID field");
	}
	if (replyAddr == NULL)
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Request does not contain reply address field");
	}

	MessageBuddy::getInternal(reply).addField(GMSEC_REPLY_UNIQUE_ID_FIELD, uniqueID.c_str());

	MessageBuddy::getInternal(reply).setSubject(replyAddr->getValue());

	// Publish the reply
	mwPublish(reply, getExternal().getConfig());

	MessageBuddy::getInternal(reply).clearField(GMSEC_REPLY_UNIQUE_ID_FIELD);

	GMSEC_DEBUG << "[Reply sent successfully: " << reply.getSubject() << "]";
}


void AMQPConnection::mwReceive(Message*& msg, GMSEC_I32 timeout)
{
	double start_s;
	bool   done = false;
	bool   first = true;

	msg = NULL;

	// Start counter if there is a timeout
	if (timeout != GMSEC_WAIT_FOREVER)
	{
		start_s = TimeUtil::getCurrentTime_s();
	}

	// Until "done" condition is set:
	while (!done)
	{
		int reason = 0;

		// If the queue is empty, either return timeout or wait. Otherwise, set "reason" flag.
		if (queue.empty())
		{
			if (timeout == GMSEC_WAIT_FOREVER)
			{
				AutoMutex inner(queueCondition.getMutex());
				reason = queueCondition.wait();
			}
			else if (timeout == 0)
			{
				done = true;
			}
			else
			{
				// 0 < timeout < ∞
				double remaining_ms = timeout - 1000 * (TimeUtil::getCurrentTime_s() - start_s);

				if (first || remaining_ms > 0)
				{
					int wait_ms = int(remaining_ms);
					if (wait_ms < 1)
					{
						wait_ms = 1;
					}
					AutoMutex inner(queueCondition.getMutex());
					reason = queueCondition.wait(wait_ms);
				}
				else
				{
					// timeout (do not treat as error)
					done = true;
					break;
				}
			}
		}
		else
		{
			reason = GOT_MESSAGE;
		}

		if (sigMismatchFlag)
		{
			sigMismatchFlag = false;
			continue;
		}

		// If a message is available:
		if (reason == GOT_MESSAGE)
		{
			if (!queue.empty())
			{
				// Read it off of the queue and check the result for errors
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
					const StringField* id = dynamic_cast<const StringField*>(msg->getField("UNIQUE-ID"));

					if (useFilter && id != NULL)
					{
						if (uniqueFilter.update(id->getValue()))
						{
							// Found a unique message
							done = true;
						}
					}
					else
					{
						// Either tracking is turned off on the publisher (No UNIQUE-ID field),
						// or filtering is turned off on the subscriber, so return the message.
						done = true;
					}

					// Check subject mapping and handle accordingly.
					if (!specs.useSubjectMapping)
					{
						ValueMap& meta = MessageBuddy::getInternal(*msg).getDetails();
						
						meta.setBoolean(GMSEC_REQ_RESP_BEHAVIOR, true);
						
						if (msg->getKind() == Message::REQUEST)
						{
							const StringField* uniqueID = dynamic_cast<const StringField*>(msg->getField(GMSEC_REPLY_UNIQUE_ID_FIELD));

							if (uniqueID != NULL)
							{
								meta.setString(GMSEC_REPLY_UNIQUE_ID_FIELD, uniqueID->getValue());
								
								// Remove GMSEC_REPLY_UNIQUE_ID_FIELD from the message
								msg->clearField(GMSEC_REPLY_UNIQUE_ID_FIELD);
							}
						}
					}
				}
				else
				{
					done = true;
				}
			}
		}
		else if (reason == MESSENGER_ERROR)
		{
			throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, AMQPConnection::MESSENGER_ERROR, "Messenger Error");
		}
		else if (reason == COULDNT_GET_MESSAGE)
		{
			throw Exception(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, AMQPConnection::COULDNT_GET_MESSAGE, "Could not retrieve message");
		}

		first = false;
	}
}


// Make GMSEC-compliant string into AMQP-compliant string
std::string AMQPConnection::fixSubject(const char* str)
{
	std::string subject = str;
	size_t i = 1;

	// Replace * with appropriate mapping
	if (wcard1 != "*")
	{
		while (true)
		{
			i = subject.find("*", i);
			if (i != std::string::npos)
			{
				subject.replace(i, 1, wcard1);
			}
			else
			{
				break;
			}
		}
	}

	i = 1;
	// Replace > with appropriate mapping
	if (wcard2 != ">")
	{
		while (true)
		{
			i = subject.find(">", i);
			if (i != std::string::npos)
			{
				subject.replace(i, 1, wcard2);
			}
			else
			{
				break;
			}
		}
	}

	i = 1;
	// Replace + with appropriate mapping
	if (wcard3 != "+")
	{
		while (true)
		{
			i = subject.find("+", i);
			if (i != std::string::npos)
			{
				subject.replace(i, 1, wcard3);
			}
			else
			{
				break;
			}
		}
	}

	i = 1;
	// Replace / with appropriate mapping
	if (wcard4 != "/")
	{
		while (true)
		{
			i = subject.find("/", i);
			if (i != std::string::npos)
			{
				subject.replace(i, 1, wcard4);
			}
			else
			{
				break;
			}
		}
	}

	return subject.c_str();
}


// Make AMQP-compliant string into GMSEC-compliant string
std::string AMQPConnection::unfixSubject(const char* str)
{
	std::string subject;

	if (!str)
	{
		return subject;
	}

	subject = str;
	size_t i = 1;

	// Replace appropriate mapping with *
	if (wcard1 != "*")
	{
		while (true)
		{
			i = subject.find(wcard1, i);
			if (i != std::string::npos)
			{
				subject.replace(i, wcard1.length(), "*");
			}
			else
			{
				break;
			}
		}
	}

	i = 1;
	// Replace appropriate mapping with >
	if (wcard2 != ">")
	{
		while (true)
		{
			i = subject.find(wcard2, i);
			if (i != std::string::npos)
			{
				subject.replace(i, wcard2.length(), ">");
			}
			else
			{
				break;
			}
		}
	}
	
	i = 1;
	// Replace appropriate mapping with +
	if (wcard3 != "+")
	{
		while (true)
		{
			i = subject.find(wcard3, i);
			if (i != std::string::npos)
			{
				subject.replace(i, wcard3.length(), "+");
			}
			else
			{
				break;
			}
		}
	}
	
	i = 1;
	// Replace appropriate mapping with /
	if (wcard4 != "/")
	{
		while (true)
		{
			i = subject.find(wcard4, i);
			if (i != std::string::npos)
			{
				subject.replace(i, wcard4.length(), "/");
			}
			else
			{
				break;
			}
		}
	}

	return subject;
}


void AMQPConnection::handleMessage(pn_message_t* message, bool replies)
{
	StdUniquePtr<Message> gmsecMessage;
	pn_bytes_t            bodyBytes;
	pn_data_t*            body;
	pn_data_t*            properties;
	const char*           subject;
	bool                  enqueue = true;
	ValueMap              meta;

	// Get message subject, body and properties
	subject = pn_message_get_subject(message);
	body    = pn_message_body(message);

	pn_data_next(body);

	// Apollo requires a binary entry, while RabbitMQ requires a string
	if (isApollo)
	{
		bodyBytes = pn_data_get_binary(body);
	}
	else
	{
		bodyBytes = pn_data_get_string(body);
	}

	// Put message contents into a GMSEC message
	DataBuffer buffer((GMSEC_U8 *) bodyBytes.start, bodyBytes.size, false);

	Message::MessageKind kind = static_cast<Message::MessageKind>(*pn_message_get_content_type(message));
	gmsecMessage.reset(new Message(subject, kind, getExternal().getMessageConfig()));

	properties = pn_message_properties(message);
	parseProperties(*gmsecMessage.get(), meta, properties);

	Status result = getExternal().getPolicy().unpackage(*gmsecMessage.get(), buffer, meta);

	if (result.isError())
	{
		if (result.getCode() == INVALID_SIGNATURE)
		{
			sigMismatchFlag = true;
			AutoMutex hold(queueCondition.getMutex());
			queueCondition.signal(GOT_MESSAGE);
		}

		GMSEC_WARNING << "Unable to unpackage message";
		enqueueResult(result, NULL);
		return;
	}

	if (gmsecMessage->getField("TESTIDENTIFIER"))
	{
		// We received our test message; no need to process it further.
		return;
	}

	// Check to see if is type reply and whether or not to enqueue message/result
	if (gmsecMessage->getKind() == Message::REPLY)
	{
		if (specs.legacy)
		{
			enqueue = !replies;
		}
		else
		{
			enqueue = specs.exposeReplies;
		}

		Message* tmp = gmsecMessage.get();
		getExternal().updateReplySubject(tmp);

		if (replies)
		{
			Message* reply = gmsecMessage.release();

			if (reply)
			{
				if (enqueue)
				{
					gmsecMessage.reset(new Message(*reply));
				}

				getExternal().onReply(reply);
			}
		}
	}
	
	if (enqueue)
	{
		enqueueResult(result, (result.isError() ? 0 : gmsecMessage.release()));
	}
}


void AMQPConnection::enqueueResult(const Status& status, Message* message)
{
	AutoMutex hold(queueCondition.getMutex());
	queue.push(new MsgSubscriptionResult(status, message));
	queueCondition.signal(GOT_MESSAGE);
}


bool AMQPConnection::getAMQPURIConfig()
{
	// Get complete AMQP config file path
	std::string sharedObjPath;
	if (!FileUtil::getCurrentSharedObjectPath(sharedObjPath))
	{
		return false;
	}

	size_t pos = sharedObjPath.rfind(FileUtil::PATH_SEPARATOR);

	std::string amqpXMLConfig = configFile.size() > 0 ? configFile : sharedObjPath.substr(0, pos) + FileUtil::PATH_SEPARATOR + "amqp" + FileUtil::PATH_SEPARATOR + "amqp.xml";

	GMSEC_DEBUG << "AMQP config file is at: " << amqpXMLConfig.c_str();

	Config amqpConfig;

	std::ifstream file(amqpXMLConfig.c_str());

	if (file)
	{
		std::ostringstream amqpXML;
		amqpXML << file.rdbuf();
	
		amqpConfig.fromXML(amqpXML.str().c_str());
	}
	else
	{
		return false;
	}

	// Get AMQP configuration values
	mwConfig(amqpConfig, "URI_SCHEME", uriScheme);
	mwConfig(amqpConfig, "VHOST", vhost);
	mwConfig(amqpConfig, "WCARD1", wcard1);
	mwConfig(amqpConfig, "WCARD2", wcard2);
	mwConfig(amqpConfig, "WCARD3", wcard3);
	mwConfig(amqpConfig, "WCARD4", wcard4);

	// Fill in defaults
	if (uriScheme == "DEFAULT_RMQ")
	{
		mwConfig(amqpConfig, "DEFAULT_RMQ", uriScheme);
	}
	else if (uriScheme == "DEFAULT_RMQ_SSL")
	{
		mwConfig(amqpConfig, "DEFAULT_RMQ_SSL", uriScheme);
	}
	else if (uriScheme == "DEFAULT_QPID")
	{
		mwConfig(amqpConfig, "DEFAULT_QPID", uriScheme);
	}
	else if (uriScheme == "DEFAULT_HQ")
	{
		mwConfig(amqpConfig, "DEFAULT_HQ", uriScheme);
	}
	else if (uriScheme == "DEFAULT_APOLLO")
	{
		mwConfig(amqpConfig, "DEFAULT_APOLLO", uriScheme);
		isApollo = true;
	}
	else if (uriScheme == "DEFAULT_APOLLO_SSL")
	{
		mwConfig(amqpConfig, "DEFAULT_APOLLO_SSL", uriScheme);
		isApollo = true;
	}

	if (wcard1 == "DEFAULT_WCARD1")
	{
		mwConfig(amqpConfig, "DEFAULT_WCARD1", wcard1);
	}

	if (wcard2 == "DEFAULT_RMQ_WCARD2")
	{
		mwConfig(amqpConfig, "DEFAULT_RMQ_WCARD2", wcard2);
	}
	else if (wcard2 == "DEFAULT_APOLLO_WCARD2")
	{
		mwConfig(amqpConfig, "DEFAULT_APOLLO_WCARD2", wcard2);
	}
	else if (wcard2 == "DEFAULT_QPID_WCARD2")
	{
		mwConfig(amqpConfig, "DEFAULT_QPID_WCARD2", wcard2);
	}

	if (wcard3 == "DEFAULT_RMQ_WCARD3")
	{
		mwConfig(amqpConfig, "DEFAULT_RMQ_WCARD3", wcard3);
	}
	else if (wcard3 == "DEFAULT_APOLLO_WCARD3")
	{
		mwConfig(amqpConfig, "DEFAULT_APOLLO_WCARD3", wcard3);
	}
	else if (wcard3 == "DEFAULT_QPID_WCARD3")
	{
		mwConfig(amqpConfig, "DEFAULT_QPID_WCARD3", wcard3);
	}

	if (wcard4 == "DEFAULT_RMQ_WCARD4")
	{
		mwConfig(amqpConfig, "DEFAULT_RMQ_WCARD4", wcard4);
	}
	else if (wcard4 == "DEFAULT_APOLLO_WCARD4")
	{
		mwConfig(amqpConfig, "DEFAULT_APOLLO_WCARD4", wcard4);
	}
	else if (wcard4 == "DEFAULT_QPID_WCARD4")
	{
		mwConfig(amqpConfig, "DEFAULT_QPID_WCARD4", wcard4);
	}

	// FOR DEBUG PURPOSES ONLY
	address = hostname.c_str();
	if (isApollo)
	{
		address = strlen(address) ? address : (const char *) "127.0.0.1:61613";
	}
	else
	{
		address = strlen(address) ? address : (const char *) "127.0.0.1:5672";
	}

	size_t i = 1;
	while (true)
	{
		i = uriScheme.find("#socket#", i);
		if (i != std::string::npos)
		{
			uriScheme.replace(i, 8, address);
		}
		else
		{
			break;
		}
	}

	i = 1;
	while (true)
	{
		i = uriScheme.find("#vhost#", i);
		if (i != std::string::npos)
		{
			uriScheme.replace(i, 7, vhost);
		}
		else
		{
			break;
		}
	}

	i = 1;
	while (true)
	{
		i = uriScheme.find("#username#", i);
		if (i != std::string::npos)
		{
			uriScheme.replace(i, 10, username);
		}
		else
		{
			break;
		}
	}

	i = 1;
	while (true)
	{
		i = uriScheme.find("#password#", i);
		if (i != std::string::npos)
		{
			uriScheme.replace(i, 10, password);
		}
		else
		{
			break;
		}
	}

	if (username == "" && password == "")
	{
		i = 1;
		while (true)
		{
			i = uriScheme.find(":@", i);
			if (i != std::string::npos)
			{
				uriScheme.replace(i, 2, "");
			}
			else
			{
				break;
			}
		}
	}

	return true;
}


std::string AMQPConnection::assembleAddress(const char* address, const char* subject)
{
	// Fill in URI sequence with provided variables
	std::string fullAddress = uriScheme.c_str();
	size_t i = 1;
	while (true)
	{
		i = fullAddress.find("#subject#", i);
		if (i != std::string::npos)
		{
			fullAddress.replace(i, 9, subject);
		}
		else
		{
			break;
		}
	}
	
	return fullAddress;
}


void AMQPConnection::publishTestMessage(const char* subject0)
{
	if (subject0 == NULL || std::string(subject0).empty())
	{
		return;
	}

	// TODO: Why are we using a subscription topic for a message subject?
	//       Now we have to convert the topic, which may contain wildcards,
	//       to a legal message subject using dubious code.
	std::string subject(subject0);

	const char* wc[] = { "*", ">", "+" };
	std::vector<std::string> wildcards( wc, std::end(wc) );

	for (std::vector<std::string>::const_iterator it = wildcards.begin(); it != wildcards.end(); ++it)
	{
		size_t i = 0;

		while (i != std::string::npos)
		{
			i = subject.find(*it, i);

			if (i != std::string::npos)
			{
				subject.replace(i, 1, "INITTEST");
			}
		}
	}

	Message msg(subject.c_str(), Message::PUBLISH);
	msg.addField("TESTIDENTIFIER", "CONNECTIONTEST");

	mwPublish(msg, getExternal().getConfig());
}


std::string AMQPConnection::generateUniqueId(long id)
{
	std::ostringstream strm;
	strm << getExternal().getID() << "_" << SystemUtil::getProcessID() << "_" << ++uniquecounter << "_" << id;
	std::string topic = TOPIC_PREFIX;
	topic.append(strm.str());
	return topic;
}


void AMQPConnection::runSubscriptionThread(SharedSubscriptionInfo shared)
{
	shared->run();
}

} // end namespace gmsec_amqp
