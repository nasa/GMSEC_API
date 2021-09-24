/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MessageSpecification

%include "std_vector.i"

%{
#include <gmsec4/mist/MessageSpecification.h>
#include <vector>
using namespace gmsec::api::mist;
%}


/* Functions containing lists that will be redefined */
%ignore gmsec::api::mist::MessageSpecification::getFieldSpecifications() const;

%rename("get_schema_id") getSchemaID;
%rename("get_field_specifications") getFieldSpecifications;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/MessageSpecification.h>


namespace std
{
    %template(FieldSpecificationArray) vector<gmsec::api::mist::FieldSpecification*>;
}


%extend gmsec::api::mist::MessageSpecification
{
    const std::vector<gmsec::api::mist::FieldSpecification*> CALL_TYPE getFieldSpecifications()
    {
        const gmsec::api::util::DataList<gmsec::api::mist::FieldSpecification*>& fieldSpecs = self->getFieldSpecifications();

        std::vector<gmsec::api::mist::FieldSpecification*> newFieldSpecs;

        for (gmsec::api::util::DataList<gmsec::api::mist::FieldSpecification*>::const_iterator it = fieldSpecs.begin(); it != fieldSpecs.end(); ++it)
        {
            newFieldSpecs.push_back(*it);
        }

        return newFieldSpecs;
    }
}

