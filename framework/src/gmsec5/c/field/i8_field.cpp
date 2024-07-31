/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file i8_field.cpp
 *
 *  @brief This file contains functions for the management of an I8Field object.
 */


#include <gmsec5/c/field/i8_field.h>

#include <gmsec5/field/I8Field.h>

#include <gmsec5/GmsecException.h>
#include <gmsec5/Status.h>


using namespace gmsec::api5;


GMSEC_Field CALL_TYPE i8FieldCreate(const char* fieldName, GMSEC_I8 value, GMSEC_BOOL isHeader, GMSEC_Status status)
{
	I8Field* field = NULL;
	Status   result;

	try
	{
		field = new I8Field(fieldName, value, isHeader == GMSEC_BOOL::GMSEC_TRUE);
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


GMSEC_I8 CALL_TYPE i8FieldGetValue(GMSEC_Field field, GMSEC_Status status)
{
	Field*   f       = reinterpret_cast<Field*>(field);
	I8Field* i8Field = dynamic_cast<I8Field*>(f);
	GMSEC_I8 value   = 0;
	Status   result;

	if (i8Field)
	{
		value = i8Field->getValue();
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not an I8Field");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}
