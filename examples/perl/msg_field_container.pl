#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.



# @file msg_field_container.pl
#
#
# This file contains an example demonstrating the creation of GMSEC Messages
# using the two available container types for Field objects: Binary Tree Map
# (STL map) or Hash Map (unordered map).

use strict;
use libgmsec_perl;
use feature "switch";

*isa = \&UNIVERSAL::isa;

sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl msg_field_container.pl <config-file>\n";
		print "\nPlease see the GMSEC_API/example/cpp/message_field_container directory for an example xml configuration file.\n";
		return -1;
	}

	#o If it was not specified in the command-line arguments, set LOGLEVEL
	# to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	# on the terminal/command line
	initializeLogging();

	eval
	{
		#o Demonstration of setting the configuration options via
		# configuration file
		createMsgUsingConfigFile(@args[0]);

		#o Demonstration of hard-coding the configuration options
		# using a Config object
		createMsgUsingConfigObject()
	};
	if(isa($@, 'libgmsec_perl::Exception'))
	{
		libgmsec_perl::LogError($@->what());
		return -1;
	}
	elsif($@)
	{
		libgmsec_perl::LogError($@);
	}

	return 0;
}

sub createMsgUsingConfigFile
{
	my ($filename) = @_;

	#o Load and parse configuration file
	my $cfgFile = libgmsec_perl::ConfigFile->new();
	$cfgFile->load($filename);

	#o Retrieve config from file
	my $cfg = $cfgFile->lookupConfig("config");

	#o Loop through three different examples outlined in the configuration
	# file provided with this example
	for (my $i = 0; $i < 3; ++$i)
	{
		#o Form message name (e.g. msg1) to reference.
		my $msgName = "msg" . ($i+1);

		#o Parse the message from the configuration file and load it
		# into a GMSEC Message
		my $msg = $cfgFile->lookupMessage($msgName);

		#o Display the message
		my $description;
		given ($i+1)
		{
			when (1) {$description = "Expecting TREE map (at size limit of 8 fields):";}
			when (2) {$description = "Using TREE but expecting HASH map (exceeds size limit of 8 fields):";}
			when (3) {$description = "Expecting HASH map (size limit is ignored):";}
		}

		displayMessage($msg, $description);
	}
}

sub createMsgUsingConfigObject
{
	my $cfg1 = libgmsec_perl::Config->new();
	my $cfg2 = libgmsec_perl::Config->new();
	my $cfg3 = libgmsec_perl::Config->new();
	my $empty = libgmsec_perl::Config->new();

	$cfg1->addValue("gmsec-msgfld-store-type", "tree");
	#o If the Message object is using a binary tree map to store fields,
	# the 'gmsec-msgfield-store-size' configuration option indicates how
	# many fields should be allowed to be stored before the Message
	# object converts to using a hash map (Default is 50).
	$cfg1->addValue("gmsec-msgfld-store-size", "10");

	$cfg2->addValue("gmsec-msgfld-store-type", "hash");

	$cfg3->addValue("gmsec-msgfld-store-type", "tree");    # container type
	$cfg3->addValue("gmsec-msgfld-store-size", "0");       # never rollover

	my $msg1 = buildMessage($cfg1,   9);   # uses bin-tree container map (under size limit)
	my $msg2 = buildMessage($cfg1,  10);   # uses bin-tree container map (at size limit)
	my $msg3 = buildMessage($cfg1,  11);   # uses hash container map (exceeds size limit)
	my $msg4 = buildMessage($cfg2,   5);   # default to using hash container map (size limit ignored)
	my $msg5 = buildMessage($cfg3,  55);   # always uses bin-tree container map
	my $msg6 = buildMessage($empty, 50);   # uses bin-tree container map (default size limit not exceeded)
	my $msg7 = buildMessage($empty, 51);   # uses hash container map (exceeds default size limit)

	displayMessage($msg1, "Message using TREE container map (under limit):");
	displayMessage($msg2, "Message using TREE container map (at max limit):");
	displayMessage($msg3, "Message was using TREE container map, but rolls over to HASH map (limit exceeded):");
	displayMessage($msg4, "Message using HASH container map (limit ignored):");
	displayMessage($msg5, "Message using TREE container map (limit ignored):");
	displayMessage($msg6, "Message using TREE container map (at limit):");
	displayMessage($msg7, "Message using HASH container map (limit exceeded):");
}

sub buildMessage
{
	my ($config, $numFields) = @_;

	my $msg = libgmsec_perl::Message->new("GMSEC.SYSTEST.TEST.REQ.DIR.LONG", $libgmsec_perl::Message::PUBLIDH, $config);

	if ($numFields > 0)
	{
		# Add Fields
		for (my $i = 0; $i < $numFields; ++$i)
		{
			my $oss = "FIELD-" . ($i < 10 ? "00" : ($i < 100 ? "0" : "")) . $i;

			$msg->addField(libgmsec_perl::U64Field->new($oss, $i));
		}
	}

	return $msg;
}

sub displayMessage
{
	my ($msg, $description) = @_;

	my $xml = $msg->toXML();
	libgmsec_perl::LogInfo($description . "\n" . $xml);
}

sub initializeLogging
{
	# Note: When the LOGLEVEL and LOGFILE parameters are set in a Config
	# object, it affects the logging level and output destination for all
	# logging macros in the entire API, regardless of whether that Config
	# object is kept around or not. In this case, we are creating a local
	# Config object just for the sake of initializing logging.
	my $config = libgmsec_perl::Config->new();

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
