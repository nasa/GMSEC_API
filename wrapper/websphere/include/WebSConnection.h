/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file WebSConnection.h
 *
 *	@author Matt Handy
 *  @date November 16, 2009
 *
 *	@brief This is the implementation of the Connection class for the websphere
 *	wrapper. All message traffic with the server is handled here.
 */

#ifndef GMSEC_WEBSPHERE_CONNECTION_H
#define GMSEC_WEBSPHERE_CONNECTION_H

#include "gmsec_websphere.h"

#include <gmsec4/ConfigOptions.h>

#include <gmsec4/internal/ConnectionInterface.h>
#include <gmsec4/internal/UniqueFilter.h>

#include <gmsec4/Config.h>
#include <gmsec4/Message.h>

#include <gmsec4/util/Buffer.h>
#include <gmsec4/util/Condition.h>
#include <gmsec4/util/CountDownLatch.h>
#include <gmsec4/util/StdSharedPtr.h>
#include <gmsec4/util/StdThread.h>
#include <gmsec4/util/StdUniquePtr.h>

#include <cmqc.h>
#include <cmqxc.h>

#include <queue>
#include <stdio.h>


namespace gmsec_websphere
{
/*
const char OPT_MAX_TOPIC_HANDLES[]                   = "MW-MAX-TOPIC-HANDLES";
const char OPT_ASYNC_PUBLISH[]                       = "MW-ASYNC-PUBLISH";
const char OPT_ASYNC_STATUS_CHECK[]                  = "MW-ASYNC-STATUS-CHECK";
const char OPT_ASYNC_STATUS_CHECK_MESSAGE_INTERVAL[] = "MW-ASYNC-STATUS-CHECK-MESSAGE-INTERVAL";
*/
class WebSConnection;


/**	@class SubscriptionInfo
 *
 *	@brief This class represents a thread that forms a subscription, listens to it,
 *	and posts messages to the appropriate queue within a connection object.
 */
class SubscriptionInfo
{
public:
	/** @fn SubscriptionInfo(xms::Destination topic, xms::MessageConsumer consumer)
	 *
	 *	@brief Constructor
	 *
	 *	@param t - The topic string
	 *	@param q - The message queue
	 *	@param c - main connection condition variable
	 *	@param subscribe - condition variable to signal when subscription complete
	 *	@param connection - reference to creating connection object
	 *  @param ts - Is this connection thread safe?
	 *  @param exconn - The MQI connection
	 */
	SubscriptionInfo(const std::string& t,
	                 gmsec::api::util::Condition& subscribe,
	                 WebSConnection* connection,
	                 MQHCONN exconn,
	                 bool forReplies);


	/** @fn ~SubscriptionInfo()
	 *
	 *	@brief Basic deconstructor
	 */
	virtual ~SubscriptionInfo();


	/** @fn run()
	 *
	 *	@brief As this is a thread, run starts the thread
	 */
	virtual void CALL_TYPE run();


	/** @fn shutdown
	 *
	 *  @brief Stops the thread
	 */
	virtual void CALL_TYPE shutdown();


	/** @fn getReasonCode()
	 *
	 *	@brief Returns the stored reason code
	 */
	MQLONG getReasonCode() const;


private:
	// defined, but not implemented
	SubscriptionInfo(const SubscriptionInfo&);
	SubscriptionInfo& operator=(const SubscriptionInfo&);


	bool initSubscriptionWithConn(MQGMO& gmo, MQSD& sd, MQMD& md);


	bool                              doneWithLife;
	bool                              connected;
	gmsec::api::util::CountDownLatch* deathlatch;
	std::string                       topic;
	MQHCONN                           mqconnection;
	gmsec::api::util::Condition&      sub;
	WebSConnection*                   conn;
	int                               currBufLen;
	char*                             buffer;
	char*                             qTopic;
	bool                              forReplies; /* is this object to process internal replies */

	MQHOBJ                            Hobj;       /* object handle used for MQGET  */
	MQHOBJ                            Hsub;       /* object handle                 */
	MQLONG                            Reason;     /* reason code                   */
	MQLONG                            CompCode;   /* completion code               */
	MQHMSG	                          hmsg;
};


/** @class WebSConnection
 *
 *	@brief This is the interface between the GMSEC API and the WebSphere server. All
 *	communication with the server goes through this class. Multiple connections may be held
 *	by a given client application, but this affords no additional functionality
 */
class GMSEC_WEBS_API WebSConnection : public gmsec::api::internal::ConnectionInterface
{
private:
	struct GMSEC_WEBS_API MsgSubscriptionResult
	{
		gmsec::api::Status   status;
		gmsec::api::Message* message;

