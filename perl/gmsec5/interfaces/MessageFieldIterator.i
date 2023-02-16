/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MessageFieldIterator
%{
#include <gmsec5/MessageFieldIterator.h>
using namespace gmsec::api5;
%}

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/MessageFieldIterator.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::MessageFieldIterator

=head1 DESCRIPTION

The MessageFieldIterator supports iterating over the fields of a Message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br><br>

MessageFieldIterator is not thread safe.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Message.html#getfielditerator">Message::getFieldIterator()</a><br>

=head2 Public Types and Constants

=head3 Selector

=over

=item libgmsec_perl::MessageFieldIterator::Selector_ALL_FIELDS

Used for iterating over all available fields.

=item libgmsec_perl::MessageFieldIterator::Selector_HEADER_FIELDS

Used for iterating over only header fields.

=item libgmsec_perl::MessageFieldIterator::Selector_NON_HEADER_FIELDS

Used for iterating over only non-header fields.

=back

=head2 Public Member Subroutines

=head3 DESTROY

C<libgmsec_perl::MessageFieldIterator-E<gt>DESTROY()>

        Destructor

=head3 hasNext

C<libgmsec_perl::MessageFieldIterator-E<gt>hasNext()>

        Provides information as to whether there are additional fields that can be referenced using next().

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        1 (true) if additional fields are available, false otherwise.

=head3 next

C<libgmsec_perl::MessageFieldIterator-E<gt>next()>

        Returns a reference to the next available Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A reference to a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        A GmsecException is thrown if the iterator has reached the end (i.e. there are no more fields).

=head3 reset

C<libgmsec_perl::MessageFieldIterator-E<gt>reset()>

        Resets the interator to the beginning of the Field list that is maintained by the Message.

=cut
%}
