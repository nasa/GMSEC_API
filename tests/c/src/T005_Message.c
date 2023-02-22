#include "TestCase.h"


void test_messageCreate()
{
	GMSEC_INFO("Test messageCreate()");

	GMSEC_Message msg = messageCreate();

	testCheckBool("Expected non-null message handle", msg != NULL);

	messageDestroy(&msg);
}


void test_messageCreateCopy()
{
	GMSEC_INFO("Test messageCreateCopy()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);

	setStandardFields(factory);

	// Nominal test
	{
		GMSEC_Message msg1 = messageFactoryCreateMessage(factory, "HB", NULL);
		GMSEC_Message msg2 = messageCreateCopy(msg1, status);

		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected schema ID", strcompare(messageGetSchemaID(msg1, NULL), messageGetSchemaID(msg2, NULL)) == 0);
		testCheckBool("Unexpected version", messageGetVersion(msg1, NULL) == messageGetVersion(msg2, NULL));
		testCheckBool("Unexpected schema level", messageGetSchemaLevel(msg1, NULL) == messageGetSchemaLevel(msg2, NULL));
		testCheckBool("Copied message has unexpected XML content", strcompare(messageToXML(msg1, NULL), messageToXML(msg2, NULL)) == 0);
		testCheckBool("Copied message has unexpected JSON content", strcompare(messageToJSON(msg1, NULL), messageToJSON(msg2, NULL)) == 0);

		messageDestroy(&msg1);
		messageDestroy(&msg2);
	}

	// Off-nominal test
	{
		// Message is NULL
		GMSEC_Message msg = messageCreateCopy(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Did not expect to have a message", msg == NULL);
	}

	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageAcknowledge()
{
	GMSEC_INFO("Test messageAcknowledge()");

	GMSEC_Status  status = statusCreate();
	GMSEC_Message msg    = messageCreate();

	// Nominal test
	{
		messageAcknowledge(msg, status);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	// Off-nominal test
	{
		// Message is NULL
		messageAcknowledge(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	statusDestroy(&status);
}


void test_messageDestroy()
{
	GMSEC_INFO("Test messageDestroy()");

	GMSEC_Message msg = messageCreate();
	testCheckBool("Expected a message", msg != NULL);

	messageDestroy(&msg);
	testCheckBool("Expected message to be set to NULL", msg == NULL);
}


void test_messageGetSchemaID()
{
	GMSEC_INFO("Test messageGetSchemaID()");

	// Nominal test
	{
		GMSEC_MessageFactory factory = messageFactoryCreate(NULL);

		setStandardFields(factory);

		GMSEC_Message msg1 = messageFactoryCreateMessage(factory, "HB", NULL);
		GMSEC_Message msg2 = messageFactoryCreateMessage(factory, "MVAL", NULL);
		GMSEC_Message msg3 = messageFactoryCreateMessage(factory, "TLMFRAME", NULL);

		testCheckBool("Message 1 has unexpected schema ID", strcompare(messageGetSchemaID(msg1, NULL), "MSG.HB") == 0);
		testCheckBool("Message 2 has unexpected schema ID", strcompare(messageGetSchemaID(msg2, NULL), "MSG.MVAL") == 0);
		testCheckBool("Message 3 has unexpected schema ID", strcompare(messageGetSchemaID(msg3, NULL), "MSG.TLMFRAME") == 0);

		messageDestroy(&msg1);
		messageDestroy(&msg2);
		messageDestroy(&msg3);
		messageFactoryDestroy(&factory);
	}

	// Off-nominal test
	{
		GMSEC_Status status = statusCreate();

		const char* schemaID = messageGetSchemaID(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected id to be NULL", schemaID == NULL);

		statusDestroy(&status);
	}
}


void test_messageGetVersion()
{
	GMSEC_INFO("Test messageGetVersion()");

	// Nominal test
	{
		GMSEC_Config         config   = configCreateUsingData("gmsec-specification-version=201900", KEY_VALUE_DATA, NULL);
		GMSEC_MessageFactory factory1 = messageFactoryCreate(NULL);
		GMSEC_MessageFactory factory2 = messageFactoryCreateUsingConfig(config, NULL);
		GMSEC_Message        msg1     = messageFactoryCreateMessage(factory1, "HB", NULL);
		GMSEC_Message        msg2     = messageFactoryCreateMessage(factory2, "HB", NULL);

		testCheckBool("Message 1 has unexpected version", messageGetVersion(msg1, NULL) == GMSEC_MSG_SPEC_CURRENT);
		testCheckBool("Message 2 has unexpected version", messageGetVersion(msg2, NULL) == GMSEC_MSG_SPEC_2019_00);

		messageDestroy(&msg1);
		messageDestroy(&msg2);
		messageFactoryDestroy(&factory1);
		messageFactoryDestroy(&factory2);
		configDestroy(&config);
	}

	// Off-nominal test
	{
		GMSEC_Status status = statusCreate();

		unsigned int version = messageGetVersion(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected version to be 0", version == 0);

		statusDestroy(&status);
	}
}


void test_messageGetSchemaLevel()
{
	GMSEC_INFO("Test messageGetSchemaLevel()");

	// Nominal test
	{
		GMSEC_Config         config   = configCreateUsingData("gmsec-schema-level=2", KEY_VALUE_DATA, NULL);
		GMSEC_MessageFactory factory1 = messageFactoryCreate(NULL);
		GMSEC_MessageFactory factory2 = messageFactoryCreateUsingConfig(config, NULL);
		GMSEC_Message        msg1     = messageFactoryCreateMessage(factory1, "HB", NULL);
		GMSEC_Message        msg2     = messageFactoryCreateMessage(factory2, "HB", NULL);

		testCheckBool("Message 1 has unexpected schema level", messageGetSchemaLevel(msg1, NULL) == GMSEC_SCHEMA_LEVEL_0);
		testCheckBool("Message 2 has unexpected schema level", messageGetSchemaLevel(msg2, NULL) == GMSEC_SCHEMA_LEVEL_2);

		messageDestroy(&msg1);
		messageDestroy(&msg2);
		messageFactoryDestroy(&factory1);
		messageFactoryDestroy(&factory2);
		configDestroy(&config);
	}

	// Off-nominal test
	{
		// Message is NULL
		GMSEC_Status status = statusCreate();

		GMSEC_SchemaLevel level = messageGetSchemaLevel(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected schema level 0", level == GMSEC_SCHEMA_LEVEL_0);

		statusDestroy(&status);
	}
}


void test_messageIsCompliant()
{
	GMSEC_INFO("Test messageIsCompliant()");

	GMSEC_Status status = statusCreate();

	// Nominal test
	{
		GMSEC_MessageFactory factory1 = messageFactoryCreate(NULL);
		GMSEC_MessageFactory factory2 = messageFactoryCreate(NULL);

		setStandardFields(factory1);

		GMSEC_Message msg1 = messageFactoryCreateMessage(factory1, "HB", NULL);
		GMSEC_Message msg2 = messageFactoryCreateMessage(factory2, "HB", NULL);  // this message lacks standard fields

		messageIsCompliant(msg1, status);
		testCheckBool("Message 1 should be compliant", statusHasError(status) == GMSEC_FALSE);

		messageIsCompliant(msg2, status);
		testCheckBool("Message 2 should NOT be compliant", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Message 2 has unexpected error reason", strcontains(statusGet(status), "Message Validation Failed") == 0);

		messageDestroy(&msg1);
		messageDestroy(&msg2);
		messageFactoryDestroy(&factory1);
		messageFactoryDestroy(&factory2);
	}

	// Off-nominal test
	{
		// Message is NULL
		GMSEC_Status status = statusCreate();

		messageIsCompliant(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		statusDestroy(&status);
	}

	statusDestroy(&status);
}


void nominalValidator(const GMSEC_Message msg, GMSEC_Status status)
{
	statusReset(status);
}


void offNominalValidator(const GMSEC_Message msg, GMSEC_Status status)
{
	statusSet(status, GMSEC_MSG_ERROR, GMSEC_MESSAGE_FAILED_VALIDATION, "My validator is not happy with the message", 0);
}


void test_messageRegisterMessageValidator()
{
	GMSEC_INFO("Test messageRegisterMessageValidator()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);

	setStandardFields(factory);

	// Nominal test
	{
		GMSEC_Message msg1 = messageFactoryCreateMessage(factory, "HB", NULL);
		GMSEC_Message msg2 = messageFactoryCreateMessage(factory, "HB", NULL);

		messageRegisterMessageValidator(msg1, &nominalValidator, NULL);
		messageRegisterMessageValidator(msg2, &offNominalValidator, NULL);

		GMSEC_Message msg3 = messageCreateCopy(msg2, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		messageIsCompliant(msg1, status);
		testCheckBool("Message 1 should be compliant", statusHasError(status) == GMSEC_FALSE);

		messageIsCompliant(msg2, status);
		testCheckBool("Message 2 should NOT be compliant", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Message 2 has unexpected error reason", strcontains(statusGet(status), "My validator is not happy with the message") == 0);

		messageIsCompliant(msg3, status);
		testCheckBool("Message 3 should NOT be compliant", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Message 3 has unexpected error reason", strcontains(statusGet(status), "My validator is not happy with the message") == 0);

		messageDestroy(&msg1);
		messageDestroy(&msg2);
		messageDestroy(&msg3);
	}

	// Off-nominal tests
	{
		// Message is NULL
		messageRegisterMessageValidator(NULL, &nominalValidator, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// Attempt to register NULL message validator
		GMSEC_Message msg = messageCreate();
		messageRegisterMessageValidator(msg, NULL, status);
		testCheckBool("Registering NULL validator should yield an error", statusHasError(status) == GMSEC_TRUE);

		messageDestroy(&msg);
	}

	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageSetFieldValue()
{
	GMSEC_INFO("Test messageSetFieldValue()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(status);

	setStandardFields(factory);

	GMSEC_Message msg = messageFactoryCreateMessage(factory, "HB", NULL);

	// Nominal tests
	{
		//o Test with string value
		messageSetFieldValue(msg, "PUB-RATE", "5", status);
		testCheckBool("Unexpected error setting PUB-RATE using a string value", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("PUB-RATE has unexpected field type", messageGetFieldType(msg, "PUB-RATE", NULL) == GMSEC_U16_TYPE);
		testCheckBool("PUB-RATE has unexpected field value", messageGetI32Value(msg, "PUB-RATE", NULL) == 5);

		//o Test with GMSEC_I16 value
		messageSetFieldValueI16(msg, "PUB-RATE", (GMSEC_I16) 10, status);
		testCheckBool("Unexpected error setting PUB-RATE using a string value", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("PUB-RATE has unexpected field type", messageGetFieldType(msg, "PUB-RATE", NULL) == GMSEC_U16_TYPE);
		testCheckBool("PUB-RATE has unexpected field value", messageGetI32Value(msg, "PUB-RATE", NULL) == 10);

		//o Test with GMSEC_U16 value
		messageSetFieldValueU16(msg, "PUB-RATE", (GMSEC_U16) 10, status);
		testCheckBool("Unexpected error setting PUB-RATE using a string value", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("PUB-RATE has unexpected field type", messageGetFieldType(msg, "PUB-RATE", NULL) == GMSEC_U16_TYPE);
		testCheckBool("PUB-RATE has unexpected field value", messageGetI32Value(msg, "PUB-RATE", NULL) == 10);

		//o Test with GMSEC_I32 value
		messageSetFieldValueI32(msg, "PUB-RATE", 10, status);
		testCheckBool("Unexpected error setting PUB-RATE using a string value", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("PUB-RATE has unexpected field type", messageGetFieldType(msg, "PUB-RATE", NULL) == GMSEC_U16_TYPE);
		testCheckBool("PUB-RATE has unexpected field value", messageGetI32Value(msg, "PUB-RATE", NULL) == 10);

		//o Test with GMSEC_U32 value
		messageSetFieldValueU32(msg, "PUB-RATE", (GMSEC_U32) 20, status);
		testCheckBool("Unexpected error setting PUB-RATE using a string value", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("PUB-RATE has unexpected field type", messageGetFieldType(msg, "PUB-RATE", NULL) == GMSEC_U16_TYPE);
		testCheckBool("PUB-RATE has unexpected field value", messageGetI32Value(msg, "PUB-RATE", NULL) == 20);

		//o Test with GMSEC_I64 value
		messageSetFieldValueI64(msg, "PUB-RATE", (GMSEC_I64) 30, status);
		testCheckBool("Unexpected error setting PUB-RATE using a string value", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("PUB-RATE has unexpected field type", messageGetFieldType(msg, "PUB-RATE", NULL) == GMSEC_U16_TYPE);
		testCheckBool("PUB-RATE has unexpected field value", messageGetI32Value(msg, "PUB-RATE", NULL) == 30);

		//o Test with GMSEC_U64 value
		messageSetFieldValueU64(msg, "PUB-RATE", (GMSEC_U64) 40, status);
		testCheckBool("Unexpected error setting PUB-RATE using a string value", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("PUB-RATE has unexpected field type", messageGetFieldType(msg, "PUB-RATE", NULL) == GMSEC_U16_TYPE);
		testCheckBool("PUB-RATE has unexpected field value", messageGetI32Value(msg, "PUB-RATE", NULL) == 40);

		//o Test with GMSEC_F64 value
		messageSetFieldValueF64(msg, "PUB-RATE", 50.123, status);
		testCheckBool("Unexpected error setting PUB-RATE using a string value", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("PUB-RATE has unexpected field type", messageGetFieldType(msg, "PUB-RATE", NULL) == GMSEC_U16_TYPE);
		testCheckBool("PUB-RATE has unexpected field value", messageGetI32Value(msg, "PUB-RATE", NULL) == 50);

		//o Verify result returned is 'GMSEC_TRUE' when replacing a field
		testCheckBool("Expected return value of GMSEC_TRUE", messageSetFieldValue(msg, "PUB-RATE", "10", NULL) == GMSEC_TRUE);

		//o Verify result returned is 'GMSEC_FALSE' when adding a new field
		testCheckBool("Expected return value of GMSEC_FALSE", messageSetFieldValue(msg, "PUBLISH-RATE", "10", NULL) == GMSEC_FALSE);
	}

	// Off-nominal tests
	{
		// Message is NULL
		GMSEC_BOOL replaced = messageSetFieldValue(NULL, "PUB-RATE", "30", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);

		// Field name is NULL
		replaced = messageSetFieldValue(msg, NULL, "30", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);
	}

	{
		// Message is NULL
		GMSEC_BOOL replaced = messageSetFieldValueI16(NULL, "PUB-RATE", (GMSEC_I16) 30, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);

		// Field name is NULL
		replaced = messageSetFieldValueI16(msg, NULL, (GMSEC_I16) 30, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);
	}

	{
		// Message is NULL
		GMSEC_BOOL replaced = messageSetFieldValueU16(NULL, "PUB-RATE", (GMSEC_U16) 30, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);

		// Field name is NULL
		replaced = messageSetFieldValueU16(msg, NULL, (GMSEC_U16) 30, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);
	}

	{
		// Message is NULL
		GMSEC_BOOL replaced = messageSetFieldValueI32(NULL, "PUB-RATE", (GMSEC_I32) 30, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);

		// Field name is NULL
		replaced = messageSetFieldValueI32(msg, NULL, (GMSEC_I32) 30, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);
	}

	{
		// Message is NULL
		GMSEC_BOOL replaced = messageSetFieldValueU32(NULL, "PUB-RATE", (GMSEC_U32) 30, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);

		// Field name is NULL
		replaced = messageSetFieldValueU32(msg, NULL, (GMSEC_U32) 30, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);
	}

	{
		// Message is NULL
		GMSEC_BOOL replaced = messageSetFieldValueI64(NULL, "PUB-RATE", (GMSEC_I64) 30, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);

		// Field name is NULL
		replaced = messageSetFieldValueI64(msg, NULL, (GMSEC_I64) 30, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);
	}

	{
		// Message is NULL
		GMSEC_BOOL replaced = messageSetFieldValueU64(NULL, "PUB-RATE", (GMSEC_U64) 30, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);

		// Field name is NULL
		replaced = messageSetFieldValueU64(msg, NULL, (GMSEC_U64) 30, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);
	}

	{
		// Message is NULL
		GMSEC_BOOL replaced = messageSetFieldValueF64(NULL, "PUB-RATE", (GMSEC_F64) 30, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);

		// Field name is NULL
		replaced = messageSetFieldValueF64(msg, NULL, (GMSEC_F64) 30, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageSetConfig()
{
	GMSEC_INFO("Test messageSetConfig()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_Config         config  = configCreateUsingData("tracking=false", KEY_VALUE_DATA, NULL);
	GMSEC_MessageFactory factory = messageFactoryCreate(status);

	setStandardFields(factory);

	GMSEC_Message msg = messageFactoryCreateMessage(factory, "TLMPROC", NULL);

	// Nominal test
	{
		messageSetConfig(msg, config, status);

		testCheckBool("Unexpected error setting config", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected config content", strcompare(configToXML(messageGetConfig(msg, NULL), NULL), configToXML(config, NULL)) == 0);
	}

	// Off-nominal tests
	{
		// Message is NULL
		messageSetConfig(NULL, config, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// Config is NULL
		messageSetConfig(msg, NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	{
		messageGetConfig(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	configDestroy(&config);
	statusDestroy(&status);
}


void test_messageSetSubject()
{
	GMSEC_INFO("Test messageSetSubject()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(status);

	setStandardFields(factory);

	GMSEC_Message msg = messageFactoryCreateMessage(factory, "TLMPROC", NULL);

	//o Nominal test
	{
		messageSetSubject(msg, "GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY", status);
		testCheckBool("Unexpected error setting subject", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Message has unexpected subject", strcompare(messageGetSubject(msg, NULL), "GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY") == 0);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		messageSetSubject(NULL, "FOO.BAR", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// Subject is NULL
		messageSetSubject(msg, NULL, status);
		testCheckBool("Expected an error setting subject to NULL", statusHasError(status) == GMSEC_TRUE);
		testCheckBool(statusGetReason(status), strcontains(statusGetReason(status), "Subject cannot be NULL, nor an empty string") == 0);
	}

	{
		// Subject is empty string
		messageSetSubject(msg, "", status);
		testCheckBool("Expected an error setting subject to empty string", statusHasError(status) == GMSEC_TRUE);
		testCheckBool(statusGetReason(status), strcontains(statusGetReason(status), "Subject cannot be NULL, nor an empty string") == 0);
	}

	{
		// Subject contains lowercase character(s)
		messageSetSubject(msg, "Subject.Cannot.Contain.Lowercase.Letters", status);
		testCheckBool("Expected an error setting subject with lowercase letters", statusHasError(status) == GMSEC_TRUE);
		testCheckBool(statusGetReason(status), strcontains(statusGetReason(status), "Invalid message subject") == 0);
	}

	{
		// Subject contains invalid character(s)
		messageSetSubject(msg, "Hello%World", status);
		testCheckBool("Expected an error setting subject with illegal char", statusHasError(status) == GMSEC_TRUE);
		testCheckBool(statusGetReason(status), strcontains(statusGetReason(status), "Invalid message subject") == 0);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetSubject()
{
	GMSEC_INFO("Test messageGetSubject()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(status);

	setStandardFields(factory);

	// Nominal test
	{
		GMSEC_Field mode    = stringFieldCreate("STREAM-MODE", "RT", GMSEC_FALSE, NULL);  // ME2 (stream mode)
		int         channel = 2;   // ME2 (virtual channel ID)
		int         ap_id   = 1;   // ME3 (AP ID)

		GMSEC_Message msg = messageFactoryCreateMessage(factory, "TLMPROC", NULL);

		messageAddField(msg, mode, NULL);

		char subject[512] = {0};

		printToString(subject, "%s.%d.%d", messageGetSubject(msg, NULL), channel, ap_id);

		messageSetSubject(msg, subject, NULL);

		testCheckBool("Message has unexpected subject", strcompare(messageGetSubject(msg, NULL), subject) == 0);

		fieldDestroy(&mode);
		messageDestroy(&msg);
	}

	// Off-nominal test
	{
		// Message is NULL
		const char* subject = messageGetSubject(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Subject should be NULL", subject == NULL);
	}

	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageSetKind()
{
	GMSEC_INFO("Test messageSetKind()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(status);

	setStandardFields(factory);

	// Nominal test
	{
		GMSEC_Message msg = messageFactoryCreateMessage(factory, "HB", NULL);
		testCheckBool("Expected PUBLISH-kind", messageGetKind(msg, NULL) == GMSEC_PUBLISH);

		// Set to REQUEST-kind, and verify
		messageSetKind(msg, GMSEC_REQUEST, NULL);
		testCheckBool("Expected REQUEST-kind", messageGetKind(msg, NULL) == GMSEC_REQUEST);

		// Set to REPLY-kind, and verify
		messageSetKind(msg, GMSEC_REPLY, NULL);
		testCheckBool("Expected REPLY-kind", messageGetKind(msg, NULL) == GMSEC_REPLY);

		// Set to PUBLISH-kind, and verify
		messageSetKind(msg, GMSEC_PUBLISH, NULL);
		testCheckBool("Expected PUBLISH-kind", messageGetKind(msg, NULL) == GMSEC_PUBLISH);

		messageDestroy(&msg);
	}

	// Off-nominal test
	{
		messageSetKind(NULL, GMSEC_PUBLISH, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetKind()
{
	GMSEC_INFO("Test messageGetKind()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(status);

	setStandardFields(factory);

	// Nominal test
	{
		GMSEC_Message msg1 = messageFactoryCreateMessage(factory, "HB", NULL);
		GMSEC_Message msg2 = messageFactoryCreateMessage(factory, "REQ.DIR", NULL);
		GMSEC_Message msg3 = messageFactoryCreateMessage(factory, "RESP.DIR", NULL);
		GMSEC_Message msg4 = messageFactoryCreateSimpleMessage(factory, NULL);
		GMSEC_Message msg5 = messageCreate(NULL);

		testCheckBool("Message 1 has unexpected kind", messageGetKind(msg1, NULL) == GMSEC_PUBLISH);
		testCheckBool("Message 2 has unexpected kind", messageGetKind(msg2, NULL) == GMSEC_REQUEST);
		testCheckBool("Message 3 has unexpected kind", messageGetKind(msg3, NULL) == GMSEC_REPLY);
		testCheckBool("Message 4 has unexpected kind", messageGetKind(msg4, NULL) == GMSEC_PUBLISH);
		testCheckBool("Message 5 has unexpected kind", messageGetKind(msg5, NULL) == GMSEC_PUBLISH);

		messageDestroy(&msg1);
		messageDestroy(&msg2);
		messageDestroy(&msg3);
		messageDestroy(&msg4);
		messageDestroy(&msg5);
	}

	// Off-nominal test
	{
		GMSEC_MessageKind kind = messageGetKind(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected a PUBLISH result ", kind == GMSEC_PUBLISH);
	}

	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageAddField()
{
	GMSEC_INFO("Test messageAddField()");

	GMSEC_Status  status = statusCreate();
	GMSEC_Message msg    = messageCreate(NULL);
	GMSEC_Field   field1 = stringFieldCreate("MY-FIELD", "MY-VALUE", GMSEC_FALSE, NULL);
	GMSEC_Field   field2 = i32FieldCreate("MY-FIELD", 10, GMSEC_FALSE, NULL);

	// Nominal test
	{
		testCheckBool("Expected MY-FIELD to be unique", messageAddField(msg, field1, status) == GMSEC_FALSE);
		testCheckBool("Expected MY-FIELD to be a string", messageGetFieldType(msg, "MY-FIELD", NULL) == GMSEC_STRING_TYPE);
		testCheckBool("Unexpected MY-FIELD value", strcompare(messageGetStringValue(msg, "MY-FIELD", NULL), "MY-VALUE") == 0);

		testCheckBool("Expected MY-FIELD to be replaced", messageAddField(msg, field2, status) == GMSEC_TRUE);
		testCheckBool("Expected MY-FIELD to be a string", messageGetFieldType(msg, "MY-FIELD", NULL) == GMSEC_I32_TYPE);
		testCheckBool("Unexpected MY-FIELD value", strcompare(messageGetStringValue(msg, "MY-FIELD", NULL), "10") == 0);
	}

	// Off-nominal test(s)
	{
		// Message is NULL
		GMSEC_BOOL replaced = messageAddField(NULL, field1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);
	}

	{
		// Field is NULL
		GMSEC_BOOL replaced = messageAddField(msg, NULL, status);
		testCheckBool("Expected an error setting NULL field", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected replaced to be false", replaced == GMSEC_FALSE);
	}

	fieldDestroy(&field1);
	fieldDestroy(&field2);
	messageDestroy(&msg);
	statusDestroy(&status);
}


void test_messageAddXXXField()
{
	GMSEC_INFO("Test messageAddXXXField()");

	GMSEC_Status  status = statusCreate();
	GMSEC_Message msg    = messageCreate(NULL);

	// Nominal tests
	{
		testCheckBool("Expected Binary field to be unique", messageAddBinaryField(msg, "FIELD-01", (GMSEC_U8*) "ABCDEF", (size_t) 6, status) == GMSEC_FALSE);
		testCheckBool("Unexpected error adding Binary field", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected field type for Binary field", messageGetFieldType(msg, "FIELD-01", NULL) == GMSEC_BINARY_TYPE);

		testCheckBool("Expected Boolean field to be unique", messageAddBooleanField(msg, "FIELD-02", GMSEC_TRUE, status) == GMSEC_FALSE);
		testCheckBool("Unexpected error adding Boolean field", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected field type for Boolean field", messageGetFieldType(msg, "FIELD-02", NULL) == GMSEC_BOOL_TYPE);
		testCheckBool("Unexpected field value for Boolean field", messageGetBooleanValue(msg, "FIELD-02", NULL) == GMSEC_TRUE);

		testCheckBool("Expected Char field to be unique", messageAddCharField(msg, "FIELD-03", 'c', status) == GMSEC_FALSE);
		testCheckBool("Unexpected error adding Char field", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected field type for Char field", messageGetFieldType(msg, "FIELD-03", NULL) == GMSEC_CHAR_TYPE);
		testCheckBool("Unexpected field value for Char field", strcompare(messageGetStringValue(msg, "FIELD-03", NULL), "c") == 0);

		testCheckBool("Expected F32 field to be unique", messageAddF32Field(msg, "FIELD-04", 3.14f, status) == GMSEC_FALSE);
		testCheckBool("Unexpected error adding F32 field", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected field type for F32 field", messageGetFieldType(msg, "FIELD-04", NULL) == GMSEC_F32_TYPE);
		testCheckBool("Unexpected field value for F32 field", messageGetF64Value(msg, "FIELD-04", NULL) == 3.14);

		testCheckBool("Expected F64 field to be unique", messageAddF64Field(msg, "FIELD-05", 3.14, status) == GMSEC_FALSE);
		testCheckBool("Unexpected error adding F64 field", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected field type for F64 field", messageGetFieldType(msg, "FIELD-05", NULL) == GMSEC_F64_TYPE);
		testCheckBool("Unexpected field value for F64 field", messageGetF64Value(msg, "FIELD-05", NULL) == 3.14);

		testCheckBool("Expected I16 field to be unique", messageAddI16Field(msg, "FIELD-06", (GMSEC_I16) 10, status) == GMSEC_FALSE);
		testCheckBool("Unexpected error adding I16 field", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected field type for I16 field", messageGetFieldType(msg, "FIELD-06", NULL) == GMSEC_I16_TYPE);
		testCheckBool("Unexpected field value for I16 field", messageGetI32Value(msg, "FIELD-06", NULL) == 10);

		testCheckBool("Expected I32 field to be unique", messageAddI32Field(msg, "FIELD-07", 20, status) == GMSEC_FALSE);
		testCheckBool("Unexpected error adding I32 field", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected field type for I32 field", messageGetFieldType(msg, "FIELD-07", NULL) == GMSEC_I32_TYPE);
		testCheckBool("Unexpected field value for I32 field", messageGetI32Value(msg, "FIELD-07", NULL) == 20);

		testCheckBool("Expected I64 field to be unique", messageAddI64Field(msg, "FIELD-08", (GMSEC_I64) 30, status) == GMSEC_FALSE);
		testCheckBool("Unexpected error adding I64 field", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected field type for I64 field", messageGetFieldType(msg, "FIELD-08", NULL) == GMSEC_I64_TYPE);
		testCheckBool("Unexpected field value for I64 field", messageGetI32Value(msg, "FIELD-08", NULL) == 30);

		testCheckBool("Expected I8 field to be unique", messageAddI8Field(msg, "FIELD-09", (GMSEC_I8) 40, status) == GMSEC_FALSE);
		testCheckBool("Unexpected error adding I8 field", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected field type for I8 field", messageGetFieldType(msg, "FIELD-09", NULL) == GMSEC_I8_TYPE);
		testCheckBool("Unexpected field value for I8 field", messageGetI32Value(msg, "FIELD-09", NULL) == 40);

		testCheckBool("Expected String field to be unique", messageAddStringField(msg, "FIELD-10", "Hello", status) == GMSEC_FALSE);
		testCheckBool("Unexpected error adding String field", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected field type for String field", messageGetFieldType(msg, "FIELD-10", NULL) == GMSEC_STRING_TYPE);
		testCheckBool("Unexpected field value for String field", strcompare(messageGetStringValue(msg, "FIELD-10", NULL), "Hello") == 0);

		testCheckBool("Expected U16 field to be unique", messageAddU16Field(msg, "FIELD-11", (GMSEC_U16) 50, status) == GMSEC_FALSE);
		testCheckBool("Unexpected error adding U16 field", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected field type for U16 field", messageGetFieldType(msg, "FIELD-11", NULL) == GMSEC_U16_TYPE);
		testCheckBool("Unexpected field value for U16 field", messageGetU32Value(msg, "FIELD-11", NULL) == (GMSEC_U32) 50);

		testCheckBool("Expected U32 field to be unique", messageAddU32Field(msg, "FIELD-12", (GMSEC_U32) 60, status) == GMSEC_FALSE);
		testCheckBool("Unexpected error adding U32 field", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected field type for U32 field", messageGetFieldType(msg, "FIELD-12", NULL) == GMSEC_U32_TYPE);
		testCheckBool("Unexpected field value for U32 field", messageGetU32Value(msg, "FIELD-12", NULL) == (GMSEC_U32) 60);

		testCheckBool("Expected U64 field to be unique", messageAddU64Field(msg, "FIELD-13", (GMSEC_U64) 70, status) == GMSEC_FALSE);
		testCheckBool("Unexpected error adding U64 field", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected field type for U64 field", messageGetFieldType(msg, "FIELD-13", NULL) == GMSEC_U64_TYPE);
		testCheckBool("Unexpected field value for U64 field", messageGetU64Value(msg, "FIELD-13", NULL) == (GMSEC_U64) 70);

		testCheckBool("Expected U8 field to be unique", messageAddU8Field(msg, "FIELD-14", (GMSEC_U8) 80, status) == GMSEC_FALSE);
		testCheckBool("Unexpected error adding U8 field", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected field type for U8 field", messageGetFieldType(msg, "FIELD-14", NULL) == GMSEC_U8_TYPE);
		testCheckBool("Unexpected field value for U8 field", messageGetU32Value(msg, "FIELD-14", NULL) == (GMSEC_U32) 80);
	}

	// Off-nominal tests
	{
		messageAddBinaryField(NULL, "FIELD-01", (GMSEC_U8*) "ABCDEF", (size_t) 6, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageAddBinaryField(msg, NULL, (GMSEC_U8*) "ABCDEF", (size_t) 6, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		messageAddBooleanField(NULL, "FIELD-01", GMSEC_TRUE, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageAddBooleanField(msg, NULL, GMSEC_TRUE, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		messageAddCharField(NULL, "FIELD-01", 'C', status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageAddCharField(msg, NULL, 'C', status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		messageAddF32Field(NULL, "FIELD-01", (GMSEC_F32) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageAddF32Field(msg, NULL, (GMSEC_F32) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		messageAddF64Field(NULL, "FIELD-01", (GMSEC_F64) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageAddF64Field(msg, NULL, (GMSEC_F64) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		messageAddI16Field(NULL, "FIELD-01", (GMSEC_I16) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageAddI16Field(msg, NULL, (GMSEC_I16) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		messageAddI32Field(NULL, "FIELD-01", (GMSEC_I32) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageAddI32Field(msg, NULL, (GMSEC_I32) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		messageAddI64Field(NULL, "FIELD-01", (GMSEC_I64) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageAddI64Field(msg, NULL, (GMSEC_I64) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		messageAddI8Field(NULL, "FIELD-01", (GMSEC_I8) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageAddI8Field(msg, NULL, (GMSEC_I8) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		messageAddStringField(NULL, "FIELD-01", "FOO-BAR", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageAddStringField(msg, NULL, "FOO-BAR", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		messageAddU16Field(NULL, "FIELD-01", (GMSEC_U16) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageAddU16Field(msg, NULL, (GMSEC_U16) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		messageAddU32Field(NULL, "FIELD-01", (GMSEC_U32) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageAddU32Field(msg, NULL, (GMSEC_U32) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		messageAddU64Field(NULL, "FIELD-01", (GMSEC_U64) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageAddU64Field(msg, NULL, (GMSEC_U64) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		messageAddU8Field(NULL, "FIELD-01", (GMSEC_U8) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageAddU8Field(msg, NULL, (GMSEC_U8) 1, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	statusDestroy(&status);
}


void test_messageAddFields()
{
	GMSEC_INFO("Test messageAddFields()");

	GMSEC_Status  status = statusCreate();
	GMSEC_Message msg    = messageCreate(NULL);

	GMSEC_Field fields[3];
	fields[0] = stringFieldCreate("ONE", "uno", GMSEC_FALSE, NULL);
	fields[1] = stringFieldCreate("TWO", "dos", GMSEC_FALSE, NULL);
	fields[2] = stringFieldCreate("THREE", "tres", GMSEC_FALSE, NULL);

	// Nominal test
	{
		messageAddFields(msg, fields, sizeof(fields)/sizeof(GMSEC_Field), status);

		testCheckBool("Unexpected error adding fields", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected number of fields", messageGetFieldCount(msg, NULL) == 3);

		testCheckBool("Expected to have field ONE", messageHasField(msg, "ONE", NULL) == GMSEC_TRUE);
		testCheckBool("Unexpected field type for ONE", messageGetFieldType(msg, "ONE", NULL) == GMSEC_STRING_TYPE);
		testCheckBool("Unexpected field value for ONE", strcompare(messageGetStringValue(msg, "ONE", NULL), "uno") == 0);

		testCheckBool("Expected to have field TWO", messageHasField(msg, "TWO", NULL) == GMSEC_TRUE);
		testCheckBool("Unexpected field type for TWO", messageGetFieldType(msg, "TWO", NULL) == GMSEC_STRING_TYPE);
		testCheckBool("Unexpected field value for TWO", strcompare(messageGetStringValue(msg, "TWO", NULL), "dos") == 0);

		testCheckBool("Expected to have field THREE", messageHasField(msg, "THREE", NULL) == GMSEC_TRUE);
		testCheckBool("Unexpected field type for THREE", messageGetFieldType(msg, "THREE", NULL) == GMSEC_STRING_TYPE);
		testCheckBool("Unexpected field value for THREE", strcompare(messageGetStringValue(msg, "THREE", NULL), "tres") == 0);
	}

	// Off-nominal tests
	{
		// Message is NULL
		messageAddFields(NULL, fields, sizeof(fields)/sizeof(GMSEC_Field), status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// Field list is NULL
		messageAddFields(msg, NULL, sizeof(fields)/sizeof(GMSEC_Field), status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// Field list size is 0
		messageAddFields(msg, fields, 0, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	fieldDestroy(&fields[0]);
	fieldDestroy(&fields[1]);
	fieldDestroy(&fields[2]);
	messageDestroy(&msg);
	statusDestroy(&status);
}


void test_messageClearFields()
{
	GMSEC_INFO("Test messageClearFields()");

	GMSEC_Status status = statusCreate();

	// Nominal test
	{
		GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
		GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);

		testCheckBool("Message has no fields!", messageGetFieldCount(msg, NULL) > 0);
		testCheckBool("Message missing CONTENT-VERSION", messageHasField(msg, "CONTENT-VERSION", NULL) == GMSEC_TRUE);
		testCheckBool("Message missing HEADER-VERSION", messageHasField(msg, "HEADER-VERSION", NULL) == GMSEC_TRUE);
		testCheckBool("Message missing MESSAGE-SUBTYPE", messageHasField(msg, "MESSAGE-SUBTYPE", NULL) == GMSEC_TRUE);
		testCheckBool("Message missing MESSAGE-TYPE", messageHasField(msg, "MESSAGE-TYPE", NULL) == GMSEC_TRUE);
		testCheckBool("Message missing SPECIFICATION", messageHasField(msg, "SPECIFICATION", NULL) == GMSEC_TRUE);

		messageClearFields(msg, status);

		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Message has fields!", messageGetFieldCount(msg, NULL) == 0);
		testCheckBool("Message has CONTENT-VERSION", messageHasField(msg, "CONTENT-VERSION", NULL) == GMSEC_FALSE);
		testCheckBool("Message has HEADER-VERSION", messageHasField(msg, "HEADER-VERSION", NULL) == GMSEC_FALSE);
		testCheckBool("Message has MESSAGE-SUBTYPE", messageHasField(msg, "MESSAGE-SUBTYPE", NULL) == GMSEC_FALSE);
		testCheckBool("Message has MESSAGE-TYPE", messageHasField(msg, "MESSAGE-TYPE", NULL) == GMSEC_FALSE);
		testCheckBool("Message has SPECIFICATION", messageHasField(msg, "SPECIFICATION", NULL) == GMSEC_FALSE);

		messageDestroy(&msg);
		messageFactoryDestroy(&factory);
	}

	// Off-nominal test
	{
		// Message is NULL
		messageClearFields(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	statusDestroy(&status);
}


void test_messageClearField()
{
	GMSEC_INFO("Test messageClearField()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);

	// Nominal test
	{
		testCheckBool("Messge should have CONTENT-VERSION", messageHasField(msg, "CONTENT-VERSION", NULL) == GMSEC_TRUE);
		testCheckBool("Expected result to indicate field cleared", messageClearField(msg, "CONTENT-VERSION", status) == GMSEC_TRUE);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Messge should not have CONTENT-VERSION", messageHasField(msg, "CONTENT-VERSION", NULL) == GMSEC_FALSE);
		testCheckBool("Expected result to indicate field not cleared", messageClearField(msg, "FOO-BAR", status) == GMSEC_FALSE);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Expected result to indicate field not cleared", messageClearField(msg, "foo-bar", status) == GMSEC_FALSE);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	// Off-nominal tests
	{
		// Message is NULL
		GMSEC_BOOL result = messageClearField(NULL, "FOO-BAR", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected result to be false", result == GMSEC_FALSE);
	}

	{
		// Field name is NULL
		GMSEC_BOOL result = messageClearField(msg, NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected result to be false", result == GMSEC_FALSE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
}


void test_messageHasField()
{
	GMSEC_INFO("Test messageHasField()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);

	// Nominal test
	{
		messageSetFieldValue(msg, "PUB-RATE", "30", NULL);

		testCheckBool("Message should have PUB-RATE", messageHasField(msg, "PUB-RATE", NULL) == GMSEC_TRUE);
		testCheckBool("Message should not have PUBLIST-RATE", messageHasField(msg, "PUBLISH-RATE", NULL) == GMSEC_FALSE);
		testCheckBool("Message should not have null field", messageHasField(msg, NULL, NULL) == GMSEC_FALSE);
		testCheckBool("Message should not have empty string field", messageHasField(msg, "", NULL) == GMSEC_FALSE);

		// Field name is NULL
		testCheckBool("Expected result to be false", messageHasField(msg, NULL, status) == GMSEC_FALSE);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_FALSE);
	}

	// Off-nominal tests
	{
		// Message is NULL
		GMSEC_BOOL result = messageHasField(NULL, "FOO-BAR", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected result to be false", result == GMSEC_FALSE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetField()
{
	GMSEC_INFO("Test messageGetField()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);

	//o Nominal tests
	{
		testCheckBool("Message should have MESSAGE-TYPE", messageGetField(msg, "MESSAGE-TYPE", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);

		testCheckBool("Message should have MSG-TYPE", messageGetField(msg, "MSG-TYPE", status) == NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	// Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Expected field to be NULL", messageGetField(NULL, "MESSAGE-TYPE", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Message should not have NULL named field", messageGetField(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Message should not have empty string named field", messageGetField(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetFieldType()
{
	GMSEC_INFO("Test messageGetFieldType()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);

	//o Nominal tests
	{
		testCheckBool("MESSAGE-TYPE has unexpected field type", messageGetFieldType(msg, "MESSAGE-TYPE", status) == GMSEC_STRING_TYPE);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		messageGetFieldType(NULL, "MESSAGE-TYPE", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field does not exist
		messageGetFieldType(msg, "MSG-TYPE", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		messageGetFieldType(msg, NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		messageGetFieldType(msg, "", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetStringValue()
{
	GMSEC_INFO("Test messageGetStringValue()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);

	messageSetFieldValue(msg, "PUB-RATE", "30", NULL);

	// Nominal tests
	{
		testCheckBool("MESSAGE-TYPE has unexpected value", strcompare(messageGetStringValue(msg, "MESSAGE-TYPE", NULL), "MSG") == 0);
		testCheckBool("PUB-RATE has unexpected value", strcompare(messageGetStringValue(msg, "PUB-RATE", NULL), "30") == 0);
	}

	// Off-nominal tests
	{
		// Message is NULL
		messageGetStringValue(NULL, "MESSAGE-TYPE", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Unknown field
		messageGetStringValue(msg, "BOGUS", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// NULL field name
		messageGetStringValue(msg, NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Empty field name
		messageGetStringValue(msg, "", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetBooleanValue()
{
	GMSEC_INFO("Test messageGetBooleanValue()");

	GMSEC_Status  status = statusCreate();
	GMSEC_Message msg    = messageCreate(NULL);

	messageAddI32Field(msg, "FIELD-1", 0, NULL);
	messageAddI32Field(msg, "FIELD-2", 1, NULL);
	messageAddStringField(msg, "FIELD-3", "10000", NULL);
	messageAddI32Field(msg, "FIELD-4", 10000, NULL);
	messageAddBooleanField(msg, "FIELD-5", GMSEC_TRUE, NULL);
	messageAddBooleanField(msg, "FIELD-6", GMSEC_FALSE, NULL);
	messageAddF64Field(msg, "FIELD-7", 2019.0, NULL);
	messageAddStringField(msg, "FIELD-8", "true", NULL);
	messageAddStringField(msg, "FIELD-9", "FALSE", NULL);
	messageAddStringField(msg, "FIELD-0", "foo bar", NULL);

	// Nominal tests
	{
		testCheckBool("1 Expected boolean value of GMSEC_FALSE", messageGetBooleanValue(msg, "FIELD-1", NULL) == GMSEC_FALSE);
		testCheckBool("2 Expected boolean value of GMSEC_TRUE",  messageGetBooleanValue(msg, "FIELD-2", NULL) == GMSEC_TRUE);
		testCheckBool("3 Expected boolean value of GMSEC_TRUE",  messageGetBooleanValue(msg, "FIELD-3", NULL) == GMSEC_TRUE);
		testCheckBool("4 Expected boolean value of GMSEC_TRUE",  messageGetBooleanValue(msg, "FIELD-4", NULL) == GMSEC_TRUE);
		testCheckBool("5 Expected boolean value of GMSEC_TRUE",  messageGetBooleanValue(msg, "FIELD-5", NULL) == GMSEC_TRUE);
		testCheckBool("6 Expected boolean value of GMSEC_FALSE", messageGetBooleanValue(msg, "FIELD-6", NULL) == GMSEC_FALSE);
		testCheckBool("7 Expected boolean value of GMSEC_TRUE",  messageGetBooleanValue(msg, "FIELD-7", NULL) == GMSEC_TRUE);
		testCheckBool("8 Expected boolean value of GMSEC_TRUE",  messageGetBooleanValue(msg, "FIELD-8", NULL) == GMSEC_TRUE);
		testCheckBool("9 Expected boolean value of GMSEC_FALSE", messageGetBooleanValue(msg, "FIELD-9", NULL) == GMSEC_FALSE);
	}

	// Off-nominal tests
	{
		// Message is NULL
		messageGetBooleanValue(NULL, "FIELD-1", status);
		testCheckBool("Expected error", statusHasError(status) == GMSEC_TRUE);

		// Field cannot be converted
		messageGetBooleanValue(msg, "FIELD-0", status);
		testCheckBool("Expected error", statusHasError(status) == GMSEC_TRUE);

		// Unknown field
		messageGetBooleanValue(msg, "BOGUS", status);
		testCheckBool("Expected error", statusHasError(status) == GMSEC_TRUE);

		// NULL field name
		messageGetBooleanValue(msg, NULL, status);
		testCheckBool("Expected error", statusHasError(status) == GMSEC_TRUE);

		// Empty field name
		messageGetBooleanValue(msg, "", status);
		testCheckBool("Expected error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	statusDestroy(&status);
}


void test_messageGetI16Value()
{
	GMSEC_INFO("Test messageGetI16Value()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);

	messageSetFieldValue(msg, "PUB-RATE", "30", NULL);

	//o Nominal test
	{
		testCheckBool("PUB-RATE has unexpected value", messageGetI16Value(msg, "PUB-RATE", status) == (GMSEC_I16) 30);
		testCheckBool("Unexpected error fetching PUB-RATE value", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal test
	{
		// Message is NULL
		messageGetI16Value(NULL, "PUB-RATE", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field cannot be converted
		testCheckBool("MESSAGE-TYPE has unexpected value", messageGetI16Value(msg, "MESSAGE-TYPE", status) == 0);
		testCheckBool("Expected error fetching MESSAGE-TYPE value", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetI32Value()
{
	GMSEC_INFO("Test messageGetI32Value()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);

	messageSetFieldValue(msg, "PUB-RATE", "30", NULL);

	//o Nominal test
	{
		testCheckBool("PUB-RATE has unexpected value", messageGetI32Value(msg, "PUB-RATE", status) == 30);
		testCheckBool("Unexpected error fetching PUB-RATE value", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal test
	{
		// Message is NULL
		messageGetI32Value(NULL, "PUB-RATE", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field cannot be converted
		testCheckBool("MESSAGE-TYPE has unexpected value", messageGetI32Value(msg, "MESSAGE-TYPE", status) == 0);
		testCheckBool("Expected error fetching MESSAGE-TYPE value", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetI64Value()
{
	GMSEC_INFO("Test messageGetI64Value()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);

	messageSetFieldValue(msg, "PUB-RATE", "30", NULL);

	//o Nominal test
	{
		testCheckBool("PUB-RATE has unexpected value", messageGetI64Value(msg, "PUB-RATE", status) == (GMSEC_I64) 30);
		testCheckBool("Unexpected error fetching PUB-RATE value", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal test
	{
		// Message is NULL
		messageGetI64Value(NULL, "PUB-RATE", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field cannot be converted
		testCheckBool("MESSAGE-TYPE has unexpected value", messageGetI64Value(msg, "MESSAGE-TYPE", status) == (GMSEC_I64) 0);
		testCheckBool("Expected error fetching MESSAGE-TYPE value", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetU16Value()
{
	GMSEC_INFO("Test messageGetU16Value()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);

	messageSetFieldValue(msg, "PUB-RATE", "30", NULL);

	//o Nominal test
	{
		testCheckBool("PUB-RATE has unexpected value", messageGetU16Value(msg, "PUB-RATE", status) == (GMSEC_U16) 30);
		testCheckBool("Unexpected error fetching PUB-RATE value", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal test
	{
		// Message is NULL
		messageGetU16Value(NULL, "PUB-RATE", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field cannot be converted
		testCheckBool("MESSAGE-TYPE has unexpected value", messageGetU16Value(msg, "MESSAGE-TYPE", status) == (GMSEC_U16) 0);
		testCheckBool("Expected error fetching MESSAGE-TYPE value", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetU32Value()
{
	GMSEC_INFO("Test messageGetU32Value()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);

	messageSetFieldValue(msg, "PUB-RATE", "30", NULL);

	//o Nominal test
	{
		testCheckBool("PUB-RATE has unexpected value", messageGetU32Value(msg, "PUB-RATE", status) == (GMSEC_U32) 30);
		testCheckBool("Unexpected error fetching PUB-RATE value", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal test
	{
		// Message is NULL
		messageGetU32Value(NULL, "PUB-RATE", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field cannot be converted
		testCheckBool("MESSAGE-TYPE has unexpected value", messageGetU32Value(msg, "MESSAGE-TYPE", status) == (GMSEC_U32) 0);
		testCheckBool("Expected error fetching MESSAGE-TYPE value", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetU64Value()
{
	GMSEC_INFO("Test messageGetU64Value()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);

	messageSetFieldValue(msg, "PUB-RATE", "30", NULL);

	//o Nominal test
	{
		testCheckBool("PUB-RATE has unexpected value", messageGetU64Value(msg, "PUB-RATE", status) == (GMSEC_U64) 30);
		testCheckBool("Unexpected error fetching PUB-RATE value", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal test
	{
		// Message is NULL
		messageGetU64Value(NULL, "PUB-RATE", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field cannot be converted
		testCheckBool("MESSAGE-TYPE has unexpected value", messageGetU64Value(msg, "MESSAGE-TYPE", status) == (GMSEC_U64) 0);
		testCheckBool("Expected error fetching MESSAGE-TYPE value", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetF64Value()
{
	GMSEC_INFO("Test messageGetF64Value()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);

	messageSetFieldValue(msg, "PUB-RATE", "30", NULL);

	//o Nominal test
	{
		testCheckBool("PUB-RATE has unexpected value", messageGetF64Value(msg, "PUB-RATE", status) == (GMSEC_F64) 30);
		testCheckBool("Unexpected error fetching PUB-RATE value", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal test
	{
		// Message is NULL
		messageGetF64Value(NULL, "PUB-RATE", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field cannot be converted
		testCheckBool("MESSAGE-TYPE has unexpected value", messageGetF64Value(msg, "MESSAGE-TYPE", status) == (GMSEC_F64) 0);
		testCheckBool("Expected error fetching MESSAGE-TYPE value", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


#define DATA_SIZE 256

void test_messageGetBinaryField()
{
	GMSEC_INFO("Test messageGetBinaryField()");

	GMSEC_Status         status   = statusCreate();
	GMSEC_MessageFactory factory  = messageFactoryCreate(NULL);
	GMSEC_Message        msg      = messageFactoryCreateMessage(factory, "MSG.TLMPKT", NULL);
	size_t               i;
	GMSEC_U8             data[ DATA_SIZE ];

	for (i = 0; i <  DATA_SIZE; ++i)
	{
		data[i] = (GMSEC_U8)(i & 0xFF);
	}

	messageAddBinaryField(msg, "DATA", data, DATA_SIZE, NULL);

	//o Nominal test
	{
		testCheckBool("Expected a field", messageGetBinaryField(msg, "DATA", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Unexpected field", messageGetBinaryField(NULL, "DATA", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field is not a BinaryField
		testCheckBool("Expected NULL", messageGetBinaryField(msg, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Expected NULL", messageGetBinaryField(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Expected NULL", messageGetBinaryField(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetBooleanField()
{
	GMSEC_INFO("Test messageGetBooleanField()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "REQ.DIR", NULL);

	messageAddBooleanField(msg, "RESPONSE", GMSEC_TRUE, NULL);

	//o Nominal test
	{
		testCheckBool("Expected a field", messageGetBooleanField(msg, "RESPONSE", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Unexpected field", messageGetBooleanField(NULL, "RESPONSE", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field is not a BooleanField
		testCheckBool("Expected NULL", messageGetBooleanField(msg, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Expected NULL", messageGetBooleanField(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Expected NULL", messageGetBooleanField(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetCharField()
{
	GMSEC_INFO("Test messageGetCharField()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "REQ.DIR", NULL);

	messageAddCharField(msg, "CHAR-FIELD", 'c', NULL);

	//o Nominal test
	{
		testCheckBool("Expected a field", messageGetCharField(msg, "CHAR-FIELD", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Unexpected field", messageGetCharField(NULL, "RESPONSE", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field is not a CharField
		testCheckBool("Expected NULL", messageGetCharField(msg, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Expected NULL", messageGetCharField(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Expected NULL", messageGetCharField(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetF32Field()
{
	GMSEC_INFO("Test messageGetF32Field()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "REQ.DIR", NULL);

	//o Nominal test
	{
		testCheckBool("Expected a field", messageGetF32Field(msg, "CONTENT-VERSION", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Unexpected field", messageGetF32Field(NULL, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field is not an F32Field
		testCheckBool("Expected NULL", messageGetF32Field(msg, "MESSAGE-TYPE", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Expected NULL", messageGetF32Field(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Expected NULL", messageGetF32Field(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetF64Field()
{
	GMSEC_INFO("Test messageGetF64Field()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "REQ.DIR", NULL);

	messageAddF64Field(msg, "F64-FIELD", 3.14, NULL);

	//o Nominal test
	{
		testCheckBool("Expected a field", messageGetF64Field(msg, "F64-FIELD", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Unexpected field", messageGetF64Field(NULL, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field is not an F64Field
		testCheckBool("Expected NULL", messageGetF64Field(msg, "MESSAGE-TYPE", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Expected NULL", messageGetF64Field(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Expected NULL", messageGetF64Field(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetI8Field()
{
	GMSEC_INFO("Test messageGetI8Field()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "REQ.DIR", NULL);

	messageAddI8Field(msg, "I8-FIELD", (GMSEC_I8) 8, NULL);

	//o Nominal test
	{
		testCheckBool("Expected a field", messageGetI8Field(msg, "I8-FIELD", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Unexpected field", messageGetI8Field(NULL, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field is not an I8Field
		testCheckBool("Expected NULL", messageGetI8Field(msg, "MESSAGE-TYPE", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Expected NULL", messageGetI8Field(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Expected NULL", messageGetI8Field(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetI16Field()
{
	GMSEC_INFO("Test messageGetI16Field()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "REQ.DIR", NULL);

	messageAddI16Field(msg, "I16-FIELD", (GMSEC_I16) 16, NULL);

	//o Nominal test
	{
		testCheckBool("Expected a field", messageGetI16Field(msg, "I16-FIELD", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Unexpected field", messageGetI16Field(NULL, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field is not an I16Field
		testCheckBool("Expected NULL", messageGetI16Field(msg, "MESSAGE-TYPE", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Expected NULL", messageGetI16Field(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Expected NULL", messageGetI16Field(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetI32Field()
{
	GMSEC_INFO("Test messageGetI32Field()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "REQ.DIR", NULL);

	messageAddI32Field(msg, "I32-FIELD", 32, NULL);

	//o Nominal test
	{
		testCheckBool("Expected a field", messageGetI32Field(msg, "I32-FIELD", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Unexpected field", messageGetI32Field(NULL, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field is not an I32Field
		testCheckBool("Expected NULL", messageGetI32Field(msg, "MESSAGE-TYPE", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Expected NULL", messageGetI32Field(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Expected NULL", messageGetI32Field(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetI64Field()
{
	GMSEC_INFO("Test messageGetI64Field()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "REQ.DIR", NULL);

	messageAddI64Field(msg, "I64-FIELD", (GMSEC_I64) 64, NULL);

	//o Nominal test
	{
		testCheckBool("Expected a field", messageGetI64Field(msg, "I64-FIELD", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Unexpected field", messageGetI64Field(NULL, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field is not an I64Field
		testCheckBool("Expected NULL", messageGetI64Field(msg, "MESSAGE-TYPE", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Expected NULL", messageGetI64Field(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Expected NULL", messageGetI64Field(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetU8Field()
{
	GMSEC_INFO("Test messageGetU8Field()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "REQ.DIR", NULL);

	messageAddU8Field(msg, "U8-FIELD", (GMSEC_U8) 8, NULL);

	//o Nominal test
	{
		testCheckBool("Expected a field", messageGetU8Field(msg, "U8-FIELD", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Unexpected field", messageGetU8Field(NULL, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field is not a U8Field
		testCheckBool("Expected NULL", messageGetU8Field(msg, "MESSAGE-TYPE", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Expected NULL", messageGetU8Field(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Expected NULL", messageGetU8Field(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetU16Field()
{
	GMSEC_INFO("Test messageGetU16Field()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "REQ.DIR", NULL);

	messageAddU16Field(msg, "U16-FIELD", (GMSEC_U16) 16, NULL);

	//o Nominal test
	{
		testCheckBool("Expected a field", messageGetU16Field(msg, "U16-FIELD", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Unexpected field", messageGetU16Field(NULL, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field is not a U16Field
		testCheckBool("Expected NULL", messageGetU16Field(msg, "MESSAGE-TYPE", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Expected NULL", messageGetU16Field(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Expected NULL", messageGetU16Field(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetU32Field()
{
	GMSEC_INFO("Test messageGetU32Field()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "REQ.DIR", NULL);

	messageAddU32Field(msg, "U32-FIELD", (GMSEC_U32) 32, NULL);

	//o Nominal test
	{
		testCheckBool("Expected a field", messageGetU32Field(msg, "U32-FIELD", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Unexpected field", messageGetU32Field(NULL, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field is not a U32Field
		testCheckBool("Expected NULL", messageGetU32Field(msg, "MESSAGE-TYPE", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Expected NULL", messageGetU32Field(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Expected NULL", messageGetU32Field(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetU64Field()
{
	GMSEC_INFO("Test messageGetU64Field()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "REQ.DIR", NULL);

	messageAddU64Field(msg, "U64-FIELD", (GMSEC_U64) 64, NULL);

	//o Nominal test
	{
		testCheckBool("Expected a field", messageGetU64Field(msg, "U64-FIELD", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Unexpected field", messageGetU64Field(NULL, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field is not a U64Field
		testCheckBool("Expected NULL", messageGetU64Field(msg, "MESSAGE-TYPE", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Expected NULL", messageGetU64Field(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Expected NULL", messageGetU64Field(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetStringField()
{
	GMSEC_INFO("Test messageGetStringField()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "REQ.DIR", NULL);

	//o Nominal test
	{
		testCheckBool("Expected a field", messageGetStringField(msg, "MESSAGE-TYPE", status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
	}

	//o Off-nominal tests
	{
		// Message is NULL
		testCheckBool("Unexpected field", messageGetStringField(NULL, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field is not a StringField
		testCheckBool("Expected NULL", messageGetStringField(msg, "CONTENT-VERSION", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is NULL
		testCheckBool("Expected NULL", messageGetStringField(msg, NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// Field name is empty-string
		testCheckBool("Expected NULL", messageGetStringField(msg, "", status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageGetFieldCount()
{
	GMSEC_INFO("Test messageGetFieldCount()");

	// Nominal test
	{
		GMSEC_MessageFactory factory = messageFactoryCreate(NULL);

		setStandardFields(factory);

		GMSEC_Message msg = messageFactoryCreateMessage(factory, "HB", NULL);

		testCheckBool("Message has unexpected field count", messageGetFieldCount(msg, NULL) == (size_t) 13);

		messageSetFieldValue(msg, "PUB-RATE", "30", NULL);

		testCheckBool("Message has unexpected field count", messageGetFieldCount(msg, NULL) == (size_t) 14);

		messageClearField(msg, "PUB-RATE", NULL);

		testCheckBool("Message has unexpected field count", messageGetFieldCount(msg, NULL) == (size_t) 13);

		messageClearFields(msg, NULL);

		testCheckBool("Message has unexpected field count", messageGetFieldCount(msg, NULL) == (size_t) 0);

		messageDestroy(&msg);
		messageFactoryDestroy(&factory);
	}

	// Off-nominal test
	{
		GMSEC_Status status = statusCreate();

		testCheckBool("Expected field count to be 0", messageGetFieldCount(NULL, status) == 0);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		statusDestroy(&status);
	}
}


void test_messageCopyFields()
{
	GMSEC_INFO("Test messageCopyFields()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg1    = messageFactoryCreateMessage(factory, "HB", NULL);
	GMSEC_Message        msg2    = messageCreateCopy(msg1, NULL);

	messageSetFieldValue(msg1, "PUB-RATE", "30", NULL);

	// Nominal test
	{
		testCheckBool("Message 2 should not have PUB-RATE", messageHasField(msg2, "PUB-RATE", NULL) == GMSEC_FALSE);

		messageCopyFields(msg1, msg2, status);

		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Message 2 should have PUB-RATE", messageHasField(msg2, "PUB-RATE", NULL) == GMSEC_TRUE);
	}

	// Off-nominal tests
	{
		// From-Message is NULL
		messageCopyFields(NULL, msg2, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		// To-Message is NULL
		messageCopyFields(msg1, NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg1);
	messageDestroy(&msg2);
	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageToXML()
{
	GMSEC_INFO("Test messageToXML()");

	GMSEC_Status status = statusCreate();

	// Nominal tests
	{
		GMSEC_MessageFactory factory = messageFactoryCreate(NULL);

		setStandardFields(factory);

		GMSEC_Message msg1 = messageFactoryCreateMessage(factory, "HB", NULL);
		GMSEC_Message msg2 = messageCreateCopy(msg1, NULL);
		GMSEC_Message msg3 = messageFactoryFromData(factory, messageToXML(msg1, NULL), XML_DATA, NULL);

		testCheckBool("Message 1 has NULL XML content", messageToXML(msg1, status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);

		testCheckBool("Message 2 has NULL XML content", messageToXML(msg2, NULL) != NULL);
		testCheckBool("Message 1 and 2 do not have same XML content", strcompare(messageToXML(msg1, NULL), messageToXML(msg2, NULL)) == 0);
		testCheckBool("Message 1 and 3 do not have same XML content", strcompare(messageToXML(msg1, NULL), messageToXML(msg3, NULL)) == 0);

		messageDestroy(&msg1);
		messageDestroy(&msg2);
		messageDestroy(&msg3);
		messageFactoryDestroy(&factory);
	}

	// Off-nominal test
	{
		testCheckBool("Expected NULL XML string", messageToXML(NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	statusDestroy(&status);
}


void test_messageToJSON()
{
	GMSEC_INFO("Test messageToJSON()");

	GMSEC_Status status = statusCreate();

	// Nominal tests
	{
		GMSEC_MessageFactory factory = messageFactoryCreate(NULL);

		setStandardFields(factory);

		GMSEC_Message msg1 = messageFactoryCreateMessage(factory, "HB", NULL);
		GMSEC_Message msg2 = messageCreateCopy(msg1, NULL);
		GMSEC_Message msg3 = messageFactoryFromData(factory, messageToJSON(msg1, NULL), JSON_DATA, NULL);

		testCheckBool("Message 1 has NULL JSON content", messageToJSON(msg1, status) != NULL);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);

		testCheckBool("Message 2 has NULL JSON content", messageToJSON(msg2, NULL) != NULL);
		testCheckBool("Message 1 and 2 do not have same JSON content", strcompare(messageToJSON(msg1, NULL), messageToJSON(msg2, NULL)) == 0);
		testCheckBool("Message 1 and 3 do not have same JSON content", strcompare(messageToJSON(msg1, NULL), messageToJSON(msg3, NULL)) == 0);

		messageDestroy(&msg1);
		messageDestroy(&msg2);
		messageDestroy(&msg3);
		messageFactoryDestroy(&factory);
	}

	// Off-nominal test
	{
		testCheckBool("Expected NULL XML string", messageToJSON(NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	statusDestroy(&status);
}


void test_messageGetSize()
{
	GMSEC_INFO("Test messageGetSize()");

	GMSEC_Status status = statusCreate();

	// Nominal test
	{
		GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
		GMSEC_Message        msg1    = messageFactoryCreateMessage(factory, "HB", NULL);
		GMSEC_Message        msg2    = messageCreateCopy(msg1, NULL);

		testCheckBool("Message 1 has unexpected size", messageGetSize(msg1, status) > 0);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);

		testCheckBool("Message 2 has unexpected size", messageGetSize(msg2, NULL) > 0);

		testCheckBool("Messages should have the same size", messageGetSize(msg1, NULL) == messageGetSize(msg2, NULL));

		messageDestroy(&msg1);
		messageDestroy(&msg2);
		messageFactoryDestroy(&factory);
	}

	// Off-nominal test
	{
		testCheckBool("Unexpected message size", messageGetSize(NULL, status) == 0);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	statusDestroy(&status);
}


void test_messageGetFieldIterator()
{
	GMSEC_INFO("Test messageGetFieldIterator()");

	GMSEC_Status status = statusCreate();

	// Nominal test
	{
		GMSEC_MessageFactory       factory = messageFactoryCreate(NULL);
		GMSEC_Message              msg     = messageFactoryCreateMessage(factory, "HB", NULL);
		GMSEC_MessageFieldIterator iter    = messageGetFieldIterator(msg, status);

		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Message Field Iterator is NULL", iter != NULL);

		messageDestroy(&msg);
		messageFactoryDestroy(&factory);
	}

	// Off-nominal test
	{
		GMSEC_MessageFieldIterator iter = messageGetFieldIterator(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Message Field Iterator should be NULL", iter == NULL);
	}

	statusDestroy(&status);
}


int test_Message()
{
	testSetDescription("Test Message");

	test_messageCreate();
	test_messageCreateCopy();
	test_messageAcknowledge();
	test_messageDestroy();
	test_messageGetSchemaID();
	test_messageGetVersion();
	test_messageGetSchemaLevel();
	test_messageIsCompliant();
	test_messageRegisterMessageValidator();
	test_messageSetFieldValue();    // also tests other variances of the function (e.g. I32, U32, etc.)
	test_messageSetConfig();        // also tests messageGetConfig()
	test_messageSetSubject();
	test_messageGetSubject();
	test_messageSetKind();
	test_messageGetKind();
	test_messageAddField();
	test_messageAddXXXField();
	test_messageAddFields();
	test_messageClearFields();
	test_messageClearField();
	test_messageHasField();
	test_messageGetField();
	test_messageGetFieldType();
	test_messageGetStringValue();
	test_messageGetBooleanValue();
	test_messageGetI16Value();
	test_messageGetI32Value();
	test_messageGetI64Value();
	test_messageGetU16Value();
	test_messageGetU32Value();
	test_messageGetU64Value();
	test_messageGetF64Value();
	test_messageGetBinaryField();
	test_messageGetBooleanField();
	test_messageGetCharField();
	test_messageGetF32Field();
	test_messageGetF64Field();
	test_messageGetI8Field();
	test_messageGetI16Field();
	test_messageGetI32Field();
	test_messageGetI64Field();
	test_messageGetU8Field();
	test_messageGetU16Field();
	test_messageGetU32Field();
	test_messageGetU64Field();
	test_messageGetStringField();
	test_messageGetFieldCount();
	test_messageCopyFields();
	test_messageToXML();
	test_messageToJSON();
	test_messageGetSize();
	test_messageGetFieldIterator();

	return 0;
}
TEST_DRIVER(test_Message)
