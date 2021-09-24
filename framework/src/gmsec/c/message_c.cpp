
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file message_c.cpp
 *
 *  This file contains the C-wrapper for functions in the Message object.
*/
#include <gmsec/c/message_c.h>

#include <gmsec/Message.h>

#include <gmsec/internal/Log.h>


using namespace gmsec::util;
using namespace gmsec;

GMSEC_BOOL CALL_TYPE isMsgValid(GMSEC_MESSAGE_HANDLE msg)
{
	if (NULL == msg)
	{
		return GMSEC_FALSE;
	}

	Message *tmpMsg = (Message *)msg;
	return tmpMsg->isValid() ? GMSEC_TRUE : GMSEC_FALSE;
}


void CALL_TYPE SetMsgKind(GMSEC_MESSAGE_HANDLE msg, GMSEC_MSG_KIND kind, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE GetMsgKind(GMSEC_MESSAGE_HANDLE msg, GMSEC_MSG_KIND *kind, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE SetMsgSubject(GMSEC_MESSAGE_HANDLE msg, char* subject, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE GetMsgSubject(GMSEC_MESSAGE_HANDLE msg, char* *subject, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE MsgSetConfig(GMSEC_MESSAGE_HANDLE msg, GMSEC_CONFIG_HANDLE config, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE MsgClearFields(GMSEC_MESSAGE_HANDLE msg,GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE MsgAddField(GMSEC_MESSAGE_HANDLE msg, GMSEC_FIELD_HANDLE field, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE MsgClearField(GMSEC_MESSAGE_HANDLE msg, char *name, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE MsgGetField(GMSEC_MESSAGE_HANDLE msg, char* name, GMSEC_FIELD_HANDLE field, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE MsgGetFieldCount(GMSEC_MESSAGE_HANDLE msg, GMSEC_I32 *count, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE MsgGetFirstField(GMSEC_MESSAGE_HANDLE msg, GMSEC_FIELD_HANDLE field, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE MsgGetNextField(GMSEC_MESSAGE_HANDLE msg, GMSEC_FIELD_HANDLE field, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE MsgToXML(GMSEC_MESSAGE_HANDLE msg, char **outst, GMSEC_STATUS_HANDLE status)
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
		result = static_cast<Message *>(msg)->ToXML((const char *&)*outst);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

GMSEC_API void CALL_TYPE MsgFromXML(GMSEC_MESSAGE_HANDLE msg, char *xml, GMSEC_STATUS_HANDLE status)
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
		result = static_cast<Message *>(msg)->FromXML((const char *)xml);
	}
	if (NULL != status)
		*((Status *)status) = result;
}


void CALL_TYPE MsgGetSize(GMSEC_MESSAGE_HANDLE msg, GMSEC_U32 *size, GMSEC_STATUS_HANDLE status)
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
		result = static_cast<Message *>(msg)->GetMSGSize(*size);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

const char * CALL_TYPE MsgGetTime()
{
	return Message::GetTime();
}

