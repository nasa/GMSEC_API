/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


%module FieldSpecification

%{
#include <gmsec4/mist/FieldSpecification.h>
using namespace gmsec::api::mist;
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/FieldSpecification.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::FieldSpecification

This class is a container for holding information on a Field Specification

See also:
MessageSpecification
Specification

=head2 Public Member Subroutines

=head3 getName

C<libgmsec_perl::FieldSpecification-E<gt>getName()>
    Returns the name of the FieldSpecification

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
    The FieldSpecification name as a string


=head3 getType

C<libgmsec_perl::FieldSpecification-E<gt>getType()>
    Returns the FieldSpecification type (e.g. STRING, I16, etc.)

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
    The FieldSpecification type as a string


=head3 getMode

C<libgmsec_perl::FieldSpecification-E<gt>getMode()>
    Returns the FieldSpecification mode (e.g. REQUIRED, OPTIONAL, etc.)

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
    The FieldSpecification mode as a string


=head3 getClassification

C<libgmsec_perl::FieldSpecification-E<gt>getClassification()>
    Returns the FieldSpecification classification

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
    The FieldSpecification classification as a string


=head3 getValue

C<libgmsec_perl::FieldSpecification-E<gt>getValue()>
    Returns the FieldSpecification default value, if any

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
    The FieldSpecification default value as a string, or an empty string if no value is available


=head3 getDescription

C<libgmsec_perl::FieldSpecification-E<gt>getDescription()>
    Returns the FieldSpecification description

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
    The FieldSpecification description as a string


=cut
%}
