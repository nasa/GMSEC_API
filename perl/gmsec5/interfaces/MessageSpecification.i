/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MessageSpecification

%{
#include <gmsec5/MessageSpecification.h>
#include <vector>
using namespace gmsec::api5;
%}


/* Functions containing lists that will be redefined */
%ignore gmsec::api5::MessageSpecification::getFieldSpecifications() const;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/MessageSpecification.h>

%include <std_vector.i>

namespace std
{
    %template(FieldSpecificationList) vector<gmsec::api5::FieldSpecification*>;
}


%extend gmsec::api5::MessageSpecification {

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
    Accessor for the Message Specification schema ID.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
    The Message Specification schema ID as a string.


=head3 getSubjectTemplate

C<libgmsec_perl::MessageSpecification-E<gt>getSubjectTemplate()>
    Accessort for the Message Specification subject template.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
    The Message Specification subject template as a string.


=head3 getFieldSpecifications

C<libgmsec_perl::MessageSpecification-E<gt>getFieldSpecifications()>

    Accessor for acquiring a vector of FieldSpecification objects associated with the Message Specification.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
    A vector of FieldSpecification objects.

=cut
%}
