/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module GmsecException

%{
#include <gmsec5/GmsecException.h>
using namespace gmsec::api5;
%}

%ignore gmsec::api5::GmsecException::operator=(const GmsecException&);

%rename("GmsecError") GmsecException;

%rename("get_error_class") getErrorClass;
%rename("get_error_code") getErrorCode;
%rename("get_error_message") getErrorMessage;
%rename("get_custom_code") getCustomCode;


%include "dox/GmsecException_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/GmsecException.h>


%exceptionclass gmsec::api5::GmsecException;

%feature("director:except") {
    if ($error != NULL) {
        PyObject *ptype, *pvalue, *ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);

        gmsec::api5::GmsecException e(NO_ERROR_CLASS, NO_ERROR_CODE, 0, "");

        void* argp1 = 0;
        int   res1  = SWIG_ConvertPtr(pvalue, &argp1, SWIGTYPE_p_gmsec__api5__GmsecException, 0);

        if (SWIG_IsOK(res1)) {
            gmsec::api5::GmsecException* tmp = reinterpret_cast<gmsec::api5::GmsecException*>(argp1);
            e = *tmp;
        }
        else {
            PyObject*   str_pvalue = PyObject_Repr(pvalue);
            PyObject*   pyStr      = PyUnicode_AsEncodedString(str_pvalue, "utf-8", "Error");
            const char* str        = PyBytes_AS_STRING(pyStr);

            if (str != NULL) {
                e = gmsec::api5::GmsecException(OTHER_ERROR, OTHER_ERROR_CODE, str);
            }
            else {
                e = gmsec::api5::GmsecException(OTHER_ERROR, OTHER_ERROR_CODE, "Unknown error");
            }

            Py_XDECREF(str_pvalue);
            Py_XDECREF(pyStr);
        }

        PyErr_Restore(ptype, pvalue, ptraceback);

        throw e;
    }
}

%exception {
    try
    {
        $action
    }
    catch (const gmsec::api5::GmsecException& e)
    {
        gmsec::api5::GmsecException *ecopy = new gmsec::api5::GmsecException(e);
        PyObject *err = SWIG_NewPointerObj(ecopy, SWIGTYPE_p_gmsec__api5__GmsecException, SWIG_POINTER_NEW | 0);
        PyErr_SetObject(SWIG_Python_ExceptionType(SWIGTYPE_p_gmsec__api5__GmsecException), err);
        SWIG_fail;
    } 
}

%extend gmsec::api5::GmsecException
{
    const char* __str__()
    {
        return $self->what();
    }
}
