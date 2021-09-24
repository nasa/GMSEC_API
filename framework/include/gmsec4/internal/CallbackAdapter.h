/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file CallbackAdapter.h
 *
 * @brief This file contains functions for the management of C++ callbacks that are
 * created in support of C function callbacks.
 */

#ifndef GMSEC_API_INTERNAL_CALLBACK_ADAPTER_H
#define GMSEC_API_INTERNAL_CALLBACK_ADAPTER_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>

#include <map>


namespace gmsec
{
namespace api
{
// Forward declaration(s)
class Callback;
class Connection;
class EventCallback;
class Message;
class ReplyCallback;

namespace internal
{
class C_CallbackAdapter;
class C_EventCallbackAdapter;
class C_ReplyCallbackAdapter;


class GMSEC_API CallbackAdapter
{
public:
	/**
	 * @fn CallbackAdapter
	 * @desc Constructor
	 */
	CallbackAdapter();


	/**
	 * @fn ~CallbackAdapter()
	 * @desc Destructor
	 */
	~CallbackAdapter();


	/**
	 * @fn Callback* createCallback(GMSEC_Callback* cb)
	 *
	 * @brief Creates a C++ Callback object that encapsulates the given C Callback function.
	 *
	 * @param cb - the C Callback function.
	 *
	 * @return A Callback object.
	 */
	Callback* CALL_TYPE createCallback(GMSEC_Callback* cb);


	/**
	 * @fn EventCallback* createEventCallback(GMSEC_EventCallback* ecb)
	 *
	 * @brief Creates a C++ EventCallback object that encapsulates the given C Event Callback function.
	 *
	 * @param ecb - the C Event Callback function.
	 *
	 * @return A EventCallback object.
	 */
	EventCallback* CALL_TYPE createEventCallback(GMSEC_EventCallback* ecb);


	/**
	 * @fn ReplyCallback* createReplyCallback(GMSEC_ReplyCallback* rcb)
	 *
	 * @brief Creates a C++ ReplyCallback object that encapsulates the given C Reply Callback function.
	 *
	 * @param rcb - the C Reply Callback function.
	 * @param ecb - the C Event Callback function.
	 *
	 * @return A ReplyCallback object.
	 */
	ReplyCallback* CALL_TYPE createReplyCallback(GMSEC_ReplyCallback* rcb, GMSEC_EventCallback* ecb);


	/**
	 * @fn void deleteCallback(GMSEC_Callback* cb)
	 *
	 * @brief Destroys the C++ Callback object that is associated with the given C Callback function.
	 *
	 * @param cb - the C Callback function.
	 */
	void CALL_TYPE deleteCallback(GMSEC_Callback* cb);


	/**
	 * @fn void deleteEventCallback(GMSEC_EventCallback* ecb)
	 *
	 * @brief Destroys the C++ EventCallback object that is associated with the given C Event Callback function.
	 *
	 * @param ecb - the C Event Callback function.
	 */
	void CALL_TYPE deleteEventCallback(GMSEC_EventCallback* ecb);


	/**
	 * @fn void deleteReplyCallback(GMSEC_ReplyCallback* rcb)
	 *
	 * @brief Destroys the C++ ReplyCallback object that is associated with the given C Reply Callback function.
	 *
	 * @param rcb - the C Reply Callback function.
	 */
	void CALL_TYPE deleteReplyCallback(GMSEC_ReplyCallback* rcb);


	/**
	 * @fn Callback* lookupCallback(GMSEC_Callback* cb)
	 *
	 * @brief Looks up the stored callback adapter for the given C callback function.
	 *
	 * @param cb - the C Callback function.
	 *
	 * @return The Callback serving as the proxy for the C callback function, or NULL if none exists.
	 */
	Callback* CALL_TYPE lookupCallback(GMSEC_Callback* cb);


	/**
	 * @fn EventCallback* lookupEventCallback(GMSEC_EventCallback* ecb)
	 *
	 * @brief Looks up the stored event callback adapter for the given C event callback function.
	 *
	 * @param ecb - the C Event Callback function.
	 *
	 * @return The EventCallback serving as the proxy for the C event callback function, or NULL if none exists.
	 */
	EventCallback* CALL_TYPE lookupEventCallback(GMSEC_EventCallback* ecb);


	/**
	 * @fn ReplyCallback* lookupReplyCallback(GMSEC_ReplyCallback* rcb)
	 *
	 * @brief Looks up the stored reply callback adapter for the given C reply callback function.
	 *
	 * @param rcb - the C Reply Callback function.
	 *
	 * @return The ReplyCallback serving as the proxy for the C reply callback function, or NULL if none exists.
	 */
	ReplyCallback* CALL_TYPE lookupReplyCallback(GMSEC_ReplyCallback* rcb);


private:
	// Defined, but not implemented.
	CallbackAdapter(const CallbackAdapter&);
	CallbackAdapter& operator=(const CallbackAdapter&);


	typedef std::map<GMSEC_Callback*, C_CallbackAdapter*>           Callbacks;
	typedef std::map<GMSEC_EventCallback*, C_EventCallbackAdapter*> EventCallbacks;
	typedef std::map<GMSEC_ReplyCallback*, C_ReplyCallbackAdapter*> ReplyCallbacks;

	Callbacks      m_callbacks;
	EventCallbacks m_eventCallbacks;
	ReplyCallbacks m_replyCallbacks;
};


}  //end namespace internal
}  //end namespace api
}  //end namespace gmsec


#endif
