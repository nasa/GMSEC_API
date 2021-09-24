
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @class Field.cpp
 *
 *  This file contains the Field class.
*/

#include <gmsec/Field.h>
#include <gmsec/internal/BaseField.h>
#include <gmsec/internal/strutil.h>

using namespace gmsec;
using namespace gmsec::internal;
using namespace gmsec::util;


Field::Field()
		:
		ptr(new BaseField())
{
}


Field::Field(BaseField *internal)
		:
		ptr(internal)
{
}


Field::Field(const char *name, GMSEC_CHAR value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value);
}


Field::Field(const char *name, GMSEC_I8 value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value);
}


Field::Field(const char *name, GMSEC_U8 value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value);
}


Field::Field(const char *name, GMSEC_BOOL value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value);
}


Field::Field(const char *name, GMSEC_I16 value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value);
}


Field::Field(const char *name, GMSEC_U16 value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value);
}


Field::Field(const char *name, GMSEC_I32 value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value);
}


Field::Field(const char *name, GMSEC_U32 value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value);
}


Field::Field(const char *name, GMSEC_U64 value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value);
}


Field::Field(const char *name, GMSEC_I64 value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value);
}


Field::Field(const char *name, GMSEC_F32 value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value);
}


Field::Field(const char *name, GMSEC_F64 value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value);
}


Field::Field(const char *name, GMSEC_STR value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value);
}


Field::Field(const char *name, GMSEC_BLOB value, GMSEC_U32 size)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value, size);
}


Field::Field(const char *name, GMSEC_STRING value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value);
}


Field::Field(const char *name, GMSEC_BIN value, GMSEC_U32 size)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(value, size);
}


Field::Field(const char *name, GMSEC_TYPE ftype, const char *value)
		:
		ptr(new BaseField())
{
	ptr->SetName(name);
	ptr->SetValue(ftype, value);
}


Field::Field(Field &field)
		:
		ptr(new BaseField(*field.ptr))
{
}


Field::~Field()
{
	delete ptr;
	ptr = 0;
}


Field &Field::operator=(Field &field)
{
	if (this == &field)
		return *this;
	*ptr = *field.ptr;
	return (*this);
}


Status Field::GetName(const char *&name)
{
	return ptr->GetName(name);
}

Status Field::SetName(const char *name)
{
	return ptr->SetName(name);
}


Status Field::SetType(GMSEC_TYPE ftype)
{
	return ptr->SetType(ftype);
}


Status Field::GetType(GMSEC_TYPE &ftype)
{
	return ptr->GetType(ftype);
}


// This function has been deprecated; remove for API 4.0
bool Field::isHeader() const
{
	return IsHeader();
}

bool Field::IsHeader() const
{
	return ptr->isHeader();
}


// This function has been deprecated; remove for API 4.0
void Field::setHeader(bool isHeader)
{
	SetHeader(isHeader);
}


void Field::SetHeader(bool isHeader)
{
	ptr->setHeader(isHeader);
}


/*! get functions for each type */
Status Field::GetValue(GMSEC_CHAR &value)
{
	return ptr->GetValue(value);
}


Status Field::GetValue(GMSEC_I8 &value)
{
	return ptr->GetValue(value);
}


Status Field::GetValue(GMSEC_U8 &value)
{
	return ptr->GetValue(value);
}


Status Field::GetValue(GMSEC_BOOL &value)
{
	return ptr->GetValue(value);
}


Status Field::GetValue(GMSEC_I16 &value)
{
	return ptr->GetValue(value);
}


Status Field::GetValue(GMSEC_U16 &value)
{
	return ptr->GetValue(value);
}


Status Field::GetValue(GMSEC_I32 &value)
{
	return ptr->GetValue(value);
}


Status Field::GetValue(GMSEC_U32 &value)
{
	return ptr->GetValue(value);
}


Status Field::GetValue(GMSEC_F32 &value)
{
	return ptr->GetValue(value);
}


Status Field::GetValue(GMSEC_F64 &value)
{
	return ptr->GetValue(value);
}


Status Field::GetValue(GMSEC_STR &value)
{
	return ptr->GetValue(value);
}


Status Field::GetValue(GMSEC_BLOB &value, GMSEC_U32 &size)
{
	return ptr->GetValue(value, size);
}


Status Field::GetValue(GMSEC_STRING &value)
{
	GMSEC_STR tmp = 0;
	Status result = ptr->GetValue(tmp);
	value = (GMSEC_STRING) tmp;
	return result;
}


Status Field::GetValue(GMSEC_BIN *value, GMSEC_U32 &size)
{
	return ptr->GetValue(value, size);
}

Status Field::GetValue(GMSEC_I64 &value)
{
	return ptr->GetValue(value);
}


Status Field::GetValue(GMSEC_U64 &value)
{
	return ptr->GetValue(value);
}


Status Field::UnSet()
{
	return ptr->UnSet();
}


/*! set functions for each type  */
Status Field::SetValue(GMSEC_CHAR value)
{
	return ptr->SetValue(value);
}


Status Field::SetValue(GMSEC_I8 value)
{
	return ptr->SetValue(value);
}


