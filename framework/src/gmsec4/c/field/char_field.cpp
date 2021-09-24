/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file char_field.cpp
 *
 *  @brief This file contains functions for the management of a CharField object.
 */


#include <gmsec4/c/field/char_field.h>

#include <gmsec4/field/CharField.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;


GMSEC_Field CALL_TYPE charFieldCreate(const char* fieldName, GMSEC_CHAR value, GMSEC_Status status)
{
	CharField* field = NULL;
	Status     result;

	try
	{
		field = new CharField(fieldName, value);
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


GMSEC_CHAR CALL_TYPE charFieldGetValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field*     f         = reinterpret_cast<const Field*>(field);
	const CharField* charField = dynamic_cast<const CharField*>(f);
	GMSEC_CHAR       value     = 0;
	Status           result;

	if (charField)
	{
		value = charField->getValue();
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not a CharField");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}
