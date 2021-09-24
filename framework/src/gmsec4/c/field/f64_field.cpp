/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file f64_field.cpp
 *
 *  @brief This file contains functions for the management of an F64Field object.
 */


#include <gmsec4/c/field/f64_field.h>

#include <gmsec4/field/F64Field.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;


GMSEC_Field CALL_TYPE f64FieldCreate(const char* fieldName, GMSEC_F64 value, GMSEC_Status status)
{
	F64Field* field = NULL;
	Status    result;

	try
	{
		field = new F64Field(fieldName, value);
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


GMSEC_F64 CALL_TYPE f64FieldGetValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field*    f        = reinterpret_cast<const Field*>(field);
	const F64Field* f64Field = dynamic_cast<const F64Field*>(f);
	GMSEC_F64       value    = 0;
	Status          result;

	if (f64Field)
	{
		value = f64Field->getValue();
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not an F64Field");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}
