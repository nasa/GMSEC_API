/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module U32Field

%{
#include <gmsec5/field/U32Field.h>
using namespace gmsec::api5;
%}

%rename("GetValue") getValue;
%rename("ToXML") toXML;
%rename("ToJSON") toJSON;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/U32Field.h>

