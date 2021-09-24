
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @class BaseField.cpp
 *
 *  This file contains the BaseField class.
*/

#include <gmsec/internal/BaseField.h>
#include <gmsec/Field.h>
#include <gmsec/util/Encoder.h>

#include <gmsec/internal/Log.h>
#include <gmsec/internal/rawbuf.h>
#include <gmsec/internal/mathutil.h>
#include <gmsec/internal/strutil.h>
#include <tinyxml2.h>
#include <json.h>

#include <iostream>
#include <iomanip>

using std::string;
using std::ostream;
using std::setw;
using std::setfill;
using std::hex;
using std::setprecision;

using namespace gmsec::internal;
using namespace gmsec;
using namespace gmsec::util;
using namespace tinyxml2;



BaseField::BaseField()
	:
	external(NULL),
	fName(),
	fXmlCache(NULL),
	fJsonCache(NULL),
	fType(GMSEC_TYPE_UNSET),
	fHeader(false),
	fI8Value(0),
	fBoolValue(GMSEC_FALSE),
	fI16Value(0),
	fU16Value(0),
	fI32Value(0),
	fU32Value(0),
	fI64Value(0),
	fF32Value(0),
	fF64Value(0),
	fStrValue(),
	fBinValue(NULL),
	fBinSize(0)
{
}


BaseField::BaseField(const BaseField &fd)
	:
	external(NULL),
	fName(),
	fXmlCache(NULL),
	fJsonCache(NULL),
	fType(GMSEC_TYPE_UNSET),
	fHeader(false),
	fI8Value(0),
	fBoolValue(GMSEC_FALSE),
	fI16Value(0),
	fU16Value(0),
	fI32Value(0),
	fU32Value(0),
	fI64Value(0),
	fF32Value(0),
	fF64Value(0),
	fStrValue(),
	fBinValue(NULL),
	fBinSize(0)
{
	(*this) = fd;
}


BaseField::~BaseField()
{
	// UnSet cleans up fBinValue, fXmlCache, fJsonCache
	UnSet();
}


void BaseField::Init()
{
	fType = GMSEC_TYPE_UNSET;
	fBinValue = NULL;
	fBinSize = 0;
	fXmlCache = NULL;
	fJsonCache = NULL;
}


Status BaseField::UnSet()
{
	Status result;

	SetType(GMSEC_TYPE_UNSET);
	setHeader(false);

	if (NULL != fBinValue)
	{
		GMSEC_BIN tmp = (GMSEC_BIN) fBinValue;
		delete [] tmp;
		fBinValue = NULL;
	}

	if (NULL != fXmlCache)
	{
		delete [] fXmlCache;
		fXmlCache = NULL;
	}

	if (NULL != fJsonCache)
	{
		delete [] fJsonCache;
		fJsonCache = NULL;
	}

	return result;
}


BaseField &BaseField::operator=(const BaseField &fd)
{
	if (this == &fd)
		return *this;

	UnSet();

	const char *name = 0;
	fd.GetName(name);
	SetName(name);

	//value
	GMSEC_TYPE ftype = GMSEC_TYPE_UNSET;
	fd.GetType(ftype);
	switch (ftype)
	{
	case GMSEC_TYPE_CHAR:
	{
		GMSEC_CHAR c;
		fd.GetValue(c);
		SetValue(c);
	}
	break;
	case GMSEC_TYPE_I8:
	{
		GMSEC_I8 u;
		fd.GetValue(u);
		SetValue(u);
	}
	break;
	case GMSEC_TYPE_U8:
	{
		GMSEC_U8 u;
		fd.GetValue(u);
		SetValue(u);
	}
	break;
	case GMSEC_TYPE_BOOL:
	{
		GMSEC_BOOL b;
		fd.GetValue(b);
		SetValue(b);
	}
	break;
	case GMSEC_TYPE_I16:
	{
		GMSEC_I16 s;
		fd.GetValue(s);
		SetValue(s);
	}
	break;
	case GMSEC_TYPE_U16:
	{
		GMSEC_U16 u;
		fd.GetValue(u);
		SetValue(u);
	}
	break;
	case GMSEC_TYPE_I32:
	{
		GMSEC_I32 l;
		fd.GetValue(l);
		SetValue(l);
	}
	break;
	case GMSEC_TYPE_U32:
	{
		GMSEC_U32 u;
		fd.GetValue(u);
		SetValue(u);
	}
	break;
	case GMSEC_TYPE_F32:
	{
		GMSEC_F32 f;
		fd.GetValue(f);
		SetValue(f);
	}
	break;
	case GMSEC_TYPE_F64:
	{
		GMSEC_F64 d;
		fd.GetValue(d);
		SetValue(d);
	}
	break;
	case GMSEC_TYPE_STRING:
	{
		fType     = GMSEC_TYPE_STRING;
		fStrValue = fd.fStrValue;
	}
	break;
	case GMSEC_TYPE_BLOB:
	{
		GMSEC_BLOB b;
		GMSEC_U32 len;
		fd.GetValue(b,len);
		SetValue(b,len);
	}
	break;
	case GMSEC_TYPE_I64:
	{
		GMSEC_I64 l;
		fd.GetValue(l);
		SetValue(l);
	}
	break;
	case GMSEC_TYPE_U64:
	{
		GMSEC_U64 u;
		fd.GetValue(u);
		SetValue(u);
	}
	break;
	}

	/* need to setHeader after SetValue since SetValue calls UnSet which
	clears header flag */
	setHeader(fd.isHeader());

	return (*this);
}


Status BaseField::GetName(const char *&name) const
{
	Status result;
	name = fName.c_str();
	return result;
}


Status BaseField::SetName(const char *name)
{
	Status result;

	if (name)
	{
		fName = name;
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_INVALID_FIELD_NAME,
		           "Field name is invalid.");
                LOG_WARNING << "BaseField::SetName : " << result.Get();
	}

	return result;
}


Status BaseField::SetType(GMSEC_TYPE ftype)
{
	Status result;
	fType = ftype;
	return result;
}


Status BaseField::GetType(GMSEC_TYPE &ftype) const
{
	Status result;
	ftype = fType;
	return result;
}


bool BaseField::isHeader() const
{
	return fHeader;
}



void BaseField::setHeader(bool flag)
{
	fHeader = flag;
}


