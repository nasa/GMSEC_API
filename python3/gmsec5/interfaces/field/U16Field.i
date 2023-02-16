/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module U16Field
%{
#include <gmsec5/field/U16Field.h>
using namespace gmsec::api5;
%}

%rename("get_value") getValue;
%rename("to_xml") toXML;
%rename("to_json") toJSON;

%include "dox/U16Field_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/U16Field.h>


%extend gmsec::api5::U16Field
{
    static U16Field* CALL_TYPE cast_field(Field* field)
    {
        U16Field* casted = dynamic_cast<U16Field*>(field);

        if (casted == NULL)
        {
            throw GmsecException(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a U16Field");
        }

        return casted;
    }
};