Status Field::SetValue(GMSEC_U8 value)
{
	return ptr->SetValue(value);
}


Status Field::SetValue(GMSEC_BOOL value)
{
	return ptr->SetValue(value);
}


Status Field::SetValue(GMSEC_I16 value)
{
	return ptr->SetValue(value);
}


Status Field::SetValue(GMSEC_U16 value)
{
	return ptr->SetValue(value);
}


Status Field::SetValue(GMSEC_I32 value)
{
	return ptr->SetValue(value);
}


Status Field::SetValue(GMSEC_U32 value)
{
	return ptr->SetValue(value);
}


Status Field::SetValue(GMSEC_F32 value)
{
	return ptr->SetValue(value);
}


Status Field::SetValue(GMSEC_F64 value)
{
	return ptr->SetValue(value);
}


Status Field::SetValue(GMSEC_STR value)
{
	return ptr->SetValue(value);
}


Status Field::SetValue(GMSEC_BLOB value, GMSEC_U32 size)
{
	return ptr->SetValue(value, size);
}


Status Field::SetValue(GMSEC_STRING value)
{
	GMSEC_STR tmp = value;
	return ptr->SetValue(tmp);
}


Status Field::SetValue(GMSEC_BIN value, GMSEC_U32 size)
{
	GMSEC_BLOB tmp = value;
	return ptr->SetValue(tmp, size);
}


Status Field::SetValue(GMSEC_I64 value)
{
	return ptr->SetValue(value);
}


Status Field::SetValue(GMSEC_U64 value)
{
	return ptr->SetValue(value);
}


Status Field::SetValue(GMSEC_TYPE ftype, const char *value)
{
	return ptr->SetValue(ftype, value);
}


Status Field::ToXML(const char *&out)
{
	return ptr->ToXML(out);
}


Status Field::FromXML(const char* xml)
{
	return ptr->FromXML(xml);
}


const char* Field::ToJSON()
{
	return ptr->ToJSON();
}


bool Field::operator== (Field &b)
{
	return *ptr == *b.ptr;
}


bool Field::operator< (Field &b)
{
	return *ptr < *b.ptr;
}


bool Field::operator!= (Field &b)
{
	return !operator==(b);
}


bool Field::operator> (Field &b)
{
	return !operator<=(b);
}


bool Field::operator>= (Field &b)
{
	return !operator<(b);
}


bool Field::operator<= (Field &b)
{
	return operator<(b) || operator==(b);
}


GMSEC_TYPE Field::LookupType(const char *ftype)
{
	GMSEC_TYPE type = GMSEC_TYPE_UNSET;

	if (!ftype)
		return GMSEC_TYPE_UNSET;

	switch (ftype[0])
	{
	case 'B':
		if (stringEquals(ftype, "BOOL"))
			type = GMSEC_TYPE_BOOL;
		else if (stringEquals(ftype, "BIN"))
			type = GMSEC_TYPE_BLOB;
		/* alternate spelling used in Java binding */
		else if (stringEquals(ftype, "BOOLEAN"))
			type = GMSEC_TYPE_BOOL;
		break;

	case 'C':
		if (stringEquals(ftype, "CHAR"))
			type = GMSEC_TYPE_CHAR;
		break;

	case 'D':
		if (stringEquals(ftype, "DOUBLE"))
			type = GMSEC_TYPE_DOUBLE;
		break;

	case 'F':
		if (stringEquals(ftype, "F32"))
			type = GMSEC_TYPE_F32;
		else if (stringEquals(ftype, "F64"))
			type = GMSEC_TYPE_F64;
		else if (stringEquals(ftype, "FLOAT"))
			type = GMSEC_TYPE_FLOAT;
		break;

	case 'I':
		if (stringEquals(ftype, "I32"))
			type = GMSEC_TYPE_I32;
		else if (stringEquals(ftype, "I64"))
			type = GMSEC_TYPE_I64;
		else if (stringEquals(ftype, "I16"))
			type = GMSEC_TYPE_I16;
		else if (stringEquals(ftype, "I8"))
			type = GMSEC_TYPE_I8;
		break;

	case 'L':
		if (stringEquals(ftype, "LONG"))
			type = GMSEC_TYPE_LONG;
		break;

	case 'S':
		if (stringEquals(ftype, "STRING"))
			type = GMSEC_TYPE_STRING;
		else if (stringEquals(ftype, "SHORT"))
			type = GMSEC_TYPE_SHORT;
		break;

	case 'U':
		if (stringEquals(ftype, "U32"))
			type = GMSEC_TYPE_U32;
#ifdef GMSEC_FIELD_TYPE_U64
		else if (stringEquals(ftype, "U64"))
			type = GMSEC_TYPE_U64;
#endif /* GMSEC_FIELD_TYPE_U64 */
		else if (stringEquals(ftype, "U16"))
			type = GMSEC_TYPE_U16;
		else if (stringEquals(ftype, "ULONG"))
			type = GMSEC_TYPE_ULONG;
		else if (stringEquals(ftype, "USHORT"))
			type = GMSEC_TYPE_USHORT;
		else if (stringEquals(ftype, "U8"))
			type = GMSEC_TYPE_U8;
		break;
	}

	return type;
}

