/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MnemonicMessage
%{
#include <gmsec4/mist/message/MnemonicMessage.h>
using namespace gmsec::api::mist::message;
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/message/MnemonicMessage.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::MnemonicMessage

	A MnemonicMessage object that is capable of storing Mnemonic objects.

	The following message schema IDs and their templates are supported:
	GMSEC.MSG.MVAL
	GMSEC.REQ.MVAL
	GMSEC.RESP.MVAL

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::MnemonicMessage-E<gt>new($subject, $schemaID, $spec)>

	Default constructor - Initializes the message instance with a template determined by the ID and spec

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$subject - the subject string for the message.
	$schemaID - the string used to identify the message schema in the GMSEC ISD.  The schema ID has the format of: major.minor.schemaLevelName.messageKind.messageType (e.g. 2016.00.GMSEC.MSG.MVAL).  You may also use the shorthand notation of messageKind.messageType (e.g. MSG.MVAL)
	$spec - a reference to the specification this message's schema will adhere to.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if Specification fails to load the template directory or if schemaID is not a valid ID	

C<libgmsec_perl::MnemonicMessage-E<gt>new($subject, $schemaID, $config, $spec)>

	Initializes the message instance with a template determined by the ID and spec and an associated Configuration object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$subject - the subject string for the message.
	$schemaID - the string used to identify the message schema in the GMSEC ISD.  The schema ID has the format of: major.minor.schemaLevelName.messageKind.messageType (e.g. 2016.00.GMSEC.MSG.MVAL). You may also use the shorthand notation messageKind.messageType (e.g. MSG.MVAL)
	$config - A Configuration object to associate with the message
	$spec - a reference to the specification this message's schema will adhere to.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if Specification fails to load the template directory or if schemaID is not a valid ID

=head3 DESTROY

C<libgmsec_perl::MnemonicMessage-E<gt>DESTROY()>

    Destructor

=head3 addMnemonic

C<libgmsec_perl::MnemonicMessage-E<gt>addMnemonic($mnemonic)>

	Adds a Mnemonic to the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$mnemonic - a Mnemonic object to attach to the message

=head3 getNumMnemonics

C<libgmsec_perl::MnemonicMessage-E<gt>getNumMnemonics()>

	Gets the number of Mnemonics in the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	The number of Mnemonic objects attached ot the message

=head3 getMnemonic

C<libgmsec_perl::MnemonicMessage-E<gt>getMnemonic($index)>

	Returns a reference to a Mnemonic object attached to the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$index - The zero-based index of the Mnemonic object to access

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A Mnemonic object at the specified index inside of MnemonicMessage

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if the index specified is not in the range of Mnemonics in the message

=head3 getMnemonicIterator

C<libgmsec_perl::Mnemonicmessage-E<gt>getMnemonicIterator()>

	Method that allows the callee to get the MnemonicIterator associated with the MnemonicMessage.  This iterator will allow for applications to iterate over the Mnemonic objects stored within the MnemonicMessage.  The Iterator is reset each time getMnemonicIterator() is called.  The iterator itself is destroyed when the MnemonicMessage object is destroyed.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

	Only one MnemonicIterator object is associated with a MnemonicMessage object; multiple calls to getMnemonicIteraotr() will return a reference to the same MnemonicIterator object.  Each call will reset the iterator.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A reference to a MnemonicIterator object

=head3 convertMessage

C<libgmsec_perl::MnemonicMessage-E<gt>convertMessage($message)>

	Constructs and returns a MnemonicMessage from an ordinary Message object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$message - the Message object to be converted to a MnemonicMessage

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A new MnemonicMessage constructed using the data from the given Message object

=cut
%}
