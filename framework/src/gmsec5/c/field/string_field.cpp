/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file string_field.cpp
 *
 *  @brief This file contains functions for the management of a StringField object.
 */


#include <gmsec5/c/field/string_field.h>

#include <gmsec5/field/StringField.h>

#include <gmsec5/GmsecException.h>
#include <gmsec5/Status.h>


using namespace gmsec::api5;


GMSEC_Field CALL_TYPE stringFieldCreate(const char* fieldName, const char* value, GMSEC_BOOL isHeader, GMSEC_Status status)
{
	StringField* field = NULL;
	Status       result;

	try
	{
		field = new StringField(fieldName, value, isHeader == GMSEC_BOOL::GMSEC_TRUE);
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


const char* CALL_TYPE stringFieldGetValue(GMSEC_Field field, GMSEC_Status status)
{
	Field*       f           = reinterpret_cast<Field*>(field);
	StringField* stringField = dynamic_cast<StringField*>(f);
	const char*  value       = 0;
	Status       result;

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
