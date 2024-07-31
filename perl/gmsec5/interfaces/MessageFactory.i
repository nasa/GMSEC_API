/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MessageFactory

%{
#include <gmsec5/MessageFactory.h>
#include <vector>
using namespace gmsec::api5;
%}

/* We'll tell SWIG to ignore these and define our own implementation using %extend */
%ignore gmsec::api5::MessageFactory::MessageFactory();
%ignore gmsec::api5::MessageFactory::MessageFactory(const Config&);

/* Ignore methods pertinent to the C binding */
%ignore gmsec::api5::MessageFactory::registerMessageValidator(GMSEC_MessageValidator*);

/* Functions containing lists that will be redefined */
%ignore gmsec::api5::MessageFactory::setStandardFields(const gmsec::api5::util::List<Field*>&);


%include <gmsec5/util/wdllexp.h>
%include <gmsec5/MessageFactory.h>


%include <std_vector.i>
namespace std
{
    %template(FieldArray) vector<gmsec::api5::Field*>;
}


%extend gmsec::api5::MessageFactory {

    static MessageFactory* CALL_TYPE create(const Config* config = NULL)
    {
        if (config == NULL) {
            return new MessageFactory();
        }
        return new MessageFactory(*config);
    }

    static void CALL_TYPE destroy(MessageFactory* factory)
    {
        delete factory;
    }

    void CALL_TYPE setStandardFields(const std::vector<gmsec::api5::Field*>& fields) {
        gmsec::api5::util::List<gmsec::api5::Field*> list_fields;

        for (std::vector<gmsec::api5::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it) {
            list_fields.push_back(*it);
        }

        return self->setStandardFields(list_fields);
    }
}


%perlcode%{
=pod

=head1 NAME

libgmsec_perl::MessageFactory

=head1 DESCRIPTION

This class provides the public interface for the MessageFactory.

=head2 Public Static Subroutines

=head3 create

C<libgmsec_perl::MessageFactory::create($config = undef)>

        Static method for creating a MessageFactory instance that produces messages based on the latest available message
        specification for the NASA?GMSEC Addendum (if no configuration provided), or based on the given configuration.

        Example 1:

        # Create a MessageFactory object using the latest NASA/GMSEC Addendum message specification
        my $factory = libgmsec_perl::MessageFactory::create();

        # Create a Heartbeat message
        my $hbMsg = $factory->createMessage("HB");

        libgmsec_perl::MessageFactory::destroy($factory);

        Example 2:

        # Create a MessageFactory object using the latest C2MS message specification
        my $config  = libgmsec_perl::Config->new("gmsec-message-specification=201900 gmsec-schema-level=0");
        my $factory = libgmsec_perl::MessageFactory::create($config);

        # Create a Heartbeat message
        my $hbMsg = $factory->createMessage("HB");

        libgmsec_perl::MessageFactory::destroy($factory);

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $config - Optional Config object of which will be used to construct a Specification object that the MessageFactory will rely on to generate messages.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException is thrown if the message specification cannot be deduced/loaded.


=head3 destroy

C<libgmsec_perl::MessageFactory::destroy($factory)>

        Static method for destroying the given MessageFactory instance.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $factory - A MessageFactory object.


=head2 Public Member Subroutines

=head3 setStandardFields

C<libgmsec_perl::MessageFactory-E<gt>setStandardFields($standardFields)>

        Allows for the setting of common (standard) fields that should be applied to all messages created by the MessageFactory.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $standardFields - The list of common/standard fields to apply to all messages.


=head3 clearStandardFields

C<libgmsec_perl::MessageFactory-E<gt>clearStandardFields()>

        Removes any common (standard) fields that have previously been associated with the MessageFactory.


=head3 setMessageConfig

C<libgmsec_perl::MessageFactory-E<gt>setMessageConfig($msgConfig)>

        Allows for the setting of a common message configuration that will be applied to all messages created by the MessageFactory.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $msgConfig - A message configuration object.


=head3 createMessage

C<libgmsec_perl::MessageFactory-E<gt>createMessage()>

        Creates and returns a Message object. The Message will contain any user-supplied message attributes (such as a message configuration, standard fields, and custom message validator).

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A Message object.


=head3 createMessage

C<libgmsec_perl::MessageFactory-E<gt>createMessage($schemaID)>

        Creates and returns a Message object that is populated with the fields derived from the working message specification and schema ID. In addtion, the Message will contain any user-supplied message attributes (such as a message configuration, standard fields, and custom message validator).

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $schemaID - The string identifying which message to create (e.g. HB).

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A Message object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException is thrown if the schemaID string is NULL, contains an empty string, or references an unknown/illegal schema ID.



=head3 fromData

C<libgmsec_perl::MessageFactory-E<gt>fromData($data, $type)>

        Creates and returns a Message object based on the given XML or JSON data.

        Example:

        # Create a message factory
        my $factory = libgmsec_perl::MessageFactory->new();

        my $xml = "<MESSAGE SUBJECT=\"GMSEC.TEST.XML.SUBJECT\" KIND=\"PUBLISH\">"
                  . "       <CONFIG>"
                  . "           <PARAMETER NAME=\"gmsec-msgfld-store-type\">tree</PARAMETER>"
                  . "           <PARAMETER NAME=\"gmsec-msgfld-store-size\">100</PARAMETER>"
                  . "        </CONFIG>"
                  . "        <FIELD TYPE=\"STRING\" NAME=\"EXAMPLE-STRING-NAME\">EXAMPLE TEXT</FIELD>"
                  . "        <FIELD TYPE=\"BOOL\" NAME=\"EXAMPLE-BOOL-FIELD-NAME\">TRUE</FIELD>"
                  . "</MESSAGE>";

        # Create a Message object using the XML string
        my $msg1 = $factory->fromData($xml, $libgmsec_perl::DataType_XML_DATA);

        my $json = "{\"MESSAGE\":{"
                   . "    \"KIND\":\"PUBLISH\","
                   . "    \"SUBJECT\":\"GMSEC.TEST.JSON.SUBJECT\","
                   . "    \"FIELD\":["
                   . "            {"
                   . "                \"NAME\":\"CHAR-FIELD\","
                   . "                \"TYPE\":\"CHAR\","
                   . "                \"VALUE\":\"c\""
                   . "            },"
                   . "            {"
                   . "                \"NAME\":\"BOOL-FIELD\","
                   . "                \"TYPE\":\"BOOL\","
                   . "                \"VALUE\":\"TRUE\""
                   . "            }"
                   . "        ]"
                   . "    }"
                   . "}";

        # Create a Message object using the JSON string
        my $msg2 = $factory->fromData($json, $libgmsec_perl::DataType_JSON_DATA);


=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $data - An XML or JSON string
        $type - The type of data being provided

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException is thrown if the XML or JSON string cannot be parsed


=head3 getSpecification

C<libgmsec_perl::Message-E<gt>getSpecification()>

        Returns a reference to the Specification object associated with the MessageFactory.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A Specification object.


=head3 registerMessageValidator

C<libgmsec_perl::Message-E<gt>registerMessageValidator($validator)>

        Registers the given custom MessageValidator with each message created using the MessageFactory.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $validator - A custom MessageValidator object.


=head1 SEE ALSO

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Config.html">Config</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html">Connection</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Message.html">Message</a>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Specification.html">Specification</a>


=cut
%}
