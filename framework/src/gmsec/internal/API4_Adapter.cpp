/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec/internal/API4_Adapter.h>

#include <gmsec/internal/InternalMessage.h>
#include <gmsec/internal/Log.h>
#include <gmsec/internal/MessageBuddy.h>

#include <gmsec/Config.h>
#include <gmsec/Connection.h>
#include <gmsec/Field.h>
#include <gmsec/Message.h>

#include <gmsec/Callback.h>
#include <gmsec/ErrorCallback.h>
#include <gmsec/ReplyCallback.h>

#include <gmsec4/Exception.h>

#include <gmsec4/internal/InternalStatus.h>
#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/StringUtil.h>

#include <memory>


using namespace gmsec;
using namespace gmsec::internal;


// Reference API 4.x message from API 3.x message
//
gmsec::api::Message* API4_Adapter::referenceAPI4Message(Message* msg)
{
	return MessageBuddy::getInternal(msg)->getAdapter();
}


// Convert 3.x Message Kind to 4.x equivalent
//
gmsec::api::Message::MessageKind API4_Adapter::API3MsgKindToAPI4(GMSEC_MSG_KIND msgKind)
{
	switch (msgKind)
	{
	case GMSEC_MSG_UNSET:
	case GMSEC_MSG_PUBLISH: return gmsec::api::Message::PUBLISH;
	case GMSEC_MSG_REQUEST: return gmsec::api::Message::REQUEST;
	case GMSEC_MSG_REPLY:   return gmsec::api::Message::REPLY;
	}

	return gmsec::api::Message::PUBLISH;
}


// Convert 4.x Message Kind to 3.x equivalent
//
int API4_Adapter::API4MsgKindToAPI3(gmsec::api::Message::MessageKind kind)
{
	switch (kind)
	{
	case gmsec::api::Message::PUBLISH: return GMSEC_MSG_PUBLISH;
	case gmsec::api::Message::REQUEST: return GMSEC_MSG_REQUEST;
	case gmsec::api::Message::REPLY:   return GMSEC_MSG_REPLY;
	}

	return GMSEC_MSG_UNSET;
}


// Convert 3.x Config to 4.x equivalent
//
gmsec::api::Config API4_Adapter::API3ConfigToAPI4(const Config* config3)
{
	gmsec::api::Config config4;

	if (config3 != NULL)
	{
		const char* key   = 0;
		const char* value = 0;

		Status status = config3->GetFirst(key, value);

		while (!status.isError() && key && value)
		{
			config4.addValue(key, value);

			status = config3->GetNext(key, value);
		}
	}

	return config4;
}


// Convert 4.x Config to 3.x equivalent
//
Config* API4_Adapter::API4ConfigToAPI3(const gmsec::api::Config& cfg4)
{
	Config* cfg3 = new Config();

	const char* name   = 0;
	const char* value  = 0;
	bool        result = cfg4.getFirst(name, value);

	while (result)
	{
		cfg3->AddValue(name, value);

		result = cfg4.getNext(name, value);
	}

	return cfg3;
}


