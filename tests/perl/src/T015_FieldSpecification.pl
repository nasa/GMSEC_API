#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


sub Test_FieldSpecification
{
	my ($test) = @_;

	$test->setDescription("Test FieldSpecification");

	test_field_specification($test);
}


sub test_field_specification
{
	libgmsec_perl::logInfo("Test FieldSpecification");

	my ($test) = @_;

	test_field_specification_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT, $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);
	test_field_specification_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT, $libgmsec_perl::Specification::SchemaLevel_LEVEL_1);
	test_field_specification_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT, $libgmsec_perl::Specification::SchemaLevel_LEVEL_2);

	test_field_specification_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_2019_00, $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);
	test_field_specification_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_2019_00, $libgmsec_perl::Specification::SchemaLevel_LEVEL_1);
	test_field_specification_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_2019_00, $libgmsec_perl::Specification::SchemaLevel_LEVEL_2);
}


sub test_field_specification_aux
{
	my ($test, $specVersion, $schemaLevel) = @_;

	my $config = libgmsec_perl::Config->new();
	$config->addValue("gmsec-specification-version", "$specVersion");
	$config->addValue("gmsec-schema-level", "$schemaLevel");

	my $spec = libgmsec_perl::Specification->new($config);

	my $msgSpecs = $spec->getMessageSpecifications();

	for (my $i = 0; $i < $msgSpecs->size(); $i++)
	{
		my $msgSpec = $msgSpecs->get($i);

		my $fieldSpecs = $msgSpec->getFieldSpecifications();

		$test->check("Expected to have at least 1 (one) field specification", $fieldSpecs->size() > 0);

		for (my $j = 0; $j < $fieldSpecs->size(); $j++)
		{
			my $fieldSpec = $fieldSpecs->get($j);

			$test->check("FieldSpecification name is empty", defined $fieldSpec->getName());
			$test->check("FieldSpecification type is empty", defined $fieldSpec->getType());
			$test->check("FieldSpecification mode is empty", defined $fieldSpec->getMode());
			$test->check("FieldSpecification classification is empty", defined $fieldSpec->getClassification());
			$test->check("FieldSpecification value is empty", defined $fieldSpec->getValue());
			$test->check("FieldSpecification description is empty", defined $fieldSpec->getDescription());
		}
	}
}


Test::driver(\&Test_FieldSpecification);
