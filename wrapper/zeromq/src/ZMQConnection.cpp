/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "ZMQConnection.h"

#include "ZMQEnums.h"
#include "ZMQMessageListener.h"

#include <gmsec4/internal/InternalConnection.h>
#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/SystemUtil.h>

#include <gmsec4/Config.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/TimeUtil.h>


#include <netdb.h>     // For getaddrinfo() in hostnameToIpv4()
#include <arpa/inet.h> // For inet_ntoa() in hostnameToIpv4()

#include <sstream>
#include <string>
#include <stdlib.h>


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;

using namespace gmsec_zeromq;


static void mwConfig(const Config& config, const char* key, std::string& out, bool requirePrefix = false);


const char* ZEROMQ_SOCKET_SETTLETIME    = "SOCKET-SETTLETIME";
const char* ZEROMQ_SUBSCRIBE_ENDPOINT   = "MW-SUB-ENDPOINT";
const char* ZEROMQ_PUBLISH_ENDPOINT     = "MW-PUB-ENDPOINT";
const char* ZEROMQ_REPLYLISTEN_ENDPOINT = "MW-REPLISTEN-ENDPOINT";


// From internal GMSEC tests, 500ms appears to be enough time
// for the ZeroMQ socket to settle before performing network operations
const int DEFAULT_SETTLE_TIME           = 500;


ZMQConnection::ZMQConnection(const Config& config)
	:
	m_context(NULL),
	m_pubSocket(NULL),
	m_subSocket(NULL),
	m_repListenSocket(NULL),
	m_settleTime(DEFAULT_SETTLE_TIME),
	m_repListenPort(1024),
	m_mwInfo(""),
	m_requestCounter(0),
	m_uniqueCounter(0),
	m_sigMismatchFlag(false)
{
	std::string threadSafeToggle;
	std::string filterToggle;
	std::string settleTimeStr;

	// Try to get the needed values out of the config object.
	mwConfig(config, "threadSafe", threadSafeToggle);
	mwConfig(config, ZEROMQ_SUBSCRIBE_ENDPOINT, m_subEndpoint);
	mwConfig(config, ZEROMQ_PUBLISH_ENDPOINT, m_pubEndpoint);
	mwConfig(config, ZEROMQ_REPLYLISTEN_ENDPOINT, m_repListenEndpoint);
	mwConfig(config, "filter-dups", filterToggle, true);

	// ZeroMQ-specific configuration options

	// ZeroMQ socket settle time - Used to determine how long to wait between when
	// a socket connects/binds to the endpoint before performing network operations
	mwConfig(config, ZEROMQ_SOCKET_SETTLETIME, settleTimeStr);

	int tmpInt;
	if (settleTimeStr.length() > 0)
	{
		if (StringUtil::str2int(tmpInt, settleTimeStr.c_str()) == StringUtil::STR2NUM_SUCCESS)
		{
			if (tmpInt >= -1)
			{
				m_settleTime = tmpInt;
			}
			else
			{
				GMSEC_WARNING << "Ignoring invalid value for " << ZEROMQ_SOCKET_SETTLETIME << ": " << settleTimeStr.c_str();
			}
		}
		else
		{
			GMSEC_WARNING << "Ignoring invalid " << ZEROMQ_SOCKET_SETTLETIME << ": " << settleTimeStr.c_str();
		}
	}

	m_threadSafe      = (threadSafeToggle.compare("yes") == 0);
	m_useUniqueFilter = (filterToggle.empty() || filterToggle.compare("yes") == 0);

	// Ensure that the Reply Listener endpoint does not have a connect: or bind: option
	m_repListenEndpoint = getUrl(m_repListenEndpoint);
}


ZMQConnection::~ZMQConnection()
{
	// Clean up GMSEC API objects
	while (!m_msgQueue.empty())
	{
		MessageSubscriptionResult* messageResult = m_msgQueue.front();

		delete messageResult->message;
		delete messageResult;

		m_msgQueue.pop();
	}

	// Clean up ZeroMQ objects
	mwDisconnect();
}


const char* ZMQConnection::getLibraryVersion()
{
	return ZEROMQ_VERSION_STRING;
}


const char* ZMQConnection::getMWInfo()
{
	if (m_mwInfo.empty())
	{
		std::ostringstream strm;

		strm << getLibraryVersion();

		m_mwInfo = strm.str();
	}

	return m_mwInfo.c_str();
}



