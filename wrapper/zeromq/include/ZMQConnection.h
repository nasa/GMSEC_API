/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
* @file ZMQConnection.h
* Holds the declaration of ZeroMQ subclass of gmsec::internal::BaseConnection.
*/

#ifndef GMSEC_ZMQ_CONNECTION_H
#define GMSEC_ZMQ_CONNECTION_H

#include "gmsec_zeromq.h"

#include <gmsec5_defs.h>

#include <gmsec5/internal/ConnectionInterface.h>
#include <gmsec5/internal/Encoder.h>
#include <gmsec5/internal/UniqueFilter.h>

#include <gmsec5/Message.h>
#include <gmsec5/Status.h>

#include <gmsec5/util/Buffer.h>
#include <gmsec5/util/Condition.h>
#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/StdThread.h>
#include <gmsec5/util/StdUniquePtr.h>

#include <queue>
#include <string>


namespace gmsec_zeromq
{

// Forward declaration(s)
class ZMQMessageListener;


/**
* @class ZMQConnection
* ZeroMQ implementation of the gmsec::Connection interface.
*/

class GMSEC_ZEROMQ_API ZMQConnection : public gmsec::api5::internal::ConnectionInterface
{
public:
	typedef gmsec::api5::util::StdUniquePtr<gmsec::api5::util::StdThread> MsgListenerThread;
	typedef gmsec::api5::util::StdSharedPtr<ZMQMessageListener>           SharedMsgListener;


private:
	// Used to determine the operation to use when connecting a socket to the network
	// Users can specify this in the mw-sub-endpoint or mw-pub-endpoint config option
	// (Ex. mw-sub-endpoint=connect:tcp://somehost:5561)
	enum SocketOperation
	{
		CONNECT,
		BIND,
		NONE
	};


	// Used to propagate errors from the MessageListener to the message queue
	struct GMSEC_ZEROMQ_API MessageSubscriptionResult
	{
		gmsec::api5::Status   status;
		gmsec::api5::Message* message;

		MessageSubscriptionResult(const gmsec::api5::Status& status, gmsec::api5::Message* msg)
			: status(status),
			  message(msg)
		{
		}
	};


	static void runMsgListener(SharedMsgListener shared);
	static void stopMsgListener(SharedMsgListener shared);


	// ZeroMQ Objects
	void*                                   m_context;
	void*                                   m_pubSocket;
	void*                                   m_subSocket;
	void*                                   m_repListenSocket;

	// ZeroMQ Configuration
	// The time that it takes after setting up a socket and calling bind/connect before network
	// activity will take place (Ie. a Message can be sent out without being dropped)
	int                                     m_settleTime;
	int                                     m_repListenPort;
	std::string                             m_subEndpoint;
	std::string                             m_pubEndpoint;
	std::string                             m_repListenEndpoint;

	// GMSEC Configuration
	std::string                             m_mwInfo;
	long                                    m_requestCounter; // Used by generateUniqueId
	int                                     m_uniqueCounter;  // Used by generateUniqueId

	MsgListenerThread                       m_msgListener;
	SharedMsgListener                       m_msgListenerObj;

	MsgListenerThread                       m_repListener;
	SharedMsgListener                       m_repListenerObj;

	gmsec::api5::internal::UniqueFilter     m_uniqueFilter;

	// The actual message queue, consisting of Status object/Message object pairs
	std::queue<MessageSubscriptionResult*>  m_msgQueue;	
	bool                                    m_threadSafe;
	bool                                    m_useUniqueFilter;
	bool                                    m_sigMismatchFlag;

	// Request/Reply Configuration
	gmsec::api5::internal::RequestSpecs     m_requestSpecs;

	// The URL endpoint added to messages to inform repliers where to set up their sockets to
	// replies to. (Unique to ZeroMQ)
	std::string                             m_replyAddress;	

	// Thread safety objects
	gmsec::api5::util::Condition            m_queueCondition;
	gmsec::api5::util::Condition            m_subscribeCondition;


	/**
	 * @fn cleanup();
	 * @brief Utility function for releasing resources.
	 */
	void cleanup();



	/**
	 * @fn mwPublishAux(const gmsec::api5::Message& message, const gmsec::api5::Config& config, void* socket)
	 * @brief Actual implementation of mwPublish -- Used by mwPublish.
	 */
	void mwPublishAux(const gmsec::api5::Message& message, const gmsec::api5::Config& config, void* socket);


	/**
	 * @fn storeProperties(const gmsec::api5::internal::ValueMap& meta, gmsec::api5::util::DataBuffer& out)
	 * @brief Encode the meta data associated with the message
	 */
	gmsec::api5::Status storeProperties(const gmsec::api5::internal::ValueMap& meta, gmsec::api5::util::DataBuffer& out);


	/**
	 * @fn findLength(const gmsec::api5::internal::ValueMap& meta, size_t& length)
	 * @brief Determine the size of the meta blob
	 */
	void findLength(const gmsec::api5::internal::ValueMap& meta, size_t& length);


	/**
	 * @fn Status storeProperty(gmsec::api5::util::Encoder* encoder, const char* valueId, const gmsec::api5::internal::Value& value, char** currentBuffer)
	 * @brief Encode the gmsec::internal::Value into the meta blob
	 */
	gmsec::api5::Status storeProperty(const char* valueId, const gmsec::api5::internal::Value& value, char** currentBuffer);


