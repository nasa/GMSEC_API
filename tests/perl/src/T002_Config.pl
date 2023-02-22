#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


sub Test_Config
{
	my ($test) = @_;

	$test->setDescription("Test Config");

	test_create($test);
	test_constructor_1($test);
	test_constructor_2($test);
	test_copy_constructor($test);
	test_addValue($test);
	test_clearValue($test);
	test_getValue($test);
	test_getBooleanValue($test);
	test_getIntegerValue($test);
	test_getDoubleValue($test);
	test_clear($test);
	test_merge($test);
	test_toXML($test);
	test_fromXML($test);
    test_getFromFile($test);
	test_toJSON($test);
}


sub test_create
{
	libgmsec_perl::logInfo("Test create (command line args)");

	my ($test) = @_;

	my @args = qw(mw-id=bolt server=localhost);

	my $c1 = libgmsec_perl::Config::create( \@args );

	$test->check("Unexpected XML content", $c1->toXML() eq "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>");
}


sub test_constructor_1
{
	libgmsec_perl::logInfo("Test constructor (no-arg)");

	my ($test) = @_;

	my $c1 = libgmsec_perl::Config->new();

	$test->check("Unexpected XML content", $c1->toXML() eq "<CONFIG>\n</CONFIG>");
}


sub test_constructor_2
{
	libgmsec_perl::logInfo("Test constructor (data args)");

	my ($test) = @_;

	# Test: data constructor

	# Using string with options
	my $c1 = libgmsec_perl::Config->new("mw-id=bolt server=localhost", $libgmsec_perl::DataType_KEY_VALUE_DATA);
	$test->check("Unexpected XML content", $c1->toXML() eq "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>");

	# Using XML
	my $c2 = libgmsec_perl::Config->new($c1->toXML(), $libgmsec_perl::DataType_XML_DATA);
	$test->check("Unexpected XML content", $c2->toXML() eq "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>");

	# Using JSON
	my $c3 = libgmsec_perl::Config->new($c1->toJSON(), $libgmsec_perl::DataType_JSON_DATA);
	$test->check("Unexpected XML content", $c3->toXML() eq "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>");

	# Using lowercase XML
	my $lowerXML = lc $c1->toXML();

	my $c4 = libgmsec_perl::Config->new($lowerXML, $libgmsec_perl::DataType_XML_DATA);
	$test->check("Unexpected XML content", $c4->toXML() eq "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>");

	# Using lowercase JSON
	my $lowerJSON = lc $c1->toJSON();

	my $c5 = libgmsec_perl::Config->new($lowerJSON, $libgmsec_perl::DataType_JSON_DATA);
	$test->check("Unexpected XML content", $c5->toXML() eq "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>");
}


sub test_copy_constructor
{
	libgmsec_perl::logInfo("Test copy-constructor");

	my ($test) = @_;

	my $c1 = libgmsec_perl::Config->new("mw-id=bolt server=localhost", $libgmsec_perl::DataType_KEY_VALUE_DATA);
	my $c2 = libgmsec_perl::Config->new($c1);

	$test->check("Config is not the same as the original", $c1->toXML() eq $c2->toXML());
}


sub test_addValue
{
	libgmsec_perl::logInfo("Test addValue()");

	my ($test) = @_;

	my $c1 = libgmsec_perl::Config->new();

	$test->check("Did not expect to find value", $c1->getValue("mw-id") eq undef);

	$c1->addValue("mw-id", "bolt");

	$test->check("Expected to find value", $c1->getValue("mw-id") eq "bolt");
}


sub test_clearValue
{
	libgmsec_perl::logInfo("Test clearValue()");

	my ($test) = @_;

	my $c1 = libgmsec_perl::Config->new("mw-id=bolt", $libgmsec_perl::DataType_KEY_VALUE_DATA);

	$test->check("Expected to find value", $c1->getValue("mw-id") eq "bolt");

	$c1->clearValue("mw-id");

	$test->check("Did not expect to find value", $c1->getValue("mw-id") eq undef);
}


sub test_getValue
{
	libgmsec_perl::logInfo("Test getValue()");

	my ($test) = @_;

	my $c1 = libgmsec_perl::Config->new("mw-id=bolt", $libgmsec_perl::DataType_KEY_VALUE_DATA);

	$test->check("Unexpected value", $c1->getValue("mw-id") eq "bolt");
	$test->check("Unexpected value", $c1->getValue("server") eq undef);

	libgmsec_perl::logInfo("Test getValue() w/ default value");

	$test->check("Unexpected default value", $c1->getValue("connectionType", "mb") eq "mb");
	$test->check("Unexpected default value", $c1->getValue(undef, "mb") eq "mb");
}