Status ZMQConnection::mwConnect()
{
	Status result;

	// Retrieve the RequestSpecs
	m_requestSpecs = getExternal().getRequestSpecs();

	// Set up the ZeroMQ Context object (for use in creating sockets)
	m_context = zmq_ctx_new();

	// According to the ZeroMQ documentation, if NULL was returned by the
	// zmq_ctx_new function, then it will also set an error code accessible
	// by zmq_errno().  That being said, the documentation does not have
	// any error codes defined for the function.
	if (m_context == NULL)
	{
		std::ostringstream ss;
		ss << "Unable to create a ZeroMQ Context, errorno code: " << zmq_errno();
		result.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, ss.str().c_str(), zmq_errno());
	}

	return result;
}


Status ZMQConnection::mwDisconnect()
{
	Status result;

	if (m_msgListener.get())
	{
		stopMsgListener(m_msgListenerObj);

		m_msgListener.reset();
	}

	if (m_repListener.get())
	{
		stopMsgListener(m_repListenerObj);

		m_repListener.reset();
	}

	// Clean up ZeroMQ objects
	int dontLinger = 0;
	if (m_pubSocket)
	{
		// Setting ZMQ_LINGER to flush unsent messages from the socket
		zmq_setsockopt(m_pubSocket, ZMQ_LINGER, &dontLinger, sizeof(dontLinger));
		zmq_close(m_pubSocket);
		m_pubSocket = 0;
	}

	if (m_subSocket)
	{
		zmq_setsockopt(m_subSocket, ZMQ_UNSUBSCRIBE, "", 0);
		zmq_setsockopt(m_subSocket, ZMQ_LINGER, &dontLinger, sizeof(dontLinger));
		zmq_close(m_subSocket);
		m_subSocket = 0;
	}

	if (m_repListenSocket)
	{
		zmq_setsockopt(m_subSocket, ZMQ_UNSUBSCRIBE, "", 0);
		zmq_setsockopt(m_repListenSocket, ZMQ_LINGER, &dontLinger, sizeof(dontLinger));
		zmq_close(m_repListenSocket);
		m_repListenSocket = 0;
	}

	// Clear out the active ZMQMessageListener threads
	if (m_context)
	{
		zmq_term(m_context);
		m_context = 0;
	}

	return result;
}


Status ZMQConnection::mwSubscribe(const char* subject, const Config& config)
{
	Status result;

	if (m_subSocket == NULL)
	{
		result = setupSocket(&m_subSocket, ZMQ_SUB, m_subEndpoint);

		if (result.isError())
		{
			return result;
		}

		zmq_setsockopt(m_subSocket, ZMQ_SUBSCRIBE, "", 0); // Use this to set filters on the subscription
	}

	bool allowReplies = false;
	if (!m_requestSpecs.useSubjectMapping)
	{
		allowReplies = m_requestSpecs.legacy || m_requestSpecs.exposeReplies;
	}

	if (!m_msgListener.get())
	{
		m_msgListenerObj.reset(new ZMQMessageListener(this, &m_subSocket, allowReplies));

		m_msgListener.reset(new StdThread(&runMsgListener, m_msgListenerObj));

		m_msgListener->start();
	}

	m_msgListenerObj->subscribe(subject);

	return result;
}


Status ZMQConnection::mwUnsubscribe(const char* subject)
{
	Status result;

	m_msgListenerObj->unsubscribe(subject);

	return result;
}


Status ZMQConnection::mwPublish(const Message& message, const Config& config)
{
	if (m_pubSocket == NULL)
	{
		Status result = setupSocket(&m_pubSocket, ZMQ_PUB, m_pubEndpoint);

		if (result.isError())
		{
			return result;
		}
	}

	return mwPublishAux(message, config, m_pubSocket);
}


static void my_free(void* data, void* hint)
{
	GMSEC_U8* buffer = reinterpret_cast<GMSEC_U8*>(data);

	delete [] buffer;
}


