#
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.
#

# 
# gmconfig.pl
# 
# A perl example demonstration of loading a configuration from a standard
# xml file.  The configuration file used for the example code is "gmconfig.xml".
# 


use strict;
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
	print "\nusage: perl gmconfig.pl <filename>\n";
}


sub init
{
	my $config = libgmsec_perl::Config->new();
	$config->addValue("LOGLEVEL", "INFO");
}


sub run
{
	my ($self, $cfgFilename) = @_;

	# Attempt to load configuration file
	libgmsec_perl::LogInfo("Attempting to load config: " . $cfgFilename);
	my $cfgFile = libgmsec_perl::ConfigFile->new();
	$cfgFile->load($cfgFilename);

	# Attempt to retrieve configurations from the configuration file
	my $config1 = $cfgFile->lookupConfig("config1");
	my $config2 = $cfgFile->lookupConfig("config2");

	# Display log of XML representation of configuration objects
	libgmsec_perl::LogInfo("Config 1:\n" . $config1->toXML());
	libgmsec_perl::LogInfo("Config 2:\n" . $config2->toXML());

	# Attempt to create a Connection object using a configuration object
	my $connection = libgmsec_perl::Connection::create($config2);

	# Destroy the connection object
	libgmsec_perl::Connection::destroy($connection);

	# Attempt to lookup subscription topic from configuration file
	my $pattern = $cfgFile->lookupSubscription("events");

	if (!$pattern)
	{
		die libgmsec_perl::Exception->new($libgmsec_perl::CONFIGFILE_ERROR,
						$libgmsec_perl::OTHER_ERROR_CODE,
						"Failed to acquire 'events' subscription topic");
	}

	libgmsec_perl::LogInfo("Subscription topic: " . $pattern);

	# Attempt to lookup message from configuration file
	my $message = $cfgFile->lookupMessage("msg1");

	# Display XML representation of the message
	libgmsec_perl::LogInfo("Message:\n" . $message->toXML());

	# Obtain Configuration File Iterator
	my $iter = $cfgFile->getIterator();

	# Acquire and display all configuration entries
	while ($iter->hasNextConfig())
	{
		my $configEntry = $iter->nextConfig();

		libgmsec_perl::LogInfo("\nConfig Name: " . $configEntry->getName()
			   . "\nConfig:\n" . $configEntry->getConfig()->toXML());
	}

	# Acquire and display all message entries
	while ($iter->hasNextMessage())
	{
		my $messageEntry = $iter->nextMessage();

		libgmsec_perl::LogInfo("\nMessage Name: " . $messageEntry->getName()
			   . "\nMessage:\n" . $messageEntry->getMessage()->toXML());
	}

	# Acquire and display all subscription entries
	while ($iter->hasNextSubscription())
	{
		my $subscriptionEntry = $iter->nextSubscription();

		libgmsec_perl::LogInfo("\nSubscription Name : " . $subscriptionEntry->getName()
			   . "\nSubscription Topic: " . $subscriptionEntry->getSubject());
	}

	# Acquire and display all custom XML entries
	while ($iter->hasNextCustomElement())
	{
		my $customElement = $iter->nextCustomElement();

		libgmsec_perl::LogInfo("\nCustom XML Element:\n" . $customElement);
	}
}


1;

__END__
