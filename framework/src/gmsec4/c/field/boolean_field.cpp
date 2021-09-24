/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file boolean_field.cpp
 *
 *  @brief This file contains functions for the management of a BooleanField object.
 */


#include <gmsec4/c/field/boolean_field.h>

#include <gmsec4/field/BooleanField.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;


GMSEC_Field CALL_TYPE booleanFieldCreate(const char* fieldName, GMSEC_BOOL value, GMSEC_Status status)
{
	BooleanField* field = NULL;
	Status        result;

	try
	{
		field = new BooleanField(fieldName, value == GMSEC_TRUE);
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


GMSEC_BOOL CALL_TYPE booleanFieldGetValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field*        f         = reinterpret_cast<const Field*>(field);
	const BooleanField* boolField = dynamic_cast<const BooleanField*>(f);
	GMSEC_BOOL          value     = GMSEC_FALSE;
	Status              result;

	if (boolField)
	{
		value = boolField->getValue() ? GMSEC_TRUE : GMSEC_FALSE;
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
