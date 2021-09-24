/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec4/internal/MessageAggregationToolkit.h>

#include <gmsec4/internal/InternalConnection.h>

#include <gmsec4/internal/ConnectionInterface.h>
#include <gmsec4/internal/Subject.h>

#include <gmsec4/field/BinaryField.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>

#include <gmsec4/util/Buffer.h>
#include <gmsec4/util/Log.h>
#include <gmsec4/util/Mutex.h>
#include <gmsec4/util/StdUniquePtr.h>
#include <gmsec4/util/TimeUtil.h>

#include <sstream>
#include <vector>

#include <cstring>  // for memcpy()


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;


static const char* AGGREGATED_MSG_FIELD_NAME = "GMSEC-AGGREGATED-MSG";


MessageAggregationToolkit::MessageAggregationToolkit(InternalConnection* conn, const Config& config)
	: m_connection(conn),
	  m_maxMsgsPerBin(100),
	  m_flushPeriod(30000),
	  m_aggregateAllMsgs(true),
	  m_messageBins(),
	  m_messageQ(),
	  m_messageFilter(),
	  m_condition(),
	  m_encoder(),
	  m_decoder()
{
	configure(config);
}


MessageAggregationToolkit::~MessageAggregationToolkit()
{
	destroyBins();
}


bool MessageAggregationToolkit::addMessage(const Message& msg, const Config& config)
{
	if (msg.getField(AGGREGATED_MSG_FIELD_NAME) != NULL)
	{
		return false;
	}

	AutoMutex lock(m_condition.getMutex());

	const char* subject = msg.getSubject();
	MessageBin* bin     = NULL;

	if (m_messageFilter.isSubjectExcluded(subject))
	{
		return false;
	}

	if (m_aggregateAllMsgs)
	{
		MessageBins::iterator it = m_messageBins.find(subject);

		if (it != m_messageBins.end())
		{
			bin = it->second;
		}
	}
	else
	{
		for (MessageBins::iterator it = m_messageBins.begin(); it != m_messageBins.end() && bin == NULL; ++it)
		{
			if (Subject::doesSubjectMatchPattern(subject, it->first))
			{
				bin = it->second;
			}
		}
	}

	if (bin == NULL)
	{
		// Message bin not found; if we are aggregating all messages and the current one in question
		// is not defined within the excluded list, then create a new bin for it.
		if (m_aggregateAllMsgs && !m_messageFilter.isSubjectExcluded(subject))
		{
			bin = new MessageBin();
			m_messageBins[subject] = bin;
		}
	}

	if (bin != NULL)
	{
		bin->m_messages.push_back(new Message(msg));
		bin->m_config = config;   // TODO: Ideally, it would be better to associate the config with the message, not the bin.

		if (bin->m_messages.size() == 1)
		{
			bin->m_flushTime = getExpirationTime();
		}

		if (bin->m_messages.size() >= m_maxMsgsPerBin)
		{
			bin->m_readyToProcess = true;
			m_condition.broadcast(FLUSH_BIN);
		}
	}

	// Return true if we found/created a bin for the message, or false if not. In case of the latter,
	// the message should be published without delay.
	return (bin != NULL);
}


Status MessageAggregationToolkit::getPublishStatus()
{
	AutoMutex lock(m_condition.getMutex());
	return m_publishStatus;
}


bool MessageAggregationToolkit::isAggregatedMsg(const Message* msg) const
{
	return (msg != NULL) && (msg->getField(AGGREGATED_MSG_FIELD_NAME) != NULL);
}


void MessageAggregationToolkit::processAggregatedMsg(Message*& msg)
{
	if (msg == NULL)
	{
		return;
	}

	GMSEC_U32          numMsgs = msg->getU32Field("NUM-MESSAGES").getValue();
	const BinaryField& dataFld = msg->getBinaryField("MSG-DATA");

	GMSEC_BIN rawData = dataFld.getValue();
	size_t    rawSize = dataFld.getLength();

	size_t    dataPos = 0;

	for (GMSEC_U32 i = 0; i < numMsgs && dataPos < rawSize; ++i)
	{
		GMSEC_U32 msgSize = 0;

		std::memcpy(&msgSize, rawData + dataPos, sizeof(msgSize));
		dataPos += sizeof(msgSize);

		GMSEC_U8* msgData = new GMSEC_U8[msgSize];

		std::memcpy(msgData, rawData + dataPos, msgSize);
		dataPos += msgSize;

		DataBuffer data(msgData, msgSize, true);  // msgData will be deleted when DataBuffer is destroyed

		StdUniquePtr<Message> tmpMsg(new Message(msg->getSubject(), msg->getKind(), m_connection->getMessageConfig()));

		m_decoder.decode(*tmpMsg.get(), data);

		{
			// critical section
			AutoMutex lock(m_condition.getMutex());

			m_messageQ.push(tmpMsg.release());
		}
	}

	delete msg;

	msg = NULL;
}


