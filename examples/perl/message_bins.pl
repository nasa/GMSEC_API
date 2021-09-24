#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# @file message_bins.pl
#
# This file contains an example describing how to aggregate multiple messages
# into a singular message sent to the middleware server.  This functionality
# is useful for cutting down on the number of messages sent across the
# middleware bus, if there is an application in the system which is publishing
# an exorbitantly large number of messages.
#
# It is recommended that you execute the application GMSEC_API/bin/gmsub
# prior to running this program in order to see that the aggregated messages
# still appear on the receiving end as if they were individual messages.
#
# Please note that if an aggregated message is sent by the GMSEC API, any
# receivers will need to be running at least version 4.2 of the API in order
# to de-aggregate the messages. Older versions can receive the messages
# without crashing, but they will be unusable.

use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

my $EXAMPLE_MESSAGE_SUBJECT     = "GMSEC.AGGREGATE.PUBLISH";
my $EXAMPLE_BIN_EXCLUDE_SUBJECT = "GMSEC.BIN-EXCLUDE.PUBLISH";

sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl message_bins.pl mw-id=<middleware ID>\n";
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

	#o Enable Message Binning
	$config->addValue("GMSEC-USE-MSG-BINS", "true");

	#o Specify the number of messages to be aggregated prior to publishing
	# the aggregate message to the middleware server (This applies to all
	# of the messages which match the subject(s) provided in the
	# GMSEC-MSG-BIN-SUBJECT-N configuration parameters
	# Note: The aggregate message will be sent to the middleware server
	# immediately upon this many messages being published, regardless of
	# the value supplied for GMSEC-MSG-BIN-TIMEOUT.
	$config->addValue("GMSEC-MSG-BIN-SIZE", "10");

	#o Specify a timeout (in milliseconds) for the aggregate message to be
	# sent to the middleware server
	# Note: The aggregate message will be sent to the middleware server
	# after this period of time if the message bin does not fill up (per
	# the value provided for GMSEC-MSG-BIN-SIZE) prior to this timeout
	$config->addValue("GMSEC-MSG-BIN-TIMEOUT", "5000");

	#o Specify the subjects to aggreate messages for.
	# Note: Subscription wildcard syntax can be used here, and has been
	# supported since GMSEC API version 4.3.
	$config->addValue("GMSEC-MSG-BIN-SUBJECT-1", "GMSEC.*.PUBLISH");

	#o Specify any subjects that should be excluded from being aggregated
	# This is useful if a wildcard subscription is provided in one of the
	# GMSEC-MSG-BIN-SUBJECT-N parameters.
	$config->addValue("GMSEC-MSG-BIN-EXCLUDE-SUBJECT-1", $EXAMPLE_BIN_EXCLUDE_SUBJECT);

	#o Since this example program uses an invalid message, we ensure the
	#  validation check is disabled.
	$config->addValue("gmsec-msg-content-validate-all", "false");

	#o If it was not specified in the command-line arguments, set LOGLEVEL
	# to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	# on the terminal/command line
	initializeLogging($config);


	libgmsec_perl::LogInfo(libgmsec_perl::ConnectionManager::getAPIVersion());

	eval
	{
		my $connMgr = libgmsec_perl::ConnectionManager->new($config);

		libgmsec_perl::LogInfo("Opening the connection to the middleware server");
		$connMgr->initialize();

		libgmsec_perl::LogInfo($connMgr->getLibraryVersion());

		{
			#o Create a message
			my $message = libgmsec_perl::Message->new($EXAMPLE_MESSAGE_SUBJECT, $libgmsec_perl::Message::PUBLISH);

			for (my $i = 0; $i < 5; $i++)
			{
				populateMessage($message, $i+1);

				#o Publish the message to the middleware bus
				$connMgr->publish($message);

				#o Display the XML string representation of the Message for
				# the sake of review
				libgmsec_perl::LogInfo("Published message: " . $message->toXML());
			}
		}

		{
			#o Create a message
			my $message = libgmsec_perl::Message->new($EXAMPLE_BIN_EXCLUDE_SUBJECT, $libgmsec_perl::Message::PUBLISH);

			populateMessage($message, 1);

			#o Publish the message to the middleware bus
			# Note: When calling publish(), if a message does NOT match
			# one of the subjects to be aggregated, it will be immediately
			# published
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
	my ($message, $value) = @_;

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
