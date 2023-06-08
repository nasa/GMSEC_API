/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file field.cpp
 */


#include <gmsec5/c/field/field.h>

#include <gmsec5/field/Field.h>

#include <gmsec5/GmsecException.h>
#include <gmsec5/Status.h>

using namespace gmsec::api5;


const char* CALL_TYPE fieldGetName(GMSEC_Field field, GMSEC_Status status)
{
	Field*      fld  = reinterpret_cast<Field*>(field);
	const char* name = 0;
	Status       result;

	if (fld)
	{
		name = fld->getName();
	}
	else
	{
		result = Status(FIELD_ERROR, UNINITIALIZED_OBJECT, "Field object is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return name;
}


GMSEC_FieldType CALL_TYPE fieldGetType(GMSEC_Field field, GMSEC_Status status)
{
	Field*          fld  = reinterpret_cast<Field*>(field);
	GMSEC_FieldType type = GMSEC_BAD_TYPE;
	Status          result;

	if (fld)
	{
		int tmp = static_cast<int>(fld->getType());   // intermediate cast required by solaris
		type = static_cast<GMSEC_FieldType>(tmp);
	}
	else
	{
		result = Status(FIELD_ERROR, UNINITIALIZED_OBJECT, "Field object is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return type;
}


GMSEC_BOOL CALL_TYPE fieldIsHeader(GMSEC_Field field, GMSEC_Status status)
{
	Field*     fld  = reinterpret_cast<Field*>(field);
	GMSEC_BOOL isHeader = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	if (fld)
	{
		isHeader = (fld->isHeader() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}
	else
	{
		result = Status(FIELD_ERROR, UNINITIALIZED_OBJECT, "Field object is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return isHeader;
}


const char* CALL_TYPE fieldToXML(GMSEC_Field field, GMSEC_Status status)
{
	Field*      fld = reinterpret_cast<Field*>(field);
	const char* xml = 0;
	Status      result;

	if (fld)
	{
		xml = fld->toXML();
	}
	else
	{
		result = Status(FIELD_ERROR, UNINITIALIZED_OBJECT, "Field object is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return xml;
}


const char* CALL_TYPE fieldToJSON(GMSEC_Field field, GMSEC_Status status)
{
	Field*      fld  = reinterpret_cast<Field*>(field);
	const char* json = 0;
	Status      result;

	if (fld)
	{
		json = fld->toJSON();
	}
	else
	{
		result = Status(FIELD_ERROR, UNINITIALIZED_OBJECT, "Field object is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return json;
}


const char* CALL_TYPE fieldGetStringValue(GMSEC_Field field, GMSEC_Status status)
{
	Field*      fld   = reinterpret_cast<Field*>(field);
	const char* value = 0;
	Status      result;

	if (fld)
	{
		value = fld->getStringValue();
	}
	else
	{
		result = Status(FIELD_ERROR, UNINITIALIZED_OBJECT, "Field object is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


GMSEC_BOOL CALL_TYPE fieldGetBooleanValue(GMSEC_Field field, GMSEC_Status status)
{
	Field*     fld   = reinterpret_cast<Field*>(field);
	GMSEC_BOOL value = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	if (fld)
	{
		try
		{
			value = (fld->getBooleanValue() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(FIELD_ERROR, UNINITIALIZED_OBJECT, "Field object is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


GMSEC_I16 CALL_TYPE fieldGetI16Value(GMSEC_Field field, GMSEC_Status status)
{
	Field*    fld   = reinterpret_cast<Field*>(field);
	GMSEC_I16 value = 0;
	Status    result;

	if (fld)
	{
		try
		{
			value = fld->getI16Value();
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(FIELD_ERROR, UNINITIALIZED_OBJECT, "Field object is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


GMSEC_I32 CALL_TYPE fieldGetI32Value(GMSEC_Field field, GMSEC_Status status)
{
	Field*    fld   = reinterpret_cast<Field*>(field);
	GMSEC_I32 value = 0;
	Status    result;

	if (fld)
	{
		try
		{
			value = fld->getI32Value();
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(FIELD_ERROR, UNINITIALIZED_OBJECT, "Field object is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


GMSEC_U16 CALL_TYPE fieldGetU16Value(GMSEC_Field field, GMSEC_Status status)
{
	Field*    fld   = reinterpret_cast<Field*>(field);
	GMSEC_U16 value = 0;
	Status    result;

	if (fld)
	{
		try
		{
			value = fld->getU16Value();
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(FIELD_ERROR, UNINITIALIZED_OBJECT, "Field object is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


GMSEC_U32 CALL_TYPE fieldGetU32Value(GMSEC_Field field, GMSEC_Status status)
{
	Field*    fld   = reinterpret_cast<Field*>(field);
	GMSEC_U32 value = 0;
	Status    result;

	if (fld)
	{
		try
		{
			value = fld->getU32Value();
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(FIELD_ERROR, UNINITIALIZED_OBJECT, "Field object is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


GMSEC_F64 CALL_TYPE fieldGetF64Value(GMSEC_Field field, GMSEC_Status status)
{
	Field*    fld   = reinterpret_cast<Field*>(field);
	GMSEC_F64 value = 0;
	Status    result;

	if (fld)
	{
		try
		{
			value = fld->getF64Value();
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(FIELD_ERROR, UNINITIALIZED_OBJECT, "Field object is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


void CALL_TYPE fieldDestroy(GMSEC_Field* field)
{
	if (field)
	{
		Field* f = reinterpret_cast<Field*>(*field);
		delete f;
		*field = NULL;
	}
}
