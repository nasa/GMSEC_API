#ifndef GMSEC_API_INTERNAL_MESSAGE_AGGREGATION_TOOLKIT_H
#define GMSEC_API_INTERNAL_MESSAGE_AGGREGATION_TOOLKIT_H

#include <gmsec4/internal/Encoder.h>

#include <gmsec4/Config.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/Condition.h>
#include <gmsec4/util/StdSharedPtr.h>

#include <list>
#include <map>
#include <queue>
#include <string>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Message;

namespace internal
{
	// Forward declaration(s)
	class InternalConnection;


class MessageAggregationToolkit
{
public:
	enum Reasons
	{
		UNMANAGED = 0,
		TIMEOUT   = gmsec::api::util::Condition::TIMEOUT,
		QUIT      = gmsec::api::util::Condition::USER,
		FLUSH_BIN = 3,
		MANAGED   = 4
	};


	MessageAggregationToolkit(InternalConnection* conn);


	~MessageAggregationToolkit();


	gmsec::api::util::Condition& getCondition();


	// Client-side Message Aggregation Toolkit (MAT) methods
	//
	bool isAggregatedMsg(const Message* msg) const;


	void processAggregatedMsg(Message*& msg);


	bool hasNextMsg();


	Message* nextMsg();


	// MAT thread-related methods
	//
	void configure(const Config& config);


	void createMsgBin(const char* subject);


	bool addMessage(const Message& msg, const Config& config);


	Status getPublishStatus();


	static void runThread(gmsec::api::util::StdSharedPtr<MessageAggregationToolkit> shared);


	void run();


	void shutdown();


	size_t getMaxMsgsPerBin() const;


	int getFlushPeriod() const;


private:
	// Defined, but not implemented
	MessageAggregationToolkit();
	MessageAggregationToolkit(const MessageAggregationToolkit& other);
	MessageAggregationToolkit& operator=(const MessageAggregationToolkit& other);


	struct MessageBin
	{
		typedef std::list<Message*> MessageList;

		MessageBin()
			: m_messages(),
			  m_config(),
			  m_readyToProcess(false),
			  m_flushTime(0)
		{
		}

		MessageList  m_messages;
		Config       m_config;
		bool         m_readyToProcess;
		double       m_flushTime;
	};


	typedef std::map<std::string, MessageBin*> MessageBins;
	typedef std::queue<Message*>               MessageQueue;


	bool checkForExpiredBins();
	void processBins();
	void processBin(MessageBin* bin);
	void destroyBins();

	double getExpirationTime() const;


	InternalConnection*                  m_connection;

	size_t                               m_maxMsgsPerBin;
	int                                  m_flushPeriod;
	MessageBins                          m_messageBins;
	bool                                 m_aggregateAllMsgs;

	MessageQueue                         m_messageQ;

	Status                               m_publishStatus;

	gmsec::api::util::Condition          m_condition;
	gmsec::api::internal::MessageEncoder m_encoder;
	gmsec::api::internal::MessageDecoder m_decoder;
};

}  // end namespace internal
}  // end namespace api
}  // end namespace gmsec


#endif
