#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


sub Test_HeartbeatGenerator
{
	my ($test) = @_;

	$test->setDescription("Test HeartbeatGenerator");

	test_create($test);
	test_start($test);
	test_stop($test);
	test_changePublishRate($test);
	test_setField($test);
}


sub test_create
{
	libgmsec_perl::logInfo("Test create()");

	my ($test) = @_;

	my $emptyFieldArray = libgmsec_perl::FieldArray->new();
	my $standardFields  = $test->getStandardFields();

	# Nominal test
	my $hbgen1 = libgmsec_perl::HeartbeatGenerator::create($test->getConfig(), 5, $emptyFieldArray);
	my $hbgen2 = libgmsec_perl::HeartbeatGenerator::create($test->getConfig(), 5, $standardFields);
	my $hbgen3 = libgmsec_perl::HeartbeatGenerator::create($test->getConfig(), 5);

	$test->check("HeartbeatGenerator should not be running", $hbgen1->isRunning() == 0);
	$test->check("HeartbeatGenerator should not be running", $hbgen2->isRunning() == 0);
	$test->check("HeartbeatGenerator should not be running", $hbgen3->isRunning() == 0);

	libgmsec_perl::HeartbeatGenerator::destroy($hbgen1);
	libgmsec_perl::HeartbeatGenerator::destroy($hbgen2);
	libgmsec_perl::HeartbeatGenerator::destroy($hbgen3);

    # Off-nominal test(s)
    eval
    {
        my $config = libgmsec_perl::Config->new($test->getConfig());
        $config->addValue("mw-id", "bogus-mw");
    	my $hbgen = libgmsec_perl::HeartbeatGenerator::create($config, 5, $emptyFieldArray);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Unable to load") != -1);
	}
}


sub test_start
{
	libgmsec_perl::logInfo("Test start()");

	my ($test) = @_;

	my $config = libgmsec_perl::Config->new($test->getConfig());

	my $hbgen = libgmsec_perl::HeartbeatGenerator::create($config, 1, $test->getStandardFields());

	# To ensure this heartbeat message is unique, we set the COMPONENT field
	$hbgen->setField( libgmsec_perl::StringField->new("COMPONENT", $test->getUniqueComponent()) );

	$hbgen->start();

	$test->check("HeartbeatGenerator should be running", $hbgen->isRunning());

	verifyHeartbeatMessage($test, $config, 1);

	$hbgen->stop();

	libgmsec_perl::HeartbeatGenerator::destroy($hbgen);

	# Off-nominal tests
	libgmsec_perl::logInfo("Off-nominal cases...");
	$config->addValue("gmsec-msg-content-validate", "true");

	my $hbgen2 = libgmsec_perl::HeartbeatGenerator::create($config, 1, $test->getStandardFields());

	# Add bogus field using a Field
	eval
	{
		$hbgen2->setField( libgmsec_perl::U16Field->new("BOGUS-FIELD", 2) );
        $hbgen2->start();
		$test->check("An expection was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message Validation Failed") != -1);
	}

	# Add bogus field using a long value
	eval
	{
		$hbgen2->setField("BOGUS-FIELD", 2);
        $hbgen2->start();
		$test->check("An expection was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message Validation Failed") != -1);
	}

	# Add bogus field using a double value
	eval
	{
		$hbgen2->setField("BOGUS-FIELD", 2.0);
        $hbgen2->start();
		$test->check("An expection was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message Validation Failed") != -1);
	}

	# Add bogus field using a string value
	eval
	{
		$hbgen2->setField("BOGUS-FIELD", "2");
        $hbgen2->start();
		$test->check("An expection was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message Validation Failed") != -1);
	}

	libgmsec_perl::HeartbeatGenerator::destroy($hbgen2);
}


sub test_stop
{
	libgmsec_perl::logInfo("Test stop()");

	my ($test) = @_;

	my $config = $test->getConfig();

	my $hbgen = libgmsec_perl::HeartbeatGenerator::create($config, 1, $test->getStandardFields());

	# To ensure this heartbeat message is unique, we set the COMPONENT field
	$hbgen->setField( libgmsec_perl::StringField->new("COMPONENT", $test->getUniqueComponent()) );

	$hbgen->start();

	$test->check("HeartbeatGenerator should be running", $hbgen->isRunning());

	$hbgen->stop();

	$test->check("HeartbeatGenerator should not be running", $hbgen->isRunning() == 0);

	libgmsec_perl::TimeUtil::millisleep(2000);

	my $conn = libgmsec_perl::Connection::create($config);
	$conn->connect();
	$conn->subscribe("C2MS.*.*.*.*.*.MSG.HB." . $test->getUniqueComponent());

	for (my $i = 0; $i < 3; $i++)
	{
		my $hbmsg = $conn->receive(5000);

		if ($hbmsg != undef)
		{
			$test->check("Unexpectedly received a Heartbeat Message", 0);
			libgmsec_perl::Message::destroy($hbmsg);
		}
	}

	$conn->disconnect();

	libgmsec_perl::Connection::destroy($conn);

	libgmsec_perl::HeartbeatGenerator::destroy($hbgen);
}


