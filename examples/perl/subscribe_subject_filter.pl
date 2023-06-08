#!/usr/bin/env perl

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file subscribe_subject_filter.pl
# 
# This file contains a simple example outlining how to subscribe
# to and retrieve messages from the middleware bus while using subject
# exclusion filtering to reduce the scope of a wildcarded subject subscription.
#
# This example program is intended to be run before starting the
# 'publish_subject_filter' example program.
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

    # Deduce subscription topic for the example program
    my $ALL_MESSAGES_TOPIC = "";
    my $HB_MESSAGES_TOPIC  = "";
    my $level = $config->getIntegerValue("gmsec-schema-level", $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);

    if ($level == $libgmsec_perl::Specification::SchemaLevel_LEVEL_0)
    {
        $ALL_MESSAGES_TOPIC = "C2MS.>";
        $HB_MESSAGES_TOPIC  = "C2MS.*.*.*.*.*.MSG.HB.+";
    }
    elsif ($level == $libgmsec_perl::Specification::SchemaLevel_LEVEL_1)
    {
        $ALL_MESSAGES_TOPIC = "C2MS-PIPE.>";
        $HB_MESSAGES_TOPIC  = "C2MS-PIPE.*.*.*.*.*.MSG.HB.+";
    }
    else
    {
        $ALL_MESSAGES_TOPIC = "GMSEC.>";
        $HB_MESSAGES_TOPIC  = "GMSEC.*.*.MSG.HB.+";
    }

    eval
    {
        # Create connection instance.
        my $conn = libgmsec_perl::Connection::create( $config );

        # Establish connection to the GMSEC Bus.
        $conn->connect();
		
        # Output information regarding the API version and connection
        libgmsec_perl::logInfo(libgmsec_perl::Connection::getAPIVersion());
        libgmsec_perl::logInfo("Middleware version = " . $conn->getLibraryVersion());

        # Subscribe to receive all messages using the GMSEC specification
        libgmsec_perl::logInfo("Subscribe to: " . $ALL_MESSAGES_TOPIC);
        $conn->subscribe( $ALL_MESSAGES_TOPIC );

        # Add specific message topic to the Connection's exclusion filter
        libgmsec_perl::logInfo("Filtering out: " . $HB_MESSAGES_TOPIC);
        $conn->excludeSubject( $HB_MESSAGES_TOPIC );

        # Receive first message (LOG) sent by publisher
        libgmsec_perl::logInfo("Waiting to receive a message...");
        my $rcvdMsg = $conn->receive();

        libgmsec_perl::logInfo("Received LOG message:\n" . $rcvdMsg->toXML());
        $rcvdMsg->acknowledge();
        libgmsec_perl::Message::destroy( $rcvdMsg );

        # Although a second message (HB) is sent by the publisher, it will not
        # be delivered to this Connection object
        libgmsec_perl::logInfo("Waiting to receive other message(s)...");
        $rcvdMsg = $conn->receive(5000);

        if (defined $rcvdMsg)
        {
            libgmsec_perl::log_warning("Received unexpected message:\n" . $rcvdMsg->toXML());
            $rcvdMsg->acknowledge();
            libgmsec_perl::Message::destroy($rcvdMsg);
        }
        else
        {
            libgmsec_perl::logInfo("As expected, a timeout occurred (i.e. no HB message received)");
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


# Main entry point
#
exit( main() );
