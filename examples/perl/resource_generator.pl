#!/usr/bin/env perl

# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file resource_generator.pl
#
# @brief This example program provides a basic demonstration of using
# the ResourceGenerator to publish resource messages onto the GMSEC
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

    # Validate messages before being sent to the bus
    $config->addValue("gmsec-msg-content-validate-send", "true");

    # Define the resource publish rate and how often resource are collected
    my $publishRate     = 5;  # in seconds
    my $sampleInterval  = 1;  # in seconds
    my $averageInterval = 10; # in seconds

    eval
    {
        # Acquire standard fields to be included with the heartbeat messages
        my $standardFields = getStandardFields();

        # Create ResourceGenerator instance.
        my $rsrcgen = libgmsec_perl::ResourceGenerator::create( $config, $publishRate, $sampleInterval, $averageInterval, $standardFields );

        # Output information regarding the API version and connection
        libgmsec_perl::logInfo(libgmsec_perl::Connection::getAPIVersion());
        libgmsec_perl::logInfo("ResourceGenerator publish rate is: $publishRate seconds");

        # Start the resource generator
        $rsrcgen->start();

        libgmsec_perl::logInfo("ResourceGenerator is running use gmsub or other utility to monitor messages");

        libgmsec_perl::logInfo("Press <enter> to stop the ResourceGenerator");
        readline(STDIN);

        # Stop the ResourceGenerator
        $rsrcgen->stop();
        libgmsec_perl::logInfo("ResourceGenerator has been stopped");

        # Destroy the ResourceGenerator instance
        libgmsec_perl::ResourceGenerator::destroy($rsrcgen);
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