/*! get functions for each type */
Status BaseField::GetValue(GMSEC_CHAR &value) const
{
	Status result;
	if (fType == GMSEC_TYPE_CHAR)
	{
		value = (GMSEC_CHAR) fI8Value;
		/* compatible types */
	}
	else if (fType == GMSEC_TYPE_BOOL)
	{
		value = fBoolValue;
                LOG_WARNING << "BaseField::GetValue_CHAR : Requesting char value instead of Bool value.";
	}
	else if (fType == GMSEC_TYPE_I16)
	{
		value = (GMSEC_CHAR) fI16Value;
                LOG_WARNING << "BaseField::GetValue_CHAR : Requesting char value instead of I16 value.";
	}
	else if (fType == GMSEC_TYPE_U16)
	{
		value = (GMSEC_CHAR) fU16Value;
                LOG_WARNING << "BaseField::GetValue_CHAR : Requesting char value instead of U16 value.";
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Field type mismatch");
                LOG_WARNING << "BaseField::GetValue_CHAR : " << result.Get();
	}
	return result;
}


Status BaseField::GetValue(GMSEC_I8 &value) const
{
	Status result;
	if (fType == GMSEC_TYPE_I8 || fType == GMSEC_TYPE_CHAR)
	{
		value = fI8Value;
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Field type mismatch");
		LOG_WARNING << "BaseField::GetValue_I8 : " << result.Get();
	}
	return result;
}


Status BaseField::GetValue(GMSEC_U8 &value) const
{
	Status result;
	if (fType == GMSEC_TYPE_U8)
	{
		value = fU8Value;
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Field type mismatch");
		LOG_WARNING << "BaseField::GetValue_U8 : " << result.Get();
	}
	return result;
}


Status BaseField::GetValue(GMSEC_BOOL &value) const
{
	Status result;
	if (fType == GMSEC_TYPE_BOOL)
	{
		value = fBoolValue;
		/* compatible types */
	}
	else if (fType == GMSEC_TYPE_CHAR)
	{
		value = (GMSEC_BOOL) fI8Value;
                 LOG_WARNING << "BaseField::GetValue_BOOL : Requesting Bool value instead of char value.";
	}
	else if (fType == GMSEC_TYPE_U8)
	{
		value = (GMSEC_BOOL) fU8Value;
                 LOG_WARNING << "BaseField::GetValue_BOOL : Requesting Bool value instead of U8 value.";
	}
	else if (fType == GMSEC_TYPE_I16)
	{
		value = (GMSEC_BOOL) fI16Value;
                LOG_WARNING << "BaseField::GetValue_BOOL : Requesting Bool value instead of I16 value.";
	}
	else if (fType == GMSEC_TYPE_U16)
	{
		value = (GMSEC_BOOL) fU16Value;
                LOG_WARNING << "BaseField::GetValue_BOOL : Requesting Bool value instead of U16 value.";
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Field type mismatch");
                LOG_WARNING << "BaseField::GetValue_BOOL : " << result.Get();
	}
	return result;
}


Status BaseField::GetValue(GMSEC_I16 &value) const
{
	Status result;
	if (fType == GMSEC_TYPE_I16)
	{
		value = fI16Value;
		/* compatible types */
	}
	else if (fType == GMSEC_TYPE_BOOL)
	{
		value = fBoolValue;
                LOG_WARNING << "BaseField::GetValue_I16 : Requesting I16 value instead of Bool value.";
	}
	else if (fType == GMSEC_TYPE_I8)
	{
		value = fI8Value;
                LOG_WARNING << "BaseField::GetValue_I16 : Requesting I16 value instead of I8 value.";
	}
	else if (fType == GMSEC_TYPE_U16)
	{
		value = fU16Value;
                LOG_WARNING << "BaseField::GetValue_I16 : Requesting I16 value instead of U16 value.";
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Field type mismatch");
                LOG_WARNING << "BaseField::GetValue_I16 : " << result.Get();
	}
	return result;
}


Status BaseField::GetValue(GMSEC_U16 &value) const
{
	Status result;
	if (fType == GMSEC_TYPE_U16)
	{
		value = fU16Value;
		/* compatible types */
	}
	else if (fType == GMSEC_TYPE_I16)
	{
		value = fI16Value;
                LOG_WARNING << "BaseField::GetValue_U16 : Requesting U16 value instead of I16 value.";
	}
	else if (fType == GMSEC_TYPE_BOOL)
	{
		value = fBoolValue;
                LOG_WARNING << "BaseField::GetValue_U16 : Requesting U16 value instead of Bool value.";
	}
	else if (fType == GMSEC_TYPE_I8)
	{
		value = fI8Value;
                LOG_WARNING << "BaseField::GetValue_U16 : Requesting U16 value instead of I8 value.";
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Field type mismatch");
                LOG_WARNING << "BaseField::GetValue_U16 : " << result.Get();
	}
	return result;
}


Status BaseField::GetValue(GMSEC_I32 &value) const
{
	Status result;
	if (fType == GMSEC_TYPE_I32)
	{
		value = fI32Value;
		/* compatible types */
	}
	else if (fType == GMSEC_TYPE_U32)
	{
		value = fU32Value;
                LOG_WARNING << "BaseField::GetValue_I32 : Requesting I32 value instead of U32 value.";
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Field type mismatch");
                LOG_WARNING << "BaseField::GetValue_I32 : " << result.Get();
	}
	return result;
}


Status BaseField::GetValue(GMSEC_U32 &value) const
{
	Status result;
	if (fType == GMSEC_TYPE_U32)
	{
		value = fU32Value;
		/* compatible types */
	}
	else if (fType == GMSEC_TYPE_I32)
	{
		value = fI32Value;
                LOG_WARNING << "BaseField::GetValue_U32 : Requesting U32 value instead of I32 value.";
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Field type mismatch");
                LOG_WARNING << "BaseField::GetValue_U32 : " << result.Get();
	}
	return result;
}


Status BaseField::GetValue(GMSEC_F32 &value) const
{
	Status result;
	if (fType == GMSEC_TYPE_F32)
	{
		value = fF32Value;
		/* compatible types */
	}
	else if (fType == GMSEC_TYPE_F64)
	{
		value = (GMSEC_F32) fF64Value;
                LOG_WARNING << "BaseField::GetValue_F32 : Requesting F32 value instead of F64 value.";
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Field type mismatch");
                LOG_WARNING << "BaseField::GetValue_F32 : " << result.Get();
	}
	return result;
}


Status BaseField::GetValue(GMSEC_F64 &value) const
{
	Status result;
	if (fType == GMSEC_TYPE_F64)
	{
		value = fF64Value;
		/* compatible types */
	}
	else if (fType == GMSEC_TYPE_F32)
	{
		value = fF32Value;
                LOG_WARNING << "BaseField::GetValue_F64 : Requesting F64 value instead of F32 value.";
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Field type mismatch");
                LOG_WARNING << "BaseField::GetValue_F64 : " << result.Get();
	}
	return result;
}


Status BaseField::GetValue(GMSEC_STR &value) const
{
	Status result;
	if (fType == GMSEC_TYPE_STRING)
	{
		value = fStrValue.c_str();
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Field type mismatch");
                LOG_WARNING << "BaseField::GetValue_STRING : " << result.Get();
	}
	return result;
}


