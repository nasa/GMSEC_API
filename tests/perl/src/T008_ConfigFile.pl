#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


my $GOOD_CONFIG_FILE  = "good_config_file.xml";
my $BAD_CONFIG_FILE_1 = "bad_config_file1.xml";
my $BAD_CONFIG_FILE_2 = "bad_config_file2.xml";

my $XML  = "<DEFINITIONS>\n";
$XML .= "    <SUBSCRIPTION NAME=\"sub1\" PATTERN=\"GMSEC.&amp;gt\">\n";
$XML .= "        <EXCLUDE PATTERN=\"GMSEC.*.*.MSG.HB.+\"/>\n";
$XML .= "        <EXCLUDE PATTERN=\"GMSEC.*.*.MSG.LOG.+\"/>\n";
$XML .= "    </SUBSCRIPTION>\n";
$XML .= "    <CONFIG NAME=\"Bolt\">\n";
$XML .= "        <PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n";
$XML .= "        <PARAMETER NAME=\"server\">localhost</PARAMETER>\n";
$XML .= "    </CONFIG>\n";
$XML .= "    <MESSAGE NAME=\"Msg1\">\n";
$XML .= "        <CONFIG NAME=\"msg_config\">\n";
$XML .= "            <PARAMETER NAME=\"GMSEC-MSGFLD-STORE-SIZE\">15</PARAMETER>\n";
$XML .= "            <PARAMETER NAME=\"GMSEC-MSGFLD-STORE-TYPE\">tree</PARAMETER>\n";
$XML .= "        </CONFIG>\n";
$XML .= "        <FIELD NAME=\"MESSAGE-TYPE\" TYPE=\"STRING\">MSG</FIELD>\n";
$XML .= "        <FIELD NAME=\"MESSAGE-SUBTYPE\" TYPE=\"STRING\">HB</FIELD>\n";
$XML .= "        <FIELD NAME=\"MISSION-ID\" TYPE=\"STRING\">MY-MISSION</FIELD>\n";
$XML .= "        <FIELD NAME=\"CONSTELLATION-ID\" TYPE=\"STRING\">MY-CONSTELLATION</FIELD>\n";
$XML .= "    </MESSAGE>\n";
$XML .= "</DEFINITIONS>";


sub Test_ConfigFile
{
	my ($test) = @_;

	$test->setDescription("Test ConfigFile");

	test_constructor_1($test);
	test_constructor_2($test);
	test_load($test);
	test_save($test);
	test_fromXML($test);
	test_toXML($test);
	test_isLoaded($test);
	test_addConfig($test);
	test_lookupConfig($test);
	test_removeConfig($test);
	test_addMessage($test);
	test_lookupMessage($test);
	test_removeMessage($test);
	test_addSubscriptionEntry($test);
	test_lookupSubscriptionEntry($test);
	test_removeSubscription($test);
	test_getIterator($test);
}


sub test_constructor_1
{
	libgmsec_perl::logInfo("Test constructor (no args)");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();
	$test->check("Expected isLoaded() to report 0", $cfgFile->isLoaded() == 0);
}


