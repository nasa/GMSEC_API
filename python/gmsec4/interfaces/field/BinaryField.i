/*
 * Copyright 2007-2017 United States Government as represented by the
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

%ignore gmsec::api::BinaryField::getValue() const;

%include "dox/BinaryField_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/BinaryField.h>

%extend gmsec::api::BinaryField {
    std::string CALL_TYPE getValue()
    {
        return std::string(reinterpret_cast<char*>(self->getValue()), self->getLength());
    }
};
