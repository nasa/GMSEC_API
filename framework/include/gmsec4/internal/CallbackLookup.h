/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file CallbackLookup.h
 *
 *  @brief This file contains a class responsible for matching incoming messages
 *  with subscriptions (including patterns).
 */

#ifndef GMSEC_API_INTERNAL_CALLBACK_LOOKUP_H
#define GMSEC_API_INTERNAL_CALLBACK_LOOKUP_H


#include <gmsec4/internal/ci_less.h>

#include <gmsec4/Callback.h>

#include <gmsec4/util/wdllexp.h>

#include <algorithm>
#include <list>
#include <map>


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


/** @class CallbackLookup
 *
 *	@brief This class keeps track of local subscriptions so that incoming messages
 *	can be matched with the callbacks that need to be called. This class is
 *	used internal to the Connection object and should not be used outside the
 *	GMSECAPI library.
 */
class GMSEC_API CallbackLookup
{
private:
	// Declared, but not implemented.
	CallbackLookup(const CallbackLookup &);
	CallbackLookup &operator=(const CallbackLookup &);

	typedef std::list<Callback*>         callbackList;
	typedef callbackList::const_iterator callbackItr;

	callbackList fCallbacks;

	typedef std::map<std::string, CallbackLookup*, ci_less> callbackLkp;
	typedef callbackLkp::const_iterator                     callbackLkpItr;
	typedef callbackLkp::iterator                           callbackLkpItrNC;

	callbackLkp fCallbackLkps;
	
	void collectCallbacksFinalStep(callbackList &callbacks, callbackList &fCallbacks);

public:
	CallbackLookup();


	virtual ~CallbackLookup();


	/** @fn addCallback(const char* subject, Callback *cb)
	 *
	 *	@brief This function registers a subject or pattern with a Callback to
	 *	be called when messages that match are received.
	 */
	virtual void CALL_TYPE addCallback(const char* subject, Callback* cb);


	/** @fn removeCallback(const char* subject)
	 *
	 *	@brief This function removes all Callbacks that are registed for a
	 *	certain subject or pattern.
	 */
	virtual void CALL_TYPE removeCallback(const char* subject);


	/** @fn removeCallback(const char* subject, Callback* cb)
	 *
	 *	@brief This function removes a particular Callback that is registed for a
	 *	certain subject or pattern.
	 */
	virtual void CALL_TYPE removeCallback(const char* subject, Callback* cb);


	virtual void CALL_TYPE collectCallbacks(const char* subject, callbackList& callbacks);


	/*
	 * NOTE:
	 *
	 * dispatchMsg() and getCallbackCount() are being kept for historical reasons (MBServer uses them).
	 */

	/** @fn dispatchMsg(const char* subject, const Message& msg )
	 *
	 *	@brief This function is called by MBServer to dispatch a
	 *	received message to all the callbacks that are registerd to
	 *	subject or pattern that matches the message's subject.
	 */
	virtual void CALL_TYPE dispatchMsg(const char* subject, const Message& msg);


	/** @fn getCallbackCount(const char* subject);
	 *
	 *	@brief This function returns the number of callbacks subscribed
	 *	the given subject.
	 */
	virtual size_t CALL_TYPE getCallbackCount(const char* subject);
};


} // namespace internal
} // namespace api
} // namespace gmsec

#endif
