/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file u16_field.cpp
 *
 *  @brief This file contains functions for the management of an U16Field object.
 */


#include <gmsec4/c/field/u16_field.h>

#include <gmsec4/field/U16Field.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;


GMSEC_Field CALL_TYPE u16FieldCreate(const char* fieldName, GMSEC_U16 value, GMSEC_Status status)
{
	U16Field* field = NULL;
	Status    result;

	try
	{
		field = new U16Field(fieldName, value);
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


GMSEC_U16 CALL_TYPE u16FieldGetValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field*    f        = reinterpret_cast<const Field*>(field);
	const U16Field* u16Field = dynamic_cast<const U16Field*>(f);
	GMSEC_U16       value    = 0;
	Status          result;

	if (u16Field)
	{
		value = u16Field->getValue();
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not a U16Field");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}
