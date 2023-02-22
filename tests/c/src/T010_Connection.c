#include "TestCase.h"


// MyEventCallback
//
GMSEC_ConnectionEvent g_event = GMSEC_ALL_EVENTS;

GMSEC_ConnectionEvent getEvent()
{
	return g_event;
}

void resetEvent()
{
	g_event = GMSEC_ALL_EVENTS;
}

void MyEventCallback(GMSEC_Connection conn, GMSEC_Status status, GMSEC_ConnectionEvent event)
{
	g_event = event;
}


// MyReplyCallback
GMSEC_BOOL g_gotReply = GMSEC_FALSE;

GMSEC_BOOL gotReply()
{
	return g_gotReply;
}

GMSEC_BOOL gotTimeout()
{
	return g_event == GMSEC_REQUEST_TIMEOUT_EVENT;
}

void reset()
{
	g_gotReply = GMSEC_FALSE;
}

void MyReplyCallback(GMSEC_Connection conn, GMSEC_Message reqMsg, GMSEC_Message repMsg)
{
	g_gotReply = GMSEC_TRUE;
}


// MyCallback
//
GMSEC_BOOL g_called = GMSEC_FALSE;

GMSEC_BOOL wasCalled()
{
	return g_called;
}

void resetCalled()
{
	g_called = GMSEC_FALSE;
}

void MyCallback(GMSEC_Connection conn, GMSEC_Message msg)
{
	g_called = GMSEC_TRUE;
}


// MyRequestCallback
GMSEC_BOOL g_provideResponse = GMSEC_TRUE;

void provideResponse(GMSEC_BOOL response)
{
	g_provideResponse = response;
}

void MyRequestCallback(GMSEC_Connection conn, GMSEC_Message msg)
{
	if (messageGetKind(msg, NULL) != GMSEC_REQUEST)
	{
		testCheckBool("Received unexpected message kind", GMSEC_FALSE);
		return;
	}

	GMSEC_Status status = statusCreate();

	if (g_provideResponse == GMSEC_TRUE)
	{
		char          tmp[OUT_STR_LEN];
		GMSEC_Message repMsg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "RESP.DIR", NULL);
		messageSetSubject(repMsg, testGetSubjectCharPtr("RESP.DIR", tmp), NULL);

		messageSetFieldValueI32(repMsg, "RESPONSE-STATUS", GMSEC_SUCCESSFUL_COMPLETION, NULL);

		if (messageHasField(msg, "COMPONENT", NULL) == GMSEC_TRUE)
		{
			messageAddStringField(repMsg, "DESTINATION-COMPONENT", messageGetStringValue(msg, "COMPONENT", NULL), NULL);
		}
		if (messageHasField(msg, "DESTINATION-COMPONENT", NULL) == GMSEC_TRUE)
		{
			messageAddStringField(repMsg, "COMPONENT", messageGetStringValue(msg, "DESTINATION-COMPONENT", NULL), NULL);
		}
		else
		{
			messageAddStringField(repMsg, "COMPONENT", "RESPONDER", NULL);
		}
		if (messageHasField(msg, "REQUEST-ID", NULL) == GMSEC_TRUE)
		{
			messageAddField(repMsg, messageGetField(msg, "REQUEST-ID", NULL), NULL);
		}

		connectionReply(conn, msg, repMsg, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		messageDestroy(&repMsg);
	}
	else
	{
		g_provideResponse = GMSEC_TRUE;
	}

	statusDestroy(&status);
}


