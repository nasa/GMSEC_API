/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file binary_field.cpp
 *
 *  @brief This file contains functions for the management of a BinaryField object.
 */


#include <gmsec4/c/field/binary_field.h>

#include <gmsec4/field/BinaryField.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;


GMSEC_Field CALL_TYPE binaryFieldCreate(const char* fieldName, const GMSEC_BIN data, size_t dataLength, GMSEC_Status status)
{
	BinaryField* field = NULL;
	Status       result;

	try
	{
		field = new BinaryField(fieldName, data, dataLength);
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


const GMSEC_BIN CALL_TYPE binaryFieldGetValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field*       f        = reinterpret_cast<const Field*>(field);
	const BinaryField* binField = dynamic_cast<const BinaryField*>(f);
	GMSEC_BIN          data     = NULL;
	Status             result;

	if (binField)
	{
		data = binField->getValue();
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not a BinaryField");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return data;
}


size_t CALL_TYPE binaryFieldGetLength(const GMSEC_Field field, GMSEC_Status status)
{
	const Field*       f        = reinterpret_cast<const Field*>(field);
	const BinaryField* binField = dynamic_cast<const BinaryField*>(f);
	size_t             dataLen  = 0;
	Status             result;

	if (binField)
	{
		dataLen = binField->getLength();
	}
	else
	{
		result = Status(FIELD_ERROR, INVALID_FIELD, "Field object is not a BinaryField");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return dataLen;
}
