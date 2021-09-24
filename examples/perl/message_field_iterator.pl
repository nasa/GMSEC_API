#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# @file message_field_iterator.pl
#
# This file contains a simple example demonstrating the use of the
# MessageFieldIterator class to iterate over the Fields of a Message.
#
# This example program is intended to be run prior to executing any example
# program which publishes a GMSEC message (e.g. The publish example).


use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

my $EXAMPLE_SUBSCRIPTION_SUBJECT = "GMSEC.TEST.PUBLISH";


sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl subscribe.pl mw-id=<middleware ID>\n";
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
		# * - Matches any one token separated by periods in a subject
		# > - Reading left to right, matches everything up to and ONE
		#	 or more tokens in a subject
		# + - Reading left to right, matches everything up to and ZERO
		#	 or more tokens in a subject
		# For more information on wildcards, please see the GMSEC API
		# User's Guide
		libgmsec_perl::LogInfo("Subscribing to the topic: " . $EXAMPLE_SUBSCRIPTION_SUBJECT);
		$connMgr->subscribe($EXAMPLE_SUBSCRIPTION_SUBJECT);

		libgmsec_perl::LogInfo("Listening for messages indefinitely, press CTRL+C to exit the program.");

		while (1)
		{
			#o Call receive() to synchronously retrieve a message that has
			# been received by the middleware client libraries
			# Timeout periods:
			# -1 - Wait forever
			#  0 - Return immediately
			# >0 - Time in milliseconds before timing out
			my $message = $connMgr->receive(-1);

			# Example error handling for calling receive() with a timeout
			if ($message != undef)
			{
				libgmsec_perl::LogInfo("Received a message with subject:\n" . $message->getSubject());

				libgmsec_perl::LogInfo("Field Name (Field Type): Field Value");
				my $iter = $message->getFieldIterator();

				while ($iter->hasNext())
				{
					my $field = $iter->next();

					# Extract the Field Name, Type, and Value (As
					# a string, to print)
					#
					# Note: 'getter' functions are also defined for
					# Integer, Unsigned Integer, and Double values.
					libgmsec_perl::LogInfo($field->getName() . " (" . typeToString($field->getType()) . "): " . $field->getStringValue());

					# Field objects can also be converted to
					# specific Field types prior to retrieval of
					# the value contained in the Field::  This is
					# useful for ensuring that data types do not
					# lose any level of precision, but requires
					# a more intricate implementation.
					#
					# See the getFieldValue() function (commented
					# out at the bottom of this example program)
					# for an example of how field can be done.
				}

				# Dispose of message
				$connMgr->release($message);
			}
		}

		#o Disconnect from the middleware and clean up the Connection
		# Note that with the infinite loop above, this will technically never
		# be called.  This is here just to demonstrate proper cleanup techniques
		# in a normal script.
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

sub typeToString
{
	my ($type) = @_;

	if ($type == $libgmsec_perl::Field::BOOL_TYPE)
	{
		return "BOOL";
	}

	if ($type == $libgmsec_perl::Field::BIN_TYPE)
	{
		return "BIN";
	}

	if ($type == $libgmsec_perl::Field::CHAR_TYPE)
	{
		return "CHAR";
	}

	if ($type == $libgmsec_perl::Field::I8_TYPE)
	{
		return "I8";
	}

	if ($type == $libgmsec_perl::Field::I16_TYPE)
	{
		return "I16";
	}

	if ($type == $libgmsec_perl::Field::I32_TYPE)
	{
		return "I32";
	}

	if ($type == $libgmsec_perl::Field::I64_TYPE)
	{
		return "I64";
	}

	if ($type == $libgmsec_perl::Field::F32_TYPE)
	{
		return "F32";
	}

	if ($type == $libgmsec_perl::Field::F64_TYPE)
	{
		return "F64";
	}

	if ($type == $libgmsec_perl::Field::STRING_TYPE)
	{
		return "STRING";
	}

	if ($type == $libgmsec_perl::Field::U8_TYPE)
	{
		return "U8";
	}

	if ($type == $libgmsec_perl::Field::U16_TYPE)
	{
		return "U16";
	}

	if ($type == $libgmsec_perl::Field::U32_TYPE)
	{
		return "U32";
	}

	if ($type == $libgmsec_perl::Field::U64_TYPE)
	{
		return "U64";
	}

	throw libgmsec_perl::Exception($libgmsec_perl::FIELD_ERROR, $libgmsec_perl::UNKNOWN_FIELD_TYPE, "Unsupported Field TYPE): " + $type);
}

exit(main(@ARGV));
