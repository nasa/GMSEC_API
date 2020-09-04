/*
 * Copyright 2007-2019 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module BinaryField

%{
#include <gmsec4/field/BinaryField.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::BinaryField(const char* name, const GMSEC_BIN, size_t);
%ignore gmsec::api::BinaryField::getValue() const;

%include "dox/BinaryField_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/BinaryField.h>


%typemap(in) (char* name, char* data, size_t length)
{
    if (!PyByteArray_Check($input))
    {
        SWIG_exception_fail(SWIG_TypeError, "Bad argument");
    }

    $1 = (char*) PyByteArray_AsString($input);
    $2 = (PyObject*) PyByteArray_FromObject($input);
    $3 = (size_t) PyByteArray_Size($input);
}


%extend gmsec::api::BinaryField
{
    BinaryField(char* name, PyObject* data, size_t length)
    {
        char* blob = PyByteArray_AsString(data);

        return new BinaryField(name, (const GMSEC_BIN) blob, length);
    }

    PyObject* CALL_TYPE getValue()
    {
        return PyByteArray_FromStringAndSize((const char*) self->getValue(), (Py_ssize_t) self->getLength());
    }
};