Status BaseField::GetValue(GMSEC_BLOB &value, GMSEC_U32 &size) const
{
	Status result;
	if (fType == GMSEC_TYPE_BLOB)
	{
		value = 0;
		size = 0;
		if (fBinValue != NULL)
		{
			size = fBinSize;
			value = fBinValue;
		}
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Field type mismatch");
                LOG_WARNING << "BaseField::GetValue_BIN : " << result.Get();
	}
	return result;
}


Status BaseField::GetValue(GMSEC_BIN *value, GMSEC_U32 &size) const
{
	Status result;
	if (value == NULL)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Pointer is NULL.");
                LOG_WARNING << "BaseField::GetValue_BIN : " << result.Get();
	}
	else
	{
		GMSEC_BLOB blob = 0;
		result = GetValue(blob, size);
		*value = (GMSEC_BIN) blob;
	}
	return result;
}


Status BaseField::GetValue(GMSEC_I64 &value) const
{
	Status result;
	if (fType == GMSEC_TYPE_I64)
	{
		value = fI64Value;
		/* compatible types */
	}
	else if (fType == GMSEC_TYPE_U64)
	{
		value = (GMSEC_I64) fU64Value;
		LOG_WARNING << "BaseField::GetValue_I64 : Requesting I64 value instead of U64 value.";
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Field type mismatch");
		LOG_WARNING << "BaseField::GetValue_I64 : " << result.Get();
	}
	return result;
}


Status BaseField::GetValue(GMSEC_U64 &value) const
{
	Status result;
	if (fType == GMSEC_TYPE_U64)
	{
		value = fU64Value;
		/* compatible types */
	}
	else if (fType == GMSEC_TYPE_I64)
	{
		value = fI64Value;
		LOG_WARNING << "BaseField::GetValue_U64 : Requesting U64 value instead of I64 value.";
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_FIELD_TYPE_MISMATCH,
		           "Field type mismatch");
		LOG_WARNING << "BaseField::GetValue_U64 : " << result.Get();
	}
	return result;
}


/*! set functions for each type  */
Status BaseField::SetValue(GMSEC_CHAR value)
{
	Status result;

	UnSet();
	SetType(GMSEC_TYPE_CHAR);
	fI8Value = (GMSEC_I8) value;
	/* compatible types */
	fBoolValue = (GMSEC_BOOL) value;
	fI16Value = value;
	fU16Value = value;

	return result;
}


Status BaseField::SetValue(GMSEC_I8 value)
{
	Status result;

	UnSet();
	SetType(GMSEC_TYPE_I8);
	fI8Value = value;
	/* compatible types */
	fU8Value   = (GMSEC_U8) value;
	fI16Value  = (GMSEC_I16) value;
	fBoolValue = (GMSEC_BOOL) value;

	return result;
}


Status BaseField::SetValue(GMSEC_U8 value)
{
	Status result;

	UnSet();
	SetType(GMSEC_TYPE_U8);
	fU8Value = value;
	/* compatible types */
	fI8Value   = (GMSEC_I8) value;
	fI16Value  = (GMSEC_I16) value;
	fBoolValue = (GMSEC_BOOL) value;

	return result;
}


Status BaseField::SetValue(GMSEC_BOOL value)
{
	Status result;

	UnSet();
	SetType(GMSEC_TYPE_BOOL);
	fBoolValue = value;
	/* compatible types */
	fI8Value = (GMSEC_I8) value;
	fI16Value = value;
	fU16Value = value;

	return result;
}


Status BaseField::SetValue(GMSEC_I16 value)
{
	Status result;

	UnSet();
	SetType(GMSEC_TYPE_I16);
	fI16Value = value;
	/* compatible types */
	fBoolValue = (GMSEC_BOOL) value;
	fI8Value = (GMSEC_I8) value;
	fU16Value = value;

	return result;
}


Status BaseField::SetValue(GMSEC_U16 value)
{
	Status result;

	UnSet();
	SetType(GMSEC_TYPE_U16);
	fU16Value = value;
	/* compatible types */
	fI16Value = (GMSEC_I16) value;
	fBoolValue = (GMSEC_BOOL) value;
	fI8Value = (GMSEC_I8) value;

	return result;
}


Status BaseField::SetValue(GMSEC_I32 value)
{
	Status result;

	UnSet();
	SetType(GMSEC_TYPE_I32);
	fI32Value = value;
	/* compatible types */
	fU32Value = value;
	return result;
}


Status BaseField::SetValue(GMSEC_U32 value)
{
	Status result;

	UnSet();
	SetType(GMSEC_TYPE_U32);
	fU32Value = value;
	/* compatible types */
	fI32Value = (GMSEC_I32) value;

	return result;
}


Status BaseField::SetValue(GMSEC_F32 value)
{
	Status result;

	UnSet();
	SetType(GMSEC_TYPE_F32);
	fF32Value = value;
	/* compatible types */
	fF64Value = value;

	return result;
}


Status BaseField::SetValue(GMSEC_F64 value)
{
	Status result;

	UnSet();
	SetType(GMSEC_TYPE_F64);
	fF64Value = value;
	/* compatible types */
	fF32Value = (GMSEC_F32) value;

	return result;
}


Status BaseField::SetValue(GMSEC_STR value)
{
	Status result;

	if (value)
	{
		UnSet();
		SetType(GMSEC_TYPE_STRING);
		fStrValue = value;
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_INVALID_FIELD_VALUE,
		           "String value is NULL.");
		LOG_WARNING << "BaseField::SetValue : " << result.Get();
	}

	return result;
}


Status BaseField::SetValue(GMSEC_BLOB value, GMSEC_U32 size)
{
	Status result;

	UnSet();
	SetType(GMSEC_TYPE_BLOB);
	if (value != NULL)
	{
		GMSEC_BIN tmp = new unsigned char[size];
		copyBytes(tmp, value, size);
		fBinValue = tmp;
		fBinSize = size;
	}
	else if (size == 0)
	{
		fBinValue = value;
		fBinSize = size;
	}
	else
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_INVALID_FIELD_VALUE,
		           "Pointer is NULL.");
                LOG_WARNING << "BaseField::SetValue : " << result.Get();
	}

	return result;
}


Status BaseField::SetValue(GMSEC_I64 value)
{
	Status result;

	UnSet();
	SetType(GMSEC_TYPE_I64);
	fI64Value = value;
	/* compatible types */
	fU64Value = value;
	return result;
}


Status BaseField::SetValue(GMSEC_U64 value)
{
	Status result;

	UnSet();
	SetType(GMSEC_TYPE_U64);
	fU64Value = value;
	/* compatible types */
	fI64Value = (GMSEC_I64) value;

	return result;
}


