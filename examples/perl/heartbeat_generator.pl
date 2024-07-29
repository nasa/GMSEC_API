#!/usr/bin/env perl

# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file heartbeat_generator.pl
#
# @brief This example program provides a basic demonstration of using
# the HeartbeatGenerator to publish heartbeat messages onto the GMSEC
# Bus.
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

    # Validate messages before they are sent to the bus
    $config->addValue("gmsec-msg-content-validate-send", "true");

    # Define the heartbeat publish rate
    my $publishRate = 2;  # in seconds

    eval
    {
        # Create HeartbeatGenerator instance.
        my $hbgen = libgmsec_perl::HeartbeatGenerator::create($config, $publishRate, getStandardFields());

        # Output information regarding the API version and connection
        libgmsec_perl::logInfo(libgmsec_perl::Connection::getAPIVersion());
        libgmsec_perl::logInfo("HeartbeatGenerator publish rate is: $publishRate seconds");

        # Start the heartbeat generator
        $hbgen->start();

        libgmsec_perl::logInfo("HeartbeatGenerator is running use gmsub or other utility to monitor messages");

        libgmsec_perl::logInfo("Press <enter> to stop the HeartbeatGenerator");
        readline(STDIN);

       # Stop the HeartbeatGenerator
       $hbgen->stop();
       libgmsec_perl::logInfo("HeartbeatGenerator has been stopped");

        # Destroy the HeartbeatGenerator instance
        libgmsec_perl::HeartbeatGenerator::destroy($hbgen);
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


sub getStandardFields
{
    my $standardFields = libgmsec_perl::FieldArray->new();

    # Define standard fields for the heartbeat message
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

    $field = libgmsec_perl::StringField->new("COMPONENT", "HB-GEN", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    return $standardFields;
}


# Main entry point
#
exit( main() );
