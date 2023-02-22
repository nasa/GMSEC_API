#include "TestCase.h"


void test_specificationCreate()
{
	GMSEC_INFO("Test specificationCreate()");

	GMSEC_Status        status = statusCreate();
	GMSEC_Specification spec   = specificationCreate(status);

	testRequireBool("Error creating specification", statusHasError(status) == GMSEC_FALSE);
	testRequireBool("Specification is NULL", spec != NULL);
	testCheckBool("Unexpected specification version", specificationGetVersion(spec, NULL) == GMSEC_MSG_SPEC_CURRENT);
	testCheckBool("Unexpected specification schema level", specificationGetSchemaLevel(spec, NULL) == GMSEC_SCHEMA_LEVEL_0);

	specificationDestroy(&spec);
	statusDestroy(&status);
}


void test_specificationCreateUsingConfig()
{
	GMSEC_INFO("Test specificationCreateUsingConfig()");

	GMSEC_Status status = statusCreate();

	// Nominal tests
	{
		GMSEC_Config        config = configCreateUsingData("useless-key=useless-value", KEY_VALUE_DATA, NULL);
		GMSEC_Specification spec   = specificationCreateUsingConfig(config, status);

		testRequireBool("Error creating specification", statusHasError(status) == GMSEC_FALSE);
		testRequireBool("Specification is NULL", spec != NULL);
		testCheckBool("Unexpected specification version", specificationGetVersion(spec, NULL) == GMSEC_MSG_SPEC_2019_00);
		testCheckBool("Unexpected specification schema level", specificationGetSchemaLevel(spec, NULL) == GMSEC_SCHEMA_LEVEL_0);

		specificationDestroy(&spec);
		configDestroy(&config);
	}

	{
		GMSEC_Config        config = configCreateUsingData("gmsec-specification-version=201900 gmsec-schema-level=0", KEY_VALUE_DATA, NULL);
		GMSEC_Specification spec   = specificationCreateUsingConfig(config, status);

		testRequireBool("Error creating specification", statusHasError(status) == GMSEC_FALSE);
		testRequireBool("Specification is NULL", spec != NULL);
		testCheckBool("Unexpected specification version", specificationGetVersion(spec, NULL) == GMSEC_MSG_SPEC_2019_00);
		testCheckBool("Unexpected specification schema level", specificationGetSchemaLevel(spec, NULL) == GMSEC_SCHEMA_LEVEL_0);

		specificationDestroy(&spec);
		configDestroy(&config);
	}

	{
		GMSEC_Config        config = configCreateUsingData("gmsec-specification-version=201900 gmsec-schema-level=0", KEY_VALUE_DATA, NULL);
		GMSEC_Specification spec   = specificationCreateUsingConfig(config, status);

		testRequireBool("Error creating specification", statusHasError(status) == GMSEC_FALSE);
		testRequireBool("Specification is NULL", spec != NULL);
		testCheckBool("Unexpected specification version", specificationGetVersion(spec, NULL) == GMSEC_MSG_SPEC_2019_00);
		testCheckBool("Unexpected specification schema level", specificationGetSchemaLevel(spec, NULL) == GMSEC_SCHEMA_LEVEL_0);

		specificationDestroy(&spec);
		configDestroy(&config);
	}

	{
		GMSEC_Config        config = configCreateUsingData("gmsec-specification-version=201900 gmsec-schema-level=1", KEY_VALUE_DATA, NULL);
		GMSEC_Specification spec   = specificationCreateUsingConfig(config, status);

		testRequireBool("Error creating specification", statusHasError(status) == GMSEC_FALSE);
		testRequireBool("Specification is NULL", spec != NULL);
		testCheckBool("Unexpected specification version", specificationGetVersion(spec, NULL) == GMSEC_MSG_SPEC_2019_00);
		testCheckBool("Unexpected specification schema level", specificationGetSchemaLevel(spec, NULL) == GMSEC_SCHEMA_LEVEL_1);

		specificationDestroy(&spec);
		configDestroy(&config);
	}

	{
		GMSEC_Config        config = configCreateUsingData("gmsec-specification-version=201900 gmsec-schema-level=2", KEY_VALUE_DATA, NULL);
		GMSEC_Specification spec   = specificationCreateUsingConfig(config, status);

		testRequireBool("Error creating specification", statusHasError(status) == GMSEC_FALSE);
		testRequireBool("Specification is NULL", spec != NULL);
		testCheckBool("Unexpected specification version", specificationGetVersion(spec, NULL) == GMSEC_MSG_SPEC_2019_00);
		testCheckBool("Unexpected specification schema level", specificationGetSchemaLevel(spec, NULL) == GMSEC_SCHEMA_LEVEL_2);

		specificationDestroy(&spec);
		configDestroy(&config);
	}

	// Off-nominal test(s)
	{
		GMSEC_Specification spec = specificationCreateUsingConfig(NULL, status);

		testRequireBool("Expected error creating specification", statusHasError(status) == GMSEC_TRUE);
		testRequireBool("Expected specification to be NULL", spec == NULL);
	}

	{
		GMSEC_Config        config = configCreateUsingData("gmsec-specification-version=201800", KEY_VALUE_DATA, NULL);
		GMSEC_Specification spec   = specificationCreateUsingConfig(config, status);

		testRequireBool("Expected error creating specification", statusHasError(status) == GMSEC_TRUE);
		testRequireBool("Expected specification to be NULL", spec == NULL);

		configDestroy(&config);
	}

	statusDestroy(&status);
}