unsigned char lookupHexDigit(char digit)
{
	switch (digit)
	{
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		return (unsigned char)((digit - 'a') + 10);
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		return (unsigned char)((digit - 'A') + 10);
	default:
		return (unsigned char)(digit - '0');
	}
}


template<class T> void extract(BaseField &field, const char *value, T &x, Status &status)
{
	std::istringstream input(value);
	input >> x;
	if (!input)
		status.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_INVALID_FIELD_VALUE,
		           value);
	else
		field.SetValue(x);
}


// extractF32/extractF64 are used to process the BITS string
static void extractF32(BaseField &field, const char *hexstr, GMSEC_F32 &x, Status &status)
{
	if (stringLength(hexstr) != 8)
	{
		status.Set(GMSEC_STATUS_FIELD_ERROR,
				GMSEC_INVALID_FIELD_VALUE,
				"Field F32 BITS must be 8 hex digits");
		return;
	}

	GMSEC_U8 raw[4];
	for (int i = 0; i < 4; i++)
	{
		int upper = lookupHexDigit(hexstr[i*2]);
		int lower = lookupHexDigit(hexstr[i*2 + 1]);
		if (upper < 0 || lower < 0)
		{
			status.Set(GMSEC_STATUS_FIELD_ERROR,
					GMSEC_INVALID_FIELD_VALUE,
					"Field F32 BITS must be 32 hex digits");
			return;
		}
		raw[i] = upper*16 + lower;
	}

	const char * pi = reinterpret_cast<char *>(&raw[0]);
	Decoder *decoder = Decoder::getDecoder();
	decoder->getF32(pi, &x);

	status = field.SetValue(x);
}


static void extractF64(BaseField &field, const char *hexstr, GMSEC_F64 &x, Status &status)
{
	if (stringLength(hexstr) != 16)
	{
		status.Set(GMSEC_STATUS_FIELD_ERROR,
				GMSEC_INVALID_FIELD_VALUE,
				"Field F64 BITS must be 16 hex digits");
		return;
	}

	GMSEC_U8 raw[8];
	for (int i = 0; i < 8; i++)
	{
		int upper = lookupHexDigit(hexstr[i*2]);
		int lower = lookupHexDigit(hexstr[i*2 + 1]);
		if (upper < 0 || lower < 0)
		{
			status.Set(GMSEC_STATUS_FIELD_ERROR,
					GMSEC_INVALID_FIELD_VALUE,
					"Field F64 BITS must be 16 hex digits");
			return;
		}
		raw[i] = upper*16 + lower;
	}

	const char * pi = reinterpret_cast<char *>(&raw[0]);
	Decoder *decoder = Decoder::getDecoder();
	decoder->getF64(pi, &x);

	status = field.SetValue(x);
}


Status BaseField::SetValue(GMSEC_TYPE ftype, const char *value)
{
	Status result;
	if (value == NULL)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_INVALID_FIELD_VALUE,
		           "value is NULL.");
	}
	else
	{
		bool bits = false;

		if (ftype & GMSEC_FLAG_BITS_VALUE)
		{
			ftype ^= GMSEC_FLAG_BITS_VALUE;
			bits = true;
		}

		switch (ftype)
		{
		case GMSEC_TYPE_CHAR:
			if (stringEquals(value, "nul"))
				SetValue((GMSEC_CHAR) 0);
			else
				SetValue(value[0]);
			break;
		case GMSEC_TYPE_I8:
			SetValue((GMSEC_I8) value[0]);
			break;
		case GMSEC_TYPE_U8:
			SetValue((GMSEC_U8) value[0]);
			break;
		case GMSEC_TYPE_BOOL:
			if (stringIsTrue(value))
				SetValue(GMSEC_TRUE);
			else
				SetValue(GMSEC_FALSE);
			break;
		case GMSEC_TYPE_I16:
			extract(*this, value, fI16Value, result);
			break;
		case GMSEC_TYPE_U16:
			extract<GMSEC_U16>(*this, value, fU16Value, result);
			break;
		case GMSEC_TYPE_I32:
			extract<GMSEC_I32>(*this, value, fI32Value, result);
			break;
		case GMSEC_TYPE_U32:
			extract<GMSEC_U32>(*this, value, fU32Value, result);
			break;
		case GMSEC_TYPE_F32:
			if (bits)
				extractF32(*this, value, fF32Value, result);
			else
			{
				fF32Value = stringRealConstant(value);
				if (fF32Value != gmsec::util::SIMPLE)
					SetValue(fF32Value);
				else
					extract<GMSEC_F32>(*this, value, fF32Value, result);
			}
			break;
		case GMSEC_TYPE_F64:
			if (bits)
				extractF64(*this, value, fF64Value, result);
			else
			{
				fF64Value = stringRealConstant(value);
				if (fF64Value != gmsec::util::SIMPLE)
					SetValue(fF64Value);
				else
					extract<GMSEC_F64>(*this, value, fF64Value, result);
			}
			break;
		case GMSEC_TYPE_STRING:
			SetValue((GMSEC_STR) value);
			break;
		case GMSEC_TYPE_BLOB:
		{
			GMSEC_U32 inlen = (GMSEC_U32) stringLength(value);
			if (inlen == 0)
			{
				SetValue(NULL, inlen);
			}
			else if (inlen % 2 != 0)
			{
				result.Set(GMSEC_STATUS_FIELD_ERROR,
					GMSEC_INVALID_FIELD_VALUE,
					"Invalid base 16 string (odd number of characters).");
			}
			else
			{
				DataBuffer tmp;
				if (convertBase16ToBlob(value, tmp))
					SetValue(tmp.get(), tmp.size());
				else
				{
					result.Set(GMSEC_STATUS_FIELD_ERROR,
						GMSEC_INVALID_FIELD_VALUE,
						"Invalid base 16 string.");
				}
			}
		}
		break;
		case GMSEC_TYPE_I64:
#ifdef GMSEC_VC6
			SetValue(gmsec::util::stringToI64(value));
#else
			extract(*this, value, fI64Value, result);
#endif
			break;
		case GMSEC_TYPE_U64:
#ifdef GMSEC_VC6
			SetValue(gmsec::util::stringToU64(value));
#else
			extract(*this, value, fU64Value, result);
#endif
			break;
		default:
			result.Set(GMSEC_STATUS_FIELD_ERROR,
			           GMSEC_UNKNOWN_FIELD_TYPE,
			           "Invalid field type specified.");
			break;
		}
	}

	if (result.isError())
	{
		LOG_WARNING << "BaseField::SetValue : " << result.Get();
	}

	return result;
}


class Escaper
{
	enum { COUNT = 256 };

	int length[COUNT];
	const char * escaped[COUNT];

private:

