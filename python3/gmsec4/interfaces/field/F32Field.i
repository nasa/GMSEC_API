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

%rename("get_value") getValue;
%rename("to_XML") toXML;
%rename("to_JSON") toJSON;

%include "dox/F32Field_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/F32Field.h>