Status ZMQConnection::mwPublishAux(const Message& message, const Config& config, void* socket)
{
	Status result;
	int    zmqResult;

	// Prepare the message
	DataBuffer gmsecBuffer;
	ValueMap   meta;

	result = getExternal().getPolicy().package(const_cast<Message&>(message), gmsecBuffer, meta);

	if (result.isError())
	{
		return result;
	}

	// Add the Message Kind to the message meta data
	meta.setI32("MSG-KIND", message.getKind());

	// Encode the Message meta data
	DataBuffer metaBuffer;
	storeProperties(meta, metaBuffer);

	// Send the subject of the message as the first frame so that the subject filter can quickly
	// process whether the accept the bulk of the message or not
	zmq_msg_t subject_frame;
	zmq_msg_t meta_frame;
	zmq_msg_t message_frame;

	std::string subject       = message.getSubject();
	size_t      meta_data_sz  = metaBuffer.size();
	GMSEC_U8*   meta_data     = metaBuffer.release();
	size_t      gmsec_data_sz = gmsecBuffer.size();
	GMSEC_U8*   gmsec_data    = gmsecBuffer.release();
	void*       hint          = NULL;

	zmq_msg_init_size(&subject_frame, subject.length());

	memcpy(zmq_msg_data(&subject_frame), (void*) subject.c_str(), subject.length());     // subject frame

	zmq_msg_init_data(&meta_frame, (void*) meta_data, meta_data_sz, my_free, hint);      // meta-data frame

	zmq_msg_init_data(&message_frame, (void*) gmsec_data, gmsec_data_sz, my_free, hint); // message frame

	int retryCounter = 0;

	// Publish each frame of the message, retry if an EINTR or EAGAIN errno is encountered
	// Make an attempt to publish each frame 3 times
	do
	{
		zmqResult = zmq_msg_send(&subject_frame, socket, ZMQ_SNDMORE);
		if (zmqResult == -1 && (zmq_errno() != EINTR || zmq_errno() != EAGAIN))
		{
			result = zmqErrorToStatus("ZeroMQ publish operation failed, POSIX errno code: ", zmq_errno());
		}
	}
	while ((zmqResult == -1 && (zmq_errno() == EINTR || zmq_errno() == EAGAIN)) && retryCounter < 3);

	// Only publish the meta_frame if the subject_frame was published without error
	if (zmqResult != -1)
	{
		retryCounter = 0;
		do
		{
			zmqResult = zmq_msg_send(&meta_frame, socket, ZMQ_SNDMORE);
			if (zmqResult == -1 && (zmq_errno() != EINTR || zmq_errno() != EAGAIN))
			{
				result = zmqErrorToStatus("ZeroMQ publish operation failed, POSIX errno code: ", zmq_errno());
			}
		}
		while ((zmqResult == -1 && (zmq_errno() == EINTR || zmq_errno() == EAGAIN)) && retryCounter < 3);
	}

	// Only publish the message_frame if the meta_frame was published without error
	if (zmqResult != -1)
	{
		retryCounter = 0;
		do
		{
			zmqResult = zmq_msg_send(&message_frame, socket, 0);
			if (zmqResult == -1 && (zmq_errno() != EINTR || zmq_errno() != EAGAIN))
			{
				result = zmqErrorToStatus("ZeroMQ publish operation failed, POSIX errno code: ", zmq_errno());
			}
		}
		while ((zmqResult == -1 && (zmq_errno() == EINTR || zmq_errno() == EAGAIN)) && retryCounter < 3);
	}

	// Note: According to the ZeroMQ documentation for zmq_msg_send,
	// zmq_msg_close() does not need to be called after a successful zmq_msg_send().
	// This implies that ZeroMQ will handle deallocation of the messages.

	return result;
}


Status ZMQConnection::storeProperties(const ValueMap& meta, DataBuffer& out)
{
	Status result;

	// Determine the length of the buffer and resize it accordingly
	size_t length = 0;
	result = findLength(meta, length);

	if (result.isError())
	{
		return result;
	}

	out.resize(length);

	// Build up the meta blob
	char* currentBuffer = reinterpret_cast<char*>(out.raw());

	// Frame 0: Number of Values
	int numValues = (int) meta.size();
	Encoder::getEncoder()->putI32(&numValues, currentBuffer);

	// Frames 1..n: Each Value
	ValueMap::Iterator i;
	meta.reset(i);
	
	const Value* value = i.next();
	while (value != NULL)
	{
		result = storeProperty(i.getID().c_str(), *value, &currentBuffer);
		value = i.next();
	}

	return result;
}


Status ZMQConnection::findLength(const ValueMap& meta, size_t& length)
{
	Status result;

	// Account for the I32 which contains the number of Values in the meta object
	length = 4;

	ValueMap::Iterator itr;
	meta.reset(itr);
	
	const Value* value = itr.next();
	while (value != NULL)
	{
		// Store the size of the type ID
		length += gmsec::api::internal::GMSEC_TYPE_BYTES;

		// Store the size of the String ID
		length += gmsec::api::internal::GMSEC_LENGTH_BYTES + strlen(itr.getID().c_str()) + 1;

		if (value->isBoolean())
		{
			length += 1;
		}
		if (value->isInteger())
		{
			length += 4;
		}
		if (value->isReal())
		{
			length += 8;
		}
		if (value->isString())
		{
			std::string valueString;
			value->getString(valueString);
			size_t count = StringUtil::stringLength(valueString.c_str());
			if (count > size_t(gmsec::api::internal::GMSEC_STRING_LIMIT))
			{
				std::ostringstream statusError;
				statusError << "Excessive string length for meta value: " << itr.getID();
				result.set(CONNECTION_ERROR, ENCODING_ERROR, statusError.str().c_str());
				return result;
			}
			else
			{
				length += gmsec::api::internal::GMSEC_LENGTH_BYTES + count + 1;
			}
		}

		value = itr.next();
	}

	return result;
}


