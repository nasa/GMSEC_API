/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ProductFile
%{
#include <gmsec4/mist/ProductFile.h>
using namespace gmsec::api::mist;
%}

%ignore gmsec::api::mist::ProductFile(const char*, const char*, const char*, const char*, GMSEC_BIN, size_t);
%ignore gmsec::api::mist::ProductFile::getContents(GMSEC_BIN&) const;

%ignore gmsec::api::mist::ProductFile::operator=(const ProductFile&);
%ignore gmsec::api::mist::ProductFile::operator==(const ProductFile&) const;
%ignore gmsec::api::mist::ProductFile::operator!=(const ProductFile&) const;

%rename("get_name") getName;
%rename("get_description") getDescription;
%rename("get_version") getVersion;
%rename("get_format") getFormat;
%rename("URI_available") uriAvailable;
%rename("get_URI") getURI;
%rename("contents_available") contentsAvailable;
%rename("get_contents") getContents;

%include "dox/ProductFile_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ProductFile.h>


%typemap(in) (char* name, char* description, char* version, char* format, char* data, size_t fileSize)
{
    if (!PyByteArray_Check($input))
    {
        SWIG_exception_fail(SWIG_TypeError, "Bad argument");
    }

    $1 = (char*) PyByteArray_AsString($input);
    $2 = (char*) PyByteArray_AsString($input);
    $3 = (char*) PyByteArray_AsString($input);
    $4 = (char*) PyByteArray_AsString($input);
    $5 = (PyObject*) PyByteArray_FromObject($input);
    $6 = (size_t) PyByteArray_Size($input);
}


%extend gmsec::api::mist::ProductFile
{
    ProductFile(char* name, char* description, char* version, char* format, PyObject* data, size_t fileSize)
    {
        char* blob = PyByteArray_AsString(data);

        return new ProductFile(name, description, version, format, (GMSEC_BIN) blob, fileSize);
    }

    PyObject* CALL_TYPE getContents()
    {
        GMSEC_BIN contents = NULL;
        size_t    fileSize = self->getContents(contents);

        return PyByteArray_FromStringAndSize((const char*) contents, (Py_ssize_t) fileSize);
    }
}