	int index(char c)
	{
		return (unsigned char) c;
	}

	void set(char c, const char * s)
	{
		length[index(c)] = (int) stringLength(s);
		escaped[index(c)] = s;
	}

public:
	Escaper()
	{
		for (int i = 0; i < COUNT; ++i)
		{
			length[i] = 1;
			escaped[i] = 0;
		}
		set('&', "&amp;");
		set('<', "&lt;");
		set('>', "&gt;");
		set('\'', "&apos;");
		set('\"', "&quot;");
	}

	void update(char c, size_t &slen, size_t &xmllen)
	{
		++slen;
		xmllen += length[index(c)];
	}

	void put(ostream &os, char c)
	{
		if (escaped[index(c)])
			os << escaped[index(c)];
		else
			os << c;
	}

	void put(std::ostringstream &oss, char c)
	{
		if (escaped[index(c)])
			oss << escaped[index(c)];
		else
			oss << c;
	}

	void study(const char * p, size_t &slen, size_t &xmllen)
	{
		slen = xmllen = 0;

		// done if it is a null pointer
		if (!p)
			return;

		while (*p)
			update(*p++, slen, xmllen);
	}

	void put(ostream &out, const char * p)
	{

		if (!p)
			return;

		while (*p)
			put(out, *p++);
	}

};


static Escaper &getEscaper()
{
	static Escaper escaper;
	return escaper;
}


Status BaseField::ToXML(const char *&out) const
{
	Status result;

	out = 0;

	/* note that this could just return the cached value if the value
		has not changed... */

	const char * type = "UNSET";
	const char * name = "";

	if (!fName.empty())
	{
		name = fName.c_str();
	}

	GMSEC_TYPE ftype = GMSEC_TYPE_UNSET;
	GetType(ftype);

	// first, determine (bound) the size of the XML value
	//	(namelen + typelen + fixlen + vallen)

	size_t namelen = stringLength(name);

	// null terminator plus length of string constants
	const size_t fixlen = 1 + stringLength("<FIELD NAME='' TYPE=''></FIELD>");

	// initialize vallen to handle 32 bit or smaller primitives
	size_t vallen = 12;
	size_t slen = 0;
	bool needsBits = false;

	switch (ftype)
	{
	case GMSEC_TYPE_CHAR:
	{
		type = "CHAR";
		// vallen = strlen("nul");
	}
	break;
	case GMSEC_TYPE_I8:
	{
		type = "I8";
	}
	break;
	case GMSEC_TYPE_U8:
	{
		type = "U8";
	}
	break;
	case GMSEC_TYPE_BOOL:
	{
		type = "BOOL";
		// vallen = strlen("FALSE");
	}
	break;
	case GMSEC_TYPE_I16:
	{
		type = "I16";
		// vallen = strlen("-16000");
	}
	break;
	case GMSEC_TYPE_U16:
	{
		type = "U16";
		// vallen = strlen("32000");
	}
	break;
	case GMSEC_TYPE_I32:
	{
		type = "I32";
		// vallen = strlen("-2000000000");
	}
	break;
	case GMSEC_TYPE_U32:
	{
		type = "U32";
		// vallen = strlen("4000000000");
	}
	break;

	case GMSEC_TYPE_F32:
	{
		type = "F32";
		vallen = 32; // strlen("BITS='8hex'" + ~strlen("sx.ffffffsee");
		needsBits = true;
	}
	break;
	case GMSEC_TYPE_F64:
	{
		type = "F64";
		vallen = 48; // strlen("BITS='16hex'") + ~strlen("sx.fffffffffffffffseee");
		needsBits = true;
	}
	break;
	case GMSEC_TYPE_STRING:
	{
		type = "STRING";
		Escaper &escaper = getEscaper();
		escaper.study(fStrValue.c_str(), slen, vallen);
	}
	break;
	case GMSEC_TYPE_BLOB:
	{
		type = "BIN";
		vallen = 2 * fBinSize;
	}
	break;
	case GMSEC_TYPE_I64:
	{
		type = "I64";
		vallen = 24; // sign + 2 * billions
	}
	break;
	case GMSEC_TYPE_U64:
	{
		type = "U64";
		vallen = 24; // 2 * billions
	}
	break;

	default:
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_UNKNOWN_FIELD_TYPE,
		           "Field has unknown or unset type.");
                LOG_WARNING << "BaseField::ToXML : " << result.Get();
		return result;
	}


	// now build the XML
	size_t typelen = stringLength(type);

	// HEADLEN = stringLength(" HEAD='T'")
	int headlen = isHeader() ? 10 : 0;
	size_t totlen = fixlen + namelen + typelen + headlen + vallen;

	char * p = 0;
	try
	{
		p = new char[totlen];
	}
	catch (std::bad_alloc &)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OUT_OF_MEMORY,
		           "Field::ToXML: new char[] failed");
                LOG_ERROR << "BaseField::ToXML : " << result.Get();
		return result;
	}

	rawbuf buffer(p, totlen, true);
	ostream xmlStream(&buffer);

	xmlStream << "<FIELD TYPE=" << '"' << type << '"'
			<< " NAME=" << '"' << name << '"';

	if (isHeader())
		xmlStream << " HEAD=\"T\"";

	if (!needsBits)
		xmlStream << ">";

	switch (ftype)
	{
	case GMSEC_TYPE_CHAR:
	{
		if (!fI8Value)
			xmlStream << "nul";
		else
			xmlStream << (char) fI8Value;
	}
	break;
	case GMSEC_TYPE_I8:
	{
		xmlStream << (int) fI8Value;
	}
	break;
	case GMSEC_TYPE_U8:
	{
		xmlStream << (unsigned int) fU8Value;
	}
	break;
	case GMSEC_TYPE_BOOL:
	{
		if (fBoolValue)
			xmlStream << "TRUE";
		else
			xmlStream << "FALSE";
	}
	break;
	case GMSEC_TYPE_I16:
	{
		xmlStream << fI16Value;
	}
	break;
	case GMSEC_TYPE_U16:
	{
		xmlStream << fI16Value;
	}
	break;
	case GMSEC_TYPE_I32:
	{
		xmlStream << fI32Value;
	}
	break;
	case GMSEC_TYPE_U32:
	{
		xmlStream << fU32Value;
	}
	break;

	case GMSEC_TYPE_F32:
	{
		xmlStream << " BITS=\"";
		Encoder *encoder = Encoder::getEncoder();

		unsigned char bytes[4];
		char *bytesTmp = reinterpret_cast<char *>(bytes);
		encoder->putF32(&fF32Value, bytesTmp);

		const char *hex = "0123456789ABCDEF";
		for (int i = 0; i < 4; i++)
		{
			xmlStream << hex[bytes[i] / 16];
			xmlStream << hex[bytes[i] % 16];
		}

		xmlStream << "\">";

		const char *p = realStringConstant(fF32Value);
		if (p)
			xmlStream << p;
		else
			xmlStream << fF32Value;
	}
	break;
	case GMSEC_TYPE_F64:
	{
		xmlStream << " BITS=\"";
		Encoder *encoder = Encoder::getEncoder();

		unsigned char bytes[8];
		char *bytesTmp = reinterpret_cast<char *>(bytes);
		encoder->putF64(&fF64Value, bytesTmp);

		const char *hex = "0123456789ABCDEF";
		for (int i = 0; i < 8; i++)
		{
			xmlStream << hex[bytes[i] / 16];
			xmlStream << hex[bytes[i] % 16];
		}

		xmlStream << "\">";

		const char *p = realStringConstant(fF64Value);
		if (p)
			xmlStream << p;
		else
			xmlStream << fF64Value;
	}
	break;
	case GMSEC_TYPE_STRING:
	{
		if (fStrValue.empty())
		{
			// Do nothing
		}
		else if (slen == vallen)
		{
			xmlStream << fStrValue;
		}
		else if (!fStrValue.empty())
		{
			Escaper &escaper = getEscaper();
			escaper.put(xmlStream, fStrValue.c_str());
		}
	}
	break;
	case GMSEC_TYPE_BLOB:
	{
		static const char *hex = "0123456789ABCDEF";
		const GMSEC_U8 * tmp = (GMSEC_U8 *) fBinValue;
		for (size_t i = 0; i < fBinSize; ++i)
		{
			int upper = unsigned(tmp[i]) / 16;
			int lower = unsigned(tmp[i]) % 16;
			xmlStream << hex[upper] << hex[lower];
		}
	}
	break;
	case GMSEC_TYPE_I64:
	{
#ifdef GMSEC_VC6
		char tmp[32];
		gmsec::util::I64toString(fI64Value, tmp, 32);
		xmlStream << tmp;
#else
		xmlStream << fI64Value;
#endif
	}
	break;
	case GMSEC_TYPE_U64:
#ifdef GMSEC_VC6
		char tmp[32];
		gmsec::util::U64toString(fU64Value, tmp);
		xmlStream << tmp;
#else
		xmlStream << fU64Value;
#endif
	break;

	default:
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_UNKNOWN_FIELD_TYPE,
		           "Field has unknown or unset type.");
                LOG_WARNING << "BaseField::ToXML : " << result.Get();
		break;
	}

	xmlStream << "</FIELD>" << std::ends;

	if (!xmlStream)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_UNKNOWN_FIELD_TYPE,
		           "Field::ToXML: bad ostream");
                LOG_WARNING << "BaseField::ToXML : " << result.Get();
	}
	else
	{
		if (fXmlCache)
		{
			delete[] fXmlCache;
		}

		fXmlCache = buffer.release();
		out = fXmlCache;
	}

	return result;
}


