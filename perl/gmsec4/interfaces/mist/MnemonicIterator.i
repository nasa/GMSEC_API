/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MnemonicIterator
%{
#include <gmsec4/mist/MnemonicIterator.h>
using namespace gmsec::api::mist;
%}

%include<gmsec4/util/wdllexp.h>
%include <gmsec4/mist/MnemonicIterator.h>


%perlcode%{
=pod

=head1 NAME

libgmsec_perl::MnemonicIterator

The MnemonicIterator supports iterating over the Mnemonic objects of a MnemonicMessage.

Please note that MnemonicIterator is not thread safe.

A MnemonicIterator can be retreived from a MnemonicMessage by invoking MnemonicMessage::getMnemonicIterator().

=head2 Public Member Subroutines

=head3 DESTROY

C<libgmsec_perl::MnemonicIterator-E<gt>DESTROY()>

	Destructor

=head3 hasNext

C<libgmsec_perl::MnemonicIterator-E<gt>hasNext()>

Provides informaiton as to whether there are additional Mnemonic objects that can be referenced using next()

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	1 if additional fields are available, 0 otherwise

=head3  next

C<libgmsec_perl::MnemonicIterator-E<gt>next()>

Returns a reference to the next available Mnemonic object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A reference to the Mnemonic object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if the iterator has reached the end (i.e. there are no more Mnemonics)

=head3 reset

C<libgmsec_perl::MnemonicIterator-E<gt>reset()>

	Resets the iterator to the beginning of the Mnemonic list that is maintained by MnemonicMessage

=cut
%}
