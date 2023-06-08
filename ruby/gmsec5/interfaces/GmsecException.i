/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Exception

%include "exception.i"

%{
#include <gmsec5/GmsecException.h>
using namespace gmsec::api5;
%}

%ignore gmsec::api5::GmsecException::operator=(const GmsecException&);

%rename("GmsecException") GmsecException;
%rename("get_error_class") getErrorClass;
%rename("get_error_code") getErrorCode;
%rename("get_error_message") getErrorMessage;
%rename("get_custom_code") getCustomCode;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/GmsecException.h>


/* Generate a Ruby-style exception in response to any action that results
 * in a GMSEC-style exception being thrown.
 */
%exceptionclass GmsecException;

%exception {
    try {
        $action
    }
    catch (const gmsec::api5::GmsecException& e) {
        static VALUE ex = rb_define_class("GmsecException", rb_eException);
        rb_raise(ex, "%s", e.what());
    }
};


%extend gmsec::api5::GmsecException
{
    const char* CALL_TYPE message()
    {
        return self->what();
    }

    GmsecException(const char* str)
    {
        if (str) {
            int clazz;
            int code;
            int custom;

            int r = sscanf(str, "[%d,%d,%d]", &clazz, &code, &custom);

            if (r == 3) {
                std::string reason(str);
                size_t pos = reason.find(" : ");
                if (pos != std::string::npos) {
                    reason = reason.substr(pos + 3);
                }

                return new GmsecException(clazz, code, custom, reason.c_str());
            }
        }

        return NULL;
    }
}
