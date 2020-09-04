/*
 * Copyright 2007-2017 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module StringField
%{
#include <gmsec4/field/StringField.h>
using namespace gmsec::api;
%}


%include "dox/StringField_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/StringField.h>

