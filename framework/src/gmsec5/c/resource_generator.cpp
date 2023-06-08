/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file resource_generator.cpp
 *
 *  @brief This file contains a class for starting a resource message generator.
 */


#include <gmsec5/c/resource_generator.h>

#include <gmsec5/field/Field.h>

#include <gmsec5/util/List.h>

#include <gmsec5/Config.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/ResourceGenerator.h>


using namespace gmsec::api5;
using namespace gmsec::api5::util;


GMSEC_ResourceGenerator CALL_TYPE resourceGeneratorCreate(GMSEC_Config config,
                                                          GMSEC_U16    pubRate,
                                                          GMSEC_U16    sampleInterval,
                                                          GMSEC_U16    averageInterval,
                                                          GMSEC_Status status)
{
	return resourceGeneratorCreateWithFields(config, pubRate, sampleInterval, averageInterval, NULL, 0, status);
}


GMSEC_ResourceGenerator CALL_TYPE resourceGeneratorCreateWithFields(GMSEC_Config config,
                                                                    GMSEC_U16    pubRate,
                                                                    GMSEC_U16    sampleInterval,
                                                                    GMSEC_U16    averageInterval,
                                                                    GMSEC_Field  fields[],
                                                                    size_t       numFields,
                                                                    GMSEC_Status status)
{
	GMSEC_ResourceGenerator rsrcgen = NULL;

	Config* cfg = reinterpret_cast<Config*>(config);
	Status  result;

	if (!cfg)
	{
		result = Status(RESOURCE_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else if (numFields > 0 && fields == NULL)
	{
		result = Status(RESOURCE_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "Number of fields is non-zero and fields array is NULL");
	}
	else
	{
		List<Field*> list_fields;

		for (size_t i = 0; i < numFields; ++i)
		{
			list_fields.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		try
		{
			rsrcgen = reinterpret_cast<GMSEC_ResourceGenerator>(new ResourceGenerator(*cfg, pubRate, sampleInterval, averageInterval, list_fields));
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

	return rsrcgen;
}


void CALL_TYPE resourceGeneratorDestroy(GMSEC_ResourceGenerator* rsrcgen)
{
	if (rsrcgen && *rsrcgen)
	{
		ResourceGenerator* rg = reinterpret_cast<ResourceGenerator*>(*rsrcgen);
		delete rg;
		*rsrcgen = NULL;
	}
}


GMSEC_BOOL CALL_TYPE resourceGeneratorStart(GMSEC_ResourceGenerator rsrcgen, GMSEC_Status status)
{
	GMSEC_BOOL rsrcgenStatus = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	ResourceGenerator* rg = reinterpret_cast<ResourceGenerator*>(rsrcgen);

	if (!rg)
	{
		result = Status(RESOURCE_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "ResourceGenerator handle is NULL");
	}
	else
	{
		try
		{
			rsrcgenStatus = (rg->start() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
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

	return rsrcgenStatus;
}


GMSEC_BOOL CALL_TYPE resourceGeneratorStop(GMSEC_ResourceGenerator rsrcgen, GMSEC_Status status)
{
	GMSEC_BOOL rsrcgenStatus = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	ResourceGenerator* rg = reinterpret_cast<ResourceGenerator*>(rsrcgen);

	if (!rg)
	{
		result = Status(RESOURCE_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "ResourceGenerator handle is NULL");
	}
	else
	{
		rsrcgenStatus = (rg->stop() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return rsrcgenStatus;
}


GMSEC_BOOL CALL_TYPE resourceGeneratorIsRunning(GMSEC_ResourceGenerator rsrcgen, GMSEC_Status status)
{
	GMSEC_BOOL isRunning = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	ResourceGenerator* rg = reinterpret_cast<ResourceGenerator*>(rsrcgen);

	if (!rg)
	{
		result = Status(RESOURCE_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "ResourceGenerator handle is NULL");
	}
	else
	{
		isRunning = (rg->isRunning() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return isRunning;
}


GMSEC_BOOL CALL_TYPE resourceGeneratorSetField(GMSEC_ResourceGenerator rsrcgen, GMSEC_Field field, GMSEC_Status status)
{
	GMSEC_BOOL overwritten = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	ResourceGenerator* rg = reinterpret_cast<ResourceGenerator*>(rsrcgen);
	Field*             f  = reinterpret_cast<Field*>(field);

	if (!rg)
	{
		result = Status(RESOURCE_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "ResourceGenerator handle is NULL");
	}
	else if (!f)
	{
		result = Status(RESOURCE_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "Field handle is NULL");
	}
	else
	{
		try
		{
			overwritten = (rg->setField(*f) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
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

	return overwritten;
}


GMSEC_Message CALL_TYPE createResourceMessage(GMSEC_MessageFactory factory, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval, GMSEC_Status status)
{
	GMSEC_Message rsrcMsg = NULL;
	Status        result;

	MessageFactory* mf = reinterpret_cast<MessageFactory*>(factory);

	if (!mf)
	{
		result = Status(RESOURCE_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "MessageFactory handle is NULL");
	}
	else
	{
		try
		{
			Message tmp = ResourceGenerator::createResourceMessage(*mf, sampleInterval, averageInterval);

			rsrcMsg = reinterpret_cast<GMSEC_Message>( new Message(tmp) );
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

	return rsrcMsg;
}