Status ZMQConnection::storeProperty(const char* valueId, const Value& value, char** currentBuffer)
{
	Status result;

	Encoder* encoder = Encoder::getEncoder();

	// Note: Code based off of MessageEncoder::encode(Field &field)
	if (value.isBoolean())
	{
		// Store the value type
		GMSEC_U16 type = Field::BOOL_TYPE;
		encoder->putU16(&type, *currentBuffer);

		// Store the name of the value
		encoder->putString(valueId, *currentBuffer);

		// Store the actual value
		bool out = false;
		value.getBoolean(out);
		GMSEC_U8 tmp = out ? 1 : 0;
		encoder->putU8(&tmp, *currentBuffer);
	}
	else if (value.isInteger())
	{
		// Store the value type
		GMSEC_U16 type = Field::I32_TYPE;
		encoder->putU16(&type, *currentBuffer);

		// Store the name of the value
		encoder->putString(valueId, *currentBuffer);

		// Store the actual value
		GMSEC_I32 out = 0;
		value.getI32(out);
		encoder->putI32(&out, *currentBuffer);
	}
	else if (value.isReal())
	{
		// Store the value type
		GMSEC_U16 type = Field::F64_TYPE;
		encoder->putU16(&type, *currentBuffer);

		// Store the name of the value
		encoder->putString(valueId, *currentBuffer);

		// Store the actual value
		GMSEC_F64 out = 0;
		value.getF64(out);
		encoder->putF64(&out, *currentBuffer);
	}
	else if (value.isString())
	{
		// Store the value type
		GMSEC_U16 type = Field::STRING_TYPE;
		encoder->putU16(&type, *currentBuffer);

		// Store the name of the value
		encoder->putString(valueId, *currentBuffer);

		// Store the actual value
		std::string valueString;
		value.getString(valueString);
		encoder->putString(valueString.c_str(), *currentBuffer);
	}

	return result;
}


Status ZMQConnection::parseProperties(ValueMap& meta, const DataBuffer& in, Message& message)
{
	Status result;

	Decoder* decoder = Decoder::getDecoder();

	const char* inputBuffer = reinterpret_cast<const char*>(in.get());

	// Determine the number of Values in the meta blob
	int numValues = 0;
	decoder->getI32(inputBuffer, &numValues);

	// Begin processing each Value out of the meta blob
	for (int i = 0; i < numValues; i++)
	{
		GMSEC_U16 type;
		decoder->getU16(inputBuffer, &type);

		const char*  valueName    = 0;
		unsigned int valueNameLen = 0;

		decoder->getString(inputBuffer, valueNameLen, valueName);

		Field*           field     = NULL;
		Field::FieldType fieldType = static_cast<Field::FieldType>(type);

		switch (fieldType)
		{
			case Field::BOOL_TYPE:
			{
				GMSEC_U8 value;
				decoder->getU8(inputBuffer, &value);

				// Create a ValueMap entry
				meta.setBoolean(valueName, value ? true: false);

				field = new BooleanField(valueName, value ? true: false);
				break;
			}
			case Field::I32_TYPE:
			{
				GMSEC_I32 value;
				decoder->getI32(inputBuffer, &value);
				meta.setI32(valueName, value);

				// No need to store the MSG-KIND as a field
				if (strcmp(valueName, "MSG-KIND") == 0)
				{
					MessageBuddy::getInternal(message).setKind(static_cast<Message::MessageKind>(value));
					continue;
				}

				field = new I32Field(valueName, value);
				break;
			}
			case Field::F64_TYPE:
			{
				GMSEC_F64 value;
				decoder->getF64(inputBuffer, &value);
				meta.setF64(valueName, value);

				field = new F64Field(valueName, value);
				break;
			}
			case Field::STRING_TYPE:
			{
				const char*  value    = 0;
				unsigned int valueLen = 0;

				decoder->getString(inputBuffer, valueLen, value);

				meta.setString(valueName, std::string(value));

				field = new StringField(valueName, value);
				break;
			}
			default:
				GMSEC_WARNING << "Unexpected meta-type decoded from input buffer: " << type;
				break;
		}

		if (field != NULL)
		{
			field->isHeader(true);

			message.addField(*field);

			delete field;
		}
	}

	return result;
}


