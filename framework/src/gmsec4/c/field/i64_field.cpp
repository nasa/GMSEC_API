/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file i64_field.cpp
 *
 *  @brief This file contains functions for the management of an I64Field object.
 */


#include <gmsec4/c/field/i64_field.h>

#include <gmsec4/field/I64Field.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;


GMSEC_Field CALL_TYPE i64FieldCreate(const char* fieldName, GMSEC_I64 value, GMSEC_Status status)
{
	I64Field* field = NULL;
	Status    result;

	try
	{
		field = new I64Field(fieldName, value);
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


GMSEC_I64 CALL_TYPE i64FieldGetValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field*    f        = reinterpret_cast<const Field*>(field);
	const I64Field* i64Field = dynamic_cast<const I64Field*>(f);
	GMSEC_I64       value    = 0;
	Status          result;

	if (i64Field)
	{
		value = i64Field->getValue();
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not an I64Field");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}
