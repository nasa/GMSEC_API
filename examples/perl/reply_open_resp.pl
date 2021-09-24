#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.



# @file reply_open_resp.pl
#
# This file contains an example demonstrating how to utilize the open
# response functionality to expose reply messages to normal subscribers.
#
# This example program is intended to be run before executing the
# 'request_open_resp' example program.
#
# It is also recommended that you run a subscriber application
# (i.e. GMSEC_API/bin/gmsub) with the configuration option 'mw-expose-resp'
# set to true in order to see how any subscriber can receive reply messages
# while using the open-response functionality.

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
		print "usage: perl reply_open_resp.pl mw-id=<middleware ID>\n";
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

	#o Ensure that the open-response is enabled
	$config->addValue("GMSEC-REQ-RESP", "OPEN-RESP");


	#o Output GMSEC API version
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

		$connMgr->subscribe($OPEN_RESP_REQUEST_SUBJECT);

		#o Call receive() to synchronously retrieve a message that has
		# been received by the middleware client libraries
		# Timeout periods:
		# -1 - Wait forever
		#  0 - Return immediately
		# >0 - Time in milliseconds before timing out
		my $requestMsg = $connMgr->receive(-1);

		# Example error handling for calling receive() with a timeout
		if ($requestMsg)
		{
			#o Display the XML representation of the received message
			libgmsec_perl::LogInfo("Received a message\n" . $requestMsg->toXML());

			#o Double-check the Message type to ensure that it is a request
			if($requestMsg->getKind() == $libgmsec_perl::Message::REQUEST)
			{
				#o Get the name of the component who issued the request
				my $component = "";

				#o Construct a Reply message
				eval
				{
					my $compField = $requestMsg->getStringField("COMPONENT");
					$component = $compField->getValue();
				};
				if(isa($@, 'libgmsec_perl::Exception'))
				{
					libgmsec_perl::LogError($@->what());
				}

				#o Set Status Code to indicate Successful Completion.
				# The GMSEC Interface Specification Document defines 6
				# unique STATUS-CODE values:
				# 1 - Acknowledgement
				# 2 - Working/keep alive
				# 3 - Successful completion
				# 4 - Failed completion
				# 5 - Invalid request
				# 6 - Final message
				# If an asynchronous requestor is awaiting a reply, the
				# ReplyCallback in use will remain active for multiple
				# messages, so long as the messages it receives contain
				# a STATUS-CODE of either 1 or 2.
				my $status_code = "3";

				#o Set the reply subject.
				# See API Interface Specificaiton Document for
				# more information on Reply Message subjects.
				# Generally speaking, they are composed
				# accordingly:
				# [Spec].[Mission].[Satellite ID].
				# [Message Type].[Component Name].[Status Code]
				my $reply_subject = $OPEN_RESP_REPLY_SUBJECT . "." . $status_code;

				#o Create reply message
				my $replyMsg = libgmsec_perl::Message->new($reply_subject, $libgmsec_perl::Message::REPLY);

				#o Add fields to the reply message
				$replyMsg->addField(libgmsec_perl::StringField->new("ANSWER", "Yup, I'm here!"));
				$replyMsg->addField(libgmsec_perl::StringField->new("COMPONENT", $component));

				#o Display XML representation of the reply message
				libgmsec_perl::LogInfo("Prepared Reply:\n" . $replyMsg->toXML());

				#o Send Reply
				$connMgr->reply($requestMsg, $replyMsg);

				#o Wait for a moment to ensure that the reply is sent out
				for (my $i = 0; $i < 10; $i++)
				{
					libgmsec_perl::TimeUtil::millisleep(100);
				}
			}

			#o Destroy request message to release its memory
			$connMgr->release($requestMsg);
		}
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
