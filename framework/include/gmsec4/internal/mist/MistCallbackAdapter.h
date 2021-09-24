/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file MistCallbackAdapter.h
 *
 * @brief This file contains functions for the management of C++ callbacks that are
 * created in support of C function callbacks.
 */

#ifndef GMSEC_API_INTERNAL_MIST_CALLBACK_ADAPTER_H
#define GMSEC_API_INTERNAL_MIST_CALLBACK_ADAPTER_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>

#include <map>


namespace gmsec
{
namespace api
{
// Forward declaration(s)
class Message;

namespace mist
{
class ConnectionManager;
class ConnectionManagerCallback;
class ConnectionManagerEventCallback;
class ConnectionManagerReplyCallback;

namespace internal
{
class C_ConnMgrCallbackAdapter;
class C_ConnMgrEventCallbackAdapter;
class C_ConnMgrReplyCallbackAdapter;


class GMSEC_API MistCallbackAdapter
{
public:
	/**
	 * @fn MistCallbackAdapter
	 * @desc Constructor
	 */
	MistCallbackAdapter();


	/**
	 * @fn ~MistCallbackAdapter()
	 * @desc Destructor
	 */
	~MistCallbackAdapter();


	/**
	 * @fn ConnectionManagerCallback* createCallback(GMSEC_ConnectionMgrCallback* cb)
	 *
	 * @brief Creates a C++ Callback object that encapsulates the given C Callback function.
	 *
	 * @param cb - the C Callback function.
	 *
	 * @return A Callback object.
	 */
	ConnectionManagerCallback* CALL_TYPE createCallback(GMSEC_ConnectionMgrCallback* cb);


	/**
	 * @fn ConnectionManagerEventCallback* createEventCallback(GMSEC_ConnectionMgrEventCallback* ecb)
	 *
	 * @brief Creates a C++ EventCallback object that encapsulates the given C Event Callback function.
	 *
	 * @param ecb - the C Event Callback function.
	 *
	 * @return A EventCallback object.
	 */
	ConnectionManagerEventCallback* CALL_TYPE createEventCallback(GMSEC_ConnectionMgrEventCallback* ecb);


	/**
	 * @fn ConnectionManagerReplyCallback* createReplyCallback(GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb)
	 *
	 * @brief Creates a C++ ReplyCallback object that encapsulates the given C Reply Callback function.
	 *
	 * @param rcb - the C Reply Callback function.
	 * @param ecb - the C Event Callback function.
	 *
	 * @return A ReplyCallback object.
	 */
	ConnectionManagerReplyCallback* CALL_TYPE createReplyCallback(GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb);


	/**
	 * @fn void deleteCallback(GMSEC_ConnectionMgrCallback* cb)
	 *
	 * @brief Destroys the C++ Callback object that is associated with the given C Callback function.
	 *
	 * @param cb - the C Callback function.
	 */
	void CALL_TYPE deleteCallback(GMSEC_ConnectionMgrCallback* cb);


	/**
	 * @fn void deleteEventCallback(GMSEC_ConnectionMgrEventCallback* ecb)
	 *
	 * @brief Destroys the C++ EventCallback object that is associated with the given C Event Callback function.
	 *
	 * @param ecb - the C Event Callback function.
	 */
	void CALL_TYPE deleteEventCallback(GMSEC_ConnectionMgrEventCallback* ecb);


	/**
	 * @fn void deleteReplyCallback(GMSEC_ConnectionMgrReplyCallback* rcb)
	 *
	 * @brief Destroys the C++ ReplyCallback object that is associated with the given C Reply Callback function.
	 *
	 * @param rcb - the C Reply Callback function.
	 */
	void CALL_TYPE deleteReplyCallback(GMSEC_ConnectionMgrReplyCallback* rcb);


	/**
	 * @fn ConnectionManagerCallback* lookupCallback(GMSEC_ConnectionMgrCallback* cb)
	 *
	 * @brief Looks up the stored callback adapter for the given C callback function.
	 *
	 * @param cb - the C Callback function.
	 *
	 * @return The Callback serving as the proxy for the C callback function, or NULL if none exists.
	 */
	ConnectionManagerCallback* CALL_TYPE lookupCallback(GMSEC_ConnectionMgrCallback* cb);


	/**
	 * @fn ConnectionManagerEventCallback* lookupEventCallback(GMSEC_ConnectionMgrEventCallback* ecb)
	 *
	 * @brief Looks up the stored event callback adapter for the given C event callback function.
	 *
	 * @param ecb - the C Event Callback function.
	 *
	 * @return The EventCallback serving as the proxy for the C event callback function, or NULL if none exists.
	 */
	ConnectionManagerEventCallback* CALL_TYPE lookupEventCallback(GMSEC_ConnectionMgrEventCallback* ecb);


	/**
	 * @fn ConnectionManagerReplyCallback* lookupReplyCallback(GMSEC_ConnectionMgrReplyCallback* rcb)
	 *
	 * @brief Looks up the stored reply callback adapter for the given C reply callback function.
	 *
	 * @param rcb - the C Reply Callback function.
	 *
	 * @return The ReplyCallback serving as the proxy for the C reply callback function, or NULL if none exists.
	 */
	ConnectionManagerReplyCallback* CALL_TYPE lookupReplyCallback(GMSEC_ConnectionMgrReplyCallback* rcb);


private:
	// Defined, but not implemented.
	MistCallbackAdapter(const MistCallbackAdapter&);
	MistCallbackAdapter& operator=(const MistCallbackAdapter&);


	typedef std::map<GMSEC_ConnectionMgrCallback*,      C_ConnMgrCallbackAdapter*>      Callbacks;
	typedef std::map<GMSEC_ConnectionMgrEventCallback*, C_ConnMgrEventCallbackAdapter*> EventCallbacks;
	typedef std::map<GMSEC_ConnectionMgrReplyCallback*, C_ConnMgrReplyCallbackAdapter*> ReplyCallbacks;

	Callbacks      m_callbacks;
	EventCallbacks m_eventCallbacks;
	ReplyCallbacks m_replyCallbacks;
};


}  //end namespace internal
}  //end namespace mist
}  //end namespace api
}  //end namespace gmsec


#endif
