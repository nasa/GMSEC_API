/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module CharField
%{
#include <gmsec5/field/CharField.h>
using namespace gmsec::api5;
%}

%rename("get_value") getValue;
%rename("to_xml") toXML;
%rename("to_json") toJSON;

%include "dox/CharField_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/CharField.h>


%extend gmsec::api5::CharField
{
    /* Obsolete as of API 5.1, but leave in place to preserve binary compatibility */
    static CharField* CALL_TYPE cast_field(Field* field)
    {
        CharField* casted = dynamic_cast<CharField*>(field);

        if (casted == NULL)
        {
            throw GmsecException(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to a CharField");
        }

        return casted;
    }
};
