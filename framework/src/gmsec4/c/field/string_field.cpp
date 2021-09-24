/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file string_field.cpp
 *
 *  @brief This file contains functions for the management of a StringField object.
 */


#include <gmsec4/c/field/string_field.h>

#include <gmsec4/field/StringField.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;


GMSEC_Field CALL_TYPE stringFieldCreate(const char* fieldName, const char* value, GMSEC_Status status)
{
	StringField* field = NULL;
	Status       result;

	try
	{
		field = new StringField(fieldName, value);
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


const char* CALL_TYPE stringFieldGetValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field*       f           = reinterpret_cast<const Field*>(field);
	const StringField* stringField = dynamic_cast<const StringField*>(f);
	const char*        value       = 0;
	Status             result;

	if (stringField)
	{
		value = stringField->getValue();
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not a StringField");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}
