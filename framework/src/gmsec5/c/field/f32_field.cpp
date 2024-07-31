/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file f32_field.cpp
 *
 *  @brief This file contains functions for the management of an F32Field object.
 */


#include <gmsec5/c/field/f32_field.h>

#include <gmsec5/field/F32Field.h>

#include <gmsec5/GmsecException.h>
#include <gmsec5/Status.h>


using namespace gmsec::api5;


GMSEC_Field CALL_TYPE f32FieldCreate(const char* fieldName, GMSEC_F32 value, GMSEC_BOOL isHeader, GMSEC_Status status)
{
	F32Field* field = NULL;
	Status    result;

	try
	{
		field = new F32Field(fieldName, value, isHeader == GMSEC_BOOL::GMSEC_TRUE);
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


GMSEC_F32 CALL_TYPE f32FieldGetValue(GMSEC_Field field, GMSEC_Status status)
{
	Field*    f        = reinterpret_cast<Field*>(field);
	F32Field* f32Field = dynamic_cast<F32Field*>(f);
	GMSEC_F32 value    = 0;
	Status    result;

	if (f32Field)
	{
		value = f32Field->getValue();
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not an F32Field");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}
