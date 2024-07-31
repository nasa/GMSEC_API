/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Message

%include "std_vector.i"
%include "std_string.i"


%{
#include <gmsec5/Specification.h>
#include <gmsec5/Message.h>
#include <vector>
using namespace gmsec::api5;
%}

/* Ignore methods that are not supported or are pertinent to the C binding */
%ignore gmsec::api5::Message::registerMessageValidator(MessageValidator*);
%ignore gmsec::api5::Message::registerMessageValidator(GMSEC_MessageValidator*);

/* Not needed for the JavaScript binding */
%ignore gmsec::api5::Message::destroy(Message*&);

/* Ignore method that cannot be translated to JavaScript */
%ignore gmsec::api5::Message::operator=(const Message&);

/* ignore overloaded addField() methods since many use types not applicable to JavaScript */
%ignore gmsec::api5::Message::addField(const char*, GMSEC_BIN, size_t);
%ignore gmsec::api5::Message::addField(const char*, bool);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_CHAR);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_F32);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_F64);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_I8);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_I16);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_I32);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_I64);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_U8);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_U16);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_U32);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_U64);
%ignore gmsec::api5::Message::addField(const char*, const char*);

/* Ignore methods that do not make sense in the JavaScript context */
%ignore gmsec::api5::Field::getI16Value();
%ignore gmsec::api5::Field::getI32Value();
%ignore gmsec::api5::Field::getU16Value();
%ignore gmsec::api5::Field::getU32Value();
%ignore gmsec::api5::Field::getU64Value();

/* Methods containing lists will be redefined */
%ignore gmsec::api5::Message::addFields(const gmsec::api5::util::List<Field*>&);

/* Conjure names for methods that allow for the acquisition of field values */
%rename("getIntegerValue") getI64Value;
%rename("getDoubleValue") getF64Value;


%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Specification.h>
%include <gmsec5/Message.h>


namespace std
{
    %template(FieldArray) vector<gmsec::api5::Field*>;
}


%extend gmsec::api5::Message {

    bool CALL_TYPE addFields(const std::vector<gmsec::api5::Field*>& fields) {
        gmsec::api5::util::List<gmsec::api5::Field*> list_fields;

        for (std::vector<gmsec::api5::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it) {
            list_fields.push_back(*it);
        }

        return self->addFields(list_fields);
    }
}
