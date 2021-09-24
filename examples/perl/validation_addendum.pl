#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# @file validation_addendum.pl
#
# This file contains an example demonstrating the extensibility of the GMSEC
# API Message validation engine through use of additional XML template files
# represending an additional layer of Message definitions to be validated.

use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

#o Note: The name 'EXAMPLE' in this subject represents the name of the message
# specification which is being used.  If using message definitions outlined in
# the GMSEC Interface Specification Document, this would instead be, "GMSEC."
my $EXAMPLE_MESSAGE_SUBJECT = "EXAMPLE.MISSION.SPACECRAFT.MSG.LOG";

sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl validation_addendum.pl mw-id=<middleware ID>\n";
		return -1;
	}

	my $config = libgmsec_perl::Config->new();

	# Split arguments and add them to the Config
	foreach my $arg (@args)
	{
		#print "$arg\n";
		my @argVals = split /=/, $arg;
		$config->addValue($argVals[0], $argVals[1]);
	}

	initializeLogging($config);

  #o Enable Message validation.  This parameter is "false" by default.
	$config->addValue("GMSEC-MSG-CONTENT-VALIDATE", "true");

	#o Tell the API that there is an additional layer of message schema to
	# validate (The 'EXAMPLE' message definitions).  This value is set to
	# 0 (Only 'GMSEC' specification validation) by default.
	#
	# Note: These levels for validation are determined by the "LEVEL-X"
	# attributes defined in the .DIRECTORY.xml file contained in the XML
	# templates directory.  In thise case, Level-0 means GMSEC and Level-1
	# means EXAMPLE.
	#
	# Note: The GMSEC team envisions using message specifications in a
	# layered hierarchical fashion.  For example, the "GMSEC" message
	# specification would be 'layer 0', followed by an organization-level
	# message specification at 'layer 1' which builds upon the message
	# specification outlined in the GMSEC ISD.  This would be followed by
	# a mission-level message specification at 'layer 2' and so on.
	$config->addValue("GMSEC-SCHEMA-LEVEL", "1");

	#o Tell the API where to find all of the schema files.
	#
	# Note: This example only demonstrates a simple usage case of this
	# functionality.  When using this functionality, if the intent is
	# to use any of the GMSEC message definitions, then ALL of the XML
	# template files must be contained in the same directory.
	# e.g. GMSEC_API/templates/2016.00 (Or the location defined in
	# GMSEC-SCHEMA-PATH)
	$config->addValue("GMSEC-SCHEMA-PATH", "templates");

	#o Since this example relies on the 2016.00 version of the templates,
	# we indicate such within the configuration object.
	$config->addValue("GMSEC-SPECIFICATION-VERSION", "201600");

	libgmsec_perl::LogInfo(libgmsec_perl::ConnectionManager::getAPIVersion());

	eval
	{
		my $connMgr= libgmsec_perl::ConnectionManager->new($config);

		libgmsec_perl::LogInfo("Opening the connection to the middleware server");
		$connMgr->initialize();

		libgmsec_perl::LogInfo($connMgr->getLibraryVersion());

		my $definedFields = libgmsec_perl::FieldList->new();

		my $missionField = libgmsec_perl::StringField->new("MISSION-ID", "GMSEC");
		my $satIdField = libgmsec_perl::StringField->new("SAT-ID-PHYSICAL", "SPACECRAFT");
		my $facilityField = libgmsec_perl::StringField->new("FACILITY", "GMSEC Lab");
		my $componentField = libgmsec_perl::StringField->new("COMPONENT", "log_message");

		$definedFields->push($missionField);
		$definedFields->push($satIdField);
		$definedFields->push($facilityField);
		$definedFields->push($componentField);

		$connMgr->setStandardFields($definedFields);

		#o Create a Message using a subject defined in the XML template
		# outlining our example addendum message definitions
		my $message = libgmsec_perl::MistMessage->new($EXAMPLE_MESSAGE_SUBJECT, "MSG.LOG", $connMgr->getSpecification());

		#o Add all of the necessary Fields to our message
		$message->addField(libgmsec_perl::U16Field->new("NUM-OF-EVENTS", 2));
		$message->setValue("EVENT.1", addTimeToString("AOS occurred at: "));
		$message->setValue("EVENT.2", addTimeToString("Telemetry downlink began at: "));

		$connMgr->addStandardFields($message);

		#o Publish the message to the middleware bus
		$connMgr->publish($message);

		#o Display the XML string representation of the Message for
		# the sake of review
		libgmsec_perl::LogInfo("Published message:\n" . $message->toXML());

		#o Setup a new message without some of the Required Fields and
		# attempt to publish it (i.e. Trigger a validation failure)
		my $badMessage = libgmsec_perl::MistMessage->new($EXAMPLE_MESSAGE_SUBJECT, "MSG.LOG", $connMgr->getSpecification());

		eval
		{
			$connMgr->publish($badMessage);
		};
		if (isa($@, 'libgmsec_perl::Exception'))
		{
			libgmsec_perl::LogInfo("This error is expected:\n" . $@->what());
		}

		#o Disconnect from the middleware and clean up the Connection
		$connMgr->cleanup();
	};
	if(isa($@, 'libgmsec_perl::Exception'))
	{
		libgmsec_perl::LogError($@->what());
		return -1;
	}
	elsif($@)
	{
		libgmsec_perl::LogError($@);
		return -1;
	}

	return 0;
}

sub initializeLogging
{
	my ($config) = @_;

	my $logLevel = $config->getValue("LOGLEVEL");
	my $logFile = $config->getValue("LOGFILE");

	if (!$logLevel)
	{
		$config->addValue("LOGLEVEL", "INFO");
	}
	if (!$logFile)
	{
		$config->addValue("LOGFILE", "STDOUT");
	}
}

sub addTimeToString
{
  my ($preface) = @_;
  my $theTime = "YYYY-DDD-HH:MM:SS.sss";
  libgmsec_perl::TimeUtil::formatTime(libgmsec_perl::TimeUtil::getCurrentTime(), $theTime);
  return $preface . $theTime;
}

exit(main(@ARGV));
