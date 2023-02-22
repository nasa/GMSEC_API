#include "TestCase.h"


GMSEC_ResponseStatus g_statusList[] =
{
  GMSEC_ACKNOWLEDGEMENT,
  GMSEC_WORKING_KEEP_ALIVE,
  GMSEC_WORKING_KEEP_ALIVE,
  GMSEC_WORKING_KEEP_ALIVE,
  GMSEC_SUCCESSFUL_COMPLETION
};


void T041_onMessage(GMSEC_Connection conn, const GMSEC_Message msg)
{
	GMSEC_Status  status = statusCreate();
	GMSEC_Message reply  = messageFactoryCreateMessage( connectionGetMessageFactory(conn, NULL), "RESP.DIR", NULL );
	char          tmp[OUT_STR_LEN];
	int           i;

	messageSetSubject(reply, testGetSubjectCharPtr("RESP.DIR", tmp), NULL);

	messageAddStringField(reply, "COMPONENT",
	                      (messageHasField(msg, "DESTINATION-COMPONENT", NULL) == GMSEC_TRUE ?
	                       messageGetStringValue(msg, "DESTINATION-COMPONENT", NULL) : "RESPONDER"), NULL);

	messageAddStringField(reply, "DESTINATION-COMPONENT",
	                      (messageHasField(msg, "COMPONENT", NULL) == GMSEC_TRUE ?
	                       messageGetStringValue(msg, "COMPONENT", NULL) : "FILL"), NULL);

	if (messageHasField(msg, "REQUEST-ID", NULL) == GMSEC_TRUE)
	{
		messageAddField(reply, messageGetField(msg, "REQUEST-ID", NULL), NULL);
	}

	GMSEC_VERBOSE("Received Request:\n%s", messageToXML(msg, NULL));

	for (i = 0; i < sizeof(g_statusList)/sizeof(GMSEC_ResponseStatus); ++i)
	{
		messageAddI16Field(reply, "RESPONSE-STATUS", (GMSEC_I16) g_statusList[i], NULL);

		GMSEC_VERBOSE("Issuing Reply:\n%s", messageToXML(reply, NULL));

		connectionReply(conn, msg, reply, status);

		if (statusHasError(status) == GMSEC_TRUE)
		{
			GMSEC_ERROR("Unexpected Error: %s", statusGet(status));
		}
	}

	messageDestroy(&reply);
	statusDestroy(&status);
}


int expectedResponses()
{
	return sizeof(g_statusList)/sizeof(GMSEC_ResponseStatus);
}


int g_repliesReceived = 0;


void T041_onReply(GMSEC_Connection conn, const GMSEC_Message request, const GMSEC_Message reply)
{
	GMSEC_Status status = statusCreate();

	GMSEC_I16 respStatus = messageGetI16Value(reply, "RESPONSE-STATUS", status);

	if (statusHasError(status) == GMSEC_FALSE)
	{
		GMSEC_INFO("Received Reply with RESPONSE-STATUS: %d", respStatus);
		++g_repliesReceived;
	}
	else
	{
		GMSEC_ERROR("GmsecException in T041_ReplyCallback: %s", statusGet(status));
	}

	statusDestroy(&status);
}


void T041_onEvent(GMSEC_Connection conn, const GMSEC_Status status, GMSEC_ConnectionEvent connEvent)
{
	GMSEC_WARNING("Received unexpected event: %s", statusGet(status));
}


int numRepliesReceived()
{
	return g_repliesReceived;
}


int test_MultiResponse()
{
	testSetDescription("Multi-Response");

	GMSEC_Status     status = statusCreate();
	GMSEC_Config     config = configCreateCopy(testGetConfig(), NULL);  // make a copy
	GMSEC_Connection conn   = NULL;
	char             tmp[OUT_STR_LEN];

	configAddValue(config, "mw-multi-resp", "true", NULL);

	conn = connectionCreate(config, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	connectionConnect(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	setStandardFields(connectionGetMessageFactory(conn, NULL));

	connectionStartAutoDispatch(conn, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_Message request = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "REQ.DIR", NULL);
	messageSetSubject(request, testGetSubjectCharPtr("REQ.DIR", tmp), NULL);
	messageAddStringField(request, "DIRECTIVE-STRING", "Do something!", NULL);
	messageAddStringField(request, "DESTINATION-COMPONENT", "RESPONDER", NULL);
	messageAddU16Field(request, "REQUEST-ID", (GMSEC_U16) 0, NULL);

	// subscribe to receive the response message (hack!)
	connectionSubscribe(conn, testGetSubjectCharPtr("RESP.DIR.+", tmp), status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	// subscribe to process the request message
	connectionSubscribeWithCallback(conn, messageGetSubject(request, NULL), &T041_onMessage, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	GMSEC_VERBOSE("Issuing Request:\n%s", messageToXML(request, NULL));

	connectionRequestWithCallback(conn, request, 5000, &T041_onReply, &T041_onEvent, GMSEC_REQUEST_REPUBLISH_NEVER, status);
	testRequireBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	timeUtilMillisleep(5000);   // allow for the response message(s) to be received

	connectionDisconnect(conn, NULL);

	testCheckBool("Unexpected response message count", numRepliesReceived() == expectedResponses());

	messageDestroy(&request);
	connectionDestroy(&conn);
	configDestroy(&config);
	statusDestroy(&status);

	return 0;
}

TEST_DRIVER(test_MultiResponse)