Status BaseField::FromXML(tinyxml2::XMLElement *element)
{
	Status result;

	if (element == NULL)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_XML_PARSE_ERROR, "Null XML element");
		LOG_WARNING << "BaseField::FromXML : " << result.Get();
		return result;
	}

	/* verify that our root node is "FIELD" */
	const char *mename = element->Name();
	if (mename == NULL || !stringEquals(mename, "FIELD"))
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_XML_PARSE_ERROR,
				"Invalid xml message format - not a FIELD");
		LOG_WARNING << "BaseField::FromXML : " << result.Get();
		return result;
	}

	const char* ftype = NULL;
	const char* fname = NULL;
	const char* value = NULL;
	const char* fhead = NULL;
	const char* fbits = NULL;
	//find field attributes
	for (const tinyxml2::XMLAttribute *attr = element->FirstAttribute();
			attr;
			attr = attr->Next())
	{
		const char* caname = attr->Name();
		const char* cavalue = attr->Value();

		if (caname == NULL)
		{
			// no attribute
		}
		else if (stringEquals(caname, "TYPE"))
		{
			ftype = cavalue;

			if (ftype == NULL)
			{
				result.Set(GMSEC_STATUS_MESSAGE_ERROR,
					   GMSEC_JSON_PARSE_ERROR,
					   "Invalid xml field format - unable to parse type");
				LOG_WARNING << "BaseField::FromXML : " << result.Get();
				return result;
			}
		}
		else if (stringEquals(caname, "NAME"))
		{
			fname = cavalue;

			if (fname == NULL)
			{
				result.Set(GMSEC_STATUS_MESSAGE_ERROR,
					   GMSEC_JSON_PARSE_ERROR,
					   "Invalid xml field format - unable to parse name");
				LOG_WARNING << "BaseField::FromXML : " << result.Get();
				return result;
			}
		}
		else if (stringEquals(caname, "HEAD"))
		{
			fhead = cavalue;
		}
		else if (stringEquals(caname, "BITS"))
		{
			fbits = cavalue;
		}
	}

	// find value
	for (tinyxml2::XMLNode* node = element->FirstChild(); node; node = node->NextSibling())
	{
		tinyxml2::XMLText *text = node->ToText();
		if (text)
		{
			value = text->Value();
		}
	}

	if (fname == NULL)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
			   GMSEC_JSON_PARSE_ERROR,
			   "Invalid xml field format - unable to parse value");
		LOG_WARNING << "BaseField::FromXML : " << result.Get();
		return result;
	}

	setValues(fname, ftype, value, fhead, fbits, "FromXML");

	return result;
}


Status BaseField::FromXML(const char* xml)
{
	Status result;

	if (xml == NULL)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_XML_PARSE_ERROR, "Invalid xml - null string");
		LOG_WARNING << "BaseField::FromXML : " << result.Get();
		return result;
	}

	tinyxml2::XMLDocument doc;
	int code = doc.Parse(xml);
	if (code != 0)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_XML_PARSE_ERROR, "Invalid xml field format");
		result.SetCustomCode(code);
		LOG_WARNING << "BaseField::FromXML : " << result.Get();
		return result;
	}

	/* get root message node */
	tinyxml2::XMLElement *element = doc.RootElement();
	if (element == NULL)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_XML_PARSE_ERROR, "Invalid xml - null root element");
		LOG_WARNING << "BaseField::FromXML : " << result.Get();
		return result;
	}

	result = FromXML(element);

	return result;
}