std::string ZMQConnection::generateUniqueId(long id)
{
	std::ostringstream uniqueId;
	uniqueId << getExternal().getID() << "_" << SystemUtil::getProcessID() << "_" << ++m_uniqueCounter << "_" << id;
	return uniqueId.str();
}


Status ZMQConnection::mwRequest(const Message& request, std::string& id)
{
	Status result;

	// Only turn on the ReplyListener if Request-Reply is enabled
	if (m_requestSpecs.requestReplyEnabled)
	{
		result = initReplyListener();

		if (result.isError())
		{
			return result;
		}
	}

	id = generateUniqueId(++m_requestCounter);
	
	InternalMessage& intMsg = MessageBuddy::getInternal(request);

	intMsg.addField(REPLY_UNIQUE_ID_FIELD, id.c_str());
	
	if (m_requestSpecs.useSubjectMapping)
	{
		intMsg.addField(OPT_REPLY_STRING, m_requestSpecs.replySubject.c_str());

		// Add the reply address (endpoint) to the message
		// NOTE: If subject mapping is turned on, this is the ReplyListener endpoint
		//     If open response is enabled, this field will not be present and thus the
		//     reply will be published on the replier's publish socket
		intMsg.addField(ZEROMQ_REPLY_ADDRESS, m_replyAddress.c_str());
	}
	else
	{
		intMsg.getDetails().setBoolean(OPT_REQ_RESP, true);
	}

	// Send request for reply
	result = mwPublish(request, getExternal().getConfig());
	
	if (!result.isError())
	{
		GMSEC_DEBUG << "[Request sent successfully: " << request.getSubject() << "]";
	}
	else
	{
		GMSEC_DEBUG << "[Problem sending request]";
	}

	return result;
}


Status ZMQConnection::mwReply(const Message& request, const Message& reply)
{
	Status result;

	const StringField* uniqueID  = dynamic_cast<const StringField*>(request.getField(REPLY_UNIQUE_ID_FIELD));
	const StringField* replySubj = dynamic_cast<const StringField*>(request.getField(OPT_REPLY_STRING));
	const StringField* replyAddr = dynamic_cast<const StringField*>(request.getField(ZEROMQ_REPLY_ADDRESS));

	if (!uniqueID)
	{
		result.set(CONNECTION_ERROR, INVALID_MSG, "Request does not contain UNIQUE-ID field");
		return result;
	}
	if (!replySubj)
	{
		result.set(CONNECTION_ERROR, INVALID_MSG, "Request does not contain MW-REPLY-SUBJECT field");
		return result;
	}
	if (!replyAddr)
	{
		result.set(CONNECTION_ERROR, INVALID_MSG, "Request does not contain REPLY-ADDRESS field");
		return result;
	}

	MessageBuddy::getInternal(reply).addField(*uniqueID);
	MessageBuddy::getInternal(reply).setSubject(replySubj->getValue());

	const char* rAddr = replyAddr->getValue();

	// Set up the reply socket
	void* repSocket = NULL;
	result = setupSocket(&repSocket, ZMQ_PUB, rAddr);

	if (result.isError())
	{
		return result;
	}
	
	// Publish the reply
	result = mwPublishAux(reply, getExternal().getConfig(), repSocket);
	
	GMSEC_DEBUG << "[Reply sent successfully: " << reply.getSubject() << "]";

	// Clean up the reply socket
	zmq_close(repSocket);

	return result;
}


