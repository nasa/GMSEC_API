#!/usr/bin/env perl

# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file message_bins.pl
#
# @brief This file contains an example describing how to aggregate multiple
# messages into a singular message sent to the middleware server. This
# functionality is useful for reducing the number of messages sent across
# the * middleware bus, if there is an application which is publishing an
# exorbitantly large number of (small) messages.
#
# It is recommended to execute the application GMSEC_API/bin/gmsub prior
# to running this program in order to see that the aggregated messages
# still appear on the receiving end as if they were individual messages.
#
# Please note that if an aggregated message is sent by the GMSEC API, any
# receivers will need to be running at least version 4.2 of the API in order
# to de-aggregate the messages. Older versions can receive the messages
# without crashing, but they will be unusable.
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

    # Deduce subject and excluded subject for message(s) that will be binned and for those that will not
    my $msg_bin_subject = undef;
    my $msg_bin_exclude_subject = undef;
    my $level = $config->getIntegerValue("gmsec-schema-level", $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);

    if ($level == $libgmsec_perl::Specification::SchemaLevel_LEVEL_0)
    {
        $msg_bin_subject         = "C2MS.*.*.*.*.*.MSG.LOG.*.AAA.>";
        $msg_bin_exclude_subject = "C2MS.*.*.*.*.*.MSG.LOG.*.TLM.>";
    }
    elsif ($level == $libgmsec_perl::Specification::SchemaLevel_LEVEL_1)
    {
        $msg_bin_subject         = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.AAA.>";
        $msg_bin_exclude_subject = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.TLM.>";
    }
    else
    {
        $msg_bin_subject         = "GMSEC.*.*.MSG.LOG.*.AAA.>";
        $msg_bin_exclude_subject = "GMSEC.*.*.MSG.LOG.*.TLM.>";
    }

    # Validate messages before they are sent to the bus
    $config->addValue("gmsec-msg-content-validate-send", "true");

    # Configure to enable message binning
    $config->addValue("gmsec-use-msg-bins", "true");

    # Specify the number of messages to be aggregated prior to publishing
    # the aggregate message to the middleware server. This applies to all
    # of the messages which match the subject(s) provided in the
    # GMSEC-MSG-BIN-SUBJECT-N configuration parameters
    # Note: The aggregate message will be sent to the middleware server
    # immediately upon this many messages being published, regardless of
    # the value supplied for GMSEC-MSG-BIN-TIMEOUT.
    $config->addValue("gmsec-msg-bin-size", "10");

    # Specify a timeout (in milliseconds) for the aggregate message to be
    # sent to the GMSEC Bus.
    # Note: The aggregate message will be sent to the GMSEC Bus after this
    # period of time if the message bin does not fill up (per the value
    # provided for gmsec-msg-bin-size) prior to this timeout
    $config->addValue("gmsec-msg-bin-timeout", "5000");

    # Specify the message subject(s) of messages that should be aggregated.
    # Subscription wildcard syntax can be used here.
    # Note: If this option is not specified, then ALL messages will be
    # aggregated (except those that are excluded).
    $config->addValue("gmsec-msg-bin-subject-1", $msg_bin_subject);

    # Specify any message subject(s) that should be excluded from being
    # aggregated.
    # This is useful if a wildcard subscription is provided in one of the
    # gmsec-msg-bin-subject-N parameters.
    $config->addValue("gmsec-msg-bin-exclude-subject-1", $msg_bin_exclude_subject);

    eval
    {
        # Create connection instance.
        my $conn = libgmsec_perl::Connection::create( $config );

        # Set up standard fields within the MessageFactory associated
        # with the connection object.
        setStandardFields( $conn->getMessageFactory(), "MESSAGE-BINS" );

        # Establish connection to the GMSEC Bus.
        $conn->connect();

        # Output information regarding the API version and connection
        libgmsec_perl::logInfo(libgmsec_perl::Connection::getAPIVersion());
        libgmsec_perl::logInfo("Middleware version = " . $conn->getLibraryVersion());

        # Create a Log messages using the MessageFactory.
        # Set both required and optional fields within the message.
        my $logMsg1 = $conn->getMessageFactory()->createMessage("LOG");
        my $logMsg2 = $conn->getMessageFactory()->createMessage("LOG");

        $logMsg1->addField( libgmsec_perl::StringField->new("SUBCLASS", "AAA") );
        $logMsg1->addField( libgmsec_perl::StringField->new("OCCURRENCE-TYPE", "PLOT") );
        $logMsg1->addField( libgmsec_perl::I16Field->new("SEVERITY", 1) );
        $logMsg1->addField( libgmsec_perl::StringField->new("MSG-TEXT", "Message will be binned") );

        $logMsg2->addField( libgmsec_perl::StringField->new("SUBCLASS", "TLM") );
        $logMsg2->addField( libgmsec_perl::StringField->new("OCCURRENCE-TYPE", "DATA") );
        $logMsg2->addField( libgmsec_perl::I16Field->new("SEVERITY", 1) );
        $logMsg2->addField( libgmsec_perl::StringField->new("MSG-TEXT", "Message will NOT be binned") );

        for (my $i = 0; $i < 5; $i++)
        {
            my $theTime = libgmsec_perl::TimeUtil::formatTime( libgmsec_perl::TimeUtil::getCurrentTime() );

            $logMsg1->addField( libgmsec_perl::StringField->new("EVENT-TIME", $theTime) );
            $logMsg2->addField( libgmsec_perl::StringField->new("EVENT-TIME", $theTime) );

            # Publish the first message it will be aggregated and published at a later time
            $conn->publish( $logMsg1 );
            libgmsec_perl::logInfo("Publishing AAA Log Message " . ($i+1) . " (will be aggregated):\n" . $logMsg1->toXML());

            # Publish the second message it will NOT be aggregated and will be published immediately
            $conn->publish( $logMsg2 );
            libgmsec_perl::logInfo("Publishing TLM Log Message " . ($i+1) . " (will NOT be aggregated):\n" . $logMsg2->toXML());

            # Add bogus processing delay
            libgmsec_perl::TimeUtil::millisleep(100);
        }

        # Delay to demonstrate message bin is flushed after a period of time (e.g. 5 seconds)
        libgmsec_perl::logInfo("Waiting for binned messages to be published...");
        libgmsec_perl::TimeUtil::millisleep(6000);

        # Disconnect from the GMSEC Bus.
        # Note: Any remaining messages in message bins will be published
        # before disconnecting from the GMSEC Bus, thus ensuring no message
        # loss occurs.
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


# Convenience function for associating standard (i.e. common) fields
# with the MessageFactory which will be included automatically within
# all messages created using the factory.
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
