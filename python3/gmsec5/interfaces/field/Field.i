/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Field
%{
#include <gmsec5/field/Field.h>
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
%rename("to_xml") toXML;
%rename("to_json") toJSON;
%rename("get_string_value") getStringValue;
%rename("get_boolean_value") getBooleanValue;
%rename("get_integer_value") getI64Value;
%rename("get_double_value") getF64Value;

%include "dox/Field_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/Field.h>

