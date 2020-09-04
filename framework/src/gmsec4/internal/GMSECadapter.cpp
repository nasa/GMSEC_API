/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file GMSECadapter.cpp
 *
**/

#include <gmsec4/internal/GMSECadapter.h>

#include <gmsec4/Callback.h>
#include <gmsec4/Config.h>
#include <gmsec4/Errors.h>
#include <gmsec4/EventCallback.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>
#include <gmsec4/Message.h>
#include <gmsec4/MessageFieldIterator.h>
#include <gmsec4/ReplyCallback.h>

#include <gmsec4/internal/InternalMessage.h>

#include <gmsec/Config.h>
#include <gmsec/Connection.h>
#include <gmsec/Field.h>
#include <gmsec/Message.h>
#include <gmsec/Status.h>

#include <gmsec/internal/InternalMessage.h>
#include <gmsec/internal/strutil.h>

#include <cctype>    // for isprint()

#include <iostream>


namespace gmsec
{
namespace api
{
namespace internal
{

gmsec::Config* GMSECadapter::createLegacyConfig(const Config& newCfg)
{
	gmsec::Config* legacyCfg = new gmsec::Config();

	const char* name   = 0;
	const char* value  = 0;
	bool        result = newCfg.getFirst(name, value);

	if (result)
	{
		while (result)
		{
			legacyCfg->AddValue(name, value);

			result = newCfg.getNext(name, value);
		}
	}

	return legacyCfg;
}


gmsec::Field* GMSECadapter::createLegacyField(const Field& newField)
{
	gmsec::Field* legacyField = 0;

	switch (newField.getType())
	{
		case Field::BIN_TYPE:
			{
				const BinaryField& field = dynamic_cast<const BinaryField&>(newField);
				legacyField = new gmsec::Field(field.getName(), (GMSEC_BIN) field.getValue(), (GMSEC_U32) field.getLength());
			}
			break;
		case Field::BOOL_TYPE:
			{
				const BooleanField& field = dynamic_cast<const BooleanField&>(newField);
				legacyField = new gmsec::Field(field.getName(), (GMSEC_BOOL) field.getValue());
			}
			break;
		case Field::CHAR_TYPE:
			{
				const CharField& field = dynamic_cast<const CharField&>(newField);
				legacyField = new gmsec::Field(field.getName(), (GMSEC_CHAR) field.getValue());
			}
			break;
		case Field::I8_TYPE:
			{
				const I8Field& field = dynamic_cast<const I8Field&>(newField);
				legacyField = new gmsec::Field(field.getName(), (GMSEC_I8) field.getValue());
			}
			break;
		case Field::I16_TYPE:
			{
				const I16Field& field = dynamic_cast<const I16Field&>(newField);
				legacyField = new gmsec::Field(field.getName(), (GMSEC_I16) field.getValue());
			}
			break;
		case Field::I32_TYPE:
			{
				const I32Field& field = dynamic_cast<const I32Field&>(newField);
				legacyField = new gmsec::Field(field.getName(), (GMSEC_I32) field.getValue());
			}
			break;
		case Field::I64_TYPE:
			{
				const I64Field& field = dynamic_cast<const I64Field&>(newField);
				legacyField = new gmsec::Field(field.getName(), (GMSEC_I64) field.getValue());
			}
			break;
		case Field::F32_TYPE:
			{
				const F32Field& field = dynamic_cast<const F32Field&>(newField);
				legacyField = new gmsec::Field(field.getName(), (GMSEC_F32) field.getValue());
			}
			break;
		case Field::F64_TYPE:
			{
				const F64Field& field = dynamic_cast<const F64Field&>(newField);
				legacyField = new gmsec::Field(field.getName(), (GMSEC_F64) field.getValue());
			}
			break;
		case Field::STRING_TYPE:
			{
				const StringField& field = dynamic_cast<const StringField&>(newField);
				legacyField = new gmsec::Field(field.getName(), field.getValue());
			}
			break;
		case Field::U8_TYPE:
			{
				const U8Field& field = dynamic_cast<const U8Field&>(newField);
				legacyField = new gmsec::Field(field.getName(), (GMSEC_U8) field.getValue());
			}
			break;
		case Field::U16_TYPE:
			{
				const U16Field& field = dynamic_cast<const U16Field&>(newField);
				legacyField = new gmsec::Field(field.getName(), (GMSEC_U16) field.getValue());
			}
			break;
		case Field::U32_TYPE:
			{
				const U32Field& field = dynamic_cast<const U32Field&>(newField);
				legacyField = new gmsec::Field(field.getName(), (GMSEC_U32) field.getValue());
			}
			break;
		case Field::U64_TYPE:
			{
				const U64Field& field = dynamic_cast<const U64Field&>(newField);
				legacyField = new gmsec::Field(field.getName(), (GMSEC_U64) field.getValue());
			}
			break;
	}

	if (legacyField)
	{
		legacyField->SetHeader(newField.isHeader());
	}

	return legacyField;
}


gmsec::Message* GMSECadapter::createLegacyMessage(gmsec::Connection* conn, const Message& newMsg)
{
	gmsec::Message* legacyMsg  = 0;
	gmsec::Config*  legacyCfg  = createLegacyConfig(newMsg.getConfig());
	GMSEC_MSG_KIND  legacyKind = GMSEC_MSG_UNSET;

	switch (newMsg.getKind())
	{
		case Message::PUBLISH:
			legacyKind = GMSEC_MSG_PUBLISH;
			break;
		case Message::REQUEST:
			legacyKind = GMSEC_MSG_REQUEST;
			break;
		case Message::REPLY:
			legacyKind = GMSEC_MSG_REPLY;
			break;
	}

	gmsec::Status status;

	// Create API 3.x message
	if (legacyCfg)
	{
		status = conn->CreateMessage(newMsg.getSubject(), legacyKind, legacyMsg, legacyCfg);
		delete legacyCfg;
	}
	else
	{
		status = conn->CreateMessage(newMsg.getSubject(), legacyKind, legacyMsg);
	}

	if (status.IsError() == 1)
	{
		StatusCode code = translateLegacyStatusCode(status.GetCode());
		throw Exception(CONNECTION_ERROR, code, status.Get());
	}

	// Copy fields from API 4.x message to API 3.x message
	const MessageFieldIterator& iter = newMsg.getFieldIterator();

	while (iter.hasNext())
	{
		const Field& field = iter.next();

		gmsec::Field* legacyField = createLegacyField(field);

		if (legacyField)
		{
			legacyMsg->AddField(*legacyField);
			delete legacyField;
		}
	}

	// Copy meta-data from API 4.x message to API 3.x message
	ValueMap& newDetails    = newMsg.m_iMessage->getDetails();
	gmsec::internal::ValueMap& legacyDetails = gmsec::internal::MessageBuddy::getInternal(legacyMsg)->getDetails();

	legacyDetails.cloneValues(newDetails);

	return legacyMsg;
}


void GMSECadapter::destroyLegacyMessage(gmsec::Connection* conn, gmsec::Message* legacyMsg)
{
	conn->DestroyMessage(legacyMsg);
}


gmsec::api::StatusClass GMSECadapter::translateLegacyStatusClass(GMSEC_U32 legacyClass)
{
	switch (legacyClass)
	{
		case GMSEC_STATUS_NO_ERROR: return NO_ERROR;
		case GMSEC_STATUS_FACTORY_ERROR: return FACTORY_ERROR;
		case GMSEC_STATUS_CONNECTION_ERROR: return CONNECTION_ERROR;
		case GMSEC_STATUS_CONFIG_ERROR: return CONFIG_ERROR;
		case GMSEC_STATUS_MIDDLEWARE_ERROR: return MIDDLEWARE_ERROR;
		case GMSEC_STATUS_MESSAGE_ERROR: return MSG_ERROR;
		case GMSEC_STATUS_FIELD_ERROR: return FIELD_ERROR;
		case GMSEC_STATUS_CALLBACK_ERROR: return CALLBACK_ERROR;
		case GMSEC_STATUS_CALLBACKLKP_ERROR: return CALLBACK_LOOKUP_ERROR;
		case GMSEC_STATUS_CONFIGFILE_ERROR: return CONFIGFILE_ERROR;
		case GMSEC_STATUS_ITERATOR_ERROR: return ITERATOR_ERROR;
		case GMSEC_STATUS_POLICY_ERROR: return POLICY_ERROR;
		case GMSEC_STATUS_CUSTOM_ERROR: return CUSTOM_ERROR;
		case GMSEC_STATUS_OTHER_ERROR: return OTHER_ERROR;
	}

	return OTHER_ERROR;
}


gmsec::api::StatusCode GMSECadapter::translateLegacyStatusCode(GMSEC_U32 legacyCode)
{
	switch (legacyCode)
	{
		case GMSEC_LIBRARY_LOAD_FAILURE: return LIBRARY_LOAD_FAILURE;
		case GMSEC_INVALID_CONNECTION_TYPE: return INVALID_CONNECTION_TYPE;
		case GMSEC_AUTODISPATCH_FAILURE: return AUTODISPATCH_FAILURE;
		case GMSEC_INVALID_CONNECTION: return INVALID_CONNECTION;
		case GMSEC_FEATURE_NOT_SUPPORTED: return FEATURE_NOT_SUPPORTED;
		case GMSEC_INVALID_CONFIG_VALUE: return INVALID_CONFIG_VALUE;
		case GMSEC_CONFIG_END_REACHED : return CONFIG_END_REACHED;
		case GMSEC_INVALID_MESSAGE: return INVALID_MSG;
		case GMSEC_UNKNOWN_MSG_TYPE: return UNKNOWN_MSG_TYPE;
		case GMSEC_FIELDS_END_REACHED: return FIELDS_END_REACHED;
		case GMSEC_FIELD_TYPE_MISMATCH: return FIELD_TYPE_MISMATCH;
		case GMSEC_UNKNOWN_FIELD_TYPE: return UNKNOWN_FIELD_TYPE;
		case GMSEC_INVALID_CALLBACK: return INVALID_CALLBACK;
		case GMSEC_REQDISPATCH_FAILURE: return REQUEST_DISPATCH_FAILURE;
		case GMSEC_MSGCONVERT_FAILURE: return MSG_CONVERT_FAILURE;
		case GMSEC_INVALID_FIELD_NAME: return INVALID_FIELD_NAME;
		case GMSEC_INVALID_FIELD_VALUE: return INVALID_FIELD_VALUE;
		case GMSEC_INVALID_CONFIG_NAME: return INVALID_CONFIG_NAME;
		case GMSEC_INVALID_SUBJECT_NAME: return INVALID_SUBJECT_NAME;
		case GMSEC_NO_MESSAGE_AVAILABLE: return NO_MSG_AVAILABLE;
		case GMSEC_TIMEOUT_OCCURRED: return TIMEOUT_OCCURRED;
		case GMSEC_TRACKING_FAILURE: return TRACKING_FAILURE;
		case GMSEC_UNUSED_CONFIG_ITEM: return UNUSED_CONFIG_ITEM;
		case GMSEC_INVALID_FIELD: return INVALID_FIELD;
		case GMSEC_XML_PARSE_ERROR: return XML_PARSE_ERROR;
		case GMSEC_INVALID_CONFIG: return INVALID_CONFIG;
		case GMSEC_ENCODING_ERROR: return ENCODING_ERROR;
		case GMSEC_OUT_OF_MEMORY: return OUT_OF_MEMORY;
		case GMSEC_INVALID_NEXT: return ITER_INVALID_NEXT;
		case GMSEC_INITIALIZATION_ERROR: return INITIALIZATION_ERROR;
		case GMSEC_USER_ACCESS_INVALID: return USER_ACCESS_INVALID;
		case GMSEC_PUBLISH_NOT_AUTHORIZED: return PUBLISH_NOT_AUTHORIZED;
		case GMSEC_SUBSCRIBE_NOT_AUTHORIZED: return SUBSCRIBE_NOT_AUTHORIZED;
		case GMSEC_BAD_MESSAGE_FORMAT: return BAD_MSG_FORMAT;
		case GMSEC_INVALID_SIGNATURE: return INVALID_SIGNATURE;
		case GMSEC_UNINITIALIZED_OBJECT: return UNINITIALIZED_OBJECT;
		case GMSEC_CUSTOM_ERROR: return CUSTOM_ERROR_CODE;
		case GMSEC_AUTODISPATCH_EXCLUSIVE: return AUTODISPATCH_EXCLUSIVE;
		case GMSEC_CONNECTION_LOST: return CONNECTION_LOST;
		case GMSEC_CONNECTION_CONNECTED : return CONNECTION_CONNECTED;
		case GMSEC_CONNECTION_RECONNECT : return CONNECTION_RECONNECT;
	}

	return OTHER_ERROR_CODE;
}


Status GMSECadapter::createNewStatus(const gmsec::Status& legacyStatus)
{
	StatusClass newClass = translateLegacyStatusClass(legacyStatus.GetClass());
	StatusCode  newCode  = translateLegacyStatusCode(legacyStatus.GetCode());

	return Status(newClass, newCode, legacyStatus.GetString(), legacyStatus.GetCustomCode());
}


Field* GMSECadapter::createNewField(gmsec::Field& legacyField)
{
	Field* newField = 0;

	const char* name = 0;
	legacyField.GetName(name);

	GMSEC_TYPE type;
	legacyField.GetType(type);

	switch (type)
	{
		case GMSEC_TYPE_BIN:
			{
				GMSEC_BIN value;
				GMSEC_U32 size;
				legacyField.GetValue(&value, size);
				newField = new BinaryField(name, value, size);
			}
			break;
		case GMSEC_TYPE_BOOL:
			{
				GMSEC_BOOL value;
				legacyField.Get(value);
				newField = new BooleanField(name, (value == GMSEC_TRUE ? true : false));
			}
			break;
		case GMSEC_TYPE_CHAR:
			{
				GMSEC_CHAR value;
				legacyField.Get(value);
				newField = new CharField(name, value);
			}
			break;
		case GMSEC_TYPE_I8:
			{
				GMSEC_I8 value;
				legacyField.Get(value);
				newField = new I8Field(name, value);
			}
			break;
		case GMSEC_TYPE_I16:
			{
				GMSEC_I16 value;
				legacyField.Get(value);
				newField = new I16Field(name, value);
			}
			break;
		case GMSEC_TYPE_I32:
			{
				GMSEC_I32 value;
				legacyField.Get(value);
				newField = new I32Field(name, value);
			}
			break;
		case GMSEC_TYPE_I64:
			{
				GMSEC_I64 value;
				legacyField.Get(value);
				newField = new I64Field(name, value);
			}
			break;
		case GMSEC_TYPE_STRING:
			{
				GMSEC_STRING value;
				legacyField.Get(value);
				newField = new StringField(name, value);
			}
			break;
		case GMSEC_TYPE_F32:
			{
				GMSEC_F32 value;
				legacyField.Get(value);
				newField = new F32Field(name, value);
			}
			break;
		case GMSEC_TYPE_F64:
			{
				GMSEC_F64 value;
				legacyField.Get(value);
				newField = new F64Field(name, value);
			}
			break;
		case GMSEC_TYPE_U8:
			{
				GMSEC_U8 value;
				legacyField.Get(value);
				newField = new U8Field(name, value);
			}
			break;
		case GMSEC_TYPE_U16:
			{
				GMSEC_U16 value;
				legacyField.Get(value);
				newField = new U16Field(name, value);
			}
			break;
		case GMSEC_TYPE_U32:
			{
				GMSEC_U32 value;
				legacyField.Get(value);
				newField = new U32Field(name, value);
			}
			break;
		case GMSEC_TYPE_U64:
			{
				GMSEC_U64 value;
				legacyField.Get(value);
				newField = new U64Field(name, value);
			}
			break;
	}

	if (newField)
	{
		newField->isHeader(legacyField.IsHeader());
	}

	return newField;
}


Message* GMSECadapter::createNewMessage(gmsec::Message* legacyMsg)
{
	Message* newMsg = 0;

	if (legacyMsg)
	{
		const char*          msgSubject = 0;
		Message::MessageKind msgKind;
		GMSEC_MSG_KIND       legacyKind;

		legacyMsg->GetSubject(msgSubject);
		legacyMsg->GetKind(legacyKind);
		switch (legacyKind)
		{
			case GMSEC_MSG_PUBLISH:
				msgKind = Message::PUBLISH;
				break;
			case GMSEC_MSG_REQUEST:
				msgKind = Message::REQUEST;
				break;
			case GMSEC_MSG_REPLY:
				msgKind = Message::REPLY;
				break;
			default:
				throw Exception(MSG_ERROR, UNKNOWN_MSG_TYPE, "Cannot translate 3.x Message to 4.x Message");
		}

		// create API 4.x message
		newMsg = new Message(msgSubject, msgKind);

		// copy fields from API 3.x message to API 4.x message
		gmsec::Field  field;
		gmsec::Status haveNext = legacyMsg->GetFirstField(field);

		while (haveNext.IsError() == 0)
		{
			Field* newField = createNewField(field);

			if (newField)
			{
				newMsg->addField(*newField);
				delete newField;
			}

			haveNext = legacyMsg->GetNextField(field);
		}

		// copy meta-data from API 3.x message to API 4.x message
		gmsec::internal::ValueMap& legacyDetails = gmsec::internal::MessageBuddy::getInternal(legacyMsg)->getDetails();
		gmsec::internal::ValueMap& newDetails    = newMsg->m_iMessage->getDetails();

		newDetails.cloneValues(legacyDetails);
	}

	return newMsg;
}


const char* GMSECadapter::toLegacyEvent(Connection::ConnectionEvent event)
{
	const char* eventString = NULL;

	switch (event)
	{
		case Connection::DISPATCHER_ERROR_EVENT:
			eventString = GMSEC_CONNECTION_DISPATCHER_ERROR_EVENT;
			break;

		case Connection::REQUEST_TIMEOUT_EVENT:
			eventString = GMSEC_CONNECTION_REQUEST_TIMEOUT_EVENT;
			break;

		case Connection::CONNECTION_SUCCESSFUL_EVENT:
			eventString = GMSEC_CONNECTION_SUCCESSFUL_EVENT;
			break;

		case Connection::CONNECTION_BROKEN_EVENT:
			eventString = GMSEC_CONNECTION_BROKEN_EVENT;
			break;

		case Connection::CONNECTION_RECONNECT_EVENT:
			eventString = GMSEC_CONNECTION_RECONNECT_EVENT;
			break;

		case Connection::CONNECTION_EXCEPTION_EVENT:
			eventString = GMSEC_CONNECTION_EXCEPTION_EVENT;
			break;

		case Connection::GMD_ERROR_EVENT:
			eventString = "SS_GMD_ERROR";
			break;

		case Connection::WSMQ_ASYNC_STATUS_CHECK_EVENT:
			eventString = "WSMQ_ASYNC_STATUS_CHECK_EVENT";
			break;

		case Connection::ALL_EVENTS:
			eventString = GMSEC_ALL_ERRORS_EVENT;
		break;
	}

	return eventString;
}


Connection::ConnectionEvent GMSECadapter::toNewEvent(const char* event)
{
	if (gmsec::util::stringEquals(event, GMSEC_CONNECTION_DISPATCHER_ERROR_EVENT))
		return Connection::DISPATCHER_ERROR_EVENT;

	if (gmsec::util::stringEquals(event, GMSEC_CONNECTION_REQUEST_TIMEOUT_EVENT) ||
	    gmsec::util::stringEquals(event, "Request timed out."))
		return Connection::REQUEST_TIMEOUT_EVENT;

	if (gmsec::util::stringEquals(event, GMSEC_CONNECTION_SUCCESSFUL_EVENT))
		return Connection::CONNECTION_SUCCESSFUL_EVENT;

	if (gmsec::util::stringEquals(event, GMSEC_CONNECTION_BROKEN_EVENT))
		return Connection::CONNECTION_BROKEN_EVENT;

	if (gmsec::util::stringEquals(event, GMSEC_CONNECTION_RECONNECT_EVENT))
		return Connection::CONNECTION_RECONNECT_EVENT;

	if (gmsec::util::stringEquals(event, GMSEC_CONNECTION_EXCEPTION_EVENT))
		return Connection::CONNECTION_EXCEPTION_EVENT;

	if (gmsec::util::stringEquals(event, "SS_GMD_ERROR"))
		return Connection::GMD_ERROR_EVENT;

	// should never reach here, but just in case...
	return Connection::ALL_EVENTS;
}


// LegacyCallback
//
LegacyCallback::LegacyCallback(gmsec::api::Connection* conn, gmsec::api::Callback* cb)
	: m_conn(conn),
	  m_cb(cb)
{
}


void LegacyCallback::OnMessage(gmsec::Connection* conn, gmsec::Message* msg)
{
	const gmsec::api::Message* newMsg = GMSECadapter::createNewMessage(msg);

	m_cb->onMessage(*m_conn, *newMsg);

	delete newMsg;
}


// LegacyErrorCallback
//
LegacyErrorCallback::LegacyErrorCallback(gmsec::api::Connection* conn, gmsec::api::EventCallback* cb)
	: m_conn(conn),
	  m_cb(cb)
{
}


void LegacyErrorCallback::OnError(gmsec::Connection* conn, gmsec::Message* msg, gmsec::Status* status, const char* event)
{
	const gmsec::api::Status newStatus = GMSECadapter::createNewStatus(*status);

	m_cb->onEvent(*m_conn, newStatus, GMSECadapter::toNewEvent(event));
}


// LegacyReplyCallback
//
LegacyReplyCallback::LegacyReplyCallback(gmsec::api::Connection* conn, gmsec::api::ReplyCallback* cb)
	: m_conn(conn),
	  m_cb(cb)
{
}


void LegacyReplyCallback::OnReply(gmsec::Connection* conn, gmsec::Message* request, gmsec::Message* reply)
{
	const gmsec::api::Message* newReq = GMSECadapter::createNewMessage(request);
	const gmsec::api::Message* newRep = GMSECadapter::createNewMessage(reply);

	m_cb->onReply(*m_conn, *newReq, *newRep);

	// Check to ensure that it is safe to dispose of the legacy reply-callback.
	//
	const I16Field* responseStatus = dynamic_cast<const I16Field*>(newRep->getField("RESPONSE-STATUS"));
	if (!responseStatus || (responseStatus && responseStatus->getValue() == 6))
	{
		LegacyReplyCallback* lcb = CallbackTable::self().removeReplyCallback(m_conn, m_cb);
		delete lcb;
	}

	delete newReq;
	delete newRep;
}


void LegacyReplyCallback::OnError(gmsec::Connection* conn, gmsec::Message* request, gmsec::Status* status, const char* event)
{
	const gmsec::api::Message* newReq    = GMSECadapter::createNewMessage(request);
	gmsec::api::Status         newStatus = GMSECadapter::createNewStatus(*status);

	m_cb->onEvent(*m_conn, newStatus, GMSECadapter::toNewEvent(event));

	delete newReq;

	LegacyReplyCallback* lcb = CallbackTable::self().removeReplyCallback(m_conn, m_cb);
	delete lcb;
}


// CallbackLookup
//
CallbackTable& CallbackTable::self()
{
	static CallbackTable table;
	return table;
}


CallbackTable::CallbackTable()
	: m_callbacks(),
	  m_eventCallbacks(),
	  m_replyCallbacks()
{
}


void CallbackTable::addCallback(Connection* owner, const char* subject, Callback* cb, LegacyCallback* lcb)
{
	gmsec::util::AutoMutex lock(m_mutex);

	m_callbacks[lcb] = new CallbackInfo(owner, subject, cb);
}


bool CallbackTable::callbackRegistered(Connection* owner, const char* subject, Callback* cb)
{
	gmsec::util::AutoMutex lock(m_mutex);

	bool found = false;

	for (Callbacks::const_iterator it = m_callbacks.begin(); it != m_callbacks.end() && !found; ++it)
	{
		if (it->second->owner == owner &&
		    it->second->subject == subject &&
		    it->second->cb == cb)
		{
			found = true;
		}
	}

	return found;
}


void CallbackTable::addEventCallback(Connection* owner, EventCallback* cb, LegacyErrorCallback* lcb)
{
	gmsec::util::AutoMutex lock(m_mutex);

	m_eventCallbacks[lcb] = new EventCallbackInfo(owner, cb);
}


void CallbackTable::addReplyCallback(Connection* owner, ReplyCallback* cb, LegacyReplyCallback* lcb)
{
	gmsec::util::AutoMutex lock(m_mutex);

	m_replyCallbacks[lcb] = new ReplyCallbackInfo(owner, cb);
}


LegacyCallback* CallbackTable::removeCallback(Connection* owner, const char* subject, Callback* cb)
{
	gmsec::util::AutoMutex lock(m_mutex);

	LegacyCallback* lcb = 0;

	Callbacks::iterator it = m_callbacks.begin();

	for (; it != m_callbacks.end(); ++it)
	{
		if (it->second->owner == owner && it->second->subject == subject && it->second->cb == cb)
		{
			break;
		}
	}

	if (it != m_callbacks.end())
	{
		lcb = it->first;
		delete it->second;
		m_callbacks.erase(it);
	}

	return lcb;
}


LegacyErrorCallback* CallbackTable::removeEventCallback(Connection* owner, EventCallback* cb)
{
	gmsec::util::AutoMutex lock(m_mutex);

	LegacyErrorCallback* lcb = 0;

	EventCallbacks::iterator it = m_eventCallbacks.begin();

	for (; it != m_eventCallbacks.end(); ++it)
	{
		if (it->second->owner == owner && it->second->cb == cb)
		{
			break;
		}
	}

	if (it != m_eventCallbacks.end())
	{
		lcb = it->first;
		delete it->second;
		m_eventCallbacks.erase(it);
	}

	return lcb;
}


LegacyReplyCallback* CallbackTable::removeReplyCallback(Connection* owner, ReplyCallback* cb)
{
	gmsec::util::AutoMutex lock(m_mutex);

	LegacyReplyCallback* lcb = 0;

	ReplyCallbacks::iterator it = m_replyCallbacks.begin();

	for (; it != m_replyCallbacks.end(); ++it)
	{
		if (it->second->owner == owner && it->second->cb == cb)
		{
			break;
		}
	}

	if (it != m_replyCallbacks.end())
	{
		lcb = it->first;
		delete it->second;
		m_replyCallbacks.erase(it);
	}

	return lcb;
}


void CallbackTable::removeAllCallbacks(Connection* owner)
{
	gmsec::util::AutoMutex lock(m_mutex);

	// Clear out Callbacks associated with 'owner'
	{
		Callbacks::iterator it = m_callbacks.begin();
		while (it != m_callbacks.end())
		{
			if (it->second->owner == owner)
			{
				delete it->first;
				delete it->second;
				m_callbacks.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}

	// Clear out EventCallbacks associated with 'owner'
	{
		EventCallbacks::iterator it = m_eventCallbacks.begin();
		while (it != m_eventCallbacks.end())
		{
			if (it->second->owner == owner)
			{
				delete it->first;
				delete it->second;
				m_eventCallbacks.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}

	// Clear out ReplyCallbacks associated with 'owner'
	{
		ReplyCallbacks::iterator it = m_replyCallbacks.begin();
		while (it != m_replyCallbacks.end())
		{
			if (it->second->owner == owner)
			{
				delete it->first;
				delete it->second;
				m_replyCallbacks.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}
}

} // namespace internal
} // namespace api
} // namespace gmsec
