#include "TestCase.h"


void verifyResourceMessage(GMSEC_Config config, GMSEC_U16 expectedPubRate);


void test_resourceGeneratorCreate()
{
	GMSEC_INFO("Test resourceGeneratorCreate()");

	GMSEC_Status status = statusCreate();

	//o Nominal test
	{
		GMSEC_Config            config   = configCreateCopy(testGetConfig(), NULL);
		GMSEC_ResourceGenerator rsrc_gen = resourceGeneratorCreate(config, (GMSEC_U16) 5, (GMSEC_U16) 1, (GMSEC_U16) 10, status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		testRequireBool("Resource Generator handle is NULL", rsrc_gen != NULL);
		testCheckBool("Resource Generator should not be running", resourceGeneratorIsRunning(rsrc_gen, NULL) == GMSEC_FALSE);

		resourceGeneratorDestroy(&rsrc_gen);
		configDestroy(&config);
	}

	//o Off-nominal test
	{
		// NULL config
		GMSEC_ResourceGenerator rsrc_gen = resourceGeneratorCreate(NULL, (GMSEC_U16) 5, (GMSEC_U16) 1, (GMSEC_U16) 10, status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testRequireBool("Resource Generator handle is NULL", rsrc_gen == NULL);
	}

	statusDestroy(&status);
}


void test_resourceGeneratorCreateWithFields()
{
	GMSEC_INFO("Test resourceGeneratorCreateWithFields()");

	GMSEC_Status status = statusCreate();
	GMSEC_Field  fields[8];

	fields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	fields[1] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST-ID", GMSEC_TRUE, NULL);
	fields[2] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[3] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	fields[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	fields[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	fields[7] = stringFieldCreate("COMPONENT", "T016", GMSEC_TRUE, NULL);

	//o Nominal test (fields provided)
	{
		GMSEC_Config            config   = configCreateCopy(testGetConfig(), NULL);
		GMSEC_ResourceGenerator rsrc_gen = resourceGeneratorCreateWithFields(config, (GMSEC_U16) 5, (GMSEC_U16) 1, (GMSEC_U16) 10, fields, sizeof(fields)/sizeof(GMSEC_Field), status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		testRequireBool("Resource Generator handle is NULL", rsrc_gen != NULL);
		testCheckBool("Resource Generator should not be running", resourceGeneratorIsRunning(rsrc_gen, NULL) == GMSEC_FALSE);

		resourceGeneratorDestroy(&rsrc_gen);
		configDestroy(&config);
	}

	//o Nominal test (no fields provided)
	{
		GMSEC_Config            config   = configCreateCopy(testGetConfig(), NULL);
		GMSEC_ResourceGenerator rsrc_gen = resourceGeneratorCreateWithFields(config, (GMSEC_U16) 5, (GMSEC_U16) 1, (GMSEC_U16) 10, NULL, 0, status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		testRequireBool("Resource Generator handle is NULL", rsrc_gen != NULL);
		testCheckBool("Resource Generator should not be running", resourceGeneratorIsRunning(rsrc_gen, NULL) == GMSEC_FALSE);

		resourceGeneratorDestroy(&rsrc_gen);
		configDestroy(&config);
	}

	//o Off-nominal test
	{
		// NULL config
		GMSEC_ResourceGenerator rsrc_gen = resourceGeneratorCreateWithFields(NULL, (GMSEC_U16) 5, (GMSEC_U16) 1, (GMSEC_U16) 10, fields, 3, status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testRequireBool("Resource Generator handle is NULL", rsrc_gen == NULL);
	}

	{
		// NULL fields, yet number of fields is 3
		GMSEC_Config            config   = configCreateCopy(testGetConfig(), NULL);
		GMSEC_ResourceGenerator rsrc_gen = resourceGeneratorCreateWithFields(config, (GMSEC_U16) 5, (GMSEC_U16) 1, (GMSEC_U16) 10, NULL, 3, status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testRequireBool("Resource Generator handle is NULL", rsrc_gen == NULL);

		configDestroy(&config);
	}

	{
		// bogus middleware
		GMSEC_Config            config   = configCreateUsingData("mw-id=bogus", KEY_VALUE_DATA, NULL);
		GMSEC_ResourceGenerator rsrc_gen = resourceGeneratorCreateWithFields(config, (GMSEC_U16) 5, (GMSEC_U16) 1, (GMSEC_U16) 10, fields, 3, status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testRequireBool("Resource Generator handle is NULL", rsrc_gen == NULL);

		configDestroy(&config);
	}

	int i;
	for (i = 0; i < sizeof(fields)/sizeof(GMSEC_Field); ++i)
	{
		fieldDestroy(&fields[i]);
	}
	statusDestroy(&status);
}


void test_resourceGeneratorDestroy()
{
	GMSEC_INFO("Test resourceGeneratorDestroy()");

	GMSEC_Config config = configCreateCopy(testGetConfig(), NULL);
	GMSEC_Field  fields[8];

	fields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	fields[1] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST-ID", GMSEC_TRUE, NULL);
	fields[2] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[3] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	fields[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	fields[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	fields[7] = stringFieldCreate("COMPONENT", "T016", GMSEC_TRUE, NULL);

	GMSEC_ResourceGenerator rsrc_gen = resourceGeneratorCreateWithFields(config, (GMSEC_U16) 5, (GMSEC_U16) 1, (GMSEC_U16) 10, fields, sizeof(fields)/sizeof(GMSEC_Field), NULL);

	testRequireBool("Resource Generator handle is NULL", rsrc_gen != NULL);

	resourceGeneratorDestroy(&rsrc_gen);

	testRequireBool("Resource Generator handle is not NULL", rsrc_gen == NULL);

	int i;
	for (i = 0; i < sizeof(fields)/sizeof(GMSEC_Field); ++i)
	{
		fieldDestroy(&fields[i]);
	}
	configDestroy(&config);
}


void test_resourceGeneratorStart()
{
	GMSEC_INFO("Test resourceGeneratorStart()");

	GMSEC_Status status   = statusCreate();
	GMSEC_Config config   = testGetConfig();
	GMSEC_U16    pubRate  = (GMSEC_U16) 1;
	GMSEC_Field  fields[8];

	fields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	fields[1] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST-ID", GMSEC_TRUE, NULL);
	fields[2] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[3] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	fields[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	fields[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	fields[7] = stringFieldCreate("COMPONENT", "T016", GMSEC_TRUE, NULL);

	GMSEC_ResourceGenerator rsrc_gen = resourceGeneratorCreateWithFields(config, (GMSEC_U16) pubRate, (GMSEC_U16) 1, (GMSEC_U16) 10, fields, sizeof(fields)/sizeof(GMSEC_Field), NULL);

	/* To ensure this resource message is unique, we set the COMPONENT field */
	char tmp[OUT_STR_LEN];
	GMSEC_Field component = stringFieldCreate("COMPONENT", testGetUniqueComponent(tmp), GMSEC_TRUE, NULL);
	resourceGeneratorSetField(rsrc_gen, component, NULL);
	fieldDestroy(&component);

	resourceGeneratorStart(rsrc_gen, status);
	testCheckBool("Resource Generator should be running", resourceGeneratorIsRunning(rsrc_gen, NULL) == GMSEC_TRUE);

	verifyResourceMessage(config, pubRate);

	// Off-nominal
	{
		GMSEC_Config            config   = configCreateUsingData("mw-id=bogus", KEY_VALUE_DATA, NULL);
		GMSEC_ResourceGenerator rsrc_gen = resourceGeneratorCreate(config, (GMSEC_U16) 5, (GMSEC_U16) 1, (GMSEC_U16) 10, status);

		resourceGeneratorStart(rsrc_gen, status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testRequireBool("Resource Generator handle is NULL", rsrc_gen == NULL);

		configDestroy(&config);
	}

	resourceGeneratorStart(NULL, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	int i;
	for (i = 0; i < sizeof(fields)/sizeof(GMSEC_Field); ++i)
	{
		fieldDestroy(&fields[i]);
	}
	resourceGeneratorDestroy(&rsrc_gen);
	statusDestroy(&status);
}


void test_resourceGeneratorStop()
{
	GMSEC_INFO("Test resourceGeneratorStop()");

	GMSEC_Status status   = statusCreate();
	GMSEC_Config config   = testGetConfig();
	GMSEC_U16    pubRate  = (GMSEC_U16) 1;
	GMSEC_Field  fields[8];

	fields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	fields[1] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST-ID", GMSEC_TRUE, NULL);
	fields[2] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[3] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	fields[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	fields[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	fields[7] = stringFieldCreate("COMPONENT", "T016", GMSEC_TRUE, NULL);

	GMSEC_ResourceGenerator rsrc_gen = resourceGeneratorCreateWithFields(config, (GMSEC_U16) pubRate, (GMSEC_U16) 1, (GMSEC_U16) 10, fields, sizeof(fields)/sizeof(GMSEC_Field), NULL);

	/* To ensure this resource message is unique, we set the COMPONENT field */
	char tmp[OUT_STR_LEN];
	GMSEC_Field component = stringFieldCreate("COMPONENT", testGetUniqueComponent(tmp), GMSEC_TRUE, NULL);
	resourceGeneratorSetField(rsrc_gen, component, NULL);
	fieldDestroy(&component);

	resourceGeneratorStart(rsrc_gen, NULL);
	testCheckBool("Resource Generator should be running", resourceGeneratorIsRunning(rsrc_gen, NULL) == GMSEC_TRUE);

	// Nominal test
	resourceGeneratorStop(rsrc_gen, NULL);
	testCheckBool("Resource Generator should not be running", resourceGeneratorIsRunning(rsrc_gen, NULL) == GMSEC_FALSE);

	/* Allow time for the RSRC-gen thread to really stop */
	timeUtilMillisleep(2000);

	// Off-nominal test
	resourceGeneratorStop(NULL, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	int i;
	for (i = 0; i < sizeof(fields)/sizeof(GMSEC_Field); ++i)
	{
		fieldDestroy(&fields[i]);
	}
	resourceGeneratorDestroy(&rsrc_gen);
	statusDestroy(&status);
}


void test_resourceGeneratorIsRunning()
{
	GMSEC_INFO("Test resourceGeneratorIsRunning()");

	GMSEC_Status status = statusCreate();

	// Off-nominal test
	{
		GMSEC_BOOL isRunning = resourceGeneratorIsRunning(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected isRunning to be false", isRunning == GMSEC_FALSE);
	}

	statusDestroy(&status);
}


void test_resourceGeneratorSetField()
{
	GMSEC_INFO("Test resourceGeneratorSetField()");

	GMSEC_Status status   = statusCreate();
	GMSEC_Config config   = testGetConfig();
	GMSEC_U16    pubRate  = (GMSEC_U16) 1;
	GMSEC_Field  fields[8];

	fields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	fields[1] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST-ID", GMSEC_TRUE, NULL);
	fields[2] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[3] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	fields[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	fields[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	fields[7] = stringFieldCreate("COMPONENT", "T016", GMSEC_TRUE, NULL);

	GMSEC_ResourceGenerator rsrc_gen = resourceGeneratorCreateWithFields(config, (GMSEC_U16) pubRate, (GMSEC_U16) 1, (GMSEC_U16) 10, fields, sizeof(fields)/sizeof(GMSEC_Field), status);

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	/* To ensure this resource message is unique, we set the COMPONENT field */
	char tmp[OUT_STR_LEN];
	GMSEC_Field component = stringFieldCreate("COMPONENT", testGetUniqueComponent(tmp), GMSEC_TRUE, NULL);
	resourceGeneratorSetField(rsrc_gen, component, NULL);
	fieldDestroy(&component);

	resourceGeneratorStart(rsrc_gen, NULL);

	// Nominal tests
	{
		GMSEC_Field newPubRate = u16FieldCreate("PUB-RATE", pubRate * 2, GMSEC_FALSE, NULL);
		resourceGeneratorSetField(rsrc_gen, newPubRate, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		verifyResourceMessage(config, pubRate * 2);
		fieldDestroy(&newPubRate);

		newPubRate = i32FieldCreate("PUB-RATE", (GMSEC_I32) pubRate * 3, GMSEC_FALSE, NULL);
		resourceGeneratorSetField(rsrc_gen, newPubRate, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		verifyResourceMessage(config, pubRate * 3);
		fieldDestroy(&newPubRate);

		newPubRate = stringFieldCreate("PUB-RATE", "2", GMSEC_FALSE, NULL);
		resourceGeneratorSetField(rsrc_gen, newPubRate, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		verifyResourceMessage(config, pubRate * 2);
		fieldDestroy(&newPubRate);
	}

	// Off-nominal tests
	{
		// RSRC-Gen is NULL
		GMSEC_Field newPubRate = u16FieldCreate("PUB-RATE", pubRate * 2, GMSEC_FALSE, NULL);
		resourceGeneratorSetField(NULL, newPubRate, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		fieldDestroy(&newPubRate);
	}

	{
		// Field is NULL
		resourceGeneratorSetField(rsrc_gen, NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// PUB-RATE is negative
		GMSEC_Field newPubRate = i16FieldCreate("PUB-RATE", -1, GMSEC_FALSE, NULL);
		resourceGeneratorSetField(rsrc_gen, newPubRate, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		fieldDestroy(&newPubRate);
	}

	resourceGeneratorStop(rsrc_gen, NULL);

	timeUtilMillisleep(2000);

	int i;
	for (i = 0; i < sizeof(fields)/sizeof(GMSEC_Field); ++i)
	{
		fieldDestroy(&fields[i]);
	}
	resourceGeneratorDestroy(&rsrc_gen);
	statusDestroy(&status);
}


void test_createResourceMessage()
{
	GMSEC_INFO("Test createResourceMessage()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_Config         config  = testGetConfig();
	GMSEC_MessageFactory factory = messageFactoryCreateUsingConfig(config, status);

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_Field fields[8];
	fields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	fields[1] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST-ID", GMSEC_TRUE, NULL);
	fields[2] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[3] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	fields[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	fields[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	fields[7] = stringFieldCreate("COMPONENT", "T016", GMSEC_TRUE, NULL);

	messageFactorySetStandardFields(factory, fields, sizeof(fields)/sizeof(GMSEC_Field), NULL);

	GMSEC_U16 sampleInterval  = 1;
	GMSEC_U16 averageInterval = 10;

	GMSEC_Message rsrcMsg = createResourceMessage(factory, sampleInterval, averageInterval, status);

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testRequireBool("Have no resource message", rsrcMsg != NULL);
	testCheckBool("Unexpected MESSAGE-TYPE", strcompare(messageGetStringValue(rsrcMsg, "MESSAGE-TYPE", NULL), "MSG") == 0);
	testCheckBool("Unexpected MESSAGE-SUBTYPE", strcompare(messageGetStringValue(rsrcMsg, "MESSAGE-SUBTYPE", NULL), "RSRC") == 0);

	// Off-nominal test(s)
	{
		// NULL MessageFactory
		(void) createResourceMessage(NULL, sampleInterval, averageInterval, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// Average Interval is 0
		(void) createResourceMessage(factory, sampleInterval, 0, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	int i;
	for (i = 0; i < sizeof(fields)/sizeof(GMSEC_Field); ++i)
	{
		fieldDestroy(&fields[i]);
	}
	messageDestroy(&rsrcMsg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


int test_ResourceGenerator()
{
	testSetDescription("Test ResourceGenerator");

	test_resourceGeneratorCreate();
	test_resourceGeneratorCreateWithFields();
	test_resourceGeneratorDestroy();
	test_resourceGeneratorStart();
	test_resourceGeneratorStop();
	test_resourceGeneratorIsRunning();
	test_resourceGeneratorSetField();
	test_createResourceMessage();

	return 0;
}
TEST_DRIVER(test_ResourceGenerator)


void verifyResourceMessage(GMSEC_Config config, GMSEC_U16 expectedPubRate)
{
	GMSEC_Status     status = statusCreate();
	GMSEC_Connection conn   = connectionCreate(config, status);
    char             topic[OUT_STR_LEN];
    char             tmp[OUT_STR_LEN];
	double           t1 = 0, t2;
	int              i;

	// Ensure we have a connection object
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

    printToString(topic, "C2MS.*.*.*.*.*.MSG.RSRC.%s.+", testGetUniqueComponent(tmp));

	// Ensure we can connect to the bus
	connectionConnect(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	// Subscribe to receive messages
	connectionSubscribe(conn, topic, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	for (i = 0; i < 7; ++i)
	{
		// Ensure we are still connected
		GMSEC_Message msg = connectionReceive(conn, 5000, status);
		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		// Ignore the first few incoming messages (if any)
		if (i < 3)
		{
			if (msg != NULL) messageDestroy(&msg);
			continue;
		}

		if (msg != NULL)
		{
			if (t1 == 0)
			{
				t1 = timeUtilGetCurrentTime_s(NULL);
			}
			else
			{
				t2 = timeUtilGetCurrentTime_s(NULL);

				double delta = t2 - t1;
				if (delta < expectedPubRate)
				{
					delta = ((t2 - t1) * 10.0 + 0.5) / 10.0;
				}

				GMSEC_INFO("Expected rate is %u, delta is: %g", expectedPubRate, delta);

				testCheckBool("Unexpected publish rate", expectedPubRate == (GMSEC_U16)delta);

				t1 = t2;
			}

			testCheckBool("Unexpected MESSAGE-TYPE", strcompare(messageGetStringValue(msg, "MESSAGE-TYPE", NULL), "MSG") == 0);
			testCheckBool("Unexpected MESSAGE-SUBTYPE", strcompare(messageGetStringValue(msg, "MESSAGE-SUBTYPE", NULL), "RSRC") == 0);
			testCheckBool("Unexpected PUB-RATE", messageGetU32Value(msg, "PUB-RATE", NULL) == (GMSEC_U32) expectedPubRate);

			messageDestroy(&msg);
		}
		else
		{
			testCheckBool("Failed to receive resource message", GMSEC_FALSE);
		}
	}

	connectionDestroy(&conn);
	statusDestroy(&status);
}
