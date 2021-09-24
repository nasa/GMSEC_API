
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file message.cpp
 *
 *  This file contains the C-wrapper for functions in the Message object.
*/

#include <gmsec/c/message.h>

#include <gmsec/Message.h>

#include <gmsec/internal/Log.h>


using namespace gmsec::util;
using namespace gmsec;

GMSEC_BOOL CALL_TYPE gmsec_isMsgValid(GMSEC_MESSAGE_OBJECT msg)
{
	if (NULL == msg)
	{
		return GMSEC_FALSE;
	}

	Message *tmpMsg = (Message *)msg;
	return tmpMsg->isValid() ? GMSEC_TRUE : GMSEC_FALSE;
}


void CALL_TYPE gmsec_SetMsgKind(GMSEC_MESSAGE_OBJECT msg, GMSEC_MSG_KIND kind, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::SetMsgKind : " << result.Get();
	}
	else
	{
		Message *tmpMsg = (Message *)msg;
		result = tmpMsg->SetKind(kind);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetMsgKind(GMSEC_MESSAGE_OBJECT msg, GMSEC_MSG_KIND *kind, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::GetMsgKind : " << result.Get();
		*kind = 0;
	}
	else
	{
		Message *tmpMsg = (Message *)msg;
		result = tmpMsg->GetKind(*kind);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetMsgSubject(GMSEC_MESSAGE_OBJECT msg, const char* subject, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::SetMsgSubject : " << result.Get();
	}
	else
	{
		Message *tmpMsg = (Message *)msg;
		result = tmpMsg->SetSubject(subject);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetMsgSubject(GMSEC_MESSAGE_OBJECT msg, const char* *subject, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::GetMsgSubject : " << result.Get();

		*subject = NULL;
	}
	else
	{
		Message *tmpMsg = (Message *)msg;
		result = tmpMsg->GetSubject((const char *&)*subject);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_MsgSetConfig(GMSEC_MESSAGE_OBJECT msg, GMSEC_CONFIG_OBJECT config, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::MsgSetConfig : " << result.Get();
	}
	else if (NULL == config)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_CONFIG_VALUE,
		           "Config handle is NULL");
                LOG_WARNING << "message_c::MsgSetConfig : " << result.Get();
	}
	else
	{
		Config *tmpconfig = (Config *)config;
		Message *tmpMsg = (Message *)msg;
		result = tmpMsg->SetConfig(tmpconfig);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_MsgClearFields(GMSEC_MESSAGE_OBJECT msg,GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::MsgClearFields : " << result.Get();
	}
	else
	{
		Message *tmpMsg = (Message *)msg;
		result = tmpMsg->ClearFields();
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_MsgAddField(GMSEC_MESSAGE_OBJECT msg, GMSEC_FIELD_OBJECT field, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::MsgAddField : " << result.Get();
	}
	else if (NULL == field)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_FIELD,
		           "Field handle is NULL");
                LOG_WARNING << "message_c::MsgAddField : " << result.Get();
	}
	else
	{
		Message *tmpMsg = (Message *)msg;
		Field *tmpFld = (Field *)field;
		result = tmpMsg->AddField(*tmpFld);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_MsgClearField(GMSEC_MESSAGE_OBJECT msg, const char *name, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::MsgClearField : " << result.Get();
	}
	else
	{
		Message *tmpMsg = (Message *)msg;
		result = tmpMsg->ClearField(name);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_MsgGetField(GMSEC_MESSAGE_OBJECT msg, const char* name, GMSEC_FIELD_OBJECT field, GMSEC_STATUS_OBJECT status)
{
	Status result;
	Field tmpFld;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::MsgGetField : " << result.Get();
	}
	else if (NULL == field)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
			GMSEC_INVALID_FIELD,
			"Field handle is NULL");
		LOG_WARNING << "message_c::MsgGetField : " << result.Get();
	}
	else
	{
		Message *tmpMsg = (Message *)msg;
		result = tmpMsg->GetField(name,tmpFld);
		*((Field *)field) = tmpFld;	//assignment operator
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_MsgGetFieldCount(GMSEC_MESSAGE_OBJECT msg, GMSEC_I32 *count, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::MsgGetFieldCount : " << result.Get();
		*count = -1;
	}
	else
	{
		Message *tmpMsg = (Message *)msg;
		result = tmpMsg->GetFieldCount(*count);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_MsgGetFirstField(GMSEC_MESSAGE_OBJECT msg, GMSEC_FIELD_OBJECT field, GMSEC_STATUS_OBJECT status)
{
	Status result;
	Field tmpFld;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::MsgGetFirstField : " << result.Get(); 
	}
	else if (NULL == field)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
				GMSEC_INVALID_FIELD,
				"Field handle is NULL");
		LOG_WARNING << "message_c::MsgGetFirstField : " << result.Get();
	}
	else
	{
		Message *tmpMsg = (Message *)msg;
		result = tmpMsg->GetFirstField(tmpFld);
		*((Field *)field) = tmpFld;	//assignment operator
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_MsgGetNextField(GMSEC_MESSAGE_OBJECT msg, GMSEC_FIELD_OBJECT field, GMSEC_STATUS_OBJECT status)
{
	Status result;
	Field tmpFld;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::MsgGetNextField : " << result.Get();
	}
	else if (NULL == field)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
				GMSEC_INVALID_FIELD,
				"Field handle is NULL");
		LOG_WARNING << "message_c::MsgGetNextField : " << result.Get();
	}
	else
	{
		Message *tmpMsg = (Message *)msg;
		result = tmpMsg->GetNextField(tmpFld);
		*((Field *)field) = tmpFld;	//assignment operator
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_MsgToXML(GMSEC_MESSAGE_OBJECT msg, const char **outst, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::MsgToXML : " << result.Get();
		*outst = NULL;
	}
	else
	{
		Message * message = (Message *)msg;
		result = message->ToXML((const char *&)*outst);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

GMSEC_API void CALL_TYPE gmsec_MsgFromXML(GMSEC_MESSAGE_OBJECT msg, const char *xml, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::MsgFromXML : " << result.Get();

	}
	else
	{
		Message * message = (Message *)msg;
		result = message->FromXML((const char *)xml);
	}
	if (NULL != status)
		*((Status *)status) = result;
}


void CALL_TYPE gmsec_MsgGetSize(GMSEC_MESSAGE_OBJECT msg, GMSEC_U32 *size, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == msg)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message handle is NULL");
                LOG_WARNING << "message_c::MsgGetSize : " << result.Get();
		*size = 0;
	}
	else
	{
		Message * message = (Message *)msg;
		result = message->GetMSGSize(*size);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

const char * CALL_TYPE gmsec_MsgGetTime()
{
	return Message::GetTime();
}

