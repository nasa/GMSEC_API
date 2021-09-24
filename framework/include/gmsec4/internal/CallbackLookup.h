/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file CallbackLookup.h
 *
 * @brief This file contains a class responsible for matching incoming messages
 * with subscriptions (including patterns).
 */

#ifndef GMSEC_API_INTERNAL_CALLBACK_LOOKUP_H
#define GMSEC_API_INTERNAL_CALLBACK_LOOKUP_H

#include <gmsec4/Callback.h>

#include <gmsec4/util/StdUniquePtr.h>
#include <gmsec4/util/wdllexp.h>

#include <list>
#include <map>
#include <string>
#include <vector>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Message;

namespace internal
{


/**
 * @class MBCallback
 *
 * @brief Specialized callback interface kludge dedicated to MagicBus.
 */
class GMSEC_API MBCallback : public Callback
{
public:
	virtual void onMessage(const Message& msg) = 0;
};


/**
 * @class CallbackLookup
 *
 * @brief This class keeps track of local subscriptions so that incoming messages
 * can be matched with the callbacks that need to be called. This class is
 * used internal to the Connection object and should not be used outside the
 * GMSECAPI library (oh, but with the exception of Message Bus!!!).
 */
class GMSEC_API CallbackLookup
{
private:
	typedef std::list<Callback*>         CallbackList;
	typedef CallbackList::const_iterator CallbackItr;

public:
	CallbackLookup();


	virtual ~CallbackLookup();


	/**
	 * @fn addCallback(const char* topic, Callback *cb)
	 *
	 * @brief This function registers a topic with a Callback to be called when messages that match are received.
	 */
	virtual void CALL_TYPE addCallback(const char* topic, Callback* cb);


	/**
	 * @fn removeCallback(const char* topic)
	 *
	 * @brief This function removes all Callbacks that are registed for a certain topic.
	 */
	virtual void CALL_TYPE removeCallback(const char* topic);


	/**
	 * @fn removeCallback(const char* topic, Callback* cb)
	 *
	 * @brief This function removes a particular Callback that is registed for a certain topic.
	 */
	virtual void CALL_TYPE removeCallback(const char* topic, Callback* cb);


	/**
	 * @fn void collectCallbacks(const char* msgSubject, CallbackList& callbacks)
	 *
	 * @brief Returns a list of callbacks associated with subscribed topics that match the
	 * given message subject.
	 */
	virtual void CALL_TYPE collectCallbacks(const char* msgSubject, CallbackList& callbacks);


	/*
	 * NOTE:
	 * dispatchMsg() and getCallbackCount() are being kept for historical reasons (MBServer uses them).
	 */

	/**
	 * @fn dispatchMsg(const Message& msg)
	 *
	 * @brief This function is called by MBServer to dispatch a
	 * received message to all the callbacks that are registerd to
	 * subject or pattern that matches the message's subject.
	 */
	virtual void CALL_TYPE dispatchMsg(const Message& msg);


	/**
	 * @fn getCallbackCount(const char* msgSubject)
	 *
	 * @brief This function returns the number of callbacks subscribed to the given subject.
	 *
	 * @sa collectCallbacks()
	 */
	size_t CALL_TYPE getCallbackCount(const char* msgSubject);


private:
	// Declared, but not implemented.
	CallbackLookup(const CallbackLookup &);
	CallbackLookup &operator=(const CallbackLookup &);


	typedef std::vector<std::string>           Elements;
	typedef std::multimap<Callback*, Elements> CallbackRegister;

	gmsec::api::util::StdUniquePtr<CallbackRegister> m_callbackRegister;
};


} // namespace internal
} // namespace api
} // namespace gmsec

#endif
