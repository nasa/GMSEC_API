/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module BinaryField

%{
#include <gmsec5/field/BinaryField.h>
#include <string>
#include <iostream>
using namespace gmsec::api5;
%}

%ignore gmsec::api5::BinaryField::getValue() const;

%rename("get_value") getValue;
%rename("get_length") getLength;
%rename("to_xml") toXML;
%rename("to_json") toJSON;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/BinaryField.h>

%extend gmsec::api5::BinaryField {

    VALUE CALL_TYPE getValue() {
        return rb_str_new((char*) self->getValue(), self->getLength());
    }
};
