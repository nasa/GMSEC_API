/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module F32Field

%{
#include <gmsec4/field/F32Field.h>
using namespace gmsec::api;
%}

%rename("GetValue") getValue;
%rename("ToXML") toXML;
%rename("ToJSON") toJSON;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/F32Field.h>

