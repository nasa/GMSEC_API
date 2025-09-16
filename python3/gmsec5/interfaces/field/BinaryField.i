/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module BinaryField

%{
#include <gmsec5/field/BinaryField.h>
using namespace gmsec::api5;
%}

%rename("get_value") getValue;
%rename("get_length") getLength;
%rename("to_xml") toXML;
%rename("to_json") toJSON;

%ignore gmsec::api5::BinaryField(const char* name, const GMSEC_BIN, size_t);
%ignore gmsec::api5::BinaryField::getValue() const;

%include "dox/BinaryField_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/BinaryField.h>


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


%extend gmsec::api5::BinaryField
{
    BinaryField(char* name, PyObject* data, size_t length)
    {
        char* blob = PyByteArray_AsString(data);

        return new BinaryField(name, (const GMSEC_U8*) blob, length);
    }

    PyObject* CALL_TYPE getValue()
    {
        return PyByteArray_FromStringAndSize((const char*) self->getValue(), (Py_ssize_t) self->getLength());
    }

    /* Obsolete as of API 5.1, but leave in place to preserve binary compatibility */
    static BinaryField* CALL_TYPE cast_field(Field* field)
    {
        BinaryField* casted = dynamic_cast<BinaryField*>(field);

        if (casted == NULL)
        {
            throw GmsecException(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a BinaryField");
        }

        return casted;
    }
};
