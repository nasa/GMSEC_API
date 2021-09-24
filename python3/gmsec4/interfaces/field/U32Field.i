/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module U32Field
%{
#include <gmsec4/field/U32Field.h>
using namespace gmsec::api;
%}

%rename("get_value") getValue;
%rename("to_XML") toXML;
%rename("to_JSON") toJSON;

%include "dox/U32Field_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/U32Field.h>


%extend gmsec::api::U32Field
{
    U32Field* CALL_TYPE clone() const
    {
        return dynamic_cast<U32Field*>(self->clone());
    }

    static U32Field* CALL_TYPE cast_field(Field* field)
    {
        U32Field* casted = dynamic_cast<U32Field*>(field);

        if (casted == NULL)
        {
            throw Exception(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a U32Field");
        }

        return casted;
    }
};
