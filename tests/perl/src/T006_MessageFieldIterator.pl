#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


sub Test_MessageFieldIterator
{
	my ($test) = @_;

	$test->setDescription("MessageFieldIterator");

	test_all_fields($test);
	test_header_fields($test);
	test_non_header_fields($test);
	test_reset($test);
}


sub test_all_fields
{
	libgmsec_perl::logInfo("Test ALL_FIELDS");

	my ($test) = @_;

	my $msg = libgmsec_perl::Message->new();
    my $field1 = libgmsec_perl::StringField->new("FIELD-1", "1", 1);
    my $field2 = libgmsec_perl::StringField->new("FIELD-2", "2", 1);
    my $field3 = libgmsec_perl::I32Field->new("FIELD-3", 3);

	$msg->addField( $field1 );
	$msg->addField( $field2 );
	$msg->addField( $field3 );

	my $iter = $msg->getFieldIterator( $libgmsec_perl::MessageFieldIterator::Selector_ALL_FIELDS );

	my $fieldsFound = 0;

	while ($iter->hasNext())
	{
		my $field = $iter->next();

		$fieldsFound += 1;

		if ($fieldsFound == 1)
		{
			$test->check("Unexpected field type 1", $field->getType() == $libgmsec_perl::Field::Type_STRING);
			$test->check("Unexpected field name 1", $field->getName() eq "FIELD-1");
			$test->check("Unexpected field value 1", $field->getStringValue() eq "1");
		}
		elsif ($fieldsFound == 2)
		{
			$test->check("Unexpected field type 2", $field->getType() == $libgmsec_perl::Field::Type_STRING);
			$test->check("Unexpected field name 2", $field->getName() eq "FIELD-2");
			$test->check("Unexpected field value 2", $field->getStringValue() eq "2");
		}
		else
		{
			$test->check("Unexpected field type 3", $field->getType() == $libgmsec_perl::Field::Type_I32);
			$test->check("Unexpected field name 3", $field->getName() eq "FIELD-3");
			$test->check("Unexpected field value 3", $field->getIntegerValue() == 3);
		}
	}

	$test->check("Unexpected field count", $fieldsFound == 3);
}


sub test_header_fields
{
	libgmsec_perl::logInfo("Test HEADER_FIELDS");

	my ($test) = @_;

	my $msg = libgmsec_perl::Message->new();
    my $field1 = libgmsec_perl::StringField->new("FIELD-1", "1", 1);
    my $field2 = libgmsec_perl::StringField->new("FIELD-2", "2", 1);
    my $field3 = libgmsec_perl::I32Field->new("FIELD-3", 3);

	$msg->addField( $field1 );
	$msg->addField( $field2 );
	$msg->addField( $field3 );

	my $iter = $msg->getFieldIterator( $libgmsec_perl::MessageFieldIterator::Selector_HEADER_FIELDS );

	my $fieldsFound = 0;

	while ($iter->hasNext())
	{
		my $field = $iter->next();

		$fieldsFound += 1;

		if ($fieldsFound == 1)
		{
			$test->check("Unexpected field type 1", $field->getType() == $libgmsec_perl::Field::Type_STRING);
			$test->check("Unexpected field name 1", $field->getName() eq "FIELD-1");
			$test->check("Unexpected field value 1", $field->getStringValue() eq "1");
		}
		elsif ($fieldsFound == 2)
		{
			$test->check("Unexpected field type 2", $field->getType() == $libgmsec_perl::Field::Type_STRING);
			$test->check("Unexpected field name 2", $field->getName() eq "FIELD-2");
			$test->check("Unexpected field value 2", $field->getStringValue() eq "2");
		}
		else
		{
            libgmsec_perl::logInfo("Unexpected field: " . $field->getName());
			$test->check("Unexpected field", 0);
		}
	}

	$test->check("Unexpected field count", $fieldsFound == 2);
}


sub test_non_header_fields
{
	libgmsec_perl::logInfo("Test NON_HEADER_FIELDS");

	my ($test) = @_;

	my $msg = libgmsec_perl::Message->new();
    my $field1 = libgmsec_perl::StringField->new("FIELD-1", "1", 1);
    my $field2 = libgmsec_perl::StringField->new("FIELD-2", "2", 1);
    my $field3 = libgmsec_perl::I32Field->new("FIELD-3", 3);

	$msg->addField( $field1 );
	$msg->addField( $field2 );
	$msg->addField( $field3 );

	my $iter = $msg->getFieldIterator( $libgmsec_perl::MessageFieldIterator::Selector_NON_HEADER_FIELDS );

	my $fieldsFound = 0;

	while ($iter->hasNext())
	{
		my $field = $iter->next();

		$fieldsFound += 1;

		if ($fieldsFound == 1)
		{
			$test->check("Unexpected field type 3", $field->getType() == $libgmsec_perl::Field::Type_I32);
			$test->check("Unexpected field name 3", $field->getName() eq "FIELD-3");
			$test->check("Unexpected field value 3", $field->getIntegerValue() == 3);
		}
		else
		{
            libgmsec_perl::logInfo("Unexpected field: " . $field->getName());
			$test->check("Unexpected field", 0);
		}
	}

	$test->check("Unexpected field count", $fieldsFound == 1);
}


sub test_reset
{
	libgmsec_perl::logInfo("Test reset()");

	my ($test) = @_;

	my $msg = libgmsec_perl::Message->new();
	$msg->addField( libgmsec_perl::StringField->new("FIELD-1", "1") );
	$msg->addField( libgmsec_perl::StringField->new("FIELD-2", "2") );
	$msg->addField( libgmsec_perl::I32Field->new("FIELD-3", 3) );

	my $iter = $msg->getFieldIterator( $libgmsec_perl::MessageFieldIterator::Selector_ALL_FIELDS );

	my $fieldsFound = 0;

	while ($iter->hasNext())
	{
		$iter->next();
		$fieldsFound += 1;
	}

	$test->check("Unexpected field count", $fieldsFound == 3);

	$test->check("Should no more field(s) to iterate over", $iter->hasNext() == 0);

	$iter->reset();

	$test->check("Should have field(s) to iterator over", $iter->hasNext());

	$fieldsFound = 0;

	while ($iter->hasNext())
	{
		$iter->next();
		$fieldsFound += 1;
	}

	$test->check("Unexpected field count", $fieldsFound == 3);
}


Test::driver(\&Test_MessageFieldIterator);
