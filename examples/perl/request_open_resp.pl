#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.



# @file request_open_resp.pl
#
# This file contains an example demonstrating how to issue a request message
# and handle a coinciding reply message when using the open-response
# configuration option. This example program is intended to be run after
# starting up the 'reply_open_resp' example program.
#
# It is also recommended that you run a subscriber application
# (i.e. GMSEC_API/bin/gmsub) with the configuration option 'mw-expose-resp'
# set to true in order to see how any subscriber can receive reply messages
# while using the open-response functionality. Note that by setting the
# configuration option 'GMSEC-REQ-RESP' to 'open-resp' in the requester
# automatically turns on the 'mw-expose-resp' option.

use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

my $OPEN_RESP_REQUEST_SUBJECT = "GMSEC.MISSION.SAT_ID.RESP.REQUEST_OPENRESP";
my $OPEN_RESP_REPLY_SUBJECT   = "GMSEC.MISSION.SAT_ID.RESP.REPLY_OPENRESP";

sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl request_open_resp.pl mw-id=<middleware ID>\n";
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

	#o Ensure that the open-response is enabled
 	# Note: Other subscribing applications should set the configuration
 	# option 'mw-expose-resp' to 'true' in order to receive exposed replies
 	# By setting the configuration option 'GMSEC-REQ-RESP' to 'open-resp'
 	# here, it automatically enables the 'mw-expose-resp' option.
	$config->addValue("GMSEC-REQ-RESP", "OPEN-RESP");

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
		my $connMgr = libgmsec_perl::ConnectionManager->new($config);

		#o Open the connection to the middleware
		libgmsec_perl::LogInfo("Opening the connection to the middleware server");
		$connMgr->initialize();

		#o Output middleware client library version
		libgmsec_perl::LogInfo($connMgr->getLibraryVersion());

		#o Subscribe to the bus in preparation to receive the
		# open-response message (Because it will not be routed
		# to the reqeust() call)
		my $reply_subject = $OPEN_RESP_REPLY_SUBJECT . ".*";
		$connMgr->subscribe($reply_subject);

		#o Output information
		libgmsec_perl::LogInfo("Issuing a request using the subject '" . $OPEN_RESP_REQUEST_SUBJECT . "'");

		#o Create message
		my $requestMsg = libgmsec_perl::Message->new($OPEN_RESP_REQUEST_SUBJECT, $libgmsec_perl::Message::REQUEST);

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
		my $replyMsg = $connMgr->request($requestMsg, 1000, $libgmsec_perl::GMSEC_REQUEST_REPUBLISH_NEVER);

		# Example error handling for calling request() with a timeout
		if ($replyMsg)
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
