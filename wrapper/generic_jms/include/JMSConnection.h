/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file JMSConnection.h
 *
 *	@brief This is the implementation of the Connection class for the generic JMS
 *	wrapper. All message traffic with the server is handled here.
 */

#ifndef GMSEC_JMS_CONNECTION_H
#define GMSEC_JMS_CONNECTION_H


#include "gmsec_generic_jms.h"

#include <gmsec4/internal/ConnectionInterface.h>

#include <gmsec4/internal/JMS.h>

#include <gmsec4/util/Atomics.h>
#include <gmsec4/util/Buffer.h>
#include <gmsec4/util/Condition.h>
#include <gmsec4/util/CountDownLatch.h>
#include <gmsec4/util/StdThread.h>

#include <queue>
#include <cstdlib>


namespace gmsec_generic_jms
{


class JMSConnection;


class MessagePullState
{
public:
	MessagePullState();

	virtual ~MessagePullState();

	void run();

	void stop();

	void initialize(JMSConnection* conn, JavaVM* jvm, const gmsec::api::jni::JMSCache* cache);

	void setMasterConsumer(jobject consumer, const std::string& s = "");

	bool getIsRunning() {
		return isRunning;
	}

	void setID(const std::string& id) {
		fID = id;
	}

private:
	gmsec::api::util::Condition      consumerSwitch;
	gmsec::api::util::AtomicBoolean  shouldStop;
	gmsec::api::util::CountDownLatch shutdownLatch;
	bool                             stopListening;
	bool                             isRunning;

	JMSConnection*                   parent;
	JNIEnv*                          envPoint;
	JavaVM*                          jvm;

	jobject                          masterConsumer;
	jobject                          newConsumer;

	std::string                      fID;
	std::string                      pendingSelector;

	const gmsec::api::jni::JMSCache* cache;
};


/**
 * @class JMSConnection
 *
 * @brief This is the interface between the GMSEC API and the JMS server. All
 * communication with the server goes through this class. Multiple connections may be held
 * by a given client application, but this affords no additional functionality
 */
class GMSEC_MW_API JMSConnection : public gmsec::api::internal::ConnectionInterface
{
public:
	enum Reasons
	{
		QUIT = gmsec::api::util::Condition::USER + 1,
		GOT_MESSAGE
	};


	/**
	 * @fn JMSConnection(const gmsec::api::Config& config)
	 *
	 * @brief This constructor accepts a standard gmsec config parameter and parses out several
	 * configuration values that are enumerated as follows.
	 *
	 * initial_context_factory - The class name of the initial context factory - middleware dependent
	 * provider_url - The url of the local file that supplies the jndi binding definition, if applicable
	 */
	JMSConnection(const gmsec::api::Config& config);


	/**
	 * @fn ~JMSConnection()
	 *
	 * @brief Standard deconstructor. Only real task is freeing the memory associated with
	 * subscription information.
	 */
	virtual ~JMSConnection();


	/**
	 * @fn getLibraryRootName()
	 *
	 * @brief This function simply returns the root name of the websphere library as defined below.
	 */
	virtual const char * CALL_TYPE getLibraryRootName();


	/**
	 * @fn getLibraryVersion()
	 *
	 * @brief This function gets the char* that contains the library version name.
	 */
	virtual const char* CALL_TYPE getLibraryVersion();


	/**
	 * @fn getMWInfo()
	 *
	 * @brief This function returns a const char * containing the middleware information
	 */
	virtual const char* CALL_TYPE getMWInfo();


