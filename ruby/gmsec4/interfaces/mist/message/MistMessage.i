/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MistMessage

%include "std_vector.i"

%{
#include <gmsec4/mist/message/MistMessage.h>
using namespace gmsec::api::mist::message;
%}

/* Ignore deprecated methods */
%ignore gmsec::api::mist::message::MistMessage::MistMessage(const char*);

/* Methods containing lists that will be redefined */
%ignore gmsec::api::mist::message::MistMessage::setStandardFields(const gmsec::api::util::DataList<gmsec::api::Field*>&);

%rename("set_standard_fields") setStandardFields;
%rename("clear_standard_fields") clearStandardFields;
%rename("get_schema_id") getSchemaID;
%rename("set_value") setValue;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/message/MistMessage.h>


%extend gmsec::api::mist::message::MistMessage
{
    static void CALL_TYPE setStandardFields(const std::vector<gmsec::api::Field*>& standardFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for (std::vector<gmsec::api::Field*>::const_iterator it = standardFields.begin(); it != standardFields.end(); ++it)
        {
            fields.push_back(*it);
        }

        MistMessage::setStandardFields(fields);
    }
}

