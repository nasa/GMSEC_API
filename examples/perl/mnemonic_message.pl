#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.



# @file mnemonic_message.pl
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

my $GMSEC_SPEC_VERSION   = "201600";
my $MVAL_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.MVAL.MNEMONIC_MESSAGE";


sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl mnemonic_message.pl mw-id=<middleware ID>\n";
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

	#o Set the GMSEC message specification version to be used to determine
	# what the structure of messages is for verification and the
	# construction of MistMessages
	$config->addValue("GMSEC-SPECIFICATION-VERSION", $GMSEC_SPEC_VERSION);

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
		my $componentField = libgmsec_perl::StringField->new("COMPONENT", "mnemonic_message");

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

		#o Populate the Mnemonic Sample(s)
		my $mSample = libgmsec_perl::MnemonicSample->new("MS1", libgmsec_perl::I32Field->new("MS1", 15));
		$mSample->setEUValue(libgmsec_perl::F32Field->new("My EU", 15.0));
		$mSample->setFlags(1);
		$mSample->setLimit($libgmsec_perl::MnemonicSample::RED_HIGH);
		# Implicitly set limit enable/disable with setting of limit
		$mSample->setQuality(1);
		$mSample->setStalenessStatus(0);
		$mSample->setTextValue("15");

		my $mnemonic_samples = libgmsec_perl::MnemonicSampleList->new();
		$mnemonic_samples->push($mSample);

		#o Add the Mnemonic values to a Mnemonic object
		my $mnemonic = libgmsec_perl::Mnemonic->new("M1", $mnemonic_samples);
		my $statusField = libgmsec_perl::I16Field->new("status", 5);
		$mnemonic->setStatus($statusField);
		$mnemonic->setUnits("units");

		#o Determine which version of the GMSEC message specification
		# the ConnectionManager was initialized with
		my $version = $connManager->getSpecification()->getVersion();
		my $gmsecSpecVersion = "";
		if ($version == 201600)
		{
			$gmsecSpecVersion = "2016.00";
		}
		elsif ($version == 201400)
		{
			$gmsecSpecVersion = "2014.00";
		}

		#o Build up the Schema ID -- This is used to identify the
		# specific type of MVAL message to construct
		my $schemaId = $gmsecSpecVersion . ".GMSEC.MSG.MVAL";

		#o Construct an MVAL Message and add the Mnemonic values to it
		my $mvalMessage = libgmsec_perl::MnemonicMessage->new($MVAL_MESSAGE_SUBJECT, $schemaId, $connManager->getSpecification());
		$mvalMessage->addMnemonic($mnemonic);

		#o If validating with the 2014 spec, the MSG-ID field is
		# required
		if ($version == 201400)
		{
			$mvalMessage->setValue("MSG-ID", "MVAL Request MSG-ID would go here");
		}

		#o Add the header fields to the MVAL message
		$connManager->addStandardFields($mvalMessage);

		libgmsec_perl::LogInfo("Publishing MVAL message:\n" . $mvalMessage->toXML());
		$connManager->publish($mvalMessage);

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
