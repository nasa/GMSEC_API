#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# @file heartbeat_generator.pl
#
# This file contains an example describing how to instantiate and
# start/stop a Heartbeat (Message) Generator.


use strict;
use libgmsec_perl;


*isa = \&UNIVERSAL::isa;


my $HB_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.HEARTBEAT-GENERATOR.HB";
my $HB_PUBLISH_RATE    = 5;


sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl heartbeat_generator.pl mw-id=<middleware ID>\n";
		return -1;
	}

	# Setup configuration with the supplied command line arguments
	my $config = libgmsec_perl::Config->new();

	# Split arguments and add them to the Config
	foreach my $arg (@args)
	{
		#print "$arg\n";
		my @argVals = split /=/, $arg;
		$config->addValue($argVals[0], $argVals[1]);
	}

	# Unless otherwise configured, setup configuration that allows us to
	# log messages to stderr.
	initializeLogging($config);

	# Display the version number of the GMSEC API
	libgmsec_perl::LogInfo(libgmsec_perl::ConnectionManager::getAPIVersion());

	# Define standard fields
	my $standardFields = libgmsec_perl::FieldList->new();

	my $field = libgmsec_perl::StringField->new("COMPONENT", "HEARTBEAT-GENERATOR");
	$standardFields->push($field);
	$field->DISOWN();

	$field = libgmsec_perl::StringField->new("MISSION-ID", "MY-MISSION");
	$standardFields->push($field);
	$field->DISOWN();

	$field = libgmsec_perl::StringField->new("SAT-ID-PHYSICAL", "MY-SAT-ID");
	$standardFields->push($field);
	$field->DISOWN();

	$field = libgmsec_perl::StringField->new("FACILITY", "MY-FACILITY");
	$standardFields->push($field);
	$field->DISOWN();

	my $specVersion = $config->getIntegerValue("gmsec-specification-version", $libgmsec_perl::GMSEC_ISD_CURRENT);

	if ($specVersion == $libgmsec_perl::GMSEC_ISD_2014_00)
	{
		$field = libgmsec_perl::StringField->new("MSG-ID", "MY-MSG-ID");
		$standardFields->push($field);
		$field->DISOWN();
	}
	elsif ($specVersion >= $libgmsec_perl::GMSEC_ISD_2018_00)
	{
		$field = libgmsec_perl::StringField->new("DOMAIN1", "MY-DOMAIN-1");
		$standardFields->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("DOMAIN2", "MY-DOMAIN-2");
		$standardFields->push($field);
		$field->DISOWN();
	}

	eval
	{
		# Instantiate the heartbeat generator
		my $hbgen = libgmsec_perl::HeartbeatGenerator->new($config, $HB_MESSAGE_SUBJECT, $HB_PUBLISH_RATE, $standardFields);

		# Start the heartbeat generator
		$hbgen->start();
		libgmsec_perl::LogInfo("Heartbeat Generator is running; use gmsub or other utility to monitor messages.");

		# Wait for user input to stop the heartbeat generator
		libgmsec_perl::LogInfo("Press <enter> to stop the heartbeat generator");
		readline(STDIN);

		# Stop the heartbeat generator
		$hbgen->stop();
		libgmsec_perl::LogInfo("Heartbeat Generator has been stopped.");
	};
	if(isa($@, 'libgmsec_perl::Exception'))
	{
		libgmsec_perl::LogError($@->what());
		return -1;
	}
	elsif($@)
	{
		libgmsec_perl::LogError($@);
	}

	return 0;
}


sub initializeLogging
{
	my ($config) = @_;

	my $logLevel = $config->getValue("loglevel");
	my $logFile = $config->getValue("logfile");

	if (!$logLevel)
	{
		$config->addValue("loglevel", "info");
	}
	if (!$logFile)
	{
		$config->addValue("logfile", "stderr");
	}
}


exit(main(@ARGV));
