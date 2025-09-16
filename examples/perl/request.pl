#!/usr/bin/env perl

# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file request.py
#
# @brief This example program provides a basic demonstration of issuing
# a synchronous Request Directive message to the GMSEC Bus, and then
# handling the receipt of a Response Directive message.
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

    # Validate messages before being sent to and as they are received from the bus
    $config->addValue("gmsec-msg-content-validate-all", "true");

    eval
    {
        # Create connection instance.
        my $conn = libgmsec_perl::Connection::create( $config );

        # Set up standard fields within the MessageFactory associated
        # with the connection object.
        setStandardFields( $conn->getMessageFactory(), "REQUEST" );

        # Establish connection to the GMSEC Bus.
        $conn->connect();

        # Output information regarding the API version and connection
        libgmsec_perl::logInfo(libgmsec_perl::Connection::getAPIVersion());
        libgmsec_perl::logInfo("Middleware version = " . $conn->getLibraryVersion());

        # Create a Request Directive message using the MessageFactory.
        # Set both required and optional fields within the message.
        my $reqMsg = $conn->getMessageFactory()->createMessage("REQ.DIR");
        $reqMsg->addField( libgmsec_perl::StringField->new("DESTINATION-COMPONENT", "REPLY", 1) );        # to whom is the request intended for
        $reqMsg->addField( libgmsec_perl::StringField->new("DIRECTIVE-STRING", "Turn on the lights") );   # operation to perform
        $reqMsg->addField( libgmsec_perl::BooleanField->new("RESPONSE", 1) );                             # a response message is requested
        $reqMsg->addField( libgmsec_perl::U16Field->new("REQUEST-ID", 0) );

        # Output in XML what we intend to send
        libgmsec_perl::logInfo("Sending Request Message:\n" . $reqMsg->toXML());

        # Timeout and republish periods have been arbitrarily chosen to be 5 seconds.
        # The republish period could also be set to GMSEC_REQUEST_REPUBLISH_NEVER to
        # prevent re-sending request message should a timeout occur.
        my $timeout   = 5000;
        my $republish = 5000;

        # Issue Request Directive.
        # Note, this is a blocking call (i.e. a synchronous operation).
        my $rspMsg = $conn->request( $reqMsg, $timeout, $republish );

        # We have returned from the previous blocking call check if we have a message.
        if (defined $rspMsg)
        {
            libgmsec_perl::logInfo("Received Response Directive message:\n" . $rspMsg->toXML());

            #Acknowledge processing the response message
            $rspMsg->acknowledge();

            # Destroy the response message
            libgmsec_perl::Message::destroy($rspMsg);
        }
        else
        {
            # This else-block is reached if a response message is not received.
            # If the republish period is set to a time period other than
            # GMSEC_REQUEST_REPUBLISH_NEVER, then this block will never be reached.
            libgmsec_perl::log_warning("Time out no Response Directive message received");
        }

        # Disconnect from the GMSEC Bus, and terminate subscriptions.
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


# Main entry point
#
exit( main() );
