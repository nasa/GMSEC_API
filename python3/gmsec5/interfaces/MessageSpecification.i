/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MessageSpecification

%{
#include <gmsec5/MessageSpecification.h>
#include <list>
using namespace gmsec::api5;
%}


/* Functions containing lists that will be redefined */
%ignore gmsec::api5::MessageSpecification::getFieldSpecifications() const;

%rename("get_schema_id") getSchemaID;
%rename("get_subject_template") getSubjectTemplate;
%rename("get_field_specifications") getFieldSpecifications;

%include "dox/MessageSpecification_dox.i"
%include "dox/FieldSpecificationList_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/MessageSpecification.h>

%include <std_list.i>

namespace std
{
    %template(FieldSpecificationList) list<gmsec::api5::FieldSpecification*>;
}


%extend gmsec::api5::MessageSpecification {

    const std::list<gmsec::api5::FieldSpecification*> CALL_TYPE getFieldSpecifications()
    {
        const gmsec::api5::util::List<gmsec::api5::FieldSpecification*>& fieldSpecs = self->getFieldSpecifications();

        std::list<gmsec::api5::FieldSpecification*> newFieldSpecs;

        for (gmsec::api5::util::List<gmsec::api5::FieldSpecification*>::const_iterator it = fieldSpecs.begin(); it != fieldSpecs.end(); ++it)
        {
            newFieldSpecs.push_back(*it);
        }

        return newFieldSpecs;
    }
}

