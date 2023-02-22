#include "TestCase.h"


void verifyHeartbeatMessage(GMSEC_Config config, GMSEC_U16 expectedPubRate);


void test_heartbeatGeneratorCreate()
{
	GMSEC_INFO("Test heartbeatGeneratorCreate()");

	GMSEC_Status status = statusCreate();

	//o Nominal test
	{
		GMSEC_Config             config = configCreateCopy(testGetConfig(), NULL);
		GMSEC_HeartbeatGenerator hbgen  = heartbeatGeneratorCreate(config, (GMSEC_U16) 5, status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		testRequireBool("Heartbeat Generator handle is NULL", hbgen != NULL);
		testCheckBool("Heartbeat Generator should not be running", heartbeatGeneratorIsRunning(hbgen, NULL) == GMSEC_FALSE);

		heartbeatGeneratorDestroy(&hbgen);
		configDestroy(&config);
	}

	//o Off-nominal test(s)
	{
		// NULL config
		GMSEC_HeartbeatGenerator hbgen = heartbeatGeneratorCreate(NULL, (GMSEC_U16) 5, status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testRequireBool("Heartbeat Generator handle is NULL", hbgen == NULL);
	}

	statusDestroy(&status);
}


void test_heartbeatGeneratorCreateWithFields()
{
	GMSEC_INFO("Test heartbeatGeneratorCreateWithFields()");

	GMSEC_Status status = statusCreate();
	GMSEC_Field  fields[8];

	fields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	fields[1] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST-ID", GMSEC_TRUE, NULL);
	fields[2] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[3] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	fields[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	fields[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	fields[7] = stringFieldCreate("COMPONENT", "T012", GMSEC_TRUE, NULL);

	//o Nominal test (fields provided)
	{
		GMSEC_Config             config = configCreateCopy(testGetConfig(), NULL);
		GMSEC_HeartbeatGenerator hbgen  = heartbeatGeneratorCreateWithFields(config, (GMSEC_U16) 5, fields, sizeof(fields)/sizeof(GMSEC_Field), status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		testRequireBool("Heartbeat Generator handle is NULL", hbgen != NULL);
		testCheckBool("Heartbeat Generator should not be running", heartbeatGeneratorIsRunning(hbgen, NULL) == GMSEC_FALSE);

		heartbeatGeneratorDestroy(&hbgen);
		configDestroy(&config);
	}

	//o Off-nominal test(s)
	{
		// NULL config
		GMSEC_HeartbeatGenerator hbgen = heartbeatGeneratorCreateWithFields(NULL, (GMSEC_U16) 5, fields, sizeof(fields)/sizeof(GMSEC_Field), status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testRequireBool("Heartbeat Generator handle is NULL", hbgen == NULL);
	}

	{
		// No fields provided
		GMSEC_Config             config = configCreateCopy(testGetConfig(), NULL);
		GMSEC_HeartbeatGenerator hbgen = heartbeatGeneratorCreateWithFields(config, (GMSEC_U16) 5, NULL, sizeof(fields)/sizeof(GMSEC_Field), status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testRequireBool("Heartbeat Generator handle is NULL", hbgen == NULL);

		configDestroy(&config);
	}

	int i;
	for (i = 0; i < sizeof(fields)/sizeof(GMSEC_Field); ++i)
	{
		fieldDestroy(&fields[i]);
	}
	statusDestroy(&status);
}


void test_heartbeatGeneratorDestroy()
{
	GMSEC_INFO("Test heartbeatGeneratorDestroy()");

	GMSEC_Config config = configCreateCopy(testGetConfig(), NULL);
	GMSEC_Field  fields[8];

	fields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	fields[1] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST-ID", GMSEC_TRUE, NULL);
	fields[2] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[3] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	fields[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	fields[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	fields[7] = stringFieldCreate("COMPONENT", "T012", GMSEC_TRUE, NULL);

	GMSEC_HeartbeatGenerator hbgen = heartbeatGeneratorCreateWithFields(config, (GMSEC_U16) 5, fields, sizeof(fields)/sizeof(GMSEC_Field), NULL);

	testRequireBool("Heartbeat Generator handle is NULL", hbgen != NULL);

	heartbeatGeneratorDestroy(&hbgen);

	testRequireBool("Heartbeat Generator handle is not NULL", hbgen == NULL);

	int i;
	for (i = 0; i < sizeof(fields)/sizeof(GMSEC_Field); ++i)
	{
		fieldDestroy(&fields[i]);
	}
	configDestroy(&config);
}


void test_heartbeatGeneratorStart()
{
	GMSEC_INFO("Test heartbeatGeneratorStart()");

	GMSEC_Status status  = statusCreate();
	GMSEC_Config config  = testGetConfig();
	GMSEC_U16    pubRate = (GMSEC_U16) 1;
	GMSEC_Field  fields[8];

	fields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	fields[1] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST-ID", GMSEC_TRUE, NULL);
	fields[2] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[3] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	fields[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	fields[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	fields[7] = stringFieldCreate("COMPONENT", "T012", GMSEC_TRUE, NULL);

	GMSEC_HeartbeatGenerator hbgen = heartbeatGeneratorCreateWithFields(config, pubRate, fields, sizeof(fields)/sizeof(GMSEC_Field), NULL);

	/* To ensure this heartbeat message is unique, we set the COMPONENT field */
	char tmp[OUT_STR_LEN];
	GMSEC_Field component = stringFieldCreate("COMPONENT", testGetUniqueComponent(tmp), GMSEC_TRUE, NULL);
	heartbeatGeneratorSetField(hbgen, component, NULL);
	fieldDestroy(&component);

	heartbeatGeneratorStart(hbgen, status);
	testCheckBool("Heartbeat Generator should be running", heartbeatGeneratorIsRunning(hbgen, NULL) == GMSEC_TRUE);

	verifyHeartbeatMessage(config, pubRate);

	// Off-nominal
	heartbeatGeneratorStart(NULL, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	int i;
	for (i = 0; i < sizeof(fields)/sizeof(GMSEC_Field); ++i)
	{
		fieldDestroy(&fields[i]);
	}
	heartbeatGeneratorDestroy(&hbgen);
	statusDestroy(&status);
}


void test_heartbeatGeneratorStop()
{
	GMSEC_INFO("Test heartbeatGeneratorStop()");

	GMSEC_Status status  = statusCreate();
	GMSEC_Config config  = testGetConfig();
	GMSEC_U16    pubRate = (GMSEC_U16) 1;
	GMSEC_Field  fields[8];

	fields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	fields[1] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST-ID", GMSEC_TRUE, NULL);
	fields[2] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[3] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	fields[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	fields[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	fields[7] = stringFieldCreate("COMPONENT", "T012", GMSEC_TRUE, NULL);

	GMSEC_HeartbeatGenerator hbgen = heartbeatGeneratorCreateWithFields(config, pubRate, fields, sizeof(fields)/sizeof(GMSEC_Field), NULL);

	/* To ensure this heartbeat message is unique, we set the COMPONENT field */
	char tmp[OUT_STR_LEN];
	GMSEC_Field component = stringFieldCreate("COMPONENT", testGetUniqueComponent(tmp), GMSEC_TRUE, NULL);
	heartbeatGeneratorSetField(hbgen, component, NULL);
	fieldDestroy(&component);

	heartbeatGeneratorStart(hbgen, NULL);
	testCheckBool("Heartbeat Generator should be running", heartbeatGeneratorIsRunning(hbgen, NULL) == GMSEC_TRUE);

	// Nominal test
	heartbeatGeneratorStop(hbgen, NULL);
	testCheckBool("Heartbeat Generator should not be running", heartbeatGeneratorIsRunning(hbgen, NULL) == GMSEC_FALSE);

	/* Allow time for the HB-gen thread to really stop */
	timeUtilMillisleep(2000);

	// Off-nominal test
	heartbeatGeneratorStop(NULL, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	int i;
	for (i = 0; i < sizeof(fields)/sizeof(GMSEC_Field); ++i)
	{
		fieldDestroy(&fields[i]);
	}
	heartbeatGeneratorDestroy(&hbgen);
	statusDestroy(&status);
}


void test_heartbeatGeneratorIsRunning()
{
	GMSEC_INFO("Test heartbeatGeneratorIsRunning()");

	GMSEC_Status status = statusCreate();

	// Off-nominal tests (nominal are tested elsewhere)
	{
		GMSEC_BOOL isRunning = heartbeatGeneratorIsRunning(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected isRunning to be false", isRunning == GMSEC_FALSE);
	}

	statusDestroy(&status);
}


void test_heartbeatGeneratorChangePublishRate()
{
	GMSEC_INFO("Test heartbeatGeneratorChangePublishRate()");

	GMSEC_Status status  = statusCreate();
	GMSEC_Config config  = testGetConfig();
	GMSEC_U16    pubRate = (GMSEC_U16) 1;
	GMSEC_Field  fields[8];

	fields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	fields[1] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST-ID", GMSEC_TRUE, NULL);
	fields[2] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[3] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	fields[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	fields[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	fields[7] = stringFieldCreate("COMPONENT", "T012", GMSEC_TRUE, NULL);

	GMSEC_HeartbeatGenerator hbgen = heartbeatGeneratorCreateWithFields(config, pubRate, fields, sizeof(fields)/sizeof(GMSEC_Field), status);

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	/* To ensure this heartbeat message is unique, we set the COMPONENT field */
	char tmp[OUT_STR_LEN];
	GMSEC_Field component = stringFieldCreate("COMPONENT", testGetUniqueComponent(tmp), GMSEC_TRUE, NULL);
	heartbeatGeneratorSetField(hbgen, component, NULL);
	fieldDestroy(&component);

	heartbeatGeneratorStart(hbgen, NULL);

	// Nominal test
	heartbeatGeneratorChangePublishRate(hbgen, pubRate * 2, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	verifyHeartbeatMessage(config, pubRate * 2);

	// Off-nominal test
	heartbeatGeneratorChangePublishRate(NULL, pubRate * 2, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	int i;
	for (i = 0; i < sizeof(fields)/sizeof(GMSEC_Field); ++i)
	{
		fieldDestroy(&fields[i]);
	}
	heartbeatGeneratorDestroy(&hbgen);
	statusDestroy(&status);
}


void test_heartbeatGeneratorSetField()
{
	GMSEC_INFO("Test heartbeatGeneratorSetField()");

	GMSEC_Status status  = statusCreate();
	GMSEC_Config config  = testGetConfig();
	GMSEC_U16    pubRate = (GMSEC_U16) 1;
	GMSEC_Field  fields[8];

	fields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	fields[1] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST-ID", GMSEC_TRUE, NULL);
	fields[2] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[3] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	fields[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	fields[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	fields[7] = stringFieldCreate("COMPONENT", "T012", GMSEC_TRUE, NULL);

	GMSEC_HeartbeatGenerator hbgen = heartbeatGeneratorCreateWithFields(config, pubRate, fields, sizeof(fields)/sizeof(GMSEC_Field), status);

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	/* To ensure this heartbeat message is unique, we set the COMPONENT field */
	char tmp[OUT_STR_LEN];
	GMSEC_Field component = stringFieldCreate("COMPONENT", testGetUniqueComponent(tmp), GMSEC_TRUE, NULL);
	heartbeatGeneratorSetField(hbgen, component, NULL);
	fieldDestroy(&component);

	heartbeatGeneratorStart(hbgen, NULL);

	// Nominal tests
	{
		GMSEC_Field newPubRate = u16FieldCreate("PUB-RATE", pubRate * 2, GMSEC_FALSE, NULL);
		heartbeatGeneratorSetField(hbgen, newPubRate, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		verifyHeartbeatMessage(config, pubRate * 2);
		fieldDestroy(&newPubRate);

		newPubRate = i32FieldCreate("PUB-RATE", (GMSEC_I32) pubRate * 3, GMSEC_FALSE, NULL);
		heartbeatGeneratorSetField(hbgen, newPubRate, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		verifyHeartbeatMessage(config, pubRate * 3);
		fieldDestroy(&newPubRate);

		newPubRate = stringFieldCreate("PUB-RATE", "2", GMSEC_FALSE, NULL);
		heartbeatGeneratorSetField(hbgen, newPubRate, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		verifyHeartbeatMessage(config, pubRate * 2);
		fieldDestroy(&newPubRate);
	}

	// Off-nominal tests
	{
		// HeartbeatGenerator is NULL
		GMSEC_Field newPubRate = u16FieldCreate("PUB-RATE", pubRate * 2, GMSEC_FALSE, NULL);
		heartbeatGeneratorSetField(NULL, newPubRate, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		fieldDestroy(&newPubRate);

		heartbeatGeneratorSetField(hbgen, NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// PUB-RATE is negative
		GMSEC_Field newPubRate = i16FieldCreate("PUB-RATE", -1, GMSEC_FALSE, NULL);
		heartbeatGeneratorSetField(hbgen, newPubRate, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		fieldDestroy(&newPubRate);
	}

	int i;
	for (i = 0; i < sizeof(fields)/sizeof(GMSEC_Field); ++i)
	{
		fieldDestroy(&fields[i]);
	}
	heartbeatGeneratorDestroy(&hbgen);
	statusDestroy(&status);
}


int test_HeartbeatGenerator()
{
	testSetDescription("Test HeartbeatGenerator");

	test_heartbeatGeneratorCreate();
	test_heartbeatGeneratorCreateWithFields();
	test_heartbeatGeneratorDestroy();
	test_heartbeatGeneratorStart();
	test_heartbeatGeneratorStop();
	test_heartbeatGeneratorIsRunning();
	test_heartbeatGeneratorChangePublishRate();
	test_heartbeatGeneratorSetField();

	return 0;
}
TEST_DRIVER(test_HeartbeatGenerator)


void verifyHeartbeatMessage(GMSEC_Config config, GMSEC_U16 expectedPubRate)
{
	GMSEC_Status     status = statusCreate();
	GMSEC_Connection conn   = connectionCreate(config, status);
	char             topic[OUT_STR_LEN];
	char             tmp[OUT_STR_LEN];
	double           t1 = 0, t2;
	int              i;

	// Ensure we have a connection object
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	printToString(topic, "C2MS.*.*.*.*.*.MSG.HB.%s.+", testGetUniqueComponent(tmp));

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
			testCheckBool("Unexpected MESSAGE-SUBTYPE", strcompare(messageGetStringValue(msg, "MESSAGE-SUBTYPE", NULL), "HB") == 0);
			testCheckBool("Unexpected PUB-RATE", messageGetU32Value(msg, "PUB-RATE", NULL) == (GMSEC_U32) expectedPubRate);

			messageDestroy(&msg);
		}
		else
		{
			testCheckBool("Failed to receive heartbeat message", GMSEC_FALSE);
		}
	}

	connectionDestroy(&conn);
	statusDestroy(&status);
}
