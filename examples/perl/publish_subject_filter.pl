#!/usr/bin/env perl

# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file publish_subject_filter.pl
# 
# This file contains an example publishing a few messages with different
# subjects in order to demonstrate the subject filtering functionality of the
# API. 
#
# This example program is intended to be run after starting the
# 'subscribe_subject_filter' example program.
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

    eval
    {
        # Create connection instance.
        my $conn = libgmsec_perl::Connection::create( $config );

        # Set up standard fields within the MessageFactory associated
        # with the connection object.
        setStandardFields( $conn->getMessageFactory(), "PUBLISH-SUBJECT-FILTER" );

        # Establish connection to the GMSEC Bus.
        $conn->connect();
		
        # Output information regarding the API version and connection
        libgmsec_perl::logInfo(libgmsec_perl::Connection::getAPIVersion());
        libgmsec_perl::logInfo("Middleware version = " . $conn->getLibraryVersion());

        # Create a valid Log message using the MessageFactory.
        my $logMsg = $conn->getMessageFactory()->createMessage("LOG");

        # Add required fields
        $logMsg->addField( libgmsec_perl::StringField->new("SUBCLASS", "INFO") );
        $logMsg->addField( libgmsec_perl::StringField->new("OCCURRENCE-TYPE", "TEST") );
        $logMsg->addField( libgmsec_perl::I16Field->new("SEVERITY", 1) );
        $logMsg->addField( libgmsec_perl::StringField->new("MSG-TEXT", "Hello subscriber!") );

        # And the current (event) time
        my $theTime = libgmsec_perl::TimeUtil::formatTime( libgmsec_perl::TimeUtil::getCurrentTime() );
        $logMsg->addField( libgmsec_perl::StringField->new("EVENT-TIME", $theTime) );

        # Create a valid Heartbeat message using the MessageFactory.
        my $hbMsg = $conn->getMessageFactory()->createMessage("HB");

        libgmsec_perl::logInfo("Publishing two messages -- Log message will be received by the subscriber,"
            . " the Heartbeat message will be filtered out");

        # Publish the Log message
        $conn->publish( $logMsg );
        libgmsec_perl::logInfo("Published LOG message:\n" . $logMsg->toXML());

        # Delay a bit (for no particular reason)
        libgmsec_perl::TimeUtil::millisleep(2000);

        # Publish the Heartbeat message
        $conn->publish( $hbMsg );
        libgmsec_perl::logInfo("Published HB message:\n" . $hbMsg->toXML());

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


# Main entry point
#
exit( main() );