const char* BaseField::ToJSON()
{
	std::ostringstream jsonStream;

	const char * name = "";
	if (!fName.empty())
	{
		name = fName.c_str();
	}

	jsonStream << "{\"NAME\":\"" << name << "\",";

	GMSEC_TYPE ftype = GMSEC_TYPE_UNSET;
	GetType(ftype);
	
	switch (ftype)
	{
		case GMSEC_TYPE_CHAR:
			jsonStream << "\"TYPE\":\"CHAR\",";
			jsonStream << "\"VALUE\":\"";
			if (!fI8Value)
			{
				jsonStream << "nul";
			}
			else
			{
				jsonStream << (char) fI8Value;
			}
			jsonStream<< "\"";
			break;
		case GMSEC_TYPE_I8:
			jsonStream << "\"TYPE\":\"I8\",";
			jsonStream << "\"VALUE\":\"" << (int) fI8Value << "\"";
			jsonStream << (int) fI8Value;
			break;
		case GMSEC_TYPE_U8:
			jsonStream << "\"TYPE\":\"U8\",";
			jsonStream << "\"VALUE\":\"" << (unsigned int) fU8Value << "\"";
			break;
		case GMSEC_TYPE_BOOL:
			jsonStream << "\"TYPE\":\"BOOL\",";
			jsonStream << "\"VALUE\":\"" << (fBoolValue ? "TRUE" : "FALSE") << "\"";
			break;
		case GMSEC_TYPE_I16:
			jsonStream << "\"TYPE\":\"I16\",";
			jsonStream << "\"VALUE\":\"" << fI16Value << "\"";
			break;
		case GMSEC_TYPE_U16:
			jsonStream << "\"TYPE\":\"U16\",";
			jsonStream << "\"VALUE\":\"" << fU16Value << "\"";
			break;
		case GMSEC_TYPE_I32:
			jsonStream << "\"TYPE\":\"I32\",";
			jsonStream << "\"VALUE\":\"" << fI32Value << "\"";
			break;
		case GMSEC_TYPE_U32:
			jsonStream << "\"TYPE\":\"U32\",";
			jsonStream << "\"VALUE\":\"" << fU32Value << "\"";
			break;
		case GMSEC_TYPE_F32:
		{
			jsonStream << "\"TYPE\":\"F32\",";
			const char *p = realStringConstant(fF32Value);
			jsonStream << "\"VALUE\":\"";
			if (p)
			{
				jsonStream << p;
			}
			else
			{
				jsonStream << fF32Value;
			}
			jsonStream << "\",";

			jsonStream << "\"BITS\":\"";
			Encoder *encoder = Encoder::getEncoder();

			unsigned char bytes[4];
			char *bytesTmp = reinterpret_cast<char *>(bytes);
			encoder->putF32(&fF32Value, bytesTmp);

			const char *hex = "0123456789ABCDEF";
			for (int i = 0; i < 4; i++)
			{
				jsonStream << hex[bytes[i] / 16];
				jsonStream << hex[bytes[i] % 16];
			}

			jsonStream << "\"";
		}
		break;
		case GMSEC_TYPE_F64:
		{
			jsonStream << "\"TYPE\":\"F64\",";
			const char *p = realStringConstant(fF64Value);
			jsonStream << "\"VALUE\":\"";
			if (p)
			{
				jsonStream << p;
			}
			else
			{
				jsonStream << fF64Value;
			}
			jsonStream << "\",";

			jsonStream << "\"BITS\":\"";
			Encoder *encoder = Encoder::getEncoder();

			unsigned char bytes[8];
			char *bytesTmp = reinterpret_cast<char *>(bytes);
			encoder->putF64(&fF64Value, bytesTmp);

			const char *hex = "0123456789ABCDEF";
			for (int i = 0; i < 8; i++)
			{
				jsonStream << hex[bytes[i] / 16];
				jsonStream << hex[bytes[i] % 16];
			}

			jsonStream << "\"";
		}
		break;
		case GMSEC_TYPE_STRING:
		{
			jsonStream << "\"TYPE\":\"STRING\",";

			size_t vallen = 12;
			size_t slen = 0;

			Escaper &escaper = getEscaper();
			escaper.study(fStrValue.c_str(), slen, vallen);

			jsonStream << "\"VALUE\":\"";

			if (fStrValue.empty())
			{
				// Do nothing
			}
			else if (slen == vallen)
			{
				jsonStream << fStrValue << "\"";
			}
			else if (!fStrValue.empty())
			{
				escaper.put(jsonStream, fStrValue.c_str());
				jsonStream << "\"";
			}
		}
		break;
		case GMSEC_TYPE_BLOB:
		{
			jsonStream << "\"TYPE\":\"BIN\",";
			jsonStream << "\"VALUE\":\"";
			static const char *hex = "0123456789ABCDEF";
			const GMSEC_U8 * tmp = (GMSEC_U8 *) fBinValue;
			for (size_t i = 0; i < fBinSize; ++i)
			{
				int upper = unsigned(tmp[i]) / 16;
				int lower = unsigned(tmp[i]) % 16;
				jsonStream << hex[upper] << hex[lower];
			}
			jsonStream << "\"";
		}
		break;
		case GMSEC_TYPE_I64:
		{
			jsonStream << "\"TYPE\":\"I64\",";
#ifdef GMSEC_VC6
			char tmp[32];
			gmsec::util::I64toString(fI64Value, tmp, 32);
			jsonStream << "\"VALUE\":\"" << tmp << "\"";
#else
			jsonStream << "\"VALUE\":\"" << fI64Value << "\"";
#endif
			break;
		case GMSEC_TYPE_U64:
			jsonStream << "\"TYPE\":\"U64\",";
#ifdef GMSEC_VC6
			char tmp[32];
			gmsec::util::U64toString(fU64Value, tmp);
			jsonStream << "\"VALUE\":\"" << tmp << "\"";
#else
			jsonStream << "\"VALUE\":\"" << fU64Value << "\"";
#endif
		}
		break;
		default:
			LOG_WARNING << "BaseField::ToJSON : Field has unknown or unset type.";
	}

	jsonStream << "}";

	if (fJsonCache)
	{
		delete[] fJsonCache;
	}

	fJsonCache = stringNew(jsonStream.rdbuf()->str().c_str());

	return fJsonCache;
}


Status BaseField::FromJSON(const Json::Value &root)
{
	Status result;

	// Required attributes
	const char* name = NULL;
	const char* type = NULL;
	const char* value = NULL;

	if (root.isMember(std::string("NAME")))
	{
		name = root["NAME"].asCString();
	}
	else
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_JSON_PARSE_ERROR,
		           "Invalid json field format - no NAME provided");
                LOG_WARNING << "BaseField::FromJSON : " << result.Get();
		return result;
	}

	if (root.isMember(std::string("TYPE")))
	{
		type = root["TYPE"].asCString();
	}
	else
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_JSON_PARSE_ERROR,
		           "Invalid json field format - no TYPE provided");
                LOG_WARNING << "BaseField::FromJSON : " << result.Get();
		return result;
	}

	if (root.isMember(std::string("VALUE")))
	{
		value = root["VALUE"].asCString();
	}
	else
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_JSON_PARSE_ERROR,
		           "Invalid json field format - no VALUE provided");
                LOG_WARNING << "BaseField::FromJSON : " << result.Get();
		return result;
	}

	// Non-required attributes
	const char* head = NULL;
	const char* bits = NULL;

	if (root.isMember(std::string("HEAD")))
	{
		head = root["HEAD"].asCString();
	}

	if (root.isMember(std::string("BITS")))
	{
		bits = root["BITS"].asCString();
	}

	setValues(name, type, value, head, bits, "FromJSON");

	return result;
}


