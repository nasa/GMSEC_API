#include "TestCase.h"


void test_schemaIDIterator_aux(int specVersion, int schemaLevel, int numExpectedIds)
{
	char data[256];
	printToString(data, "gmsec-specification-version=%d gmsec-schema-level=%d", specVersion, schemaLevel);

	GMSEC_Status           status = statusCreate();
	GMSEC_Config           config = configCreateUsingData(data, KEY_VALUE_DATA, NULL);
	GMSEC_Specification    spec   = specificationCreateUsingConfig(config, status);
	GMSEC_SchemaIDIterator iter   = specificationGetSchemaIDIterator(spec, NULL);
	int                    numIds = 0;

	while (schemaIDIteratorHasNext(iter, NULL) == GMSEC_TRUE)
	{
		const char* schemaID = schemaIDIteratorNext(iter, NULL);

		testCheckBool("Schema ID is NULL", schemaID != NULL);

		++numIds;
	}
	testCheckBool("Unexpected number of schema IDs found", numIds == numExpectedIds);


	// Off-nominal test(s)
	(void) schemaIDIteratorNext(iter, status);
	testCheckBool("Expected status to indicate an error", statusHasError(status) == GMSEC_TRUE);

	(void) schemaIDIteratorHasNext(NULL, status);
	testCheckBool("Expected status to indicate an error", statusHasError(status) == GMSEC_TRUE);

	specificationDestroy(&spec);
	configDestroy(&config);
	statusDestroy(&status);
}


void test_schemaIDIterator()
{
	GMSEC_INFO("Test schemaIDIterator");

	// Nominal tests
	{
		test_schemaIDIterator_aux(GMSEC_MSG_SPEC_CURRENT, GMSEC_SCHEMA_LEVEL_0, 73);
		test_schemaIDIterator_aux(GMSEC_MSG_SPEC_CURRENT, GMSEC_SCHEMA_LEVEL_1, 103);
		test_schemaIDIterator_aux(GMSEC_MSG_SPEC_CURRENT, GMSEC_SCHEMA_LEVEL_2, 106);

		test_schemaIDIterator_aux(GMSEC_MSG_SPEC_2019_00, GMSEC_SCHEMA_LEVEL_0, 73);
		test_schemaIDIterator_aux(GMSEC_MSG_SPEC_2019_00, GMSEC_SCHEMA_LEVEL_1, 103);
		test_schemaIDIterator_aux(GMSEC_MSG_SPEC_2019_00, GMSEC_SCHEMA_LEVEL_2, 106);
	}

	// Off-nominal test(s)
	{
		GMSEC_Status status = statusCreate();

		const char* schemaID = schemaIDIteratorNext(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected NULL schema ID", schemaID == NULL);

		statusDestroy(&status);
	}
}


void test_schemaIDIteratorReset()
{
	GMSEC_INFO("Test schemaIDIteratorReset");

	GMSEC_Status           status = statusCreate();
	GMSEC_Specification    spec   = specificationCreate(NULL);
	GMSEC_SchemaIDIterator iter   = specificationGetSchemaIDIterator(spec, NULL);

	while (schemaIDIteratorHasNext(iter, NULL) == GMSEC_TRUE)
	{
		(void) schemaIDIteratorNext(iter, NULL);
	}

	testCheckBool("Expected no more schema IDs", schemaIDIteratorHasNext(iter, NULL) == GMSEC_FALSE);

	schemaIDIteratorReset(iter, NULL);

	testCheckBool("Expected to have schema IDs", schemaIDIteratorHasNext(iter, NULL) == GMSEC_TRUE);

	// Off-nominal test(s)
	(void) schemaIDIteratorReset(NULL, status);
	testCheckBool("Expected status to indicate an error", statusHasError(status) == GMSEC_TRUE);

	specificationDestroy(&spec);
	statusDestroy(&status);
}


int test_SchemaIDIterator()
{
	testSetDescription("Test SchemaIDIterator");

	test_schemaIDIterator();       // tests ...HasNext() and ...Next()
	test_schemaIDIteratorReset();

	return 0;
}
TEST_DRIVER(test_SchemaIDIterator)