void test_specificationCreateCopy()
{
	GMSEC_INFO("Test specificationCreateCopy()");

	GMSEC_Status status = statusCreate();

	// Nominal test
	{
		GMSEC_Config        config = configCreateUsingData("gmsec-specification-version=201900 gmsec-schema-level=2", KEY_VALUE_DATA, NULL);
		GMSEC_Specification spec1  = specificationCreateUsingConfig(config, NULL);
		GMSEC_Specification spec2  = specificationCreateCopy(spec1, status);

		testRequireBool("Error creating specification", statusHasError(status) == GMSEC_FALSE);
		testRequireBool("Specification is NULL", spec2 != NULL);
		testCheckBool("Unexpected specification version", specificationGetVersion(spec1, NULL) == specificationGetVersion(spec2, NULL));
		testCheckBool("Unexpected specification schema level", specificationGetSchemaLevel(spec1, NULL) == specificationGetSchemaLevel(spec2, NULL));

		specificationDestroy(&spec1);
		specificationDestroy(&spec2);
		configDestroy(&config);
	}

	// Off-nominal test
	{
		GMSEC_Specification spec = specificationCreateCopy(NULL, status);

		testRequireBool("Expected error creating specification", statusHasError(status) == GMSEC_TRUE);
		testRequireBool("Expected specification to be NULL", spec == NULL);
	}

	statusDestroy(&status);
}


void test_specificationDestroy()
{
	GMSEC_INFO("Test specificationDestroy()");

	GMSEC_Specification spec = specificationCreate(NULL);

	testRequireBool("Specification is NULL", spec != NULL);

	specificationDestroy(&spec);

	testRequireBool("Specification should be NULL", spec == NULL);
}


void test_specificationGetSchemaIDIterator()
{
	GMSEC_INFO("Test specificationGetSchemaIDIterator()");

	GMSEC_Status        status = statusCreate();
	GMSEC_Specification spec   = specificationCreate(NULL);

	// Nominal test
	GMSEC_SchemaIDIterator iter = specificationGetSchemaIDIterator(spec, status);
	testCheckBool("Expected non-NULL iterator", iter != NULL);
	testCheckBool("Expected no error", statusHasError(status) == GMSEC_FALSE);

	// Off-nominal test
	iter = specificationGetSchemaIDIterator(NULL, status);
	testCheckBool("Expected NULL iterator", iter == NULL);
	testCheckBool("Expected error", statusHasError(status) == GMSEC_TRUE);

	specificationDestroy(&spec);
	statusDestroy(&status);
}


