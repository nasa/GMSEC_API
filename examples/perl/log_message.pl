#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# @file log_message.pl
#
# This file contains an example outlining how to create and publish a
# GMSEC-compliant LOG message to the middleware bus using the Messaging
# Interface Standardization Toolkit (MIST) namespace ConnectionManager class
# function calls available for such operations.
#
# It is also recommended that you run a subscriber application
# (i.e. GMSEC_API/bin/gmsub) to see the LOG Messages which are published to the
# bus.

use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

my $GMSEC_SPEC_VERSION  = "201600";
my $LOG_MESSAGE_SUBJECT = "GMSEC.MISSION.SPACECRAFT.MSG.LOG";


sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl log_message.pl mw-id=<middleware ID>\n";
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

	libgmsec_perl::LogInfo(libgmsec_perl::ConnectionManager::getAPIVersion());

	eval
	{
		my $connManager = libgmsec_perl::ConnectionManager->new($config);

		libgmsec_perl::LogInfo("Opening the connection to the middleware server");
		$connManager->initialize();

		libgmsec_perl::LogInfo($connManager->getLibraryVersion());

		#o Begin the steps necessary to create a GMSEC-compliant LOG
		# message using the ConnectionManager

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
		my $componentField = libgmsec_perl::StringField->new("COMPONENT", "log_message");

		$definedFields->push($missionField);
		$definedFields->push($satIdField);
		$definedFields->push($facilityField);
		$definedFields->push($componentField);

		$connManager->setStandardFields($definedFields);

		#o Use MistMessage to construct a GMSEC LOG message based off
		# of the latest XML templates provided with the GMSEC API.
		# This will automatically populate the Message with all of the
		# Fields which have specific values defined for them in the XML
		# template files.  For more information on which Fields have
		# values defined for them, please review the XML template files
		# located in GMSEC_API/templates.
		#
		# Note: The second parameter is an identifier for the type of
		# message to construct.
		my $logMsg = libgmsec_perl::MistMessage->new($LOG_MESSAGE_SUBJECT, "MSG.LOG", $connManager->getSpecification());

		#o Add the LOG-specific fields to the LOG message
		#
		# Note: Since these Fields contain variable values which are
		# based on the context in which they are used, they cannot be
		# automatically populated using MistMessage.
		my $eventTime = "YYYY-DDD-HH:MM:SS.sss";
		libgmsec_perl::TimeUtil::formatTime(libgmsec_perl::TimeUtil::getCurrentTime(), $eventTime);

		$logMsg->addField(libgmsec_perl::I16Field->new("SEVERITY", 1));
		$logMsg->setValue("MSG-TEXT", "Creating an example GMSEC LOG Message");
		$logMsg->setValue("OCCURRENCE-TYPE", "SYS");
		$logMsg->setValue("SUBCLASS", "AST");
		$logMsg->setValue("EVENT-TIME", $eventTime);

		#o Add the standard fields to the LOG message
		$connManager->addStandardFields($logMsg);

		$connManager->publish($logMsg);

		libgmsec_perl::LogInfo("Published LOG message:\n" . $logMsg->toXML());

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
