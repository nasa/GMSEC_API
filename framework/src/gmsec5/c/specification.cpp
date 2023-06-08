/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file specification.cpp
 *
 *  @brief This file contains functions for the management of a Specification object.
 */


#include <gmsec5/c/specification.h>

#include <gmsec5/c/schema_id_iterator.h>

#include <gmsec5/internal/StringUtil.h>

#include <gmsec5/Config.h>
#include <gmsec5/FieldSpecification.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/Message.h>
#include <gmsec5/MessageSpecification.h>
#include <gmsec5/SchemaIDIterator.h>
#include <gmsec5/Specification.h>

#include <gmsec5/util/List.h>


using namespace gmsec::api5;
using namespace gmsec::api5::util;


GMSEC_Specification CALL_TYPE specificationCreate(GMSEC_Status status)
{
	GMSEC_Specification spec = NULL;
	Status              result;

	try
	{
		spec = reinterpret_cast<GMSEC_Specification>(new Specification());
	}
	catch (const GmsecException& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return spec;
}


GMSEC_Specification CALL_TYPE specificationCreateUsingConfig(GMSEC_Config config, GMSEC_Status status)
{
	GMSEC_Specification spec = NULL;
	Config*             cfg  = reinterpret_cast<Config*>(config);
	Status              result;

	if (!cfg)
	{
		result = Status(SPECIFICATION_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else
	{
		try
		{
			spec = reinterpret_cast<GMSEC_Specification>(new Specification(*cfg));
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

	return spec;
}


GMSEC_Specification CALL_TYPE specificationCreateCopy(GMSEC_Specification otherSpec, GMSEC_Status status)
{
	GMSEC_Specification spec  = NULL;
	Specification*      other = reinterpret_cast<Specification*>(otherSpec);
	Status               result;

	if (!other)
	{
		result = Status(SPECIFICATION_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else
	{
		spec = reinterpret_cast<GMSEC_Specification>(new Specification(*other));
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


GMSEC_SchemaIDIterator CALL_TYPE specificationGetSchemaIDIterator(GMSEC_Specification spec, GMSEC_Status status)
{
	Specification*         s    = reinterpret_cast<Specification*>(spec);
	GMSEC_SchemaIDIterator iter = NULL;
	Status                 result;

	if (!s)
	{
		result = Status(SPECIFICATION_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
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
		result = Status(SPECIFICATION_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
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

	GMSEC_SchemaLevel level = GMSEC_SchemaLevel::GMSEC_SCHEMA_LEVEL_0;

	if (!s)
	{
		result = Status(SPECIFICATION_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else
	{
		switch (s->getSchemaLevel())
		{
		case Specification::SchemaLevel::LEVEL_0: level = GMSEC_SchemaLevel::GMSEC_SCHEMA_LEVEL_0; break;
		case Specification::SchemaLevel::LEVEL_1: level = GMSEC_SchemaLevel::GMSEC_SCHEMA_LEVEL_1; break;
		case Specification::SchemaLevel::LEVEL_2: level = GMSEC_SchemaLevel::GMSEC_SCHEMA_LEVEL_2; break;
		case Specification::SchemaLevel::LEVEL_3: level = GMSEC_SchemaLevel::GMSEC_SCHEMA_LEVEL_3; break;
		case Specification::SchemaLevel::LEVEL_4: level = GMSEC_SchemaLevel::GMSEC_SCHEMA_LEVEL_4; break;
		case Specification::SchemaLevel::LEVEL_5: level = GMSEC_SchemaLevel::GMSEC_SCHEMA_LEVEL_5; break;
		case Specification::SchemaLevel::LEVEL_6: level = GMSEC_SchemaLevel::GMSEC_SCHEMA_LEVEL_6; break;
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return level;
}


void CALL_TYPE specificationGetMessageSpecifications(GMSEC_Specification spec, GMSEC_MessageSpecification** msgSpecs, size_t* numMsgSpecs, GMSEC_Status status)
{
	Specification* s = reinterpret_cast<Specification*>(spec);
	Status         result;

	*msgSpecs    = NULL;
	*numMsgSpecs = 0;

	if (!s)
	{
		result = Status(SPECIFICATION_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else
	{
		const List<MessageSpecification*>& cppMsgSpecs = s->getMessageSpecifications();

		*msgSpecs    = new GMSEC_MessageSpecification[cppMsgSpecs.size()];
		*numMsgSpecs = cppMsgSpecs.size();

		size_t spec = 0;
		for (List<MessageSpecification*>::const_iterator it = cppMsgSpecs.begin(); it != cppMsgSpecs.end(); ++it)
		{
			const List<FieldSpecification*>& cppFldSpecs = (*it)->getFieldSpecifications();

			(*msgSpecs)[spec].schemaID        = StringUtil::stringNew((*it)->getSchemaID());
			(*msgSpecs)[spec].subjectTemplate = StringUtil::stringNew((*it)->getSubjectTemplate());
			(*msgSpecs)[spec].fieldSpecs      = new GMSEC_FieldSpecification[cppFldSpecs.size()];
			(*msgSpecs)[spec].numFieldSpecs   = cppFldSpecs.size();

			size_t fld = 0;
			for (List<FieldSpecification*>::const_iterator it2 = cppFldSpecs.begin(); it2 != cppFldSpecs.end(); ++it2)
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


void CALL_TYPE specificationDestroyMessageSpecifications(GMSEC_MessageSpecification* msgSpecs, size_t numMsgSpecs, GMSEC_Status status)
{
	Status result;

	if (!msgSpecs)
	{
		result = Status(SPECIFICATION_ERROR, UNINITIALIZED_OBJECT, "GMSEC_MessageSpecification is NULL");
	}
	else
	{
		for (size_t i = 0; i < numMsgSpecs; ++i)
		{
			StringUtil::stringDestroy(msgSpecs[i].schemaID);
			StringUtil::stringDestroy(msgSpecs[i].subjectTemplate);

			for (size_t j = 0; j < msgSpecs[i].numFieldSpecs; ++j)
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

void CALL_TYPE specificationGetHeaderFieldNames(GMSEC_Specification spec, const char* schemaName, const char*** headerFields, size_t* numHeaderFields, GMSEC_Status status)
{
	Specification*	s = reinterpret_cast<Specification*>(spec);
	Status			result;
	
	*headerFields = NULL;
	*numHeaderFields = 0;

	if (!s)
	{
		result = Status(SPECIFICATION_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else
	{
		List<const char*> hdrFieldList = s->getHeaderFieldNames(schemaName);

		*headerFields = new const char* [hdrFieldList.size()];
		*numHeaderFields = hdrFieldList.size();

		int i = 0;
		for (List<const char*>::const_iterator it = hdrFieldList.begin(); it != hdrFieldList.end(); ++it)
		{
			(*headerFields)[i++] = *it;
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE specificationDestroyHeaderList(const char*** headerFields)
{
	if (*headerFields)
	{
		delete[] * headerFields;
	}
}