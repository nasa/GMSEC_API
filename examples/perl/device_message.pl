#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# @file device_message.pl
#
# This file contains an example outlining how to create and publish a
# GMSEC-compliant Mnemonic Data Value message to the middleware bus using the
# Messaging Interface Standardization Toolkit (MIST) namespace
# ConnectionManager class function calls available for such operations.
#
# It is also recommended that you run a subscriber application
# (i.e. GMSEC_API/bin/gmsub) to see the LOG Messages which are published to the
# bus.

use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

my $DEV_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.DEV.DEVICE_MESSAGE.DEV";

sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl device_message.pl mw-id=<middleware ID>\n";
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

	libgmsec_perl::LogInfo(libgmsec_perl::ConnectionManager::getAPIVersion());

	eval
	{
		my $connManager = libgmsec_perl::ConnectionManager->new($config);

		libgmsec_perl::LogInfo("Opening the connection to the middleware server");
		$connManager->initialize();

		libgmsec_perl::LogInfo($connManager->getLibraryVersion());

		#o Create all of the GMSEC Message header Fields which will
		# be used by all GMSEC Messages
		#
		# Note: Since these Fields contain variable values which are
		# based on the context in which they are used, they cannot be
		# automatically populated using MistMessage.
		my $definedFields = libgmsec_perl::FieldList->new();

		my $missionField = libgmsec_perl::StringField->new("MISSION-ID", "GMSEC");
		# Note: SAT-ID-PHYSICAL is an optional header Field, according
		# to the GMSEC ISD.
		my $satIdField = libgmsec_perl::StringField->new("SAT-ID-PHYSICAL", "SPACECRAFT");
		my $facilityField = libgmsec_perl::StringField->new("FACILITY", "GMSEC Lab");
		my $componentField = libgmsec_perl::StringField->new("COMPONENT", "device_message");

		$definedFields->push($missionField);
		$definedFields->push($satIdField);
		$definedFields->push($facilityField);
		$definedFields->push($componentField);

		#o Use setStandardFields to define a set of header fields for
		# all messages which are created or published on the
		# ConnectionManager using the following functions:
		# createLogMessage, publishLog, createHeartbeatMessage,
		# startHeartbeatService, createResourceMessage,
		# publishResourceMessage, or startResourceMessageService
		$connManager->setStandardFields($definedFields);

		my $paramVal = libgmsec_perl::I32Field->new("DEVICE.1.PARAM.1.VALUE", 79);
		my $param = libgmsec_perl::DeviceParam->new("DEV parameter 1", "parameter 1 timestamp", $paramVal);

		my $device1 = libgmsec_perl::Device->new("device 1", $libgmsec_perl::Device::RED);
		$device1->setGroup("group");
		$device1->setRole("role");
		$device1->setModel("model");
		$device1->setSerial("1138");
		$device1->setVersion("1.4.5.2.3.4.5");
		my $devInfo = libgmsec_perl::I16Field->new("info", 5);
		$device1->setInfo($devInfo);
		my $devNum = libgmsec_perl::I16Field->new("num", 5);
		$device1->setNumber($devNum);
		$device1->addParam($param);

		#o Construct an DEV Message and add the Device values to it
		my $devMessage = libgmsec_perl::DeviceMessage->new($DEV_MESSAGE_SUBJECT, $connManager->getSpecification());
		$devMessage->addDevice($device1);

		$connManager->addStandardFields($devMessage);

		$connManager->publish($devMessage);

		libgmsec_perl::LogInfo("Published DEV message:\n" . $devMessage->toXML());

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
