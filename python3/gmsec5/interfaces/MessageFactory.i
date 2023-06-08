/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MessageFactory

%{
#include <gmsec5/MessageFactory.h>
#include <list>
using namespace gmsec::api5;
%}

/* Ignore methods pertinent to the C binding */
%ignore gmsec::api5::MessageFactory::registerMessageValidator(GMSEC_MessageValidator*);

/* Functions containing lists that will be redefined */
%ignore gmsec::api5::MessageFactory::setStandardFields(const gmsec::api5::util::List<Field*>&);

%rename("set_standard_fields") setStandardFields;
%rename("clear_standard_fields") clearStandardFields;
%rename("set_message_config") setMessageConfig;
%rename("create_message") createMessage;
%rename("from_data") fromData;
%rename("get_specification") getSpecification;
%rename("register_message_validator") registerMessageValidator;

%include "dox/MessageFactory_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/MessageFactory.h>


%include <std_list.i>

namespace std
{
    %template(FieldList) list<gmsec::api5::Field*>;
}


%extend gmsec::api5::MessageFactory {

    void CALL_TYPE setStandardFields(const std::list<gmsec::api5::Field*>& fields) {
        gmsec::api5::util::List<gmsec::api5::Field*> list_fields;

        for (std::list<gmsec::api5::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it) {
            list_fields.push_back(*it);
        }

        return self->setStandardFields(list_fields);
    }
}