sub test_constructor_2
{
	libgmsec_perl::logInfo("Test constructor (Config)");

	my ($test) = @_;

	eval
	{
        my $config  = libgmsec_perl::Config->new();
	    my $cfgFile = libgmsec_perl::ConfigFile->new( $config );
    	$test->check("Expected isLoaded() to report 0", $cfgFile->isLoaded() == 0);

	    $cfgFile->load( $test->getDataFile($GOOD_CONFIG_FILE) );
	    $test->check("Expected isLoaded to report 0", $cfgFile->isLoaded());

        my $msg = $cfgFile->lookupMsg("Msg1");

        $test->check("Expected C2MS specification", $msg->getSchemaLevel() == $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}

	eval
	{
        my $config  = libgmsec_perl::Config->new("gmsec-schema-level=2", $libgmsec_perl::DataType_KEY_VALUE_DATA);
	    my $cfgFile = libgmsec_perl::ConfigFile->new( $config );
    	$test->check("Expected isLoaded() to report 0", $cfgFile->isLoaded() == 0);

	    $cfgFile->load( $test->getDataFile($GOOD_CONFIG_FILE) );
	    $test->check("Expected isLoaded to report 0", $cfgFile->isLoaded());

        my $msg = $cfgFile->lookupMsg("Msg1");

        $test->check("Expected GMSEC specification", $msg->getSchemaLevel() == $libgmsec_perl::Specification::SchemaLevel_LEVEL_2);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
}


sub test_load
{
	libgmsec_perl::logInfo("Test load()");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();

	# Nominal test
	$cfgFile->load( $test->getDataFile($GOOD_CONFIG_FILE) );
	$test->check("Expected isLoaded to report 0", $cfgFile->isLoaded());

	# Off-nominal test
	eval
	{
		$cfgFile->load(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Invalid or missing config file path") != -1);
		$test->check("Expected isLoaded() to report 0", $cfgFile->isLoaded() == 0);
	}

	eval
	{
		$cfgFile->load("nonexistent.xml");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Error: File not found") != -1);
		$test->check("Expected isLoaded() to report 0", $cfgFile->isLoaded() == 0);
	}

	eval
	{
		$cfgFile->load( $test->getDataFile($BAD_CONFIG_FILE_1) );
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Invalid XML document - missing DEFINITIONS") != -1);
		$test->check("Expected isLoaded() to report 0", $cfgFile->isLoaded() == 0);
	}

	eval
	{
		$cfgFile->load( $test->getDataFile($BAD_CONFIG_FILE_2) );
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Error: Mismatched element") != -1);
		$test->check("Expected isLoaded() to report 0", $cfgFile->isLoaded() == 0);
	}
}


sub test_save
{
	libgmsec_perl::logInfo("Test save()");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();
	$cfgFile->load( $test->getDataFile($GOOD_CONFIG_FILE) );

	$cfgFile->save("new_config_file.xml");

	my $cfgFile2 = libgmsec_perl::ConfigFile->new();
	$cfgFile2->load("new_config_file.xml");

	$test->check("Unexpected XML content", $cfgFile->toXML() eq $cfgFile2->toXML());

	unlink("new_config_file.xml");
}


sub test_fromXML
{
	libgmsec_perl::logInfo("Test fromXML()");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();

	# Nominal case
	$cfgFile->fromXML( $XML );
	$test->check("Expected isLoaded() to report true", $cfgFile->isLoaded());

	# Off-nominal cases

	# undef content
	eval
	{
		$cfgFile->fromXML(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Invalid XML string") != -1);
		$test->check("Expected isLoaded() to report 0", $cfgFile->isLoaded() == 0);
	}

	# empty-string content
	eval
	{
		$cfgFile->fromXML("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Invalid XML string") != -1);
		$test->check("Expected isLoaded() to report 0", $cfgFile->isLoaded() == 0);
	}

	# invalid content
	eval
	{
		$cfgFile->fromXML("<FOOBAR/>");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Invalid XML document - missing DEFINITIONS") != -1);
		$test->check("Expected isLoaded() to report 0", $cfgFile->isLoaded() == 0);
	}

	# incomplete content
	eval
	{
		$cfgFile->fromXML("<DEFINITIONS><SUBSCRIPTION NAME=\"bad xml\" PATTERN=\"GMSEC.>\">");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Unable to parse XML string") != -1);
		$test->check("Expected isLoaded() to report 0", $cfgFile->isLoaded() == 0);
	}
}


sub test_toXML
{
	libgmsec_perl::logInfo("Test toXML()");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();

	$cfgFile->fromXML( $XML );

	$test->check("Unexpected XML content from config file", $cfgFile->toXML() eq $XML);
}


sub test_isLoaded
{
	libgmsec_perl::logInfo("Test isLoaded()");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();
	$test->check("Expected isLoaded() to report 0", $cfgFile->isLoaded() == 0);

	$cfgFile->load( $test->getDataFile($GOOD_CONFIG_FILE) );
	$test->check("Expected isLoaded() to report true", $cfgFile->isLoaded());

	eval
	{
		$cfgFile->load( $test->getDataFile($BAD_CONFIG_FILE_1) );
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		$test->check("Expected isLoaded() to report 0", $cfgFile->isLoaded() == 0);
	}
}


sub test_addConfig
{
	libgmsec_perl::logInfo("Test addConfig()");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();

	# Nominal test
	my $config = libgmsec_perl::Config->new("name1=value1", $libgmsec_perl::DataType_KEY_VALUE_DATA);

	$cfgFile->addConfig("Cfg1", $config);

	my $tmp = $cfgFile->lookupConfig("Cfg1");

	$test->check("Unexpected config value", $config->getValue("name1") eq $tmp->getValue("name1"));

	# Off-nominal tests

	# null config name
	eval
	{
		$cfgFile->addConfig(undef, $config);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Configuration name cannot be NULL, nor an empty string") != -1);
	}

	# empty-string config name
	eval
	{
		$cfgFile->addConfig("", $config);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Configuration name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_lookupConfig
{
	libgmsec_perl::logInfo("Test lookupConfig()");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();

	# Nominal case
	$cfgFile->load( $test->getDataFile($GOOD_CONFIG_FILE) );

	my $cfg = $cfgFile->lookupConfig("Bolt");
	$test->check("Unexpected mw-id value", $cfg->getValue("mw-id") eq "bolt");
	$test->check("Unexpected server value", $cfg->getValue("server") eq "localhost");

	$cfg = $cfgFile->lookupConfig("ActiveMQ");
	$test->check("Unexpected mw-id value", $cfg->getValue("mw-id") eq "activemq39");
	$test->check("Unexpected server value", $cfg->getValue("server") eq "tcp://localhost:61616");

	# Off-nominal cases

	# bogus config name
	eval
	{
		$cfgFile->lookupConfig("bogus");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Configuration does not exist for the name bogus") != -1);
	}

	# undef config name
	eval
	{
		$cfgFile->lookupConfig(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Configuration name is NULL, or is an empty string") != -1);
	}

	# empty-string config name
	eval
	{
		$cfgFile->lookupConfig("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Configuration name is NULL, or is an empty string") != -1);
	}
}


sub test_removeConfig
{
	libgmsec_perl::logInfo("Test removeConfig()");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();

	$cfgFile->addConfig("Bolt", libgmsec_perl::Config->new("mw-id=bolt server=localhost", $libgmsec_perl::DataType_KEY_VALUE_DATA));

	$test->check("Expected to remove existing config", $cfgFile->removeConfig("Bolt"));
	$test->check("Did not expect to remove config", $cfgFile->removeConfig("Bolt") == 0);
	$test->check("Did not expect to remove config", $cfgFile->removeConfig("bogus") == 0);
	$test->check("Did not expect to remove config", $cfgFile->removeConfig(undef) == 0);
	$test->check("Did not expect to remove config", $cfgFile->removeConfig("") == 0);
}


sub test_addMessage
{
	libgmsec_perl::logInfo("Test addMessage()");

	my ($test) = @_;

	my $cfgFile    = libgmsec_perl::ConfigFile->new();
	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	my $msg = $msgFactory->createMessage("HB");
	$msg->addField( libgmsec_perl::StringField->new("FIELD1", "ONE") );
	$msg->addField( libgmsec_perl::StringField->new("FIELD2", "TWO") );

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	# Nominal test
	$cfgFile->addMessage("my-msg", $msg);

	my $tmp = $cfgFile->lookupMessage("my-msg");

	$test->check("Unexpected message subject", $tmp->getSubject() eq $msg->getSubject());
	$test->check("Unexpected message kind", $tmp->getKind() == $msg->getKind());
	$test->check("Unexpected message field count", $tmp->getFieldCount() == $msg->getFieldCount());
	$test->check("Unexpected field value", $tmp->getStringValue("FIELD1") eq "ONE");
	$test->check("Unexpected field value", $tmp->getStringValue("FIELD2") eq "TWO");
	$test->check("Unexpected XML content", $tmp->toXML() eq $msg->toXML());
	$test->check("Unexpected JSON content", $tmp->toJSON() eq $msg->toJSON());

	# Off-nominal tests

	# undef message name
	eval
	{
		$cfgFile->addMessage(undef, $msg);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message name cannot be NULL, nor an empty string") != -1);
	}

	# empty-string message name
	eval
	{
		$cfgFile->addMessage("", $msg);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_lookupMessage
{
	libgmsec_perl::logInfo("Test lookupMessage()");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();

	# Nominal case
	$cfgFile->load( $test->getDataFile($GOOD_CONFIG_FILE) );

	my $msg = $cfgFile->lookupMessage("Msg1");
	$test->check("Unexpected MESSAGE-TYPE", $msg->getStringValue("MESSAGE-TYPE") eq "MSG");
	$test->check("Unexpected MESSAGE-SUBTYPE", $msg->getStringValue("MESSAGE-SUBTYPE") eq "HB");

	# Off-nominal cases

	# bogus message name
	eval
	{
		$cfgFile->lookupMessage("bogus");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not exist for the name bogus") != -1);
	}

	# undef message name
	eval
	{
		$cfgFile->lookupMessage(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message name cannot be NULL, nor an empty string") != -1);
	}

	# empty-string message name
	eval
	{
		$cfgFile->lookupMessage("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_removeMessage
{
	libgmsec_perl::logInfo("Test removeMessage()");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	$cfgFile->addMessage("Heartbeat", $msgFactory->createMessage("HB"));

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$test->check("Expected to remove existing message", $cfgFile->removeMessage("Heartbeat"));
	$test->check("Did not expect to remove message", $cfgFile->removeMessage("Heartbeat") == 0);
	$test->check("Did not expect to remove message", $cfgFile->removeMessage("bogus") == 0);
	$test->check("Did not expect to remove message", $cfgFile->removeMessage(undef) == 0);
	$test->check("Did not expect to remove message", $cfgFile->removeMessage("") == 0);
}


sub test_addSubscriptionEntry
{
	libgmsec_perl::logInfo("Test addSubscriptionEntry()");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();

	# Nominal case
	my $entry = libgmsec_perl::SubscriptionEntry->new("Sub1", "GMSEC.>");
	$entry->addExcludedPattern("GMSEC.*.*.MSG.HB.>");

	$cfgFile->addSubscriptionEntry($entry);

	$entry = $cfgFile->lookupSubscriptionEntry("Sub1");

	$test->check("Unexpected subscription entry name", $entry->getName() eq "Sub1");
	$test->check("Unexpected subscription entry pattern", $entry->getPattern() eq "GMSEC.>");
	$test->check("Expected an excluded pattern", $entry->hasNextExcludedPattern() == 1);
	$test->check("Unexpected excluded pattern", $entry->nextExcludedPattern() eq "GMSEC.*.*.MSG.HB.>");
	$test->check("Expected no additional excluded patterns", $entry->hasNextExcludedPattern() == 0);
}


sub test_lookupSubscriptionEntry
{
	libgmsec_perl::logInfo("Test lookupSubscriptionEntry()");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();

	# Nominal case
	$cfgFile->load( $test->getDataFile($GOOD_CONFIG_FILE) );

	my $entry = $cfgFile->lookupSubscriptionEntry("Sub1");

	$test->check("Unexpected subscription entry name", $entry->getName() eq "Sub1");
	$test->check("Unexpected subscription entry pattern", $entry->getPattern() eq "GMSEC.>");

	# Off-nominal cases

	# bogus subscription name
	eval
	{
		$cfgFile->lookupSubscriptionEntry("bogus");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "SubscriptionEntry does not exist for the name bogus") != -1);
	}

	# undef subscription name
	eval
	{
		$cfgFile->lookupSubscriptionEntry(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "SubscriptionEntry name cannot be NULL, nor an empty string") != -1);
	}

	# empty-string subscription name
	eval
	{
		$cfgFile->lookupSubscriptionEntry("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "SubscriptionEntry name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_removeSubscription
{
	libgmsec_perl::logInfo("Test removeSubscription()");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();

	$cfgFile->addSubscriptionEntry( libgmsec_perl::SubscriptionEntry->new("AllMessages", "GMSEC.>") );

	$test->check("Expected to remove existing subscription", $cfgFile->removeSubscriptionEntry("AllMessages"));
	$test->check("Did not expect to remove subscription", $cfgFile->removeSubscriptionEntry("AllMessages") == 0);
	$test->check("Did not expect to remove subscription", $cfgFile->removeSubscriptionEntry("bogus") == 0);
	$test->check("Did not expect to remove subscription", $cfgFile->removeSubscriptionEntry(undef) == 0);
	$test->check("Did not expect to remove subscription", $cfgFile->removeSubscriptionEntry("") == 0);
}


sub test_getIterator
{
	libgmsec_perl::logInfo("Test getIterator()");

	my ($test) = @_;

	my $cfgFile = libgmsec_perl::ConfigFile->new();

	$cfgFile->addSubscriptionEntry( libgmsec_perl::SubscriptionEntry->new("AllMessages", "GMSEC.>") );
	$cfgFile->addSubscriptionEntry( libgmsec_perl::SubscriptionEntry->new("LogMessages", "GMSEC.*.*.MSG.LOG.+") );

	$cfgFile->addConfig("Bolt", libgmsec_perl::Config->new("mw-id=bolt server=localhost", $libgmsec_perl::DataType_KEY_VALUE_DATA));
	$cfgFile->addConfig("ActiveMQ", libgmsec_perl::Config->new("mw-id=activemq39 server=tcp://localhost:61616", $libgmsec_perl::DataType_KEY_VALUE_DATA));

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	$cfgFile->addMessage("Heartbeat", $msgFactory->createMessage("HB"));
	$cfgFile->addMessage("Log", $msgFactory->createMessage("LOG"));

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	my $iter = $cfgFile->getIterator();

	my $numSubscriptions = 0;
	my $numConfigs = 0;
	my $numMessages = 0;

	while ($iter->hasNextSubscription())
	{
		$iter->nextSubscription();
		$numSubscriptions += 1;
	}

	while ($iter->hasNextConfig())
	{
		$iter->nextConfig();
		$numConfigs += 1;
	}

	while ($iter->hasNextMessage())
	{
		$iter->nextMessage();
		$numMessages += 1;
	}

	$test->check("Unexpected number of subscriptions", $numSubscriptions == 2);
	$test->check("Unexpected number of configs", $numConfigs == 2);
	$test->check("Unexpected number of messages", $numMessages == 2);
}


Test::driver(\&Test_ConfigFile);