sub test_getBooleanValue
{
	libgmsec_perl::logInfo("Test getBooleanValue()");

	my ($test) = @_;

	my $c1 = libgmsec_perl::Config->new("gmsec-rocks=True", $libgmsec_perl::DataType_KEY_VALUE_DATA);
	my $c2 = libgmsec_perl::Config->new("gmsec-rocks=sure", $libgmsec_perl::DataType_KEY_VALUE_DATA);

	$test->check("Unexpected boolean value", $c1->getBooleanValue("gmsec-rocks"));

	eval
	{
		$c1->getBooleanValue(undef);
		$test->check("Expected getBooleanValue() to throw exception", 0);
	};
    if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 1);
	}

	eval
	{
		$c2->getBooleanValue("gmsec-rocks");
		$test->check("Expected getBooleanValue() to throw exception", 0);
	};
    if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 1);
	}

	libgmsec_perl::logInfo("Test getBooleanValue() w/ default value");

	$test->check("Expected getBooleanValue() to return default", $c1->getBooleanValue("gmsec-missing", 0) == 0);
	$test->check("Expected getBooleanValue() to return default", $c1->getBooleanValue("gmsec-missing", 1));
	$test->check("Expected getBooleanValue() to return default", $c2->getBooleanValue("gmsec-rocks", 1));
	$test->check("Expected getBooleanValue() to return default", $c2->getBooleanValue("", 1));
	$test->check("Expected getBooleanValue() to return default", $c2->getBooleanValue(undef, 0) == 0);
}


