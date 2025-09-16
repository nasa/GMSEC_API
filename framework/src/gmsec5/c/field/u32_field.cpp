/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file u32_field.cpp
 *
 *  @brief This file contains functions for the management of an U32Field object.
 */


#include <gmsec5/c/field/u32_field.h>

#include <gmsec5/field/U32Field.h>

#include <gmsec5/GmsecException.h>
#include <gmsec5/Status.h>


using namespace gmsec::api5;


GMSEC_Field CALL_TYPE u32FieldCreate(const char* fieldName, GMSEC_U32 value, GMSEC_BOOL isHeader, GMSEC_Status status)
{
	U32Field* field = NULL;
	Status    result;

	try
	{
		field = new U32Field(fieldName, value, isHeader == GMSEC_BOOL::GMSEC_TRUE);
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


GMSEC_U32 CALL_TYPE u32FieldGetValue(GMSEC_Field field, GMSEC_Status status)
{
	Field*    f        = reinterpret_cast<Field*>(field);
	U32Field* u32Field = dynamic_cast<U32Field*>(f);
	GMSEC_U32 value    = 0;
	Status    result;

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