void test_connectionCreate()
{
	GMSEC_INFO("Test connectionCreate()");

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = testGetConfig();

	// Nominal test
	GMSEC_Connection conn = connectionCreate(config, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testRequireBool("Connection is NULL", conn != NULL);
	connectionDestroy(&conn);

	// Off-nominal tests

	//o missing mw-id
	config = configCreate();
	conn = connectionCreate(config, status);
	testRequireBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	testRequireBool("Connection is not NULL", conn == NULL);
	configDestroy(&config);

	//o bogus middleware
	config = configCreateUsingData("mw-id=bogus", KEY_VALUE_DATA, NULL);
	conn = connectionCreate(config, status);
	testRequireBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	testRequireBool("Connection is not NULL", conn == NULL);
	configDestroy(&config);

	//o NULL config
	conn = connectionCreate(NULL, status);
	testRequireBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	testRequireBool("Connection is not NULL", conn == NULL);

	statusDestroy(&status);
}


void test_connectionCreateWithFactory()
{
	GMSEC_INFO("Test connectionCreateWithFactory()");

	GMSEC_Status         status = statusCreate();
	GMSEC_Config         config = testGetConfig();
	GMSEC_MessageFactory factory = messageFactoryCreateUsingConfig(config, status);

	// Nominal test
	{
		GMSEC_Connection conn = connectionCreateWithFactory(config, factory, status);
		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		testRequireBool("Connection is NULL", conn != NULL);
		connectionDestroy(&conn);
	}

	// Off-nominal test
	{
		// Config is NULL
		GMSEC_Connection conn = connectionCreateWithFactory(NULL, factory, status);
		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testRequireBool("Connection should be NULL", conn == NULL);

		// MessageFactory is NULL
		conn = connectionCreateWithFactory(config, NULL, status);
		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testRequireBool("Connection should be NULL", conn == NULL);

		// Bogus middleware
		GMSEC_Config config2 = configCreateUsingData("mw-id=bogus", KEY_VALUE_DATA, NULL);
		conn = connectionCreateWithFactory(config2, factory, status);
		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testRequireBool("Connection should be NULL", conn == NULL);
		configDestroy(&config2);
	}

	messageFactoryDestroy(&factory);
	statusDestroy(&status);
}


void test_connectionDestroy()
{
	GMSEC_INFO("Test connectionDestroy()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);

	testRequireBool("Connection is NULL", conn != NULL);

	connectionDestroy(&conn);

	testRequireBool("Connection is not NULL", conn == NULL);

	statusDestroy(&status);
}


void test_connectionGetAPIVersion()
{
	GMSEC_INFO("Test connectionGetAPIVersion()");

	testRequireBool("Expected to get a non-NULL string", connectionGetAPIVersion() != NULL);
	testCheckBool("Unexpected version number", strcontains(connectionGetAPIVersion(), GMSEC_VERSION_NUMBER) == 0);
}


void test_connectionConnect()
{
	GMSEC_INFO("Test connectionConnect()");

	GMSEC_Status status = statusCreate();

	// Nominal test
	{
		GMSEC_Config     config = testGetConfig();
		GMSEC_Connection conn   = connectionCreate(config, status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		connectionConnect(conn, status);

		testRequireBool("Cannot connect", statusHasError(status) == GMSEC_FALSE);

		connectionDisconnect(conn, NULL);

		connectionDestroy(&conn);
	}

	// Off-nominal test
	{
		GMSEC_Config     config = configCreateUsingData("mw-id=bolt server=gs580s-gc764mw1:1234", KEY_VALUE_DATA, NULL);
		GMSEC_Connection conn   = connectionCreate(config, NULL);

		connectionConnect(conn, status);

		testCheckBool("Expected to NOT be initialized", statusHasError(status) == GMSEC_TRUE);

		connectionDestroy(&conn);
		configDestroy(&config);
	}

	// Off-nominal test
	{
		connectionConnect(NULL, status);

		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	}

	statusDestroy(&status);
}


void test_connectionDisconnect()
{
	GMSEC_INFO("Test connectionDisconnect()");

	GMSEC_Status status = statusCreate();

	// Nominal test
	{
		GMSEC_Config     config = testGetConfig();
		GMSEC_Connection conn   = connectionCreate(config, status);

		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		connectionConnect(conn, NULL);

		connectionDisconnect(conn, status);

		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		connectionDestroy(&conn);
	}

	// Off-nominal test
	{
		connectionDisconnect(NULL, status);

		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	}

	statusDestroy(&status);
}


void test_connectionGetLibraryName()
{
	GMSEC_INFO("Test connectionGetLibraryName()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	char             libname[256];

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	printToString(libname, "gmsec_%s", configGetValueOrDefault(testGetConfig(), "mw-id", "unknown", NULL));

	// Nominal test
	testCheckBool("Unexpected library name", strcontains(connectionGetLibraryName(conn, NULL), libname) == 0);

	// Off-nominal test
	(void) connectionGetLibraryName(NULL, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionGetLibraryVersion()
{
	GMSEC_INFO("Test connectionGetLibraryVersion()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	// Nominal test
	{
		testCheckBool("Expected a library version string", connectionGetLibraryVersion(conn, NULL) != NULL);
	}

	// Off-nominal test
	{
		testCheckBool("Expected NULL library version", connectionGetLibraryVersion(NULL, status) == NULL);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}

	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionGetMessageFactory()
{
	GMSEC_INFO("Test connectionGetMessageFactory()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	// Nominal test
	testCheckBool("Expected to get MessageFactory", connectionGetMessageFactory(conn, NULL) != NULL);

	// Off-nominal test
	(void) connectionGetMessageFactory(NULL, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionRegisterEventCallback()
{
	GMSEC_INFO("Test connectionRegisterEventCallback()");

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = testGetConfig();

    const char* mw = configGetValueOrDefault(testGetConfig(), "mw-id", "unknown", NULL);

    if (strcompare(mw, "unknown") == 0     ||
	    strcompare(mw, "mb") == 0          ||
	    strcompare(mw, "generic_jms") == 0 ||
	    strcompare(mw, "zeromq413") == 0   ||
	    strcompare(mw, "zeromq432") == 0)
    {
		/**
         * Test is being skipped because of lame middleware wrapper
         * which does not support event notification upon connect
         * and/or disconnect to/from the GMSEC Bus.
		 */
        return;
    }

	GMSEC_Connection conn = connectionCreate(config, status);

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	// Nominal test
	connectionRegisterEventCallback(conn, GMSEC_ALL_EVENTS, MyEventCallback, status);

	connectionConnect(conn, NULL);

	testCheckBool("Event callback received wrong event", getEvent() == GMSEC_CONN_SUCCESSFUL_EVENT);

	resetEvent();

	connectionDisconnect(conn, NULL);

	testCheckBool("Event callback received wrong event", getEvent() == GMSEC_CONN_BROKEN_EVENT);

	resetEvent();

	// Off-nominal test
	connectionRegisterEventCallback(NULL, GMSEC_ALL_EVENTS, MyEventCallback, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	connectionRegisterEventCallback(conn, GMSEC_ALL_EVENTS, NULL, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionSubscribe()
{
	GMSEC_INFO("Test connectionSubscribe()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	char             tmp[OUT_STR_LEN];

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_SubscriptionInfo info1 = connectionSubscribe(conn, testGetSubjectCharPtr("*.BAR", tmp), status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	testCheckBool("Unexpected subscription topic in info", strcompare(testGetSubjectCharPtr("*.BAR", tmp), subscriptionInfoGetSubjectPattern(info1)) == 0);

	GMSEC_SubscriptionInfo info2 = connectionSubscribe(conn, testGetSubjectCharPtr("FOO.BAZ", tmp), status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_Message msg1 = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", NULL);
	GMSEC_Message msg2 = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", NULL);
	GMSEC_Message msg3 = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", NULL);

	messageSetSubject(msg1, testGetSubjectCharPtr("GIN.BAR", tmp), NULL);
	messageSetSubject(msg2, testGetSubjectCharPtr("FOO.BAZ", tmp), NULL);
	messageSetSubject(msg3, testGetSubjectCharPtr("FOO.GAZ", tmp), NULL);

	// Nominal tests
	{
		connectionPublish(conn, msg1, NULL);

		GMSEC_Message msg = connectionReceive(conn, 5000, NULL);
		testRequireBool("Failed to receive message", msg != NULL);
		testCheckBool("Unexpected message subject", strcompare(messageGetSubject(msg, NULL), testGetSubjectCharPtr("GIN.BAR", tmp)) == 0);
		messageDestroy(&msg);
	}

	{
		connectionPublish(conn, msg2, NULL);

		GMSEC_Message msg = connectionReceive(conn, 5000, NULL);
		testRequireBool("Failed to receive message", msg != NULL);
		testCheckBool("Unexpected message subject", strcompare(messageGetSubject(msg, NULL), testGetSubjectCharPtr("FOO.BAZ", tmp)) == 0);
		messageDestroy(&msg);
	}

	{
		connectionPublish(conn, msg3, NULL);

		GMSEC_Message msg = connectionReceive(conn, 5000, NULL);
		testCheckBool("Was not expecting to receive a message", msg == NULL);

		if (msg != NULL)
		{
			messageDestroy(&msg);
		}
	}

	connectionUnsubscribe(conn, &info1, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionUnsubscribe(conn, &info2, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	// Off-nominal tests
	connectionSubscribe(NULL, "GMSEC.>", status);
	testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);

	connectionSubscribe(conn, NULL, status);
	testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);

	messageDestroy(&msg1);
	messageDestroy(&msg2);
	messageDestroy(&msg3);

	connectionDisconnect(conn, NULL);

	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionSubscribeWithCallback()
{
	GMSEC_INFO("Test connectionSubscribeWithCallback()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	char             tmp[OUT_STR_LEN];

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, NULL);

	GMSEC_SubscriptionInfo info1 = connectionSubscribeWithCallback(conn, testGetSubjectCharPtr("*.BAR", tmp), &MyCallback, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_SubscriptionInfo info2 = connectionSubscribeWithCallback(conn, testGetSubjectCharPtr("FOO.BAZ", tmp), &MyCallback, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionStartAutoDispatch(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_Message msg1 = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", NULL);
	GMSEC_Message msg2 = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", NULL);
	GMSEC_Message msg3 = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", NULL);

	messageSetSubject(msg1, testGetSubjectCharPtr("GIN.BAR", tmp), NULL);
	messageSetSubject(msg2, testGetSubjectCharPtr("FOO.BAZ", tmp), NULL);
	messageSetSubject(msg3, testGetSubjectCharPtr("FOO.GAZ", tmp), NULL);

	// Nominal tests
	{
		connectionPublish(conn, msg1, NULL);

		timeUtilMillisleep(5000);

		testCheckBool("MyCallback was not summoned", wasCalled() == GMSEC_TRUE);

		resetCalled();
	}

	{
		connectionPublish(conn, msg2, NULL);

		timeUtilMillisleep(5000);

		testCheckBool("MyCallback was not summoned", wasCalled() == GMSEC_TRUE);

		resetCalled();
	}

	{
		connectionPublish(conn, msg3, NULL);

		timeUtilMillisleep(5000);

		testCheckBool("MyCallback was unexpected summoned", wasCalled() == GMSEC_FALSE);

		resetCalled();
	}

	connectionUnsubscribe(conn, &info1, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionUnsubscribe(conn, &info2, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionStopAutoDispatch(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	messageDestroy(&msg1);
	messageDestroy(&msg2);
	messageDestroy(&msg3);

	// Off-nominal test
	{
		// Connection is NULL
		(void) connectionSubscribeWithCallback(NULL, "GMSEC.>", &MyCallback, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Callback is NULL
		(void) connectionSubscribeWithCallback(conn, "GMSEC.>", NULL, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	}
	//TODO: Not working because core API creates new internal C-callback for each subscription
	/*
	{
		// Duplicate subscription
		GMSEC_SubscriptionInfo info3 = connectionSubscribeWithCallback(conn, "GMSEC.>", &MyCallback, status);
		testCheckBool("An error was not expected", statusHasError(status) == GMSEC_FALSE);

		(void) connectionSubscribeWithCallback(conn, "GMSEC.>", &MyCallback, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);

		connectionUnsubscribe(conn, &info3, status);
	}
	*/
	{
		// Not connected
		connectionDisconnect(conn, NULL);

		(void) connectionSubscribeWithCallback(conn, "GMSEC.>", &MyCallback, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}

	connectionDestroy(&conn);
	testCheckBool("Connection should be NULL", conn == NULL);

	statusDestroy(&status);
}


void test_connectionSubscribeWithConfig()
{
	GMSEC_INFO("Test connectionSubscribeWithConfig()");

	GMSEC_Status     status    = statusCreate();
	GMSEC_Config     config    = testGetConfig();
	GMSEC_Config     subconfig = configCreateUsingData("foo=bar", KEY_VALUE_DATA, NULL);
	GMSEC_Connection conn      = connectionCreate(config, status);
	char             tmp[OUT_STR_LEN];

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, NULL);

	GMSEC_SubscriptionInfo info1 = connectionSubscribeWithConfig(conn, testGetSubjectCharPtr("*.BAR", tmp), subconfig, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_SubscriptionInfo info2 = connectionSubscribeWithConfig(conn, testGetSubjectCharPtr("FOO.BAZ", tmp), subconfig, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_Message msg1 = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", NULL);
	GMSEC_Message msg2 = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", NULL);
	GMSEC_Message msg3 = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", NULL);

	messageSetSubject(msg1, testGetSubjectCharPtr("GIN.BAR", tmp), NULL);
	messageSetSubject(msg2, testGetSubjectCharPtr("FOO.BAZ", tmp), NULL);
	messageSetSubject(msg3, testGetSubjectCharPtr("FOO.GAZ", tmp), NULL);

	// Nominal tests
	{
		connectionPublish(conn, msg1, NULL);

		GMSEC_Message msg = connectionReceive(conn, 5000, NULL);
		testRequireBool("Failed to receive message", msg != NULL);
		testCheckBool("Unexpected message subject", strcompare(messageGetSubject(msg, NULL), testGetSubjectCharPtr("GIN.BAR", tmp)) == 0);
		messageDestroy(&msg);
	}

	{
		connectionPublish(conn, msg2, NULL);

		GMSEC_Message msg = connectionReceive(conn, 5000, NULL);
		testRequireBool("Failed to receive message", msg != NULL);
		testCheckBool("Unexpected message subject", strcompare(messageGetSubject(msg, NULL), testGetSubjectCharPtr("FOO.BAZ", tmp)) == 0);
		messageDestroy(&msg);
	}

	{
		connectionPublish(conn, msg3, NULL);

		GMSEC_Message msg = connectionReceive(conn, 5000, NULL);
		testCheckBool("Was not expecting to receive a message", msg == NULL);

		if (msg != NULL)
		{
			messageDestroy(&msg);
		}
	}

	connectionUnsubscribe(conn, &info1, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionUnsubscribe(conn, &info2, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	messageDestroy(&msg1);
	messageDestroy(&msg2);
	messageDestroy(&msg3);

	// Off-nominal tests
	{
		// Connection is NULL
		connectionSubscribeWithConfig(NULL, "GMSEC.>", subconfig, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// Subscription topic is NULL
		connectionSubscribeWithConfig(conn, NULL, subconfig, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// Config is NULL
		connectionSubscribeWithConfig(conn, "GMSEC.>", NULL, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	}

	connectionDisconnect(conn, NULL);
	connectionDestroy(&conn);
	testCheckBool("Connection should be NULL", conn == NULL);

	configDestroy(&subconfig);
	statusDestroy(&status);
}


void test_connectionSubscribeWithConfigAndCallback()
{
	GMSEC_INFO("Test connectionSubscribeWithConfigAndCallback()");

	GMSEC_Status     status    = statusCreate();
	GMSEC_Config     config    = testGetConfig();
	GMSEC_Config     subconfig = configCreateUsingData("foo=bar", KEY_VALUE_DATA, NULL);
	GMSEC_Connection conn      = connectionCreate(config, status);
	char             tmp[OUT_STR_LEN];

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, NULL);

	GMSEC_SubscriptionInfo info1 = connectionSubscribeWithConfigAndCallback(conn, testGetSubjectCharPtr("*.BAR", tmp), subconfig, &MyCallback, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_SubscriptionInfo info2 = connectionSubscribeWithConfigAndCallback(conn, testGetSubjectCharPtr("FOO.BAZ", tmp), subconfig, &MyCallback, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionStartAutoDispatch(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_Message msg1 = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", NULL);
	GMSEC_Message msg2 = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", NULL);
	GMSEC_Message msg3 = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", NULL);

	messageSetSubject(msg1, testGetSubjectCharPtr("GIN.BAR", tmp), NULL);
	messageSetSubject(msg2, testGetSubjectCharPtr("FOO.BAZ", tmp), NULL);
	messageSetSubject(msg3, testGetSubjectCharPtr("FOO.GAZ", tmp), NULL);

	// Nominal tests
	{
		connectionPublish(conn, msg1, NULL);

		timeUtilMillisleep(5000);

		testCheckBool("MyCallback was not summoned", wasCalled() == GMSEC_TRUE);

		resetCalled();
	}

	{
		connectionPublish(conn, msg2, NULL);

		timeUtilMillisleep(5000);

		testCheckBool("MyCallback was not summoned", wasCalled() == GMSEC_TRUE);

		resetCalled();
	}

	{
		connectionPublish(conn, msg3, NULL);

		timeUtilMillisleep(5000);

		testCheckBool("MyCallback was unexpected summoned", wasCalled() == GMSEC_FALSE);

		resetCalled();
	}

	connectionUnsubscribe(conn, &info1, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionUnsubscribe(conn, &info2, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionStopAutoDispatch(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	messageDestroy(&msg1);
	messageDestroy(&msg2);
	messageDestroy(&msg3);

	// Off-nominal test
	(void) connectionSubscribeWithConfigAndCallback(NULL, "GMSEC.>", subconfig, &MyCallback, status);
	testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);

	(void) connectionSubscribeWithConfigAndCallback(conn, NULL, subconfig, &MyCallback, status);
	testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);

	(void) connectionSubscribeWithConfigAndCallback(conn, "GMSEC.>", NULL, &MyCallback, status);
	testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);

	(void) connectionSubscribeWithConfigAndCallback(conn, "GMSEC.>", subconfig, NULL, status);
	testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);

	connectionDisconnect(conn, NULL);
	connectionDestroy(&conn);
	testCheckBool("Connection should be NULL", conn == NULL);

	configDestroy(&subconfig);
	statusDestroy(&status);
}


void test_connectionUnsubscribe()
{
	GMSEC_INFO("Test connectionUnsubscribe()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn1  = connectionCreate(config, status);
	GMSEC_Connection conn2  = connectionCreate(config, status);
	char             tmp[OUT_STR_LEN];

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn1, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	connectionConnect(conn2, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_SubscriptionInfo info1 = connectionSubscribe(conn1, testGetSubjectCharPtr("FOO.BAR", tmp), status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_SubscriptionInfo info2 = connectionSubscribe(conn2, testGetSubjectCharPtr("FOO.BAR", tmp), status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionUnsubscribe(conn1, &info1, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testCheckBool("SubscriptionInfo object not NULL", info1 == NULL);

	GMSEC_Message hb = messageFactoryCreateMessage(connectionGetMessageFactory(conn1, NULL), "HB", NULL);
	messageSetSubject(hb, testGetSubjectCharPtr("FOO.BAR", tmp), NULL);

	connectionPublish(conn1, hb, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_Message msg = connectionReceive(conn1, 5000, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Received unexpected message", msg == NULL);

	// Off-nominal tests
	{
		// Use SubscriptionInfo from another Connection
		connectionUnsubscribe(conn1, &info2, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("SubscriptionInfo object is NULL", info2 != NULL);

		connectionUnsubscribe(conn2, &info2, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		testCheckBool("SubscriptionInfo object not NULL", info2 == NULL);
	}

	{
		connectionUnsubscribe(NULL, &info1, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	}

	{
		connectionUnsubscribe(conn1, NULL, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	}

	connectionDisconnect(conn1, NULL);
	connectionDisconnect(conn2, NULL);

	messageDestroy(&hb);
	connectionDestroy(&conn1);
	connectionDestroy(&conn2);
	statusDestroy(&status);
}


void test_connectionStartAutoDispatch()
{
	GMSEC_INFO("Test connectionStartAutoDispatch()");

	GMSEC_Status status = statusCreate();

	// Nominal tests
	// connectionStartAutoDispatch() already tested within test_connectionSubscribeWithCallback()

	// Off-nominal tests
	{
		// Connection is NULL
		connectionStartAutoDispatch(NULL, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// Failed to connect to the bus before starting message dispatcher
		GMSEC_Config     config = testGetConfig();
		GMSEC_Connection conn   = connectionCreate(config, status);
		testRequireBool("Expected an error", statusHasError(status) == GMSEC_FALSE);

		connectionStartAutoDispatch(conn, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		connectionDestroy(&conn);
	}

	statusDestroy(&status);
}


void test_connectionStopAutoDispatch()
{
	GMSEC_INFO("Test connectionStopAutoDispatch()");

	GMSEC_Status status = statusCreate();

	// Nominal test(s)
	// connectionStopAutoDispatch() already tested within test_connectionSubscribeWithCallback()

	// Off-nominal test
	{
		// Connection is NULL
		connectionStopAutoDispatch(NULL, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	}

	{
		// Failed to connect to the bus before stopping message dispatcher
		GMSEC_Config     config = testGetConfig();
		GMSEC_Connection conn   = connectionCreate(config, status);
		testRequireBool("Expected an error", statusHasError(status) == GMSEC_FALSE);

		connectionStopAutoDispatch(conn, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

		connectionDestroy(&conn);
	}

	statusDestroy(&status);
}


void test_connectionPublish()
{
	GMSEC_INFO("Test connectionPublish()");

	GMSEC_Status status = statusCreate();
	GMSEC_Config config = configCreateCopy(testGetConfig(), NULL);
	char         tmp[OUT_STR_LEN];

	// Nominal test
	{
		GMSEC_Connection conn = connectionCreate(config, status);
		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		connectionConnect(conn, status);
		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		setStandardFields(connectionGetMessageFactory(conn, NULL));

		connectionSubscribe(conn, testGetSubjectCharPtr(">", tmp), status);
		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		setStandardFields(connectionGetMessageFactory(conn, NULL));

		GMSEC_Message msg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "HB", NULL);
		messageSetSubject(msg, testGetSubjectCharPtr("HB", tmp), NULL);

		connectionPublish(conn, msg, status);
		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		GMSEC_Message rcvd = connectionReceive(conn, 5000, status);
		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		if (rcvd != NULL)
		{
			testCheckBool("Message has no subject", messageGetSubject(rcvd, NULL) != NULL);
			testCheckBool("Message has unexpected kind", messageGetKind(rcvd, NULL) == GMSEC_PUBLISH);

			verifyStandardFields(rcvd);

			messageDestroy(&rcvd);
		}
		else
		{
			testCheckBool("Failed to receive published message", GMSEC_FALSE);
		}

		connectionDisconnect(conn, NULL);

		messageDestroy(&msg);
		connectionDestroy(&conn);
	}

	// Off-nominal tests
	{
		// Connection is NULL
		GMSEC_Message msg = messageCreate();
		connectionPublish(NULL, msg, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
		messageDestroy(&msg);
	}

	{
		// Message is NULL
		GMSEC_Connection conn = connectionCreate(config, NULL);

		connectionConnect(conn, NULL);

		connectionPublish(conn, NULL, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);

		connectionDestroy(&conn);
	}

	{
		// Message validation fails (missing a standard field)
		configAddValue(config, "gmsec-msg-content-validate", "true", NULL);

		GMSEC_Connection conn = connectionCreate(config, status);
		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		setStandardFields(connectionGetMessageFactory(conn, NULL));

		GMSEC_Message msg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "HB", NULL);
		messageClearField(msg, "CONTENT-VERSION", NULL);

		connectionConnect(conn, status);
		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		connectionPublish(conn, msg, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
		testCheckBool(statusGet(status), strcontains(statusGet(status), "Message Validation Failed") == 0);

		connectionDisconnect(conn, NULL);

		messageDestroy(&msg);
		connectionDestroy(&conn);
	}

	{
		// Connection not established
		GMSEC_Connection conn = connectionCreate(config, NULL);
		GMSEC_Message    msg  = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "HB", NULL);

		connectionPublish(conn, msg, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
		testCheckBool(statusGet(status), strcontains(statusGet(status), "Connection has not been initialized") == 0);

		messageDestroy(&msg);
		connectionDestroy(&conn);
	}

	{
		// Non-PUBLISH kind message
		GMSEC_Connection conn = connectionCreate(config, NULL);
		GMSEC_Message    msg  = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "REQ.DIR", NULL);

		connectionConnect(conn, NULL);

		connectionPublish(conn, msg, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
		testCheckBool(statusGet(status), strcontains(statusGet(status), "Cannot publish message with non-PUBLISH message kind") == 0);

		connectionDisconnect(conn, NULL);

		messageDestroy(&msg);
		connectionDestroy(&conn);
	}

	configDestroy(&config);
	statusDestroy(&status);
}


void test_connectionPublishWithMwConfig()
{
	GMSEC_INFO("Test connectionPublishWithConfig()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	char             subject[OUT_STR_LEN];

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	(void) testGetSubjectCharPtr("MW-CONFIG", subject);

	connectionConnect(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	setStandardFields(connectionGetMessageFactory(conn, NULL));

	GMSEC_Message msg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "HB", NULL);
	messageSetSubject(msg, subject, NULL);

	// Nominal test... provide mwConfig
	{
		GMSEC_Config mwConfig = configCreate();

		connectionSubscribe(conn, subject, status);
		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		connectionPublishWithConfig(conn, msg, mwConfig, status);
		testCheckBool("An error was NOT expected", statusHasError(status) == GMSEC_FALSE);

		GMSEC_Message rcv = connectionReceive(conn, 5000, status);
		testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		testRequireBool("Failed to receive message", rcv != NULL);

		messageDestroy(&rcv);

		configDestroy(&mwConfig);
	}


	// Off-nominal test(s)... missing mwConfig
	{
		connectionPublishWithConfig(conn, msg, NULL, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionRequestWithCallback()
{
	GMSEC_INFO("Test connectionRequestWithCallback()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	char             tmp[OUT_STR_LEN];

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	setStandardFields(connectionGetMessageFactory(conn, NULL));

	GMSEC_Message reqMsg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "REQ.DIR", NULL);
	messageSetSubject(reqMsg, testGetSubjectCharPtr("REQ.DIR", tmp), NULL);
	messageAddStringField(reqMsg, "DIRECTIVE-STRING", "Do something!", NULL);
	messageAddStringField(reqMsg, "DESTINATION-COMPONENT", "RESPONDER", NULL);

	// subscribe to receive the response message
	connectionSubscribe(conn, testGetSubjectCharPtr("RESP.DIR.+", tmp), NULL);

	// subscribe to process the request message
	connectionSubscribeWithCallback(conn, messageGetSubject(reqMsg, NULL), &MyRequestCallback, NULL);
	connectionStartAutoDispatch(conn, NULL);

	// Verify a response is returned
	messageAddU16Field(reqMsg, "REQUEST-ID", (GMSEC_U16) 0, NULL);
	connectionRequestWithCallback(conn, reqMsg, 5000, &MyReplyCallback, &MyEventCallback, GMSEC_REQUEST_REPUBLISH_NEVER, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	timeUtilMillisleep(5500);
	testCheckBool("Expected MyReplyCallback to be summoned", gotReply() == GMSEC_TRUE);
	testCheckBool("Did not expect a timeout event", gotTimeout() == GMSEC_FALSE);
	resetEvent();
	reset();

	// Verify NULL is returned on timeout
	provideResponse(GMSEC_FALSE);
	messageAddU16Field(reqMsg, "REQUEST-ID", (GMSEC_U16) 1, NULL);
	connectionRequestWithCallback(conn, reqMsg, 5000, &MyReplyCallback, &MyEventCallback, GMSEC_REQUEST_REPUBLISH_NEVER, status);
	timeUtilMillisleep(5500);
	testCheckBool("Expected MyReplyCallback not to be summoned", gotReply() == GMSEC_FALSE);
	testCheckBool("Expected a timeout event", gotTimeout() == GMSEC_TRUE);
	resetEvent();
	reset();

	// Verify request is issued multiple times until a response is delivered
	provideResponse(GMSEC_FALSE);
	messageAddU16Field(reqMsg, "REQUEST-ID", (GMSEC_U16) 2, NULL);
	connectionRequestWithCallback(conn, reqMsg, 5000, &MyReplyCallback, &MyEventCallback, 5000, status);
	timeUtilMillisleep(5500*2);
	testCheckBool("Expected MyReplyCallback to be summoned", gotReply() == GMSEC_TRUE);
	testCheckBool("Expected a timeout event", gotTimeout() == GMSEC_TRUE);
	resetEvent();
	reset();

	// Verify request is issued multiple times until a response is delivered
	provideResponse(GMSEC_FALSE);
	messageAddU16Field(reqMsg, "REQUEST-ID", (GMSEC_U16) 3, NULL);
	connectionRequestWithCallback(conn, reqMsg, -1, &MyReplyCallback, &MyEventCallback, 5000, status);
	timeUtilMillisleep(5500*2);
	testCheckBool("Expected MyReplyCallback to be summoned", gotReply() == GMSEC_TRUE);
	testCheckBool("Expected a timeout event", gotTimeout() == GMSEC_FALSE);
	resetEvent();
	reset();

	// Off-nominal tests
	{
		// Connection is NULL
		messageAddU16Field(reqMsg, "REQUEST-ID", (GMSEC_U16) 4, NULL);
		connectionRequestWithCallback(NULL, reqMsg, 5000, &MyReplyCallback, &MyEventCallback, 1000, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Message is NULL
		connectionRequestWithCallback(conn, NULL, 5000, &MyReplyCallback, &MyEventCallback, 1000, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// ReplyCallback function is NULL
		connectionRequestWithCallback(conn, reqMsg, 5000, NULL, &MyEventCallback, 1000, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// EventCallback function is NULL
		connectionRequestWithCallback(conn, reqMsg, 5000, &MyReplyCallback, NULL, 1000, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Non-request message
		GMSEC_Message hbMsg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "HB", NULL);

		connectionRequestWithCallback(conn, hbMsg, 5000, &MyReplyCallback, &MyEventCallback, 1000, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);

		messageDestroy(&hbMsg);
	}

	connectionDisconnect(conn, NULL);

	messageDestroy(&reqMsg);
	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionRequest()
{
	GMSEC_INFO("Test connectionRequest()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	char             tmp[OUT_STR_LEN];

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	setStandardFields(connectionGetMessageFactory(conn, NULL));

	GMSEC_Message reqMsg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "REQ.DIR", NULL);
	messageSetSubject(reqMsg, testGetSubjectCharPtr("REQ.DIR", tmp), NULL);
	messageAddStringField(reqMsg, "DIRECTIVE-STRING", "Do something!", NULL);
	messageAddStringField(reqMsg, "DESTINATION-COMPONENT", "RESPONDER", NULL);

	// subscribe to receive the response message
	connectionSubscribe(conn, testGetSubjectCharPtr("RESP.DIR.+", tmp), NULL);

	// subscribe to process the request message
	connectionSubscribeWithCallback(conn, messageGetSubject(reqMsg, NULL), &MyRequestCallback, NULL);
	connectionStartAutoDispatch(conn, NULL);

	//o Verify a response is returned
	provideResponse(GMSEC_TRUE);
	messageAddU16Field(reqMsg, "REQUEST-ID", (GMSEC_U16) 0, NULL);
	GMSEC_Message repMsg = connectionRequest(conn, reqMsg, 5000, GMSEC_REQUEST_REPUBLISH_NEVER, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testRequireBool("Reply message not received", repMsg != NULL);
	messageDestroy(&repMsg);

	//o Verify NULL is returned on timeout
	provideResponse(GMSEC_FALSE);
	messageAddU16Field(reqMsg, "REQUEST-ID", (GMSEC_U16) 1, NULL);
	repMsg = connectionRequest(conn, reqMsg, 5000, GMSEC_REQUEST_REPUBLISH_NEVER, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testRequireBool("Unexpected message received", repMsg == NULL);

	//o Verify request is issued multiple times until a response is delivered
	provideResponse(GMSEC_FALSE);
	messageAddU16Field(reqMsg, "REQUEST-ID", (GMSEC_U16) 2, NULL);
	repMsg = connectionRequest(conn, reqMsg, GMSEC_WAIT_FOREVER, 1000, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testRequireBool("Reply message not received", repMsg != NULL);
	messageDestroy(&repMsg);

	// Off-nominal tests
	{
		// Connection is NULL
		(void) connectionRequest(NULL, reqMsg, 5000, 1000, status);
		testRequireBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Message is NULL
		(void) connectionRequest(conn, NULL, 5000, 1000, status);
		testRequireBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Non-request message
		GMSEC_Message hbMsg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "HB", NULL);

		(void) connectionRequest(conn, hbMsg, 5000, 1000, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);

		messageDestroy(&hbMsg);
	}

	connectionDisconnect(conn, NULL);

	messageDestroy(&reqMsg);
	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionReply()
{
	GMSEC_INFO("Test connectionReply()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	// Nominal tests for connectionReply tested earlier

	// Off-nominal tests
	{
		//o Connection is NULL
		GMSEC_Message reqMsg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "REQ.DIR", NULL);
		GMSEC_Message repMsg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "RESP.DIR", NULL);

		connectionReply(NULL, reqMsg, repMsg, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool(statusGet(status), strcontains(statusGet(status), "Connection handle is NULL") == 0);

		messageDestroy(&reqMsg);
		messageDestroy(&repMsg);
	}
	{
		//o Reply message is NULL
		GMSEC_Message reqMsg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "REQ.DIR", NULL);

		connectionReply(conn, reqMsg, NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool(statusGet(status), strcontains(statusGet(status), "Reply Message handle is NULL") == 0);

		messageDestroy(&reqMsg);
	}
	{
		//o Non-reply message
		GMSEC_Message reqMsg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "REQ.DIR", NULL);
		GMSEC_Message hbMsg  = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "HB", NULL);

		(void) connectionReply(conn, reqMsg, hbMsg, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);

		messageDestroy(&reqMsg);
		messageDestroy(&hbMsg);
	}

	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionDispatch()
{
	GMSEC_INFO("Test connectionDispatch()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	char             tmp[OUT_STR_LEN];

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	setStandardFields(connectionGetMessageFactory(conn, NULL));

	GMSEC_Message msg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "HB", NULL);
	messageSetSubject(msg, testGetSubjectCharPtr("HB", tmp), NULL);

	connectionSubscribeWithCallback(conn, messageGetSubject(msg, NULL), &MyCallback, NULL);

	connectionPublish(conn, msg, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_Message rcvd = connectionReceive(conn, 5000, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testRequireBool("Failed to receive message", rcvd != NULL);

	// Nominal test
	connectionDispatch(conn, rcvd, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	timeUtilMillisleep(1000);
	testCheckBool("Expected callback to be summoned", wasCalled() == GMSEC_TRUE);

	// Off-nominal tests
	{
		// Connection is NULL
		connectionDispatch(NULL, msg, status);
		testCheckBool(statusGet(status), strcontains(statusGet(status), "Connection handle is NULL") == 0);
	}
	{
		// Message is NULL
		connectionDispatch(conn, NULL, status);
		testCheckBool(statusGet(status), strcontains(statusGet(status), "Message handle is NULL") == 0);
	}
	{
		// Not connected
		connectionDisconnect(conn, NULL);

		connectionDispatch(conn, msg, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&rcvd);
	messageDestroy(&msg);
	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionReceive()
{
	GMSEC_INFO("Test connectionReceive()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	char             tmp[OUT_STR_LEN];

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	setStandardFields(connectionGetMessageFactory(conn, NULL));

	//o Timeout occurs (no subscription)
	GMSEC_Message rcvd = connectionReceive(conn, 10, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Received unexpected message", rcvd == NULL);

	//o Receive message
	GMSEC_Message msg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "HB", NULL);
	messageSetSubject(msg, testGetSubjectCharPtr("HB", tmp), NULL);

	connectionSubscribe(conn, messageGetSubject(msg, NULL), status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionPublish(conn, msg, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	rcvd = connectionReceive(conn, 5000, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected to receive a message", rcvd != NULL);
	messageDestroy(&rcvd);

	// Off-nominal tests
	{
		// Connection is NULL
		connectionReceive(NULL, 5000, status);
		testCheckBool(statusGet(status), strcontains(statusGet(status), "Connection handle is NULL") == 0);
	}
	{
		// Not connected
		connectionDisconnect(conn, NULL);

		connectionReceive(conn, 5000, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionExcludeSubject()
{
	GMSEC_INFO("Test connectionExcludeSubject()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	char             tmp[OUT_STR_LEN];

	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	setStandardFields(connectionGetMessageFactory(conn, NULL));

	GMSEC_Message msg1 = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "HB", NULL);
	GMSEC_Message msg2 = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", NULL);

	GMSEC_Field   sub = stringFieldCreate("SUBCLASS", "NO-CLASS", GMSEC_FALSE, NULL);
	GMSEC_Field   occ = stringFieldCreate("OCCURRENCE-TYPE", "FREQUENTLY", GMSEC_FALSE, NULL);
	GMSEC_Field   sev = i16FieldCreate("SEVERITY", (short) 1, GMSEC_FALSE, NULL);

	messageSetSubject(msg1, testGetSubjectCharPtr("HB", tmp), NULL);
	messageSetSubject(msg2, testGetSubjectCharPtr("LOG", tmp), NULL);

	messageAddField(msg2, sub, NULL);
	messageAddField(msg2, occ, NULL);
	messageAddField(msg2, sev, NULL);

	fieldDestroy(&sub);
	fieldDestroy(&occ);
	fieldDestroy(&sev);

	connectionSubscribe(conn, testGetSubjectCharPtr(">", tmp), status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionExcludeSubject(conn, testGetSubjectCharPtr("LOG.+", tmp), status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionPublish(conn, msg1, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_Message rcvd = connectionReceive(conn, 5000, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testRequireBool("Failed to receive a message", rcvd != NULL);
	messageDestroy(&rcvd);

	connectionPublish(conn, msg2, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	rcvd = connectionReceive(conn, 5000, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testRequireBool("Received unexpected message", rcvd == NULL);

	connectionRemoveExcludedSubject(conn, testGetSubjectCharPtr("LOG.+", tmp), status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionPublish(conn, msg2, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	rcvd = connectionReceive(conn, 5000, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testRequireBool("Failed to receive a message", rcvd != NULL);
	messageDestroy(&rcvd);

	// Off-nominal tests
	connectionExcludeSubject(NULL, "GMSEC.+", status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);

	connectionExcludeSubject(conn, NULL, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);

	connectionDisconnect(conn, NULL);

	messageDestroy(&msg1);
	messageDestroy(&msg2);
	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionRemoveExcludedSubject()
{
	GMSEC_INFO("Test connectionRemoveExcludedSubject()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	// Nominal test is conducted within test_connectionExcludeSubject()

	// Off-nominal tests
	{
		// Connection is NULL
		connectionRemoveExcludedSubject(NULL, "GMSEC.+", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Subject is NULL
		connectionRemoveExcludedSubject(conn, NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Not connected
		connectionDisconnect(conn, NULL);

		connectionRemoveExcludedSubject(conn, "GMSEC.>", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}

	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionGetName()
{
	GMSEC_INFO("Test connectionGetName()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected a connection name to be non-NULL", connectionGetName(conn, NULL) != NULL);

	connectionSetName(conn, "FOOBAR", status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected a connection name to be FOOBAR", strcompare(connectionGetName(conn, NULL), "FOOBAR") ==0);

	// Off-nominal tests
	connectionGetName(NULL, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);

	connectionSetName(NULL, "FOOBAZ", status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);

	connectionSetName(conn, NULL, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);

	connectionDisconnect(conn, NULL);

	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionGetID()
{
	GMSEC_INFO("Test connectionGetID()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	// Nominal test
	testCheckBool("Expected a connection ID to be non-NULL", connectionGetID(conn, NULL) != NULL);

	// Off-nominal test
	connectionGetID(NULL, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);

	connectionDisconnect(conn, NULL);

	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionGetMWInfo()
{
	GMSEC_INFO("Test connectionGetMWInfo()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	// Nominal test
	testCheckBool("Unexpected mw-info string", strcontains(connectionGetMWInfo(conn, NULL), configGetValueOrDefault(config, "mw-id", "unknown", NULL)) == 0);

	// Off-nominal test
	connectionGetMWInfo(NULL, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);

	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionGetConnectionEndpoint()
{
	GMSEC_INFO("Test connectionGetConnectionEndpoint()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	// Nominal test
	testCheckBool("Unexpected connection endpoint", connectionGetConnectionEndpoint(conn, NULL) != NULL);

	// Off-nominal test
	connectionGetConnectionEndpoint(NULL, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);

	connectionDisconnect(conn, NULL);

	connectionDestroy(&conn);
	statusDestroy(&status);
}


void test_connectionGetPublishQueueMessageCount()
{
	GMSEC_INFO("Test connectionGetPublishQueueMessageCount()");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = testGetConfig();
	GMSEC_Connection conn   = connectionCreate(config, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	setStandardFields(connectionGetMessageFactory(conn, NULL));

	GMSEC_Message msg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", NULL);
	GMSEC_Field   sub = stringFieldCreate("SUBCLASS", "NO-CLASS", GMSEC_FALSE, NULL);
	GMSEC_Field   occ = stringFieldCreate("OCCURRENCE-TYPE", "FREQUENTLY", GMSEC_FALSE, NULL);
	GMSEC_Field   sev = i16FieldCreate("SEVERITY", (short) 1, GMSEC_FALSE, NULL);

	messageAddField(msg, sub, NULL);
	messageAddField(msg, occ, NULL);
	messageAddField(msg, sev, NULL);

	fieldDestroy(&sub);
	fieldDestroy(&occ);
	fieldDestroy(&sev);

	int i;

	// Nominal test
	for (i = 0; i < 5; ++i)
	{
		GMSEC_U32 count = connectionGetPublishQueueMessageCount(conn, NULL);
		testCheckBool("Unexpected message count", count <= 1);
		connectionPublish(conn, msg, NULL);
		timeUtilMillisleep(500);
	}
	timeUtilMillisleep(500);
	testCheckBool("Expected message count to be 0 (zero)", connectionGetPublishQueueMessageCount(conn, NULL) == 0);

	// Off-nominal test
	(void) connectionGetPublishQueueMessageCount(NULL, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);

	connectionDisconnect(conn, NULL);

	messageDestroy(&msg);
	connectionDestroy(&conn);
	statusDestroy(&status);
}


int test_Connection()
{
	testSetDescription("Connection");

	test_connectionCreate();
	test_connectionCreateWithFactory();
	test_connectionDestroy();
	test_connectionGetAPIVersion();
	test_connectionConnect();
	test_connectionDisconnect();
	test_connectionGetLibraryName();
	test_connectionGetLibraryVersion();
	test_connectionGetMessageFactory();
	test_connectionRegisterEventCallback();
	test_connectionSubscribe();
	test_connectionSubscribeWithCallback();
	test_connectionSubscribeWithConfig();
	test_connectionSubscribeWithConfigAndCallback();
	test_connectionUnsubscribe();
	test_connectionStartAutoDispatch();
	test_connectionStopAutoDispatch();
	test_connectionPublish();
	test_connectionPublishWithMwConfig();
	test_connectionRequestWithCallback();
	test_connectionRequest();
	test_connectionReply();
	test_connectionDispatch();
	test_connectionReceive();
	test_connectionExcludeSubject();
	test_connectionRemoveExcludedSubject();
	test_connectionGetName();
	test_connectionGetID();
	test_connectionGetMWInfo();
	test_connectionGetConnectionEndpoint();
	test_connectionGetPublishQueueMessageCount();

	return 0;
}
TEST_DRIVER(test_Connection)
