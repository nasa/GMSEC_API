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

%include <std_string.i>
%include <arrays_csharp.i>
#ifdef SWIGCSHARP
%include "csmodule.i"
#endif

%apply unsigned char INOUT[] {unsigned char* blob}
%apply int {size_t}

%ignore gmsec::api5::BinaryField::getValue() const;

%rename("GetValue") getValue;
%rename("GetLength") getLength;
%rename("ToXML") toXML;
%rename("ToJSON") toJSON;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/BinaryField.h>

%extend gmsec::api5::BinaryField {
    GmsecBinaryData CALL_TYPE getValue()
    {
        return GmsecBinaryData(const_cast<GMSEC_U8*>(self->getValue()), (int) self->getLength());
    }
};