		MsgSubscriptionResult(const gmsec::api::Status& status, gmsec::api::Message* msg)
			: status(status), message(msg)
		{
		}
	};


	typedef gmsec::api::util::StdUniquePtr<gmsec::api::util::StdThread> SubscriptionThread;
	typedef gmsec::api::util::StdSharedPtr<SubscriptionInfo>            SharedSubscriptionInfo;
	typedef std::map<std::string, SharedSubscriptionInfo>               Subscriptions;


	// configuration
	std::map<std::string, MQHOBJ>          publishers;
	gmsec::api::util::Condition            queueCondition;
	gmsec::api::util::Condition            subscribeCondition;
	gmsec::api::internal::UniqueFilter     uniqueFilter;
	std::queue<MsgSubscriptionResult*>     queue;
	int                                    uniquecounter;
	long                                   requestCounter;

	SubscriptionThread                     requestThread;
	SharedSubscriptionInfo                 requestSubscriptionInfo;

	Subscriptions                          subscriptions;

	gmsec::api::internal::RequestSpecs     requestSpecs;

	std::string hostname;
	std::string channel;
	std::string qmanager;
	std::string pKeyReposStem;
	std::string pCipherSpec;
	bool        useFilter;
	bool        connAsyncPublish;
	bool        connCheckAsyncStatus;
	int         checkAsyncStatusMessageInterval;
	int         messageCounter;
	int         messageWaitPeriod;

	std::string mwInfo;
	bool        sigMismatchFlag;

	int         maxConnectionRetries;
	int         connectionRetryInterval;
	size_t      maxTopicHandles;

	MQHCONN     Hcon;


	static void runSubscriptionThread(SharedSubscriptionInfo shared);


	std::string generateUniqueId();
	std::string generateUniqueId(long id);


	void openObjectHandle(const std::string& subject, MQMD& md, MQHOBJ& objHandle);

	void createMsgHandle(gmsec::api::internal::ValueMap& meta, MQHMSG& msgHandle, MQPMO& msgOptions);

	void publishMessage(MQHOBJ& objHandle, MQMD& msgDesc, MQPMO& msgOpts, gmsec::api::util::DataBuffer& buffer);

	gmsec::api::Status reconnect();

	void flushTopicHandles();

	void checkAsyncPublishStatus();

	// defined, but not implemented
	WebSConnection(const WebSConnection&);
	WebSConnection& operator=(const WebSConnection&);

public:
	enum Reasons
	{
		QUIT = gmsec::api::util::Condition::USER + 1,
		GOT_MESSAGE,
		UNABLE_TO_SUBSCRIBE
	};


	/**	@fn WebSConnection(const gmsec::api::Config& config)
	 *
	 *	@brief This constructor accepts a standard gmsec config parameter and parses out several
	 *	configuration values that are enumerated as follows.
	 */
	WebSConnection(const gmsec::api::Config& config);


	/** @fn ~WebSConnection()
	 *
	 *	@brief Standard deconstructor. Only real task is freeing the memory associated with
	 *	subscription information.
	 */
	virtual ~WebSConnection();


	/**	@fn getLibraryRootName()
	 *
	 *	@brief This function simply returns the root name of the websphere library as
	 *	defined below.
	 */
	virtual const char* CALL_TYPE getLibraryRootName()
	{
		return WEBSPHERE_VERSION_STRING;	// defined in gmsec_websphere.h
	}


	/**	@fn getLibraryVersion()
	 *
	 *	@brief This function gets the char* that contains the library version name.
	 */
	virtual const char* CALL_TYPE getLibraryVersion();


	/**	@fn getMWInfo()
	 *
	 *	@brief This function returns a char* containing the middleware information
	 */
	virtual const char* CALL_TYPE getMWInfo();


