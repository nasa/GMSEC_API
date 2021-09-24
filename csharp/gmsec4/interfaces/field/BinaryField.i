/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module BinaryField

%{
#include <gmsec4/field/BinaryField.h>
using namespace gmsec::api;
%}

%include <std_string.i>
%include <arrays_csharp.i>
#ifdef SWIGCSHARP
%include "csmodule.i"
#endif

%apply unsigned char INOUT[] {unsigned char* blob}
%apply int {size_t}

%ignore gmsec::api::BinaryField::getValue() const;

%rename("GetValue") getValue;
%rename("GetLength") getLength;
%rename("ToXML") toXML;
%rename("ToJSON") toJSON;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/BinaryField.h>

%extend gmsec::api::BinaryField {
    GMSEC_BinaryData CALL_TYPE getValue()
    {
        return GMSEC_BinaryData(self->getValue(), (int) self->getLength());
    }
};
