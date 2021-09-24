
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/* @file VoidConnection.h
 *  This file provides a template for implementing a middleware wrapper.
 */

#ifndef _VoidConnection_h_
#define _VoidConnection_h_

#include <gmsec_void.h>

#include <gmsec/internal/BaseConnection.h>

#include <VoidMessage.h>


/** @class VoidConnection
 * This class provides a sample (dummy) implementation of the
 * BaseConnection abstract class.
*/
class GMSEC_VOID_API VoidConnection
		:	public gmsec::internal::BaseConnection
{
private:

public:

	/** @fn VoidConnection(gmsec::Config *cfg)
	* Standard constructor that requires a config
	*/
	VoidConnection(gmsec::Config *cfg);
	virtual ~VoidConnection();

	/** @fn CloneMessage( gmsec::Message *in, gmsec::Message *&out )
	* This function copies a message without knowing what type it is
	*/
	virtual gmsec::Status CALL_TYPE CloneMessage(gmsec::Message *in, gmsec::Message *&out);

	/** @fn GetLibraryVersion()
	* Retrieve the version of the underlying middleware libraries
	*/
	virtual const char * CALL_TYPE GetLibraryVersion();

	/** @fn GetLibraryRootName()
	* Retrieve the root library name
	*/
	virtual const char * CALL_TYPE GetLibraryRootName()
	{
		return "gmsec_void";
	}

	/** @fn mwConnect()
	* @brief Establish connection to the server.
	*/
	virtual gmsec::Status CALL_TYPE mwConnect();

	/** @fn mwDisconnect()
	* @brief End connection to the server.
	*/
	virtual gmsec::Status CALL_TYPE mwDisconnect();

	/** @fn mwSubscribe(const char *subject, const gmsec::Config &config)
	* @brief Subscribe to a subject without a callback
	*/
	virtual gmsec::Status CALL_TYPE mwSubscribe(const char *subject, const gmsec::Config &config);

	/** @fn mwUnSubscribe(const char *subject)
	* @brief Unsubscribe from a subject
	*/
	virtual gmsec::Status CALL_TYPE mwUnSubscribe(const char *subject);

	/** @fn mwCreateMessage(const char *subject, GMSEC_MSG_KIND msgKind, gmsec::Message *&msg)
	* @brief Create a middleware specific message.
	*/
	virtual gmsec::Status CALL_TYPE mwCreateMessage(const char *subject, GMSEC_MSG_KIND msgKind, gmsec::Message *&msg);

	/** @fn DestroyMessage(gmsec::Message *msg)
	* destroy an void message
	*/
	virtual gmsec::Status CALL_TYPE DestroyMessage(gmsec::Message *msg);

	/** @fn mwPublish(gmsec::Message *msg, const gmsec::Config &config)
	* @brief Send the message to the middleware.
	*/
	virtual gmsec::Status CALL_TYPE mwPublish(gmsec::Message *msg, const gmsec::Config &config);

	/** @fn mwRequest(gmsec::Message *request, long timeout, gmsec::Message *&reply)
	* @brief Send a request message to the middleware.
	*/
	virtual gmsec::Status CALL_TYPE mwRequest(gmsec::Message *request, std::string &id);

	/** @fn mwReply(gmsec::Message *request,gmsec::Message *reply)
	* @brief Send a reply message to the middleware.
	*/
	virtual gmsec::Status CALL_TYPE mwReply(gmsec::Message *request,gmsec::Message *reply);

	/** @fn GetNextMsg(gmsec::Message *&msg, long timeout)
	* pull the next message off the inbound queue
	*/
	virtual gmsec::Status CALL_TYPE mwGetNextMsg(gmsec::Message *&msg, long timeout);

};
#endif	//	VoidConnection_h
