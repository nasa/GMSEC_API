/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file u32_field.cpp
 *
 *  @brief This file contains functions for the management of an U32Field object.
 */


#include <gmsec4/c/field/u32_field.h>

#include <gmsec4/field/U32Field.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;


GMSEC_Field CALL_TYPE u32FieldCreate(const char* fieldName, GMSEC_U32 value, GMSEC_Status status)
{
	U32Field* field = NULL;
	Status    result;

	try
	{
		field = new U32Field(fieldName, value);
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


GMSEC_U32 CALL_TYPE u32FieldGetValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field*    f        = reinterpret_cast<const Field*>(field);
	const U32Field* u32Field = dynamic_cast<const U32Field*>(f);
	GMSEC_U32       value    = 0;
	Status          result;

	if (u32Field)
	{
		value = u32Field->getValue();
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not a U32Field");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}
