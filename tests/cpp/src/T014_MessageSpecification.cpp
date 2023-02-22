#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


void test_message_specification_aux(Test& test, unsigned int specVersion, Specification::SchemaLevel schemaLevel)
{
	Config config;

	std::ostringstream ss;
	ss << specVersion;
	config.addValue("gmsec-specification-version", ss.str().c_str());

	ss.str("");
	ss << (int) schemaLevel;
	config.addValue("gmsec-schema-level", ss.str().c_str());

	Specification spec(config);

	const List<MessageSpecification*>& msgSpecs = spec.getMessageSpecifications();

	test.check("Expected at least 1 (one) message specification", 0 < msgSpecs.size());

	for (List<MessageSpecification*>::const_iterator it = msgSpecs.begin(); it != msgSpecs.end(); ++it)
	{
		const MessageSpecification* msgSpec = *it;

		test.check("Schema ID is NULL", NULL != msgSpec->getSchemaID());
		test.check("Subject Template is NULL", NULL != msgSpec->getSubjectTemplate());

		const List<FieldSpecification*>& fieldSpecs = msgSpec->getFieldSpecifications();

		test.check("Expected at least 1 (one) field specification", 0 < fieldSpecs.size());
	}
}


void test_message_specification(Test& test)
{
	test_message_specification_aux(test, GMSEC_MSG_SPEC_CURRENT, Specification::SchemaLevel::LEVEL_0);
	test_message_specification_aux(test, GMSEC_MSG_SPEC_CURRENT, Specification::SchemaLevel::LEVEL_1);
	test_message_specification_aux(test, GMSEC_MSG_SPEC_CURRENT, Specification::SchemaLevel::LEVEL_2);

	test_message_specification_aux(test, GMSEC_MSG_SPEC_2019_00, Specification::SchemaLevel::LEVEL_0);
	test_message_specification_aux(test, GMSEC_MSG_SPEC_2019_00, Specification::SchemaLevel::LEVEL_1);
	test_message_specification_aux(test, GMSEC_MSG_SPEC_2019_00, Specification::SchemaLevel::LEVEL_2);
}


int test_MessageSpecification(Test& test)
{
	test.setDescription("Test MessageSpecification");

	test_message_specification(test);

	return 0;
}

TEST_DRIVER(test_MessageSpecification)
