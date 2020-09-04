/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Message
%{
#include <gmsec4/Message.h>
#include <list>
using namespace gmsec::api;
%}

/* Functions containing lists that will be redefined */
%ignore gmsec::api::Message::addFields(const gmsec::api::util::DataList<Field*>&);
%ignore gmsec::api::Message::operator=(const Message&);
%ignore gmsec::api::Message::addField(const char*, const GMSEC_BIN, size_t);


%include "dox/Message_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Message.h>

%include <std_list.i>


%extend gmsec::api::Message
{
    bool CALL_TYPE addFields(const std::list<gmsec::api::Field*>& fields) {
        gmsec::api::util::DataList<gmsec::api::Field*> dl_fields;

        for (std::list<gmsec::api::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it) {
            dl_fields.push_back(*it);
        }

        return self->addFields(dl_fields);
    }

    bool CALL_TYPE addField(char* name, PyObject* data, size_t length)
    {
        char* blob = PyByteArray_AsString(data);

        return self->addField((const char*) name, (const GMSEC_BIN) blob, length);
    }
}
