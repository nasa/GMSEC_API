/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file message.cpp
 *
 *  @brief This file contains function for managing messages.
 */

#include <gmsec4/c/message.h>

#include <gmsec4/Fields.h>

#include <gmsec4/Config.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>
#include <gmsec4/MessageFieldIterator.h>

using namespace gmsec::api;


GMSEC_Message CALL_TYPE messageCreate(const char* subject, GMSEC_MessageKind msgKind, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;;
	Status        result;

	try
	{
		int kind = static_cast<int>(msgKind);  // intermediate cast required by solaris
		msg = reinterpret_cast<GMSEC_Message>(new Message(subject, static_cast<Message::MessageKind>(kind)));
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message CALL_TYPE messageCreateWithConfig(const char* subject, GMSEC_MessageKind msgKind, const GMSEC_Config config, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;;
	Config*       cfg = reinterpret_cast<Config*>(config);
	Status        result;

	try
	{
		if (cfg)
		{
			int kind = static_cast<int>(msgKind);  // intermediate cast required by solaris
			msg = reinterpret_cast<GMSEC_Message>(new Message(subject, static_cast<Message::MessageKind>(kind), *cfg));
		}
		else
		{
			result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
		}
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message messageCreateUsingData(const char* data, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;;
	Status        result;

	try
	{
		msg = reinterpret_cast<GMSEC_Message>(new Message(data));
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message CALL_TYPE messageCreateCopy(const GMSEC_Message otherMsg, GMSEC_Status status)
{
	GMSEC_Message msg   = NULL;;
	Message*      other = reinterpret_cast<Message*>(otherMsg);
	Status        result;

	try
	{
		if (other)
		{
			msg = reinterpret_cast<GMSEC_Message>(new Message(*other));
		}
		else
		{
			result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
		}
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
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


void CALL_TYPE messageAddConfig(GMSEC_Message msg, const GMSEC_Config cfg, GMSEC_Status status)
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
		m->addConfig(*c);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


const GMSEC_Config CALL_TYPE messageGetConfig(const GMSEC_Message msg, GMSEC_Status status)
{
	GMSEC_Config cfg = NULL;
	Status       result;

	const Message* m = reinterpret_cast<const Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		const Config& config = m->getConfig();

		cfg = (GMSEC_Config) &config;
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return cfg;
}


const char* CALL_TYPE messageGetSubject(const GMSEC_Message msg, GMSEC_Status status)
{
	const char* subject = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (const Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_MessageKind CALL_TYPE messageGetKind(const GMSEC_Message msg, GMSEC_Status status)
{
	GMSEC_MessageKind kind = GMSEC_PUBLISH;
	Status            result;

	const Message* m = reinterpret_cast<const Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		int tmp = m->getKind();
		kind = static_cast<GMSEC_MessageKind>(tmp);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return kind;
}


GMSEC_BOOL CALL_TYPE messageAddField(GMSEC_Message msg, const GMSEC_Field field, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
		overwrite = (m->addField(*f) ? GMSEC_TRUE : GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwrite;
}


GMSEC_BOOL CALL_TYPE messageAddBinaryField(GMSEC_Message msg, const char* name, GMSEC_BIN data, size_t dataLen, GMSEC_Status status)
{
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
			overwrite = (m->addField(name, data, dataLen) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
			overwrite = (m->addField(name, value == GMSEC_TRUE) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
			overwrite = (m->addField(name, value) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
			overwrite = (m->addField(name, value) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
			overwrite = (m->addField(name, value) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
			overwrite = (m->addField(name, value) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
			overwrite = (m->addField(name, value) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
			overwrite = (m->addField(name, value) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
			overwrite = (m->addField(name, value) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
			overwrite = (m->addField(name, value) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
			overwrite = (m->addField(name, value) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
			overwrite = (m->addField(name, value) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
			overwrite = (m->addField(name, value) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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
	GMSEC_BOOL overwrite = GMSEC_FALSE;
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
			overwrite = (m->addField(name, value) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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


GMSEC_BOOL CALL_TYPE messageAddFields(GMSEC_Message msg, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
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
				fieldsReplaced |= (messageAddField(msg, fields[i], NULL) == GMSEC_TRUE);
			}
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return (fieldsReplaced ? GMSEC_TRUE : GMSEC_FALSE);
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
	GMSEC_BOOL removed = GMSEC_FALSE;
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
			removed = (m->clearField(name) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
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


GMSEC_I64 CALL_TYPE messageGetIntegerValue(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
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
			value = m->getIntegerValue(fieldName);
		}
		catch (Exception& e)
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


GMSEC_U64 CALL_TYPE messageGetUnsignedIntegerValue(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
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
			value = m->getUnsignedIntegerValue(fieldName);
		}
		catch (Exception& e)
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


GMSEC_F64 CALL_TYPE messageGetDoubleValue(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
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
			value = m->getDoubleValue(fieldName);
		}
		catch (Exception& e)
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
		catch (Exception& e)
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


GMSEC_BOOL CALL_TYPE messageHasField(GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_BOOL  found = GMSEC_FALSE;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			found = (m->hasField(name) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (const Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return found;
}


const GMSEC_Field CALL_TYPE messageGetField(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


GMSEC_FieldType CALL_TYPE messageGetFieldType(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_FieldType fieldType = GMSEC_BAD_TYPE;
	Status          result;

	const Message* m = reinterpret_cast<const Message*>(msg);

	if (!m)
	{
		result = Status(MSG_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			int tmp = m->getFieldType(name);

			fieldType = static_cast<GMSEC_FieldType>(tmp);
		}
		catch (Exception& e)
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


const GMSEC_Field CALL_TYPE messageGetBinaryField(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


const GMSEC_Field CALL_TYPE messageGetBooleanField(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


const GMSEC_Field CALL_TYPE messageGetCharField(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


const GMSEC_Field CALL_TYPE messageGetF32Field(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


const GMSEC_Field CALL_TYPE messageGetF64Field(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


const GMSEC_Field CALL_TYPE messageGetI8Field(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


const GMSEC_Field CALL_TYPE messageGetI16Field(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


const GMSEC_Field CALL_TYPE messageGetI32Field(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


const GMSEC_Field CALL_TYPE messageGetI64Field(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


const GMSEC_Field CALL_TYPE messageGetU8Field(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


const GMSEC_Field CALL_TYPE messageGetU16Field(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


const GMSEC_Field CALL_TYPE messageGetU32Field(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


const GMSEC_Field CALL_TYPE messageGetU64Field(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


const GMSEC_Field CALL_TYPE messageGetStringField(const GMSEC_Message msg, const char* name, GMSEC_Status status)
{
	GMSEC_Field field = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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
		catch (Exception& e)
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


size_t CALL_TYPE messageGetFieldCount(const GMSEC_Message msg, GMSEC_Status status)
{
	size_t count = 0;
	Status result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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


void CALL_TYPE messageCopyFields(const GMSEC_Message msg, GMSEC_Message toMsg, GMSEC_Status status)
{
	Status result;

	const Message* m1 = reinterpret_cast<const Message*>(msg);
	Message*       m2 = reinterpret_cast<Message*>(toMsg);

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


const char* CALL_TYPE messageToXML(const GMSEC_Message msg, GMSEC_Status status)
{
	const char* xml = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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


const char* CALL_TYPE messageToJSON(const GMSEC_Message msg, GMSEC_Status status)
{
	const char* json = NULL;
	Status      result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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


size_t CALL_TYPE messageGetSize(const GMSEC_Message msg, GMSEC_Status status)
{
	size_t size = 0;
	Status result;

	const Message* m = reinterpret_cast<const Message*>(msg);

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


GMSEC_MessageFieldIterator CALL_TYPE messageGetFieldIterator(const GMSEC_Message msg, GMSEC_Status status)
{
	const Message*         message = reinterpret_cast<const Message*>(msg);
	GMSEC_MessageFieldIterator iter    = NULL;
	Status                 result;

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
