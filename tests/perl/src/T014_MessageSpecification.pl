#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


sub Test_MessageSpecification
{
	my ($test) = @_;

	$test->setDescription("Test MessageSpecification");

	test_message_specification($test);
}


sub test_message_specification
{
	libgmsec_perl::logInfo("Test MessageSpecification");

	my ($test) = @_;

	test_message_specification_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT, $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);
	test_message_specification_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT, $libgmsec_perl::Specification::SchemaLevel_LEVEL_1);
	test_message_specification_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT, $libgmsec_perl::Specification::SchemaLevel_LEVEL_2);

	test_message_specification_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_2019_00, $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);
	test_message_specification_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_2019_00, $libgmsec_perl::Specification::SchemaLevel_LEVEL_1);
	test_message_specification_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_2019_00, $libgmsec_perl::Specification::SchemaLevel_LEVEL_2);
}


sub test_message_specification_aux
{
	my ($test, $specVersion, $schemaLevel) = @_;

	my $config = libgmsec_perl::Config->new();
	$config->addValue("gmsec-specification-version", "$specVersion");
	$config->addValue("gmsec-schema-level", "$schemaLevel");

	my $spec = libgmsec_perl::Specification->new($config);

	my $msgSpecs = $spec->getMessageSpecifications();

	$test->check("Expected at least 1 (one) message specification", $msgSpecs->size() > 0);

	for (my $i = 0; $i < $msgSpecs->size(); $i++)
	{
		my $msgSpec = $msgSpecs->get($i);

		$test->check("Schema ID is NULL", $msgSpec->getSchemaID() ne undef);
		$test->check("Subject Template is NULL", $msgSpec->getSubjectTemplate() ne undef);

		my $fieldSpecs = $msgSpec->getFieldSpecifications();

		$test->check("Expected at least 1 (one) field specification", $fieldSpecs->size() > 0);
	}
}


Test::driver(\&Test_MessageSpecification);