sub test_changePublishRate
{
	libgmsec_perl::logInfo("Test changePublishRate()");

	my ($test) = @_;

	my $config = $test->getConfig();

	# Initialize set rate to 1 (second)
	my $hbgen = libgmsec_perl::HeartbeatGenerator::create($config, 1, $test->getStandardFields());

	# To ensure this heartbeat message is unique, we set the COMPONENT field
	$hbgen->setField( libgmsec_perl::StringField->new("COMPONENT", $test->getUniqueComponent()) );

	$hbgen->start();

	# Change rate to 2 (seconds)
	$hbgen->changePublishRate(2);
	verifyHeartbeatMessage($test, $config, 2);

	my $conn = libgmsec_perl::Connection::create($config);
	$conn->connect();
	$conn->subscribe("C2MS.*.*.*.*.*.MSG.HB." . $test->getUniqueComponent());

	libgmsec_perl::TimeUtil::millisleep(1000);

	# Change rate to 0 (seconds); expect only one additional heartbeat message to be published
	$hbgen->changePublishRate(0);

	libgmsec_perl::TimeUtil::millisleep(1000);

	my $lastMessage = 0;

	for (my $i = 0; $i < 3 and $lastMessage == 0; $i++)
	{
		my $hbmsg = $conn->receive(5000);

		if ($hbmsg == undef) 
		{
			$test->check("Failed to receive Heartbeat Message", 0);
		}
		elsif ($hbmsg->getIntegerValue("PUB-RATE") != 0)
		{
			libgmsec_perl::Message::destroy($hbmsg);
		}
		else
		{
			$lastMessage = 1;
			libgmsec_perl::Message::destroy($hbmsg);
		}
	}

	$test->check("Failed to receive message with PUB-RATE of 0", $lastMessage == 1);

	$conn->disconnect();

	libgmsec_perl::Connection::destroy($conn);

	$hbgen->stop();

	libgmsec_perl::HeartbeatGenerator::destroy($hbgen);
}


sub test_setField
{
	my ($test) = @_;

	my $config = libgmsec_perl::Config->new( $test->getConfig() );     # make a copy

	my $pubRate = 1;

	# Nominal tests
	my $hbgen = libgmsec_perl::HeartbeatGenerator::create($config, $pubRate, $test->getStandardFields());

	# To ensure this heartbeat message is unique, we set the COMPONENT field
	$hbgen->setField( libgmsec_perl::StringField->new("COMPONENT", $test->getUniqueComponent()) );

	$hbgen->start();

	libgmsec_perl::TimeUtil::millisleep(2000);

	libgmsec_perl::logInfo("Test setField() with a Field");
	$hbgen->setField( libgmsec_perl::U16Field->new("PUB-RATE", 2) );
	verifyHeartbeatMessage($test, $config, 2);

	libgmsec_perl::logInfo("Test setField() with a Field Name and long value");
	$hbgen->setField("PUB-RATE", 3);
	verifyHeartbeatMessage($test, $config, 3);

	libgmsec_perl::logInfo("Test setField() with a Field Name and double value");
	$hbgen->setField("PUB-RATE", 2.0);
	verifyHeartbeatMessage($test, $config, 2);

	libgmsec_perl::logInfo("Test setField() with a Field Name and string value");
	$hbgen->setField("PUB-RATE", "3");
	verifyHeartbeatMessage($test, $config, 3);

	$hbgen->stop();

	libgmsec_perl::logInfo("Cleanup...");
	libgmsec_perl::HeartbeatGenerator::destroy($hbgen);
}


sub verifyHeartbeatMessage
{
	my ($test, $config, $expectedPubRate) = @_;

	my $conn = libgmsec_perl::Connection::create($config);
	$conn->connect();
	$conn->subscribe("C2MS.*.*.*.*.*.MSG.HB." . $test->getUniqueComponent() . ".+");

	my $t1 = 0;
	my $t2 = 0;

	for (my $i = 0; $i < 7; $i++)
	{
		my $hbmsg = $conn->receive(5000);

		# ignore the first few incoming messages (if any)
		if ($i < 3)
		{
			if (defined $hbmsg)
			{
				libgmsec_perl::Message::destroy($hbmsg);
			}
			next;
		}

		if ($hbmsg == undef)
		{
			$test->check("Failed to received Heartbeat Message", 0);
		}
		else
		{
			if ($t1 == 0)
			{
				$t1 = libgmsec_perl::TimeUtil::getCurrentTime_s();
			}
			else
			{
				$t2 = libgmsec_perl::TimeUtil::getCurrentTime_s();

                my $delta = $t2 - $t1;
                if ($delta < $expectedPubRate)
                {
                    $delta = (($t2 - $t1) * 10.0 + 0.5) / 10.0;
                }

				libgmsec_perl::logInfo("Expected rate is: $expectedPubRate, delta is: $delta");

				$test->check("Unexpected publish rate", int($expectedPubRate) == int($delta));

				$t1 = $t2;

				$test->check("Unexpected MESSAGE-TYPE", $hbmsg->getStringValue("MESSAGE-TYPE") eq "MSG");
				$test->check("Unexpected MESSAGE-SUBTYPE", $hbmsg->getStringValue("MESSAGE-SUBTYPE") eq "HB");
				$test->check("Unexpected PUB-RATE", $hbmsg->getIntegerValue("PUB-RATE") == $expectedPubRate);

				libgmsec_perl::Message::destroy($hbmsg);
			}
		}
	}

	$conn->disconnect();

	libgmsec_perl::Connection::destroy($conn);
}


Test::driver(\&Test_HeartbeatGenerator);
