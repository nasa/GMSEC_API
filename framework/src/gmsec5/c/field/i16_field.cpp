/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file i16_field.cpp
 *
 *  @brief This file contains functions for the management of an I16Field object.
 */


#include <gmsec5/c/field/i16_field.h>

#include <gmsec5/field/I16Field.h>

#include <gmsec5/GmsecException.h>
#include <gmsec5/Status.h>


using namespace gmsec::api5;


GMSEC_Field CALL_TYPE i16FieldCreate(const char* fieldName, GMSEC_I16 value, GMSEC_BOOL isHeader, GMSEC_Status status)
{
	I16Field* field = NULL;
	Status    result;

	try
	{
		field = new I16Field(fieldName, value, isHeader == GMSEC_BOOL::GMSEC_TRUE);
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


GMSEC_I16 CALL_TYPE i16FieldGetValue(GMSEC_Field field, GMSEC_Status status)
{
	Field*    f        = reinterpret_cast<Field*>(field);
	I16Field* i16Field = dynamic_cast<I16Field*>(f);
	GMSEC_I16 value    = 0;
	Status    result;

	if (i16Field)
	{
		value = i16Field->getValue();
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not an I16Field");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}
