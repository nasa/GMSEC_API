/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Exception
%{
#include <gmsec4/Exception.h>
using namespace gmsec::api;
%}


%include "dox/Exception_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Exception.h>

%exceptionclass gmsec::api::Exception;

%exception {
        try
        {
                $action
        }
        catch (gmsec::api::Exception& e)
        {
                gmsec::api::Exception *ecopy = new gmsec::api::Exception(e);
                PyObject *err = SWIG_NewPointerObj(ecopy, SWIGTYPE_p_gmsec__api__Exception, 1);
                PyErr_SetObject(SWIG_Python_ExceptionType(SWIGTYPE_p_gmsec__api__Exception), err);
                SWIG_fail;
        } 
}


