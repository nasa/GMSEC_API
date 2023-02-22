#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


sub Test_MessageFactory
{
	my ($test) = @_;

	$test->setDescription("Test MessageFactory");

	test_create($test);
	test_setStandardFields($test);
	test_clearStandardFields($test);
	test_setMessageConfig($test);
	test_createMessage_1($test);
	test_createMessage_2($test);
	test_fromData($test);
	test_getSpecification($test);
	test_registerMessageValidator($test);
}


sub test_create
{
	libgmsec_perl::logInfo("Test create()");

	my ($test) = @_;

	# No config
	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->check("Unexpected specification version", $msgFactory->getSpecification()->getVersion() == $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT);
	$test->check("Unexpected specification schema level", $msgFactory->getSpecification()->getSchemaLevel() == $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);

	libgmsec_perl::MessageFactory::destroy($msgFactory);


	# With config
	my $config = libgmsec_perl::Config->new("gmsec-schema-level=2", $libgmsec_perl::DataType_KEY_VALUE_DATA);

	$msgFactory = libgmsec_perl::MessageFactory::create($config);

	$test->check("Unexpected specification version", $msgFactory->getSpecification()->getVersion() == $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT);
	$test->check("Unexpected specification schema level", $msgFactory->getSpecification()->getSchemaLevel() == $libgmsec_perl::Specification::SchemaLevel_LEVEL_2);

	libgmsec_perl::MessageFactory::destroy($msgFactory);
}


