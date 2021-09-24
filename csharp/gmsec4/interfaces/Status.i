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

%ignore gmsec::api::Status::Status(const Exception&);
%ignore gmsec::api::Status::operator=(const Status&);
%ignore gmsec::api::Status::get();

%rename("IsError") isError;
//DMW %rename("Get") get;
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


%typemap(cscode) gmsec::api::Status %{
    public Status(GmsecException e)
        : this(e.GetErrorClass(), e.GetErrorCode(), e.GetErrorMessage(), e.GetCustomCode())
    {
    }


    public string Get()
    {
        System.Text.StringBuilder sb = new System.Text.StringBuilder();

        sb.Append("[").Append(this.GetClass()).Append(",").Append(this.GetCode()).Append(",").Append(this.GetCustomCode()).Append("]: ").Append(this.GetReason());

        return sb.ToString();
    }

%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Status.h>
