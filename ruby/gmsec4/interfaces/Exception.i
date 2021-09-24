/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Exception

%include "exception.i"

%{
#include <gmsec4/Exception.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::Exception::operator=(const Exception&);

%rename("GmsecException") Exception;
%rename("get_error_class") getErrorClass;
%rename("get_error_code") getErrorCode;
%rename("get_error_message") getErrorMessage;
%rename("get_custom_code") getCustomCode;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Exception.h>


/* Generate a Ruby-style exception in response to any action that results
 * in a GMSEC-style exception being thrown.
 */
%exceptionclass GmsecException;

%exception {
    try {
        $action
    }
    catch (const gmsec::api::Exception& e) {
        static VALUE ex = rb_define_class("GmsecException", rb_eException);
        rb_raise(ex, "%s", e.what());
    }
};
