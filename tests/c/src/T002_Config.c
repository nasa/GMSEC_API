#include "TestCase.h"


void test_configCreate()
{
	GMSEC_INFO("Test configCreate()");

	const char* name  = NULL;
	const char* value = NULL;

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreate();

	configGetFirst(config, &name, &value, status);
	testCheckBool("Error expected", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Expected no name", name == NULL);
	testCheckBool("Expected no value", value == NULL);

	configDestroy(&config);
	statusDestroy(&status);
}


void test_configCreateWithArgs()
{
	GMSEC_INFO("Test configCreateWithArgs()");

	const char* name  = NULL;
	const char* value = NULL;

	int argc = 2;
	char* argv[] = { (char*) "mw-id=bolt", (char*) "server=localhost", NULL };

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreateWithArgs(argc, argv);

	configGetFirst(config, &name, &value, status);
	testCheckBool("Expected config entry", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected mw-id", strcompare(name, "mw-id") == 0);
	testCheckBool("Expected bolt", strcompare(value, "bolt") == 0);

	configGetNext(config, &name, &value, status);
	testCheckBool("Expected config entry", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected server", strcompare(name, "server") == 0);
	testCheckBool("Expected localhost", strcompare(value, "localhost") == 0);

	configGetNext(config, &name, &value, status);
	testCheckBool("Expected no additional config entries", statusHasError(status) == GMSEC_TRUE);

	configDestroy(&config);
	statusDestroy(&status);
}


void test_configCreateUsingData()
{
	GMSEC_INFO("Test configCreateUsingData()");

	const char* name  = NULL;
	const char* value = NULL;

	GMSEC_Status status = statusCreate();

	// Using key=value pairs
	GMSEC_Config config1 = configCreateUsingData("mw-id=bolt server=localhost", KEY_VALUE_DATA, status);
	testRequireBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);

	configGetFirst(config1, &name, &value, status);
	testCheckBool("Expected config entry", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected mw-id", strcompare(name, "mw-id") == 0);
	testCheckBool("Expected bolt", strcompare(value, "bolt") == 0);

	configGetNext(config1, &name, &value, status);
	testCheckBool("Expected config entry", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected server", strcompare(name, "server") == 0);
	testCheckBool("Expected localhost", strcompare(value, "localhost") == 0);

	configGetNext(config1, &name, &value, status);
	testCheckBool("Expected no additional config entries", statusHasError(status) == GMSEC_TRUE);

	// Using XML data
	GMSEC_Config config2 = configCreateUsingData(configToXML(config1, NULL), XML_DATA, status);
	testRequireBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);

	configGetFirst(config2, &name, &value, status);
	testCheckBool("Expected config entry", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected mw-id", strcompare(name, "mw-id") == 0);
	testCheckBool("Expected bolt", strcompare(value, "bolt") == 0);

	configGetNext(config2, &name, &value, status);
	testCheckBool("Expected config entry", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected server", strcompare(name, "server") == 0);
	testCheckBool("Expected localhost", strcompare(value, "localhost") == 0);

	configGetNext(config2, &name, &value, status);
	testCheckBool("Expected no additional config entries", statusHasError(status) == GMSEC_TRUE);

	// Using JSON data
	GMSEC_Config config3 = configCreateUsingData(configToJSON(config1, NULL), JSON_DATA, status);
	testRequireBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);

	configGetFirst(config3, &name, &value, status);
	testCheckBool("Expected config entry", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected mw-id", strcompare(name, "mw-id") == 0);
	testCheckBool("Expected bolt", strcompare(value, "bolt") == 0);

	configGetNext(config3, &name, &value, status);
	testCheckBool("Expected config entry", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected server", strcompare(name, "server") == 0);
	testCheckBool("Expected localhost", strcompare(value, "localhost") == 0);

	configGetNext(config3, &name, &value, status);
	testCheckBool("Expected no additional config entries", statusHasError(status) == GMSEC_TRUE);

	// Using malformed XML data
	GMSEC_Config config4 = configCreateUsingData("<CONFIG>", XML_DATA, status);
	testCheckBool("Unexpected error", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("config4 is not NULL", config4 == NULL);

	configDestroy(&config1);
	configDestroy(&config2);
	configDestroy(&config3);
	statusDestroy(&status);
}


void test_configCreateCopy()
{
	GMSEC_INFO("Test configCreateCopy()");

	const char* name  = NULL;
	const char* value = NULL;

	GMSEC_Status status = statusCreate();

	// Nominal tests
	GMSEC_Config config1 = configCreateUsingData("mw-id=bolt server=localhost", KEY_VALUE_DATA, NULL);
	GMSEC_Config config2 = configCreateCopy(config1, status);

	testRequireBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);

	configGetFirst(config2, &name, &value, status);
	testCheckBool("Expected config entry", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected mw-id", strcompare(name, "mw-id") == 0);
	testCheckBool("Expected bolt", strcompare(value, "bolt") == 0);

	configGetNext(config2, &name, &value, status);
	testCheckBool("Expected config entry", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected server", strcompare(name, "server") == 0);
	testCheckBool("Expected localhost", strcompare(value, "localhost") == 0);

	configGetNext(config2, &name, &value, status);
	testCheckBool("Expected no additional config entries", statusHasError(status) == GMSEC_TRUE);

	// Off-nominal
	{
		// Config is NULL
		GMSEC_Config config = configCreateCopy(NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected Config to be NULL", config == NULL);
	}

	configDestroy(&config1);
	configDestroy(&config2);
	statusDestroy(&status);
}


void test_configDestroy()
{
	GMSEC_INFO("Test configDestroy()");

	GMSEC_Config config = configCreate();
	testCheckBool("Expected non-NULL handle", config != NULL);

	configDestroy(&config);
	testCheckBool("Expected NULL handle", config == NULL);
}


void test_configAddValue()
{
	GMSEC_INFO("Test configAddValue()");

	const char* name  = NULL;
	const char* value = NULL;

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreate();

	// Nominal test(s)
	{
		configAddValue(config, "mw-id", "bolt", status);
		testCheckBool("Unexpected error adding value", statusHasError(status) == GMSEC_FALSE);

		configGetFirst(config, &name, &value, status);
		testCheckBool("Expected to retried name/value", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Expected mw-id", strcompare(name, "mw-id") == 0);
		testCheckBool("Expected bolt", strcompare(value, "bolt") == 0);

		configGetNext(config, &name, &value, status);
		testCheckBool("Expected no additional entries", statusHasError(status) == GMSEC_TRUE);
	}

	// Off-nominal test(s)
	{
		// Name is NULL
		configAddValue(config, NULL, "bolt", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Value is NULL
		configAddValue(config, "whatever", NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}

	configDestroy(&config);
	statusDestroy(&status);
}


void test_configClearValue()
{
	GMSEC_INFO("Test configClearValue()");

	const char* name  = NULL;
	const char* value = NULL;

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreate();

	// Nominal test(s)
	{
		configAddValue(config, "mw-id", "bolt", status);

		GMSEC_BOOL cleared = configClearValue(config, "server", status);
		testCheckBool("Expected false when clearing non-existing key", cleared == GMSEC_FALSE);

		cleared = configClearValue(config, "mw-id", status);
		testCheckBool("Expected true when clearing existing key", cleared == GMSEC_TRUE);

		configGetNext(config, &name, &value, status);
		testCheckBool("Expected no additional entries", statusHasError(status) == GMSEC_TRUE);
	}

	// Off-nominal test(s)
	{
		// Name is NULL
		configClearValue(config, NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}

	configDestroy(&config);
	statusDestroy(&status);
}


void test_configGetFirst()
{
	GMSEC_INFO("Test configGetFirst()");

	const char* name;
	const char* value;

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreate();

	// Off-Nominal test(s)
	{
		// Config is NULL
		configGetFirst(NULL, &name, &value, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected name to be NULL", name == NULL);
		testCheckBool("Expected value to be NULL", value == NULL);
	}
	{
		// Name is NULL
		configGetFirst(config, NULL, &value, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Value is NULL
		configGetFirst(config, &name, NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
}


void test_configGetNext()
{
	GMSEC_INFO("Test configGetNext()");

	const char* name;
	const char* value;

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreate();

	// Off-Nominal test(s)
	{
		// Config is NULL
		configGetNext(NULL, &name, &value, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected name to be NULL", name == NULL);
		testCheckBool("Expected value to be NULL", value == NULL);
	}
	{
		// Name is NULL
		configGetNext(config, NULL, &value, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Value is NULL
		configGetNext(config, &name, NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
}


void test_configGetValue()
{
	GMSEC_INFO("Test configGetValue()");

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreateUsingData("mw-id=bolt", KEY_VALUE_DATA, status);

	testCheckBool("Unexpected value", strcompare(configGetValue(config, "mw-id", NULL), "bolt") == 0);
	testCheckBool("Unexpected result for non-existent key", configGetValue(config, "server", NULL) == NULL);

	testCheckBool("Unexpected result for NULL key", configGetValue(config, NULL, NULL) == NULL);
	testCheckBool("Unexpected result for empty key", configGetValue(config, "", NULL) == NULL);

	configDestroy(&config);
	statusDestroy(&status);
}


void test_configGetValueOrDefault()
{
	GMSEC_INFO("Test configGetValueOrDefault()");

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreateUsingData("mw-id=bolt", KEY_VALUE_DATA, status);

	testCheckBool("Unexpected default value", strcompare(configGetValueOrDefault(config, "server", "localhost", NULL), "localhost") == 0);
	testCheckBool("Unexpected default value", strcompare(configGetValueOrDefault(config, NULL,     "localhost", NULL), "localhost") == 0);
	testCheckBool("Unexpected default value", strcompare(configGetValueOrDefault(config, "",       "localhost", NULL), "localhost") == 0);

	configDestroy(&config);
	statusDestroy(&status);
}


void test_configGetBooleanValue()
{
	GMSEC_INFO("Test configGetBooleanValue()");

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreateUsingData("gmsec-rocks=true mw-id=bolt", KEY_VALUE_DATA, status);

	testCheckBool("Expected gmsec-rocks to be true", configGetBooleanValue(config, "gmsec-rocks", NULL) == GMSEC_TRUE);

	testCheckBool("Expected result to be false", configGetBooleanValue(config, "mw-id", status) == GMSEC_FALSE);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	configGetBooleanValue(config, NULL, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	configDestroy(&config);
	statusDestroy(&status);
}


void test_configGetBooleanValueOrDefault()
{
	GMSEC_INFO("Test configGetBooleanValueOrDefault()");

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreateUsingData("gmsec-rocks=true mw-id=bolt", KEY_VALUE_DATA, status);

	testCheckBool("Expected default value of true",  configGetBooleanValueOrDefault(config, "gmsec-rocks", GMSEC_FALSE, NULL) == GMSEC_TRUE);
	testCheckBool("Expected default value of true",  configGetBooleanValueOrDefault(config, "mw-id", GMSEC_FALSE, NULL) == GMSEC_FALSE);
	testCheckBool("Expected default value of false", configGetBooleanValueOrDefault(config, "gmsec-missing", GMSEC_FALSE, NULL) == GMSEC_FALSE);
	testCheckBool("Expected default value of true",  configGetBooleanValueOrDefault(config, "gmsec-missing", GMSEC_TRUE, NULL) == GMSEC_TRUE);
	testCheckBool("Expected default value of true",  configGetBooleanValueOrDefault(config, "", GMSEC_TRUE, NULL) == GMSEC_TRUE);
	testCheckBool("Expected default value of false", configGetBooleanValueOrDefault(config, NULL, GMSEC_FALSE, NULL) == GMSEC_FALSE);

	configDestroy(&config);
	statusDestroy(&status);
}


void test_configGetIntegerValue()
{
	GMSEC_INFO("Test configGetIntegerValue()");

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreateUsingData("gmsec-value=10 mw-id=bolt", KEY_VALUE_DATA, status);

	testCheckBool("Expected integer value of 10", configGetIntegerValue(config, "gmsec-value", status) == 10);
	testCheckBool("Expected status with no error", statusHasError(status) == GMSEC_FALSE);

	configGetIntegerValue(config, "mw-id", status);
	testCheckBool("Expected status with error", statusHasError(status) == GMSEC_TRUE);

	configGetIntegerValue(config, NULL, status);
	testCheckBool("Expected status with error", statusHasError(status) == GMSEC_TRUE);

	configDestroy(&config);
	statusDestroy(&status);
}


void test_configGetIntegerValueOrDefault()
{
	GMSEC_INFO("Test configGetIntegerValueOrDefault()");

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreateUsingData("gmsec-value=10 mw-id=bolt", KEY_VALUE_DATA, status);

	testCheckBool("Expected integer value of 10", configGetIntegerValueOrDefault(config, "gmsec-value", 50, status) == 10);
	testCheckBool("Expected status with no error", statusHasError(status) == GMSEC_FALSE);

	testCheckBool("Expected integer value of 20", configGetIntegerValueOrDefault(config, "missing", 20, status) == 20);
	testCheckBool("Expected status with no error", statusHasError(status) == GMSEC_FALSE);

	testCheckBool("Expected integer value of 30", configGetIntegerValueOrDefault(config, "mw-id", 30, status) == 30);
	testCheckBool("Expected status with no error", statusHasError(status) == GMSEC_FALSE);

	testCheckBool("Expected integer value of 10", configGetIntegerValueOrDefault(config, "", 10, status) == 10);
	testCheckBool("Expected status with no error", statusHasError(status) == GMSEC_FALSE);

	testCheckBool("Expected integer value of 10", configGetIntegerValueOrDefault(config, NULL, 10, status) == 10);
	testCheckBool("Expected status with no error", statusHasError(status) == GMSEC_FALSE);

	configDestroy(&config);
	statusDestroy(&status);
}


void test_configGetDoubleValue()
{
	GMSEC_INFO("Test configGetDoubleValue()");

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreateUsingData("gmsec-value=10.15 mw-id=bolt", KEY_VALUE_DATA, status);

	testCheckBool("Expected double value of 10.15", configGetDoubleValue(config, "gmsec-value", status) == 10.15);
	testCheckBool("Expected status with no error", statusHasError(status) == GMSEC_FALSE);

	configGetDoubleValue(config, "mw-id", status);
	testCheckBool("Expected status with error", statusHasError(status) == GMSEC_TRUE);

	configGetDoubleValue(config, NULL, status);
	testCheckBool("Expected status with error", statusHasError(status) == GMSEC_TRUE);

	configDestroy(&config);
	statusDestroy(&status);
}


void test_configGetDoubleValueOrDefault()
{
	GMSEC_INFO("Test configGetDoubleValueOrDefault()");

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreateUsingData("gmsec-value=10.15 mw-id=bolt", KEY_VALUE_DATA, status);

	testCheckBool("Expected double value of 10.15", configGetDoubleValueOrDefault(config, "gmsec-value", 1.234, status) == 10.15);
	testCheckBool("Expected status with no error", statusHasError(status) == GMSEC_FALSE);

	testCheckBool("Expected integer value of 20.22", configGetDoubleValueOrDefault(config, "missing", 20.22, status) == 20.22);
	testCheckBool("Expected status with no error", statusHasError(status) == GMSEC_FALSE);

	testCheckBool("Expected integer value of 30.33", configGetDoubleValueOrDefault(config, "mw-id", 30.33, status) == 30.33);
	testCheckBool("Expected status with no error", statusHasError(status) == GMSEC_FALSE);

	testCheckBool("Expected integer value of 10.11", configGetDoubleValueOrDefault(config, "", 10.11, status) == 10.11);
	testCheckBool("Expected status with no error", statusHasError(status) == GMSEC_FALSE);

	testCheckBool("Expected integer value of 10.11", configGetDoubleValueOrDefault(config, NULL, 10.11, status) == 10.11);
	testCheckBool("Expected status with no error", statusHasError(status) == GMSEC_FALSE);

	configDestroy(&config);
	statusDestroy(&status);
}


void test_configClear()
{
	GMSEC_INFO("Test configClear()");

	const char* name  = NULL;
	const char* value = NULL;

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreateUsingData("mw-id=bolt server=localhost", KEY_VALUE_DATA, status);

	configClear(NULL, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	configClear(config, status);
	testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);

	configGetNext(config, &name, &value, status);
	testCheckBool("Error expected", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Expected no name", name == NULL);
	testCheckBool("Expected no value", value == NULL);

	configGetFirst(config, &name, &value, status);
	testCheckBool("Error expected", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Expected no name", name == NULL);
	testCheckBool("Expected no value", value == NULL);

	configDestroy(&config);
	statusDestroy(&status);
}


void test_configMerge()
{
	GMSEC_INFO("Test configMerge()");

	const char* name  = NULL;
	const char* value = NULL;

	GMSEC_Status status  = statusCreate();
	GMSEC_Config config1 = configCreateUsingData("mw-id=bolt", KEY_VALUE_DATA, NULL);
	GMSEC_Config config2 = configCreateUsingData("mw-id=bolt", KEY_VALUE_DATA, NULL);
	GMSEC_Config config3 = configCreateUsingData("mw-id=mb server=localhost", KEY_VALUE_DATA, NULL);

	// Merge w/ overwrite
	configMerge(config1, config3, GMSEC_TRUE, status);
	testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);

	configGetFirst(config1, &name, &value, status);
	testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected entry name", strcompare(name, "mw-id") == 0);
	testCheckBool("Unexpected entry value", strcompare(value, "mb") == 0);

	configGetNext(config1, &name, &value, status);
	testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected entry name", strcompare(name, "server") == 0);
	testCheckBool("Unexpected entry value", strcompare(value, "localhost") == 0);

	configGetNext(config1, &name, &value, status);
	testCheckBool("Expected error", statusHasError(status) == GMSEC_TRUE);

	// Merge w/ no overwrite
	configMerge(config2, config3, GMSEC_FALSE, status);
	testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);

	configGetFirst(config2, &name, &value, status);
	testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected entry name", strcompare(name, "mw-id") == 0);
	testCheckBool("Unexpected entry value", strcompare(value, "bolt") == 0);

	configGetNext(config2, &name, &value, status);
	testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected entry name", strcompare(name, "server") == 0);
	testCheckBool("Unexpected entry value", strcompare(value, "localhost") == 0);

	configGetNext(config2, &name, &value, status);
	testCheckBool("Expected error", statusHasError(status) == GMSEC_TRUE);

	// NULL config target
	configMerge(NULL, config3, GMSEC_TRUE, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	// NULL config other
	configMerge(config1, NULL, GMSEC_TRUE, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	configDestroy(&config1);
	configDestroy(&config2);
	configDestroy(&config3);
	statusDestroy(&status);
}


void test_configToXML()
{
	GMSEC_INFO("Test configToXML()");

	const char* xml = "<CONFIG>\n"
	                  "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
	                  "\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
	                  "</CONFIG>";

	GMSEC_Config config = configCreateUsingData("mw-id=bolt server=localhost", KEY_VALUE_DATA, NULL);

	testCheckBool("Unexpected XML content", strcompare(configToXML(config, NULL), xml) == 0);

	configDestroy(&config);
}


void test_configFromXML()
{
	GMSEC_INFO("Test configFromXML()");

	const char* xml = "<CONFIG>\n"
	                  "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
	                  "\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
	                  "</CONFIG>";

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreate();

	configFromXML(config, xml, status);

	testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected XML content", strcompare(configToXML(config, NULL), xml) == 0);

	configDestroy(&config);
	statusDestroy(&status);
}


void test_configGetFromFile()
{
	GMSEC_INFO("Test configGetFromFile()");

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = NULL;
	char         filename[OUT_STR_LEN];

	/* Nominal test */
	testGetDataFile("T002_Config_good.xml", filename);
	config = configGetFromFile(filename, "Bolt", status);
	testCheckBool("Unexpected Config handle", config != NULL);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	configDestroy(&config);

	config = configGetFromFile(filename, "ActiveMQ", status);
	testCheckBool("Unexpected Config handle", config != NULL);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	configDestroy(&config);

	config = configGetFromFile(filename, NULL, status);
	testCheckBool("Unexpected Config handle", config != NULL);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	configDestroy(&config);

	/* Off-nominal test */
	testGetDataFile("T002_Config_good.xml", filename);
	config = configGetFromFile(filename, "FooBar", status);
	testCheckBool("Unexpected Config handle", config == NULL);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);

	testGetDataFile("T002_Config_bad.xml", filename);
	config = configGetFromFile(filename, "Bolt", status);
	testCheckBool("Unexpected Config handle", config == NULL);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_configToJSON()
{
	GMSEC_INFO("Test configToJSON()");

    const char* json = "{\"CONFIG\":{\"PARAMETER\":[{\"NAME\":\"mw-id\",\"VALUE\":\"bolt\"},{\"NAME\":\"server\",\"VALUE\":\"localhost\"}]}}";

	GMSEC_Config config = configCreateUsingData("mw-id=bolt server=localhost", KEY_VALUE_DATA, NULL);

	testCheckBool("Unexpected JSON content", strcompare(configToJSON(config, NULL), json) == 0);

	configDestroy(&config);
}


int test_Config()
{
	testSetDescription("Test Config");

	test_configCreate();
	test_configCreateWithArgs();
	test_configCreateUsingData();
	test_configCreateCopy();
	test_configDestroy();
	test_configAddValue();
	test_configClearValue();
	test_configGetValue();
	test_configGetValueOrDefault();
	test_configGetBooleanValue();
	test_configGetBooleanValueOrDefault();
	test_configGetIntegerValue();
	test_configGetIntegerValueOrDefault();
	test_configGetDoubleValue();
	test_configGetDoubleValueOrDefault();
	test_configClear();
	test_configGetFirst();
	test_configGetNext();
	test_configMerge();
	test_configToXML();
	test_configFromXML();
	test_configGetFromFile();
	test_configToJSON();

	return 0;
}
TEST_DRIVER(test_Config)
