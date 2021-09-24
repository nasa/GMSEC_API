/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



// Manageg
#include <field/FieldConverter_Net.h>
#include <Fields_Net.h>

// C++ API native
#include <gmsec4/Fields.h>

using namespace GMSEC::API;


Field^ FieldConverter::CreateFieldReference(gmsec::api::Field* nativeField)
{
	Field^ field = nullptr;

	switch (nativeField->getType())
	{
		case gmsec::api::Field::BIN_TYPE:    field = gcnew BinaryField(nativeField); break;
		case gmsec::api::Field::BOOL_TYPE:   field = gcnew BooleanField(nativeField); break;
		case gmsec::api::Field::CHAR_TYPE:   field = gcnew CharField(nativeField); break;
		case gmsec::api::Field::F32_TYPE:    field = gcnew F32Field(nativeField); break;
		case gmsec::api::Field::F64_TYPE:    field = gcnew F64Field(nativeField); break;
		case gmsec::api::Field::I8_TYPE:     field = gcnew I8Field(nativeField); break;
		case gmsec::api::Field::I16_TYPE:    field = gcnew I16Field(nativeField); break;
		case gmsec::api::Field::I32_TYPE:    field = gcnew I32Field(nativeField); break;
		case gmsec::api::Field::I64_TYPE:    field = gcnew I64Field(nativeField); break;
		case gmsec::api::Field::U8_TYPE:     field = gcnew U8Field(nativeField); break;
		case gmsec::api::Field::U16_TYPE:    field = gcnew U16Field(nativeField); break;
		case gmsec::api::Field::U32_TYPE:    field = gcnew U32Field(nativeField); break;
		case gmsec::api::Field::U64_TYPE:    field = gcnew U64Field(nativeField); break;
		case gmsec::api::Field::STRING_TYPE: field = gcnew StringField(nativeField); break;
	}

	return field;
}