	/**
	 * @fn mwConnect()
	 *
	 * @brief This function causes the connection to start communication with the server.
	 *
	 * @return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwConnect();


	/**
	 * @fn mwDisconnect()
	 *
	 * @brief This function ceases communication with the server, but the underlying connection
	 * is not destroyed.
	 *
	 * @return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwDisconnect();


	/**
	 * @fn mwSubscribe(const char* subject, const gmsec::api::Config& config)
	 *
	 * @brief This function begins a subscription to the given subject. All messages received pertaining
	 * to this subject heading will be placed on the queue for the connection object.
	 *
	 * @note Note that the subject as passed in will be converted from the standard GMSEC format to the WebSphere
	 * format.
	 *
	 * @param subject - The GMSEC formatted subsciption
	 *
	 * @param config - The GMSEC config containing subscribe operation options
	 *
	 * @return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwSubscribe(const char* subject, const gmsec::api::Config& config);


	/**
	 * @fn mwUnsubscribe(const char* subject)
	 *
	 * @brief This function removes subscription to a given topic by deleting the subscription info
	 * object, which in turn deletes the MessageConsumer, so messages are no longer forwarded to this
	 * connection by the server
	 *
	 * @param subject - The GMSEC formatted subscription
	 *
	 * @return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwUnsubscribe(const char* subject);


	/**
	 * @fn mwPublish(const gmsec::api::Message& msg, const gmsec::api::Config& config)
	 *
	 * @brief This function publishes a message by extracting its Destination and then publishing
	 * to the server based on that Destination
	 *
	 * @param msg - This is a reference to the message to be published.
	 *
	 * @param config - The GMSEC config containing publish operation options
	 *
	 * @return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwPublish(const gmsec::api::Message& msg, const gmsec::api::Config& config);


	/**
	 * @fn mwRequest(const gmsec::api::Message& request, std::string& id)
	 *
	 * @brief This function publishes a request via the same mechanism as the Publish method.
	 *
	 * @param request - This is a reference to the message that is to be published.
	 * @param id - The unique id that is assigned to this request call.
	 *
	 * @return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwRequest(const gmsec::api::Message& request, std::string& id);


	/**
	 * @fn mwReply(const gmsec::api::Message& request, const gmsec::api::Message& reply)
	 *
	 * @brief This function is called when an application receives a message and acknowledges
	 * that it was sent as a request due to its GMSEC_MSG_TYPE. The function takes references
	 * to two messages and publishes the second message based on the reply information supplied
	 * by the first message.
	 *
	 * @param request - This is a reference to the request.
	 * @param reply - This is a reference to the message that the application has generated in reply.
	 * It will be published based on information gathered from the request, namely by setting the
	 * destination of the reply to the value returned from calling getJMSReplyTo() on the request
	 * message.
	 *
	 * @return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwReply(const gmsec::api::Message& request, const gmsec::api::Message& reply);


	/**
	 * @fn mwReceive(gmsec::api::Message*& msg, GMSEC_I32 timeout)
	 *
	 * @brief This function is the basic mechanism used by applications to process incoming
	 * messages. All messages received by this connection are placed on the connection's queue,
	 * where they wait to be handed to the application by this function. This function will wait
	 * until signalled by a listener in the event that there is no available message upon calling
	 *
	 * @param msg - This is a reference to a pointer which will be directed to the message
	 * returned by the function.
	 * @param timeout - If there is no message immediately available, this is the length of time
	 * that the application should wait for one to be delivered. Note that this can be set to
	 * GMSEC_WAIT_FOREVER, in which case the function will hold indefinitely while waiting for a message.
	 *
	 * @return status - result of the connection operation
	 */
	virtual void CALL_TYPE mwReceive(gmsec::api::Message*& msg, GMSEC_I32 timeout);


	/**
	 * @fn handle(JNIEnv *jenv, jobject msg);
	 *
	 * @brief This function accepts (JNI) messages from the Message pull thread.
	 */
	bool handle(JNIEnv* jenv, jobject msg);


private:
	// Defined, but not implemented
	JMSConnection(const JMSConnection&);
	JMSConnection &operator=(const JMSConnection&);


	/**
	 * @fn handleReply(gmsec::api::Message* gmsecMessage)
	 *
	 * @brief This function hands off a reply message to the RequestThread of a connection
	 */
	void handleReply(gmsec::api::Message* gmsecMessage);


	/**
	 * @fn prepareBytes(const gmsec::api::Message& msg, gmsec::api::util::DataBuffer& buffer)
	 *
	 * @brief This function converts gmsec message to a bytes
	 */
	void prepareBytes(const gmsec::api::Message& msg, gmsec::api::util::DataBuffer& buffer);


	/**
	 * @fn unloadBytes(const gmsec::api::util::DataBuffer& buffer,
	 *                 gmsec::api::Message::MessageKind kind,
	 *                 gmsec::api::Message*& gmsecMessage,
	 *                 const std::string& rawSubject)
	 *
	 * @brief This function converts bytes to a gmsec message.
	 * The inSubject reference is set to the subject of the received
	 * message.  Note that in the case of a REPLY, the Message subject
	 * will be mapped.
	 */
	void unloadBytes(const gmsec::api::util::DataBuffer& buffer,
	                       gmsec::api::Message::MessageKind kind,
                           gmsec::api::Message*& gmsecMessage,
                           std::string& rawSubject);


	std::string generateUniqueId();


	std::string generateUniqueId(long id);


	void resetConsumer();


	bool addPropertiesToMessage(const std::string& topic, jobject msg, JNIEnv* chosenEnv);


	bool combineSelectors(std::string& selector);


	// Member data
	std::string                        fID;
	std::string                        initial_context_factory;
	std::string                        provider_url;

	std::string                        username;
	std::string                        password;
	std::string                        replyString;
	bool                               persistent;

	int                                uniquecounter;
	long                               requestCounter;

	JavaVM*                            jvm;

	jobject                            connectionFactory;
	jobject                            connection;
	jobject                            masterDestination;
	jobject                            inputSession;
	jobject                            outputSession;
	jobject                            masterProducer;

	gmsec::api::util::Condition        queueCondition;
	std::queue<gmsec::api::Message*>   queue;

	const gmsec::api::jni::JMSCache*   cache;

	MessagePullState                   pullState;
	gmsec::api::util::StdThread        pullThread;

	std::map<std::string, std::string> namesToSelector;

	std::string                        mwInfo;

	gmsec::api::internal::RequestSpecs requestSpecs;
};

}  // end namespace gmsec_generic_jms

#endif // GMSEC_JMS_CONNECTION_H
