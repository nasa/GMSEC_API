/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef API_4_ADAPTER_H
#define API_4_ADAPTER_H

#include <gmsec_defs.h>

#include <gmsec/ErrorCallback.h>
#include <gmsec/Status.h>

#include <gmsec4/Config.h>
#include <gmsec4/Connection.h>
#include <gmsec4/Message.h>
#include <gmsec4/Callback.h>
#include <gmsec4/EventCallback.h>
#include <gmsec4/ReplyCallback.h>

#include <gmsec4/field/Field.h>
#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
class Config;
class Connection;
class Field;
class Message;
class Callback;
class ReplyCallback;

namespace internal
{


class API4_Adapter
{
public:
	// Reference API 4.x message from API 3.x message
	//
	static gmsec::api::Message* referenceAPI4Message(Message* msg);


	// Convert 3.x Message Kind to 4.x equivalent
	//
	static gmsec::api::Message::MessageKind API3MsgKindToAPI4(GMSEC_MSG_KIND msgKind);


	// Convert 4.x Message Kind to 3.x equivalent
	//
	static int API4MsgKindToAPI3(gmsec::api::Message::MessageKind kind);


	// Convert 3.x Config to 4.x equivalent
	//
	static gmsec::api::Config API3ConfigToAPI4(const Config* config3);


	// Convert 4.x Config to 3.x equivalent
	//
	static Config* API4ConfigToAPI3(const gmsec::api::Config& cfg4);


	// Convert 3.x Field to 4.x equivalent
	//
	static gmsec::api::Field* API3FieldToAPI4(Field& field3);


	// Convert 4.x Field to 3.x equivalent
	//
	static Field* API4FieldToAPI3(const gmsec::api::Field& field4);


	// Convert 4.x Message to 3.x equivalent
	//
	static Message* API4MessageToAPI3(const gmsec::api::Message& msg4);


	// Convert 4.x Status Class to 3.x equivalent
	//
	static GMSEC_STATUS_CLASS API4StatusClassToAPI3(gmsec::api::StatusClass class4);


	// Convert 4.x Status Code to 3.x equivalent
	//
	static GMSEC_U32 API4StatusCodeToAPI3(gmsec::api::StatusCode code4);


	// Convert 4.x Exception to 3.x Status
	//
	static Status API4ExceptionToAPI3(gmsec::api::Exception& e4);


	// Convert 4.x Status to 3.x Status
	//
	static Status API4StatusToAPI3(const gmsec::api::Status& status4);


	// Convert 3.x Event string to 4.x Event
	//
	static gmsec::api::Connection::ConnectionEvent API3EventToAPI4(const char* event);


	// Convert 4.x Event to 3.x Event string
	//
	static const char* API4EventToAPI3(gmsec::api::Connection::ConnectionEvent event);


	// API 3.x ErrorCallback proxy
	//
	class GMSEC_API API4EventCallback : public gmsec::api::EventCallback
	{
	public:
		API4EventCallback(Connection* conn3, gmsec::ErrorCallback* ecb3);
		virtual void CALL_TYPE onEvent(gmsec::api::Connection& conn4, const gmsec::api::Status& status4, gmsec::api::Connection::ConnectionEvent event4);
		Connection*           m_conn3;
		gmsec::ErrorCallback* m_ecb3;
	};


	// API 3.x Callback proxy
	//
	class GMSEC_API API4Callback : public gmsec::api::Callback
	{
	public:
		API4Callback(Connection* conn3, gmsec::Callback* cb3);
		virtual void CALL_TYPE onMessage(gmsec::api::Connection& conn4, const gmsec::api::Message& msg4);
		Connection* m_conn3;
		gmsec::Callback*   m_cb3;
	};


	// API 3.x ReplyCallback proxy
	//
	class GMSEC_API API4ReplyCallback : public gmsec::api::ReplyCallback
	{
	public:
		API4ReplyCallback(Connection* conn3, gmsec::Callback* cb3, gmsec::ReplyCallback* rcb3);
		virtual void CALL_TYPE onReply(gmsec::api::Connection& conn4, const gmsec::api::Message& request4, const gmsec::api::Message& reply4);
		virtual void CALL_TYPE onEvent(gmsec::api::Connection& conn4, const gmsec::api::Status& status4, gmsec::api::Connection::ConnectionEvent event4);
		Connection*           m_conn3;
		gmsec::Callback*      m_cb3;
		gmsec::ReplyCallback* m_rcb3;
	};
};


}  // end namespace internal
}  // end namespace gmsec

#endif
