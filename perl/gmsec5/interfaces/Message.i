/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Message

%{
#include <gmsec5/Message.h>
#include <vector>
using namespace gmsec::api5;
%}

/* functions containing lists will be redefined */
%ignore gmsec::api5::Message::addFields(const gmsec::api5::util::List<Field*>&);

/* We'll tell SWIG to ignore them and define our own implementation using %extend */
%ignore gmsec::api5::Message::destroy(Message*&);

/* ignore overloaded addField() methods since many use types not applicable to Perl */
%ignore gmsec::api5::Message::addField(const char*, GMSEC_BIN, size_t);
%ignore gmsec::api5::Message::addField(const char*, bool);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_CHAR);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_F32);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_F64);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_I8);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_I16);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_I32);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_I64);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_U8);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_U16);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_U32);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_U64);
%ignore gmsec::api5::Message::addField(const char*, const char*);

/* ignore methods that do not make sense in the Perl context */
%ignore gmsec::api5::Field::getI16Value();
%ignore gmsec::api5::Field::getI32Value();
%ignore gmsec::api5::Field::getU16Value();
%ignore gmsec::api5::Field::getU32Value();
%ignore gmsec::api5::Field::getU64Value();

%rename("getIntegerValue") getI64Value;
%rename("getDoubleValue")  getF64Value;


/* not supported */
%ignore gmsec::api5::Message::operator=(const Message&);

%ignore gmsec::api5::Specification::getMessageSpecifications() const;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Specification.h>
%include <gmsec5/Message.h>


%extend gmsec::api5::Message {

    bool CALL_TYPE addFields(const std::vector<gmsec::api5::Field*>& fields) {
        gmsec::api5::util::List<gmsec::api5::Field*> list_fields;

        for (std::vector<gmsec::api5::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it) {
            list_fields.push_back(*it);
        }

        return self->addFields(list_fields);
    }

    static void CALL_TYPE destroy(Message* msg)
    {
        Message::destroy(msg);
    }
}


