/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file specification.cpp
 *
 *  @brief This file contains functions for the management of a Specification object.
 */


#include <gmsec4/c/mist/specification.h>

#include <gmsec4/c/mist/schema_id_iterator.h>

#include <gmsec4/Config.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>

#include <gmsec4/mist/SchemaIDIterator.h>
#include <gmsec4/mist/Specification.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;


GMSEC_Specification CALL_TYPE specificationCreate(const GMSEC_Config config, GMSEC_Status status)
{
	GMSEC_Specification spec = NULL;
	const Config*       cfg  = reinterpret_cast<const Config*>(config);
	Status              result;

	try
	{
		if (cfg)
		{
			spec = reinterpret_cast<GMSEC_Specification>(new Specification(*cfg));
		}
		else
		{
			result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
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

	return spec;
}


GMSEC_Specification CALL_TYPE specificationCreateCopy(const GMSEC_Specification otherSpec, GMSEC_Status status)
{
	GMSEC_Specification  spec  = NULL;
	const Specification* other = reinterpret_cast<const Specification*>(otherSpec);
	Status               result;

	try
	{
		if (other)
		{
			spec = reinterpret_cast<GMSEC_Specification>(new Specification(*other));
		}
		else
		{
			result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
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

	return spec;
}


void CALL_TYPE specificationDestroy(GMSEC_Specification* spec)
{
	if (spec)
	{
		Specification* s = reinterpret_cast<Specification*>(*spec);

		delete s;

		*spec = NULL;
	}
}


void CALL_TYPE specificationValidateMessage(GMSEC_Specification spec, const GMSEC_Message msg, GMSEC_Status status)
{
	Specification* s = reinterpret_cast<Specification*>(spec);
	const Message* m = reinterpret_cast<const Message*>(msg);
	Status         result;

	if (!s)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else if (!m)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			s->validateMessage(*m);
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


GMSEC_SchemaIDIterator CALL_TYPE specificationGetSchemaIDIterator(GMSEC_Specification spec, GMSEC_Status status)
{
	Specification*         s    = reinterpret_cast<Specification*>(spec);
	GMSEC_SchemaIDIterator iter = NULL;
	Status                 result;

	if (!s)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else
	{
		SchemaIDIterator& tmp = s->getSchemaIDIterator();

		iter = reinterpret_cast<GMSEC_SchemaIDIterator>(&tmp);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return iter;
}


unsigned int CALL_TYPE specificationGetVersion(GMSEC_Specification spec, GMSEC_Status status)
{
	Specification* s       = reinterpret_cast<Specification*>(spec);
	unsigned int   version = 0;
	Status         result;

	if (!s)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else
	{
		version = s->getVersion();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return version;
}


const char* CALL_TYPE specificationGetTemplateXML(GMSEC_Specification spec, const char* subject, const char* schemaID, GMSEC_Status status)
{
	Specification* s   = reinterpret_cast<Specification*>(spec);
	const char*    xml = NULL;
	Status         result;

	if (!s)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else if (!subject)
	{
		result = Status(MIST_ERROR, INVALID_SUBJECT_NAME, "Subject is NULL");
	}
	else if (!schemaID)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "SchemaID is NULL");
	}
	else
	{
		xml = s->getTemplateXML(subject, schemaID);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return xml;
}
