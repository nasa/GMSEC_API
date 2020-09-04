#include <gmsec4/internal/MessageAggregationToolkit.h>

#include <gmsec4/internal/InternalConnection.h>

//TODO: Replace with consolidated configuration file
#include <gmsec4/internal/ConnectionInterface.h>

#include <gmsec4/field/BinaryField.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>

#include <gmsec4/util/Buffer.h>
#include <gmsec4/util/Log.h>
#include <gmsec4/util/Mutex.h>
#include <gmsec4/util/TimeUtil.h>

#include <memory>
#include <sstream>
#include <vector>

#include <cstring>  // for memcpy()


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;


static const char* AGGREGATED_MSG_FIELD_NAME = "GMSEC-AGGREGATED-MSG";


MessageAggregationToolkit::MessageAggregationToolkit(InternalConnection* conn)
	: m_connection(conn),
	  m_maxMsgsPerBin(100),
	  m_flushPeriod(30000),
	  m_messageBins(),
	  m_aggregateAllMsgs(true),
	  m_messageQ(),
	  m_condition(),
	  m_encoder(),
	  m_decoder()
{
}


MessageAggregationToolkit::~MessageAggregationToolkit()
{
}


Condition& MessageAggregationToolkit::getCondition()
{
	return m_condition;
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

		std::auto_ptr<Message> tmpMsg(new Message(msg->getSubject(), msg->getKind()));

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

	MessageBin* bin = m_messageBins[subject];

	if (!bin)
	{
		m_messageBins[subject] = new MessageBin();
	}

	m_aggregateAllMsgs = false;
}


bool MessageAggregationToolkit::addMessage(const Message& msg, const Config& config)
{
	if (msg.getField(AGGREGATED_MSG_FIELD_NAME) != NULL)
	{
		return false;
	}

	AutoMutex lock(m_condition.getMutex());

	MessageBin* bin = m_messageBins[msg.getSubject()];

	if (!bin)
	{
		if (m_aggregateAllMsgs)
		{
			// No bin exists for message, and since we are caching
			// all messages we need to create it.
			bin = new MessageBin();
			m_messageBins[msg.getSubject()] = bin;
		}
		else
		{
			// No bin exists for message, and since we are NOT caching
			// just any ol' message, we return immediately.
			return false;
		}
	}

	bin->m_messages.push_back(new Message(msg));
	bin->m_config = config;

	if (bin->m_messages.size() == 1)
	{
		bin->m_flushTime = getExpirationTime();
	}

	if (bin->m_messages.size() >= m_maxMsgsPerBin)
	{
		bin->m_readyToProcess = true;
		m_condition.broadcast(FLUSH_BIN);
	}

	return true;
}


void MessageAggregationToolkit::runThread(StdSharedPtr<MessageAggregationToolkit> shared)
{
	shared->run();
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


size_t MessageAggregationToolkit::getMaxMsgsPerBin() const
{
	return m_maxMsgsPerBin;
}


int MessageAggregationToolkit::getFlushPeriod() const
{
	return m_flushPeriod;
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


void MessageAggregationToolkit::processBin(MessageBin* bin)
{
	AutoMutex lock(m_condition.getMutex());

	if (bin && bin->m_messages.size() > 0)
	{
		std::auto_ptr<Message> aggMsg;

		std::vector<GMSEC_U8> aggregatedData;

		for (MessageBin::MessageList::iterator it = bin->m_messages.begin(); it != bin->m_messages.end(); ++it)
		{
			Message*           cachedMsg = *it;
			std::ostringstream fieldName;
			DataBuffer         buffer;

			// use first cached message to create framework for aggregated message
			if (it == bin->m_messages.begin())
			{
				aggMsg.reset(new Message(cachedMsg->getSubject(), cachedMsg->getKind()));

				aggMsg.get()->addField(AGGREGATED_MSG_FIELD_NAME, true);
				aggMsg.get()->addField("NUM-MESSAGES", GMSEC_U32(bin->m_messages.size()));
			}

			// encode message into data buffer
			m_encoder.encode(*cachedMsg, buffer);

			GMSEC_U32 bufSize  = GMSEC_U32(buffer.size());
			GMSEC_U8* dataSize = reinterpret_cast<GMSEC_U8*>(&bufSize);
			GMSEC_U8* data     = reinterpret_cast<GMSEC_U8*>(buffer.raw());

			// aggregate encoded data into data buffer
			aggregatedData.reserve(aggregatedData.size() + sizeof(bufSize) + buffer.size());
			aggregatedData.insert(aggregatedData.end(), dataSize, dataSize + sizeof(bufSize));
			aggregatedData.insert(aggregatedData.end(), data, data + buffer.size());

			delete cachedMsg;
		}

		// add aggregated data as BinaryField in message
		aggMsg.get()->addField("MSG-DATA", &aggregatedData[0], aggregatedData.size());

		try
		{
			// publish message containing aggregated data
			m_connection->publish(*(aggMsg.get()), bin->m_config);

			m_publishStatus.reset();
		}
		catch (Exception& e)
		{
			GMSEC_ERROR << "Failed to send aggregated message with topic " << aggMsg.get()->getSubject()
			            << " due to: " << e.what();

			m_publishStatus = Status(e);

			m_connection->dispatchEvent(Connection::MSG_PUBLISH_FAILURE_EVENT, m_publishStatus);
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
