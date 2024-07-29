/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file message_factory.cpp
 *
 *  @brief This file contains functions for the management of a MessageFactory object.
 */


#include <gmsec5/c/message_factory.h>

#include <gmsec5/Config.h>
#include <gmsec5/Message.h>
#include <gmsec5/MessageFactory.h>
#include <gmsec5/Specification.h>
#include <gmsec5/Status.h>

#include <gmsec5/util/List.h>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


GMSEC_MessageFactory CALL_TYPE messageFactoryCreate(GMSEC_Status status)
{
	GMSEC_MessageFactory factory = NULL;
	Status result;

	try
	{
		factory = reinterpret_cast<GMSEC_MessageFactory>(new MessageFactory());
	}
	catch (const GmsecException& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return factory;
}


GMSEC_MessageFactory CALL_TYPE messageFactoryCreateUsingConfig(GMSEC_Config config, GMSEC_Status status)
{
	GMSEC_MessageFactory factory = NULL;
	Status result;

	Config* cfg = reinterpret_cast<Config*>(config);

	if (cfg == NULL)
	{
		result = Status(MSG_FACTORY_ERROR, UNINITIALIZED_OBJECT, "Config is NULL");
	}
	else
	{
		try
		{
			factory = reinterpret_cast<GMSEC_MessageFactory>(new MessageFactory(*cfg));
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

	return factory;
}


void CALL_TYPE messageFactoryDestroy(GMSEC_MessageFactory* factory)
{
	if (factory && *factory)
	{
		MessageFactory* f = reinterpret_cast<MessageFactory*>(*factory);

		delete f;

		*factory = NULL;
	}
}


void CALL_TYPE messageFactorySetStandardFields(GMSEC_MessageFactory factory, GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
{
	Status result;

	MessageFactory* f = reinterpret_cast<MessageFactory*>(factory);

	if (f == NULL)
	{
		result = Status(MSG_FACTORY_ERROR, UNINITIALIZED_OBJECT, "MessageFactory is NULL");
	}
	else if (!fields || numFields == 0)
	{
		result = Status(MSG_FACTORY_ERROR, UNINITIALIZED_OBJECT, "Array of fields is NULL or field count is zero");
	}
	else
	{
		List<Field*> l_fields;

		for (size_t i = 0; i < numFields; ++i)
		{
			l_fields.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		f->setStandardFields(l_fields);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE messageFactoryClearStandardFields(GMSEC_MessageFactory factory, GMSEC_Status status)
{
	Status result;

	MessageFactory* f = reinterpret_cast<MessageFactory*>(factory);

	if (f == NULL)
	{
		result = Status(MSG_FACTORY_ERROR, UNINITIALIZED_OBJECT, "MessageFactory is NULL");
	}
	else
	{
		f->clearStandardFields();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE messageFactorySetMessageConfig(GMSEC_MessageFactory factory, GMSEC_Config msgConfig, GMSEC_Status status)
{
	Status result;

	MessageFactory* f = reinterpret_cast<MessageFactory*>(factory);
	Config*         c = reinterpret_cast<Config*>(msgConfig);

	if (f == NULL)
	{
		result = Status(MSG_FACTORY_ERROR, UNINITIALIZED_OBJECT, "MessageFactory is NULL");
	}
	else if (c == NULL)
	{
		result = Status(MSG_FACTORY_ERROR, UNINITIALIZED_OBJECT, "Config is NULL");
	}
	else
	{
		f->setMessageConfig(*c);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_Message CALL_TYPE messageFactoryCreateSimpleMessage(GMSEC_MessageFactory factory, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status result;

	MessageFactory* f = reinterpret_cast<MessageFactory*>(factory);

	if (f == NULL)
	{
		result = Status(MSG_FACTORY_ERROR, UNINITIALIZED_OBJECT, "MessageFactory is NULL");
	}
	else
	{
		msg = reinterpret_cast<GMSEC_Message>( new Message( f->createMessage() ) );
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message CALL_TYPE messageFactoryCreateMessage(GMSEC_MessageFactory factory, const char* schemaID, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status result;

	MessageFactory* f = reinterpret_cast<MessageFactory*>(factory);

	if (f == NULL)
	{
		result = Status(MSG_FACTORY_ERROR, UNINITIALIZED_OBJECT, "MessageFactory is NULL");
	}
	else
	{
		try
		{
			Message* tmp = new Message(f->createMessage(schemaID));

			msg = reinterpret_cast<GMSEC_Message>(tmp);
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

	return msg;
}


GMSEC_Message CALL_TYPE messageFactoryFromData(GMSEC_MessageFactory factory, const char* data, DataType type, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status result;

	MessageFactory* f = reinterpret_cast<MessageFactory*>(factory);

	if (f == NULL)
	{
		result = Status(MSG_FACTORY_ERROR, UNINITIALIZED_OBJECT, "MessageFactory is NULL");
	}
	else
	{
		try
		{
			Message* tmp = new Message(f->fromData(data, type));

			msg = reinterpret_cast<GMSEC_Message>(tmp);
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

	return msg;
}


GMSEC_Specification CALL_TYPE messageFactoryGetSpecification(GMSEC_MessageFactory factory, GMSEC_Status status)
{
	GMSEC_Specification spec = NULL;
	Status result;

	MessageFactory* f = reinterpret_cast<MessageFactory*>(factory);

	if (f == NULL)
	{
		result = Status(MSG_FACTORY_ERROR, UNINITIALIZED_OBJECT, "MessageFactory is NULL");
	}
	else
	{
		spec = reinterpret_cast<GMSEC_Specification>( const_cast<Specification*>(&f->getSpecification()) );
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return spec;
}


void CALL_TYPE messageFactoryRegisterMessageValidator(GMSEC_MessageFactory factory, GMSEC_MessageValidator* validator, GMSEC_Status status)
{
	Status result;

	MessageFactory* f = reinterpret_cast<MessageFactory*>(factory);

	if (f == NULL)
	{
		result = Status(MSG_FACTORY_ERROR, UNINITIALIZED_OBJECT, "MessageFactory is NULL");
	}
	else
	{
		try
		{
			f->registerMessageValidator(validator);
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