%perlcode%{
=pod

=head1 NAME

libgmsec_perl::Message

=head1 DESCRIPTION

The Message object is a container for GMSEC Messages. The methods of this class allow the construction and manipulation of the data in a message.

=head2 Public Types and Constants

=head3 ResponseStatus

=over

=item libgmsec_perl::Message::ACKNOWLEDGEMENT

Basic acknowledgement

=item libgmsec_perl::Message::WORKING_KEEP_ALIVE

Still working on operation

=item libgmsec_perl::Message::SUCCESSFUL_COMPLETION

Operation successfully completed

=item libgmsec_perl::Message::FAILED_COMPLETION

Operation failed to complete

=item libgmsec_perl::Message::INVALID_REQUEST

The request for an operation was invalid

=item libgmsec_perl::Message::FINAL_MESSAGE

This message is the last in a set of responses

=back

=head3 Kind

=over

=item libgmsec_perl::Message::PUBLISH

For messages that will be published

=item libgmsec_perl::Message::REQUEST

For request messages

=item libgmsec_perl::Message::REPLY

For reply messages

=back

=head2 Public Static Subroutines

=head3 destroy

C<libgmsec_perl::Message::destroy($message)>

        This static method will destroy a Message object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $message - message to be destroyed

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException on error destroying Message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#receive">receive()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#request">request()</a><br>


=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::Message-E<gt>new()>

        Default constructor - initializes the message object.

        Example:

        # Create a Message object
        my $message = libgmsec_perl::Message->new();

        # You can add Fields to the message with addField(),
        my $stringField1 = libgmsec_perl::StringField->new("STRING-FIELD-NAME-1", "Example text");
        my $stringField2 = libgmsec_perl::StringField->new("STRING-FIELD-NAME-2", "Example text");
        $message->addField($stringField1);
        $message->addField($stringField2);

        my $config = libgmsec_perl::Config->new();

        # Split arguments and add them to the Config
        foreach my $arg (@args)
        {
                #print "$arg\n";
                my @argVals = split /=/, $arg;
                $config->addValue($argVals[0], $argVals[1]);
        }

        # Create a Connection
        eval
        {
            my $conn = libgmsec_perl::Connection($config);

            # Establish the connection
            $conn->connect();

            ...

            # Publish the message
            $conn->publish($message);

            ...

            # Disconnect from the middleware server
            $conn->disconnect();
        };
        if ($@)
        {
            # Handle error
        }


=head3 acknowledge

C<libgmsec_perl::Message<gt>acknowledge()>

        Acknowledges the processing of the message that has been received from the GMSEC Bus.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        Some middleware can retain a delivered message in a queue until said time
        the message has been acknowledged as being processed. Should the message not be
        acknowledged, the middleware can deliver the message to another subscriber client.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#receive">receive()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#request">request()</a><br>


=head3 getSchemaID

C<libgmsec_perl::Message-E<gt>getSchemaID()>

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns a string that identifies (if known) the schema from which the message is based.


=head3 getVersion

C<libgmsec_perl::Message-E<gt>getVersion()>

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the specification version number that was used to create the message.


=head3 getSchemaLevel

C<libgmsec_perl::Message-E<gt>getSchemaLevel()>

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the specification schema level that was used to create the message.


=head3 isCompliant

C<libgmsec_perl::Message-E<gt>isCompliant()>

        The contents of the message are compared to the template to ensure the message is in compliance with the message specification.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Status object to indicate whether the message is valid. If invalid, a list of errors will be provided of issues found within the message.


=head3 registerMessageValidator

C<libgmsec_perl::Message-E<gt>registerMessageValidator($validator)>

        Registers the given message validator to use when the message is checked for compliance.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $validator - A custom MessageValidator object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

        libgmsec_perl::MessageValidator


=head3 setFieldValue

C<libgmsec_perl::Message-E<gt>setFieldValue($name, $value)>

        Adds/replaces a field with the given name and value to the message. If
        a field template for the schema in use is available for the named field, then
        the value type will be coerced to match that which is described in the field
        template.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        This method is not very efficient; if the type of the field is known,
        consider calling addField() instead.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name  - The name of the field.
        $value - The value to associated with the field. Value can be string, integer, or double.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns true if the field was replaced; false otherwise.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>
        $libgmsec_perl::GmsecException is thrown if the field name is null, is an empty string, or is otherwise non-compliant.
        $libgmsec_perl::GmsecException is thrown if the field value type cannot be converted into the type required by the field, or if the string is too big when converting to a char.


=head3 setConfig

C<libgmsec_perl::Message-E<gt>setConfig($config)>

        This function adds configuration items to the message. This can be used to pass middleware specific items to the Message object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $config - Config that contains configuration values to add to the Message.


=head3 getConfig

C<libgmsec_perl::Message-E<gt>getConfig()>

        Returns the configuration associcated with the message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A reference to the Config object associated with the Message.


=head3 setSubject

C<libgmsec_perl::Message-E<gt>setSubject($subject)>

        Allows for the setting/changing of the message subject.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $subject - the subject/topic to assign to the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException is thrown if the subject is NULL or contains an empty-string


=head3 setSubjectElement

C<libgmsec_perl::Message-E<gt>setSubjectElement($name, $value)>

        Allows for the setting of individual subject elements. The name of the elements are defined
		by the corresponding message template. This value will be overridden by automatic subject
		generation if the subject element is defined by an existing field in the message, or if the 
		subject was manually defined with setSubject.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - the name of the subject element

        $value - the value of the subject element. An empty or null value will be seen as FILL in the subject line.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException is thrown if the name is NULL, an empty string, or does not match 
		a subject element name defined the message template, or if the message does not have a corresponding 
		message template.


=head3 getSubject

C<libgmsec_perl::Message-E<gt>getSubject()>

        This function get the subject this message will be/was published upon.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        The subject assigned to the Message.


=head3 setKind

C<libgmsec_perl::Message-E<gt>setKind($kind)>

        This function sets the message kind. Note The API can auto-deduce the message kind, but there may be cases where
        this method may be useful. For most common Use Cases, this method should not need to be called.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $kind - the message kind


=head3 getKind

C<libgmsec_perl::Message-E<gt>getKind()>

        This function returns the message kind.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        The message kind assigned to the Message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Message.html#kind">Kind</a><br>


=head3 addField

C<libgmsec_perl::Message-E<gt>addField($field)>

        This function will add a new field to this message. A new field will be added to the message that matches the field passed in. Therefore the field is owned by the client program and can be reused. The client program must also clean up that field when no longer needed.

        Example:

        my $field = libgmsec_perl::I32Field("MyField", 1234);
        my $replaced = $msg.addField($field);

        if ($replaced)
        {
            # Field was replaced
        }
        else
        {
           # New Field added
        }

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - Field object to copy info from

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns 1 (true) if the Field is replacing one with the same name; 0 (false) otherwise.


=head3 addFields

C<libgmsec_perl::Message-E<gt>addFields($fields)>

        This function will add the fields in the provided list to the message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $fields - the list of Field objects

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
        Returns true if any existing field in the Message has been replaced; false otherwise.


=head3 clearFields

C<libgmsec_perl::Message-E<gt>clearFields()>

        This function will remove all fields form this message.


=head3 clearField

C<libgmsec_perl::Message-E<gt>clearField($name)>

        This function will remove a field from this message by name.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of the Field to remove

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns true if the field was removed; false otherwise.


=head3 hasField

C<libgmsec_perl::Message-E<gt>hasField($fieldName)>

        Reports whether the field, identified by name, is present in the Message or not.


=head3 getStringValue

C<libgmsec_perl::Message-E<gt>getStringValue($fieldName)>

        Attempts to convert the field value into string representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $fieldName - the name of the field from which to reference the value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the field value as a string value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the field cannot be found, or if it cannot successfully be converted to a string.


=head3 getBooleanValue

C<libgmsec_perl::Message-E<gt>getBooleanValue($fieldName)>

        Attempts to convert the field value into boolean representation. Any non-zero value will be interpreted as true, otherwise as false.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $fieldName - the name of the field from which to reference the value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the field value as a boolean value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the field cannot be found, or if it cannot successfully be converted to a boolean.


=head3 getIntegerValue

C<libgmsec_perl::Message-E<gt>getIntegerValue($fieldName)>

        Attempts to convert the field value into a signed 64-bit integer number representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $fieldName - the name of the Field from which to reference the value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the Field value as an integer value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the Field cannot be found, or if it cannot successfully be converted to an integer.


=head3 getDoubleValue

C<libgmsec_perl::Message-E<gt>getDoubleValue($fieldName)>

        Attempts to convert the field value into an 64-bit floating-point number representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - the name of the field from which to reference the value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the field value as a floating-point value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the field cannot be found, or if it cannot successfully be converted to a floating-point number.


=head3 getField

C<libgmsec_perl::Message-E<gt>getField($fieldName)>

        This function will return the named Field object contained within the Message object, if it exists. Otherwise a NULL pointer is returned.

        Example:

        my $field = $message->getField("BOOL-FIELD");

        if ($field)
        {
            if ($field->getFieldType() == $libgmsec_perl::Field::BOOL_TYPE)
            {
                my $bool = $field->getIntegerValue();

                # Do something with the retrieved value
            }
        }
        else
        {
            # Handle error
        }

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of field to get

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        If found, a pointer to the field; otherwise NULL.


=head3 getFieldType

C<libgmsec_perl::Message-E<gt>getFieldType($name)>

        Returns the FieldType associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of field to get type for

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        FieldType as defined in Field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field cannot be found.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Field.html#fieldtype">FieldType</a><br>


=head3 getBinaryField

C<libgmsec_perl::Message-E<gt>getBinaryField($name)>

        Returns a reference to the BinaryField associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A refences to a BinaryField object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field is NOT a BinaryField or cannot be found.


=head3 getBooleanField

C<libgmsec_perl::Message-E<gt>getBooleanField($name)>

        Returns a reference to the BooleanField associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A refences to a BooleanField object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field is NOT a BooleanField or cannot be found.


=head3 getCharField

C<libgmsec_perl::Message-E<gt>getCharField($name)>

        Returns a reference to the CharField associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A refences to a CharField object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field is NOT a CharField or cannot be found.


=head3 getF32Field

C<libgmsec_perl::Message-E<gt>getF32Field($name)>

        Returns a reference to the F32Field associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A refences to a F32Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field is NOT a F32Field or cannot be found.


=head3 getF64Field

C<libgmsec_perl::Message-E<gt>getF64Field($name)>

        Returns a reference to the F64Field associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A refences to a F64Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field is NOT a F64Field or cannot be found.


=head3 getI8Field

C<libgmsec_perl::Message-E<gt>getI8Field($name)>

        Returns a reference to the I8Field associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A refences to a I8Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field is NOT a I8Field or cannot be found.


=head3 getI16Field

C<libgmsec_perl::Message-E<gt>getI16Field($name)>

        Returns a reference to the I16Field associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A refences to a I16Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field is NOT a I16Field or cannot be found.


=head3 getI32Field

C<libgmsec_perl::Message-E<gt>getI32Field($name)>

        Returns a reference to the I32Field associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A refences to a I32Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field is NOT a I32Field or cannot be found.


=head3 getI64Field

C<libgmsec_perl::Message-E<gt>getI64Field($name)>

        Returns a reference to the I64Field associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A refences to a I64Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field is NOT a I64Field or cannot be found.


=head3 getU8Field

C<libgmsec_perl::Message-E<gt>getU8Field($name)>

        Returns a reference to the U8Field associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A refences to a U8Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field is NOT a U8Field or cannot be found.


=head3 getU16Field

C<libgmsec_perl::Message-E<gt>getU16Field($name)>

        Returns a reference to the U16Field associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A refences to a U16Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field is NOT a U16Field or cannot be found.


=head3 getU32Field

C<libgmsec_perl::Message-E<gt>getU32Field($name)>

        Returns a reference to the U32Field associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A refences to a U32Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field is NOT a U32Field or cannot be found.


=head3 getU64Field

C<libgmsec_perl::Message-E<gt>getU64Field($name)>

        Returns a reference to the U64Field associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A refences to a U64Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field is NOT a U64Field or cannot be found.


=head3 getStringField

C<libgmsec_perl::Message-E<gt>getStringField($name)>

        Returns a reference to the StringField associated with the named field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A refences to a StringField object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        $libgmsec_perl::GmsecException is thrown if the named field is NOT a StringField or cannot be found.


=head3 getFieldCount

C<libgmsec_perl::Message-E<gt>getFieldCount()>

        This function gets the number of fields contained in this message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        The number of fields contained within Message.


=head3 copyFields

C<libgmsec_perl::Message-E<gt>copyFields($message)>

        This function will copy all fields from this message to a new message in the fastest way possible.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        The fields are NOT copied by reference. As such, the original message and its fields can be deleted or modified after it has been copied to the new message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameter:</b><br>

        $message - the message to copy fields into


=head3 toXML

C<libgmsec_perl::Message-E<gt>toXML()>

        This function will dump a message to an XML formatted string.

        XML format example:

        <MESSAGE SUBJECT="GMSEC.MISSION.CONST.SAT.EVT.MSG" KIND="PUBLISH">
            <FIELD TYPE="CHAR" NAME="CHAR-FIELD">c</FIELD>
            <FIELD TYPE="BOOL" NAME="BOOL-FIELD">TRUE</FIELD>
            <FIELD TYPE="I16" NAME="SHORT-FIELD">-123</FIELD>
            <FIELD TYPE="U16" NAME="USHORT-FIELD">123</FIELD>
            <FIELD TYPE="I32" NAME="INT-FIELD">-12345678</FIELD>
            <FIELD TYPE="U32" NAME="UINT-FIELD">123456789</FIELD>
            <FIELD TYPE="STRING" NAME="STRING-FIELD">This is a test of the Message::toXML function</FIELD>
            <FIELD TYPE="F32" NAME="LONG-FIELD" BITS="42F6E666">123.45</FIELD>
            <FIELD TYPE="F64" NAME="DOUBLE-FIELD" BITS="4B94216CA930D75A">1.234e56</FIELD>
            <FIELD TYPE="BIN" NAME="BIN-FIELD">4a4c4d4e4f5051</FIELD>
        </MESSAGE>

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        XML string representation of the message.


=head3 toJSON

C<libgmsec_perl::Message-E<gt>toJSON()>

        This function will dump a message to an JSON formatted string.

        JSON format example:

        {
            "MESSAGE":{
                "KIND":"PUBLISH",
                "SUBJECT":"GMSEC.MISSION.CONST.SAT.EVT.MSG",
                "FIELD":[
                    {
                        "NAME":"CHAR-FIELD",
                        "TYPE":"CHAR",
                        "VALUE":"c"
                    },
                    {
                        "NAME":"BOOL-FIELD",
                        "TYPE":"BOOL",
                        "VALUE":"TRUE"
                    },
                    {
                        "NAME":"SHORT-FIELD",
                        "TYPE":"I16",
                        "VALUE":"-123"
                    },
                    {
                        "NAME":"USHORT-SHORT",
                        "TYPE":"U16",
                        "VALUE":"123"
                    },
                    {
                        "NAME":"INT-FIELD",
                        "TYPE":"I32",
                        "VALUE":"-12345678"
                    },
                    {
                        "NAME":"UINT-FIELD",
                        "TYPE":"U32",
                        "VALUE":"123456789"
                    },
                    {
                        "NAME":"STRING-FIELD",
                        "TYPE":"STRING",
                        "VALUE":"This is a test of the Message::toJSON function"
                    },
                    {
                        "NAME":"LONG-FIELD",
                        "TYPE":"F32",
                        "VALUE":"123.45"
                        "BITS":"42F6E666"
                    },
                    {
                        "NAME":"DOUBLE-FIELD",
                        "TYPE":"F64",
                        "VALUE":"1.234e56"
                        "BITS":"4B94216CA930D75A"
                    },
                    {
                        "NAME":"BIN-FIELD",
                        "TYPE":"BIN",
                        "VALUE":"4a4c4d4e4f5051"
                    } 
                ]
            }
        }

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        JSON string representation of the message.


=head3 getSize

C<libgmsec_perl::Message-E<gt>getSize()>

        This function will return the physical storage size of the message in bytes. Please note that additional tracking data fields are added to messages at the time of publication which will affect the size of the message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Size of message in bytes


=head3 getFieldIterator

C<libgmsec_perl::Message-E<gt>getFieldIterator()>

        Method that allows the callee to get the MessageFieldIterator associated with the Message. This iterator will allow for applications to iterate over the Field objects stored within the Message. The iterator is reset each time getFieldIterator() is called. The iterator itself is destroyed when the Message object is destroyed.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        Only one MessageFieldIterator object is associated with a Message object; multiple calls to getFieldIterator() will return a reference to the same MessageFieldIterator object. Each call will reset the iterator.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A reference to a MessageFieldIterator object.


=head1 SEE ALSO

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Config.html">Config</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html">Connection</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Field.html">Field</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="MessageFactory.html">Connection</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="MessageFieldIterator.html">MessageFieldIterator</a>

=cut
%}
