#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


sub Test_ResourceGenerator
{
	my ($test) = @_;

	$test->setDescription("Test ResourceGenerator");

	test_create($test);
	test_start($test);
	test_stop($test);
	test_setField($test);
	test_createResourceMessage($test);
}


sub test_create
{
	libgmsec_perl::logInfo("Test create()");

	my ($test) = @_;

	my $emptyFieldArray = libgmsec_perl::FieldArray->new();
	my $standardFields  = $test->getStandardFields();

	# Nominal test (empty, populated, and null list of fields)
	my $rsrcgen1 = libgmsec_perl::ResourceGenerator::create($test->getConfig(), 5, 1, 10, $emptyFieldArray);
	my $rsrcgen2 = libgmsec_perl::ResourceGenerator::create($test->getConfig(), 5, 1, 10, $standardFields);
	my $rsrcgen3 = libgmsec_perl::ResourceGenerator::create($test->getConfig(), 5, 1, 10);

	$test->check("ResourceGenerator should not be running", $rsrcgen1->isRunning() == 0);
	$test->check("ResourceGenerator should not be running", $rsrcgen2->isRunning() == 0);
	$test->check("ResourceGenerator should not be running", $rsrcgen3->isRunning() == 0);

	libgmsec_perl::ResourceGenerator::destroy($rsrcgen1);
	libgmsec_perl::ResourceGenerator::destroy($rsrcgen2);
	libgmsec_perl::ResourceGenerator::destroy($rsrcgen3);

    # Off-nominal test(s)
    eval
    {
        my $config = libgmsec_perl::Config->new($test->getConfig());
        $config->addValue("mw-id", "bogus-mw");
    	my $rsrcgen = libgmsec_perl::ResourceGenerator::create($config, 5, , 1, 10, $emptyFieldArray);
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

	my $config  = libgmsec_perl::Config->new($test->getConfig());
	my $pubRate = 1;

	my $rsrcgen = libgmsec_perl::ResourceGenerator::create($config, $pubRate, 1, 10, $test->getStandardFields());

	# To ensure this resource message is unique, we set the COMPONENT field
	$rsrcgen->setField( libgmsec_perl::StringField->new("COMPONENT", $test->getUniqueComponent()) );

	$rsrcgen->start();

	$test->check("ResourceGenerator should be running", $rsrcgen->isRunning());

	verifyResourceMessage($test, $config, $pubRate);

	$rsrcgen->stop();

	libgmsec_perl::ResourceGenerator::destroy($rsrcgen);

	# Off-nominal tests
	libgmsec_perl::logInfo("Off-nominal cases...");
	$config->addValue("gmsec-msg-content-validate", "true");

	my $rsrcgen2 = libgmsec_perl::ResourceGenerator::create($config, 1, 1, 10, $test->getStandardFields());

	# Add bogus field using a Field
	eval
	{
		$rsrcgen2->setField( libgmsec_perl::U16Field->new("BOGUS-FIELD", 2) );
        $rsrcgen2->start();
		$test->check("An expection was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message Validation Failed") != -1);
	}
}


sub test_stop
{
	libgmsec_perl::logInfo("Test stop()");

	my ($test) = @_;

	my $config  = $test->getConfig();
	my $pubRate = 1;

	my $rsrcgen = libgmsec_perl::ResourceGenerator::create($config, $pubRate, 1, 10, $test->getStandardFields());

	# To ensure this resource message is unique, we set the COMPONENT field
	$rsrcgen->setField( libgmsec_perl::StringField->new("COMPONENT", $test->getUniqueComponent()) );

	$rsrcgen->start();

	$test->check("ResourceGenerator should be running", $rsrcgen->isRunning());

	$rsrcgen->stop();

	$test->check("ResourceGenerator should not be running", $rsrcgen->isRunning() == 0);

	libgmsec_perl::TimeUtil::millisleep(2000);

	my $conn = libgmsec_perl::Connection::create($config);
	$conn->connect();
	$conn->subscribe("C2MS.*.*.*.*.*.MSG.RSRC." . $test->getUniqueComponent());

	for (my $i = 0; $i < 3; $i++)
	{
		my $rsrcMsg = $conn->receive(5000);

		if (defined $rsrcMsg)
		{
			$test->check("Unexpectedly received a Resource Message", 0);
			libgmsec_perl::Message::destroy($rsrcMsg);
		}
	}

	$conn->disconnect();

	libgmsec_perl::Connection::destroy($conn);
	libgmsec_perl::ResourceGenerator::destroy($rsrcgen);
}


