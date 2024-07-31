/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Field
%{
#include <gmsec5/field/Field.h>
#include <gmsec5/field/BinaryField.h>
#include <gmsec5/field/BooleanField.h>
#include <gmsec5/field/CharField.h>
#include <gmsec5/field/F32Field.h>
#include <gmsec5/field/F64Field.h>
#include <gmsec5/field/I16Field.h>
#include <gmsec5/field/I32Field.h>
#include <gmsec5/field/I64Field.h>
#include <gmsec5/field/I8Field.h>
#include <gmsec5/field/StringField.h>
#include <gmsec5/field/U16Field.h>
#include <gmsec5/field/U32Field.h>
#include <gmsec5/field/U64Field.h>
#include <gmsec5/field/U8Field.h>
using namespace gmsec::api5;
%}

/* ignore methods that do not make sense in the python context */
%ignore gmsec::api5::Field::getI16Value();
%ignore gmsec::api5::Field::getI32Value();
%ignore gmsec::api5::Field::getU16Value();
%ignore gmsec::api5::Field::getU32Value();
%ignore gmsec::api5::Field::getU64Value();


%rename("get_name") getName;
%rename("get_type") getType;
%rename("is_header") isHeader;
%rename("is_tracking") isTracking;
%rename("to_xml") toXML;
%rename("to_json") toJSON;
%rename("get_string_value") getStringValue;
%rename("get_boolean_value") getBooleanValue;
%rename("get_integer_value") getI64Value;
%rename("get_double_value") getF64Value;

%include "dox/Field_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/Field.h>

%extend gmsec::api5::Field
{
    static gmsec::api5::BinaryField* toBinaryField(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::BinaryField*>(field);
    }

    static gmsec::api5::BooleanField* toBooleanField(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::BooleanField*>(field);
    }

    static gmsec::api5::CharField* toCharField(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::CharField*>(field);
    }

    static gmsec::api5::F32Field* toF32Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::F32Field*>(field);
    }

    static gmsec::api5::F64Field* toF64Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::F64Field*>(field);
    }

    static gmsec::api5::I16Field* toI16Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::I16Field*>(field);
    }

    static gmsec::api5::I32Field* toI32Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::I32Field*>(field);
    }

    static gmsec::api5::I64Field* toI64Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::I64Field*>(field);
    }

    static gmsec::api5::I8Field* toI8Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::I8Field*>(field);
    }

    static gmsec::api5::StringField* toStringField(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::StringField*>(field);
    }

    static gmsec::api5::U16Field* toU16Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::U16Field*>(field);
    }

    static gmsec::api5::U32Field* toU32Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::U32Field*>(field);
    }

    static gmsec::api5::U64Field* toU64Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::U64Field*>(field);
    }

    static gmsec::api5::U8Field* toU8Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::U8Field*>(field);
    }
}