Status ZMQConnection::mwReceive(Message*& message, GMSEC_I32 timeout)
{
	Status result;
	double start_s;
	bool done = false, first = true;

	// Ensure that the message pointer is empty
	message = NULL;
	
	// Start counter if there is a timeout
	if (timeout != GMSEC_WAIT_FOREVER)
	{
		start_s = TimeUtil::getCurrentTime_s();
	}

	// Check the queue for messages until "done" condition is set:
	while (!done)
	{
		int reason = 0;
		
		// If the queue is empty, either return timeout or wait. Otherwise, set "reason" flag.
		if (m_msgQueue.empty())
		{
			if (timeout == GMSEC_WAIT_FOREVER)
			{
				AutoMutex inner(m_queueCondition.getMutex());
				reason = m_queueCondition.wait();
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
					AutoMutex inner(m_queueCondition.getMutex());
					reason = m_queueCondition.wait(wait_ms);
				}
				else
				{
					// timeout (not an error)
					done = true;
					break;
				}
			}
		}
		else
		{
			reason = GOT_MESSAGE;
		}

		if (m_sigMismatchFlag)
		{
			result.set(POLICY_ERROR, INVALID_SIGNATURE, "Signature mismatch");
			m_sigMismatchFlag = false;
			continue;
		}

		// If a message is available:
		if (reason == GOT_MESSAGE)
		{
			if (!m_msgQueue.empty())
			{
				// Read it off of the queue and check the result for errors
				MessageSubscriptionResult* messageResult = m_msgQueue.front();

				result = messageResult->status;

				if (!result.isError())
				{
					message = messageResult->message;

					const StringField* uniqueID = dynamic_cast<const StringField*>(message->getField("UNIQUE-ID"));

					if (m_useUniqueFilter && uniqueID)
					{
						const char* value = uniqueID->getValue();

						if (m_uniqueFilter.update(value))
						{
							// Found a unique message
							done = true;
						}
					}
					else
					{
						// Either tracking is turned off on the publisher (No UNIQUE-ID field), or filtering is turned off
						// on the subscriber, so return the message.
						done = true;
					}

					// For Request Messages: Check subject mapping and handle accordingly.
					if (!m_requestSpecs.useSubjectMapping)
					{
						ValueMap& meta = MessageBuddy::getInternal(*message).getDetails();

						meta.setBoolean(OPT_REQ_RESP, true);
						
						if (message->getKind() == Message::REQUEST)
						{
							// Extract and remove REPLY_UNIQUE_ID_FIELD from the message (if it exists)
							const StringField* idField = dynamic_cast<const StringField*>(message->getField(REPLY_UNIQUE_ID_FIELD));

							if (idField != NULL)
							{
								meta.setString(REPLY_UNIQUE_ID_FIELD, idField->getValue());

								message->clearField(REPLY_UNIQUE_ID_FIELD);
							}

							// Extract and remove the REPLY-ADDRESS field from the message (if it exists)
							const StringField* replyAddr = dynamic_cast<const StringField*>(message->getField(ZEROMQ_REPLY_ADDRESS));

							if (replyAddr != NULL)
							{
								meta.setString(ZEROMQ_REPLY_ADDRESS, replyAddr->getValue());

								message->clearField(ZEROMQ_REPLY_ADDRESS);
							}
						}
					}
				}
				else
				{
					// Will exit loop, remove item from queue and return the error message
					done = true;
				}
				
				// Remove from queue
				m_msgQueue.pop();
				delete messageResult;
			}
		}

		first = false;
	}
	
	if (result.isError())
	{
		// error already assigned
	}
	else if (message != NULL)
	{
		GMSEC_DEBUG << "[Received published message: " << message->getSubject() << "]";
	}

	return result;
}


ZMQConnection::SocketOperation ZMQConnection::getSocketOperation(const std::string& endpoint)
{
	SocketOperation socketOp = ZMQConnection::NONE;

	if (endpoint.find("bind:") != std::string::npos)
	{
		socketOp = ZMQConnection::BIND;
	}
	else if (endpoint.find("connect:") != std::string::npos)
	{
		socketOp = ZMQConnection::CONNECT;
	}

	return socketOp;
}


std::string ZMQConnection::getUrl(const std::string& endpoint)
{
	std::string url = endpoint;

	std::size_t pos = endpoint.find("bind:");
	if (pos != std::string::npos)
	{
		url = endpoint.substr(pos + 5);
	}
	else
	{
		pos = endpoint.find("connect:");
		if (pos != std::string::npos)
		{
			url = endpoint.substr(pos + 8);
		}
	}

	return url;
}


std::string ZMQConnection::getEndpointKey(const std::string& endpoint)
{
	std::string url = getUrl(endpoint);

	std::size_t pos = url.find("tcp://");
	if (pos != std::string::npos)
	{
		url = endpoint.substr(pos + 6);
	}

	return url;
}


Status ZMQConnection::setupSocket(void** socket, int type, const std::string& endpoint, bool replyListener)
{
	Status result;

	// Ensure that the type provided is a valid type
	// Note: See ZeroMQ docs or zmq.h for valid types
	if (type < ZMQ_PAIR || type > ZMQ_STREAM)
	{
		std::ostringstream errorStr;
		errorStr << "Unknown ZeroMQ Socket type: " << type << ".";
		result.set(MIDDLEWARE_ERROR, INVALID_CONNECTION, errorStr.str().c_str());
		return result;
	}

	// Construct the socket
	if (*socket == NULL)
	{
		*socket = zmq_socket(m_context, type);
	}

	// Check to see whether the user wants to bind or connect to an endpoint
	// Default operation: Connect
	SocketOperation socketOp = ZMQConnection::NONE;

	if (!replyListener)
	{
		socketOp = getSocketOperation(endpoint);
	}

	// Default Operations:
	// Publish: Bind
	// Subscribe: Connect
	// NOTE: ReplyListener will always bind, no exceptions -- This is by design to reduce implementation/maintenance
	// effort.  No need to overcomplicate the design since the other wrappers work similarly (Subscribe, tell the
	// replier where to send its reply).
	if (socketOp == ZMQConnection::BIND || (type == ZMQ_PUB && socketOp != ZMQConnection::CONNECT) || replyListener)
	{
		result = bindSocket(socket, endpoint);
	}
	else
	{
		result = connectSocket(socket, endpoint);
	}

	// Wait for m_settleTime to give the socket enough time to settle
	// before performing any network-related activities
	TimeUtil::millisleep(m_settleTime);

	return result;
}


