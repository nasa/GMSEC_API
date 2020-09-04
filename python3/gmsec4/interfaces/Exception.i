/*
 * Copyright 2007-2019 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Exception


%{
#include <gmsec4/Exception.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::Exception::operator=(const Exception&);

%rename("GmsecError") Exception;

%rename("get_error_class") getErrorClass;
%rename("get_error_code") getErrorCode;
%rename("get_error_message") getErrorMessage;
%rename("get_custom_code") getCustomCode;


%include "dox/Exception_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Exception.h>


%exceptionclass gmsec::api::Exception;


%exception {
    try
    {
        $action
    }
    catch (const gmsec::api::Exception& e)
    {
        gmsec::api::Exception *ecopy = new gmsec::api::Exception(e);
        PyObject *err = SWIG_NewPointerObj(ecopy, SWIGTYPE_p_gmsec__api__Exception, 1);
        PyErr_SetObject(SWIG_Python_ExceptionType(SWIGTYPE_p_gmsec__api__Exception), err);
        SWIG_fail;
    } 
}


%extend gmsec::api::Exception
{
    const char* __str__()
    {
        return $self->what();
    }
}