bool MessageAggregationToolkit::hasNextMsg()
{
	AutoMutex lock(m_condition.getMutex());

	return m_messageQ.size() > 0;
}


Message* MessageAggregationToolkit::nextMsg()
{
	Message* msg = NULL;

	AutoMutex lock(m_condition.getMutex());

	if (m_messageQ.size() > 0)
	{
		msg = m_messageQ.front();

		m_messageQ.pop();
	}

	return msg;
}


void MessageAggregationToolkit::runThread(StdSharedPtr<MessageAggregationToolkit> shared)
{
	shared->run();
}


void MessageAggregationToolkit::shutdown()
{
	AutoMutex lock(m_condition.getMutex());

	destroyBins();

	m_condition.broadcast(QUIT);

	bool threadDone = false;
	int  tries = 3;

	while (!threadDone && tries-- > 0)
	{
		int reason = m_condition.wait(1000);

		threadDone = (reason == UNMANAGED);
	}
}


Condition& MessageAggregationToolkit::getCondition()
{
	return m_condition;
}


size_t MessageAggregationToolkit::getMaxMsgsPerBin() const
{
	return m_maxMsgsPerBin;
}


int MessageAggregationToolkit::getFlushPeriod() const
{
	return m_flushPeriod;
}


void MessageAggregationToolkit::configure(const Config& config)
{
	AutoMutex lock(m_condition.getMutex());

	for (int N = 1; true; ++N)
	{
		std::ostringstream key;
		key << "GMSEC-MSG-BIN-SUBJECT-" << N;

		const char* subject = config.getValue(key.str().c_str());

		if (subject && !std::string(subject).empty())
		{
			createMsgBin(subject);

			m_aggregateAllMsgs = false;
		}
		else
		{
			break;
		}
	}

	for (int N = 1; true; ++N)
	{
		std::ostringstream key;
		key << "GMSEC-MSG-BIN-EXCLUDE-SUBJECT-" << N;

		const char* subject = config.getValue(key.str().c_str());

		if (subject && !std::string(subject).empty())
		{
			m_messageFilter.excludeSubject(subject);
		}
		else
		{
			break;
		}
	}

	m_maxMsgsPerBin = config.getIntegerValue(GMSEC_MSG_BIN_SIZE, (int) m_maxMsgsPerBin);
	m_flushPeriod   = config.getIntegerValue(GMSEC_MSG_BIN_TIMEOUT, m_flushPeriod);
}


