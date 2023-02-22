#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


sub Test_Specification
{
	my ($test) = @_;

	$test->setDescription("Test Specification");

	test_constructor_1($test);
	test_constructor_2($test);
	test_copy_constructor($test);
	test_getSchemaIDIterator($test);
	test_getMessageSpecifications($test);
	test_getHeaderFieldNames($test);
}


sub test_constructor_1
{
	libgmsec_perl::logInfo("Test constructor (no args)");

	my ($test) = @_;

	my $spec = libgmsec_perl::Specification->new();
	$test->check("Unexpected specification version", $spec->getVersion() == $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT);
	$test->check("Unexpected specification schema level", $spec->getSchemaLevel() == $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);
}


sub test_constructor_2
{
	libgmsec_perl::logInfo("Test constructor w/ Config");

	my ($test) = @_;

	# Nominal tests
	my $spec = libgmsec_perl::Specification->new( libgmsec_perl::Config->new() );
	$test->check("Unexpected specification version", $spec->getVersion() == $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT);
	$test->check("Unexpected specification schema level", $spec->getSchemaLevel() == $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);

	my $spec0 = libgmsec_perl::Specification->new( libgmsec_perl::Config->new("gmsec-specification-version=201900 gmsec-schema-level=0", $libgmsec_perl::DataType_KEY_VALUE_DATA) );
	my $spec1 = libgmsec_perl::Specification->new( libgmsec_perl::Config->new("gmsec-specification-version=201900 gmsec-schema-level=1", $libgmsec_perl::DataType_KEY_VALUE_DATA) );
	my $spec2 = libgmsec_perl::Specification->new( libgmsec_perl::Config->new("gmsec-specification-version=201900 gmsec-schema-level=2", $libgmsec_perl::DataType_KEY_VALUE_DATA) );

	$test->check("Unexpected specification version", $libgmsec_perl::GMSEC_MSG_SPEC_2019_00 == $spec0->getVersion());
	$test->check("Unexpected specification schema level", $libgmsec_perl::Specification::SchemaLevel_LEVEL_0 == $spec0->getSchemaLevel());

	$test->check("Unexpected specification version", $libgmsec_perl::GMSEC_MSG_SPEC_2019_00 == $spec1->getVersion());
	$test->check("Unexpected specification schema level", $libgmsec_perl::Specification::SchemaLevel_LEVEL_1 == $spec1->getSchemaLevel());

	$test->check("Unexpected specification version", $libgmsec_perl::GMSEC_MSG_SPEC_2019_00 == $spec2->getVersion());
	$test->check("Unexpected specification schema level", $libgmsec_perl::Specification::SchemaLevel_LEVEL_2 == $spec2->getSchemaLevel());

	$spec = libgmsec_perl::Specification->new( libgmsec_perl::Config->new("name=value", $libgmsec_perl::DataType_KEY_VALUE_DATA) );
	$test->check("Unexpected specification version", $spec->getVersion() == $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT);
	$test->check("Unexpected specification schema level", $spec->getSchemaLevel() == $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);

	# Off-nominal tests
	eval
	{
		$spec = libgmsec_perl::Specification->new( libgmsec_perl::Config->new("gmsec-specification-version=201300", $libgmsec_perl::DataType_KEY_VALUE_DATA) );
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Could not list files in template directory") != -1);
	}

	eval
	{
		$spec = libgmsec_perl::Specification->new( libgmsec_perl::Config->new("gmsec-specification-version=ABC", $libgmsec_perl::DataType_KEY_VALUE_DATA) );
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "\"GMSEC-SPECIFICATION-VERSION\" contains invalid value") != -1);
	}

	eval
	{
		$spec = libgmsec_perl::Specification->new( libgmsec_perl::Config->new("gmsec-schema-level=3", $libgmsec_perl::DataType_KEY_VALUE_DATA) );
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "DIRECTORY is missing definition for LEVEL-3 HEADER") != -1);
	}

	eval
	{
		$spec = libgmsec_perl::Specification->new( libgmsec_perl::Config->new("gmsec-schema-level=10", $libgmsec_perl::DataType_KEY_VALUE_DATA) );
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "\"GMSEC-SCHEMA-LEVEL\" contains out-of-range integer") != -1);
	}

	eval
	{
		$spec = libgmsec_perl::Specification->new( libgmsec_perl::Config->new("gmsec-schema-level=ABC", $libgmsec_perl::DataType_KEY_VALUE_DATA) );
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "\"GMSEC-SCHEMA-LEVEL\" contains invalid value") != -1);
	}

	eval
	{
		$spec = libgmsec_perl::Specification->new( libgmsec_perl::Config->new("gmsec-schema-path=here", $libgmsec_perl::DataType_KEY_VALUE_DATA) );
		$test->check("An exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Could not list files in template directory") != -1);
	}
}


sub test_copy_constructor
{
	libgmsec_perl::logInfo("Test copy-constructor");

	my ($test) = @_;

	my $spec1 = libgmsec_perl::Specification->new();
	my $spec2 = libgmsec_perl::Specification->new($spec1);

	$test->check("Unexpected specification version", $spec1->getVersion() == $spec2->getVersion());
	$test->check("Unexpected specification schema level", $spec1->getSchemaLevel() == $spec2->getSchemaLevel());
	$test->check("Unexpected message specifications", $spec1->getMessageSpecifications()->size() == $spec2->getMessageSpecifications()->size());
}


sub test_getSchemaIDIterator
{
	libgmsec_perl::logInfo("Test getSchemaIDIterator()");

	my ($test) = @_;

	my $spec = libgmsec_perl::Specification->new();

	my $iter = $spec->getSchemaIDIterator();

	$test->check("Expected availability of schema IDs", $iter->hasNext());
}


sub test_getMessageSpecifications
{
	libgmsec_perl::logInfo("Test getMessageSpecifications()");

	my ($test) = @_;

	my $spec = libgmsec_perl::Specification->new();

	my $msgSpecs = $spec->getMessageSpecifications();

	$test->check("Expected to have message specifications", $msgSpecs->size() > 0);
}


sub test_getHeaderFieldNames
{
	libgmsec_perl::logInfo("Test getHeaders()");

	my ($test) = @_;

	my $spec = libgmsec_perl::Specification->new();

	my $headers = $spec->getHeaderFieldNames();
	$test->check("Expected to have headers", $headers->size() == 26);

	my $headers = $spec->getHeaderFieldNames("");
	$test->check("Expected to have headers", $headers->size() == 26);

	my $defaultHeaders = $spec->getHeaderFieldNames("DEFAULT");
	$test->check("Expected to have headers", $defaultHeaders->size() == 26);

	my $C2MSHeaders = $spec->getHeaderFieldNames("C2MS");
	$test->check("Expected to have message headers", $C2MSHeaders->size() == 26);
}


Test::driver(\&Test_Specification);
