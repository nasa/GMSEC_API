#!/usr/bin/env perl

# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file subscribe.pl
#
# @brief This example program provides a basic demonstration of subscribing
# to receive a message from the GMSEC Bus.
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

    # Validate messages received from the bus
    $config->addValue("gmsec-msg-content-validate-recv", "true");

    # Deduce subscription topic for the example program
    my $subscriptionTopic = "";
    my $level = $config->getIntegerValue("gmsec-schema-level", $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);

    if ($level == $libgmsec_perl::Specification::SchemaLevel_LEVEL_0)
    {
        $subscriptionTopic = "C2MS.>";
    }
    elsif ($level == $libgmsec_perl::Specification::SchemaLevel_LEVEL_1)
    {
        $subscriptionTopic = "C2MS-PIPE.>";
    }
    else
    {
        $subscriptionTopic = "GMSEC.>";
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

        # Subscribe to receive message
        $conn->subscribe( $subscriptionTopic );
        libgmsec_perl::logInfo("Subscribed to topic: " . $subscriptionTopic);

        # Define timeout period, or use Gmsec.WAIT_FOREVER to block
        # indefinitely to receive a message.
        my $timeout = 10000;   # 10 seconds

        # Attempt to receive a message
        my $msg = $conn->receive( $timeout );

        if (defined $msg)
        {
            # Received a message!
            libgmsec_perl::logInfo("Received Message:\n" . $msg->toXML());

            # Acknowledge processing the message
            $msg->acknowledge();

            # Release received message
            libgmsec_perl::Message::destroy( $msg );
        }
        else
        {
          # Time out!
          libgmsec_perl::logWarning("Time out no message was received");
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


# Main entry point
#
exit( main() );
