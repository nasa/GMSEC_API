#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# @file heartbeat_service.pl
#
# This file contains an example outlining how to use the Messaging Interface
# Standardization Toolkit (MIST) namespace ConnectionManager's HeartbeatService
# to publish GMSEC-compliant Heartbeat (HB) messages to the middleware bus.
#
# It is also recommended that you run a subscriber application
# (i.e. GMSEC_API/bin/gmsub) to see the HB Messages which are published to the
# bus.

use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

my $HB_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.HEARTBEAT-SERVICE.HB";
my $HB_PUBLISH_RATE    = 5; # in seconds

sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl heartbeat_service.pl mw-id=<middleware ID>\n";
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
		my $componentField = libgmsec_perl::StringField->new("COMPONENT", "HEARTBEAT-SERVICE");
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


		# Note: Fields are immutable, so plan appropriately if you wish
		# to re-use variable names!
		{
			#o Create all of the GMSEC Message header Fields which
			# will be used by all GMSEC HB Messages
			my $hbStandardFields = libgmsec_perl::FieldList->new();

			#o Determine which version of the GMSEC message specification
			# the ConnectionManager was initialized with and add
			# the correctly typed Fields to the Message
			my $pubField;
			my $counterField;
			if($version >= 201600)
			{
				$pubField = libgmsec_perl::U16Field->new("PUB-RATE", $HB_PUBLISH_RATE);
				$counterField = libgmsec_perl::U16Field->new("COUNTER", 1);
				$hbStandardFields->push($pubField);
				$hbStandardFields->push($counterField);
			}
			elsif($version == 201400)
			{
				$pubField = libgmsec_perl::I16Field->new("PUB-RATE", $HB_PUBLISH_RATE);
				$counterField = libgmsec_perl::I16Field->new("COUNTER", 1);
				$hbStandardFields->push($pubField);
				$hbStandardFields->push($counterField);
			}

			#o Note: COMPONENT-STATUS is an optional field used to
			# denote the operating status of the component, the
			# values are as follows:
			# 0 - Debug
			# 1 - Normal / Green
			# 2 - Warning / Yellow
			# 3 - Orange
			# 4 - Error / Red
			my $componentStatusField = libgmsec_perl::I16Field->new("COMPONENT-STATUS", 0);

			$hbStandardFields->push($componentStatusField);

			#o Create and publish a Heartbeat message using
			# createLogMessage() and publish()
			#
			# Note: This is useful for applications which may need
			# to create proxy heartbeats on behalf of a subsystem,
			# as creating multiple ConnectionManagers can consume
			# more memory than necessary.  In this case, extra
			# logic would need to be added to handle the timing of
			# the publications.
			my $hbMsg = $connManager->createHeartbeatMessage($HB_MESSAGE_SUBJECT, $hbStandardFields);
			libgmsec_perl::LogInfo("Publishing the GMSEC C2CX HB message which was just created using createHeartbeatMessage():\n" . $hbMsg->toXML());
			$connManager->publish($hbMsg);

			#o Kick off the Heartbeat Service -- This will publish
			# heartbeat messages automatically every X seconds,
			# where Xis the value which was provided for PUB-RATE
			# Note: If PUB-RATE was not provided, it will default
			# to 30 seconds per automatic Heartbeat publication
			libgmsec_perl::LogInfo("Starting the Heartbeat service, a message will be published every " . $pubField->getStringValue() ." seconds");
			$connManager->startHeartbeatService($HB_MESSAGE_SUBJECT, $hbStandardFields);
		}

		{
			#o Use setHeartbeatServiceField to change the state of the
			# COMPONENT-STATUS Field to indicate that the component has
			# transitioned from a startup/debug state to a running/green
			# state.
			my $componentStatusField = libgmsec_perl::I16Field->new("COMPONENT-STATUS", 1);
			$connManager->setHeartbeatServiceField($componentStatusField);
		}

		#o Wait for user input to end the program
		libgmsec_perl::LogInfo("Publishing C2CX Heartbeat Messages indefinitely, press <enter> to exit the program");
		readline(STDIN);

		#o Stop the Heartbeat Service
		$connManager->stopHeartbeatService();

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

exit(main(@ARGV));
