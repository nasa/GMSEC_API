/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file mist_message.cpp
 *
 *  @brief This file contains functions for the management of generic MIST Messages.
 */


#include <gmsec4/c/mist/message/mist_message.h>

#include <gmsec4/mist/message/MistMessage.h>

#include <gmsec4/mist/Specification.h>

#include <gmsec4/Config.h>
#include <gmsec4/Exception.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::util;


GMSEC_Message CALL_TYPE mistMessageCreate(const char* subject,
	                                      const char* schemaID,
	                                      const GMSEC_Specification spec,
	                                      GMSEC_Status status)
{
	GMSEC_Message  msg = NULL;
	Specification* s   = reinterpret_cast<Specification*>(spec);
	Status         result;

	try
	{
		if (!s)
		{
			result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
		}
		else
		{
			msg = reinterpret_cast<GMSEC_Message>(new MistMessage(subject, schemaID, *s));
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


GMSEC_Message CALL_TYPE mistMessageCreateWithConfig(const char* subject,
	                                                const char* schemaID,
	                                                const GMSEC_Config config,
	                                                const GMSEC_Specification spec,
	                                                GMSEC_Status status)
{
	GMSEC_Message  msg = NULL;
	Config*        cfg = reinterpret_cast<Config*>(config);
	Specification* s   = reinterpret_cast<Specification*>(spec);
	Status         result;

	try
	{
		if (!cfg)
		{
			result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
		}
		else if (!s)
		{
			result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
		}
		else
		{
			msg = reinterpret_cast<GMSEC_Message>(new MistMessage(subject, schemaID, *cfg, *s));
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


GMSEC_Message CALL_TYPE mistMessageCreateUsingData(const char* data, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	try
	{
		GMSEC_DISABLE_DEPRECATED_WARNINGS

		msg = reinterpret_cast<GMSEC_Message>(new MistMessage(data));

		GMSEC_ENABLE_DEPRECATED_WARNINGS
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


GMSEC_Message CALL_TYPE mistMessageCreateUsingSpecAndData(const GMSEC_Specification spec, const char* data, GMSEC_Status status)
{
	GMSEC_Message  msg = NULL;
	Specification* s   = reinterpret_cast<Specification*>(spec);
	Status         result;

	if (!s)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else
	{
		try
		{
			msg = reinterpret_cast<GMSEC_Message>(new MistMessage(*s, data));
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

	return msg;
}


GMSEC_Message CALL_TYPE mistMessageCreateCopy(const GMSEC_Message other, GMSEC_Status status)
{
	GMSEC_Message msg      = NULL;
	Message*      tmpMsg   = reinterpret_cast<Message*>(other);
	MistMessage*  otherMsg = dynamic_cast<MistMessage*>(tmpMsg);
	Status        result;

	try
	{
		if (!otherMsg)
		{
			result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MIST Message handle is NULL");
		}
		else
		{
			msg = reinterpret_cast<GMSEC_Message>(new MistMessage(*otherMsg));
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


GMSEC_Message CALL_TYPE mistMessageCreateFromMessage(const GMSEC_Message msg, const GMSEC_Config specConfig, GMSEC_Status status)
{
	GMSEC_Message retMsg = NULL;
	Message*      cxxMsg = reinterpret_cast<Message*>(msg);
	Config*       cxxCfg = reinterpret_cast<Config*>(specConfig);
	Status        result;

	try
	{
		if (!cxxMsg)
		{
			result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
		}
		else
		{
			if (cxxCfg == NULL)
			{
				retMsg = reinterpret_cast<GMSEC_Message>(new MistMessage(*cxxMsg));
			}
			else
			{
				retMsg = reinterpret_cast<GMSEC_Message>(new MistMessage(*cxxMsg, *cxxCfg));
			}
		}
	}
	catch (const Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return retMsg;
}


void CALL_TYPE mistMessageSetStandardFields(const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
{
	Status result;

	if (!fields || numFields == 0)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Array of standard Fields is NULL or field count is zero");
	}
	else
	{
		DataList<Field*> standardFields;

		for (size_t i = 0; i < numFields; ++i)
		{
			standardFields.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		MistMessage::setStandardFields(standardFields);
	}

	if (status)
	{
		*((Status*) status) = result;
	}
}


void CALL_TYPE mistMessageClearStandardFields()
{
	MistMessage::clearStandardFields();
}


const char* CALL_TYPE mistMessageGetSchemaID(const GMSEC_Message msg, GMSEC_Status status)
{
	const char*  schemaID = NULL;
	Message*     tmpMsg   = reinterpret_cast<Message*>(msg);
	MistMessage* mistMsg  = dynamic_cast<MistMessage*>(tmpMsg);
	Status       result;

	if (!mistMsg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL or is not a MIST Message");
	}
	else
	{
		schemaID = mistMsg->getSchemaID();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return schemaID;
}


void CALL_TYPE mistMessageSetValue(const GMSEC_Message msg, const char* fieldName, const char* value, GMSEC_Status status)
{
	Message*     tmpMsg  = reinterpret_cast<Message*>(msg);
	MistMessage* mistMsg = dynamic_cast<MistMessage*>(tmpMsg);
	Status       result;

	if (!mistMsg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL or is not a MIST Message");
	}
	else
	{
		try
		{
			mistMsg->setValue(fieldName, value);
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
}


void CALL_TYPE mistMessageSetValueI64(const GMSEC_Message msg, const char* fieldName, GMSEC_I64 value, GMSEC_Status status)
{
	Message*     tmpMsg  = reinterpret_cast<Message*>(msg);
	MistMessage* mistMsg = dynamic_cast<MistMessage*>(tmpMsg);
	Status       result;

	if (!mistMsg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL or is not a MIST Message");
	}
	else
	{
		try
		{
			mistMsg->setValue(fieldName, value);
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
}


void CALL_TYPE mistMessageSetValueF64(const GMSEC_Message msg, const char* fieldName, GMSEC_F64 value, GMSEC_Status status)
{
	Message*     tmpMsg  = reinterpret_cast<Message*>(msg);
	MistMessage* mistMsg = dynamic_cast<MistMessage*>(tmpMsg);
	Status       result;

	if (!mistMsg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL or is not a MIST Message");
	}
	else
	{
		try
		{
			mistMsg->setValue(fieldName, value);
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
}
