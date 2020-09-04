/*
 * Copyright 2007-2017 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MistMessage
%{
#include <gmsec4/mist/message/MistMessage.h>
using namespace gmsec::api::mist::message;
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/message/MistMessage.h>


%perlcode%{
=pod

=head1 NAME

libgmsec_perl::MistMessage

	The Message object shares all the functionality of standard messages, but can also be auto-populated based on how the message schema identifies itself within the given specification.

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::MistMessage-E<gt>new($subject, $schemaID, $spec)>

	Default constructor - Initializes the message instance with a template determined by the ID and spec

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$subject - the subject string for the message.
	$schemaID - the string used to identify the message schema in the GMSEC ISD.  The schema ID has the format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype (e.g. 2016.00.GMSEC.MSG.C2CX.HB).  Note that not all schemaIDs contain a messageSubtype
	$spec - a reference to the specification this message's schema will adhere to.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if Specification fails to load the template directory or if schemaID is not a valid ID	

C<libgmsec_perl::MistMessage-E<gt>new($subject, $schemaID, $config, $spec)>

	Initializes the message instance with a template determined by the ID and spec and an associated Configuration object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$subject - the subject string for the message.
	$schemaID - the string used to identify the message schema in the GMSEC ISD.  The schema ID has the format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype (e.g. 2016.00.GMSEC.MSG.C2CX.HB).  Note that not all schemaIDs contain a messageSubtype
	$config - A Configuration object to associate with the message
	$spec - a reference to the specification this message's schema will adhere to.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if Specification fails to load the template directory or if schemaID is not a valid ID

=head3 DESTROY

C<libgmsec_perl::MistMessage-E<gt>DESTROY()>

    Destructor

=head3 getSchemaID

C<libgmsec_perl::MistMessage-E<gt>getSchemaID()>

	Returns a string that identifies the schema that the message is based off of

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	The schema ID string

=head3 setValue

C<libgmsec_perl::MistMessage-E<gt>setValue($fieldName, $value)>

	Sets the value for the field indicated by the user.  The value's type is automatically determined by the function depending on the message schema being used.  If the field does not have a required type, a new StringField will be created with the specified name and value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$fieldName - the name of the field to be modified/created
	$value - the value of the field.  The value's type is automatically determined based on the message schema.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if the field name is NULL ot contains an empty string
	Exception if the input's value cannot be converted into the type required by the field, or if the string is too big when converting to char.

=cut
%}
