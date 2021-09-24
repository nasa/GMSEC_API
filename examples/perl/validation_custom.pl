#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# @file validation_custom.py
 
# This file contains an example demonstrating how to implement additional
# Message validation logic in addition to that which the GMSEC API provides.

use strict;
use libgmsec_perl;


*isa = \&UNIVERSAL::isa;


my $HB_MSG_SUBJECT     = "GMSEC.MISSION.SATELLITE.MSG.HB.VALIDATION-CUSTOM";
my $ALT_HB_MSG_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.VALIDATION-CUSTOM.HB";


# CustomMessageValidator
{
	package CustomMessageValidator;
	use base 'libgmsec_perl::MessageValidator';

	sub new
	{
		# Initialize the base package (libgmsec_perl::MessageValidator)
		my $class = shift;
		my $self = $class->SUPER::new();
		return $self;
	}

	sub validateMessage
	{
		my ($self, $msg) = @_;

		my $status = libgmsec_perl::Status->new();

		# Get message type and subtype
		my $msgType = "";
		my $subtype = "";

		if ($msg->hasField("MESSAGE-TYPE"))
		{
			$msgType = $msg->getStringValue("MESSAGE-TYPE");
		}
		if ($msg->hasField("C2CX-SUBTYPE"))
		{
			$subtype = $msg->getStringValue("C2CX-SUBTYPE");
		}
		elsif ($msg->hasField("MESSAGE-SUBTYPE"))
		{
			$subtype = $msg->getStringValue("MESSAGE-SUBTYPE");
		}

		# Ensure we have a Heartbeat message and it contains the PUB-RATE field
		if (($msgType cmp "MSG") == 0 and ($subtype cmp "HB") == 0 and ($msg->hasField("PUB-RATE")))
		{
            my $pubRate = $msg->getIntegerValue("PUB-RATE");

            if ($pubRate < 10 or $pubRate > 60)
			{
                $status->set($libgmsec_perl::MSG_ERROR, $libgmsec_perl::VALUE_OUT_OF_RANGE, "PUB-RATE field does not have a valid value");
			}
		}
        else
		{
            $status->set($libgmsec_perl::MSG_ERROR, $libgmsec_perl::INVALID_MSG, "Non-Heartbeat message received");
		}

        return $status;
	}
} # end of CustomMessageValidator declaration


sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl validation_custom.pl mw-id=<middleware ID>\n";
		return -1;
	}

    # Load the command-line input into a GMSEC Config object
	my $config = libgmsec_perl::Config->new();

	foreach my $arg (@args)
	{
		my @argVals = split /=/, $arg;
		$config->addValue($argVals[0], $argVals[1]);
	}

    # If it was not specified in the command-line arguments, set LOGLEVEL
    # to 'INFO' and LOGFILE to 'stdout' to allow the program report output
    # on the terminal/command line
	initializeLogging($config);


    # Enable Message validation.  This parameter is "false" by default.
    $config->addValue("gmsec-msg-content-validate-send", "true");
    $config->addValue("gmsec-validation-level", "3");

    libgmsec_perl::LogInfo("API version: " . libgmsec_perl::ConnectionManager::getAPIVersion());

	my $validator = CustomMessageValidator->new();

	eval
	{
		my $connMgr = libgmsec_perl::ConnectionManager->new($config);

		# Register custom message validator
		$connMgr->registerMessageValidator($validator);

		# Connect to the bus
		$connMgr->initialize();

		libgmsec_perl::LogInfo("Middleware version: " . $connMgr->getLibraryVersion());

		# Set up standard/common fields used with all messages
		my $spec = $connMgr->getSpecification();
		my $specVersion = $spec->getVersion();
		setupStandardFields($specVersion);

		# Create Heartbeat Message
		# Note: Message subject and schema ID vary depending on the specification in use
		my $subject  = "";
		my $schemaID = "";
		if ($specVersion > $libgmsec_perl::GMSEC_ISD_2018_00)
		{
			$subject  = $HB_MSG_SUBJECT;
			$schemaID = "MSG.HB";
		}
		else
		{
			$subject  = $ALT_HB_MSG_SUBJECT;
			$schemaID = "MSG.C2CX.HB";
		}

		my $msg = libgmsec_perl::MistMessage->new($subject, $schemaID, $connMgr->getSpecification());

		# Add PUB-RATE field with illegal value
		$msg->setValue("PUB-RATE", "5");

		# For very old specifications, we need to add a MSG-ID field
		if ($specVersion <= $libgmsec_perl::GMSEC_ISD_2014_00)
		{
			$msg->addField("MSG-ID", "12345");
		}

		# Attempt to publish malformed message
		eval
		{
			libgmsec_perl::LogInfo("Attempting to publish malformed message...");

			$connMgr->publish($msg);

			libgmsec_perl::.LogWarning("Was expecting an error");
		};
		if (isa($@, 'libgmsec_perl::Exception'))
		{
			my $error = $@;
			# We expect to encounter error with the PUB-RATE field
			libgmsec_perl::LogInfo("This is an expected error:\n" . $error->what());
		}

		# Fix PUB-RATE field with legal value
		$msg->setValue("PUB-RATE", "15");

		# Publish a good message
		eval
		{
			libgmsec_perl::LogInfo("Attempting to publish good message...");
			$connMgr->publish($msg);

			libgmsec_perl::LogInfo("Message published!");
		};
		if (isa($@, 'libgmsec_perl::Exception'))
		{
			my $error = $@;
			libgmsec_perl::LogInfo("Unexpected error:\n" . $error->what());
		}

		# Disconnect from the middleware and clean up the Connection
		$connMgr->cleanup();

		# Clear standard/common fields used with all messages
		clearStandardFields();
	};
	if (isa($@, 'libgmsec_perl::Exception'))
	{
		libgmsec_perl::LogInfo($@->what());
		return -1;
	}

	return 0;
}


sub initializeLogging
{
	my ($config) = @_;

	my $logLevel = $config->getValue("LOGLEVEL");
	my $logFile  = $config->getValue("LOGFILE");

	if (!$logLevel)
	{
		$config->addValue("LOGLEVEL", "INFO");
	}
	if (! $logFile)
	{
		$config->addValue("LOGFILE", "STDERR");
	}
}


sub setupStandardFields
{
	my ($specVersion) = @_;

	my $definedFields = libgmsec_perl::FieldList->new();

	my $mission       = libgmsec_perl::StringField->new("MISSION-ID", "MISSION");
	my $constellation = libgmsec_perl::StringField->new("CONSTELLATION-ID", "CONSTELLATION");
	my $satIdPhys     = libgmsec_perl::StringField->new("SAT-ID-PHYSICAL", "SATELLITE");
	my $satIdLog      = libgmsec_perl::StringField->new("SAT-ID-LOGICAL", "SATELLITE");
	my $facility      = libgmsec_perl::StringField->new("FACILITY", "GMSEC-LAB");
	my $component     = libgmsec_perl::StringField->new("COMPONENT", "VALIDATION-CUSTOM");
	my $domain1       = libgmsec_perl::StringField->new("DOMAIN1", "DOMAIN1");
	my $domain2       = libgmsec_perl::StringField->new("DOMAIN2", "DOMAIN2");

	$definedFields->push($mission);
	$definedFields->push($constellation);
	$definedFields->push($satIdPhys);
	$definedFields->push($satIdLog);
	$definedFields->push($facility);
	$definedFields->push($component);

	if ($specVersion >= $libgmsec_perl::GMSEC_ISD_2018_00)
	{
		$definedFields->push($domain1);
		$definedFields->push($domain2);
	}

	libgmsec_perl::MistMessage::setStandardFields($definedFields);
}


sub clearStandardFields
{
	libgmsec_perl::MistMessage::clearStandardFields();
}


exit(main(@ARGV));
