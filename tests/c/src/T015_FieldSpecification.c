#include "TestCase.h"


void test_fieldSpecification_aux(int specVersion, int schemaLevel)
{
	char data[256];
	printToString(data, "gmsec-specification-version=%d gmsec-schema-level=%d", specVersion, schemaLevel);

	GMSEC_Status                status   = statusCreate();
	GMSEC_Config                config   = configCreateUsingData(data, KEY_VALUE_DATA, NULL);
	GMSEC_Specification         spec     = specificationCreateUsingConfig(config, status);
	GMSEC_MessageSpecification* msgSpecs = NULL;
	size_t                      numSpecs = 0;
	size_t                      i, j;

	specificationGetMessageSpecifications(spec, &msgSpecs, &numSpecs, status);

	for (i = 0; i < numSpecs; ++i)
	{
		GMSEC_MessageSpecification msgSpec = msgSpecs[i];

		testCheckBool("Expected to have at least 1 (one) field specification", msgSpec.numFieldSpecs > 0);

		for (j = 0; j < msgSpec.numFieldSpecs; ++j)
		{
			GMSEC_FieldSpecification fldSpec = msgSpec.fieldSpecs[j];

			testCheckBool("FieldSpecification name is NULL", fldSpec.name != NULL);
			testCheckBool("FieldSpecification type is NULL", fldSpec.type != NULL);
			testCheckBool("FieldSpecification mode is NULL", fldSpec.mode != NULL);
			testCheckBool("FieldSpecification classification is NULL", fldSpec.classification != NULL);
			testCheckBool("FieldSpecification value is NULL", fldSpec.value != NULL);
			testCheckBool("FieldSpecification description is NULL", fldSpec.description != NULL);
		}
	}

	specificationDestroyMessageSpecifications(msgSpecs, numSpecs, NULL);
	specificationDestroy(&spec);
	configDestroy(&config);
	statusDestroy(&status);
}


void test_fieldSpecification()
{
	GMSEC_INFO("Test fieldSpecification");

	test_fieldSpecification_aux(GMSEC_MSG_SPEC_CURRENT, GMSEC_SCHEMA_LEVEL_0);
	test_fieldSpecification_aux(GMSEC_MSG_SPEC_CURRENT, GMSEC_SCHEMA_LEVEL_1);
	test_fieldSpecification_aux(GMSEC_MSG_SPEC_CURRENT, GMSEC_SCHEMA_LEVEL_2);

	test_fieldSpecification_aux(GMSEC_MSG_SPEC_2019_00, GMSEC_SCHEMA_LEVEL_0);
	test_fieldSpecification_aux(GMSEC_MSG_SPEC_2019_00, GMSEC_SCHEMA_LEVEL_1);
	test_fieldSpecification_aux(GMSEC_MSG_SPEC_2019_00, GMSEC_SCHEMA_LEVEL_2);
}


int test_FieldSpecification()
{
	testSetDescription("Test FieldSpecification");

	test_fieldSpecification();

	return 0;
}
TEST_DRIVER(test_FieldSpecification)