// Convert 3.x Field to 4.x equivalent
//
gmsec::api::Field* API4_Adapter::API3FieldToAPI4(Field& field3)
{
	static bool firstTime = true;
	if (firstTime)
	{
		firstTime = false;
		gmsec::api::util::StringConverter::instance().setMode(gmsec::api::util::StringConverter::NO_CONVERSION);
	}

	GMSEC_TYPE fieldType;
	Status     status = field3.GetType(fieldType);

	if (status.isError())
	{
		return NULL;
	}

	const char* fieldName = NULL;

	field3.GetName(fieldName);

	if (fieldName == NULL)
	{
		return NULL;
	}

	gmsec::api::Field* field4 = NULL;

	switch (fieldType)
	{
		case GMSEC_TYPE_CHAR:
		{
			GMSEC_CHAR value;
			if (!field3.GetValue(value).isError())
			{
				field4 = new gmsec::api::CharField(fieldName, value);
			}
		}
		break;

		case GMSEC_TYPE_BOOL:
		{
			GMSEC_BOOL value;
			if (!field3.GetValue(value).isError())
			{
				field4 = new gmsec::api::BooleanField(fieldName, value == GMSEC_TRUE);
			}
		}
		break;

		case GMSEC_TYPE_I16:
		{
			GMSEC_I16 value;
			if (!field3.GetValue(value).isError())
			{
				field4 = new gmsec::api::I16Field(fieldName, value);
			}
		}
		break;

		case GMSEC_TYPE_U16:
		{
			GMSEC_U16 value;
			if (!field3.GetValue(value).isError())
			{
				field4 = new gmsec::api::U16Field(fieldName, value);
			}
		}
		break;

		case GMSEC_TYPE_I32:
		{
			GMSEC_I32 value;
			if (!field3.GetValue(value).isError())
			{
				field4 = new gmsec::api::I32Field(fieldName, value);
			}
		}
		break;

		case GMSEC_TYPE_U32:
		{
			GMSEC_U32 value;
			if (!field3.GetValue(value).isError())
			{
				field4 = new gmsec::api::U32Field(fieldName, value);
			}
		}
		break;

		case GMSEC_TYPE_F32:
		{
			GMSEC_F32 value;
			if (!field3.GetValue(value).isError())
			{
				field4 = new gmsec::api::F32Field(fieldName, value);
			}
		}
		break;

		case GMSEC_TYPE_F64:
		{
			GMSEC_F64 value;
			if (!field3.GetValue(value).isError())
			{
				field4 = new gmsec::api::F64Field(fieldName, value);
			}
		}
		break;

		case GMSEC_TYPE_STRING:
		{
			const char* value;
			if (!field3.GetValue(value).isError() && value)
			{
				field4 = new gmsec::api::StringField(fieldName, value);
			}
		}
		break;

		case GMSEC_TYPE_BIN:
		{
			GMSEC_BIN value;
			GMSEC_U32 size;

			if (!field3.GetValue(&value, size).isError())
			{
				field4 = new gmsec::api::BinaryField(fieldName, value, size);
			}
		}
		break;

		case GMSEC_TYPE_I8:
		{
			GMSEC_I8 value;
			if (!field3.GetValue(value).isError())
			{
				field4 = new gmsec::api::I8Field(fieldName, value);
			}
		}
		break;

		case GMSEC_TYPE_U8:
		{
			GMSEC_U8 value;
			if (!field3.GetValue(value).isError())
			{
				field4 = new gmsec::api::U8Field(fieldName, value);
			}
		}
		break;

		case GMSEC_TYPE_I64:
		{
			GMSEC_I64 value;
			if (!field3.GetValue(value).isError())
			{
				field4 = new gmsec::api::I64Field(fieldName, value);
			}
		}
		break;

		case GMSEC_TYPE_U64:
		{
			GMSEC_U64 value;
			if (!field3.GetValue(value).isError())
			{
				field4 = new gmsec::api::U64Field(fieldName, value);
			}
		}
		break;

		case GMSEC_TYPE_UNSET:
		case GMSEC_TYPE_COMPOUND:
		{
			// unsupported in API 4.x
		}
		break;
	}

	if (field4 != NULL)
	{
		field4->isHeader(field3.IsHeader());
	}

	return field4;
}


