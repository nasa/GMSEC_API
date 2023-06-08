/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module I8Field
%{
#include <gmsec5/field/I8Field.h>
using namespace gmsec::api5;
%}

%rename("get_value") getValue;
%rename("to_xml") toXML;
%rename("to_json") toJSON;

%include "dox/I8Field_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/I8Field.h>


%extend gmsec::api5::I8Field
{
    /* Obsolete as of API 5.1, but leave in place to preserve binary compatibility */
    static I8Field* CALL_TYPE cast_field(Field* field)
    {
        I8Field* casted = dynamic_cast<I8Field*>(field);

        if (casted == NULL)
        {
            throw GmsecException(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a I8Field");
        }

        return casted;
    }
};
