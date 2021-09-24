/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Field

%{
#include <gmsec4/field/Field.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::Field::is_header() const;
%ignore gmsec::api::Field::is_header(bool);

%rename("get_field_type") getType;
%rename("get_field_name") getName;
%rename("set_field_name") setName;
%rename("is_header") isHeader;
%rename("to_xml") toXML;
%rename("to_json") toJSON;
%rename("get_integer_value") getIntegerValue;
%rename("get_unsigned_integer_value") getUnsignedIntegerValue;
%rename("get_double_value") getDoubleValue;
%rename("get_string_value") getStringValue;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/Field.h>

%extend gmsec::api::Field {
    void set_header(bool flag) {
        self->isHeader(flag);
    }

    bool is_header() {
        return self->isHeader();
    }
}
