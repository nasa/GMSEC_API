#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# @file product_message.pl
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

my $PROD_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.PROD.PRODUCT_MESSAGE";


sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl product_message.pl mw-id=<middleware ID>\n";
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
		my $componentField = libgmsec_perl::StringField->new("COMPONENT", "product_message");

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

		#o Create a ProductFile object with the product name,
		# description, version, file format, and the URI
		my $externalFile = libgmsec_perl::ProductFile->new("External File", "External File Description", "1.0.0", "TXT", "#hostname/dir/filename");

		my $fSize = 8;
		my $filePayload;
		for (my $i = 0; $i < $fSize; $i++)
		{
			$filePayload .= chr($i);
		}

		#o Create a ProductFile object with the product name,
		# description, version, format, binary array, and file size
		my $binaryFile = libgmsec_perl::ProductFile->new("File as Binary", "Binary File Description", "1.0.0", "BIN", $filePayload, $fSize);

		#o Create a Product File Message with the subject,
		# RESPONSE-STATUS Field value, Message type (publish, request,
		# or reply), PROD-TYPE Field value, PROD-SUBTYPE Field value,
		# and pass it the Specification object from the Connection
		# Manager
		my $productMessage = libgmsec_perl::ProductFileMessage->new($PROD_MESSAGE_SUBJECT, $libgmsec_perl::ResponseStatus::SUCCESSFUL_COMPLETION, $libgmsec_perl::Message::PUBLISH, "AUTO", "DM", $connManager->getSpecification());
		$productMessage->addProductFile($externalFile);
		$productMessage->addProductFile($binaryFile);

		$connManager->addStandardFields($productMessage);

		$connManager->publish($productMessage);

		libgmsec_perl::LogInfo("Published DEV message:\n" . $productMessage->toXML());

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
