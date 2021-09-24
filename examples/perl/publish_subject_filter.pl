#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# @file publish_subject_filter.pl
#
# This file contains an example publishing a few messages with different
# subjects in order to demonstrate the subject filtering functionality of the
# API.
#
# This example program is intended to be run after starting the
# 'subscribe_subject_filter' example program.

use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

my $EXAMPLE_MESSAGE_SUBJECT  = "GMSEC.TEST.PUBLISH";
my $FILTERED_MESSAGE_SUBJECT = "GMSEC.TEST.FILTERED";

sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl publish_subject_filter.pl mw-id=<middleware ID>\n";
		return -1;
	}

	#o Load the command-line input into a GMSEC Config object
	# A Config object is basically a key-value pair map which is used to
	# pass configuration options into objects such as Connections,
	# ConnectionManagers, Subscribe and Publish function calls, Messages,
	# etc.
	my $config = libgmsec_perl::Config->new();

	# Split arguments and add them to the Config
	foreach my $arg (@args)
	{
		#print "$arg\n";
		my @argVals = split /=/, $arg;
		$config->addValue($argVals[0], $argVals[1]);
	}

    #o Since this example program uses an invalid message, we ensure the
    #  validation check is disabled.
    $config->addValue("gmsec-msg-content-validate-all", "false");

	#o If it was not specified in the command-line arguments, set LOGLEVEL
	# to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	# on the terminal/command line
	initializeLogging($config);


	#o Print the GMSEC API version number using the GMSEC Logging
	# interface
	# This is useful for determining which version of the API is
	# configured within the environment
	libgmsec_perl::LogInfo(libgmsec_perl::ConnectionManager::getAPIVersion());

	eval
	{
		#o Create a ConnectionManager object
		# This is the linchpin for all communications between the
		# GMSEC API and the middleware server
		my $connMgr = libgmsec_perl::ConnectionManager->new($config);

		#o Open the connection to the middleware
		libgmsec_perl::LogInfo("Opening the connection to the middleware server");
		$connMgr->initialize();

		#o Output middleware client library version
		libgmsec_perl::LogInfo($connMgr->getLibraryVersion());

		libgmsec_perl::LogInfo("Publishing two messages -- One will be received by the subscriber the other will be filtered out");

		#o Create a message which will be received by the subscriber
		# in this set of example programs
		{
			my $message = libgmsec_perl::Message->new($EXAMPLE_MESSAGE_SUBJECT, $libgmsec_perl::Message::PUBLISH);
			populateMessage($message);

			#o Publish the message to the middleware bus
			$connMgr->publish($message);

			#o Display the XML string representation of the Message for
			# the sake of review
			libgmsec_perl::LogInfo("Published message: " . $message->toXML());
		}

		#o Create a message which will NOT be received by the subscriber
		# in this set of example programs
		{
			my $message = libgmsec_perl::Message->new($FILTERED_MESSAGE_SUBJECT, $libgmsec_perl::Message::PUBLISH);
			populateMessage($message);

			#o Publish the message to the middleware bus
			$connMgr->publish($message);

			#o Display the XML string representation of the Message for
			# the sake of review
			libgmsec_perl::LogInfo("Published message: " . $message->toXML());
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

sub populateMessage
{
	my ($message) = @_;

	# Add Fields of varying data types to the Message
	my $value = 1;

	$message->addField(libgmsec_perl::CharField->new("CHAR-FIELD", "c"));
	$message->addField(libgmsec_perl::BooleanField->new("BOOL-FIELD-TRUE", 1));
	$message->addField(libgmsec_perl::BooleanField->new("BOOL-FIELD-TRUE", 0));
	$message->addField(libgmsec_perl::I8Field->new("I8-FIELD", $value));
	$message->addField(libgmsec_perl::I16Field->new("I16-FIELD", $value));
	$message->addField(libgmsec_perl::I32Field->new("I32-FIELD", $value));
	$message->addField(libgmsec_perl::I64Field->new("I64-FIELD", $value));
	$message->addField(libgmsec_perl::U8Field->new("U8-FIELD", $value));
	$message->addField(libgmsec_perl::U16Field->new("U16-FIELD", $value));
	$message->addField(libgmsec_perl::U32Field->new("U32-FIELD", $value));
	$message->addField(libgmsec_perl::U64Field->new("U64-FIELD", $value));
	$message->addField(libgmsec_perl::StringField->new("CHAR-FIELD", "This is a test"));
	$message->addField(libgmsec_perl::F32Field->new("F32-FIELD", (1 + 1.0 / $value)));
	$message->addField(libgmsec_perl::F64Field->new("F64-FIELD", (1 + 1.0 / $value)));
	$message->addField(libgmsec_perl::BinaryField->new("BIN-FIELD", "JLMNOPQ", 7));
}

exit(main(@ARGV));
