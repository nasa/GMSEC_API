/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module SchemaIDIterator
%{
#include <gmsec4/mist/SchemaIDIterator.h>
using namespace gmsec::api::mist;
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/SchemaIDIterator.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::SchemaIDIterator

This class supports the iteration of Specification's loaded schema templates.

A SchemaIDIterator can be retreived from a Specification by invoking Specification::getSchemaIDIterator().

=head2 Public Member Subroutines

=head3 DESTROY

C<libgmsec_perl::SchemaIDIterator-E<gt>DESTROY()>

	Destructor

=head3 hasNext

C<libgmsec_perl::SchemaIDIterator-E<gt>hasNext()>

Provides information as to whether there are additional Schema IDs that can be referenced using next()

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	1 if additional fields are available, 0 otherwise

=head3  next

C<libgmsec_perl::SchemaIDIterator-E<gt>next()>

Returns a reference to the next available Schema IDs

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A schema ID string

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if the iterator has reached the end (i.e. there are no more Schema IDs)

=head3 reset

C<libgmsec_perl::SchemaIDIterator-E<gt>reset()>

	Resets the iterator to the beginning of the Schema list that is maintained by Specification

=cut
%}
