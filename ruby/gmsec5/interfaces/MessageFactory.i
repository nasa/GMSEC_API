/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MessageFactory

%include <std_vector.i>

%{
#include <gmsec5/MessageFactory.h>
#include <vector>
using namespace gmsec::api5;
%}

/* We'll tell SWIG to ignore these and define our own implementation using %extend */
%ignore gmsec::api5::MessageFactory::MessageFactory();
%ignore gmsec::api5::MessageFactory::MessageFactory(const Config&);
%ignore gmsec::api5::MessageFactory::createMessage();
%ignore gmsec::api5::MessageFactory::createMessage(const char*);

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

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/MessageFactory.h>


namespace std
{
    %template(FieldArray) vector<gmsec::api5::Field*>;
}


%extend gmsec::api5::MessageFactory {

    static MessageFactory* CALL_TYPE create(const Config* config = NULL)
    {
        if (config == NULL) {
            return new MessageFactory();
        }
        return new MessageFactory(*config);
    }

    static void CALL_TYPE destroy(MessageFactory* factory)
    {
        delete factory;
    }

    Message CALL_TYPE createMessage(const char* schemaID = NULL) {
        if (schemaID == NULL) {
            return self->createMessage();
        }
        return self->createMessage(schemaID);
    }

    void CALL_TYPE setStandardFields(const std::vector<gmsec::api5::Field*>& fields) {
        gmsec::api5::util::List<gmsec::api5::Field*> list_fields;

        for (std::vector<gmsec::api5::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it) {
            list_fields.push_back(*it);
        }

        return self->setStandardFields(list_fields);
    }
}
