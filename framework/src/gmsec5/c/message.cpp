/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file message.cpp
 *
 *  @brief This file contains function for managing messages.
 */

#include <gmsec5/c/message.h>

#include <gmsec5/Fields.h>

#include <gmsec5/Config.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/Message.h>
#include <gmsec5/MessageFieldIterator.h>
#include <gmsec5/Status.h>

using namespace gmsec::api5;


GMSEC_Message CALL_TYPE messageCreate()
{
	return reinterpret_cast<GMSEC_Message>(new Message());
}


GMSEC_Message CALL_TYPE messageCreateCopy(GMSEC_Message otherMsg, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;;
	Status        result;

	Message* other = reinterpret_cast<Message*>(otherMsg);

	if (other == NULL)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		msg = reinterpret_cast<GMSEC_Message>(new Message(*other));
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


void CALL_TYPE messageAcknowledge(GMSEC_Message msg, GMSEC_Status status)
{
	Status result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(CONNECTION_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		m->acknowledge();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE messageDestroy(GMSEC_Message* msg)
{
	if (msg)
	{
		Message* m = reinterpret_cast<Message*>(*msg);

		delete m;

		*msg = NULL;
	}
}


const char* CALL_TYPE messageGetSchemaID(GMSEC_Message msg, GMSEC_Status status)
{
	const char* id = NULL;
	Status      result;

	Message* m  = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		id = m->getSchemaID();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return id;
}


unsigned int CALL_TYPE messageGetVersion(GMSEC_Message msg, GMSEC_Status status)
{
	unsigned int version = 0;
	Status       result;

	Message* m  = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		version = m->getVersion();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return version;
}


GMSEC_SchemaLevel CALL_TYPE messageGetSchemaLevel(GMSEC_Message msg, GMSEC_Status status)
{
	GMSEC_SchemaLevel level = GMSEC_SchemaLevel::GMSEC_SCHEMA_LEVEL_0;
	Status            result;

	Message* m  = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		level = static_cast<GMSEC_SchemaLevel>(m->getSchemaLevel());
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return level;
}


void CALL_TYPE messageIsCompliant(GMSEC_Message msg, GMSEC_Status status)
{
	Status result;

	Message* m  = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		result = m->isCompliant();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE messageRegisterMessageValidator(GMSEC_Message msg, GMSEC_MessageValidator* validator, GMSEC_Status status)
{
	Status result;

	Message* m  = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			m->registerMessageValidator(validator);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE messageSetFieldValue(GMSEC_Message msg, const char* fieldName, const char* value, GMSEC_Status status)
{
	Message*   m = reinterpret_cast<Message*>(msg);
	Status     result;
	GMSEC_BOOL replaced = GMSEC_BOOL::GMSEC_FALSE;

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			replaced = (m->setFieldValue(fieldName, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return replaced;
}


GMSEC_BOOL CALL_TYPE messageSetFieldValueI16(GMSEC_Message msg, const char* fieldName, GMSEC_I16 value, GMSEC_Status status)
{
	Message*   m = reinterpret_cast<Message*>(msg);
	Status     result;
	GMSEC_BOOL replaced = GMSEC_BOOL::GMSEC_FALSE;

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			replaced = (m->setFieldValue(fieldName, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return replaced;
}


GMSEC_BOOL CALL_TYPE messageSetFieldValueU16(GMSEC_Message msg, const char* fieldName, GMSEC_U16 value, GMSEC_Status status)
{
	Message*   m = reinterpret_cast<Message*>(msg);
	Status     result;
	GMSEC_BOOL replaced = GMSEC_BOOL::GMSEC_FALSE;

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			replaced = (m->setFieldValue(fieldName, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return replaced;
}


GMSEC_BOOL CALL_TYPE messageSetFieldValueI32(GMSEC_Message msg, const char* fieldName, GMSEC_I32 value, GMSEC_Status status)
{
	Message*   m = reinterpret_cast<Message*>(msg);
	Status     result;
	GMSEC_BOOL replaced = GMSEC_BOOL::GMSEC_FALSE;

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			replaced = (m->setFieldValue(fieldName, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return replaced;
}


GMSEC_BOOL CALL_TYPE messageSetFieldValueU32(GMSEC_Message msg, const char* fieldName, GMSEC_U32 value, GMSEC_Status status)
{
	Message*   m = reinterpret_cast<Message*>(msg);
	Status     result;
	GMSEC_BOOL replaced = GMSEC_BOOL::GMSEC_FALSE;

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			replaced = (m->setFieldValue(fieldName, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return replaced;
}


GMSEC_BOOL CALL_TYPE messageSetFieldValueI64(GMSEC_Message msg, const char* fieldName, GMSEC_I64 value, GMSEC_Status status)
{
	Message*   m = reinterpret_cast<Message*>(msg);
	Status     result;
	GMSEC_BOOL replaced = GMSEC_BOOL::GMSEC_FALSE;

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			replaced = (m->setFieldValue(fieldName, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return replaced;
}


GMSEC_BOOL CALL_TYPE messageSetFieldValueU64(GMSEC_Message msg, const char* fieldName, GMSEC_U64 value, GMSEC_Status status)
{
	Message*   m = reinterpret_cast<Message*>(msg);
	Status     result;
	GMSEC_BOOL replaced = GMSEC_BOOL::GMSEC_FALSE;

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			replaced = (m->setFieldValue(fieldName, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return replaced;
}


GMSEC_BOOL CALL_TYPE messageSetFieldValueF64(GMSEC_Message msg, const char* fieldName, GMSEC_F64 value, GMSEC_Status status)
{
	Message*   m = reinterpret_cast<Message*>(msg);
	Status     result;
	GMSEC_BOOL replaced = GMSEC_BOOL::GMSEC_FALSE;

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			replaced = (m->setFieldValue(fieldName, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return replaced;
}


void CALL_TYPE messageSetConfig(GMSEC_Message msg, const GMSEC_Config cfg, GMSEC_Status status)
{
	Message* m = reinterpret_cast<Message*>(msg);
	Config*  c = reinterpret_cast<Config*>(cfg);
	Status   result;

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else if (!c)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else
	{
		m->setConfig(*c);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_Config CALL_TYPE messageGetConfig(GMSEC_Message msg, GMSEC_Status status)
{
	GMSEC_Config cfg = NULL;
	Status       result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		Config& config = m->getConfig();

		cfg = reinterpret_cast<GMSEC_Config>(&config);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return cfg;
}


void CALL_TYPE messageSetSubject(GMSEC_Message msg, const char* subject, GMSEC_Status status)
{
	Status result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			m->setSubject(subject);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


const char* CALL_TYPE messageGetSubject(GMSEC_Message msg, GMSEC_Status status)
{
	const char* subject = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		subject = m->getSubject();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return subject;
}


void CALL_TYPE messageSetKind(GMSEC_Message msg, GMSEC_MessageKind kind, GMSEC_Status status)
{
	Status result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		m->setKind( static_cast<Message::Kind>(kind) );
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_MessageKind CALL_TYPE messageGetKind(GMSEC_Message msg, GMSEC_Status status)
{
	GMSEC_MessageKind kind = GMSEC_PUBLISH;
	Status            result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		int tmp = static_cast<int>(m->getKind());
		kind = static_cast<GMSEC_MessageKind>(tmp);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return kind;
}


GMSEC_BOOL CALL_TYPE messageAddField(GMSEC_Message msg, GMSEC_Field field, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);
	Field*   f = reinterpret_cast<Field*>(field);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else if (!f)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Field handle is NULL");
	}
	else
	{
		overwrite = (m->addField(*f) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddBinaryField(GMSEC_Message msg, const char* name, const GMSEC_U8* data, size_t dataLen, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			overwrite = (m->addField(name, data, dataLen) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddBooleanField(GMSEC_Message msg, const char* name, GMSEC_BOOL value, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			overwrite = (m->addField(name, value == GMSEC_BOOL::GMSEC_TRUE) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddCharField(GMSEC_Message msg, const char* name, GMSEC_CHAR value, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			overwrite = (m->addField(name, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddF32Field(GMSEC_Message msg, const char* name, GMSEC_F32 value, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			overwrite = (m->addField(name, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddF64Field(GMSEC_Message msg, const char* name, GMSEC_F64 value, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			overwrite = (m->addField(name, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddI8Field(GMSEC_Message msg, const char* name, GMSEC_I8 value, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			overwrite = (m->addField(name, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddI16Field(GMSEC_Message msg, const char* name, GMSEC_I16 value, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			overwrite = (m->addField(name, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddI32Field(GMSEC_Message msg, const char* name, GMSEC_I32 value, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			overwrite = (m->addField(name, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddI64Field(GMSEC_Message msg, const char* name, GMSEC_I64 value, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			overwrite = (m->addField(name, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL messageAddStringField(GMSEC_Message msg, const char* name, const char* value, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			overwrite = (m->addField(name, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddU8Field(GMSEC_Message msg, const char* name, GMSEC_U8 value, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			overwrite = (m->addField(name, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddU16Field(GMSEC_Message msg, const char* name, GMSEC_U16 value, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			overwrite = (m->addField(name, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddU32Field(GMSEC_Message msg, const char* name, GMSEC_U32 value, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			overwrite = (m->addField(name, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddU64Field(GMSEC_Message msg, const char* name, GMSEC_U64 value, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			overwrite = (m->addField(name, value) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddFields(GMSEC_Message msg, GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
{
	bool   fieldsReplaced = false;
	Status result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else if (!fields || numFields == 0)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Array of Fields is NULL or field count is zero");
	}
	else
	{
		for (size_t i = 0; i < numFields; ++i)
		{
			if (fields[i] != NULL)
			{
				fieldsReplaced |= (messageAddField(msg, fields[i], NULL) == GMSEC_BOOL::GMSEC_TRUE);
			}
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return (fieldsReplaced ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
}


void CALL_TYPE messageClearFields(GMSEC_Message msg, GMSEC_Status status)
{
	Status result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		m->clearFields();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE messageClearField(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_BOOL removed = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			removed = (m->clearField(name) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return removed;
}


GMSEC_BOOL CALL_TYPE messageHasField(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_BOOL  found = GMSEC_BOOL::GMSEC_FALSE;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		found = (m->hasField(name) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return found;
}


const GMSEC_Field CALL_TYPE messageGetField(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			Field* tmp = const_cast<Field*>(m->getField(name));

			field = reinterpret_cast<GMSEC_Field>(tmp);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


GMSEC_FieldType CALL_TYPE messageGetFieldType(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_FieldType fieldType = GMSEC_BAD_TYPE;
	Status          result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			int tmp = static_cast<int>(m->getFieldType(name));

			fieldType = static_cast<GMSEC_FieldType>(tmp);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return fieldType;
}


const char* CALL_TYPE messageGetStringValue(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
{
	const char* value = 0;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			value = m->getStringValue(fieldName);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


GMSEC_BOOL messageGetBooleanValue(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
{
	GMSEC_BOOL value = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			value = (m->getBooleanValue(fieldName) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


GMSEC_I16 CALL_TYPE messageGetI16Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
{
	GMSEC_I16 value = 0;
	Status    result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			value = m->getI16Value(fieldName);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


GMSEC_I32 CALL_TYPE messageGetI32Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
{
	GMSEC_I32 value = 0;
	Status    result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			value = m->getI32Value(fieldName);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


GMSEC_I64 CALL_TYPE messageGetI64Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
{
	GMSEC_I64 value = 0;
	Status    result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			value = m->getI64Value(fieldName);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


GMSEC_U16 CALL_TYPE messageGetU16Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
{
	GMSEC_U16 value = 0;
	Status    result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			value = m->getU16Value(fieldName);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


GMSEC_U32 CALL_TYPE messageGetU32Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
{
	GMSEC_U32 value = 0;
	Status    result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			value = m->getU32Value(fieldName);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


GMSEC_U64 CALL_TYPE messageGetU64Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
{
	GMSEC_U64 value = 0;
	Status    result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			value = m->getU64Value(fieldName);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


GMSEC_F64 CALL_TYPE messageGetF64Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
{
	GMSEC_F64 value = 0;
	Status    result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			value = m->getF64Value(fieldName);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


const GMSEC_Field CALL_TYPE messageGetBinaryField(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			BinaryField& f = const_cast<BinaryField&>(m->getBinaryField(name));

			field = reinterpret_cast<GMSEC_Field>(&f);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


const GMSEC_Field CALL_TYPE messageGetBooleanField(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			BooleanField& f = const_cast<BooleanField&>(m->getBooleanField(name));

			field = reinterpret_cast<GMSEC_Field>(&f);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


const GMSEC_Field CALL_TYPE messageGetCharField(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			CharField& f = const_cast<CharField&>(m->getCharField(name));

			field = reinterpret_cast<GMSEC_Field>(&f);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


const GMSEC_Field CALL_TYPE messageGetF32Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			F32Field& f = const_cast<F32Field&>(m->getF32Field(name));

			field = reinterpret_cast<GMSEC_Field>(&f);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


const GMSEC_Field CALL_TYPE messageGetF64Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			F64Field& f = const_cast<F64Field&>(m->getF64Field(name));

			field = reinterpret_cast<GMSEC_Field>(&f);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


const GMSEC_Field CALL_TYPE messageGetI8Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			I8Field& f = const_cast<I8Field&>(m->getI8Field(name));

			field = reinterpret_cast<GMSEC_Field>(&f);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


const GMSEC_Field CALL_TYPE messageGetI16Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			I16Field& f = const_cast<I16Field&>(m->getI16Field(name));

			field = reinterpret_cast<GMSEC_Field>(&f);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


const GMSEC_Field CALL_TYPE messageGetI32Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			I32Field& f = const_cast<I32Field&>(m->getI32Field(name));

			field = reinterpret_cast<GMSEC_Field>(&f);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


const GMSEC_Field CALL_TYPE messageGetI64Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			I64Field& f = const_cast<I64Field&>(m->getI64Field(name));

			field = reinterpret_cast<GMSEC_Field>(&f);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


const GMSEC_Field CALL_TYPE messageGetU8Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			U8Field& f = const_cast<U8Field&>(m->getU8Field(name));

			field = reinterpret_cast<GMSEC_Field>(&f);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


const GMSEC_Field CALL_TYPE messageGetU16Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			U16Field& f = const_cast<U16Field&>(m->getU16Field(name));

			field = reinterpret_cast<GMSEC_Field>(&f);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


const GMSEC_Field CALL_TYPE messageGetU32Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			U32Field& f = const_cast<U32Field&>(m->getU32Field(name));

			field = reinterpret_cast<GMSEC_Field>(&f);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


const GMSEC_Field CALL_TYPE messageGetU64Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			U64Field& f = const_cast<U64Field&>(m->getU64Field(name));

			field = reinterpret_cast<GMSEC_Field>(&f);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


const GMSEC_Field CALL_TYPE messageGetStringField(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			StringField& f = const_cast<StringField&>(m->getStringField(name));

			field = reinterpret_cast<GMSEC_Field>(&f);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


size_t CALL_TYPE messageGetFieldCount(GMSEC_Message msg, GMSEC_Status status)
{
	size_t count = 0;
	Status result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		count = m->getFieldCount();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return count;
}


void CALL_TYPE messageCopyFields(GMSEC_Message msg, GMSEC_Message toMsg, GMSEC_Status status)
{
	Status result;

	Message* m1 = reinterpret_cast<Message*>(msg);
	Message* m2 = reinterpret_cast<Message*>(toMsg);

	if (!m1)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else if (!m2)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "To-Message handle is NULL");
	}
	else
	{
		m1->copyFields(*m2);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


const char* CALL_TYPE messageToXML(GMSEC_Message msg, GMSEC_Status status)
{
	const char* xml = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		xml = m->toXML();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return xml;
}


const char* CALL_TYPE messageToJSON(GMSEC_Message msg, GMSEC_Status status)
{
	const char* json = NULL;
	Status      result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		json = m->toJSON();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return json;
}


size_t CALL_TYPE messageGetSize(GMSEC_Message msg, GMSEC_Status status)
{
	size_t size = 0;
	Status result;

	Message* m = reinterpret_cast<Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		size = m->getSize();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return size;
}


GMSEC_MessageFieldIterator CALL_TYPE messageGetFieldIterator(GMSEC_Message msg, GMSEC_Status status)
{
	GMSEC_MessageFieldIterator iter = NULL;
	Status                     result;

	Message* message = reinterpret_cast<Message*>(msg);

	if (message)
	{
		MessageFieldIterator& tmp = message->getFieldIterator();

		iter = (GMSEC_MessageFieldIterator) &tmp;
	}
	else
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return iter;
}
