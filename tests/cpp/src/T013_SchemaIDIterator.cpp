#include "TestCase.h"

using namespace gmsec::api5;


void test_schema_id_iterator_aux(Test& test, unsigned int specVersion, Specification::SchemaLevel schemaLevel, int expectedSchemaIds)
{
	Config config;

	std::ostringstream ss;
	ss << specVersion;
	config.addValue("gmsec-specification-version", ss.str().c_str());

	ss.str("");
	ss << (int) schemaLevel;
	config.addValue("gmsec-schema-level", ss.str().c_str());

	Specification spec(config);

	SchemaIDIterator& iter = spec.getSchemaIDIterator();

	int numSchemaIds = 0;

	while (iter.hasNext())
	{
		const char* id = iter.next();

		test.check("Schema ID is NULL", NULL != id);

		//std::cout << "schemaID is: " << id << std::endl;

		++numSchemaIds;
	}

	//std::cout << "version: " << specVersion << ", level: " << (int)schemaLevel << ", ids: " << numSchemaIds << std::endl;

	test.check("Unexpected number of schema IDs", expectedSchemaIds == numSchemaIds);
}


void test_schema_id_iterator(Test& test)
{
	test_schema_id_iterator_aux(test, GMSEC_MSG_SPEC_CURRENT, Specification::SchemaLevel::LEVEL_0, 73);
	test_schema_id_iterator_aux(test, GMSEC_MSG_SPEC_CURRENT, Specification::SchemaLevel::LEVEL_1, 103);
	test_schema_id_iterator_aux(test, GMSEC_MSG_SPEC_CURRENT, Specification::SchemaLevel::LEVEL_2, 106);

	test_schema_id_iterator_aux(test, GMSEC_MSG_SPEC_2019_00, Specification::SchemaLevel::LEVEL_0, 73);
	test_schema_id_iterator_aux(test, GMSEC_MSG_SPEC_2019_00, Specification::SchemaLevel::LEVEL_1, 103);
	test_schema_id_iterator_aux(test, GMSEC_MSG_SPEC_2019_00, Specification::SchemaLevel::LEVEL_2, 106);
}


void test_schema_id_iterator_reset(Test& test)
{
	Specification spec;

	SchemaIDIterator& iter = spec.getSchemaIDIterator();

	while (iter.hasNext())
	{
		(void) iter.next();
	}

	test.check("Expected no more schema IDs", false == iter.hasNext());

	iter.reset();

	test.check("Expected to have schema IDs", true == iter.hasNext());
}


int test_SchemaIterator(Test& test)
{
	test.setDescription("Test SchemaIterator");

	test_schema_id_iterator(test);
	test_schema_id_iterator_reset(test);

	return 0;
}

TEST_DRIVER(test_SchemaIterator)