// Convert Field from 4.x to 3.x equivalent
//
Field* API4_Adapter::API4FieldToAPI3(const gmsec::api::Field& field4)
{
	Field* field3 = NULL;

	switch (field4.getType())
	{
		case gmsec::api::Field::CHAR_TYPE:
		{
			const gmsec::api::CharField& childField = (const gmsec::api::CharField&) field4;
			field3 = new Field(childField.getName(), (GMSEC_CHAR) childField.getValue());
		}
		break;

		case gmsec::api::Field::BOOL_TYPE:
		{
			const gmsec::api::BooleanField& childField = (const gmsec::api::BooleanField&) field4;
			field3 = new Field(childField.getName(), (GMSEC_BOOL) (childField.getValue() ? GMSEC_TRUE : GMSEC_FALSE));
		}
		break;

		case gmsec::api::Field::I16_TYPE:
		{
			const gmsec::api::I16Field& childField = (const gmsec::api::I16Field&) field4;
			field3 = new Field(childField.getName(), (GMSEC_I16) childField.getValue());
		}
		break;

		case gmsec::api::Field::U16_TYPE:
		{
			const gmsec::api::U16Field& childField = (const gmsec::api::U16Field&) field4;
			field3 = new Field(childField.getName(), (GMSEC_U16) childField.getValue());
		}
		break;

		case gmsec::api::Field::I32_TYPE:
		{
			const gmsec::api::I32Field& childField = (const gmsec::api::I32Field&) field4;
			field3 = new Field(childField.getName(), (GMSEC_I32) childField.getValue());
		}
		break;

		case gmsec::api::Field::U32_TYPE:
		{
			const gmsec::api::U32Field& childField = (const gmsec::api::U32Field&) field4;
			field3 = new Field(childField.getName(), (GMSEC_U32) childField.getValue());
		}
		break;

		case gmsec::api::Field::F32_TYPE:
		{
			const gmsec::api::F32Field& childField = (const gmsec::api::F32Field&) field4;
			field3 = new Field(childField.getName(), (GMSEC_F32) childField.getValue());
		}
		break;

		case gmsec::api::Field::F64_TYPE:
		{
			const gmsec::api::F64Field& childField = (const gmsec::api::F64Field&) field4;
			field3 = new Field(childField.getName(), (GMSEC_F64) childField.getValue());
		}
		break;

		case gmsec::api::Field::STRING_TYPE:
		{
			const gmsec::api::StringField& childField = (const gmsec::api::StringField&) field4;
			field3 = new Field(childField.getName(), childField.getValue());
		}
		break;

		case gmsec::api::Field::BIN_TYPE:
		{
			const gmsec::api::BinaryField& childField = (const gmsec::api::BinaryField&) field4;
			field3 = new Field(childField.getName(), (GMSEC_BIN) childField.getValue(), (GMSEC_U32) childField.getLength());
		}
		break;

		case gmsec::api::Field::I8_TYPE:
		{
			const gmsec::api::I8Field& childField = (const gmsec::api::I8Field&) field4;
			field3 = new Field(childField.getName(), (GMSEC_I8) childField.getValue());
		}
		break;

		case gmsec::api::Field::U8_TYPE:
		{
			const gmsec::api::U8Field& childField = (const gmsec::api::U8Field&) field4;
			field3 = new Field(childField.getName(), (GMSEC_U8) childField.getValue());
		}
		break;

		case gmsec::api::Field::I64_TYPE:
		{
			const gmsec::api::I64Field& childField = (const gmsec::api::I64Field&) field4;
			field3 = new Field(childField.getName(), (GMSEC_I64) childField.getValue());
		}
		break;

		case gmsec::api::Field::U64_TYPE:
		{
			const gmsec::api::U64Field& childField = (const gmsec::api::U64Field&) field4;
			field3 = new Field(childField.getName(), (GMSEC_U64) childField.getValue());
		}
		break;
	}

	if (field3 != NULL)
	{
		field3->SetHeader(field4.isHeader());
	}

	return field3;
}


// Convert Message from 4.x to 3.x equivalent
//
Message* API4_Adapter::API4MessageToAPI3(const gmsec::api::Message& msg4)
{
	return MessageBuddy::createMessage(new gmsec::api::Message(msg4));
}


