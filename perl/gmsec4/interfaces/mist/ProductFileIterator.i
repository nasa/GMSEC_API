/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ProductFileIterator
%{
#include <gmsec4/mist/ProductFileIterator.h>
using namespace gmsec::api::mist;
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ProductFileIterator.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::ProductFileIterator

This class supports iterating over the ProductFile objects of a ProductFileMessage.

Note that ProductFileIterator is not thread safe.

A ProductFileIterator can be retreived from a ProductFileMessage by invoking ProductFileMessage::getProductFileIterator().

=head2 Public Member Subroutines

=head3 DESTROY

C<libgmsec_perl::ProductFileIterator-E<gt>DESTROY()>

	Destructor

=head3 hasNext

C<libgmsec_perl::ProductFileIterator-E<gt>hasNext()>

Provides informaiton as to whether there are additional ProductFile objects that can be referenced using next()

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	1 if additional fields are available, 0 otherwise

=head3  next

C<libgmsec_perl::ProductFileIterator-E<gt>next()>

Returns a reference to the next available ProductFile object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A reference to the ProductFile object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if the iterator has reached the end (i.e. there are no more ProductFiles)

=head3 reset

C<libgmsec_perl::ProductFileIterator-E<gt>reset()>

	Resets the iterator to the beginning of the ProductFile list that is maintained by ProductFileMessage

=cut
%}