sub test_setStandardFields
{
	libgmsec_perl::logInfo("Test setStandardFields()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	my $msg = $msgFactory->createMessage();

	$test->verifyStandardFields($msg);

	$msgFactory->clearStandardFields();

	libgmsec_perl::MessageFactory::destroy($msgFactory);
}


sub test_clearStandardFields
{
	libgmsec_perl::logInfo("Test clearStandardFields()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	my $msg1 = $msgFactory->createMessage();

	$test->verifyStandardFields($msg1);

	$msgFactory->clearStandardFields();

	my $msg2 = $msgFactory->createMessage();

	$test->check("Expected message field count to be 0 (zero)", $msg2->getFieldCount() == 0);

	libgmsec_perl::MessageFactory::destroy($msgFactory);
}


sub test_setMessageConfig
{
	libgmsec_perl::logInfo("Test setMessageConfig()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msgConfig = libgmsec_perl::Config->new("tracking=false", $libgmsec_perl::DataType_KEY_VALUE_DATA);

	$msgFactory->setMessageConfig($msgConfig);

	my $msg = $msgFactory->createMessage();

	$test->check("Unexpected message config", $msg->getConfig()->toXML() eq $msgConfig->toXML());

	libgmsec_perl::MessageFactory::destroy($msgFactory);
}


sub test_createMessage_1
{
	libgmsec_perl::logInfo("Test createMessage() w/ no args");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage();

	$test->check("Expected no fields in the message", $msg->getFieldCount() == 0);

	libgmsec_perl::MessageFactory::destroy($msgFactory);
}


sub test_createMessage_2
{
	libgmsec_perl::logInfo("Test createMessage()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	# Nominal case (valid schema ID)
	my $msg = $msgFactory->createMessage("HB");

	$test->check("Unexpected MESSAGE-TYPE", $msg->getStringValue("MESSAGE-TYPE") eq "MSG");
	$test->check("Unexpected MESSAGE-SUBTYPE", $msg->getStringValue("MESSAGE-SUBTYPE") eq "HB");

	# Off-nominal case (invalid schema ID)
	eval
	{
		$msg = $msgFactory->createMessage("BOGUS");
		$test->check("Expected an exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "SchemaID \"BOGUS\" could not be found in list of available schema") != -1);
	}

	# Off-nominal case (empty-string schema ID)
	eval
	{
		$msg = $msgFactory->createMessage("");
		$test->check("Expected an exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "SchemaID cannot be NULL, nor be an empty string") != -1);
	}

	# Off-nominal case (null schema ID)
	eval
	{
		$msg = $msgFactory->createMessage(undef);
		$test->check("Expected an exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "SchemaID cannot be NULL, nor be an empty string") != -1);
	}

	libgmsec_perl::MessageFactory::destroy($msgFactory);
}


sub test_fromData
{
	libgmsec_perl::logInfo("Test fromData()");

	my ($test) = @_;

	# Nominal tests
	my $xml = "";
	$xml .= "<MESSAGE SUBJECT=\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\" KIND=\"PUBLISH\">\n";
	$xml .= "\t<FIELD NAME=\"COMPONENT\" TYPE=\"STRING\">GMPUB</FIELD>\n";
	$xml .= "\t<FIELD NAME=\"CONSTELLATION-ID\" TYPE=\"STRING\">CNS1</FIELD>\n";
	$xml .= "\t<FIELD NAME=\"CONTENT-VERSION\" TYPE=\"F32\" BITS=\"44FC6000\">2019</FIELD>\n";
	$xml .= "\t<FIELD NAME=\"COUNTER\" TYPE=\"U16\">1</FIELD>\n";
	$xml .= "\t<FIELD NAME=\"DOMAIN1\" TYPE=\"STRING\">DOM1</FIELD>\n";
	$xml .= "\t<FIELD NAME=\"DOMAIN2\" TYPE=\"STRING\">DOM2</FIELD>\n";
	$xml .= "\t<FIELD NAME=\"FACILITY\" TYPE=\"STRING\">MY-FACILITY</FIELD>\n";
	$xml .= "\t<FIELD NAME=\"HEADER-VERSION\" TYPE=\"F32\" BITS=\"44FC6000\">2019</FIELD>\n";
	$xml .= "\t<FIELD NAME=\"MESSAGE-SUBTYPE\" TYPE=\"STRING\">HB</FIELD>\n";
	$xml .= "\t<FIELD NAME=\"MESSAGE-TYPE\" TYPE=\"STRING\">MSG</FIELD>\n";
	$xml .= "\t<FIELD NAME=\"MISSION-ID\" TYPE=\"STRING\">MSSN</FIELD>\n";
	$xml .= "\t<FIELD NAME=\"PUB-RATE\" TYPE=\"U16\">1</FIELD>\n";
	$xml .= "\t<FIELD NAME=\"SAT-ID-LOGICAL\" TYPE=\"STRING\">SAT1</FIELD>\n";
	$xml .= "\t<FIELD NAME=\"SAT-ID-PHYSICAL\" TYPE=\"STRING\">SAT1</FIELD>\n";
	$xml .= "\t<FIELD NAME=\"SPECIFICATION\" TYPE=\"STRING\">GMSEC</FIELD>\n";
	$xml .= "</MESSAGE>";

	my $xml_lower = "";
	$xml_lower .= "<message subject=\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\" kind=\"publish\">\n";
	$xml_lower .= "\t<field name=\"COMPONENT\" type=\"STRING\">GMPUB</field>\n";
	$xml_lower .= "\t<field name=\"CONSTELLATION-ID\" type=\"STRING\">CNS1</field>\n";
	$xml_lower .= "\t<field name=\"CONTENT-VERSION\" type=\"F32\" bits=\"44FC6000\">2019</field>\n";
	$xml_lower .= "\t<field name=\"COUNTER\" type=\"U16\">1</field>\n";
	$xml_lower .= "\t<field name=\"DOMAIN1\" type=\"STRING\">DOM1</field>\n";
	$xml_lower .= "\t<field name=\"DOMAIN2\" type=\"STRING\">DOM2</field>\n";
	$xml_lower .= "\t<field name=\"FACILITY\" type=\"STRING\">MY-FACILITY</field>\n";
	$xml_lower .= "\t<field name=\"HEADER-VERSION\" type=\"F32\" bits=\"44FC6000\">2019</field>\n";
	$xml_lower .= "\t<field name=\"MESSAGE-SUBTYPE\" type=\"STRING\">HB</field>\n";
	$xml_lower .= "\t<field name=\"MESSAGE-TYPE\" type=\"STRING\">MSG</field>\n";
	$xml_lower .= "\t<field name=\"MISSION-ID\" type=\"STRING\">MSSN</field>\n";
	$xml_lower .= "\t<field name=\"PUB-RATE\" type=\"U16\">1</field>\n";
	$xml_lower .= "\t<field name=\"SAT-ID-LOGICAL\" type=\"STRING\">SAT1</field>\n";
	$xml_lower .= "\t<field name=\"SAT-ID-PHYSICAL\" type=\"STRING\">SAT1</field>\n";
	$xml_lower .= "\t<field name=\"SPECIFICATION\" type=\"STRING\">GMSEC</field>\n";
	$xml_lower .= "</message>";

	my $json = "";
	$json .= "{\"MESSAGE\":{\"SUBJECT\":\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\",\"KIND\":\"PUBLISH\",";
	$json .= "\"FIELD\":[";
	$json .= "{\"NAME\":\"COMPONENT\",\"TYPE\":\"STRING\",\"VALUE\":\"GMPUB\"},";
	$json .= "{\"NAME\":\"CONSTELLATION-ID\",\"TYPE\":\"STRING\",\"VALUE\":\"CNS1\"},";
	$json .= "{\"NAME\":\"CONTENT-VERSION\",\"TYPE\":\"F32\",\"BITS\":\"44FC6000\",\"VALUE\":\"2019\"},";
	$json .= "{\"NAME\":\"COUNTER\",\"TYPE\":\"U16\",\"VALUE\":\"1\"},";
	$json .= "{\"NAME\":\"DOMAIN1\",\"TYPE\":\"STRING\",\"VALUE\":\"DOM1\"},";
	$json .= "{\"NAME\":\"DOMAIN2\",\"TYPE\":\"STRING\",\"VALUE\":\"DOM2\"},";
	$json .= "{\"NAME\":\"FACILITY\",\"TYPE\":\"STRING\",\"VALUE\":\"MY-FACILITY\"},";
	$json .= "{\"NAME\":\"HEADER-VERSION\",\"TYPE\":\"F32\",\"BITS\":\"44FC6000\",\"VALUE\":\"2019\"},";
	$json .= "{\"NAME\":\"MESSAGE-SUBTYPE\",\"TYPE\":\"STRING\",\"VALUE\":\"HB\"},";
	$json .= "{\"NAME\":\"MESSAGE-TYPE\",\"TYPE\":\"STRING\",\"VALUE\":\"MSG\"},";
	$json .= "{\"NAME\":\"MISSION-ID\",\"TYPE\":\"STRING\",\"VALUE\":\"MSSN\"},";
	$json .= "{\"NAME\":\"PUB-RATE\",\"TYPE\":\"U16\",\"VALUE\":\"1\"},";
	$json .= "{\"NAME\":\"SAT-ID-LOGICAL\",\"TYPE\":\"STRING\",\"VALUE\":\"SAT1\"},";
	$json .= "{\"NAME\":\"SAT-ID-PHYSICAL\",\"TYPE\":\"STRING\",\"VALUE\":\"SAT1\"},";
	$json .= "{\"NAME\":\"SPECIFICATION\",\"TYPE\":\"STRING\",\"VALUE\":\"GMSEC\"}";
	$json .= "]}}";

	my $json_lower = "";
	$json_lower .= "{\"message\":{\"subject\":\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\",\"kind\":\"publish\",";
	$json_lower .= "\"field\":[";
	$json_lower .= "{\"name\":\"COMPONENT\",\"type\":\"string\",\"value\":\"GMPUB\"},";
	$json_lower .= "{\"name\":\"CONSTELLATION-ID\",\"type\":\"string\",\"value\":\"CNS1\"},";
	$json_lower .= "{\"name\":\"CONTENT-VERSION\",\"type\":\"f32\",\"bits\":\"44FC6000\",\"value\":\"2019\"},";
	$json_lower .= "{\"name\":\"COUNTER\",\"type\":\"u16\",\"value\":\"1\"},";
	$json_lower .= "{\"name\":\"DOMAIN1\",\"type\":\"string\",\"value\":\"DOM1\"},";
	$json_lower .= "{\"name\":\"DOMAIN2\",\"type\":\"string\",\"value\":\"DOM2\"},";
	$json_lower .= "{\"name\":\"FACILITY\",\"type\":\"string\",\"value\":\"MY-FACILITY\"},";
	$json_lower .= "{\"name\":\"HEADER-VERSION\",\"type\":\"f32\",\"bits\":\"44FC6000\",\"value\":\"2019\"},";
	$json_lower .= "{\"name\":\"MESSAGE-SUBTYPE\",\"type\":\"string\",\"value\":\"HB\"},";
	$json_lower .= "{\"name\":\"MESSAGE-TYPE\",\"type\":\"string\",\"value\":\"MSG\"},";
	$json_lower .= "{\"name\":\"MISSION-ID\",\"type\":\"string\",\"value\":\"MSSN\"},";
	$json_lower .= "{\"name\":\"PUB-RATE\",\"type\":\"u16\",\"value\":\"1\"},";
	$json_lower .= "{\"name\":\"SAT-ID-LOGICAL\",\"type\":\"string\",\"value\":\"SAT1\"},";
	$json_lower .= "{\"name\":\"SAT-ID-PHYSICAL\",\"type\":\"string\",\"value\":\"SAT1\"},";
	$json_lower .= "{\"name\":\"SPECIFICATION\",\"type\":\"string\",\"value\":\"GMSEC\"}";
	$json_lower .= "]}}";

	my $config     = libgmsec_perl::Config->new("gmsec-schema-level=2", $libgmsec_perl::DataType_KEY_VALUE_DATA);
	my $msgFactory = libgmsec_perl::MessageFactory::create( $config );

	my $msg1 = fromData($test, $msgFactory, $xml, $libgmsec_perl::DataType_XML_DATA, undef);
	my $msg2 = fromData($test, $msgFactory, $json, $libgmsec_perl::DataType_JSON_DATA, undef);
	my $msg3 = fromData($test, $msgFactory, $xml_lower, $libgmsec_perl::DataType_XML_DATA, undef);
	my $msg4 = fromData($test, $msgFactory, $json_lower, $libgmsec_perl::DataType_JSON_DATA, undef);

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$test->check("Unexpected XML",  $msg1->toXML() eq $xml);
	$test->check("Unexpected JSON", $msg2->toJSON() eq $json);
	$test->check("Unexpected XML",  $msg3->toXML() eq $xml);
	$test->check("Unexpected JSON", $msg4->toJSON() eq $json);

	$test->check("Message 1 missing CONTENT-VERSION", $msg1->hasField("CONTENT-VERSION"));
	$test->check("Message 1 missing HEADER-VERSION", $msg1->hasField("HEADER-VERSION"));
	$test->check("Message 1 missing MESSAGE-SUBTYPE", $msg1->hasField("MESSAGE-SUBTYPE"));
	$test->check("Message 1 missing MESSAGE-TYPE", $msg1->hasField("MESSAGE-TYPE"));
	$test->check("Message 1 missing PUB-RATE", $msg1->hasField("PUB-RATE"));

	$test->check("Message 2 missing CONTENT-VERSION", $msg2->hasField("CONTENT-VERSION"));
	$test->check("Message 2 missing HEADER-VERSION", $msg2->hasField("HEADER-VERSION"));
	$test->check("Message 2 missing MESSAGE-SUBTYPE", $msg2->hasField("MESSAGE-SUBTYPE"));
	$test->check("Message 2 missing MESSAGE-TYPE", $msg2->hasField("MESSAGE-TYPE"));
	$test->check("Message 2 missing PUB-RATE", $msg2->hasField("PUB-RATE"));

	$test->check("Message 3 missing CONTENT-VERSION", $msg3->hasField("CONTENT-VERSION"));
	$test->check("Message 3 missing HEADER-VERSION", $msg3->hasField("HEADER-VERSION"));
	$test->check("Message 3 missing MESSAGE-SUBTYPE", $msg3->hasField("MESSAGE-SUBTYPE"));
	$test->check("Message 3 missing MESSAGE-TYPE", $msg3->hasField("MESSAGE-TYPE"));
	$test->check("Message 3 missing PUB-RATE", $msg3->hasField("PUB-RATE"));

	$test->check("Message 4 missing CONTENT-VERSION", $msg4->hasField("CONTENT-VERSION"));
	$test->check("Message 4 missing HEADER-VERSION", $msg4->hasField("HEADER-VERSION"));
	$test->check("Message 4 missing MESSAGE-SUBTYPE", $msg4->hasField("MESSAGE-SUBTYPE"));
	$test->check("Message 4 missing MESSAGE-TYPE", $msg4->hasField("MESSAGE-TYPE"));
	$test->check("Message 4 missing PUB-RATE", $msg4->hasField("PUB-RATE"));

	# Off-nominal tests
	$msgFactory = libgmsec_perl::MessageFactory::create();

	my $invalid_msg_subject = "<MESSAGE SUBJECT=\"GMSEC FOO BAR\"/>";
	my $invalid_xml         = "<MESSAGE SUBJECT=\"GMSEC.FOO.BAR\">";
	my $invalid_json        = "{\"MESSAGE\":{\"SUBJECT\":\"GMSEC.FOO.BAR\"";
	my $empty_str           = "";
	my $undef_str           = undef;

	fromData($test, $msgFactory, $invalid_msg_subject, $libgmsec_perl::DataType_XML_DATA, "Invalid message subject");
	fromData($test, $msgFactory, $invalid_xml, $libgmsec_perl::DataType_XML_DATA, "Mismatched element");
	fromData($test, $msgFactory, $invalid_json, $libgmsec_perl::DataType_JSON_DATA, "Invalid JSON message format -- invalid JSON string data");
	fromData($test, $msgFactory, $empty_str, $libgmsec_perl::DataType_XML_DATA, "Data string cannot be NULL, nor empty");
	fromData($test, $msgFactory, $undef_str, $libgmsec_perl::DataType_JSON_DATA, "Data string cannot be NULL, nor empty");

	libgmsec_perl::MessageFactory::destroy($msgFactory);
}


sub test_getSpecification
{
	libgmsec_perl::logInfo("Test getSpecification()");

	my ($test) = @_;

	my $msgFactory1 = libgmsec_perl::MessageFactory::create();
	my $msgFactory2 = libgmsec_perl::MessageFactory::create( libgmsec_perl::Config->new("gmsec-specification-version=201900", $libgmsec_perl::DataType_KEY_VALUE_DATA) );

	$test->check("Unexpected specification version", $msgFactory1->getSpecification()->getVersion() == $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT);
	$test->check("Unexpected specification version", $msgFactory2->getSpecification()->getVersion() == $libgmsec_perl::GMSEC_MSG_SPEC_2019_00);

	libgmsec_perl::MessageFactory::destroy($msgFactory1);
	libgmsec_perl::MessageFactory::destroy($msgFactory2);
}


sub test_registerMessageValidator
{
	libgmsec_perl::logInfo("Test registerMessageValidator()");

	my ($test) = @_;

	my $val1 = MyValidator->new(1);
	my $val2 = MyValidator->new(0);

	# Nominal case
	my $msgFactory1 = libgmsec_perl::MessageFactory::create();
	$test->setStandardFields($msgFactory1);
	$msgFactory1->registerMessageValidator($val1);
	my $msg1 = $msgFactory1->createMessage("HB");
	my $status1 = $msg1->isCompliant();
	$test->check("Expected message to be compliant", $status1->hasError() == 0);
	libgmsec_perl::MessageFactory::destroy($msgFactory1);

	# Missing standard fields
	my $msgFactory2 = libgmsec_perl::MessageFactory::create();
	$msgFactory2->registerMessageValidator($val2);
	my $msg2 = $msgFactory2->createMessage("HB");
	my $status2 = $msg2->isCompliant();
	$test->check("Expected message to be non-compliant", $status2->hasError());
	$test->check($status2->getReason(), index($status2->getReason(), "MyValidator is not happy with the message") != -1);
	libgmsec_perl::MessageFactory::destroy($msgFactory2);

	# Off-nominal case
	my $msgFactory = undef;
	eval
	{
		$msgFactory = libgmsec_perl::MessageFactory::create();
		$msgFactory->registerMessageValidator(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "MessageValidator cannot be null") != -1);
	}
	libgmsec_perl::MessageFactory::destroy($msgFactory);
}


sub fromData
{
	my ($test, $msgFactory, $data, $type, $expectedError) = @_;

	my $msg = undef;

	eval
	{
		$msg = $msgFactory->fromData($data, $type);

		if ($expectedError)
		{
			$test->check("Expected an error: " . $expectedError);
		}
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;

		if ($expectedError eq undef)
		{
			$test->check($error->what(), 0);
		}
		else
		{
			$test->check($error->what(), index($error->what(), $expectedError) != -1);
		}
	}

	return $msg;
}


# MyValidator
{
	package MyValidator;
	use base 'libgmsec_perl::MessageValidator';

	sub new
	{
		my $class = shift;
		my $self = $class->SUPER::new();
		($self->{NOMINAL}, $self->{USED}) = @_;
		return $self;
	}

	sub wasUsed
	{
		my ($self) = @_;
		return $self->{USED};
	}

	sub reset
	{
		my ($self) = @_;
		$self->{USED} = 0;
	}

	sub validateMessage
	{
		my ($self, $msg) = @_;

		my $status = libgmsec_perl::Status->new();

		if ($self->{NOMINAL} == 0)
		{
			$status = libgmsec_perl::Status->new($libgmsec_perl::MSG_ERROR, $libgmsec_perl::MESSAGE_FAILED_VALIDATION, "MyValidator is not happy with the message");
		}

		return $status;
	}
}


Test::driver(\&Test_MessageFactory);
