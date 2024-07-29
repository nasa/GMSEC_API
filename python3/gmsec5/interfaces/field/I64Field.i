/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module I64Field
%{
#include <gmsec5/field/I64Field.h>
using namespace gmsec::api5;
%}

%rename("get_value") getValue;
%rename("to_xml") toXML;
%rename("to_json") toJSON;

%include "dox/I64Field_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/I64Field.h>


%extend gmsec::api5::I64Field
{
    /* Obsolete as of API 5.1, but leave in place to preserve binary compatibility */
    static I64Field* CALL_TYPE cast_field(Field* field)
    {
        I64Field* casted = dynamic_cast<I64Field*>(field);

        if (casted == NULL)
        {
            throw GmsecException(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a I64Field");
        }

        return casted;
    }
};
