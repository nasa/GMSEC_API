#include "TestCase.h"


void test_messageSpecification_aux(int specVersion, int schemaLevel)
{
	char data[256];
	printToString(data, "gmsec-specification-version=%d gmsec-schema-level=%d", specVersion, schemaLevel);

	GMSEC_Status                status   = statusCreate();
	GMSEC_Config                config   = configCreateUsingData(data, KEY_VALUE_DATA, NULL);
	GMSEC_Specification         spec     = specificationCreateUsingConfig(config, status);
	GMSEC_MessageSpecification* msgSpecs = NULL;
	size_t                      numSpecs = 0;
	size_t                      i;

	testRequireBool("Unexpected error in attempt to create Specification", statusHasError(status) == GMSEC_FALSE);

	specificationGetMessageSpecifications(spec, &msgSpecs, &numSpecs, status);

	testCheckBool("Unexpected error in attempt to acquire msg specs", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected number of msg specs", numSpecs > 0);
	testCheckBool("Array of msg specs is NULL", msgSpecs != NULL);

	for (i = 0; i < numSpecs; ++i)
	{
		GMSEC_MessageSpecification msgSpec = msgSpecs[i];

		testCheckBool("Schema ID is NULL", msgSpec.schemaID != NULL);
		testCheckBool("Subject Template is NULL", msgSpec.subjectTemplate != NULL);
		testCheckBool("Unexpected number of field specs", msgSpec.numFieldSpecs > 0);
		testCheckBool("Array of field specs is NULL", msgSpec.fieldSpecs != NULL);
	}

	specificationDestroyMessageSpecifications(msgSpecs, numSpecs, NULL);
	specificationDestroy(&spec);
	configDestroy(&config);
	statusDestroy(&status);
}


void test_messageSpecification()
{
	GMSEC_INFO("Test messageSpecification");

	test_messageSpecification_aux(GMSEC_MSG_SPEC_CURRENT, GMSEC_SCHEMA_LEVEL_0);
	test_messageSpecification_aux(GMSEC_MSG_SPEC_CURRENT, GMSEC_SCHEMA_LEVEL_1);
	test_messageSpecification_aux(GMSEC_MSG_SPEC_CURRENT, GMSEC_SCHEMA_LEVEL_2);

	test_messageSpecification_aux(GMSEC_MSG_SPEC_2019_00, GMSEC_SCHEMA_LEVEL_0);
	test_messageSpecification_aux(GMSEC_MSG_SPEC_2019_00, GMSEC_SCHEMA_LEVEL_1);
	test_messageSpecification_aux(GMSEC_MSG_SPEC_2019_00, GMSEC_SCHEMA_LEVEL_2);
}


int test_MessageSpecification()
{
	testSetDescription("Test MessageSpecification");

	test_messageSpecification();

	return 0;
}
TEST_DRIVER(test_MessageSpecification)
