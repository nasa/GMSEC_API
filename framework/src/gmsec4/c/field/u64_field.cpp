/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file u64_field.cpp
 *
 *  @brief This file contains functions for the management of an U64Field object.
 */


#include <gmsec4/c/field/u64_field.h>

#include <gmsec4/field/U64Field.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;


GMSEC_Field CALL_TYPE u64FieldCreate(const char* fieldName, GMSEC_U64 value, GMSEC_Status status)
{
	U64Field* field = NULL;
	Status    result;

	try
	{
		field = new U64Field(fieldName, value);
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


GMSEC_U64 CALL_TYPE u64FieldGetValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field*    f        = reinterpret_cast<const Field*>(field);
	const U64Field* u64Field = dynamic_cast<const U64Field*>(f);
	GMSEC_U64       value    = 0;
	Status          result;

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
