/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Status
%{
#include <gmsec5/Status.h>
using namespace gmsec::api5;
%}

%ignore gmsec::api5::Status::operator=(const Status&);

%rename("has_error") hasError;
%rename("get_class") getClass;
%rename("set_class") setClass;
%rename("get_code") getCode;
%rename("set_code") setCode;
%rename("get_reason") getReason;
%rename("set_reason") setReason;
%rename("get_custom_code") getCustomCode;
%rename("set_custom_code") setCustomCode;

%include "dox/Status_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Status.h>

