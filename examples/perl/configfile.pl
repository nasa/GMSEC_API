#!/usr/bin/env perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# @file configfile.pl
#
# A Perl example demonstration of loading pre-defined subscriptions, Config
# objects, GMSEC Messages, and custom XML blocks from an xml file.  The
# configuration file used for this example program is "configfile.xml".

use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

sub main
{
	my @args = @_;
	if (scalar @args < 1)
	{
		print "usage: perl configfile.pl <filename>\n";
		return -1;
	}

	eval
	{
		initializeLogging();

		# load configuration file
		my $cfgFile = libgmsec_perl::ConfigFile->new();
		my $cfgFileName = @args[0];
		$cfgFile->load($cfgFileName);

		# retrieve config objects from the Config file
		my $c1 = $cfgFile->lookupConfig("config1");
		my $c2 = $cfgFile->lookupConfig("config2");

		# Display log of XML representation of Config objects
		libgmsec_perl::LogInfo("Config 1:\n" . $c1->toXML());
		libgmsec_perl::LogInfo("Config 2:\n" . $c2->toXML());

		# lookup subscription entry from configuration file, including excluded patterns related to such
		my $subEntry = $cfgFile->lookupSubscriptionEntry("events");

		libgmsec_perl::LogInfo("Subscription pattern: " . $subEntry->getPattern());

		while ($subEntry->hasNextExcludedPattern())
		{
			libgmsec_perl::LogInfo("Subscription excluded pattern: " . $subEntry->nextExcludedPattern());
		}

		# lookup a Message from the configuration file
		my $message = $cfgFile->lookupMessage("msg1");

		# Display XML representation of the message
		libgmsec_perl::LogInfo("Message:\n" . $message->toXML());

		# Obtain ConfigFile Iterator to examine all of the various
		# entries defined in the ConfigFile
		my $iter = $cfgFile->getIterator();

		# Acquire and display all Config entries
		while ($iter->hasNextConfig())
		{
			my $entry = $iter->nextConfig();

			libgmsec_perl::LogInfo("\nConfig Name: " . $entry->getName() . "\nConfig     :\n" . $entry->getConfig()->toXML());
		}

		# Acquire and display all Message entries
		while ($iter->hasNextMessage())
		{
			my $entry = $iter->nextMessage();

			libgmsec_perl::LogInfo("\nMessage Name: " . $entry->getName() . "\nMessage     :\n" . $entry->getMessage()->toXML());
		}

		# Acquire and display all subscription entries
		while ($iter->hasNextSubscription())
		{
			my $entry = $iter->nextSubscription();

			libgmsec_perl::LogInfo("\nSubscription Name : " . $entry->getName() . "\nSubscription Topic:\n" . $entry->getPattern());

			if($entry->hasNextExcludedPattern())
			{
				libgmsec_perl::LogInfo("\nExcluded Pattern: " . $entry->nextExcludedPattern());
			}
		}

		# Acquire and display all custom XML entries
		while ($iter->hasNextCustomElement())
		{
			my $element = $iter->nextCustomElement();

			libgmsec_perl::LogInfo("\nCustom XML Element:\n" . $element);
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
	}

	return 0;
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