// Convert Status Class from 4.x to 3.x equivalent
//
GMSEC_STATUS_CLASS API4_Adapter::API4StatusClassToAPI3(gmsec::api::StatusClass class4)
{
	switch (class4)
	{
	case gmsec::api::NO_ERROR_CLASS: return GMSEC_STATUS_NO_ERROR;
	case gmsec::api::FACTORY_ERROR: return GMSEC_STATUS_FACTORY_ERROR;
	case gmsec::api::CONNECTION_ERROR: return GMSEC_STATUS_CONNECTION_ERROR;
	case gmsec::api::CONFIG_ERROR: return GMSEC_STATUS_CONFIG_ERROR;
	case gmsec::api::MIDDLEWARE_ERROR: return GMSEC_STATUS_MIDDLEWARE_ERROR;
	case gmsec::api::MSG_ERROR: return GMSEC_STATUS_MESSAGE_ERROR;
	case gmsec::api::FIELD_ERROR: return GMSEC_STATUS_FIELD_ERROR;
	case gmsec::api::CALLBACK_ERROR: return GMSEC_STATUS_CALLBACK_ERROR;
	case gmsec::api::CALLBACK_LOOKUP_ERROR: return GMSEC_STATUS_CALLBACKLKP_ERROR;
	case gmsec::api::CONFIGFILE_ERROR: return GMSEC_STATUS_CONFIGFILE_ERROR;
	case gmsec::api::ITERATOR_ERROR: return GMSEC_STATUS_ITERATOR_ERROR;
	case gmsec::api::POLICY_ERROR: return GMSEC_STATUS_POLICY_ERROR;
	case gmsec::api::CUSTOM_ERROR: return GMSEC_STATUS_CUSTOM_ERROR;
	case gmsec::api::OTHER_ERROR: return GMSEC_STATUS_OTHER_ERROR;
	}

    return (GMSEC_STATUS_CLASS) class4;
}


