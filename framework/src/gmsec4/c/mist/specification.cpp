/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/mist/FieldSpecification.h>
#include <gmsec4/mist/MessageSpecification.h>
#include <gmsec4/mist/SchemaIDIterator.h>
#include <gmsec4/mist/Specification.h>

#include <gmsec4/Config.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::util;


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
			// We need to be explicit in calling the base-class validateMessage()
			// for cases where a user may have registered their own custom function
			// using connectionManagerSetSpecification().
			//
			// In such cases, the user's custom function will be called automatically
			// when message validation needs to take place, and the user's function in
			// turn may call the function we are in now.  By being explicit with the call
			// below, we avoid a severe case of recursion.
			//
			s->Specification::validateMessage(*m);
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


GMSEC_SchemaLevel CALL_TYPE specificationGetSchemaLevel(GMSEC_Specification spec, GMSEC_Status status)
{
	Specification* s = reinterpret_cast<Specification*>(spec);
	Status         result;

	GMSEC_SchemaLevel level = GMSEC_SCHEMA_LEVEL_0;

	if (!s)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else
	{
		switch (s->getSchemaLevel())
		{
		case Specification::LEVEL_0: level = GMSEC_SCHEMA_LEVEL_0; break;
		case Specification::LEVEL_1: level = GMSEC_SCHEMA_LEVEL_1; break;
		case Specification::LEVEL_2: level = GMSEC_SCHEMA_LEVEL_2; break;
		case Specification::LEVEL_3: level = GMSEC_SCHEMA_LEVEL_3; break;
		case Specification::LEVEL_4: level = GMSEC_SCHEMA_LEVEL_4; break;
		case Specification::LEVEL_5: level = GMSEC_SCHEMA_LEVEL_5; break;
		case Specification::LEVEL_6: level = GMSEC_SCHEMA_LEVEL_6; break;
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return level;
}


void CALL_TYPE specificationGetMessageSpecifications(GMSEC_Specification spec, GMSEC_MessageSpecification** msgSpecs, int* numMsgSpecs, GMSEC_Status status)
{
	Specification* s = reinterpret_cast<Specification*>(spec);
	Status         result;

	if (!s)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else
	{
		const DataList<MessageSpecification*>& cppMsgSpecs = s->getMessageSpecifications();

		*msgSpecs    = new GMSEC_MessageSpecification[cppMsgSpecs.size()];
		*numMsgSpecs = (int) cppMsgSpecs.size();

		int spec = 0;
		for (DataList<MessageSpecification*>::const_iterator it = cppMsgSpecs.begin(); it != cppMsgSpecs.end(); ++it)
		{
			const DataList<FieldSpecification*>& cppFldSpecs = (*it)->getFieldSpecifications();

			(*msgSpecs)[spec].schemaID      = StringUtil::stringNew((*it)->getSchemaID());
			(*msgSpecs)[spec].fieldSpecs    = new GMSEC_FieldSpecification[cppFldSpecs.size()];
			(*msgSpecs)[spec].numFieldSpecs = (int) cppFldSpecs.size();

			int fld = 0;
			for (DataList<FieldSpecification*>::const_iterator it2 = cppFldSpecs.begin(); it2 != cppFldSpecs.end(); ++it2)
			{
				(*msgSpecs)[spec].fieldSpecs[fld].name           = StringUtil::stringNew((*it2)->getName());
				(*msgSpecs)[spec].fieldSpecs[fld].type           = StringUtil::stringNew((*it2)->getType());
				(*msgSpecs)[spec].fieldSpecs[fld].mode           = StringUtil::stringNew((*it2)->getMode());
				(*msgSpecs)[spec].fieldSpecs[fld].classification = StringUtil::stringNew((*it2)->getClassification());
				(*msgSpecs)[spec].fieldSpecs[fld].value          = StringUtil::stringNew((*it2)->getValue());
				(*msgSpecs)[spec].fieldSpecs[fld].description    = StringUtil::stringNew((*it2)->getDescription());

				++fld;
			}

			++spec;
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE specificationDestroyMessageSpecifications(GMSEC_MessageSpecification* msgSpecs, int numMsgSpecs, GMSEC_Status status)
{
	Status result;

	if (!msgSpecs)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "GMSEC_MessageSpecification is NULL");
	}
	else
	{
		for (int i = 0; i < numMsgSpecs; ++i)
		{
			StringUtil::stringDestroy(msgSpecs[i].schemaID);

			for (int j = 0; j < msgSpecs[i].numFieldSpecs; ++j)
			{
				StringUtil::stringDestroy(msgSpecs[i].fieldSpecs[j].name);
				StringUtil::stringDestroy(msgSpecs[i].fieldSpecs[j].type);
				StringUtil::stringDestroy(msgSpecs[i].fieldSpecs[j].mode);
				StringUtil::stringDestroy(msgSpecs[i].fieldSpecs[j].classification);
				StringUtil::stringDestroy(msgSpecs[i].fieldSpecs[j].value);
				StringUtil::stringDestroy(msgSpecs[i].fieldSpecs[j].description);
			}

			delete [] msgSpecs[i].fieldSpecs;
		}

		delete [] msgSpecs;
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE specificationRegisterMessageValidator(GMSEC_Specification spec, GMSEC_MessageValidator* validator, GMSEC_Status status)
{
	Status result;

	Specification* s = reinterpret_cast<Specification*>(spec);

	if (s == NULL)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else if (validator == NULL || *validator == NULL)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MessageValidator function is NULL");
	}
	else
	{
		s->registerMessageValidator(validator);
	}

	if (status)
	{
		*((Status*) status) = result;
	}
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
		GMSEC_DISABLE_DEPRECATED_WARNINGS

		xml = s->getTemplateXML(subject, schemaID);

		GMSEC_ENABLE_DEPRECATED_WARNINGS
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return xml;
}
