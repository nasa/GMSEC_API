/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module FieldSpecification

%{
#include <gmsec5/FieldSpecification.h>
using namespace gmsec::api5;
%}

%rename("get_name") getName;
%rename("get_field_type") getType;
%rename("get_mode") getMode;
%rename("get_classification") getClassification;
%rename("get_value") getValue;
%rename("get_description") getDescription;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/FieldSpecification.h>