sub test_setField
{
	my ($test) = @_;

	my $config = libgmsec_perl::Config->new( $test->getConfig() );    # make a copy

	my $pubRate = 1;
	my $samInt  = 1;
	my $avgInt  = 10;

	# Nominal tests
	my $rsrcgen = libgmsec_perl::ResourceGenerator::create($config, $pubRate, $samInt, $avgInt, $test->getStandardFields());

	# To ensure this resource message is unique, we set the COMPONENT field
	$rsrcgen->setField( libgmsec_perl::StringField->new("COMPONENT", $test->getUniqueComponent()) );

	$rsrcgen->start();

	libgmsec_perl::TimeUtil::millisleep(2000);

	libgmsec_perl::logInfo("Test setField() with a Field");
	my $newPubRate = 2;
	$rsrcgen->setField( libgmsec_perl::U16Field->new("PUB-RATE", $newPubRate) );
	verifyResourceMessage($test, $config, $newPubRate);

	$rsrcgen->stop();

	libgmsec_perl::logInfo("Cleanup...");
	libgmsec_perl::ResourceGenerator::destroy($rsrcgen);
}


sub test_createResourceMessage
{
	libgmsec_perl::logInfo("Test createResourceMessage()");

	my ($test) = @_;

    my $factory = libgmsec_perl::MessageFactory::create( $test->getConfig() );
    my $samInt  = 1;
    my $avgInt  = 10;

    $test->setStandardFields( $factory );

    my $rsrcMsg = libgmsec_perl::ResourceGenerator::createResourceMessage( $factory, $samInt, $avgInt );

	$test->check( "Unexpected MESSAGE-TYPE", $rsrcMsg->getStringValue("MESSAGE-TYPE") eq "MSG" );
	$test->check( "Unexpected MESSAGE-SUBTYPE", $rsrcMsg->getStringValue("MESSAGE-SUBTYPE") eq "RSRC" );

    libgmsec_perl::MessageFactory::destroy( $factory );
}


sub verifyResourceMessage
{
	my ($test, $config, $expectedPubRate) = @_;

	my $conn = libgmsec_perl::Connection::create($config);
	$conn->connect();
	$conn->subscribe("C2MS.*.*.*.*.*.MSG.RSRC." . $test->getUniqueComponent() . ".+");

	my $t1 = 0;
	my $t2 = 0;

	for (my $i = 0; $i < 7; $i++)
	{
		# Start time t1 is at the time start to receive data
		$t1 = libgmsec_perl::TimeUtil::getCurrentTime_s();
		my $rsrcMsg = $conn->receive(5000);

		# The end time t2 has to be measured immediately right after 
        # it completes receiving data.
		$t2 = libgmsec_perl::TimeUtil::getCurrentTime_s();

		# ignore the first few incoming messages (if any)
		if ($i < 3)
		{
			if (defined $rsrcMsg)
			{
			    libgmsec_perl::Message::destroy($rsrcMsg);
			}
			next;
		}

		if ($rsrcMsg == undef)
		{
			$test->check("Failed to received Resource Message", 0);
		}
		else
		{
			# if ($t1 == 0)
			# {
			#	$t1 = libgmsec_perl::TimeUtil::getCurrentTime_s();
			#}
			#else
			#{
			#	$t2 = libgmsec_perl::TimeUtil::getCurrentTime_s();

            my $delta = $t2 - $t1;
			my $roundUpPubRateToSecond = int($delta);
            if ($delta < $expectedPubRate)
			{
				#$delta = (($t2 - $t1) * 10.0 + 0.5) / 10.0;
				$roundUpPubRateToSecond = int($delta + 0.5);
			}

			libgmsec_perl::logInfo("Expected rate is: $expectedPubRate, delta is: $delta");

			#	$test->check("Unexpected publish rate", int($expectedPubRate) <= int($delta));
			$test->check("Unexpected publish rate", $expectedPubRate <= $roundUpPubRateToSecond);
				
			#	$t1 = $t2;
			#}

            $test->check("Unexpected MESSAGE-TYPE", $rsrcMsg->getStringValue("MESSAGE-TYPE") eq "MSG");
            $test->check("Unexpected MESSAGE-SUBTYPE", $rsrcMsg->getStringValue("MESSAGE-SUBTYPE") eq "RSRC");
            $test->check("Unexpected PUB-RATE", $rsrcMsg->getIntegerValue("PUB-RATE") == $expectedPubRate);

			libgmsec_perl::Message::destroy($rsrcMsg);
		}
	}

	$conn->disconnect();

	libgmsec_perl::Connection::destroy($conn);
}


Test::driver(\&Test_ResourceGenerator);
