/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module FieldSpecification

%{
#include <gmsec4/mist/FieldSpecification.h>
using namespace gmsec::api::mist;
%}

%rename("GetName") getName;
%rename("GetFieldType") getType;
%rename("GetMode") getMode;
%rename("GetClassification") getClassification;
%rename("GetValue") getValue;
%rename("GetDescription") getDescription;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/FieldSpecification.h>