// Convert Status Code from 4.x to 3.x equivalent
//
GMSEC_U32 API4_Adapter::API4StatusCodeToAPI3(gmsec::api::StatusCode code4)
{
	switch (code4)
	{
	case gmsec::api::NO_ERROR_CODE: return 0;
	case gmsec::api::LIBRARY_LOAD_FAILURE: return GMSEC_LIBRARY_LOAD_FAILURE;
	case gmsec::api::INVALID_CONNECTION_TYPE: return GMSEC_INVALID_CONNECTION_TYPE;
	case gmsec::api::AUTODISPATCH_FAILURE: return GMSEC_AUTODISPATCH_FAILURE;
	case gmsec::api::INVALID_CONNECTION: return GMSEC_INVALID_CONNECTION;
	case gmsec::api::FEATURE_NOT_SUPPORTED: return GMSEC_FEATURE_NOT_SUPPORTED;
	case gmsec::api::INVALID_CONFIG_VALUE: return GMSEC_INVALID_CONFIG_VALUE;
	case gmsec::api::CONFIG_END_REACHED: return GMSEC_CONFIG_END_REACHED;
	case gmsec::api::INVALID_MSG: return GMSEC_INVALID_MESSAGE;
	case gmsec::api::UNKNOWN_MSG_TYPE: return GMSEC_UNKNOWN_MSG_TYPE;
	case gmsec::api::FIELDS_END_REACHED: return GMSEC_FIELDS_END_REACHED;
	case gmsec::api::FIELD_TYPE_MISMATCH: return GMSEC_FIELD_TYPE_MISMATCH;
	case gmsec::api::UNKNOWN_FIELD_TYPE: return GMSEC_UNKNOWN_FIELD_TYPE;
	case gmsec::api::INVALID_CALLBACK: return GMSEC_INVALID_CALLBACK;
	case gmsec::api::REQUEST_DISPATCH_FAILURE: return GMSEC_REQDISPATCH_FAILURE;
	case gmsec::api::MSG_CONVERT_FAILURE: return GMSEC_MSGCONVERT_FAILURE;
	case gmsec::api::INVALID_FIELD_NAME: return GMSEC_INVALID_FIELD_NAME;
	case gmsec::api::INVALID_FIELD_VALUE: return GMSEC_INVALID_FIELD_VALUE;
	case gmsec::api::INVALID_CONFIG_NAME: return GMSEC_INVALID_CONFIG_NAME;
	case gmsec::api::INVALID_SUBJECT_NAME: return GMSEC_INVALID_SUBJECT_NAME;
	case gmsec::api::NO_MSG_AVAILABLE: return GMSEC_NO_MESSAGE_AVAILABLE;
	case gmsec::api::TIMEOUT_OCCURRED: return GMSEC_TIMEOUT_OCCURRED;
	case gmsec::api::TRACKING_FAILURE: return GMSEC_TRACKING_FAILURE;
	case gmsec::api::UNUSED_CONFIG_ITEM: return GMSEC_UNUSED_CONFIG_ITEM;
	case gmsec::api::INVALID_FIELD: return GMSEC_INVALID_FIELD;
	case gmsec::api::XML_PARSE_ERROR: return GMSEC_XML_PARSE_ERROR;
	case gmsec::api::INVALID_CONFIG: return GMSEC_INVALID_CONFIG;
	case gmsec::api::ENCODING_ERROR: return GMSEC_ENCODING_ERROR;
	case gmsec::api::OUT_OF_MEMORY: return GMSEC_OUT_OF_MEMORY;
	case gmsec::api::ITER_INVALID_NEXT: return GMSEC_INVALID_NEXT;
	case gmsec::api::INITIALIZATION_ERROR: return GMSEC_INITIALIZATION_ERROR;
	case gmsec::api::USER_ACCESS_INVALID: return GMSEC_USER_ACCESS_INVALID;
	case gmsec::api::PUBLISH_NOT_AUTHORIZED: return GMSEC_PUBLISH_NOT_AUTHORIZED;
	case gmsec::api::SUBSCRIBE_NOT_AUTHORIZED: return GMSEC_SUBSCRIBE_NOT_AUTHORIZED;
	case gmsec::api::BAD_MSG_FORMAT: return GMSEC_BAD_MESSAGE_FORMAT;
	case gmsec::api::INVALID_SIGNATURE: return GMSEC_INVALID_SIGNATURE;
	case gmsec::api::UNINITIALIZED_OBJECT: return GMSEC_UNINITIALIZED_OBJECT;
	case gmsec::api::CUSTOM_ERROR_CODE: return GMSEC_CUSTOM_ERROR;
	case gmsec::api::AUTODISPATCH_EXCLUSIVE: return GMSEC_AUTODISPATCH_EXCLUSIVE;
	case gmsec::api::CONNECTION_LOST: return GMSEC_CONNECTION_LOST;
	case gmsec::api::CONNECTION_CONNECTED: return GMSEC_CONNECTION_CONNECTED;
	case gmsec::api::CONNECTION_RECONNECT: return GMSEC_CONNECTION_RECONNECT;
	case gmsec::api::JSON_PARSE_ERROR: return GMSEC_JSON_PARSE_ERROR;
	case gmsec::api::OTHER_ERROR_CODE:return GMSEC_OTHER_ERROR;
	}

	return (GMSEC_U32) code4;
}


// Convert 4.x Exception to 3.x Status
//
Status API4_Adapter::API4ExceptionToAPI3(gmsec::api::Exception& e4)
{
	GMSEC_STATUS_CLASS class3 = API4StatusClassToAPI3(e4.getErrorClass());
	GMSEC_U32          code3  = API4StatusCodeToAPI3(e4.getErrorCode());
	GMSEC_I32          custom = e4.getCustomCode();

	Status status(class3, code3, e4.getErrorMessage());
	status.SetCustomCode(custom);
	return status;
}


// Convert 4.x Status to 3.x Status
//
Status API4_Adapter::API4StatusToAPI3(const gmsec::api::Status& status4)
{
	GMSEC_STATUS_CLASS class3 = API4StatusClassToAPI3(status4.getClass());
	GMSEC_U32          code3  = API4StatusCodeToAPI3(status4.getCode());
	GMSEC_I32          custom = status4.getCustomCode();

	Status status(class3, code3, status4.getReason());
	status.SetCustomCode(custom);
	return status;
}


