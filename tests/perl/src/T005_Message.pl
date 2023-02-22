#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


sub Test_Message
{
	my ($test) = @_;

	$test->setDescription("Test Message");

	test_constructor($test);
	test_copy_constructor($test);
	test_getSchemaID($test);
	test_getVersion($test);
	test_getSchemaLevel($test);
	test_isCompliant($test);
	test_registerMessageValidator($test);
	test_setFieldValue($test);
	test_setConfig($test);
	test_setSubject($test);
	test_getSubject($test);
	test_setKind($test);
	test_getKind($test);
	test_addField($test);
	test_addFields($test);
	test_clearFields($test);
	test_clearField($test);
	test_getStringValue($test);
	test_getBooleanValue($test);
	test_getIntegerValue($test);
	test_getDoubleValue($test);
	test_hasField($test);
	test_getField($test);
	test_getFieldType($test);
	test_getBinaryField($test);
	test_getBooleanField($test);
	test_getCharField($test);
	test_getF32Field($test);
	test_getF64Field($test);
	test_getI8Field($test);
	test_getI16Field($test);
	test_getI32Field($test);
	test_getI64Field($test);
	test_getU8Field($test);
	test_getU16Field($test);
	test_getU32Field($test);
	test_getU64Field($test);
	test_getStringField($test);
	test_getFieldCount($test);
	test_copyFields($test);
	test_toXML($test);
	test_toJSON($test);
	test_getSize($test);
	test_getFieldIterator($test);
}


sub test_constructor
{
	libgmsec_perl::logInfo("Test constructor");

	my ($test) = @_;

	my $msg = libgmsec_perl::Message->new();

	$test->check("Unexpected subject", $msg->getSubject() eq "");
	$test->check("Unexpected kind", $msg->getKind() == $libgmsec_perl::Message::Kind_PUBLISH);
	$test->check("Unexpected schema ID", $msg->getSchemaID() == "");
	$test->check("Unexpected version", $msg->getVersion() == $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT);
	$test->check("Unexpected schema level", $msg->getSchemaLevel() == $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);
	$test->check("Unexpected compliancy", $msg->isCompliant()->hasError() == 1);
	$test->check("Unexpected field count", $msg->getFieldCount() == 0);
	$test->check("Unexpected XML", $msg->toXML() eq "<MESSAGE SUBJECT=\"\" KIND=\"PUBLISH\"/>");
	$test->check("Unexpected JSON", $msg->toJSON() eq "{\"MESSAGE\":{\"SUBJECT\":\"\",\"KIND\":\"PUBLISH\",}}");
}


sub test_copy_constructor
{
	libgmsec_perl::logInfo("Test copy-constructor");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	my $msg1 = $msgFactory->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	# Nominal test
	my $msg2 = libgmsec_perl::Message->new($msg1);

	$test->check("Unexpected subject", $msg1->getSubject() eq $msg2->getSubject());
	$test->check("Unexpected kind", $msg1->getKind() == $msg2->getKind());
	$test->check("Unexpected schema ID", $msg1->getSchemaID() == $msg2->getSchemaID());
	$test->check("Unexpected version", $msg1->getVersion() == $msg2->getVersion());
	$test->check("Unexpected schema level", $msg1->getSchemaLevel() == $msg2->getSchemaLevel());
	$test->check("Expected compliant message", $msg1->isCompliant()->hasError() == $msg2->isCompliant()->hasError());
	$test->check("Unexpected field count", $msg1->getFieldCount() == $msg2->getFieldCount());
	$test->check("Unexpected XML", $msg1->toXML() eq $msg2->toXML());
	$test->check("Unexpected JSON", $msg1->toJSON() eq $msg2->toJSON());
}


sub test_getSchemaID
{
	libgmsec_perl::logInfo("Test getSchemaID()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	my $msg1 = $msgFactory->createMessage("HB");
	my $msg2 = $msgFactory->createMessage("MVAL");
	my $msg3 = $msgFactory->createMessage("TLMFRAME");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$test->check("Message 1 has unexpected schema ID", $msg1->getSchemaID() == "MSG.HB");
	$test->check("Message 2 has unexpected schema ID", $msg2->getSchemaID() == "MSG.MVAL");
	$test->check("Message 3 has unexpected schema ID", $msg3->getSchemaID() == "MSG.TLMFRAME");
}


sub test_getVersion
{
	libgmsec_perl::logInfo("Test getVersion()");

	my ($test) = @_;

	my $msgFactory1 = libgmsec_perl::MessageFactory::create();
	my $msgFactory2 = libgmsec_perl::MessageFactory::create( libgmsec_perl::Config->new("gmsec-specification-version=201900", $libgmsec_perl::DataType_KEY_VALUE_DATA) );

	my $msg1 = $msgFactory1->createMessage("HB");
	my $msg2 = $msgFactory2->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory1);
	libgmsec_perl::MessageFactory::destroy($msgFactory2);

	$test->check("Message 1 has unexpected version", $msg1->getVersion() == $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT);
	$test->check("Message 2 has unexpected version", $msg2->getVersion() == $libgmsec_perl::GMSEC_MSG_SPEC_2019_00);
}


sub test_getSchemaLevel
{
	libgmsec_perl::logInfo("Test getSchemaLevel()");

	my ($test) = @_;

	my $msgFactory1 = libgmsec_perl::MessageFactory::create();
	my $msgFactory2 = libgmsec_perl::MessageFactory::create( libgmsec_perl::Config->new("gmsec-schema-level=2", $libgmsec_perl::DataType_KEY_VALUE_DATA) );

	my $msg1 = $msgFactory1->createMessage("HB");
	my $msg2 = $msgFactory2->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory1);
	libgmsec_perl::MessageFactory::destroy($msgFactory2);

	$test->check("Message 1 has unexpected version", $msg1->getSchemaLevel() == $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);
	$test->check("Message 2 has unexpected version", $msg2->getSchemaLevel() == $libgmsec_perl::Specification::SchemaLevel_LEVEL_2);
}


