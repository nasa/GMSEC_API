/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module CharField

%{
#include <gmsec4/field/CharField.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::CharField::getValue() const;

%rename("get_value") getValue;
%rename("to_xml") toXML;
%rename("to_json") toJSON;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/CharField.h>

%extend gmsec::api::CharField {

    GMSEC_CHAR CALL_TYPE getValue()
    {
        // restrict to single-byte ASCII values
        return self->getValue() & 0x7F;
    }
};
