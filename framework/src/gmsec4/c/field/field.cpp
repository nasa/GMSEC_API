/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file field.cpp
 */


#include <gmsec4/c/field/field.h>

#include <gmsec4/field/Field.h>

#include <gmsec4/Status.h>

using namespace gmsec::api;


const char* CALL_TYPE fieldGetName(const GMSEC_Field field, GMSEC_Status status)
{
	const Field* fld  = reinterpret_cast<const Field*>(field);
	const char*  name = 0;
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


void CALL_TYPE fieldSetName(GMSEC_Field field, const char* name, GMSEC_Status status)
{
	Field*  fld  = reinterpret_cast<Field*>(field);
	Status  result;

	if (fld)
	{
		try
		{
			fld->setName(name);
		}
		catch (const Exception& e)
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
}


GMSEC_FieldType CALL_TYPE fieldGetType(const GMSEC_Field field, GMSEC_Status status)
{
	const Field*    fld  = reinterpret_cast<const Field*>(field);
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


const char* CALL_TYPE fieldToXML(const GMSEC_Field field, GMSEC_Status status)
{
	const Field* fld = reinterpret_cast<const Field*>(field);
	const char*  xml = 0;
	Status       result;

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


const char* CALL_TYPE fieldToJSON(const GMSEC_Field field, GMSEC_Status status)
{
	const Field* fld  = reinterpret_cast<const Field*>(field);
	const char*  json = 0;
	Status       result;

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


GMSEC_I64 CALL_TYPE fieldGetIntegerValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field* fld   = reinterpret_cast<const Field*>(field);
	GMSEC_I64    value = 0;
	Status       result;

	if (fld)
	{
		try
		{
			value = fld->getIntegerValue();
		}
		catch (Exception& e)
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


GMSEC_U64 CALL_TYPE fieldGetUnsignedIntegerValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field* fld   = reinterpret_cast<const Field*>(field);
	GMSEC_U64    value = 0;
	Status       result;

	if (fld)
	{
		try
		{
			value = fld->getUnsignedIntegerValue();
		}
		catch (Exception& e)
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


GMSEC_F64 CALL_TYPE fieldGetDoubleValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field* fld   = reinterpret_cast<const Field*>(field);
	GMSEC_F64    value = 0;
	Status       result;

	if (fld)
	{
		try
		{
			value = fld->getDoubleValue();
		}
		catch (Exception& e)
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


const char* CALL_TYPE fieldGetStringValue(const GMSEC_Field field, GMSEC_Status status)
{
	const Field* fld   = reinterpret_cast<const Field*>(field);
	const char*  value = 0;
	Status       result;

	if (fld)
	{
		try
		{
			value = fld->getStringValue();
		}
		catch (Exception& e)
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


GMSEC_Field CALL_TYPE fieldClone(const GMSEC_Field field, GMSEC_Status status)
{
	GMSEC_Field  clone = 0;
	const Field* fld   = reinterpret_cast<const Field*>(field);
	Status       result;

	if (fld)
	{
		clone = reinterpret_cast<GMSEC_Field>(fld->clone());
	}
	else
	{
		result = Status(FIELD_ERROR, UNINITIALIZED_OBJECT, "Field object is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return clone;
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
