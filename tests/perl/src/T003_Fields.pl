#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


sub Test_Fields
{
	my ($test) = @_;

	$test->setDescription("Test Fields");

	test_BinaryField($test);
	test_BooleanField($test);
	test_CharField($test);
	test_StringField($test);

	test_F32Field($test);
	test_F64Field($test);

	test_I8Field($test);
	test_I16Field($test);
	test_I32Field($test);
	test_I64Field($test);

	test_U8Field($test);
	test_U16Field($test);
	test_U32Field($test);
	test_U64Field($test);

	test_Field($test);
}


sub test_BinaryField
{
	libgmsec_perl::logInfo("Test BinaryField");

	my ($test) = @_;

	# Nominal test
	my $name  = "MY-FIELD";
	my $value = "";
	for (my $i = 0; $i < 4; $i++)
	{
		$value = $value . $i;
	}

	# Constructor
	my $field1 = libgmsec_perl::BinaryField->new($name, $value, length $value);
	$test->check("Unexpected field type", $field1->getType() == $libgmsec_perl::Field::Type_BINARY);
	$test->check("Unexpected field name", $field1->getName() eq $name);
	$test->check("Unexpected field value", $field1->getLength() == length $value);
	$test->check("Unexpected XML", $field1->toXML() eq "<FIELD NAME=\"MY-FIELD\" TYPE=\"BIN\">30313233</FIELD>");
	$test->check("Unexpected JSON", $field1->toJSON() eq "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"BIN\",\"VALUE\":\"30313233\"}");

	my $blob = $field1->getValue();
	for (my $i = 0; $i < $field1->getLength(); $i++)
	{
		if (! substr($blob, $i, 1) eq substr($value, $i, 1))
		{
			libgmsec_perl::logDebug("BinaryField value at position $i is wrong");
			$test->check("BinaryField value is wrong", 0);
		}
	}

	# Copy constructor
	my $field2 = libgmsec_perl::BinaryField->new($field1);
	$test->check("Unexpected field type", $field2->getType() == $field1->getType());
	$test->check("Unexpected field name", $field2->getName() eq $field1->getName());
	$test->check("Unexpected field value", $field2->getLength() == $field1->getLength());
	$test->check("Unexpected XML", $field2->toXML() eq $field1->toXML());
	$test->check("Unexpected JSON", $field2->toJSON() eq $field1->toJSON());

	$blob = $field2->getValue();
	for (my $i = 0; $i < $field2->getLength(); $i++)
	{
		if (! substr($blob, $i, 1) eq substr($value, $i, 1))
		{
			libgmsec_perl::logDebug("BinaryField value at position $i is wrong");
			$test->check("BinaryField value is wrong", 0);
		}
	}

	# Off-Nominal test
	eval
	{
		# undef field name
		libgmsec_perl::BinaryField->new(undef, $value, length $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}

	eval
	{
		# empty-string field name
		libgmsec_perl::BinaryField->new("", $value, length $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}
}


sub test_BooleanField
{
	libgmsec_perl::logInfo("Test BooleanField");

	my ($test) = @_;

	# Nominal test
	my $name  = "MY-FIELD";
	my $value = 1;

	# Constructor
	my $field1 = libgmsec_perl::BooleanField->new($name, $value);
	$test->check("Unexpected field type", $field1->getType() == $libgmsec_perl::Field::Type_BOOL);
	$test->check("Unexpected field name", $field1->getName() eq $name);
	$test->check("Unexpected field value", $field1->getValue() == $value);
	$test->check("Unexpected XML", $field1->toXML() eq "<FIELD NAME=\"MY-FIELD\" TYPE=\"BOOL\">TRUE</FIELD>");
	$test->check("Unexpected JSON", $field1->toJSON() eq "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"BOOL\",\"VALUE\":\"TRUE\"}");

	# Copy-constructor
	my $field2 = libgmsec_perl::BooleanField->new($field1);
	$test->check("Unexpected field type", $field2->getType() == $field1->getType());
	$test->check("Unexpected field name", $field2->getName() eq $field1->getName());
	$test->check("Unexpected field value", $field2->getValue() == $field1->getValue());
	$test->check("Unexpected XML", $field2->toXML() eq $field1->toXML());
	$test->check("Unexpected JSON", $field2->toJSON() eq $field1->toJSON());

	# Off-Nominal test
	eval
	{
		# undef field name
		libgmsec_perl::BooleanField->new(undef, $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}

	eval
	{
		# empty-string field name
		libgmsec_perl::BooleanField->new("", $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}
}


sub test_CharField
{
	libgmsec_perl::logInfo("Test CharField");

	my ($test) = @_;

	# Nominal test
	my $name  = "MY-FIELD";
	my $value = 'f';

	# Constructor
	my $field1 = libgmsec_perl::CharField->new($name, $value);
	$test->check("Unexpected field type", $field1->getType() == $libgmsec_perl::Field::Type_CHAR);
	$test->check("Unexpected field name", $field1->getName() eq $name);
	$test->check("Unexpected field value", $field1->getValue() == $value);
	$test->check("Unexpected XML", $field1->toXML() eq "<FIELD NAME=\"MY-FIELD\" TYPE=\"CHAR\">f</FIELD>");
	$test->check("Unexpected JSON", $field1->toJSON() eq "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"CHAR\",\"VALUE\":\"f\"}");

	# Copy-constructor
	my $field2 = libgmsec_perl::CharField->new($field1);
	$test->check("Unexpected field type", $field2->getType() == $field1->getType());
	$test->check("Unexpected field name", $field2->getName() eq $field1->getName());
	$test->check("Unexpected field value", $field2->getValue() == $field1->getValue());
	$test->check("Unexpected XML", $field2->toXML() eq $field1->toXML());
	$test->check("Unexpected JSON", $field2->toJSON() eq $field1->toJSON());

	# Off-Nominal test
	eval
	{
		# undef field name
		libgmsec_perl::CharField(undef, $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}

	eval
	{
		# empty-string field name
		libgmsec_perl::CharField("", $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}
}


sub test_StringField
{
	libgmsec_perl::logInfo("Test StringField");

	my ($test) = @_;

	# Nominal test
	my $name  = "MY-FIELD";
	my $value = "Test String";

	# Constructor
	my $field1 = libgmsec_perl::StringField->new($name, $value);
	$test->check("Unexpected field type", $field1->getType() == $libgmsec_perl::Field::Type_STRING);
	$test->check("Unexpected field name", $field1->getName() eq $name);
	$test->check("Unexpected field value", $field1->getValue() eq $value);
	$test->check("Unexpected XML", $field1->toXML() eq "<FIELD NAME=\"MY-FIELD\" TYPE=\"STRING\">Test String</FIELD>");
	$test->check("Unexpected JSON", $field1->toJSON() eq "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"STRING\",\"VALUE\":\"Test String\"}");

	# Copy-constructor
	my $field2 = libgmsec_perl::StringField->new($field1);
	$test->check("Unexpected field type", $field2->getType() == $field1->getType());
	$test->check("Unexpected field name", $field2->getName() eq $field1->getName());
	$test->check("Unexpected field value", $field2->getValue() eq $field1->getValue());
	$test->check("Unexpected XML", $field2->toXML() eq $field1->toXML());
	$test->check("Unexpected JSON", $field2->toJSON() eq $field1->toJSON());

	# Off-Nominal test
	eval
	{
		# undef field value
		libgmsec_perl::StringField->new($name, undef);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "StringField value cannot be NULL") != -1);
	}

	eval
	{
		# undef field name
		libgmsec_perl::StringField->new(undef, $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}

	eval
	{
		# empty-string field name
		libgmsec_perl::StringField->new("", $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}
}


sub test_F32Field
{
	libgmsec_perl::logInfo("Test F32Field");

	my ($test) = @_;

	# Nominal test
	my $name  = "MY-FIELD";
	my $value = 10;

	# Constructor
	my $field1 = libgmsec_perl::F32Field->new($name, $value);
	$test->check("Unexpected field type", $field1->getType() == $libgmsec_perl::Field::Type_F32);
	$test->check("Unexpected field name", $field1->getName() eq $name);
	$test->check("Unexpected field value", $field1->getValue() == $value);
	$test->check("Unexpected XML", $field1->toXML() eq "<FIELD NAME=\"MY-FIELD\" TYPE=\"F32\" BITS=\"41200000\">10</FIELD>");
	$test->check("Unexpected JSON", $field1->toJSON() eq "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"F32\",\"BITS\":\"41200000\",\"VALUE\":\"10\"}");

	# Copy-constructor
	my $field2 = libgmsec_perl::F32Field->new($field1);
	$test->check("Unexpected field type", $field2->getType() == $field1->getType());
	$test->check("Unexpected field name", $field2->getName() eq $field1->getName());
	$test->check("Unexpected field value", $field2->getValue() == $field1->getValue());
	$test->check("Unexpected XML", $field2->toXML() eq $field1->toXML());
	$test->check("Unexpected JSON", $field2->toJSON() eq $field1->toJSON());

	# Off-Nominal test
	eval
	{
		# undef field name
		libgmsec_perl::F32Field->new(undef, $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}

	eval
	{
		# empty-string field name
		libgmsec_perl::F32Field->new("", $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}
}


sub test_F64Field
{
	libgmsec_perl::logInfo("Test F64Field");

	my ($test) = @_;

	# Nominal test
	my $name  = "MY-FIELD";
	my $value = 10;

	# Constructor
	my $field1 = libgmsec_perl::F64Field->new($name, $value);
	$test->check("Unexpected field type", $field1->getType() == $libgmsec_perl::Field::Type_F64);
	$test->check("Unexpected field name", $field1->getName() eq $name);
	$test->check("Unexpected field value", $field1->getValue() == $value);
	$test->check("Unexpected XML", $field1->toXML() eq "<FIELD NAME=\"MY-FIELD\" TYPE=\"F64\" BITS=\"4024000000000000\">10</FIELD>");
	$test->check("Unexpected JSON", $field1->toJSON() eq "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"F64\",\"BITS\":\"4024000000000000\",\"VALUE\":\"10\"}");

	# Copy-constructor
	my $field2 = libgmsec_perl::F64Field->new($field1);
	$test->check("Unexpected field type", $field2->getType() == $field1->getType());
	$test->check("Unexpected field name", $field2->getName() eq $field1->getName());
	$test->check("Unexpected field value", $field2->getValue() == $field1->getValue());
	$test->check("Unexpected XML", $field2->toXML() eq $field1->toXML());
	$test->check("Unexpected JSON", $field2->toJSON() eq $field1->toJSON());

	# Off-Nominal test
	eval
	{
		# undef field name
		libgmsec_perl::F64Field->new(undef, $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}

	eval
	{
		# empty-string field name
		libgmsec_perl::F64Field->new("", $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}
}


sub test_I8Field
{
	libgmsec_perl::logInfo("Test I8Field");

	my ($test) = @_;

	# Nominal test
	my $name  = "MY-FIELD";
	my $value = 10;

	# Constructor
	my $field1 = libgmsec_perl::I8Field->new($name, $value);
	$test->check("Unexpected field type", $field1->getType() == $libgmsec_perl::Field::Type_I8);
	$test->check("Unexpected field name", $field1->getName() eq $name);
	$test->check("Unexpected field value", $field1->getValue() == $value);
	$test->check("Unexpected XML", $field1->toXML() eq "<FIELD NAME=\"MY-FIELD\" TYPE=\"I8\">10</FIELD>");
	$test->check("Unexpected JSON", $field1->toJSON() eq "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I8\",\"VALUE\":\"10\"}");

	# Copy-constructor
	my $field2 = libgmsec_perl::I8Field->new($field1);
	$test->check("Unexpected field type", $field2->getType() == $field1->getType());
	$test->check("Unexpected field name", $field2->getName() eq $field1->getName());
	$test->check("Unexpected field value", $field2->getValue() == $field1->getValue());
	$test->check("Unexpected XML", $field2->toXML() eq $field1->toXML());
	$test->check("Unexpected JSON", $field2->toJSON() eq $field1->toJSON());

	# Off-Nominal test
	eval
	{
		# undef field name
		libgmsec_perl::I8Field->new(undef, $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}

	eval
	{
		# empty-string field name
		libgmsec_perl::I8Field->new("", $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}
}


sub test_I16Field
{
	libgmsec_perl::logInfo("Test I16Field");

	my ($test) = @_;

	# Nominal test
	my $name  = "MY-FIELD";
	my $value = 10;

	# Constructor
	my $field1 = libgmsec_perl::I16Field->new($name, $value);
	$test->check("Unexpected field type", $field1->getType() == $libgmsec_perl::Field::Type_I16);
	$test->check("Unexpected field name", $field1->getName() eq $name);
	$test->check("Unexpected field value", $field1->getValue() == $value);
	$test->check("Unexpected XML", $field1->toXML() eq "<FIELD NAME=\"MY-FIELD\" TYPE=\"I16\">10</FIELD>");
	$test->check("Unexpected JSON", $field1->toJSON() eq "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I16\",\"VALUE\":\"10\"}");

	# Copy-constructor
	my $field2 = libgmsec_perl::I16Field->new($field1);
	$test->check("Unexpected field type", $field2->getType() == $field1->getType());
	$test->check("Unexpected field name", $field2->getName() eq $field1->getName());
	$test->check("Unexpected field value", $field2->getValue() == $field1->getValue());
	$test->check("Unexpected XML", $field2->toXML() eq $field1->toXML());
	$test->check("Unexpected JSON", $field2->toJSON() eq $field1->toJSON());

	# Off-Nominal test
	eval
	{
		# undef field name
		libgmsec_perl::I16Field->new(undef, $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}

	eval
	{
		# empty-string field name
		libgmsec_perl::I16Field->new("", $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}
}


sub test_I32Field
{
	libgmsec_perl::logInfo("Test I32Field");

	my ($test) = @_;

	# Nominal test
	my $name  = "MY-FIELD";
	my $value = 10;

	# Constructor
	my $field1 = libgmsec_perl::I32Field->new($name, $value);
	$test->check("Unexpected field type", $field1->getType() == $libgmsec_perl::Field::Type_I32);
	$test->check("Unexpected field name", $field1->getName() eq $name);
	$test->check("Unexpected field value", $field1->getValue() == $value);
	$test->check("Unexpected XML", $field1->toXML() eq "<FIELD NAME=\"MY-FIELD\" TYPE=\"I32\">10</FIELD>");
	$test->check("Unexpected JSON", $field1->toJSON() eq "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I32\",\"VALUE\":\"10\"}");

	# Copy-constructor
	my $field2 = libgmsec_perl::I32Field->new($field1);
	$test->check("Unexpected field type", $field2->getType() == $field1->getType());
	$test->check("Unexpected field name", $field2->getName() eq $field1->getName());
	$test->check("Unexpected field value", $field2->getValue() == $field1->getValue());
	$test->check("Unexpected XML", $field2->toXML() eq $field1->toXML());
	$test->check("Unexpected JSON", $field2->toJSON() eq $field1->toJSON());

	# Off-Nominal test
	eval
	{
		# undef field name
		libgmsec_perl::I32Field->new(undef, $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}

	eval
	{
		# empty-string field name
		libgmsec_perl::I32Field->new("", $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}
}


sub test_I64Field
{
	libgmsec_perl::logInfo("Test I64Field");

	my ($test) = @_;

	# Nominal test
	my $name  = "MY-FIELD";
	my $value = 10;

	# Constructor
	my $field1 = libgmsec_perl::I64Field->new($name, $value);
	$test->check("Unexpected field type", $field1->getType() == $libgmsec_perl::Field::Type_I64);
	$test->check("Unexpected field name", $field1->getName() eq $name);
	$test->check("Unexpected field value", $field1->getValue() == $value);
	$test->check("Unexpected XML", $field1->toXML() eq "<FIELD NAME=\"MY-FIELD\" TYPE=\"I64\">10</FIELD>");
	$test->check("Unexpected JSON", $field1->toJSON() eq "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I64\",\"VALUE\":\"10\"}");

	# Copy-constructor
	my $field2 = libgmsec_perl::I64Field->new($field1);
	$test->check("Unexpected field type", $field2->getType() == $field1->getType());
	$test->check("Unexpected field name", $field2->getName() eq $field1->getName());
	$test->check("Unexpected field value", $field2->getValue() == $field1->getValue());
	$test->check("Unexpected XML", $field2->toXML() eq $field1->toXML());
	$test->check("Unexpected JSON", $field2->toJSON() eq $field1->toJSON());

	# Off-Nominal test
	eval
	{
		# undef field name
		libgmsec_perl::I64Field->new(undef, $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}

	eval
	{
		# empty-string field name
		libgmsec_perl::I64Field->new("", $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}
}


sub test_U8Field
{
	libgmsec_perl::logInfo("Test U8Field");

	my ($test) = @_;

	# Nominal test
	my $name  = "MY-FIELD";
	my $value = 10;

	# Constructor
	my $field1 = libgmsec_perl::U8Field->new($name, $value);
	$test->check("Unexpected field type", $field1->getType() == $libgmsec_perl::Field::Type_U8);
	$test->check("Unexpected field name", $field1->getName() eq $name);
	$test->check("Unexpected field value", $field1->getValue() == $value);
	$test->check("Unexpected XML", $field1->toXML() eq "<FIELD NAME=\"MY-FIELD\" TYPE=\"U8\">10</FIELD>");
	$test->check("Unexpected JSON", $field1->toJSON() eq "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U8\",\"VALUE\":\"10\"}");

	# Copy-constructor
	my $field2 = libgmsec_perl::U8Field->new($field1);
	$test->check("Unexpected field type", $field2->getType() == $field1->getType());
	$test->check("Unexpected field name", $field2->getName() eq $field1->getName());
	$test->check("Unexpected field value", $field2->getValue() == $field1->getValue());
	$test->check("Unexpected XML", $field2->toXML() eq $field1->toXML());
	$test->check("Unexpected JSON", $field2->toJSON() eq $field1->toJSON());

	# Off-Nominal test
	eval
	{
		# undef field name
		libgmsec_perl::U8Field->new(undef, $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}

	eval
	{
		# empty-string field name
		libgmsec_perl::U8Field->new("", $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}
}


sub test_U16Field
{
	libgmsec_perl::logInfo("Test U16Field");

	my ($test) = @_;

	# Nominal test
	my $name  = "MY-FIELD";
	my $value = 10;

	# Constructor
	my $field1 = libgmsec_perl::U16Field->new($name, $value);
	$test->check("Unexpected field type", $field1->getType() == $libgmsec_perl::Field::Type_U16);
	$test->check("Unexpected field name", $field1->getName() eq $name);
	$test->check("Unexpected field value", $field1->getValue() == $value);
	$test->check("Unexpected XML", $field1->toXML() eq "<FIELD NAME=\"MY-FIELD\" TYPE=\"U16\">10</FIELD>");
	$test->check("Unexpected JSON", $field1->toJSON() eq "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U16\",\"VALUE\":\"10\"}");

	# Copy-constructor
	my $field2 = libgmsec_perl::U16Field->new($field1);
	$test->check("Unexpected field type", $field2->getType() == $field1->getType());
	$test->check("Unexpected field name", $field2->getName() eq $field1->getName());
	$test->check("Unexpected field value", $field2->getValue() == $field1->getValue());
	$test->check("Unexpected XML", $field2->toXML() eq $field1->toXML());
	$test->check("Unexpected JSON", $field2->toJSON() eq $field1->toJSON());

	# Off-Nominal test
	eval
	{
		# undef field name
		libgmsec_perl::U16Field->new(undef, $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}

	eval
	{
		# empty-string field name
		libgmsec_perl::U16Field->new("", $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}
}


sub test_U32Field
{
	libgmsec_perl::logInfo("Test U32Field");

	my ($test) = @_;

	# Nominal test
	my $name  = "MY-FIELD";
	my $value = 10;

	# Constructor
	my $field1 = libgmsec_perl::U32Field->new($name, $value);
	$test->check("Unexpected field type", $field1->getType() == $libgmsec_perl::Field::Type_U32);
	$test->check("Unexpected field name", $field1->getName() eq $name);
	$test->check("Unexpected field value", $field1->getValue() == $value);
	$test->check("Unexpected XML", $field1->toXML() eq "<FIELD NAME=\"MY-FIELD\" TYPE=\"U32\">10</FIELD>");
	$test->check("Unexpected JSON", $field1->toJSON() eq "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U32\",\"VALUE\":\"10\"}");

	# Copy-constructor
	my $field2 = libgmsec_perl::U32Field->new($field1);
	$test->check("Unexpected field type", $field2->getType() == $field1->getType());
	$test->check("Unexpected field name", $field2->getName() eq $field1->getName());
	$test->check("Unexpected field value", $field2->getValue() == $field1->getValue());
	$test->check("Unexpected XML", $field2->toXML() eq $field1->toXML());
	$test->check("Unexpected JSON", $field2->toJSON() eq $field1->toJSON());

	# Off-Nominal test
	eval
	{
		# undef field name
		libgmsec_perl::U32Field->new(undef, $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}

	eval
	{
		# empty-string field name
		libgmsec_perl::U32Field->new("", $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}
}


sub test_U64Field
{
	libgmsec_perl::logInfo("Test U64Field");

	my ($test) = @_;

	# Nominal test
	my $name  = "MY-FIELD";
	my $value = 10;

	# Constructor
	my $field1 = libgmsec_perl::U64Field->new($name, $value);
	$test->check("Unexpected field type", $field1->getType() == $libgmsec_perl::Field::Type_U64);
	$test->check("Unexpected field name", $field1->getName() eq $name);
	$test->check("Unexpected field value", $field1->getValue() == $value);
	$test->check("Unexpected XML", $field1->toXML() eq "<FIELD NAME=\"MY-FIELD\" TYPE=\"U64\">10</FIELD>");
	$test->check("Unexpected JSON", $field1->toJSON() eq "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U64\",\"VALUE\":\"10\"}");

	# Copy-constructor
	my $field2 = libgmsec_perl::U64Field->new($field1);
	$test->check("Unexpected field type", $field2->getType() == $field1->getType());
	$test->check("Unexpected field name", $field2->getName() eq $field1->getName());
	$test->check("Unexpected field value", $field2->getValue() == $field1->getValue());
	$test->check("Unexpected XML", $field2->toXML() eq $field1->toXML());
	$test->check("Unexpected JSON", $field2->toJSON() eq $field1->toJSON());

	# Off-Nominal test
	eval
	{
		# undef field name
		libgmsec_perl::U64Field->new(undef, $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}

	eval
	{
		# empty-string field name
		libgmsec_perl::U64Field->new("", $value);
		$test->check("Exception was expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field name is not compliant") != -1);
	}
}


sub test_Field
{
	my ($test) = @_;

	my $field1 = libgmsec_perl::I32Field->new("I32-FIELD", -1234);
	my $field2 = libgmsec_perl::U32Field->new("U32-FIELD", 1234);
	my $field3 = libgmsec_perl::F32Field->new("F32-FIELD", 1234.56);
	my $field4 = libgmsec_perl::StringField->new("STR-FIELD", "Hello World");
	my $field5 = libgmsec_perl::StringField->new("STR-FIELD", "FALSE");
	my $field6 = libgmsec_perl::StringField->new("STR-FIELD", "true");
	my $field7 = libgmsec_perl::BooleanField->new("BOOL-FIELD", 1);

	# getStringValue()
	libgmsec_perl::logInfo("Test Field->getStringValue()");
	$test->check("Fail getStringValue 1", $field1->getStringValue() == "-1234");
	$test->check("Fail getStringValue 2", $field2->getStringValue() == "1234");
	$test->check("Fail getStringValue 3", $field3->getStringValue() == "1234.56");
	$test->check("Fail getStringValue 4", $field4->getStringValue() == "Hello World");

	# getBooleanValue()
	libgmsec_perl::logInfo("Test Field->getBooleanValue()");
	$test->check("Fail getBooleanValue 1", $field1->getBooleanValue() == 1);
	$test->check("Fail getBooleanValue 2", $field2->getBooleanValue() == 1);
	$test->check("Fail getBooleanValue 3", $field3->getBooleanValue() == 1);
	$test->check("Fail getBooleanValue 5", $field5->getBooleanValue() == 0);
	$test->check("Fail getBooleanValue 6", $field6->getBooleanValue() == 1);
	$test->check("Fail getBooleanValue 7", $field7->getBooleanValue() == 1);

	eval
	{
		$field4->getBooleanValue();
		$test->check("Expected boolean conversion exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field cannot be represented as a boolean") != -1);
	}

	# getIntegerValue()
	libgmsec_perl::logInfo("Test Field->getIntegerValue()");
	$test->check("Fail getIntegerValue 1", $field1->getIntegerValue() == -1234);
	$test->check("Fail getIntegerValue 2", $field2->getIntegerValue() == 1234);
	$test->check("Fail getIntegerValue 3", $field3->getIntegerValue() == 1234);

	eval
	{
		$field4->getIntegerValue();
		$test->check("Fail getIntegerValue 4", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field cannot be converted to a GMSEC_I64") != -1);
	}

	# getDoubleValue()
	libgmsec_perl::logInfo("Test Field->getDoubleValue()");
	$test->check("Fail getDoubleValue 1", $field1->getDoubleValue() == -1234.0);
	$test->check("Fail getDoubleValue 2", $field2->getDoubleValue() == 1234.0);
	$test->check("Fail getDoubleValue 3", $field3->getDoubleValue() == 1234.56);

	eval
	{
		$field4->getDoubleValue();
		$test->check("Fail getDoubleValue 4", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Field cannot be converted to a GMSEC_F64") != -1);
	}
}


Test::driver(\&Test_Fields);
