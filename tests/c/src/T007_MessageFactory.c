#include "TestCase.h"


void test_messageFactoryCreate()
{
	GMSEC_INFO("Test messageFactoryCreate()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(status);

	testCheckBool("Unexpected error creating factory", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected specification version", specificationGetVersion(messageFactoryGetSpecification(factory, NULL), NULL) == GMSEC_MSG_SPEC_CURRENT);
	testCheckBool("Unexpected specification schema level", specificationGetSchemaLevel(messageFactoryGetSpecification(factory, NULL), NULL) == GMSEC_SCHEMA_LEVEL_0);

	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageFactoryCreateUsingConfig()
{
	GMSEC_INFO("Test messageFactoryCreateUsingConfig()");

	GMSEC_Status status = statusCreate();

	// Nominal test
	{
		GMSEC_Config         config  = configCreateUsingData("gmsec-schema-level=1", KEY_VALUE_DATA, NULL);
		GMSEC_MessageFactory factory = messageFactoryCreateUsingConfig(config, status);

		testCheckBool("Unexpected error creating factory", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected specification version", specificationGetVersion(messageFactoryGetSpecification(factory, NULL), NULL) == GMSEC_MSG_SPEC_CURRENT);
		testCheckBool("Unexpected specification schema level", specificationGetSchemaLevel(messageFactoryGetSpecification(factory, NULL), NULL) == GMSEC_SCHEMA_LEVEL_1);

		messageFactoryDestroy(&factory);
		configDestroy(&config);
	}

	// Off-nominal test(s)
	{
		// NULL Config
		GMSEC_MessageFactory factory = messageFactoryCreateUsingConfig(NULL, status);

		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected NULL factory", factory == NULL);
	}

	{
		// Unknown specification
		GMSEC_Config         config  = configCreateUsingData("gmsec-specification-version=201500", KEY_VALUE_DATA, NULL);
		GMSEC_MessageFactory factory = messageFactoryCreateUsingConfig(config, status);

		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected NULL factory", factory == NULL);

		configDestroy(&config);
	}

	statusDestroy(&status);
}


void test_messageFactoryDestroy()
{
	GMSEC_INFO("Test messageFactoryDestroy()");

	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);

	testCheckBool("Expected non-NULL factory", factory != NULL);

	messageFactoryDestroy(&factory);

	testCheckBool("Expected NULL factory", factory == NULL);
}


void test_messageFactorySetStandardFields()
{
	GMSEC_INFO("Test messageFactorySetStandardFields()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);

	// Nominal test
	{
		setStandardFields(factory);

		GMSEC_Message msg = messageFactoryCreateSimpleMessage(factory, NULL);

		verifyStandardFields(msg);

		messageDestroy(&msg);
	}

	// Off-nominal tests
	{
		// MessageFactory is NULL
		messageFactorySetStandardFields(NULL, NULL, 0, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// Field list is NULL
		messageFactorySetStandardFields(factory, NULL, 0, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	}

	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageFactoryClearStandardFields()
{
	GMSEC_INFO("Test messageFactoryClearStandardFields()");

	GMSEC_Status status = statusCreate();

	// Nominal test
	{
		GMSEC_MessageFactory factory = messageFactoryCreate(NULL);

		setStandardFields(factory);

		messageFactoryClearStandardFields(factory, NULL);

		GMSEC_Message msg = messageFactoryCreateSimpleMessage(factory, NULL);

		testCheckBool("Expected field count to be 0 (zero)", messageGetFieldCount(msg, NULL) == 0);

		messageDestroy(&msg);
		messageFactoryDestroy(&factory);
	}

	// Off-nominal test
	{
		// MessageFactory is NULL
		messageFactoryClearStandardFields(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	statusDestroy(&status);
}


void test_messageFactorySetMessageConfig()
{
	GMSEC_INFO("Test messageFactorySetMessageConfig()");

	GMSEC_Status         status    = statusCreate();       
	GMSEC_Config         msgConfig = configCreateUsingData("tracking=false", KEY_VALUE_DATA, NULL);
	GMSEC_MessageFactory factory   = messageFactoryCreate(NULL);

	// Nominal test
	{
		messageFactorySetMessageConfig(factory, msgConfig, status);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);

		GMSEC_Message msg = messageFactoryCreateSimpleMessage(factory, NULL);
		testCheckBool("Unexpected message config", strcompare(configToXML(messageGetConfig(msg, NULL), NULL), configToXML(msgConfig, NULL)) == 0);

		messageDestroy(&msg);
	}

	// Off-nominal test
	{
		// MessageFactory is NULL
		messageFactorySetMessageConfig(NULL, msgConfig, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// Config is NULL
		messageFactorySetMessageConfig(factory, NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	messageFactoryDestroy(&factory);
	configDestroy(&msgConfig);
	statusDestroy(&status);
}


void test_messageFactoryCreateSimpleMessage()
{
	GMSEC_INFO("Test messageFactoryCreateSimpleMessage()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);

	// Nominal test
	{
		GMSEC_Message msg = messageFactoryCreateSimpleMessage(factory, status);

		testCheckBool("Unexpexted error", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Expected no fields in message", messageGetFieldCount(msg, NULL) == 0);

		messageDestroy(&msg);
	}

	// Off-nominal test(s)
	{
		// NULL MessageFactory
		GMSEC_Message msg = messageFactoryCreateSimpleMessage(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected a NULL message", msg == NULL);
	}

	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageFactoryCreateMessage()
{
	GMSEC_INFO("Test messageFactoryCreateMessage()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg;

	// Nominal test (valid schema ID given)
	{
		msg = messageFactoryCreateMessage(factory, "HB", status);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Message is NULL", msg != NULL);
		testCheckBool("Unexpected MESSAGE-TYPE", strcompare(messageGetStringValue(msg, "MESSAGE-TYPE", NULL), "MSG") == 0);
		testCheckBool("Unexpected MESSAGE-SUBTYPE", strcompare(messageGetStringValue(msg, "MESSAGE-SUBTYPE", NULL), "HB") == 0);
		messageDestroy(&msg);
	}

	// Off-nominal test(s)
	{
		// Invalid schema ID given
		msg = messageFactoryCreateMessage(factory, "BOGUS", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Message is not NULL", msg == NULL);
	}

	{
		// Empty-string schema ID given
		msg = messageFactoryCreateMessage(factory, "", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Message is not NULL", msg == NULL);
	}

	{
		// NULL schema ID given
		msg = messageFactoryCreateMessage(factory, NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Message is not NULL", msg == NULL);
	}

	{
		// NULL MessageFactory
		msg = messageFactoryCreateMessage(NULL, "HB", status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected a NULL message", msg == NULL);
	}

	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageFactoryFromData()
{
	GMSEC_INFO("Test messageFactoryFromData()");

	const char* xml =
		"<MESSAGE SUBJECT=\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\" KIND=\"PUBLISH\">\n"
			"\t<FIELD NAME=\"COMPONENT\" TYPE=\"STRING\">GMPUB</FIELD>\n"
			"\t<FIELD NAME=\"CONSTELLATION-ID\" TYPE=\"STRING\">CNS1</FIELD>\n"
			"\t<FIELD NAME=\"CONTENT-VERSION\" TYPE=\"F32\" BITS=\"44FC6000\">2019</FIELD>\n"
			"\t<FIELD NAME=\"COUNTER\" TYPE=\"U16\">1</FIELD>\n"
			"\t<FIELD NAME=\"DOMAIN1\" TYPE=\"STRING\">DOM1</FIELD>\n"
			"\t<FIELD NAME=\"DOMAIN2\" TYPE=\"STRING\">DOM2</FIELD>\n"
			"\t<FIELD NAME=\"FACILITY\" TYPE=\"STRING\">MY-FACILITY</FIELD>\n"
			"\t<FIELD NAME=\"HEADER-VERSION\" TYPE=\"F32\" BITS=\"44FC6000\">2019</FIELD>\n"
			"\t<FIELD NAME=\"MESSAGE-SUBTYPE\" TYPE=\"STRING\">HB</FIELD>\n"
			"\t<FIELD NAME=\"MESSAGE-TYPE\" TYPE=\"STRING\">MSG</FIELD>\n"
			"\t<FIELD NAME=\"MISSION-ID\" TYPE=\"STRING\">MSSN</FIELD>\n"
			"\t<FIELD NAME=\"PUB-RATE\" TYPE=\"U16\">1</FIELD>\n"
			"\t<FIELD NAME=\"SAT-ID-LOGICAL\" TYPE=\"STRING\">SAT1</FIELD>\n"
			"\t<FIELD NAME=\"SAT-ID-PHYSICAL\" TYPE=\"STRING\">SAT1</FIELD>\n"
			"\t<FIELD NAME=\"SPECIFICATION\" TYPE=\"STRING\">GMSEC</FIELD>\n"
		"</MESSAGE>";

	GMSEC_Status         status  = statusCreate();
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg;

	// Nominal test
	{
		msg = messageFactoryFromData(factory, xml, XML_DATA, status);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Message is NULL", msg != NULL);
		testCheckBool("Message has bad subject", strcompare(messageGetSubject(msg, NULL), "GMSEC.MSSN.SAT1.MSG.HB.GMPUB") == 0);
		testCheckBool("Message has bad kind", messageGetKind(msg, NULL) == GMSEC_PUBLISH);
		testCheckBool("Missing MESSAGE-TYPE", messageHasField(msg, "MESSAGE-TYPE", NULL) == GMSEC_TRUE);
		testCheckBool("Missing MESSAGE-SUBTYPE", messageHasField(msg, "MESSAGE-SUBTYPE", NULL) == GMSEC_TRUE);
		testCheckBool("Missing CONTENT-VERSION", messageHasField(msg, "CONTENT-VERSION", NULL) == GMSEC_TRUE);
		testCheckBool("Missing HEADER-VERSION", messageHasField(msg, "HEADER-VERSION", NULL) == GMSEC_TRUE);
		testCheckBool("Missing PUB-RATE", messageHasField(msg, "PUB-RATE", NULL) == GMSEC_TRUE);
		messageDestroy(&msg);
	}

	// Off-nominal tests
	{
		msg = messageFactoryFromData(factory, NULL, XML_DATA, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Message should be NULL", msg == NULL);
	}

	{
		msg = messageFactoryFromData(factory, "", XML_DATA, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Message should be NULL", msg == NULL);
	}

	{
		// NULL MessageFactory
		msg = messageFactoryFromData(NULL, xml, XML_DATA, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected a NULL message", msg == NULL);
	}

	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_messageFactoryGetSpecification()
{
	GMSEC_INFO("Test messageFactoryGetSpecification()");

	GMSEC_Status status = statusCreate();

	// Nominal test
	{
		GMSEC_Config         config   = configCreateUsingData("gmsec-specification-version=201900", KEY_VALUE_DATA, NULL);
		GMSEC_MessageFactory factory1 = messageFactoryCreate(NULL);
		GMSEC_MessageFactory factory2 = messageFactoryCreateUsingConfig(config, NULL);

		testCheckBool("Unexpected specification version", specificationGetVersion(messageFactoryGetSpecification(factory1, NULL), NULL) == GMSEC_MSG_SPEC_CURRENT);
		testCheckBool("Unexpected specification version", specificationGetVersion(messageFactoryGetSpecification(factory2, NULL), NULL) == GMSEC_MSG_SPEC_2019_00);

		messageFactoryDestroy(&factory1);
		messageFactoryDestroy(&factory2);
		configDestroy(&config);
	}

	// Off-nominal test(s)
	{
		// NULL MessageFactory
		GMSEC_Specification spec = messageFactoryGetSpecification(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected a NULL spec", spec == NULL);
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


void test_messageFactoryRegisterMessageValidator()
{
	GMSEC_INFO("Test messageFactoryRegisterMessageValidator()");

	GMSEC_Status status = statusCreate();

	// Nominal tests
	{
		GMSEC_MessageFactory factory1 = messageFactoryCreate(NULL);
		GMSEC_MessageFactory factory2 = messageFactoryCreate(NULL);

		setStandardFields(factory1);

		messageFactoryRegisterMessageValidator(factory1, nominalValidator, status);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);

		messageFactoryRegisterMessageValidator(factory2, offNominalValidator, status);
		testCheckBool("Unexpected error", statusHasError(status) == GMSEC_FALSE);

		//o Message is compliant (has all standard fields)
		GMSEC_Message msg1 = messageFactoryCreateMessage(factory1, "HB", NULL);
		messageIsCompliant(msg1, status);
		testCheckBool("Expected message to be compliant", statusHasError(status) == GMSEC_FALSE);

		//o Message is not compliant (missing standard fields)
		GMSEC_Message msg2 = messageFactoryCreateMessage(factory2, "HB", NULL);
		messageIsCompliant(msg2, status);
		testCheckBool("Expected message to be compliant", statusHasError(status) == GMSEC_TRUE);
		testCheckBool(statusGet(status), strcontains(statusGet(status), "My validator is not happy with the message") == 0);

		messageDestroy(&msg1);
		messageDestroy(&msg2);
		messageFactoryDestroy(&factory1);
		messageFactoryDestroy(&factory2);
	}

	// Off-nominal test(s)
	{
		// NULL MessageFactory
		messageFactoryRegisterMessageValidator(NULL, nominalValidator, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// NULL MessageValidator
		GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
		messageFactoryRegisterMessageValidator(factory, NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		messageFactoryDestroy(&factory);
	}

	statusDestroy(&status);
}


int test_MessageFactory()
{
	testSetDescription("Test MessageFactory");

	test_messageFactoryCreate();
	test_messageFactoryCreateUsingConfig();
	test_messageFactoryDestroy();
	test_messageFactorySetStandardFields();
	test_messageFactoryClearStandardFields();
	test_messageFactorySetMessageConfig();
	test_messageFactoryCreateSimpleMessage();
	test_messageFactoryCreateMessage();
	test_messageFactoryFromData();
	test_messageFactoryGetSpecification();
	test_messageFactoryRegisterMessageValidator();

	return 0;
}
TEST_DRIVER(test_MessageFactory)
