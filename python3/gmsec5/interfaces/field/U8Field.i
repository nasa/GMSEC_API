/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module U8Field
%{
#include <gmsec5/field/U8Field.h>
using namespace gmsec::api5;
%}

%rename("get_value") getValue;
%rename("to_xml") toXML;
%rename("to_json") toJSON;

%include "dox/U8Field_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/U8Field.h>


%extend gmsec::api5::U8Field
{
    /* Obsolete as of API 5.1, but leave in place to preserve binary compatibility */
    static U8Field* CALL_TYPE cast_field(Field* field)
    {
        U8Field* casted = dynamic_cast<U8Field*>(field);

        if (casted == NULL)
        {
            throw GmsecException(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a U8Field");
        }

        return casted;
    }
};
