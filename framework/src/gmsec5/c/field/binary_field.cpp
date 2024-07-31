/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file binary_field.cpp
 *
 *  @brief This file contains functions for the management of a BinaryField object.
 */


#include <gmsec5/c/field/binary_field.h>

#include <gmsec5/field/BinaryField.h>

#include <gmsec5/GmsecException.h>
#include <gmsec5/Status.h>


using namespace gmsec::api5;


GMSEC_Field CALL_TYPE binaryFieldCreate(const char* fieldName, const GMSEC_U8* data, size_t dataLength, GMSEC_BOOL isHeader, GMSEC_Status status)
{
	BinaryField* field = NULL;
	Status       result;

	try
	{
		field = new BinaryField(fieldName, data, dataLength, isHeader == GMSEC_BOOL::GMSEC_TRUE);
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


GMSEC_U8 const* CALL_TYPE binaryFieldGetValue(GMSEC_Field field, GMSEC_Status status)
{
	Field*          f        = reinterpret_cast<Field*>(field);
	BinaryField*    binField = dynamic_cast<BinaryField*>(f);
	GMSEC_U8 const* data     = NULL;
	Status          result;

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


size_t CALL_TYPE binaryFieldGetLength(GMSEC_Field field, GMSEC_Status status)
{
	Field*       f        = reinterpret_cast<Field*>(field);
	BinaryField* binField = dynamic_cast<BinaryField*>(f);
	size_t       dataLen  = 0;
	Status       result;

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
