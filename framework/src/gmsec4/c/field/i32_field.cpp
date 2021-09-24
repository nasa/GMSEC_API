/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file i32_field.cpp
 *
 *  @brief This file contains functions for the management of an I32Field object.
 */


#include <gmsec4/c/field/i32_field.h>

#include <gmsec4/field/I32Field.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;


GMSEC_Field CALL_TYPE i32FieldCreate(const char* fieldName, GMSEC_I32 value, GMSEC_Status status)
{
	I32Field* field = NULL;
	Status    result;

	try
	{
		field = new I32Field(fieldName, value);
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


GMSEC_I32 CALL_TYPE i32FieldGetValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field*    f        = reinterpret_cast<const Field*>(field);
	const I32Field* i32Field = dynamic_cast<const I32Field*>(f);
	GMSEC_I32       value    = 0;
	Status          result;

	if (i32Field)
	{
		value = i32Field->getValue();
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not an I32Field");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}
