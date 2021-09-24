/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module I16Field
%{
#include <gmsec4/field/I16Field.h>
using namespace gmsec::api;
%}

%rename("get_value") getValue;
%rename("to_XML") toXML;
%rename("to_JSON") toJSON;

%include "dox/I16Field_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/I16Field.h>


%extend gmsec::api::I16Field
{
    I16Field* CALL_TYPE clone() const
    {
        return dynamic_cast<I16Field*>(self->clone());
    }

    static I16Field* CALL_TYPE cast_field(Field* field)
    {
        I16Field* casted = dynamic_cast<I16Field*>(field);

        if (casted == NULL)
        {
            throw Exception(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a I16Field");
        }

        return casted;
    }
};
