/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MistMessage
%{
#include <gmsec4/mist/message/MistMessage.h>
using namespace gmsec::api::mist::message;
%}

// Methods containing lists that will be redefined
%ignore gmsec::api::mist::message::MistMessage::setStandardFields(const gmsec::api::util::DataList<gmsec::api::Field*>&);

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/message/MistMessage.h>

%extend gmsec::api::mist::message::MistMessage
{
    static void CALL_TYPE setStandardFields(const std::list<gmsec::api::Field*>& standardFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for (std::list<gmsec::api::Field*>::const_iterator it = standardFields.begin(); it != standardFields.end(); ++it)
        {
            fields.push_back(*it);
        }

        MistMessage::setStandardFields(fields);
    }
}

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
    $schemaID - the string used to identify the message schema in C2MS or other message specification (e.g. MSG.HB).
    $spec - a reference to the specification to which this message schema will adhere.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if Specification fails to load the template directory or if schemaID is not a valid ID



C<libgmsec_perl::MistMessage-E<gt>new($subject, $schemaID, $config, $spec)>

    Initializes the message instance with a template determined by the ID and spec and an associated Configuration object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - the subject string for the message.
    $schemaID - the string used to identify the message schema in C2MS or other message specification (e.g. MSG.HB).
    $config - A Configuration object to associate with the message
    $spec - a reference to the specification to which this messages schema will adhere.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if Specification fails to load the template directory or if schemaID is not a valid ID



C<libgmsec_perl::MistMessage-E<gt>new($other)>

    Copy constructor - initializes the message instance using the other given MistMessage

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $other - The message to be copied

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if Specification fails to load the template directory or if schemaID is not a valid ID



C<libgmsec_perl::MistMessage-E<gt>new($msg)>

    Special constructor that construct a MistMessage using a simple Message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $msg - The simple message to reference while building the MistMessage

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if the simple Message does not house sufficient information to construct a MistMessage



C<libgmsec_perl::MistMessage-E<gt>new($msg, $config)>

    Special constructor that construct a MistMessage using a simple Message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $msg - The simple message to reference while building the MistMessage
    $config - A Configuration object to associate with the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if the simple Message does not house sufficient information to construct a MistMessage



C<libgmsec_perl::MistMessage-E<gt>new($data)>

    Initializes a MistMessage using an XML or JSON string.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $data - XML or JSON string used to initialize the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if the given data does not represent a valid XML or JSON statement.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

    This method has been deprecated.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;libgmsec_perl::MistMessage->new($spec, $data)



C<libgmsec_perl::MistMessage-E<gt>new($spec, $data)>

    Initializes a MistMessage using an XML or JSON string.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $spec - A reference to the specification to which this message will adhere
    $data - XML or JSON string used to initialize the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if the given data does not represent a valid XML or JSON statement.



=head3 DESTROY

C<libgmsec_perl::MistMessage-E<gt>DESTROY()>

    Destructor



=head3 setStandardFields

C<libgmsec_perl::MistMessage-E<gt>setStandardFields($standardFields)>

     Sets the internal list of fields which are to be automatically placed in all MistMessage objects that are created.  Internal copies of the Fields are made, and thus ownership of the fields that are provided are not retained by MistMessage.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $standardFields - A list of fields to be copied to the internal set of fields, which will in turn be included with all MistMessage objects that are created.



=head3 clearStandardFields

C<libgmsec_perl::MistMessage-E<gt>clearStandardFields()>

     Destroys the lists of standard fields that are included with MistMessage objects.



=head3 getSchemaID

C<libgmsec_perl::MistMessage-E<gt>getSchemaID()>

    Returns a string that identifies the schema that the message is based off of

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    The schema ID string



=head3 setValue

C<libgmsec_perl::MistMessage-E<gt>setValue($fieldName, $value)>

    Sets the value for the field indicated by the user. The value type is automatically determined by the function depending on the message schema being used. If the field does not have a required type, a new StringField will be created with the specified name and value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $fieldName - the name of the field to be modified/created
    $value - the value of the field. The values type is automatically determined based on the message schema.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if the field name is NULL ot contains an empty string
    Exception if the inputs value cannot be converted into the type required by the field, or if the string is too big when converting to char.

=cut
%}