Status ZMQConnection::bindSocket(void** socket, const std::string& endpointList)
{
	Status result;

	std::istringstream urlSplitter(getUrl(endpointList));
	std::string endpoint;

	while (getline(urlSplitter, endpoint, ';'))
	{
		int zmqResult = zmq_bind(*socket, getUrl(endpoint).c_str()); 

		if (zmqResult == -1)
		{	
			result = zmqErrorToStatus("ZeroMQ socket bind operation failed for endpoint " + endpoint + ", POSIX errno code: ", zmq_errno());
			// Immediately return if any of the URLs had an issue binding
			return result;
		}
	}

	return result;
}


Status ZMQConnection::connectSocket(void** socket, const std::string& endpointList)
{
	Status result;

	std::istringstream urlSplitter(getUrl(endpointList));
	std::string endpoint;

	while (getline(urlSplitter, endpoint, ';'))
	{
		int zmqResult = zmq_connect(*socket, getUrl(endpoint).c_str()); 

		if (zmqResult == -1)
		{	
			result = zmqErrorToStatus("ZeroMQ socket bind operation failed for endpoint " + endpoint + ", POSIX errno code: ", zmq_errno());
			// Immediately return if any of the URLs had an issue binding
			return result;
		}
	}

	return result;
}


void ZMQConnection::handleMessage(zmq_msg_t* zmqMessage, int zmqMsgSize, zmq_msg_t* metaMessage, int metaSize, const char* subject, bool isReply)
{
	bool enqueue = true;

	// Extract the contents of the encoded GMSEC message from the ZeroMQ message
	DataBuffer gmsecBuffer((GMSEC_U8*) zmq_msg_data(zmqMessage), zmqMsgSize, false);

	// Extract the contents of the encoded meta object from the ZeroMQ message
	DataBuffer metaBuffer((GMSEC_U8*) zmq_msg_data(metaMessage), metaSize, false);

	std::auto_ptr<Message> message(new Message(subject, Message::PUBLISH));

	// Decode the meta object
	ValueMap meta;
	parseProperties(meta, metaBuffer, *message.get());

	Status result = getExternal().getPolicy().unpackage(*message.get(), gmsecBuffer, meta);

	if (result.getCode() == INVALID_SIGNATURE)
	{
		m_sigMismatchFlag = true;

		AutoMutex hold(m_queueCondition.getMutex());

		m_queueCondition.signal(GOT_MESSAGE);
	}

	if (result.isError())
	{
		GMSEC_WARNING << "Unable to unpackage message";
		enqueueResult(result, (result.isError() ? 0 : message.release()));
		return;
	}

	// Check to see if is type reply and whether or not to enqueue message/result
	if (message->getKind() == Message::REPLY)
	{
		if (m_requestSpecs.legacy)
		{
			enqueue = !isReply;
		}
		else
		{
			enqueue = m_requestSpecs.exposeReplies;
		}
		
		Message* tmpMsg = message.get();

		getExternal().updateReplySubject(tmpMsg);
		
		if (isReply)
		{
			Message* reply = message.release();
			
			if (enqueue)
			{
				// Create a clone of the reply message
				// which will later be enqueued.
				message.reset(new Message(*reply));
			}
			
			if (reply)
			{
				getExternal().onReply(reply);
			}
		}
	}
	
	if (enqueue)
	{
		enqueueResult(result, (result.isError() ? 0 : message.release()));
	}

	return;
}


