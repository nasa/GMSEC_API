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

%ignore gmsec::api5::Status::Status(const GmsecException&);
%ignore gmsec::api5::Status::operator=(const Status&);
%ignore gmsec::api5::Status::get();

%rename("HasError") hasError;
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


%typemap(cscode) gmsec::api5::Status %{
    public Status(GmsecException e)
        : this(e.GetErrorClass(), e.GetErrorCode(), e.GetErrorMessage(), e.GetCustomCode())
    {
    }


    public string Get()
    {
        System.Text.StringBuilder sb = new System.Text.StringBuilder();

        sb.Append("[").Append(this.GetClass()).Append(",").Append(this.GetCode()).Append(",").Append(this.GetCustomCode()).Append("] : ").Append(this.GetReason());

        return sb.ToString();
    }

%}

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Status.h>

