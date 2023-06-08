#!/usr/bin/env perl

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file specification_addendum.pl
# 
# This file contains an example demonstrating the extensibility of the GMSEC
# API Message validation engine through use of additional XML template files
# represending an additional layer of Message definitions to be validated.
#


use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;


sub main
{
    # Load the command-line input into a GMSEC Config object
    my $config = undef;
    if ($#ARGV ge 0)
    {
        $config = libgmsec_perl::Config::create(\@ARGV);
    }
    else
    {
        $config = libgmsec_perl::Config->new();
    }

    # Initialize log level for output
    initializeLogLevel($config);

    # Validate messages before being sent to the bus
    $config->addValue("gmsec-msg-content-validate-send", "true");

    # Tell the API that there is an additional layer of message schema to
    # validate (The 'EXAMPLE' message definitions). By default, this value
    # is set to 0 (only 'C2MS' specification validation).
    # 
    # Note: These levels for validation are determined by the "LEVEL-X"
    # attributes defined in the DIRECTORY.xml file contained in the XML
    # templates directory. In the case of this example, Level-0 implies
    # C2MS, and Level-1 is our example extension.
    # 
    # Note: The GMSEC team envisions using message specifications in a
    # layered hierarchical fashion. For example, the "C2MS" message
    # specification would be 'layer 0', followed by an organization-level
    # message specification at 'layer 1' which builds upon the message
    # specification outlined in C2MS.  This would be followed by a mission
    # level message specification at 'layer 2' (e.g. GMSEC) and so on.
    $config->addValue("GMSEC-SCHEMA-LEVEL", "1");

    # Tell the API where to find all of the schema files.
    # 
    # Note: This example only demonstrates a simple usage case of this
    # functionality. When using this functionality, if the intent is
    # to use any of the GMSEC message definitions, then ALL of the XML
    # template files must be contained in the same directory.
    # e.g. GMSEC_API/templates/2019.00 (or the location defined in
    # GMSEC-SCHEMA-PATH)
    $config->addValue("GMSEC-SCHEMA-PATH", "templates");

    # Since this example relies on the 2019.00 version of the templates,
    # we indicate such within the configuration object.
    $config->addValue("GMSEC-SPECIFICATION-VERSION", "201900");

    eval
    {
        # Create connection instance using custom configuration.
        my $conn = libgmsec_perl::Connection::create( $config );

        # Set up standard fields within the MessageFactory associated
        # with the connection object.
        setStandardFields( $conn->getMessageFactory(), "SPECIFICATION-ADDENDUM" );

        # Establish connection to the GMSEC Bus.
        $conn->connect();

        # Output information regarding the API version and connection.
        libgmsec_perl::logInfo(libgmsec_perl::Connection::getAPIVersion());
        libgmsec_perl::logInfo("Middleware version = " . $conn->getLibraryVersion());

        # Create a LOG Message using a subject defined in the XML template
        # outlining our example addendum message definitions.
        my $logMsg = $conn->getMessageFactory()->createMessage("LOG");

        # Add all of the required fields to the LOG message.
        $logMsg->addField( libgmsec_perl::U16Field->new("NUM-OF-EVENTS", 2) );
        $logMsg->addField( libgmsec_perl::StringField->new("EVENT.1.DESC", addTimeToString("AOS occurred at: ")) );
        $logMsg->addField( libgmsec_perl::StringField->new("EVENT.2.DESC", addTimeToString("Telemetry downlink began at: ")) );

        # Publish the LOG message to the GMSEC Bus.
        $conn->publish( $logMsg );

        # Display the XML string representation of the LOG message just published.
        libgmsec_perl::logInfo("Published custom LOG Message:\n" . $logMsg->toXML());

        # Setup a new LOG message without the NUM-OF-EVENTS required field and
        # attempt to publish it (i.e. trigger a validation failure)
        my $badLogMsg = $conn->getMessageFactory()->createMessage("LOG");

        eval
        {
            $conn->publish( $badLogMsg );
            libgmsec_perl::logWarning("Surprisingly was able to publish an invalid message:\n" . $badLogMsg->toXML());
        };
        if (isa($@, 'libgmsec_perl::GmsecException'))
        {
            my $error = $@;
            libgmsec_perl::logInfo("This validation error is expected for the incomplete LOG message:\n" . $error->what());
        }

        # Disconnect from the GMSEC Bus.
        $conn->disconnect();

        # Destroy the connection instance
        libgmsec_perl::Connection::destroy($conn);
    };
    if (isa($@, 'libgmsec_perl::GmsecException'))
    {
        # If here, the most likely case is the middleware could not be
        # deduced, or a failure occurred when attempting to connect to
        # the GMSEC Bus.
        my $error = $@;
        libgmsec_perl::logError("Exception: " . $error->what());
        return -1;
    }

    return 0;
}


sub initializeLogLevel
{
    my ($config) = @_;
    my $level = libgmsec_perl::Log::fromString( $config->getValue("loglevel", "info") );
    libgmsec_perl::Log::setReportingLevel($level);
}


# Convenience method for associating standard (i.e. common) fields
# with the MessageFactory which will be included automatically within
# all messages created using the factory.
#
sub setStandardFields
{
    my ($factory, $component) = @_;

    my $standardFields = libgmsec_perl::FieldArray->new();

    my $field = libgmsec_perl::StringField->new("MISSION-ID", "MY-MISSION", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $field = libgmsec_perl::StringField->new("SAT-ID-PHYSICAL", "MY-SAT-ID", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $field = libgmsec_perl::StringField->new("SAT-ID-LOGICAL", "MY-SAT-ID", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $field = libgmsec_perl::StringField->new("CONSTELLATION-ID", "MY-CNST", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $field = libgmsec_perl::StringField->new("FACILITY", "MY-FACILITY", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $field = libgmsec_perl::StringField->new("DOMAIN1", "MY-DOMAIN-1", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $field = libgmsec_perl::StringField->new("DOMAIN2", "MY-DOMAIN-2", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $field = libgmsec_perl::StringField->new("COMPONENT", $component, 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $factory->setStandardFields( $standardFields );
}


sub addTimeToString
{
    my ($preface) = @_;

    return $preface . libgmsec_perl::TimeUtil::formatTime( libgmsec_perl::TimeUtil::getCurrentTime() );
}


# Main entry point
#
exit( main() );
