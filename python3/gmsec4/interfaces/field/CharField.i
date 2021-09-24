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

%rename("get_value") getValue;
%rename("to_XML") toXML;
%rename("to_JSON") toJSON;

%include "dox/CharField_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/CharField.h>


%extend gmsec::api::CharField
{
    CharField* CALL_TYPE clone() const
    {
        return dynamic_cast<CharField*>(self->clone());
    }

    static CharField* CALL_TYPE cast_field(Field* field)
    {
        CharField* casted = dynamic_cast<CharField*>(field);

        if (casted == NULL)
        {
            throw Exception(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a CharField");
        }

        return casted;
    }
};