// Convert 3.x Event string to 4.x Event
//
gmsec::api::Connection::ConnectionEvent API4_Adapter::API3EventToAPI4(const char* event)
{
	gmsec::api::Connection::ConnectionEvent event4;

	if (gmsec::api::util::StringUtil::stringEquals(event, GMSEC_CONNECTION_DISPATCHER_ERROR_EVENT))
	{
		event4 = gmsec::api::Connection::DISPATCHER_ERROR_EVENT;
	}
	else if (gmsec::api::util::StringUtil::stringEquals(event, GMSEC_CONNECTION_REQUEST_TIMEOUT_EVENT))
	{
		event4 = gmsec::api::Connection::REQUEST_TIMEOUT_EVENT;
	}
	else if (gmsec::api::util::StringUtil::stringEquals(event, GMSEC_CONNECTION_SUCCESSFUL_EVENT))
	{
		event4 = gmsec::api::Connection::CONNECTION_SUCCESSFUL_EVENT;
	}
	else if (gmsec::api::util::StringUtil::stringEquals(event, GMSEC_CONNECTION_BROKEN_EVENT))
	{
		event4 = gmsec::api::Connection::CONNECTION_BROKEN_EVENT;
	}
	else if (gmsec::api::util::StringUtil::stringEquals(event, GMSEC_CONNECTION_RECONNECT_EVENT))
	{
		event4 = gmsec::api::Connection::CONNECTION_RECONNECT_EVENT;
	}
	else if (gmsec::api::util::StringUtil::stringEquals(event, GMSEC_CONNECTION_EXCEPTION_EVENT))
	{
		event4 = gmsec::api::Connection::CONNECTION_EXCEPTION_EVENT;
	}
	else if (gmsec::api::util::StringUtil::stringEquals(event, "SS_GMD_ERROR"))
	{
		event4 = gmsec::api::Connection::GMD_ERROR_EVENT;
	}
	else if (gmsec::api::util::StringUtil::stringEquals(event, GMSEC_CONNECTION_WSMQ_ASYNC_STATUS_CHECK_EVENT))
	{
		event4 = gmsec::api::Connection::WSMQ_ASYNC_STATUS_CHECK_EVENT;
	}
	else if (gmsec::api::util::StringUtil::stringEquals(event, GMSEC_ALL_ERRORS_EVENT))
	{
		event4 = gmsec::api::Connection::ALL_EVENTS;
	}
	else
	{
		throw gmsec::api::Exception(gmsec::api::CONNECTION_ERROR, gmsec::api::OTHER_ERROR_CODE, "Unknown event string");
	}

	return event4;
}


// Convert 4.x Event to 3.x Event string
//
const char* API4_Adapter::API4EventToAPI3(gmsec::api::Connection::ConnectionEvent event)
{
	const char* event3 = "<Unknown Event>";

	switch (event)
	{
	case gmsec::api::Connection::DISPATCHER_ERROR_EVENT:
		event3 = GMSEC_CONNECTION_DISPATCHER_ERROR_EVENT;
		break;

	case gmsec::api::Connection::REQUEST_TIMEOUT_EVENT:
		event3 = GMSEC_CONNECTION_REQUEST_TIMEOUT_EVENT;
		break;

	case gmsec::api::Connection::CONNECTION_SUCCESSFUL_EVENT:
		event3 = GMSEC_CONNECTION_SUCCESSFUL_EVENT;
		break;

	case gmsec::api::Connection::CONNECTION_BROKEN_EVENT:
		event3 = GMSEC_CONNECTION_BROKEN_EVENT;
		break;

	case gmsec::api::Connection::CONNECTION_RECONNECT_EVENT:
		event3 = GMSEC_CONNECTION_RECONNECT_EVENT;
		break;

	case gmsec::api::Connection::CONNECTION_EXCEPTION_EVENT:
		event3 = GMSEC_CONNECTION_EXCEPTION_EVENT;
		break;

	case gmsec::api::Connection::GMD_ERROR_EVENT:
		event3 = "SS_GMD_ERROR";
		break;

	case gmsec::api::Connection::WSMQ_ASYNC_STATUS_CHECK_EVENT:
		event3 = "WSMQ_ASYNC_STATUS_CHECK_EVENT";
		break;

	case gmsec::api::Connection::ALL_EVENTS:
		event3 = GMSEC_ALL_ERRORS_EVENT;
		break;

	default:
		event3 = GMSEC_ALL_ERRORS_EVENT;
		break;
	}

	return event3;
}


