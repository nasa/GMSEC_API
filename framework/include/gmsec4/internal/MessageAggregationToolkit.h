#ifndef GMSEC_API_INTERNAL_MESSAGE_AGGREGATION_TOOLKIT_H
#define GMSEC_API_INTERNAL_MESSAGE_AGGREGATION_TOOLKIT_H

#include <gmsec4/internal/Encoder.h>
#include <gmsec4/internal/ExclusionFilter.h>

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


	/**
	 * @fn MessageAggregationToolkit(InternalConnection* conn)
	 *
	 * @brief Default constructor.  Note that the maximum number of messages that will be stored
	 * in a Configurable Message Bin will be 100, that the flush period per bin will be 30000ms,
	 * and that all messages will be aggregated.  These settings of course can be changed by
	 * configuring the Message Aggregation Toolkit (MAT).
	 *
	 * @param conn   - Internal Connection object that will be used to publish aggregated messages.
	 * @param config - Configuration object to configure the Configurable Message Bins.
	 */
	MessageAggregationToolkit(InternalConnection* conn, const Config& config);


	/**
	 * @fn ~MessageAggregationToolkit()
	 *
	 * @brief Destructor
	 */
	~MessageAggregationToolkit();


	/**
	 * @fn bool addMessage(const Message& msg, const Config& config)
	 *
	 * @brief Adds the non-aggregated message to a Configurable Message bin if all messages
	 * are being aggregated, or if the message's subject/topic matches that of which has been
	 * configured to be stored.
	 *
	 * @param msg    - the Message object to consider for storage in a bin.
	 * @param config - the Config object given when the message is being published.
	 *
	 * @return A value of true is returned if the message is stored in a bin; false otherwise.
	 */
	bool addMessage(const Message& msg, const Config& config);


	/**
	 * @fn Status getPublishStatus()
	 *
	 * @brief Returns the last known status result of a published aggregated message.
	 *
	 * @return The publish status.
	 */
	Status getPublishStatus();


	/**
	 * @fn bool isAggregateMsg(const Message* msg) const
	 *
	 * @brief Determines whether the given message already consist of aggregated message(s).
	 *
	 * @return Returns true if the message is an aggregated message; false otherwise.
	 *
	 * @sa processAggregatedMsg()
	 */
	bool isAggregatedMsg(const Message* msg) const;


	/**
	 * @fn void processAggregatedMsg(Message*& msg)
	 *
	 * @brief Extracts individual message(s) from the given aggregated message.  When this
	 * task has completed, the given message is destroyed and set to NULL.
	 *
	 * @param msg - an aggregated message.
	 *
	 * @sa isAggregatedMsg()
	 */
	void processAggregatedMsg(Message*& msg);


	/**
	 * @fn bool hasNextMsg()
	 *
	 * @brief Reports whether the message queue contains any messages that were previously
	 * extracted from an aggregated message.
	 *
	 * @return True if a message is available; false otherwise.
	 *
	 * @sa nextMsg()
	 */
	bool hasNextMsg();


	/**
	 * @fn Message* nextMsg()
	 *
	 * @brief Acquires the next available message from the queue.
	 *
	 * @return The next available message, or NULL if none is available.
	 *
	 * @sa hasNextMsg()
	 */
	Message* nextMsg();


	/**
	 * @fn void runThread(gmsec::api::util::StdSharedPtr<MessageAggregationToolkit> shared)
	 *
	 * @brief Starts the Message Aggregation Toolkit (MAT) thread.
	 *
	 * @param The MAT object to share between the MAT thread and the main-thread.
	 */
	static void runThread(gmsec::api::util::StdSharedPtr<MessageAggregationToolkit> shared);


	/**
	 * @fn void shutdown()
	 *
	 * @brief Stops the Message Aggregation Toolkit (MAT) thread.
	 */
	void shutdown();


	/**
	 * @fn gmsec_api::util::Condition& getCondition()
	 *
	 * @brief Returns the associated Condition object.
	 *
	 * @return A reference to a Condition object.
	 */
	gmsec::api::util::Condition& getCondition();


	/**
	 * @fn size_t getMaxMsgsPerBin() const
	 *
	 * @return The maximum number of messages that will be stored in a Configurable Message Bin.
	 */
	size_t getMaxMsgsPerBin() const;


	/**
	 * @fn int getFlushPeriod() const
	 *
	 * @return The flush period for messages that are stored in a Configurable Message Bin.
	 */
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


	void configure(const Config& config);
	void createMsgBin(const char* subject);
	void run();
	bool checkForExpiredBins();
	void processBins();
	void processBin(MessageBin* bin);
	void destroyBins();
	double getExpirationTime() const;


	typedef std::map<std::string, MessageBin*> MessageBins;
	typedef std::queue<Message*>               MessageQueue;


	// Class member data
	//
	InternalConnection*                   m_connection;

	size_t                                m_maxMsgsPerBin;
	int                                   m_flushPeriod;
	bool                                  m_aggregateAllMsgs;

	MessageBins                           m_messageBins;
	MessageQueue                          m_messageQ;
	gmsec::api::internal::ExclusionFilter m_messageFilter;

	Status                                m_publishStatus;

	gmsec::api::util::Condition           m_condition;
	gmsec::api::internal::MessageEncoder  m_encoder;
	gmsec::api::internal::MessageDecoder  m_decoder;
};

}  // end namespace internal
}  // end namespace api
}  // end namespace gmsec


#endif
