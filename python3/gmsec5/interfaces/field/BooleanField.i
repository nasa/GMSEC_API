/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module BooleanField
%{
#include <gmsec5/field/BooleanField.h>
using namespace gmsec::api5;
%}

%rename("get_value") getValue;
%rename("to_xml") toXML;
%rename("to_json") toJSON;

%include "dox/BooleanField_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/BooleanField.h>


%extend gmsec::api5::BooleanField
{
    /* Obsolete as of API 5.1, but leave in place to preserve binary compatibility */
    static BooleanField* CALL_TYPE cast_field(Field* field)
    {
        BooleanField* casted = dynamic_cast<BooleanField*>(field);

        if (casted == NULL)
        {
            throw GmsecException(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a BooleanField");
        }

        return casted;
    }
};