// API 3.x ErrorCallback proxy
//
API4_Adapter::API4EventCallback::API4EventCallback(Connection* conn3, gmsec::ErrorCallback* ecb3)
	: m_conn3(conn3),
	  m_ecb3(ecb3)
{
}


void API4_Adapter::API4EventCallback::onEvent(gmsec::api::Connection& conn4,
                                              const gmsec::api::Status& status4,
                                              gmsec::api::Connection::ConnectionEvent event4)
{
	Status      status3 = API4_Adapter::API4StatusToAPI3(status4);
	const char* event3  = API4EventToAPI3(event4);

	gmsec::api::Message* reqMsg = gmsec::api::internal::StatusBuddy::getInternal(status4).getRequestMessage();

	if (reqMsg != NULL)
	{
		Message* msg3 = MessageBuddy::createMessage(new gmsec::api::Message(*reqMsg));

		m_ecb3->OnError(m_conn3, msg3, &status3, event3);

		MessageBuddy::destroyMessage(msg3);
	}
	else
	{
		m_ecb3->OnError(m_conn3, NULL, &status3, event3);
	}
}


// API 3.x Callback proxy
//
API4_Adapter::API4Callback::API4Callback(Connection* conn3, gmsec::Callback* cb3)
	: m_conn3(conn3),
	  m_cb3(cb3)
{
}


void API4_Adapter::API4Callback::onMessage(gmsec::api::Connection& conn, const gmsec::api::Message& msg)
{
	Message* msg3 = MessageBuddy::createMessage(new gmsec::api::Message(msg));

	m_cb3->OnMessage(m_conn3, msg3);

	MessageBuddy::destroyMessage(msg3);
}


// API 3.x ReplyCallback proxy
//
API4_Adapter::API4ReplyCallback::API4ReplyCallback(Connection* conn3, gmsec::Callback* cb3, gmsec::ReplyCallback* rcb3)
	: m_conn3(conn3),
	  m_cb3(cb3),
	  m_rcb3(rcb3)
{
}


void API4_Adapter::API4ReplyCallback::onReply(gmsec::api::Connection& conn,
                                              const gmsec::api::Message& request,
                                              const gmsec::api::Message& reply)
{
	Message* req3 = MessageBuddy::createMessage(const_cast<gmsec::api::Message*>(new gmsec::api::Message(request)));
	Message* rep3 = MessageBuddy::createMessage(const_cast<gmsec::api::Message*>(new gmsec::api::Message(reply)));

	if (m_cb3)
	{
		m_cb3->OnMessage(m_conn3, rep3);
	}
	else // if (m_rcb3)
	{
		m_rcb3->OnReply(m_conn3, req3, rep3);
	}

	MessageBuddy::destroyMessage(req3);
	MessageBuddy::destroyMessage(rep3);
}


void API4_Adapter::API4ReplyCallback::onEvent(gmsec::api::Connection& conn4,
                                              const gmsec::api::Status& status4,
                                              gmsec::api::Connection::ConnectionEvent event4)
{
	if (!m_rcb3)
	{
		return;
	}

	Status      status3 = API4_Adapter::API4StatusToAPI3(status4);
	const char* event3  = API4EventToAPI3(event4);

	gmsec::api::Message* reqMsg = gmsec::api::internal::StatusBuddy::getInternal(status4).getRequestMessage();

	if (reqMsg != NULL)
	{
		Message* msg3 = MessageBuddy::createMessage(new gmsec::api::Message(*reqMsg));

		m_rcb3->OnError(m_conn3, msg3, &status3, event3);

		MessageBuddy::destroyMessage(msg3);
	}
	else
	{
		m_rcb3->OnError(m_conn3, NULL, &status3, event3);
	}
}
