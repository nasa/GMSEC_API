/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Field

%{
#include <gmsec5/field/Field.h>
using namespace gmsec::api5;
%}

/* Avoid name clash with Object.GetType() */
%rename("GetName") getName;
%rename("GetFieldType") getType;
%rename("IsHeader") isHeader;
%rename("ToXML") toXML;
%rename("ToJSON") toJSON;
%rename("GetStringValue") getStringValue;
%rename("GetBooleanValue") getBooleanValue;
%rename("GetI16Value") getI16Value;
%rename("GetI32Value") getI32Value;
%rename("GetU16Value") getU16Value;
%rename("GetU32Value") getU32Value;
%rename("GetI64Value") getI64Value;
%rename("GetU64Value") getU64Value;
%rename("GetF64Value") getF64Value;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/Field.h>

