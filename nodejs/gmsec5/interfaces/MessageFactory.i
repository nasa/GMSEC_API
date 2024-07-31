/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MessageFactory

%include "std_vector.i"
%include "std_string.i"

%{
#include <gmsec5/MessageFactory.h>
#include <vector>
using namespace gmsec::api5;
%}

/* Ignore methods that are not supported or are pertinent to the C binding */
%ignore gmsec::api5::MessageFactory::registerMessageValidator(MessageValidator*);
%ignore gmsec::api5::MessageFactory::registerMessageValidator(GMSEC_MessageValidator*);

/* Methods requiring lists will be redefined */
%ignore gmsec::api5::MessageFactory::setStandardFields(const gmsec::api5::util::List<Field*>&);


%include <gmsec5/util/wdllexp.h>
%include <gmsec5/MessageFactory.h>


namespace std
{
    %template(FieldArray) vector<gmsec::api5::Field*>;
}


%extend gmsec::api5::MessageFactory {

    void CALL_TYPE setStandardFields(const std::vector<gmsec::api5::Field*>& fields) {
        gmsec::api5::util::List<gmsec::api5::Field*> list_fields;

        for (std::vector<gmsec::api5::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it) {
            list_fields.push_back(*it);
        }

        return self->setStandardFields(list_fields);
    }
}