sub test_getIntegerValue
{
	libgmsec_perl::logInfo("Test getIntegerValue()");

	my ($test) = @_;

	my $c1 = libgmsec_perl::Config->new("gmsec-value-1=10", $libgmsec_perl::DataType_KEY_VALUE_DATA);
	my $c2 = libgmsec_perl::Config->new("gmsec-value-2=NAN", $libgmsec_perl::DataType_KEY_VALUE_DATA);

	$test->check("Unexpected integer value", $c1->getIntegerValue("gmsec-value-1") == 10);

	eval
	{
		$c1->getIntegerValue(undef);
		$test->check("Expected getIntegerValue() to throw exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 1);
	}

	eval
	{
		$c2->getIntegerValue("gmsec-value-2");
		$test->check("Expected getIntegerValue() to throw exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 1);
	}

	libgmsec_perl::logInfo("Test getIntegerValue() w/ default value");

	$test->check("Expected getIntegerValue() to return default", $c1->getIntegerValue("gmsec-missing", 20) == 20);
	$test->check("Expected getIntegerValue() to return default", $c2->getIntegerValue("gmsec-value-2", 30) == 30);
	$test->check("Expected getIntegerValue() to return default", $c2->getIntegerValue("", 10) == 10);
	$test->check("Expected getIntegerValue() to return default", $c2->getIntegerValue(undef, 10) == 10);
}


sub test_getDoubleValue
{
	libgmsec_perl::logInfo("Test getDoubleValue()");

	my ($test) = @_;

	my $c1 = libgmsec_perl::Config->new("gmsec-value-1=10.15", $libgmsec_perl::DataType_KEY_VALUE_DATA);
	my $c2 = libgmsec_perl::Config->new("gmsec-value-2=NAN", $libgmsec_perl::DataType_KEY_VALUE_DATA);

	$test->check("Unexpected double value", $c1->getDoubleValue("gmsec-value-1") == 10.15);

	eval
	{
		$c1->getDoubleValue(undef);
		$test->check("Expected getDoubleValue() to throw exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 1);
	}

	eval
	{
		$c2->getDoubleValue("gmsec-value-2");
		$test->check("Expected getDoubleValue() to throw exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 1);
	}

	libgmsec_perl::logInfo("Test getDoubleValue() w/ default value");

	$test->check("Expected getDoubleValue() to return default", $c1->getDoubleValue("gmsec-missing", 20.22) == 20.22);
	$test->check("Expected getDoubleValue() to return default", $c2->getDoubleValue("gmsec-value-2", 30.33) == 30.33);
	$test->check("Expected getDoubleValue() to return default", $c2->getDoubleValue("", 10.33) == 10.33);
	$test->check("Expected getDoubleValue() to return default", $c2->getDoubleValue(undef, 10.33) == 10.33);
}


sub test_clear
{
	libgmsec_perl::logInfo("Test clear()");

	my ($test) = @_;

	my $c1 = libgmsec_perl::Config->new("mw-id=bolt server=localhost", $libgmsec_perl::DataType_KEY_VALUE_DATA);

	$c1->clear();

	$test->check("Unexpected XML content", $c1->toXML() eq "<CONFIG>\n</CONFIG>");
}


sub test_merge
{
	libgmsec_perl::logInfo("Test merge()");

	my ($test) = @_;

	my $c1 = libgmsec_perl::Config->new("mw-id=bolt", $libgmsec_perl::DataType_KEY_VALUE_DATA);
	my $c2 = libgmsec_perl::Config->new("mw-id=activemq39", $libgmsec_perl::DataType_KEY_VALUE_DATA);
	my $c3 = libgmsec_perl::Config->new("mw-id=mb server=localhost", $libgmsec_perl::DataType_KEY_VALUE_DATA);

	# Overwrite existing config entries
	$c1->merge($c3, 1);

	$test->check("Unexpected mw-id value", $c1->getValue("mw-id") eq "mb");
	$test->check("Unexpected server value", $c1->getValue("server") eq "localhost");

	# Do not overwrite existing entries
	$c2->merge($c3, 0);

	$test->check("Unexpected mw-id value", $c2->getValue("mw-id") eq "activemq39");
	$test->check("Unexpected server value", $c2->getValue("server") eq "localhost");
}


sub test_toXML
{
	libgmsec_perl::logInfo("Test toXML()");

	my ($test) = @_;

	my $xml  = "<CONFIG>\n"
	           . "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
	           . "\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
	           . "</CONFIG>";

	my $c1 = libgmsec_perl::Config->new("mw-id=bolt server=localhost", $libgmsec_perl::DataType_KEY_VALUE_DATA);

	$test->check("Unexpected XML content", $c1->toXML() eq $xml);
}


sub test_fromXML
{
	libgmsec_perl::logInfo("Test fromXML()");

	my ($test) = @_;

	my $xml  = "<CONFIG>\n"
			   . "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
			   . "\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
			   . "</CONFIG>";

	my $c1 = libgmsec_perl::Config->new();

	$c1->fromXML($xml);

	$test->check("Unexpected XML content", $c1->toXML() eq $xml);
}


sub test_getFromFile
{
	libgmsec_perl::logInfo("Test getFromFile()");

	my ($test) = @_;

    # Nominal test
    eval
    {
        my $config1 = libgmsec_perl::Config::getFromFile( $test->getDataFile("T002_Config_good.xml"), "Bolt" );
        $test->check("Unexpected return value", defined $config1);
        $test->check("Unexpected mw-id value", $config1->getValue("mw-id") eq "bolt");
        $test->check("Unexpected server value", $config1->getValue("server") eq "localhost");

        my $config2 = libgmsec_perl::Config::getFromFile( $test->getDataFile("T002_Config_good.xml"), "ActiveMQ" );
        $test->check("Unexpected return value", defined $config2);
        $test->check("Unexpected mw-id value", $config2->getValue("mw-id") eq "activemq39");
        $test->check("Unexpected server value", $config2->getValue("server") eq "tcp://localhost:61616");

        my $config3 = libgmsec_perl::Config::getFromFile( $test->getDataFile("T002_Config_good.xml"), "" );
        $test->check("Unexpected return value", defined $config3);
        $test->check("Unexpected mw-id value", $config3->getValue("mw-id") eq "bolt");
        $test->check("Unexpected server value", $config3->getValue("server") eq "localhost");

    };
    if (isa($@, 'libgmsec_perl::GmsecException'))
    {
        my $error = $@;
        $test->check($error->what(), 0);
    }

    # Off-nominal tests
    eval
    {
        libgmsec_perl::Config::getFromFile( $test->getDataFile("T002_Config_bad.xml"), "Bolt" );
        $test->check("Unexpected result", 0);
    };
    if (isa($@, 'libgmsec_perl::GmsecException'))
    {
        my $error = $@;
        $test->check($error->what(), index($error->what(), "Invalid XML format -- parse error") != -1);
    }

    eval
    {
        libgmsec_perl::Config::getFromFile( "file-does-not-exist", "Bolt" );
        $test->check("Unexpected result", 0);
    };
    if (isa($@, 'libgmsec_perl::GmsecException'))
    {
        my $error = $@;
        $test->check($error->what(), index($error->what(), "Configuration file could not be found or opened") != -1);
    }
}


sub test_toJSON
{
	libgmsec_perl::logInfo("Test toJSON()");

	my ($test) = @_;

	my $json = "{\"CONFIG\":{\"PARAMETER\":[{\"NAME\":\"mw-id\",\"VALUE\":\"bolt\"},{\"NAME\":\"server\",\"VALUE\":\"localhost\"}]}}";

	my $c1 = libgmsec_perl::Config->new("mw-id=bolt server=localhost", $libgmsec_perl::DataType_KEY_VALUE_DATA);

	$test->check("Unexpected JSON content", $c1->toJSON() eq $json);
}


Test::driver(\&Test_Config);
