#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# @file request.pl
#
# This file contains an example demonstrating how to issue a request message
# and handle a coinciding reply message. This example program is intended to
# be run after starting up either the 'reply' or 'reply_async' example program.

use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

my $DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST";


sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl request.pl mw-id=<middleware ID>\n";
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
	libgmsec_perl::LogInfo(libgmsec_perl::Connection::getAPIVersion());

	eval
	{
		#o Create the ConnectionManager
		my $connMgr = libgmsec_perl::ConnectionManager->new($config);

		#o Open the connection to the middleware
		libgmsec_perl::LogInfo("Opening the connection to the middleware server");
		$connMgr->initialize();

		#o Output middleware client library version
		libgmsec_perl::LogInfo($connMgr->getLibraryVersion());

		#o Output information
		libgmsec_perl::LogInfo("Issuing a request using the DEFAULT_REQUEST_SUBJECT '" . $DEFAULT_REQUEST_SUBJECT . "'");

		#o Create message
		my $requestMsg = libgmsec_perl::Message->new($DEFAULT_REQUEST_SUBJECT, $libgmsec_perl::Message::REQUEST);

		#o Add fields to message
		$requestMsg->addField(libgmsec_perl::StringField->new("QUESTION", "Is there anyone out there?"));
		$requestMsg->addField(libgmsec_perl::StringField->new("COMPONENT", "request"));

		#o Display XML representation of request message
		libgmsec_perl::LogInfo("Sending request message:\n" . $requestMsg->toXML());

		#o Send Request Message
		# Timeout periods:
		# -1 - Wait forever
		#  0 - Return immediately
		# >0 - Time in milliseconds before timing out
		my $replyMsg = $connMgr->request($requestMsg, -1, -1);

		# Example error handling for calling request() with a timeout
		if($replyMsg)
		{
			# Display the XML string representation of the reply
			libgmsec_perl::LogInfo("Received replyMsg:\n" . $replyMsg->toXML());

			#o Destroy the replyMsg message
			$connMgr->release($replyMsg);
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