void test_specificationGetVersion()
{
	GMSEC_INFO("Test specificationGetVersion()");

	GMSEC_Status status = statusCreate();

	// Nominal tests have been performed earlier

	// Off-nominal test
	specificationGetVersion(NULL, status);
	testCheckBool("An error is expected", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_specificationGetSchemaLevel()
{
	GMSEC_INFO("Test specificationGetSchemaLevel()");

	GMSEC_Status status = statusCreate();

	// Nominal tests have been performed earlier

	// Off-nominal test
	specificationGetSchemaLevel(NULL, status);
	testCheckBool("An error is expected", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_specificationGetMessageSpecifications()
{
	GMSEC_INFO("Test specificationGetMessageSpecifications()");

	GMSEC_Status                status   = statusCreate();
	GMSEC_Specification         spec     = specificationCreate(NULL);
	size_t                      numSpecs = 0;
	GMSEC_MessageSpecification* msgSpecs;

	specificationGetMessageSpecifications(spec, &msgSpecs, &numSpecs, NULL);

	// Nominal test
	testCheckBool("Expected message specifications", numSpecs > 0);
	specificationDestroyMessageSpecifications(msgSpecs, numSpecs, status);
	testCheckBool("An error is not expected", statusHasError(status) == GMSEC_FALSE);

	// Off-nominal test
	specificationGetMessageSpecifications(NULL, &msgSpecs, &numSpecs, status);
	testCheckBool("An error is expected", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Expected no message specifications", numSpecs == 0);

	specificationDestroyMessageSpecifications(NULL, numSpecs, status);
	testCheckBool("An error is expected", statusHasError(status) == GMSEC_TRUE);

	specificationDestroy(&spec);
	statusDestroy(&status);
}


void test_specificationGetHeaderFieldNames()
{
	GMSEC_INFO("Test specificationGetHeaderFieldNames()");

	GMSEC_Status                status = statusCreate();
	GMSEC_Specification         spec = specificationCreate(NULL);
	size_t                      numHeaders = 0;
	const char**				headerList;

	// Nominal test
	specificationGetHeaderFieldNames(spec, "DEFAULT", &headerList, &numHeaders, NULL);
	testCheckBool("Expected headers", numHeaders == 26);
	specificationDestroyHeaderList(&headerList);
	testCheckBool("An error is not expected", statusHasError(status) == GMSEC_FALSE);

	specificationGetHeaderFieldNames(spec, "C2MS", &headerList, &numHeaders, NULL);
	testCheckBool("Expected headers", numHeaders == 26);
	specificationDestroyHeaderList(&headerList);

	specificationGetHeaderFieldNames(spec, "", &headerList, &numHeaders, NULL);
	testCheckBool("Expected headers", numHeaders == 26);
	specificationDestroyHeaderList(&headerList);

	specificationGetHeaderFieldNames(spec, NULL, &headerList, &numHeaders, NULL);
	testCheckBool("Expected headers", numHeaders == 26);
	specificationDestroyHeaderList(&headerList);

	// Off-nominal test
	specificationGetHeaderFieldNames(NULL, "DEFAULT", &headerList, &numHeaders, status);
	testCheckBool("An error is expected", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Expected no headers", numHeaders == 0);

	specificationDestroy(&spec);
	statusDestroy(&status);
}


int test_Specification()
{
	testSetDescription("Test Specification");

	test_specificationCreate();
	test_specificationCreateUsingConfig();
	test_specificationCreateCopy();
	test_specificationDestroy();
	test_specificationGetSchemaIDIterator();
	test_specificationGetVersion();
	test_specificationGetSchemaLevel();
	test_specificationGetMessageSpecifications();   // also tests specificationDestroyMessageSpecifications()
	test_specificationGetHeaderFieldNames();   // also tests specificationDestroyHeaderList()

	return 0;
}
TEST_DRIVER(test_Specification)