sub test_isCompliant
{
	libgmsec_perl::logInfo("Test isCompliant()");

	my ($test) = @_;

	my $config = libgmsec_perl::Config->new( $test->getConfig() );   # make a copy
	$config->addValue("gmsec-msg-content-validate", "1");

	my $msgFactory = libgmsec_perl::MessageFactory::create($config);

	$test->setStandardFields($msgFactory);

	my $msg = $msgFactory->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	# Nominal test
	$test->check("Expected compliant message", $msg->isCompliant()->hasError() == 0);

	# Off-Nominal test
	$msg->clearField("DOMAIN1");
	$test->check("Expected non-compliant message", $msg->isCompliant()->hasError() == 1);
}


sub test_registerMessageValidator
{
	libgmsec_perl::logInfo("Test registerMessageValidator()");

	my ($test) = @_;

	my $config = libgmsec_perl::Config->new( $test->getConfig() );   # make a copy
	$config->addValue("gmsec-msg-content-validate", "1");

	my $msgFactory = libgmsec_perl::MessageFactory::create($config);

	$test->setStandardFields($msgFactory);

	my $msg = $msgFactory->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	my $validator = T005_MessageValidator->new();

	$msg->registerMessageValidator($validator);

	# Expecting no error
	my $status = $msg->isCompliant();
	$test->check("Expected compliant message", $status->hasError() == 0);

	# Expecting an error
	$status = $msg->isCompliant();
	$test->check("Expected non-compliant message", $status->hasError() == 1);
	$test->check("Expected non-compliant message", $status->getReason() == "Field is missing");

	# Expecting an error
	my $msg2 = libgmsec_perl::Message->new($msg);
	$status = $msg2->isCompliant();
	$test->check("Expected non-compliant message", $status->hasError() == 1);
	$test->check("Expected non-compliant message", $status->getReason() == "Field is missing");
}


