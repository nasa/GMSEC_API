/*
 * Copyright 2007-2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Article 17, U.S. Code.
 * All Rights Reserved.
 */

%module FieldSpecification

%{
#include <gmsec4/mist/FieldSpecification.h>
using namespace gmsec::api::mist;
%}

%rename("get_name") getName;
%rename("get_field_type") getType;
%rename("get_mode") getMode;
%rename("get_classification") getClassification;
%rename("get_value") getValue;
%rename("get_description") getDescription;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/FieldSpecification.h>
