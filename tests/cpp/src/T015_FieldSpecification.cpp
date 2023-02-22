#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


void test_field_specification_aux(Test& test, unsigned int specVersion, Specification::SchemaLevel schemaLevel)
{
	Config config;

	std::ostringstream ss;
	ss << specVersion;
	config.addValue("gmsec-specification-version", ss.str().c_str());

	ss.str("");
	ss << (int) schemaLevel;
	config.addValue("gmsec-schema-level", ss.str().c_str());

	try
	{
		Specification spec(config);

		const List<MessageSpecification*>& msgSpecs = spec.getMessageSpecifications();

		for (List<MessageSpecification*>::const_iterator it = msgSpecs.begin(); it != msgSpecs.end(); ++it)
		{
			const MessageSpecification* msgSpec = *it;

			const List<FieldSpecification*>& fieldSpecs = msgSpec->getFieldSpecifications();

			test.check("Expected to have at least 1 (one) field specification", 0 < fieldSpecs.size());

			for (List<FieldSpecification*>::const_iterator it2 = fieldSpecs.begin(); it2 != fieldSpecs.end(); ++it2)
			{
				const FieldSpecification* fieldSpec = *it2;

				test.check("FieldSpecification name is NULL", NULL != fieldSpec->getName());
				test.check("FieldSpecification type is NULL", NULL != fieldSpec->getType());
				test.check("FieldSpecification mode is NULL", NULL != fieldSpec->getMode());
				test.check("FieldSpecification classification is NULL", NULL != fieldSpec->getClassification());
				test.check("FieldSpecification value is NULL", NULL != fieldSpec->getValue());
				test.check("FieldSpecification description is NULL", NULL != fieldSpec->getDescription());
			}
		}
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_field_specification(Test& test)
{
	test_field_specification_aux(test, GMSEC_MSG_SPEC_CURRENT, Specification::SchemaLevel::LEVEL_0);
	test_field_specification_aux(test, GMSEC_MSG_SPEC_CURRENT, Specification::SchemaLevel::LEVEL_1);
	test_field_specification_aux(test, GMSEC_MSG_SPEC_CURRENT, Specification::SchemaLevel::LEVEL_2);

	test_field_specification_aux(test, GMSEC_MSG_SPEC_2019_00, Specification::SchemaLevel::LEVEL_0);
	test_field_specification_aux(test, GMSEC_MSG_SPEC_2019_00, Specification::SchemaLevel::LEVEL_1);
	test_field_specification_aux(test, GMSEC_MSG_SPEC_2019_00, Specification::SchemaLevel::LEVEL_2);
}


int test_FieldSpecification(Test& test)
{
	test.setDescription("Test FieldSpecification");

	test_field_specification(test);

	return 0;
}

TEST_DRIVER(test_FieldSpecification)
