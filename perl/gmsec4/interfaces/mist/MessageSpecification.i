/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MessageSpecification

%{
#include <gmsec4/mist/MessageSpecification.h>
#include <vector>
using namespace gmsec::api::mist;
%}


/* Functions containing lists that will be redefined */
%ignore gmsec::api::mist::MessageSpecification::getFieldSpecifications() const;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/MessageSpecification.h>

%include <std_vector.i>

namespace std
{
    %template(FieldSpecificationList) vector<gmsec::api::mist::FieldSpecification*>;
}


%extend gmsec::api::mist::MessageSpecification {

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


%perlcode%{
=pod

=head1 NAME

libgmsec_perl::MessageSpecification

This class is a container for holding information on a Message Specification

See also:
FieldSpecification
Specification

=head2 Public Member Subroutines

=head3 getSchemaID

C<libgmsec_perl::MessageSpecification-E<gt>getSchemaID()>
    Accessor for the Message Specification schema ID

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
    The Message Specification schema ID as a string


=head3 getFieldSpecifications

C<libgmsec_perl::MessageSpecification-E<gt>getFieldSpecifications()>

    Accessor for acquiring a vector of FieldSpecification objects associated with the Message Specification

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
    A vector of FieldSpecification objects

=cut
%}
