/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module StringField
%{
#include <gmsec5/field/StringField.h>
using namespace gmsec::api5;
%}

%rename("get_value") getValue;
%rename("to_xml") toXML;
%rename("to_json") toJSON;

%include "dox/StringField_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/StringField.h>


%extend gmsec::api5::StringField
{
    static StringField* CALL_TYPE cast_field(Field* field)
    {
        StringField* casted = dynamic_cast<StringField*>(field);

        if (casted == NULL)
        {
            throw GmsecException(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a StringField");
        }

        return casted;
    }
};