	/**
	 * @fn parseProperties(gmsec::api5::internal::ValueMap& meta, const gmsec::api5::util::DataBuffer& out, gmsec::api5::Message& message)
	 * @brief Decode the meta data associated with the message
	 */
	gmsec::api5::Status parseProperties(gmsec::api5::internal::ValueMap& meta, const gmsec::api5::util::DataBuffer& out, gmsec::api5::Message& message);


	/**
	 * @fn mwReceive(gmsec::api5::Message*& msg, GMSEC_I32 timeout)
	 * @brief Actual implementation of mwGetNextMsg.
	 */
	void mwReceiveAux(gmsec::api5::Message*& msg, GMSEC_I32 timeout);


	/**
	 * @fn setupSocket(void* socket, int type, const std::string& providedEndpoint, bool replyListener = false)
	 * @brief Helper function to construct and initialize ZeroMQ Socket objects
	 */
	void setupSocket(void** socket, int type, const std::string& providedEndpoint, bool replyListener = false);


	/**
	 * @fn bindSocket(void* socket, const std::string& endpoint)
	 * @brief Helper function to bind a ZeroMQ socket to an endpoint and handle error checking 
	 */
	void bindSocket(void** socket, const std::string& endpoint);


	/**
	 * @fn connectSocket(void* socket, const std::string& endpoint)
	 * @brief Helper function to connect a ZeroMQ socket to an endpoint and handle error checking 
	 */
	void connectSocket(void** socket, const std::string& endpoint);


	/**
	 * @fn enum SocketOperation getSocketOperation(const std::string& endpoint)
	 * @brief Helper function to determine the socket operation from the URL string
	 */
	SocketOperation getSocketOperation(const std::string& endpoint);


	/**
	 * @fn const char* getUrl(const std::string& endpoint)
	 * @brief Helper function to determine the URL from the URL string
	 * (Which may also contain the socket operation)
	 */
	std::string getUrl(const std::string& endpoint);


	/**
	 * @fn const char* getEndpointKey(const std::string& endpoint)
	 * @brief Helper function to determine the Endpoint Key (for the
	 * HeartbeatPublisher) from the URL string (Which may also contain
	 * the socket operation)
	 */
	std::string getEndpointKey(const std::string& endpoint);



	/**
	 * @fn enqueueResult(const gmsec::api5::Status& status, gmsec::api5::Message* message = NULL)
	 * @brief Helper function to add a GMSEC Message to the message queue
	 */
	void enqueueResult(const gmsec::api5::Status& status, gmsec::api5::Message* message = NULL);


	/**
	 * @fn generateUniqueId(long id)
	 * @brief Generate a Unique ID for Request Messages.
	 */
	std::string generateUniqueId(long id); // May not be needed (ZMQ may provide its own Request IDs when using ZEROMQ_REQ_CORRELATE)


	/**
	 * @fn zmqErrorToException(const std::string& errorMsg, const int errnoCode)
	 * @brief Helper function to package ZeroMQ errno codes into a GMSEC Status object
	 */
	void zmqErrorToException(const std::string& errorMsg, const int errnoCode);


	/**
	 * @fn hostnameToIpv4(const std::string& hostname, std::string& ip)
	 * @brief Helper function to translate the provided hostname into an IP
	 */
	bool hostnameToIpv4(const std::string& hostname, std::string& ip);


public:
	/**
	 * @fn ZMQConnection(const gmsec::api5::Config& config)
	 * @brief Create a ZMQ Connection with configuration.
	 */
	ZMQConnection(const gmsec::api5::Config& config);


	/**
	 * @brief Destroy a ZMQ Connection.
	 */
	virtual ~ZMQConnection();


	/**
	 * @brief Determines the middleware wrapper tag.
	 */
	virtual const char* getLibraryRootName()
	{
		return ZEROMQ_VERSION_STRING; // defined in the gmsec_zmq.h
	}

	/**
	 * @brief Determines the middleware version string.
	 */
	virtual const char* getLibraryVersion();


	/**
	 * @brief This function returns the middleware information
	 */
	virtual const char* getMWInfo();


	/**
	 * @brief This function does nothing for ZeroMQ.
	 * @see gmsec::Connection::Connect
	 */
	virtual void mwConnect();


	/**
	 * @brief Disconnect from the middleware server.
	 * @see gmsec::Connection::Disconnect
	 */
	virtual void mwDisconnect();


	/**
	 * @brief Subscribe to @p subject (which possibly contains wildcards)
	 * using the configuration supplied in the config object.
	 */
	virtual void mwSubscribe(const char* subject, const gmsec::api5::Config& config);


	/**
	 * @brief Unsubscribe from @p subject (which possibly contains wildcards).
	 */
	virtual void mwUnsubscribe(const char* subject);



	/**
	 * @brief Publish message @p msg using @p configuration.
	 */
	virtual void mwPublish(const gmsec::api5::Message& msg, const gmsec::api5::Config& config);


	/**
	 * @brief Send request message.
	 */
	virtual void mwRequest(const gmsec::api5::Message& request, std::string& id);


	/**
	 * @brief Wait up to @p timeout milliseconds for next message.
	 */
	virtual void mwReceive(gmsec::api5::Message*& msg, GMSEC_I32 timeout);


	/**
	 * handleMessage(zmq_msg_t* zmqMessage, int zmqMsgSize, zmq_msg_t* metaMessage, const int& metaSize, const char* subject)
	 * @brief Helper function to decode a GMSEC Message
	 */
	void handleMessage(zmq_msg_t* zmqMessage, int zmqMsgSize, zmq_msg_t* metaMessage, int metaSize, const char* subject);
};


}  // end namespace gmsec_zeromq

#endif
