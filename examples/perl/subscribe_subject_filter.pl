#!/usr/bin/env perl

# Copyright 2007-2018 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# @file subscribe_subject_filter.pl
#
# This file contains a simple example outlining how to subscribe
# to and retrieve messages from the middleware bus while using subject
# exclusion filtering to reduce the scope of a wildcarded subject subscription.
#
# This example program is intended to be run before starting the
# 'publish_subject_filter' example program.

use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

my $EXAMPLE_SUBSCRIPTION_SUBJECT = "GMSEC.>";
my $FILTERED_MESSAGE_SUBJECT = "GMSEC.TEST.FILTERED";

sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl subscribe_subject_filter.pl mw-id=<middleware ID>\n";
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

		#o Set up a subscription to listen for Messages with the topic
		# "GMSEC.TEST.PUBLISH" which are published to the middleware
		# bus
		# Subscription subject wildcard syntax:
		## - Matches any one token separated by periods in a subject
		# > - Reading left to right, matches everything up to and ONE
		#     or more tokens in a subject
		# + - Reading left to right, matches everything up to and ZERO
		#     or more tokens in a subject
		# For more information on wildcards, please see the GMSEC API
		# User's Guide
		libgmsec_perl::LogInfo("Subscribing to the topic: " . $EXAMPLE_SUBSCRIPTION_SUBJECT);
		$connMgr->subscribe($EXAMPLE_SUBSCRIPTION_SUBJECT);

		#o Add a specific message subject to the Connection's exclusion
		# filter
		libgmsec_perl::LogInfo("Adding a filter rule for the topic: " . $FILTERED_MESSAGE_SUBJECT);
		$connMgr->excludeSubject($FILTERED_MESSAGE_SUBJECT);

		#o Call receive() to synchronously retrieve a message that has
		# been received by the middleware client libraries
		# Timeout periods:
		# -1 - Wait forever
		#  0 - Return immediately
		# >0 - Time in milliseconds before timing out
		libgmsec_perl::LogInfo("Waiting to receive a Message");
		my $message = $connMgr->receive(-1);

		# Example error handling for calling receive() with a timeout
		if ($message != undef)
		{
			libgmsec_perl::LogInfo("Received message:\n" . $message->toXML());
		}

		libgmsec_perl::LogInfo("Waiting 5 seconds to demonstrate that a second message will not be received");
		$message = $connMgr->receive(5000);

		if ($message != undef)
		{
			libgmsec_perl::LogError("Unexpectedly received a filtered message:\n" . $message->toXML());
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

exit(main(@ARGV));
