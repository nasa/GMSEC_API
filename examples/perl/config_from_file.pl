#!/usr/bin/env perl

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file config_from_file.pl
# 
# @brief This example program provides a basic demonstration of creating
# a Config object using the XML contents of a file that represent a
# connection (or message) configuration.
#
# A configuration file should have contents using a format similar to:
#
# <CONFIG NAME="Bolt">
#     <PARAMETER NAME="mw-id">bolt</PARAMETER>
#     <PARAMETER NAME="server">my-server</PARAMETER>
# </CONFIG>
#
# <CONFIG NAME="ActiveMQ">
#     <PARAMETER NAME="mw-id">activemq39</PARAMETER>
#     <PARAMETER NAME="server">tcp://my-server:61616</PARAMETER>
# </CONFIG>
#
# See config.xml for a simple example.
#


use strict;
use libgmsec_perl;


*isa = \&UNIVERSAL::isa;


sub main
{
    my ($args) = @_;

    # Initialize log level for output
    initializeLogLevel();

    eval
    {
        my $config1 = libgmsec_perl::Config::getFromFile( "config.xml", "Bolt" );
        my $config2 = libgmsec_perl::Config::getFromFile( "config.xml", "ActiveMQ" );
        my $config3 = libgmsec_perl::Config::getFromFile( "config.xml", "");

        libgmsec_perl::logInfo("Config 1 is:\n" . $config1->toXML());
        libgmsec_perl::logInfo("Config 2 is:\n" . $config2->toXML());
        libgmsec_perl::logInfo("Config 3 is:\n" . $config3->toXML());
    };
    if (isa($@, 'libgmsec_perl::GmsecException'))
    {
        my $error = $@;

        libgmsec_perl::logError("Exception: " . $error->what());
    }
}


sub initializeLogLevel()
{
    libgmsec_perl::Log::setReportingLevel( $libgmsec_perl::LogLevel_logINFO );
}


# Main entry point
#
exit( main( @ARGV ) );
