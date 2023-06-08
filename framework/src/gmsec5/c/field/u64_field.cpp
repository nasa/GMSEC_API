/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file u64_field.cpp
 *
 *  @brief This file contains functions for the management of an U64Field object.
 */


#include <gmsec5/c/field/u64_field.h>

#include <gmsec5/field/U64Field.h>

#include <gmsec5/GmsecException.h>
#include <gmsec5/Status.h>


using namespace gmsec::api5;


GMSEC_Field CALL_TYPE u64FieldCreate(const char* fieldName, GMSEC_U64 value, GMSEC_BOOL isHeader, GMSEC_Status status)
{
	U64Field* field = NULL;
	Status    result;

	try
	{
		field = new U64Field(fieldName, value, isHeader == GMSEC_BOOL::GMSEC_TRUE);
	}
	catch (const GmsecException& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


GMSEC_U64 CALL_TYPE u64FieldGetValue(GMSEC_Field field, GMSEC_Status status)
{
	Field*    f        = reinterpret_cast<Field*>(field);
	U64Field* u64Field = dynamic_cast<U64Field*>(f);
	GMSEC_U64 value    = 0;
	Status    result;

	if (u64Field)
	{
		value = u64Field->getValue();
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not a U64Field");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}
