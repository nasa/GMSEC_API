#
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.
#

# 
# gm_optimized_msg.pl
# 
# A perl example demonstration of creating GMSEC Messages using two available
# container types: Binary Tree Map (STL map) or Hash Map (unordered map).
# 


use strict;
use feature qw(say switch);

use Util;

use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

{
	my $self = bless({});

	# If no arguments were provided, print usage and exit with an erroneous code value
	if (scalar(@ARGV) < 1)
	{
		$self->printUsage();
		exit -1;
	}


	# Initialize logging in the GMSEC API in order to display output
	$self->init();

	eval
	{
		$self->run($ARGV[0]);
	};
	if ($@)
	{
		print "caught: $@";
	}
}


sub printUsage
{
	print "\nusage: perl gm_optimized_msg.pl <filename>\n";
}


sub init
{
	my $config = libgmsec_perl::Config->new();
	$config->addValue("LOGLEVEL", "INFO");
}


sub run
{
	my ($self, $cfgFilename) = @_;

	eval
	{
		# Attempt to load configuration file
		my $cfgFile = libgmsec_perl::ConfigFile->new();
		$cfgFile->load($cfgFilename);

		# Retrieve config from file
		my $cfg = $cfgFile->lookupConfig("config");

		$self->createMsgUsingConfigFile($cfgFile, $cfg);

		$self->createMsgUsingConfigObject();
	};
	if (isa($@, 'libgmsec_perl::Exception'))
	{
		libgmsec_perl::LogError($@->what());
	}
	elsif ($@)
	{
		print "caught: $@";
	}
}


sub createMsgUsingConfigFile
{
	my ($self, $cfgFile) = @_;

	for (my $i = 0; $i < 3; ++$i)
	{
		# Form message name (e.g. msg1) to reference.
		my $msgName = "msg" . ($i+1);

		# Reference configuration file for message
		my $msg = $cfgFile->lookupMessage($msgName);

		# Display message
		my $hdr = 0;
		given ($i+1)
		{
			when (1)
			{
				$hdr = "Expecting TREE map (at size limit of 8 fields):";
			}
			when (2)
			{
				$hdr = "Using TREE but expecting HASH map (exceeds size limit of 8 fields):";
			}
			when (3)
			{
				$hdr = "Expecting HASH map (size limit is ignored):";
			}
		}

		displayMessage($msg, $hdr);
	}
}


sub createMsgUsingConfigObject
{
	my ($self, $cfgFile) = @_;
	my $cfg1 = libgmsec_perl::Config->new();
	my $cfg2 = libgmsec_perl::Config->new();
	my $cfg3 = libgmsec_perl::Config->new();
	my $empty = libgmsec_perl::Config->new();

	$cfg1->addValue("gmsec-msgfld-store-type", "tree");
	$cfg1->addValue("gmsec-msgfld-store-size", "10");      # max storage size before rollover (default is 50)

	$cfg2->addValue("gmsec-msgfld-store-type", "hash");

	$cfg3->addValue("gmsec-msgfld-store-type", "tree");    # storage type
	$cfg3->addValue("gmsec-msgfld-store-size", "0");       # never rollover

	my $msg1 = buildMessage($cfg1,   9);    # uses bin-tree storage map (under size limit)
	my $msg2 = buildMessage($cfg1,  10);    # uses bin-tree storage map (at size limit)
	my $msg3 = buildMessage($cfg1,  11);    # uses hash storage map (exceeds size limit)
	my $msg4 = buildMessage($cfg2,   5);    # default to using hash storage map (size limit ignored)
	my $msg5 = buildMessage($cfg3,  55);    # always uses bin-tree storage map
	my $msg6 = buildMessage($empty, 50);    # uses bin-tree storage map (default size limit not exceeded)
	my $msg7 = buildMessage($empty, 51);    # uses hash storage map (exceeds default size limit)

	displayMessage($msg1, "Message using TREE storage map (under limit):");
	displayMessage($msg2, "Message using TREE storage map (at max limit):");
	displayMessage($msg3, "Message was using TREE storage map, but rolls over to HASH map (limit exceeded):");
	displayMessage($msg4, "Message using HASH storage map (limit ignored):");
	displayMessage($msg5, "Message using TREE storage map (limit ignored):");
	displayMessage($msg6, "Message using TREE storage map (at limit):");
	displayMessage($msg7, "Message using HASH storage map (limit exceeded):");
}


sub buildMessage
{
	my ($config, $numFields) = @_;
	my $msg = libgmsec_perl::Message->new("GMSEC.SYSTEST.TEST.REQ.DIR.LONG", $libgmsec_perl::Message::PUBLISH, $config);

	if ($numFields > 0)
	{
		# Add Fields
		for (my $i = 0; $i < $numFields; ++$i)
		{
			my $fieldName = "FIELD-" . ($i < 10 ? "00" : ($i < 100 ? "0" : "")) . $i;

			my $field = libgmsec_perl::U64Field->new($fieldName, $i);
			$msg->addField($field);
		}
	}

	return $msg;
}


sub displayMessage
{
	my ($msg, $header) = @_;
	my $xml = $msg->toXML();

	if ($header)
	{
		libgmsec_perl::LogInfo($header . "\n" . $xml);
	}
	else
	{
		libgmsec_perl::LogInfo($xml);
	}
}
