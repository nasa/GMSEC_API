/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file boolean_field.cpp
 *
 *  @brief This file contains functions for the management of a BooleanField object.
 */


#include <gmsec5/c/field/boolean_field.h>

#include <gmsec5/field/BooleanField.h>

#include <gmsec5/GmsecException.h>
#include <gmsec5/Status.h>


using namespace gmsec::api5;


GMSEC_Field CALL_TYPE booleanFieldCreate(const char* fieldName, GMSEC_BOOL value, GMSEC_BOOL isHeader, GMSEC_Status status)
{
	BooleanField* field = NULL;
	Status        result;

	try
	{
		field = new BooleanField(fieldName, value == GMSEC_BOOL::GMSEC_TRUE, isHeader == GMSEC_BOOL::GMSEC_TRUE);
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


GMSEC_BOOL CALL_TYPE booleanFieldGetValue(GMSEC_Field field, GMSEC_Status status)
{
	Field*        f         = reinterpret_cast<Field*>(field);
	BooleanField* boolField = dynamic_cast<BooleanField*>(f);
	GMSEC_BOOL    value     = GMSEC_BOOL::GMSEC_FALSE;
	Status        result;

	if (boolField)
	{
		value = boolField->getValue() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE;
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not a BooleanField");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}
