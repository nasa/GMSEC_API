/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file i64_field.cpp
 *
 *  @brief This file contains functions for the management of an I64Field object.
 */


#include <gmsec5/c/field/i64_field.h>

#include <gmsec5/field/I64Field.h>

#include <gmsec5/GmsecException.h>
#include <gmsec5/Status.h>


using namespace gmsec::api5;


GMSEC_Field CALL_TYPE i64FieldCreate(const char* fieldName, GMSEC_I64 value, GMSEC_BOOL isHeader, GMSEC_Status status)
{
	I64Field* field = NULL;
	Status    result;

	try
	{
		field = new I64Field(fieldName, value, isHeader == GMSEC_BOOL::GMSEC_TRUE);
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


GMSEC_I64 CALL_TYPE i64FieldGetValue(GMSEC_Field field, GMSEC_Status status)
{
	Field*    f        = reinterpret_cast<Field*>(field);
	I64Field* i64Field = dynamic_cast<I64Field*>(f);
	GMSEC_I64 value    = 0;
	Status    result;

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
