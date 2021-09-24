/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file u8_field.cpp
 *
 *  @brief This file contains functions for the management of an U8Field object.
 */


#include <gmsec4/c/field/u8_field.h>

#include <gmsec4/field/U8Field.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;


GMSEC_Field CALL_TYPE u8FieldCreate(const char* fieldName, GMSEC_U8 value, GMSEC_Status status)
{
	U8Field* field = NULL;
	Status   result;

	try
	{
		field = new U8Field(fieldName, value);
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


GMSEC_U8 CALL_TYPE u8FieldGetValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field*   f       = reinterpret_cast<const Field*>(field);
	const U8Field* u8Field = dynamic_cast<const U8Field*>(f);
	GMSEC_U8       value   = 0;
	Status         result;

	if (u8Field)
	{
		value = u8Field->getValue();
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not a U8Field");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}
