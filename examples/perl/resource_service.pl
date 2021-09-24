#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.



# @file resource_service.pl
#
# This file contains an example outlining how to use the Messaging Interface
# Standardization Toolkit (MIST) namespace ConnectionManager's ResourceMessageService
# to publish GMSEC-compliant Resource (RSRC) messages to the middleware bus.
#
# It is also recommended that you run a subscriber application
# (i.e. GMSEC_API/bin/gmsub) to see the HB Messages which are published to the
# bus.

use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

my $RSRC_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.RESOURCE-SERVICE.RSRC";
my $RSRC_PUBLISH_RATE    = 5; # in seconds

sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl msg_field_container.pl mw-id=<middleware ID>\n";
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

	libgmsec_perl::LogInfo(libgmsec_perl::ConnectionManager::getAPIVersion());

	eval
	{
		my $connManager = libgmsec_perl::ConnectionManager->new($config);

		libgmsec_perl::LogInfo("Opening the connection to the middleware server");
		$connManager->initialize();

		libgmsec_perl::LogInfo($connManager->getLibraryVersion());

		#o Create all of the GMSEC Message header Fields which will
		# be used by all GMSEC Messages
		my $headerFields = libgmsec_perl::FieldList->new();

		my $version = $connManager->getSpecification()->getVersion();

		my $missionField = libgmsec_perl::StringField->new("MISSION-ID", "MY-MISSION");
		my $facilityField = libgmsec_perl::StringField->new("FACILITY", "MY-FACILITY");
		my $componentField = libgmsec_perl::StringField->new("COMPONENT", "RESOURCE-SERVICE");
		my $domain1Field = libgmsec_perl::StringField->new("DOMAIN1", "MY-DOMAIN-1");
		my $domain2Field = libgmsec_perl::StringField->new("DOMAIN2", "MY-DOMAIN-2");
		my $msgID = libgmsec_perl::StringField->new("MSG-ID", "MY-MSG-ID");

		$headerFields->push($missionField);
		$headerFields->push($facilityField);
		$headerFields->push($componentField);

		if($version == 201400)
		{
			$headerFields->push($msgID);
		}
		elsif($version >= 201800)
		{
			$headerFields->push($domain1Field);
			$headerFields->push($domain2Field);
		}
		#o Use setStandardFields to define a set of header fields for
		# all messages which are created or published on the
		# ConnectionManager using the following functions:
		# createLogMessage, publishLog, createHeartbeatMessage,
		# startHeartbeatService, createResourceMessage,
		# publishResourceMessage, or startResourceMessageService
		$connManager->setStandardFields($headerFields);


		#o Create and publish a Resource message using
		# createResourceMessage() and publish()
		#
		# Note: This is useful for applications which may need to add
		# additional Fields to the Resource Messages which are not
		# currently added by the GMSEC API
		my $rsrcMsg = $connManager->createResourceMessage($RSRC_MESSAGE_SUBJECT);
		libgmsec_perl::LogInfo("Publishing the GMSEC C2CX RSRC message which was created using createResourceMessage():\n" . $rsrcMsg->toXML());
		$connManager->publish($rsrcMsg);


		#o Kick off the Resource Service -- This will publish resource
		# messages automatically every X seconds, where X is the second
		# parameter provided to the startResourceMessageService() function.
		# If an interval is not provided, the service will default to
		# publishing a message every 60 seconds.
		libgmsec_perl::LogInfo("Starting the Resource Message service, a message will be published every " . $RSRC_PUBLISH_RATE . " seconds");
		$connManager->startResourceMessageService($RSRC_MESSAGE_SUBJECT, $RSRC_PUBLISH_RATE);

		#o Wait for user input to end the program
		libgmsec_perl::LogInfo("Publishing C2CX Resource Messages indefinitely, press <enter> to exit the program");
		readline(STDIN);

		#o Stop the Heartbeat Service
		$connManager->stopResourceMessageService();

		#o Cleanup
		$connManager->cleanup();
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

exit(main(@ARGV));