void MessageAggregationToolkit::createMsgBin(const char* subject)
{
	AutoMutex lock(m_condition.getMutex());

	if (!subject || std::string(subject).empty())
	{
		throw Exception(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Subject cannot be NULL nor an empty string");
	}

	MessageBins::iterator it = m_messageBins.find(subject);

	if (it == m_messageBins.end())
	{
		m_messageBins[subject] = new MessageBin();
	}
}


void MessageAggregationToolkit::run()
{
	AutoMutex lock(m_condition.getMutex());

	bool running = true;

	m_condition.broadcast(MANAGED);

	while (running)
	{
		bool flush  = false;
		int  reason = m_condition.wait(100);

		switch (reason)
		{
			case TIMEOUT:
				flush = checkForExpiredBins();
				break;

			case FLUSH_BIN:
				flush = true;
				break;

			case QUIT:
				running = false;
				break;

			default:
				GMSEC_DEBUG << "MessageAggregationToolkit::run unexpected reason " << reason;
				break;
		}

		if (running && flush)
		{
			processBins();
		}
	}

	m_condition.broadcast(UNMANAGED);
}


bool MessageAggregationToolkit::checkForExpiredBins()
{
	bool haveExpiredBins = false;

	AutoMutex lock(m_condition.getMutex());

	for (MessageBins::iterator it = m_messageBins.begin(); it != m_messageBins.end(); ++it)
	{
		MessageBin* bin = it->second;

		if (bin->m_flushTime < TimeUtil::getCurrentTime_s())
		{
			haveExpiredBins       = true;
			bin->m_readyToProcess = true;
			bin->m_flushTime      = getExpirationTime();
		}
	}

	return haveExpiredBins;
}


void MessageAggregationToolkit::processBins()
{
	AutoMutex lock(m_condition.getMutex());

	for (MessageBins::iterator it = m_messageBins.begin(); it != m_messageBins.end(); ++it)
	{
		MessageBin* bin = it->second;

		if (bin->m_readyToProcess)
		{
			processBin(bin);
		}
	}
}


struct AggregatedMessage
{
	AggregatedMessage(const char* subject, Message::MessageKind kind)
		: msg(new Message(subject, kind)),
		  numAggregated(0),
		  data()
	{
	}

	~AggregatedMessage()
	{
		delete msg;
	}

	Message*              msg;
	GMSEC_U32             numAggregated;
	std::vector<GMSEC_U8> data;
};


void MessageAggregationToolkit::processBin(MessageBin* bin)
{
	AutoMutex lock(m_condition.getMutex());

	if (bin && bin->m_messages.size() > 0)
	{
		typedef std::map<std::string, AggregatedMessage*> AggregatedMessages;

		// The Message Bin may contain a heterogeneous collection of messages with somewhat similar
		// subjects (undoubtedly matching the pattern of a configured wilcard subject).
		//
		// To assist with the aggregation of messages bearing the same subject, we rely on keeping
		// an individual 'bucket' for each unique subject.  Each bucket will contain a Message object,
		// that in turn contains the aggregated encoded message data, and the number of messages
		// included in the aggregated data.
		//
		// When the bucket(s) have been prepared, the message contained within will be published.

		AggregatedMessages aggregatedMessages;

		for (MessageBin::MessageList::iterator it = bin->m_messages.begin(); it != bin->m_messages.end(); ++it)
		{
			Message*           cachedMsg = *it;
			std::ostringstream fieldName;
			DataBuffer         buffer;

			AggregatedMessage* aggMsg = NULL;

			AggregatedMessages::iterator it2 = aggregatedMessages.find(cachedMsg->getSubject());

			if (it2 == aggregatedMessages.end())
			{
				// create new aggregated message bucket
				aggMsg = new AggregatedMessage(cachedMsg->getSubject(), cachedMsg->getKind());

				aggMsg->msg->addField(AGGREGATED_MSG_FIELD_NAME, true);
				aggMsg->msg->addField("NUM-MESSAGES", aggMsg->numAggregated);

				aggregatedMessages[cachedMsg->getSubject()] = aggMsg;
			}
			else
			{
				// use existing aggregated message bucket
				aggMsg = it2->second;
			}

			// encode message into data buffer
			m_encoder.encode(*cachedMsg, buffer);

			GMSEC_U32 bufSize  = GMSEC_U32(buffer.size());
			GMSEC_U8* dataSize = reinterpret_cast<GMSEC_U8*>(&bufSize);
			GMSEC_U8* data     = reinterpret_cast<GMSEC_U8*>(buffer.raw());

			// add encoded data into aggregate data buffer
			aggMsg->data.reserve(aggMsg->data.size() + sizeof(bufSize) + buffer.size());
			aggMsg->data.insert(aggMsg->data.end(), dataSize, dataSize + sizeof(bufSize));
			aggMsg->data.insert(aggMsg->data.end(), data, data + buffer.size());

			// increment message count and add aggregated data buffer as BinaryField in message
			aggMsg->msg->addField("NUM-MESSAGES", ++aggMsg->numAggregated);
			aggMsg->msg->addField("MSG-DATA", &aggMsg->data[0], aggMsg->data.size());

			delete cachedMsg;
		}

		// Publish message contained within each aggregated message bucket
		for (AggregatedMessages::iterator it = aggregatedMessages.begin(); it != aggregatedMessages.end(); ++it)
		{
			try
			{
				m_connection->publish(*(it->second->msg), bin->m_config);

				m_publishStatus.reset();
			}
			catch (Exception& e)
			{
				GMSEC_ERROR << "Failed to send aggregated message with topic " << it->second->msg->getSubject()
				            << " due to: " << e.what();

				m_publishStatus = Status(e);

				m_connection->dispatchEvent(Connection::MSG_PUBLISH_FAILURE_EVENT, m_publishStatus);
			}

			delete it->second;
		}

		bin->m_messages.clear();
		bin->m_readyToProcess = false;
	}
}


void MessageAggregationToolkit::destroyBins()
{
	AutoMutex lock(m_condition.getMutex());

	for (MessageBins::iterator it = m_messageBins.begin(); it != m_messageBins.end(); ++it)
	{
		MessageBin* bin = it->second;

		processBin(bin);

		delete bin;
	}

	m_messageBins.clear();
}


double MessageAggregationToolkit::getExpirationTime() const
{
	return TimeUtil::getCurrentTime_s() + (m_flushPeriod / 1000) + double(m_flushPeriod % 1000) / 1000.0;
}
