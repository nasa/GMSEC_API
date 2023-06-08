#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


sub Test_SchemaIDIterator
{
	my ($test) = @_;

	$test->setDescription("Test SchemaIDIterator");

	test_iterator($test);
	test_reset($test);
}


sub test_iterator
{
	libgmsec_perl::logInfo("Test hasNext() and next()");

	my ($test) = @_;

	test_schema_id_iterator_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT, $libgmsec_perl::Specification::SchemaLevel_LEVEL_0, 73);
	test_schema_id_iterator_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT, $libgmsec_perl::Specification::SchemaLevel_LEVEL_1, 103);
	test_schema_id_iterator_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT, $libgmsec_perl::Specification::SchemaLevel_LEVEL_2, 106);

	test_schema_id_iterator_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_2019_00, $libgmsec_perl::Specification::SchemaLevel_LEVEL_0, 73);
	test_schema_id_iterator_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_2019_00, $libgmsec_perl::Specification::SchemaLevel_LEVEL_1, 103);
	test_schema_id_iterator_aux($test, $libgmsec_perl::GMSEC_MSG_SPEC_2019_00, $libgmsec_perl::Specification::SchemaLevel_LEVEL_2, 106);
}


sub test_reset
{
	libgmsec_perl::logInfo("Test reset()");

	my ($test) = @_;

	my $spec = libgmsec_perl::Specification->new();

	my $iter = $spec->getSchemaIDIterator();

	while ($iter->hasNext())
	{
		$iter->next();
	}

	$test->check("Expected no more schema IDs", $iter->hasNext() == 0);

	$iter->reset();

	$test->check("Expected to have schema IDs", $iter->hasNext());
}


sub test_schema_id_iterator_aux
{
	my ($test, $specVersion, $schemaLevel, $expectedSchemaIds) = @_;

	my $config = libgmsec_perl::Config->new();
	$config->addValue("gmsec-specification-version", "$specVersion");
	$config->addValue("gmsec-schema-level", "$schemaLevel");

	my $spec = libgmsec_perl::Specification->new($config);

	my $iter = $spec->getSchemaIDIterator();

	my $numSchemaIds = 0;

	while ($iter->hasNext())
	{
		my $id = $iter->next();

		$test->check("Schema ID is null", $id != undef);

		$numSchemaIds += 1;
	}

	$test->check("Unexpected number of schema IDs", $expectedSchemaIds == $numSchemaIds);
}


Test::driver(\&Test_SchemaIDIterator);
