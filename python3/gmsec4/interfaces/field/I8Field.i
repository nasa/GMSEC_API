/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module I8Field
%{
#include <gmsec4/field/I8Field.h>
using namespace gmsec::api;
%}

%rename("get_value") getValue;
%rename("to_XML") toXML;
%rename("to_JSON") toJSON;

%include "dox/I8Field_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/I8Field.h>


%extend gmsec::api::I8Field
{
    I8Field* CALL_TYPE clone() const
    {
        return dynamic_cast<I8Field*>(self->clone());
    }

    static I8Field* CALL_TYPE cast_field(Field* field)
    {
        I8Field* casted = dynamic_cast<I8Field*>(field);

        if (casted == NULL)
        {
            throw Exception(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a I8Field");
        }

        return casted;
    }
};
