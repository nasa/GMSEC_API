#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


void test_constructor_1(Test& test)
{
	GMSEC_INFO << "Basic constructor (no args)...";

	try
	{
		Specification spec;

		test.check("Unexpected specification version", GMSEC_MSG_SPEC_CURRENT == spec.getVersion());
		test.check("Unexpected specification schema level", Specification::SchemaLevel::LEVEL_0 == spec.getSchemaLevel());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_constructor_2(Test& test)
{
	GMSEC_INFO << "Constructor with Config...";

	// Nominal tests
	try
	{
		Specification spec(test.getConfig());
		test.check("Unexpected specification version", GMSEC_MSG_SPEC_CURRENT == spec.getVersion());
		test.check("Unexpected specification schema level", Specification::SchemaLevel::LEVEL_0 == spec.getSchemaLevel());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	try
	{
		Specification spec0(Config("gmsec-specification-version=201900 gmsec-schema-level=0", DataType::KEY_VALUE_DATA));
		Specification spec1(Config("gmsec-specification-version=201900 gmsec-schema-level=1", DataType::KEY_VALUE_DATA));
		Specification spec2(Config("gmsec-specification-version=201900 gmsec-schema-level=2", DataType::KEY_VALUE_DATA));

		test.check("Unexpected specification version", GMSEC_MSG_SPEC_2019_00 == spec0.getVersion());
		test.check("Unexpected specification schema level", Specification::SchemaLevel::LEVEL_0 == spec0.getSchemaLevel());

		test.check("Unexpected specification version", GMSEC_MSG_SPEC_2019_00 == spec1.getVersion());
		test.check("Unexpected specification schema level", Specification::SchemaLevel::LEVEL_1 == spec1.getSchemaLevel());

		test.check("Unexpected specification version", GMSEC_MSG_SPEC_2019_00 == spec2.getVersion());
		test.check("Unexpected specification schema level", Specification::SchemaLevel::LEVEL_2 == spec2.getSchemaLevel());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	try
	{
		Specification spec(Config("name=value", DataType::KEY_VALUE_DATA));
		test.check("Unexpected specification version", GMSEC_MSG_SPEC_CURRENT == spec.getVersion());
		test.check("Unexpected specification schema level", Specification::SchemaLevel::LEVEL_0 == spec.getSchemaLevel());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal tests
	try
	{
		Specification spec(Config("gmsec-specification-version=201300", DataType::KEY_VALUE_DATA));
		test.check("Expected exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Could not list files in template directory") != std::string::npos);
	}

	try
	{
		Specification spec(Config("gmsec-specification-version=ABC", DataType::KEY_VALUE_DATA));
		test.check("Expected exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("\"GMSEC-SPECIFICATION-VERSION\" contains invalid value") != std::string::npos);
	}

	try
	{
		Specification spec(Config("gmsec-schema-level=3", DataType::KEY_VALUE_DATA));
		test.check("Expected exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("DIRECTORY is missing definition for LEVEL-3 HEADER") != std::string::npos);
	}

	try
	{
		Specification spec(Config("gmsec-schema-level=10", DataType::KEY_VALUE_DATA));
		test.check("Expected exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("\"GMSEC-SCHEMA-LEVEL\" contains out-of-range integer") != std::string::npos);
	}

	try
	{
		Specification spec(Config("gmsec-schema-level=ABC", DataType::KEY_VALUE_DATA));
		test.check("Expected exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("\"GMSEC-SCHEMA-LEVEL\" contains invalid value") != std::string::npos);
	}

	try
	{
		Specification spec(Config("gmsec-schema-path=here", DataType::KEY_VALUE_DATA));
		test.check("Expected exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Could not list files in template directory") != std::string::npos);
	}
}


void test_copy_constructor(Test& test)
{
	GMSEC_INFO << "Test copy-constructor...";

	try
	{
		Specification spec;
		Specification spec_copy = spec;

		test.check("Unexpected specification version", spec.getVersion() == spec_copy.getVersion());
		test.check("Unexpected specification schema level", spec.getSchemaLevel() == spec_copy.getSchemaLevel());
		test.check("Unexpected message specifications", spec.getMessageSpecifications().size() == spec_copy.getMessageSpecifications().size());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_schema_id_iterator(Test& test)
{
	GMSEC_INFO << "Test iterator...";
	try
	{
		Specification spec;

		SchemaIDIterator& iter = spec.getSchemaIDIterator();

		test.check("Expected availability of schema IDs", iter.hasNext());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_message_specifications(Test& test)
{
	GMSEC_INFO << "Test get message specifications...";
	try
	{
		Specification spec;

		const List<MessageSpecification*>& msgSpecs = spec.getMessageSpecifications();

		test.check("Expected to have message specifications", 0 < msgSpecs.size());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_headers(Test& test)
{
	GMSEC_INFO << "Test get headers...";
	try
	{
		Specification spec;

		const List<const char*> headers = spec.getHeaderFieldNames();
		test.check("Expected to have headers", headers.size() == 26);

		const List<const char*> headers2 = spec.getHeaderFieldNames("");
		test.check("Expected to have headers", headers2.size() == 26);

		const List<const char*> headers3 = spec.getHeaderFieldNames(NULL);
		test.check("Expected to have headers", headers3.size() == 26);

		const List<const char*> defaultHeaders = spec.getHeaderFieldNames("DEFAULT");
		test.check("Expected to have headers", defaultHeaders.size() == 26);

		const List<const char*>  C2MSHeaders = spec.getHeaderFieldNames("C2MS");
		test.check("Expected to have headers", C2MSHeaders.size() == 26);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


int test_Specification(Test& test)
{
	test.setDescription("Test Specification");

	test_constructor_1(test);
	test_constructor_2(test);
	test_copy_constructor(test);
	test_get_schema_id_iterator(test);
	test_get_message_specifications(test);
	test_get_headers(test);

	return 0;
}

TEST_DRIVER(test_Specification)