	/**	@fn mwConnect()
	 *
	 *	@brief This function causes the connection to start communication with the server.
	 *
	 *	@return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwConnect();


	/** @fn mwDisconnect()
	 *
	 *	@brief This function ceases communication with the server, but the underlying connection
	 *	is not destroyed.
	 *
	 *	@return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwDisconnect();


	/**	@fn mwSubscribe(const char* subject, const gmsec::api::Config& config)
	 *
	 *	@brief This function begins a subscription to the given subject. All messages received pertaining
	 *	to this subject heading will be placed on the queue for the connection object.
	 *
	 *	@note Note that the subject as passed in will be converted from the standard GMSEC format to the WebSphere
	 *	format.
	 *
	 *	@param subject - The GMSEC formatted subsciption
	 *	@param config  - The GMSEC config containing subscribe operation options
	 *
	 *  @return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwSubscribe(const char* subject, const gmsec::api::Config& config);


	/**	@fn mwUnsubscribe(const char* subject)
	 *
	 *	@brief This function removes subscription to a given topic by deleting the subscription info
	 *	object, which in turn deletes the MessageConsumer, so messages are no longer forwarded to this
	 *	connection by the server
	 *
	 *	@param subject - The GMSEC formatted subscription
	 *
	 *	@return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwUnsubscribe(const char* subject);


	/**	@fn mwPublish(const gmsec::api::Message& msg, const gmsec::api::Config& config)
	 *
	 *	@brief This function publishes a message by extracting its Destination and then publishing
	 *	to the server based on that Destination
	 *
	 *	@param msg    - This is a reference to the message to be published.
	 *	@param config - The GMSEC config containing publish operation options
	 *
	 *	@return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwPublish(const gmsec::api::Message& msg, const gmsec::api::Config& config);


	/**	@fn mwRequest(const gmsec::api::Message& request, std::string &id)
	 *
	 *	@brief This function publishes a request via the same mechanism as the Publish method.
	 *
	 *	@param request - This is a reference to the message that is to be published.
	 *	@param id      - The unique id that is assigned to this request call.
	 *
	 *	@return status - result of the connection operation
	 *
	**/
	virtual void CALL_TYPE mwRequest(const gmsec::api::Message& request, std::string &id);


	/**	@fn mwReply(const gmsec::api::Message& request, const gmsec::api::Message& reply)
	 *
	 *	@brief This function is called when an application receives a message and acknowledges
	 *	that it was sent as a request due to its GMSEC_MSG_TYPE. The function takes references
	 *	to two messages and publishes the second message based on the reply information supplied
	 *	by the first message.
	 *
	 *	@param request - This is a reference to the request.
	 *	@param reply   - This is a reference to the message that the application has generated in reply.
	 *
	 *	It will be published based on information gathered from the request, namely by setting the
	 *	destination of the reply to the value returned from calling getJMSReplyTo() on the request
	 *	message.
	 *
	 *	@return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwReply(const gmsec::api::Message& request, const gmsec::api::Message& reply);


	/**	@fn mwReceive(gmsec::api::Message*& msg, GMSEC_I32 timeout)
	 *
	 *	@brief This function is the basic mechanism used by applications to process incoming
	 *	messages. All messages received by this connection are placed on the connection's queue,
	 *	where they wait to be handed to the application by this function. This function will wait
	 *	until signalled by a listener in the event that there is no available message upon calling
	 *
	 *	@param msg - This is a reference to a pointer which will be directed to the message returned by the function.
	 *	@param timeout - If there is no message immediately available, this is the length of time
	 *	that the application should wait for one to be delivered. Note that this can be set to
	 *	GMSEC_WAIT_FOREVER, in which case the function will hold indefinitely while waiting for a message.
	 *
	 *	@return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwReceive(gmsec::api::Message*& msg, GMSEC_I32 timeout);

	bool fromMW(const gmsec::api::util::DataBuffer& buffer, MQHCONN hcon, MQHMSG hmsg, bool forReplies);


	void enqueueResult(const gmsec::api::Status& status, gmsec::api::Message* message = 0);


	//For use by SubscriptionInfo when restablishing connection
	std::string getQueueManager(){ return qmanager; }
	std::string getHostname(){ return hostname; }
	std::string getChannel(){ return channel; }
	std::string getCipherSpec(){ return pCipherSpec; }
	std::string getKeyReposStem(){ return pKeyReposStem; }
	int getMaxConnectionRetries() { return maxConnectionRetries; }
	int getConnectionRetryInterval() { return connectionRetryInterval; }
	int getMessageWaitPeriod() { return messageWaitPeriod; }


	/** @fn createLiveConnection
	 *
	 *	@brief Populates a MQHCONN handle with an active, connected WebSphere MQ connection handle
	 *
	 */
	static void createLiveConnection(MQHCONN& subConn,
	                                 const std::string& qmanager,
	                                 const std::string& hostname, 
	                                 const std::string& channel,
	                                 const std::string& pCipherSpec,
	                                 const std::string& pKeyReposStem);
};


}  // end namespace gmsec_websphere

#endif  // GMSEC_WEBSPHERE_CONNECTION_H