sub test_setFieldValue
{
	libgmsec_perl::logInfo("Test setFieldValue()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	my $msg = $msgFactory->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	# Test with string value
	$msg->setFieldValue("PUB-RATE", "5");
	$test->check("PUB-RATE has unexpected field type", $msg->getField("PUB-RATE")->getType() == $libgmsec_perl::Field::Type_U16);
	$test->check("PUB-RATE has unexpected field value", $msg->getIntegerValue("PUB-RATE") == 5);

	# Test with int value
	$msg->setFieldValue("PUB-RATE", 10);
	$test->check("PUB-RATE has unexpected field type", $msg->getField("PUB-RATE")->getType() == $libgmsec_perl::Field::Type_U16);
	$test->check("PUB-RATE has unexpected field value", $msg->getIntegerValue("PUB-RATE") == 10);

	# Test with double value
	$msg->setFieldValue("PUB-RATE", 40.123);
	$test->check("PUB-RATE has unexpected field type", $msg->getField("PUB-RATE")->getType() == $libgmsec_perl::Field::Type_U16);
	$test->check("PUB-RATE has unexpected field value", $msg->getIntegerValue("PUB-RATE") == 40);

    # Verify result returned is '1' when replacing a field
    $test->check("Expected return value of true", $msg->setFieldValue("PUB-RATE", 10) == 1);

    # Verify result returned is '0' when adding a new field
    $test->check("Expected return value of false", $msg->setFieldValue("PUBLISH-RATE", 10) == 0);
}


sub test_setConfig
{
	libgmsec_perl::logInfo("Test setConfig()");

	my ($test) = @_;

	my $config = libgmsec_perl::Config->new("tracking=0", $libgmsec_perl::DataType_KEY_VALUE_DATA);

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	my $msg = $msgFactory->createMessage("TLMPROC");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->setConfig($config);

	$test->check("Message has unexpected Config content", $config->toXML() eq $msg->getConfig()->toXML());
}


sub test_setSubject
{
	libgmsec_perl::logInfo("Test setSubject()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	my $msg = $msgFactory->createMessage("TLMPROC");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->setSubject("GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY");

	$test->check("Message has unexpected subject", $msg->getSubject() eq "GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY");

	# Off-nominal tests
	eval
	{
		# null subject
		my $tmp = libgmsec_perl::Message->new();
		$tmp->setSubject(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Subject cannot be NULL, nor an empty string") != -1);
	}

	eval
	{
		# zero-length subject
		my $tmp = libgmsec_perl::Message->new();
		$tmp.setSubject("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Subject cannot be NULL, nor an empty string") != -1);
	}

	eval
	{
		# invalid subject content
		my $tmp = libgmsec_perl::Message->new();
		$tmp->setSubject("Subject.Cannot.Contain.Lowercase.Letters");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Invalid message subject") != -1);
	}

	eval
	{
		# invalid subject content
		my $tmp = libgmsec_perl::Message->new();
		$tmp->setSubject("HELLO%WORLD");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Invalid message subject") != -1);
	}
}


sub test_getSubject
{
	libgmsec_perl::logInfo("Test getSubject()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	my $msg = $msgFactory->createMessage("TLMPROC");
	$msg->addField( libgmsec_perl::StringField->new("STREAM-MODE", "RT") );  # add ME2 (stream mode)

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	my $subject = $msg->getSubject() . ".2.1";  # add ME2 (virtual channel ID) and ME3 (AP ID)

	$msg->setSubject($subject);

	$test->check("Message has unexpected subject", $msg->getSubject() eq $subject);
}


sub test_setKind
{
	libgmsec_perl::logInfo("Test setKind()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	my $msg = $msgFactory->createMessage("HB");
	$test->check("Expected PUBLISH-kind", $msg->getKind() == $libgmsec_perl::Message::Kind_PUBLISH);

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	# Set to REQUEST-kind, and verify
	$msg->setKind($libgmsec_perl::Message::Kind_REQUEST);
	$test->check("Expected REQUEST-kind", $msg->getKind() == $libgmsec_perl::Message::Kind_REQUEST);

	# Set to REPLY-kind, and verify
	$msg->setKind($libgmsec_perl::Message::Kind_REPLY);
	$test->check("Expected REPLY-kind", $msg->getKind() == $libgmsec_perl::Message::Kind_REPLY);

	# Set to PUBLISH-kind, and verify
	$msg->setKind($libgmsec_perl::Message::Kind_PUBLISH);
	$test->check("Expected PUBLISH-kind", $msg->getKind() == $libgmsec_perl::Message::Kind_PUBLISH);
}


sub test_getKind
{
	libgmsec_perl::logInfo("Test getKind()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	my $msg1 = $msgFactory->createMessage("HB");
	my $msg2 = $msgFactory->createMessage("REQ.DIR");
	my $msg3 = $msgFactory->createMessage("RESP.DIR");
	my $msg4 = $msgFactory->createMessage();
	my $msg5 = libgmsec_perl::Message->new();

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$test->check("Message 1 has unexpected kind", $msg1->getKind() == $libgmsec_perl::Message::Kind_PUBLISH);
	$test->check("Message 2 has unexpected kind", $msg2->getKind() == $libgmsec_perl::Message::Kind_REQUEST);
	$test->check("Message 3 has unexpected kind", $msg3->getKind() == $libgmsec_perl::Message::Kind_REPLY);
	$test->check("Message 4 has unexpected kind", $msg4->getKind() == $libgmsec_perl::Message::Kind_PUBLISH);
	$test->check("Message 5 has unexpected kind", $msg5->getKind() == $libgmsec_perl::Message::Kind_PUBLISH);
}


sub test_addField
{
	libgmsec_perl::logInfo("Test addField()");

	my ($test) = @_;

	my $field = libgmsec_perl::StringField->new("SPECIFICATION", "C2MS");

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("LOG");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	# replace existing field
	$test->check("Expected field to be replaced", $msg->addField($field) == 1);

	my $blob = "";
	for (my $i = 0; $i < 256; $i++)
	{
		$blob = $blob . $i;
	}

	$test->check("Expected field to be added", $msg->addField( libgmsec_perl::BinaryField->new("BINARY", $blob, length $blob) ) == 0);
	$test->check("Expected field to be added", $msg->addField( libgmsec_perl::BooleanField->new("BOOLEAN", 1) ) == 0);
	$test->check("Expected field to be added", $msg->addField( libgmsec_perl::CharField->new("CHAR", 'c') ) == 0);
	$test->check("Expected field to be added", $msg->addField( libgmsec_perl::F32Field->new("FLOAT", 1) ) == 0);
	$test->check("Expected field to be added", $msg->addField( libgmsec_perl::F64Field->new("DOUBLE", 2.4) ) == 0);
	$test->check("Expected field to be added", $msg->addField( libgmsec_perl::I8Field->new("BYTE", 5) ) == 0);
	$test->check("Expected field to be added", $msg->addField( libgmsec_perl::I16Field->new("SHORT", 55) ) == 0);
	$test->check("Expected field to be added", $msg->addField( libgmsec_perl::I32Field->new("INT", 555) ) == 0);
	$test->check("Expected field to be added", $msg->addField( libgmsec_perl::I64Field->new("LONG", 5555) ) == 0);
	$test->check("Expected field to be added", $msg->addField( libgmsec_perl::StringField->new("STRING", "The old fox") ) == 0);
	$test->check("Expected field to be added", $msg->addField( libgmsec_perl::U8Field->new("UBYTE", 5) ) == 0);
	$test->check("Expected field to be added", $msg->addField( libgmsec_perl::U16Field->new("USHORT", 55) ) == 0);
	$test->check("Expected field to be added", $msg->addField( libgmsec_perl::U32Field->new("UINT", 555) ) == 0);
	$test->check("Expected field to be added", $msg->addField( libgmsec_perl::U64Field->new("ULONG", 5555) ) == 0);

	# validate field settings
	$test->check("SPECIFICATION not found", $msg->hasField("SPECIFICATION"));
	$test->check("SPECIFICATION has wrong type", $msg->getField("SPECIFICATION")->getType() == $libgmsec_perl::Field::Type_STRING);
	$test->check("SPECIFICATION has wrong value", $msg->getStringValue("SPECIFICATION") == "C2MS");

	$test->check("BINARY not found", $msg->hasField("BINARY"));
	$test->check("BINARY has wrong type", $msg->getField("BINARY")->getType() == $libgmsec_perl::Field::Type_BINARY);
	$test->check("BINARY has wrong length", $msg->getBinaryField("BINARY")->getLength() == length $blob);

	$test->check("BOOLEAN not found", $msg->hasField("BOOLEAN"));
	$test->check("BOOLEAN has wrong type", $msg->getField("BOOLEAN")->getType() == $libgmsec_perl::Field::Type_BOOL);
	$test->check("BOOLEAN has wrong value", $msg->getBooleanField("BOOLEAN")->getValue() == 1);

	$test->check("CHAR not found", $msg->hasField("CHAR"));
	$test->check("CHAR has wrong type", $msg->getField("CHAR")->getType() == $libgmsec_perl::Field::Type_CHAR);
	$test->check("CHAR has wrong value", $msg->getCharField("CHAR")->getValue() == 'c');

	$test->check("FLOAT not found", $msg->hasField("FLOAT"));
	$test->check("FLOAT has wrong type", $msg->getField("FLOAT")->getType() == $libgmsec_perl::Field::Type_F32);
	$test->check("FLOAT has wrong value", $msg->getF32Field("FLOAT")->getValue() == 1);

	$test->check("DOUBLE not found", $msg->hasField("DOUBLE"));
	$test->check("DOUBLE has wrong type", $msg->getField("DOUBLE")->getType() == $libgmsec_perl::Field::Type_F64);
	$test->check("DOUBLE has wrong value", $msg->getF64Field("DOUBLE")->getValue() == 2.4);

	$test->check("BYTE not found", $msg->hasField("BYTE"));
	$test->check("BYTE has wrong type", $msg->getField("BYTE")->getType() == $libgmsec_perl::Field::Type_I8);
	$test->check("BYTE has wrong value", $msg->getI8Field("BYTE")->getValue() == 5);

	$test->check("SHORT not found", $msg->hasField("SHORT"));
	$test->check("SHORT has wrong type", $msg->getField("SHORT")->getType() == $libgmsec_perl::Field::Type_I16);
	$test->check("SHORT has wrong value", $msg->getI16Field("SHORT")->getValue() == 55);

	$test->check("INT not found", $msg->hasField("INT"));
	$test->check("INT has wrong type", $msg->getField("INT")->getType() == $libgmsec_perl::Field::Type_I32);
	$test->check("INT has wrong value", $msg->getI32Field("INT")->getValue() == 555);

	$test->check("LONG not found", $msg->hasField("LONG"));
	$test->check("LONG has wrong type", $msg->getField("LONG")->getType() == $libgmsec_perl::Field::Type_I64);
	$test->check("LONG has wrong value", $msg->getI64Field("LONG")->getValue() == 5555);

	$test->check("STRING not found", $msg->hasField("STRING"));
	$test->check("STRING has wrong type", $msg->getField("STRING")->getType() == $libgmsec_perl::Field::Type_STRING);
	$test->check("STRING has wrong value", $msg->getStringField("STRING")->getValue() == "The old fox");

	$test->check("UBYTE not found", $msg->hasField("UBYTE"));
	$test->check("UBYTE has wrong type", $msg->getField("UBYTE")->getType() == $libgmsec_perl::Field::Type_U8);
	$test->check("UBYTE has wrong value", $msg->getU8Field("UBYTE")->getValue() == 5);

	$test->check("USHORT not found", $msg->hasField("USHORT"));
	$test->check("USHORT has wrong type", $msg->getField("USHORT")->getType() == $libgmsec_perl::Field::Type_U16);
	$test->check("USHORT has wrong value", $msg->getU16Field("USHORT")->getValue() == 55);

	$test->check("UINT not found", $msg->hasField("UINT"));
	$test->check("UINT has wrong type", $msg->getField("UINT")->getType() == $libgmsec_perl::Field::Type_U32);
	$test->check("UINT has wrong value", $msg->getU32Field("UINT")->getValue() == 555);

	$test->check("ULONG not found", $msg->hasField("ULONG"));
	$test->check("ULONG has wrong type", $msg->getField("ULONG")->getType() == $libgmsec_perl::Field::Type_U64);
	$test->check("ULONG has wrong value", $msg->getU64Field("ULONG")->getValue() == 5555);
}


sub test_addFields
{
	libgmsec_perl::logInfo("Test addFields()");

	my ($test) = @_;

	my $field1 = libgmsec_perl::StringField->new("ONE", "uno");
	my $field2 = libgmsec_perl::StringField->new("TWO", "dos");
	my $field3 = libgmsec_perl::StringField->new("THREE", "tres");

	my $fields = libgmsec_perl::FieldArray->new();
	$fields->push($field1);
	$fields->push($field2);
	$fields->push($field3);

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("LOG");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$test->check("Expected only new fields to be added", $msg->addFields($fields) == 0);

	$test->check("Expected to have field ONE", 1 == $msg->hasField("ONE"));
	$test->check("Unexpected field type for ONE", $libgmsec_perl::Field::Type_STRING == $msg->getField("ONE")->getType());
	$test->check("Unexpected field value for ONE", $msg->getStringValue("ONE") == "uno");

	$test->check("Expected to have field TWO", 1 == $msg->hasField("TWO"));
	$test->check("Unexpected field type for TWO", $libgmsec_perl::Field::Type_STRING == $msg->getField("TWO")->getType());
	$test->check("Unexpected field value for TWO", $msg->getStringValue("TWO") == "dos");

	$test->check("Expected to have field THREE", 1 == $msg->hasField("THREE"));
	$test->check("Unexpected field type for THREE", $libgmsec_perl::Field::Type_STRING == $msg->getField("THREE")->getType());
	$test->check("Unexpected field value for THREE", $msg->getStringValue("THREE") == "tres");
}


sub test_clearFields
{
	libgmsec_perl::logInfo("Test clearFields()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$test->check("Message missing CONTENT-VERSION", 1 == $msg->hasField("CONTENT-VERSION"));
	$test->check("Message missing HEADER-VERSION", 1 == $msg->hasField("HEADER-VERSION"));
	$test->check("Message missing MESSAGE-SUBTYPE", 1 == $msg->hasField("MESSAGE-SUBTYPE"));
	$test->check("Message missing MESSAGE-TYPE", 1 == $msg->hasField("MESSAGE-TYPE"));
	$test->check("Message missing SPECIFICATION", 1 == $msg->hasField("SPECIFICATION"));

	$msg->clearFields();

	$test->check("Message has CONTENT-VERSION", 0 == $msg->hasField("CONTENT-VERSION"));
	$test->check("Message has HEADER-VERSION", 0 == $msg->hasField("HEADER-VERSION"));
	$test->check("Message has MESSAGE-SUBTYPE", 0 == $msg->hasField("MESSAGE-SUBTYPE"));
	$test->check("Message has MESSAGE-TYPE", 0 == $msg->hasField("MESSAGE-TYPE"));
	$test->check("Message has SPECIFICATION", 0 == $msg->hasField("SPECIFICATION"));
}


sub test_clearField
{
	libgmsec_perl::logInfo("Test clearField()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$test->check("CONTENT-VERSION not found in message", $msg->hasField("CONTENT-VERSION"));

	$test->check("CONTENT-VERSION not cleared from message", $msg->clearField("CONTENT-VERSION") == 1);
	$test->check("FOO-BAR does not exist and cannot be cleared", $msg->clearField("FOO-BAR") == 0);
	$test->check("foo-bar does not exist and cannot be cleared", $msg->clearField("foo-bar") == 0);

	$test->check("CONTENT-VERSION should not exist in message", $msg->hasField("CONTENT-VERSION") == 0);
}


sub test_getStringValue
{
	libgmsec_perl::logInfo("Test getStringValue()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->setFieldValue("PUB-RATE", "30");

	$test->check("MESSAGE-TYPE has unexpected value", $msg->getStringValue("MESSAGE-TYPE") == "MSG");
	$test->check("PUB-RATE has unexpected value", $msg->getStringValue("PUB-RATE") == "30");
}


sub test_getBooleanValue
{
	libgmsec_perl::logInfo("Test getBooleanValue()");

	my ($test) = @_;

	my $msg = libgmsec_perl::Message->new();

	$msg->addField( libgmsec_perl::I16Field->new("FIELD-1", 0) );
	$msg->addField( libgmsec_perl::I16Field->new("FIELD-2", 1) );
	$msg->addField( libgmsec_perl::StringField->new("FIELD-3", "10000") );
	$msg->addField( libgmsec_perl::I32Field->new("FIELD-4", 10000) );
	$msg->addField( libgmsec_perl::BooleanField->new("FIELD-5", 1) );
	$msg->addField( libgmsec_perl::BooleanField->new("FIELD-6", 0) );
	$msg->addField( libgmsec_perl::F64Field->new("FIELD-7", 2019.0) );
	$msg->addField( libgmsec_perl::StringField->new("FIELD-8", "1") );
	$msg->addField( libgmsec_perl::StringField->new("FIELD-9", "FALSE") );
	$msg->addField( libgmsec_perl::StringField->new("FIELD-0", "foo bar") );

	# Nominal tests
	$test->check("1 Expected boolean value of 0", $msg->getBooleanValue("FIELD-1") == 0);
	$test->check("2 Expected boolean value of 1", $msg->getBooleanValue("FIELD-2"));
	$test->check("3 Expected boolean value of 1", $msg->getBooleanValue("FIELD-3"));
	$test->check("4 Expected boolean value of 1", $msg->getBooleanValue("FIELD-4"));
	$test->check("5 Expected boolean value of 1", $msg->getBooleanValue("FIELD-5"));
	$test->check("6 Expected boolean value of 0", $msg->getBooleanValue("FIELD-6") == 0);
	$test->check("7 Expected boolean value of 1", $msg->getBooleanValue("FIELD-7"));
	$test->check("8 Expected boolean value of 1", $msg->getBooleanValue("FIELD-8"));
	$test->check("9 Expected boolean value of 0", $msg->getBooleanValue("FIELD-9") == 0);

	# Off-nominal tests
	# Field cannot be converted
	eval
	{
		$msg->getBooleanValue("FIELD-0");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field cannot be represented as a boolean") != -1);
	}

	# Unknown field
	eval
	{
		$msg->getBooleanValue("BOGUS");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain field with name: BOGUS") != -1);
	}

	# undef field name
	eval
	{
		$msg->getBooleanValue(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# empty-string field name
	eval
	{
		$msg->getBooleanValue("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getIntegerValue
{
	libgmsec_perl::logInfo("Test getIntegerValue()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->setFieldValue("PUB-RATE", "30");

	$test->check("PUB-RATE has unexpected value", $msg->getIntegerValue("PUB-RATE") == 30);

	# Off-nominal test
	eval
	{
		$msg->getIntegerValue("MESSAGE-TYPE");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field cannot be converted to a GMSEC_I64") != -1);
	}
}


sub test_getDoubleValue
{
	libgmsec_perl::logInfo("Test getDoubleValue()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->setFieldValue("PUB-RATE", "30");

	$test->check("PUB-RATE has unexpected value", $msg->getDoubleValue("PUB-RATE") == 30);

	# Off-nominal test
	eval
	{
		$msg->getDoubleValue("MESSAGE-TYPE");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field cannot be converted to a GMSEC_F64") != -1);
	}
}


sub test_hasField
{
	libgmsec_perl::logInfo("Test hasField()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->setFieldValue("PUB-RATE", "30");

	$test->check("Message should have PUB-RATE", $msg->hasField("PUB-RATE"));
	$test->check("Message should not have PUBLISH-RATE", $msg->hasField("PUBLISH-RATE") == 0);
	$test->check("Message should not have null field", $msg->hasField(undef) == 0);
	$test->check("Message should not have empty string field", $msg->hasField("") == 0);
}


sub test_getField
{
	libgmsec_perl::logInfo("Test getField()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	# Use valid field name
	$test->check("Message should have MESSAGE-TYPE", $msg->getField("MESSAGE-TYPE") != undef);

	# Use bogus field name
	$test->check("Message should have MSG-TYPE", $msg->getField("MSG-TYPE") == undef);

	# Use null field name
	eval
	{
		$msg->getField(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getField("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getFieldType
{
	libgmsec_perl::logInfo("Test getFieldType()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$test->check("MESSAGE-TYPE has unexpected field type", $msg->getFieldType("MESSAGE-TYPE") == $libgmsec_perl::Field::Type_STRING);
	$test->check("CONTENT-VERSION has unexpected field type", $msg->getFieldType("CONTENT-VERSION") == $libgmsec_perl::Field::Type_F32);

	# Use bogus field name
	eval
	{
		$msg->getFieldType("MSG-TYPE");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain field with name: MSG-TYPE") != -1);
	}

	# Use undef field name
	eval
	{
		$msg->getFieldType(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getFieldType("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getBinaryField
{
	libgmsec_perl::logInfo("Test getBinaryField()");

	my ($test) = @_;

	my $blob = "";
	for (my $i = 0; $i < 256; $i++)
	{
		$blob = $blob . $i;
	}

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("MSG.TLMPKT");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->addField( libgmsec_perl::BinaryField->new("DATA", $blob, length $blob) );

	# Use valid field name
	my $field = $msg->getBinaryField("DATA");
	$test->require("Expected to get binary field", $field != undef);
	$test->check("Unexpected field type", $field->getType() == $libgmsec_perl::Field::Type_BINARY);
	$test->check("Unexpected field name", $field->getName() eq "DATA");
	$test->check("Unexpected data length", $field->getLength() == length $blob);

	# Use non-binary field name
	eval
	{
		$msg->getBinaryField("CONTENT-VERSION");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain BinaryField with name: CONTENT-VERSION") != -1);
	}

	# Use null field name
	eval
	{
		$msg->getBinaryField(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getBinaryField("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getBooleanField
{
	libgmsec_perl::logInfo("Test getBooleanField()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("REQ.DIR");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->addField( libgmsec_perl::BooleanField->new("RESPONSE", 1) );

	# Use valid field name
	my $field = $msg->getBooleanField("RESPONSE");
	$test->check("Unexpected field type", $field->getType() == $libgmsec_perl::Field::Type_BOOL);
	$test->check("Unexpected field name", $field->getName() eq "RESPONSE");
	$test->check("Unexpected data value", $field->getValue() == 1);

	# Use non-boolean field name
	eval
	{
		$msg->getBooleanField("CONTENT-VERSION");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain BooleanField with name: CONTENT-VERSION") != -1);
	}

	# Use null field name
	eval
	{
		$msg->getBooleanField(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getBooleanField("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getCharField
{
	libgmsec_perl::logInfo("Test getCharField()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("REQ.DIR");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->addField( libgmsec_perl::CharField->new("CHAR-FIELD", 'c') );

	# Use valid field name
	my $field = $msg->getCharField("CHAR-FIELD");
	$test->check("Unexpected field type", $field->getType() == $libgmsec_perl::Field::Type_CHAR);
	$test->check("Unexpected field name", $field->getName() eq "CHAR-FIELD");
	$test->check("Unexpected data value", $field->getValue() == 'c');

	# Use non-char field name
	eval
	{
		$msg->getCharField("CONTENT-VERSION");
		$test->check("An exception was expected", 0)
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain CharField with name: CONTENT-VERSION") != -1);
	}

	# Use null field name
	eval
	{
		$msg->getCharField(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getCharField("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getF32Field
{
	libgmsec_perl::logInfo("Test getF32Field()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("REQ.DIR");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	# Use valid field name
	my $field = $msg->getF32Field("CONTENT-VERSION");
	$test->check("Unexpected field type", $field->getType() == $libgmsec_perl::Field::Type_F32);
	$test->check("Unexpected field name", $field->getName() eq "CONTENT-VERSION");

	# Use non-F32 field name
	eval
	{
		$msg->getF32Field("MESSAGE-TYPE");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain F32Field with name: MESSAGE-TYPE") != -1);
	}

	# Use null field name
	eval
	{
		$msg->getF32Field(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getF32Field("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getF64Field
{
	libgmsec_perl::logInfo("Test getF64Field()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("REQ.DIR");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->addField( libgmsec_perl::F64Field->new("F64-FIELD", 3.14) );

	# Use valid field name
	my $field = $msg->getF64Field("F64-FIELD");
	$test->check("Unexpected field type", $field->getType() == $libgmsec_perl::Field::Type_F64);
	$test->check("Unexpected field name", $field->getName() eq "F64-FIELD");
	$test->check("Unexpected field value", $field->getValue() == 3.14);

	# Use non-F64 field name
	eval
	{
		$msg->getF64Field("MESSAGE-TYPE");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain F64Field with name: MESSAGE-TYPE") != -1);
	}

	# Use null field name
	eval
	{
		$msg->getF64Field(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getF64Field("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getI8Field
{
	libgmsec_perl::logInfo("Test getI8Field()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("REQ.DIR");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->addField( libgmsec_perl::I8Field->new("I8-FIELD", 8) );

	# Use valid field name
	my $field = $msg->getI8Field("I8-FIELD");
	$test->check("Unexpected field type", $field->getType() == $libgmsec_perl::Field::Type_I8);
	$test->check("Unexpected field name", $field->getName() eq "I8-FIELD");
	$test->check("Unexpected field value", $field->getValue() == 8);

	# Use non-I8 field name
	eval
	{
		$msg->getI8Field("MESSAGE-TYPE");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain I8Field with name: MESSAGE-TYPE") != -1);
	}

	# Use null field name
	eval
	{
		$msg->getI8Field(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getI8Field("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getI16Field
{
	libgmsec_perl::logInfo("Test getI16Field()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("REQ.DIR");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->addField( libgmsec_perl::I16Field->new("I16-FIELD", 16) );

	# Use valid field name
	my $field = $msg->getI16Field("I16-FIELD");
	$test->check("Unexpected field type", $field->getType() == $libgmsec_perl::Field::Type_I16);
	$test->check("Unexpected field name", $field->getName() eq "I16-FIELD");
	$test->check("Unexpected field value", $field->getValue() == 16);

	# Use non-I16 field name
	eval
	{
		$msg->getI16Field("MESSAGE-TYPE");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain I16Field with name: MESSAGE-TYPE") != -1);
	}

	# Use null field name
	eval
	{
		$msg->getI16Field(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getI16Field("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getI32Field
{
	libgmsec_perl::logInfo("Test getI32Field()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("REQ.DIR");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->addField( libgmsec_perl::I32Field->new("I32-FIELD", 32) );

	# Use valid field name
	my $field = $msg->getI32Field("I32-FIELD");
	$test->check("Unexpected field type", $field->getType() == $libgmsec_perl::Field::Type_I32);
	$test->check("Unexpected field name", $field->getName() eq "I32-FIELD");
	$test->check("Unexpected field value", $field->getValue() == 32);

	# Use non-I32 field name
	eval
	{
		$msg->getI32Field("MESSAGE-TYPE");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain I32Field with name: MESSAGE-TYPE") != -1);
	}

	# Use null field name
	eval
	{
		$msg->getI32Field(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getI32Field("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getI64Field
{
	libgmsec_perl::logInfo("Test getI64Field()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("REQ.DIR");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->addField( libgmsec_perl::I64Field->new("I64-FIELD", 64) );

	# Use valid field name
	my $field = $msg->getI64Field("I64-FIELD");
	$test->check("Unexpected field type", $field->getType() == $libgmsec_perl::Field::Type_I64);
	$test->check("Unexpected field name", $field->getName() eq "I64-FIELD");
	$test->check("Unexpected field value", $field->getValue() == 64);

	# Use non-I64 field name
	eval
	{
		$msg->getI64Field("MESSAGE-TYPE");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain I64Field with name: MESSAGE-TYPE") != -1);
	}

	# Use null field name
	eval
	{
		$msg->getI64Field(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getI64Field("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getU8Field
{
	libgmsec_perl::logInfo("Test getU8Field()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("REQ.DIR");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->addField( libgmsec_perl::U8Field->new("U8-FIELD", 8) );

	# Use valid field name
	my $field = $msg->getU8Field("U8-FIELD");
	$test->check("Unexpected field type", $field->getType() == $libgmsec_perl::Field::Type_U8);
	$test->check("Unexpected field name", $field->getName() eq "U8-FIELD");
	$test->check("Unexpected field value", $field->getValue() == 8);

	# Use non-U8 field name
	eval
	{
		$msg->getU8Field("MESSAGE-TYPE");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain U8Field with name: MESSAGE-TYPE") != -1);
	}

	# Use null field name
	eval
	{
		$msg->getU8Field(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getU8Field("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getU16Field
{
	libgmsec_perl::logInfo("Test getU16Field()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("REQ.DIR");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->addField( libgmsec_perl::U16Field->new("U16-FIELD", 16) );

	# Use valid field name
	my $field = $msg->getU16Field("U16-FIELD");
	$test->check("Unexpected field type", $field->getType() == $libgmsec_perl::Field::Type_U16);
	$test->check("Unexpected field name", $field->getName() eq "U16-FIELD");
	$test->check("Unexpected field value", $field->getValue() == 16);

	# Use non-U16 field name
	eval
	{
		$msg->getU16Field("MESSAGE-TYPE");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain U16Field with name: MESSAGE-TYPE") != -1);
	}

	# Use null field name
	eval
	{
		$msg->getU16Field(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getU16Field("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getU32Field
{
	libgmsec_perl::logInfo("Test getU32Field()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("REQ.DIR");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->addField( libgmsec_perl::U32Field->new("U32-FIELD", 32) );

	# Use valid field name
	my $field = $msg->getU32Field("U32-FIELD");
	$test->check("Unexpected field type", $field->getType() == $libgmsec_perl::Field::Type_U32);
	$test->check("Unexpected field name", $field->getName() eq "U32-FIELD");
	$test->check("Unexpected field value", $field->getValue() == 32);

	# Use non-U32 field name
	eval
	{
		$msg->getU32Field("MESSAGE-TYPE");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain U32Field with name: MESSAGE-TYPE") != -1);
	}

	# Use null field name
	eval
	{
		$msg->getU32Field(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getU32Field("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getU64Field
{
	libgmsec_perl::logInfo("Test getU64Field()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("REQ.DIR");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg->addField( libgmsec_perl::U64Field->new("U64-FIELD", 64) );

	# Use valid field name
	my $field = $msg->getU64Field("U64-FIELD");
	$test->check("Unexpected field type", $field->getType() == $libgmsec_perl::Field::Type_U64);
	$test->check("Unexpected field name", $field->getName() eq "U64-FIELD");
	$test->check("Unexpected field value", $field->getValue() == 64);

	# Use non-U64 field name
	eval
	{
		$msg->getU64Field("MESSAGE-TYPE");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain U64Field with name: MESSAGE-TYPE") != -1);
	}

	# Use null field name
	eval
	{
		$msg->getU64Field(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getU64Field("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getStringField
{
	libgmsec_perl::logInfo("Test getStringField()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("REQ.DIR");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	# Use valid field name
	my $field = $msg->getStringField("MESSAGE-TYPE");
	$test->check("Unexpected field type", $field->getType() == $libgmsec_perl::Field::Type_STRING);
	$test->check("Unexpected field name", $field->getName() eq "MESSAGE-TYPE");
	$test->check("Unexpected field value", $field->getValue() eq "REQ");

	# Use non-string field name
	eval
	{
		$msg->getStringField("CONTENT-VERSION");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message does not contain StringField with name: CONTENT-VERSION") != -1);
	}

	# Use null field name
	eval
	{
		$msg->getStringField(undef);
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}

	# Use empty-string field name
	eval
	{
		$msg->getStringField("");
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name cannot be NULL, nor an empty string") != -1);
	}
}


sub test_getFieldCount
{
	libgmsec_perl::logInfo("Test getFieldCount()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	my $msg = $msgFactory->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$test->check("Message has unexpected field count", $msg->getFieldCount() == 13);

	$msg->setFieldValue("PUB-RATE", "30");

	$test->check("Message has unexpected field count", $msg->getFieldCount() == 14);

   	$msg->clearField("PUB-RATE");

	$test->check("Message has unexpected field count", $msg->getFieldCount() == 13);

	$msg->clearFields();

	$test->check("Message has unexpected field count", $msg->getFieldCount() == 0);
}


sub test_copyFields
{
	libgmsec_perl::logInfo("Test copyFields()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg1 = $msgFactory->createMessage("HB");
	my $msg2 = libgmsec_perl::Message->new($msg1);

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$msg1->setFieldValue("PUB-RATE", "30");

	$test->check("Message 2 should not have PUB-RATE", $msg2->hasField("PUB-RATE") == 0);

	$msg1->copyFields($msg2);

	$test->check("Message 2 should have PUB-RATE", $msg2->hasField("PUB-RATE"));
}


sub test_toXML
{
	libgmsec_perl::logInfo("Test toXML()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	my $msg1 = $msgFactory->createMessage("HB");
	my $msg2 = libgmsec_perl::Message->new($msg1);

	$test->check("Message 1 has NULL XML content", not $msg1->toXML() eq "");
	$test->check("Message 2 has NULL XML content", not $msg2->toXML() eq "");
	$test->check("Messages 1 and 2 do not have same XML content", $msg1->toXML() eq $msg2->toXML());

	my $msg3 = $msgFactory->fromData($msg1->toXML(), $libgmsec_perl::DataType_XML_DATA);
	$test->check("Messages 1 and 3 do not have same XML content", $msg1->toXML() eq $msg3->toXML());

	libgmsec_perl::MessageFactory::destroy($msgFactory);
}


sub test_toJSON
{
	libgmsec_perl::logInfo("Test toJSON()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	$test->setStandardFields($msgFactory);

	my $msg1 = $msgFactory->createMessage("HB");
	my $msg2 = libgmsec_perl::Message->new($msg1);

	$test->check("Message 1 has NULL JSON content", not $msg1->toJSON() eq "");
	$test->check("Message 2 has NULL JSON content", not $msg2->toJSON() eq "");
	$test->check("Messages 1 and 2 do not have same JSON content", $msg1->toJSON() eq $msg2->toJSON());

	my $msg3 = $msgFactory->fromData($msg1->toJSON(), $libgmsec_perl::DataType_JSON_DATA);
	$test->check("Messages 1 and 3 do not have same JSON content", $msg1->toJSON() eq $msg3->toJSON());

	libgmsec_perl::MessageFactory::destroy($msgFactory);
}


sub test_getSize
{
	libgmsec_perl::logInfo("Test getSize()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg1 = $msgFactory->createMessage("HB");
	my $msg2 = libgmsec_perl::Message->new($msg1);

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	$test->check("Message 1 has unexpected size", $msg1->getSize() > 0);
	$test->check("Message 2 has unexpected size", $msg2->getSize() > 0);
	$test->check("Messages should have the same size", $msg1->getSize() == $msg2->getSize());
}


sub test_getFieldIterator
{
	libgmsec_perl::logInfo("Test getFieldIterator()");

	my ($test) = @_;

	my $msgFactory = libgmsec_perl::MessageFactory::create();

	my $msg = $msgFactory->createMessage("HB");

	libgmsec_perl::MessageFactory::destroy($msgFactory);

	my $iter = $msg->getFieldIterator();
	$test->check("Got the field iterator", 1)
}


# T005_MessageValidator
{
	package T005_MessageValidator;
	use base 'libgmsec_perl::MessageValidator';

	sub new
	{
		my $class = shift;
		my $self = $class->SUPER::new();
		$self->{ITER} = 0;
		return $self;
	}

	sub validateMessage
	{
		my ($self, $msg) = @_;

		my $status = libgmsec_perl::Status->new();

		if ($self->{ITER} > 0)
		{
			$status = libgmsec_perl::Status->new($libgmsec_perl::MSG_ERROR, $libgmsec_perl::MESSAGE_FAILED_VALIDATION, "Field is missing");
		}

        $self->{ITER} += 1;

		return $status;
	}

	sub reset
	{
		my ($self) = @_;

        $self->{ITER} = 0;
	}
}


Test::driver(\&Test_Message);
