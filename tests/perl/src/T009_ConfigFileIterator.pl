#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


my $cfgFile    = undef;
my $msgFactory = undef;


sub Test_ConfigFileIterator
{
	my ($test) = @_;

	$test->setDescription("Test ConfigFileIterator");

	setup($test);

	test_iterator($test);
	test_reset($test);

	teardown($test);
}


sub setup
{
	my ($test) = @_;

	$cfgFile    = libgmsec_perl::ConfigFile->new();
	$msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	$cfgFile->addSubscriptionEntry( libgmsec_perl::SubscriptionEntry->new("AllMessages", "GMSEC.>") );
	$cfgFile->addSubscriptionEntry( libgmsec_perl::SubscriptionEntry->new("LogMessages", "GMSEC.*.*.MSG.LOG.+") );

	$cfgFile->addConfig("ActiveMQ", libgmsec_perl::Config->new("mw-id=activemq39 server=tcp://localhost:61616", $libgmsec_perl::DataType_KEY_VALUE_DATA));
	$cfgFile->addConfig("Bolt", libgmsec_perl::Config->new("mw-id=bolt server=localhost", $libgmsec_perl::DataType_KEY_VALUE_DATA));

	$cfgFile->addMessage("Heartbeat", $msgFactory->createMessage("HB"));
	$cfgFile->addMessage("Log", $msgFactory->createMessage("LOG"));
}


sub teardown
{
	my ($test) = @_;

	libgmsec_perl::MessageFactory::destroy($msgFactory);
}


sub test_iterator
{
	my ($test) = @_;

	my $iter = $cfgFile->getIterator();

	libgmsec_perl::logInfo("Test hasNextSubscription() and nextSubscription()");
	my $numSubscriptions = 0;
	while ($iter->hasNextSubscription())
	{
		my $entry = $iter->nextSubscription();

		$numSubscriptions += 1;

		if ($numSubscriptions == 1)
		{
			$test->check("Unexpected subscription entry 1 name", $entry->getName() eq "AllMessages");
			$test->check("Unexpected subscription entry 1 pattern", $entry->getPattern() eq "GMSEC.>");
		}
		else
		{
			$test->check("Unexpected subscription entry 2 name", $entry->getName() eq "LogMessages");
			$test->check("Unexpected subscription entry 2 pattern", $entry->getPattern() eq "GMSEC.*.*.MSG.LOG.+");
		}
	}

	$test->check("Unexpected number of subscription entries", $numSubscriptions == 2);


	libgmsec_perl::logInfo("Test hasNextConfig() and nextConfig()");
	my $numConfigs = 0;
	while ($iter->hasNextConfig())
	{
		my $entry = $iter->nextConfig();

		$numConfigs += 1;

		if ($numConfigs == 1)
		{
			$test->check("Unexpected config entry 1 name", $entry->getName() eq "ActiveMQ");
			$test->check("Unexpected config entry mw-id", $entry->getConfig()->getValue("mw-id") eq "activemq39");
			$test->check("Unexpected config entry server", $entry->getConfig()->getValue("server") eq "tcp://localhost:61616");
		}
		else
		{
			$test->check("Unexpected config entry 2 name", $entry->getName() eq "Bolt");
			$test->check("Unexpected config entry mw-id", $entry->getConfig()->getValue("mw-id") eq "bolt");
			$test->check("Unexpected config entry server", $entry->getConfig()->getValue("server") eq "localhost");
		}
	}

	$test->check("Unexpected number of config entries", $numConfigs == 2);


	libgmsec_perl::logInfo("Test hasNextMessage() and nextMessage()");
	my $numMessages = 0;
	while ($iter->hasNextMessage())
	{
		my $entry = $iter->nextMessage();

		$numMessages += 1;

		if ($numMessages == 1)
		{
			$test->check("Unexpected message entry 1 name", $entry->getName() eq "Heartbeat");
			$test->check("Unexpected message entry 1", $entry->getMessage()->toXML() eq $msgFactory->createMessage("HB")->toXML());
		}
		else
		{
			$test->check("Unexpected message entry 2 name", $entry->getName() eq "Log");
			$test->check("Unexpected message entry 2", $entry->getMessage()->toXML() eq $msgFactory->createMessage("LOG")->toXML());
		}
	}

	$test->check("Unexpected number of message entries", $numMessages == 2);
}


sub test_reset
{
	my ($test) = @_;

	libgmsec_perl::logInfo("Test reset()");

	my $iter = $cfgFile->getIterator();

	$test->check("Expected to have subscriptions", $iter->hasNextSubscription());
	$test->check("Expected to have configs", $iter->hasNextConfig());
	$test->check("Expected to have messages", $iter->hasNextMessage());

	while ($iter->hasNextSubscription())
	{
		$iter->nextSubscription();
	}
	while ($iter->hasNextConfig())
	{
		$iter->nextConfig();
	}
	while ($iter->hasNextMessage())
	{
		$iter->nextMessage();
	}

	$test->check("Expected NOT to have additional subscriptions", $iter->hasNextSubscription() == 0);
	$test->check("Expected NOT to have additional configs", $iter->hasNextConfig() == 0);
	$test->check("Expected NOT to have additional messages", $iter->hasNextMessage() == 0);

	$iter->reset();

	$test->check("Expected to have subscriptions", $iter->hasNextSubscription());
	$test->check("Expected to have configs", $iter->hasNextConfig());
	$test->check("Expected to have messages", $iter->hasNextMessage());
}


Test::driver(\&Test_ConfigFileIterator);
