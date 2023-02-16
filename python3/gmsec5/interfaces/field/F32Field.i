/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module F32Field
%{
#include <gmsec5/field/F32Field.h>
using namespace gmsec::api5;
%}

%rename("get_value") getValue;
%rename("to_xml") toXML;
%rename("to_json") toJSON;

%include "dox/F32Field_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/F32Field.h>


%extend gmsec::api5::F32Field
{
    static F32Field* CALL_TYPE cast_field(Field* field)
    {
        F32Field* casted = dynamic_cast<F32Field*>(field);

        if (casted == NULL)
        {
            throw GmsecException(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a F32Field");
        }

        return casted;
    }
};