Status ZMQConnection::initReplyListener()
{
	Status result;

	// If subject mapping is turned on and the reply listener has not been started yet,
	// start the reply listening thread
	if (m_requestSpecs.useSubjectMapping && m_repListenSocket == NULL)
	{
		// Determine if the user supplied an endpoint for the Reply Listener
		bool userSuppliedEndpoint = m_repListenEndpoint.length() > 0;

		// If not, use the default listen port
		std::string hostname;
		char ipv4[16] = {0};
		if (!userSuppliedEndpoint)
		{
			SystemUtil::getHostName(hostname);
			hostnameToIpv4(hostname.c_str(), ipv4);

			std::ostringstream endpoint;
			endpoint << "tcp://" << ipv4 << ":" << m_repListenPort;
			m_repListenEndpoint = endpoint.str();
		}

		result = setupSocket(&m_repListenSocket, ZMQ_SUB, m_repListenEndpoint, true);

		// If the user supplied an endpoint and it failed, immediately return the error
		if (result.isError())
		{
			if (userSuppliedEndpoint || result.getCustomCode() != EADDRINUSE)
			{
				return result;
			}
			// If the user did not supply an endpoint and the error indicated that the port is in use,
			// try until a usable port is found or it hits port 49151 as that is the highest non-ephemeral
			// port number available on Linux systems (Google it)
			else
			{
				GMSEC_INFO << "Scanning for a port to set up the ReplyListener on because an endpoint"
					" was not provided in the " << ZEROMQ_REPLYLISTEN_ENDPOINT << " parameter";

				while (result.isError() && m_repListenPort <= 49151)
				{
					GMSEC_DEBUG << "Reply Listener was unable to bind to endpoint: " << m_repListenEndpoint.c_str();

					std::ostringstream endpoint;
					endpoint << "tcp://" << ipv4 << ":" << ++m_repListenPort;
					m_repListenEndpoint = endpoint.str();

					GMSEC_DEBUG << "Reply Listener attempting to bind to endpoint: " << m_repListenEndpoint.c_str();

					result = setupSocket(&m_repListenSocket, ZMQ_SUB, m_repListenEndpoint, true);
				}

				// If the loop ended and there is still an error, port 49151 was reached
				if (result.isError())
				{
					return result;
				}
			}
		}

		if (userSuppliedEndpoint)
		{
			GMSEC_DEBUG << "Reply Listener bound to endpoint: " << m_repListenEndpoint.c_str();
		}
		else
		{
			GMSEC_INFO << "Reply Listener bound to endpoint: " << m_repListenEndpoint.c_str();
		}

		zmq_setsockopt(m_repListenSocket, ZMQ_SUBSCRIBE, "", 0); // Use this to set filters on the subscription

		if (!m_repListener.get())
		{
			m_repListenerObj.reset(new ZMQMessageListener(this, &m_repListenSocket, true));

			m_repListenerObj->subscribe(m_requestSpecs.replySubject);

			m_repListener.reset(new StdThread(&runMsgListener, m_repListenerObj));

			m_repListener->start();
		}

		// Set the reply address (Will be used to tell the Replier where to send the reply)
		std::ostringstream repAddrStream;
		repAddrStream << "connect:" << m_repListenEndpoint;
		m_replyAddress = repAddrStream.str();
	}

	return result;
}


Status ZMQConnection::zmqErrorToStatus(const std::string& errorMsg, const int& errnoCode)
{
	Status status;

	// Package up the ZeroMQ error into a Status object
	std::ostringstream errorString;
	errorString << errorMsg << errnoCode;
	status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, errorString.str().c_str(), zmq_errno());

	return status;
}


bool ZMQConnection::hostnameToIpv4(const char* hostname, char* ip)
{
	Status result;

	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_in *h;
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // us AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(hostname, NULL, &hints, &servinfo)) != 0)
	{
		GMSEC_ERROR << "getaddrinfo: " << gai_strerror(rv);
		return false;
	}

	// Loop through all the results and connect to the first one we can
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		h = (struct sockaddr_in *) p->ai_addr;
		strcpy(ip, inet_ntoa(h->sin_addr));
	}

	freeaddrinfo(servinfo); // Clean up this structure
	return true;
}


void ZMQConnection::enqueueResult(const Status& status, Message* message)
{
	AutoMutex hold(m_queueCondition.getMutex());

	m_msgQueue.push(new MessageSubscriptionResult(status, message));

	m_queueCondition.signal(GOT_MESSAGE);
}


void ZMQConnection::runMsgListener(SharedMsgListener shared)
{
	shared->run();
}


void ZMQConnection::stopMsgListener(SharedMsgListener shared)
{
	shared->shutdown();
}


static void mwConfig(const Config& config, const char* key, std::string& out, bool requirePrefix)
{
	const char* value = 0;
	if (requirePrefix)
	{
		std::string prefixedKey = std::string("MW-") + key;

		value = config.getValue(prefixedKey.c_str());

		if (value != 0)
		{
			out = value;
		}
	}
	else
	{
		value = config.getValue(key);

		if (value != 0)
		{
			out = value;
		}
	}
}
