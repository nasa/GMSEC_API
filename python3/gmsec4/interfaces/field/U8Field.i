/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module U8Field
%{
#include <gmsec4/field/U8Field.h>
using namespace gmsec::api;
%}

%rename("get_value") getValue;
%rename("to_XML") toXML;
%rename("to_JSON") toJSON;

%include "dox/U8Field_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/U8Field.h>


%extend gmsec::api::U8Field
{
    U8Field* CALL_TYPE clone() const
    {
        return dynamic_cast<U8Field*>(self->clone());
    }

    static U8Field* CALL_TYPE cast_field(Field* field)
    {
        U8Field* casted = dynamic_cast<U8Field*>(field);

        if (casted == NULL)
        {
            throw Exception(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a U8Field");
        }

        return casted;
    }
};