Status BaseField::setValues(const char* name, const char* type, const char* value, const char* head, const char* bits, const char* callingFunction)
{
	// Note: Both FromXML and FromJSON both check for null name, type, and value before calling this function
	// This was done intentionally so as to discern between the two functions in their error messages.
	Status result;

	SetName(name);

	GMSEC_TYPE itype = Field::LookupType(type);

	if (itype == GMSEC_TYPE_UNSET)
	{
		LOG_WARNING << "BaseField::setValues (" << callingFunction << "): bad type '" << type << "'";
	}
	else
	{
		if (bits != NULL)
		{
			itype |= GMSEC_FLAG_BITS_VALUE;
			value = bits;
		}

		result = SetValue(itype, value);

		if (result.isError())
		{
			LOG_WARNING << "BaseField::setValues (" << callingFunction << "): unable to SetValue for " << name << " type " << type << ": " << result.Get();
		}

		if (head && head[0] == 'T')
		{
			setHeader(true);
		}
	}

	return result;
}


bool BaseField::operator== (const BaseField &b) const
{
	GMSEC_TYPE atype = GMSEC_TYPE_UNSET;
	GMSEC_TYPE btype = GMSEC_TYPE_UNSET;

	GetType(atype);
	b.GetType(btype);

	if (atype != btype && atype != GMSEC_TYPE_UNSET)
		return false;

	switch (atype)
	{
	case GMSEC_TYPE_CHAR:
	{
		GMSEC_CHAR aval = 0;
		GMSEC_CHAR bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval == bval);
	}
	case GMSEC_TYPE_I8:
	{
		GMSEC_I8 aval = 0;
		GMSEC_I8 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval == bval);
	}
	case GMSEC_TYPE_U8:
	{
		GMSEC_U8 aval = 0;
		GMSEC_U8 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval == bval);
	}
	case GMSEC_TYPE_BOOL:
	{
		GMSEC_BOOL aval = GMSEC_FALSE;
		GMSEC_BOOL bval = GMSEC_FALSE;
		GetValue(aval);
		b.GetValue(bval);
		return (aval == bval);
	}
	case GMSEC_TYPE_I16:
	{
		GMSEC_I16 aval = 0;
		GMSEC_I16 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval == bval);
	}
	case GMSEC_TYPE_U16:
	{
		GMSEC_U16 aval = 0;
		GMSEC_U16 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval == bval);
	}
	case GMSEC_TYPE_I32:
	{
		GMSEC_I32 aval = 0;
		GMSEC_I32 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval == bval);
	}
	case GMSEC_TYPE_U32:
	{
		GMSEC_U32 aval = 0;
		GMSEC_U32 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval == bval);
	}
	case GMSEC_TYPE_F32:
	{
		GMSEC_F32 aval = 0;
		GMSEC_F32 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval == bval);
	}
	case GMSEC_TYPE_F64:
	{
		GMSEC_F64 aval = 0;
		GMSEC_F64 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval == bval);
	}
	case GMSEC_TYPE_STRING:
	{
		GMSEC_STR aval = 0;
		GMSEC_STR bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return stringEquals(aval, bval);
	}
	case GMSEC_TYPE_BLOB:
		// tbd
		break;
	case GMSEC_TYPE_I64:
	{
		GMSEC_I64 aval = 0;
		GMSEC_I64 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval == bval);
	}
	case GMSEC_TYPE_U64:
	{
		GMSEC_U64 aval;
		GMSEC_U64 bval;
		GetValue(aval);
		b.GetValue(bval);
		return (aval == bval);
	}
	}
	return false;
}


bool BaseField::operator< (const BaseField &b) const
{
	GMSEC_TYPE atype = GMSEC_TYPE_UNSET;
	GMSEC_TYPE btype = GMSEC_TYPE_UNSET;

	GetType(atype);
	b.GetType(btype);

	if (atype != btype && atype != GMSEC_TYPE_UNSET)
		return false;

	switch (atype)
	{
	case GMSEC_TYPE_CHAR:
	{
		GMSEC_CHAR aval = 0;
		GMSEC_CHAR bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval < bval);
	}
	case GMSEC_TYPE_I8:
	{
		GMSEC_I8 aval = 0;
		GMSEC_I8 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval < bval);
	}
	case GMSEC_TYPE_U8:
	{
		GMSEC_U8 aval = 0;
		GMSEC_U8 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval < bval);
	}
	case GMSEC_TYPE_BOOL:
	{
		GMSEC_BOOL aval = GMSEC_FALSE;
		GMSEC_BOOL bval = GMSEC_FALSE;
		GetValue(aval);
		b.GetValue(bval);
		return (aval < bval);
	}
	case GMSEC_TYPE_I16:
	{
		GMSEC_I16 aval = 0;
		GMSEC_I16 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval < bval);
	}
	case GMSEC_TYPE_U16:
	{
		GMSEC_U16 aval = 0;
		GMSEC_U16 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval < bval);
	}
	case GMSEC_TYPE_I32:
	{
		GMSEC_I32 aval = 0;
		GMSEC_I32 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval < bval);
	}
	case GMSEC_TYPE_U32:
	{
		GMSEC_U32 aval = 0;
		GMSEC_U32 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval < bval);
	}
	case GMSEC_TYPE_F32:
	{
		GMSEC_F32 aval = 0;
		GMSEC_F32 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval < bval);
	}
	case GMSEC_TYPE_F64:
	{
		GMSEC_F64 aval = 0;
		GMSEC_F64 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval < bval);
	}
	case GMSEC_TYPE_STRING:
	{
		GMSEC_STR aval = 0;
		GMSEC_STR bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (!stringEquals(aval, bval));
	}
	case GMSEC_TYPE_BLOB:
		// tbd
		break;
	case GMSEC_TYPE_I64:
	{
		GMSEC_I64 aval = 0;
		GMSEC_I64 bval = 0;
		GetValue(aval);
		b.GetValue(bval);
		return (aval < bval);
	}
	case GMSEC_TYPE_U64:
	{
		GMSEC_U64 aval;
		GMSEC_U64 bval;
		GetValue(aval);
		b.GetValue(bval);
		return (aval < bval);
	}
	}

	return false;
}


