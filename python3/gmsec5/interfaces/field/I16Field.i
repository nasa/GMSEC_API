/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module I16Field
%{
#include <gmsec5/field/I16Field.h>
using namespace gmsec::api5;
%}

%rename("get_value") getValue;
%rename("to_xml") toXML;
%rename("to_json") toJSON;

%include "dox/I16Field_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/I16Field.h>


%extend gmsec::api5::I16Field
{
    /* Obsolete as of API 5.1, but leave in place to preserve binary compatibility */
    static I16Field* CALL_TYPE cast_field(Field* field)
    {
        I16Field* casted = dynamic_cast<I16Field*>(field);

        if (casted == NULL)
        {
            throw GmsecException(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a I16Field");
        }

        return casted;
    }
};
