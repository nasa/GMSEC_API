/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MessageSpecification

%include "std_vector.i"

%{
#include <gmsec5/MessageSpecification.h>
#include <vector>
using namespace gmsec::api5;
%}


/* Functions containing lists that will be redefined */
%ignore gmsec::api5::MessageSpecification::getFieldSpecifications() const;

%rename("get_schema_id") getSchemaID;
%rename("get_subject_template") getSubjectTemplate;
%rename("get_field_specifications") getFieldSpecifications;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/MessageSpecification.h>


namespace std
{
    %template(FieldSpecificationArray) vector<gmsec::api5::FieldSpecification*>;
}


%extend gmsec::api5::MessageSpecification
{
    const std::vector<gmsec::api5::FieldSpecification*> CALL_TYPE getFieldSpecifications()
    {
        const gmsec::api5::util::List<gmsec::api5::FieldSpecification*>& fieldSpecs = self->getFieldSpecifications();

        std::vector<gmsec::api5::FieldSpecification*> newFieldSpecs;

        for (gmsec::api5::util::List<gmsec::api5::FieldSpecification*>::const_iterator it = fieldSpecs.begin(); it != fieldSpecs.end(); ++it)
        {
            newFieldSpecs.push_back(*it);
        }

        return newFieldSpecs;
    }
}

