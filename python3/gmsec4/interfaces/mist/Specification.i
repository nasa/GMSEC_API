/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") Specification

%{
#include <gmsec4/mist/Specification.h>
using namespace gmsec::api::mist;
%}

// Turn on director mapping for the Specification package
%feature("director") Specification;

%ignore gmsec::api::mist::Specification::getMessageSpecifications() const;

/* Ignore deprecated methods */
%ignore gmsec::api::mist::Specification::getTemplateXML(const char*, const char*);

/* Ignore C methods */
%ignore gmsec::api::mist::Specification::registerMessageValidator(GMSEC_MessageValidator*);

%rename("validate_message") validateMessage;
%rename("get_schema_id_iterator") getSchemaIDIterator;
%rename("get_version") getVersion;
%rename("get_schema_level") getSchemaLevel;
%rename("get_message_specifications") getMessageSpecifications;
%rename("register_message_validator") registerMessageValidator;

%include "dox/Specification_dox.i"
%include "dox/MessageSpecificationList_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/Specification.h>

%include <std_list.i>

namespace std
{
    %template(MessageSpecificationList) list<gmsec::api::mist::MessageSpecification*>;
}


%extend gmsec::api::mist::Specification {

    const std::list<gmsec::api::mist::MessageSpecification*> CALL_TYPE getMessageSpecifications()
    {
        const gmsec::api::util::DataList<gmsec::api::mist::MessageSpecification*>& msgSpecs = self->getMessageSpecifications();

        std::list<gmsec::api::mist::MessageSpecification*> newMsgSpecs;

        for (gmsec::api::util::DataList<gmsec::api::mist::MessageSpecification*>::const_iterator it = msgSpecs.begin(); it != msgSpecs.end(); ++it)
        {
            newMsgSpecs.push_back(*it);
        }

        return newMsgSpecs;
    }
}

