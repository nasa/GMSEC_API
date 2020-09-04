/*
 * Copyright 2007-2019 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Status

%{
#include <gmsec4/Status.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::Status::operator=(const Status&);

%rename("IsError") isError;
%rename("Get") get;
%rename("Set") set;
%rename("GetClass") getClass;
%rename("SetClass") setClass;
%rename("GetCode") getCode;
%rename("SetCode") setCode;
%rename("GetReason") getReason;
%rename("SetReason") setReason;
%rename("GetCustomCode") getCustomCode;
%rename("SetCustomCode") setCustomCode;
%rename("Reset") reset;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Status.h>

