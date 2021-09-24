/*
 * Copyright 2007-2021 United States Government as represented by the
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

%rename("is_error") isError;
%rename("get") get;
%rename("set") set;
%rename("get_class") getClass;
%rename("set_class") setClass;
%rename("get_code") getCode;
%rename("set_code") setCode;
%rename("get_reason") getReason;
%rename("set_reason") setReason;
%rename("get_custom_code") getCustomCode;
%rename("set_custom_code") setCustomCode;
%rename("reset") reset;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Status.h>

