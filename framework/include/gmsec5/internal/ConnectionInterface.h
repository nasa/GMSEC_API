/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConnectionInterface.h
 *
 * @brief This file defines the interface that all middleware wrapper
 * connection classes need to implement.
 */

#ifndef GMSEC_API5_INTERNAL_CONNECTION_INTERFACE_H
#define GMSEC_API5_INTERNAL_CONNECTION_INTERFACE_H

#include <gmsec5_defs.h>

#include <gmsec5/internal/RequestSpecs.h>

#include <gmsec5/ConfigOptions.h>
#include <gmsec5/Connection.h>

#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/wdllexp.h>

#include <gmsec5/secure/Policy.h>

#include <string>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Config;
	class Message;

namespace internal
{
	// Forward declaration(s)
	class InternalConnection;
	class MiddlewareInfo;


/**
 * @class ConnectionInterface
 *
 * @brief Abstract class for representing a middleware connection.
 */
class GMSEC_API ConnectionInterface
{
protected:
	/**
	 * @fn ConnectionInterface()
	 *
	 * @brief Constructor.
	 */
	ConnectionInterface() : m_external(0) {}


public:
	/**
	 * @fn ~ConnectionInterface()
	 *
	 * @brief Destructor.
	 */
	virtual ~ConnectionInterface() {}


	/**
	 * @fn InternalConnection& getExternal()
	 *
	 * @brief Returns the pointer to the InternalConnection object that owns this object.
	 */
	InternalConnection& getExternal()
	{
		return *m_external;
	}


	/**
	 * @fn void registerExternal(InternalConnection* external)
	 *
	 * @brief Register parent InternalConnection object that is managing this
	 * ConnectionInterface.
	 */
	void setExternal(InternalConnection* external)
	{
		m_external = external;
	}


	/**
	 * @fn const char* getLibraryRootName()
	 *
	 * @brief Returns string that identifies the root library name, and therefore
	 * the connection type, that this connection is associated with.  For example,
	 * the root library name for the Bolt middleware library is "gmsec_bolt" and
	 * matches the base name of the Windows library (gmsec_bolt.dll) and the Linux
	 * library (libgmsec_bolt.so).
	 */
	virtual const char* CALL_TYPE getLibraryRootName() = 0;


	/**
	 * @fn const char* getLibraryVersion()
	 *
	 * @brief Returns a string containing the version information for this
	 * connection's associated middleware.
	 */
	virtual const char* CALL_TYPE getLibraryVersion() = 0;


	/**
	 * @fn const char* getMWInfo()
	 *
	 * @brief Returns string containing middleware information.
	 */
	virtual const char* CALL_TYPE getMWInfo() = 0;


	/**
	 * @fn mwConnect()
	 *
	 * @brief Establishes a connection to the middleware.
	 */
	virtual void CALL_TYPE mwConnect() = 0;


	/**
	 * @fn mwDisconnect()
	 *
	 * @brief Terminates the connection to the middleware.
	 */
	virtual void CALL_TYPE mwDisconnect() = 0;


	/**
	 * @fn mwSubscribe(const char* subject, const Config& config)
	 *
	 * @brief Subscribe to a particular subject or pattern and uses the provided
	 * configuration object to enable/disable special middleware-level subscription
	 * functionalities (e.g. ActiveMQ's durable consumer).
	 *
	 * @param subject - subject pattern to match received messages
	 * @param config  - configuration object
	 */
	virtual void CALL_TYPE mwSubscribe(const char* subject, const Config& config) = 0;


	/**
	 * @fn mwUnsubscribe(const char* subject)
	 *
	 * @brief Unsubscribes to a particular subject or pattern, thus stopping the
	 * reception of messages that match the pattern.
	 *
	 * @param subject - subject pattern that is currently in use to receive messages
	 */
	virtual void CALL_TYPE mwUnsubscribe(const char* subject) = 0;


	/**
	 * @fn mwPublish(const Message& msg, const Config& config)
	 *
	 * @brief Publishes the given message to the middleware using the provided
	 * configuration object.  The configuration object can be used to enable/disable
	 * special middleware-level functionalities (e.g. ActiveMQ's durable producer).
	 *
	 * @param msg    - message to publish
	 * @param config - configuration object
	 */
	virtual void CALL_TYPE mwPublish(const Message& msg, const Config& config) = 0;


	/**
	 * @fn mwRequest(const Message& request, const std::string& uniqueID)
	 *
	 * @brief Issues the request message with a unique ID.  The unique ID must be stored
	 * within the GMSEC_REPLY_UNIQUE_ID field within the sent message and returned.
	 *
	 * @param request  - the request message to issue to the middleware
	 * @param uniqueID - a unique string identifier for request/reply correlation purposes
	 *
	 * @see mwGetUniqueID()
	 */
	virtual void CALL_TYPE mwRequest(const Message& request, const std::string& uniqueID) = 0;


	/**
	 * @fn mwReceive(Message*& msg, GMSEC_I32 timeout)
	 *
	 * @brief Returns the next message received within the specified timeout.
	 * The received messages are determined by the existing subject pattern subscriptions.
	 *
	 * @param msg     - a message that has been received (if no error occurs)
	 * @param timeout - the maximum time to block waiting for a message, in milliseconds.
	 *                  If the timeout is -1, the function will block indefinely until a
	 *                  message is received.
	 */
	virtual void CALL_TYPE mwReceive(Message*& msg, GMSEC_I32 timeout) = 0;


	/**
	 * @fn mwAcknowledge(gmsec::api5::util::StdSharedPtr<MiddlewareInfo>& mwInfo)
	 *
	 * @brief For acknowledging the processing of a middleware message.
	 *
	 * @note Some middleware can retain a delivered message in a queue until said time
	 * it has been acknowledged as being processed. Should the message not be
	 * acknowledged, it can be delivered to another subscriber client.
	 *
	 * @param info - The MiddlewareInfo object containing data needed to ack a middleware message.
	 */
	virtual void CALL_TYPE mwAcknowledge(gmsec::api5::util::StdSharedPtr<MiddlewareInfo>& info) { /* no-op */ }


	/**
	 * @fn std::string getUniqueID()
	 *
	 * @brief Returns a unique identifier string to be included within a request messages.
	 *
	 * @return A string.
	 */
	virtual std::string mwGetUniqueID() = 0;


private:
	// Defined but not implemented
	ConnectionInterface(const ConnectionInterface&);
	ConnectionInterface& operator=(const ConnectionInterface&);


	// Pointer to the parent class that owns this ConnectionInterface.
	InternalConnection* m_external;
};


}  // end namespace internal
}  // end namespace api5
}  // end namespace gmsec

#endif
