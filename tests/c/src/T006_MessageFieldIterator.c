#include "TestCase.h"


static GMSEC_Message createMessage()
{
	GMSEC_Message msg = messageCreate();

	messageAddStringField(msg, "FIELD-1", "1", NULL);
	messageAddStringField(msg, "FIELD-2", "2", NULL);
	messageAddI32Field(msg, "FIELD-3", 3, NULL);

	return msg;
}


void test_messageFieldIteratorHasNext()
{
	GMSEC_Status status = statusCreate();

	// Off-nominal test(s)
	{
		// NULL MessageFieldIterator
		GMSEC_BOOL hasNext = messageFieldIteratorHasNext(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected not to have next field", hasNext == GMSEC_FALSE);
	}

	statusDestroy(&status);
}


void test_messageFieldIteratorNext()
{
	GMSEC_Status status = statusCreate();

	// Off-nominal test(s)
	{
		// NULL MessageFieldIterator
		GMSEC_Field nextField = messageFieldIteratorNext(NULL, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected not to have next field", nextField == NULL);
	}

	{
		// No next field to have
		GMSEC_Message msg = createMessage();
		GMSEC_MessageFieldIterator iter = messageGetFieldIterator(msg, NULL);
		GMSEC_Field nextField;

		while (messageFieldIteratorHasNext(iter, NULL) == GMSEC_TRUE)
		{
			nextField = messageFieldIteratorNext(iter, NULL);
		}

		nextField = messageFieldIteratorNext(iter, status);
		testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected not to have next field", nextField == NULL);
	}

	statusDestroy(&status);
}


void test_messageFieldIteratorReset()
{
	GMSEC_Status  status = statusCreate();
	GMSEC_Message msg    = createMessage();

	// Nominal test
	{
		GMSEC_MessageFieldIterator iter = messageGetFieldIterator(msg, NULL);

		int fieldsFound = 0;

		while (messageFieldIteratorHasNext(iter, NULL) == GMSEC_TRUE)
		{
			(void) messageFieldIteratorNext(iter, NULL);
			++fieldsFound;
		}
		testCheckBool("Unexpected field count", fieldsFound == 3);

		testCheckBool("Should be no more field(s) to iterate over", messageFieldIteratorHasNext(iter, NULL) == GMSEC_FALSE);

		messageFieldIteratorReset(iter, status);

		testCheckBool("Unexpected error when resetting the iterator", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Should have field(s) to iterate over", messageFieldIteratorHasNext(iter, NULL) == GMSEC_TRUE);

		fieldsFound = 0;

		while (messageFieldIteratorHasNext(iter, NULL) == GMSEC_TRUE)
		{
			(void) messageFieldIteratorNext(iter, NULL);
			++fieldsFound;
		}
		testCheckBool("Unexpected field count", fieldsFound == 3);
	}

	// Off-nominal test
	{
		messageFieldIteratorReset(NULL, status);
		testCheckBool("Expected error", statusHasError(status) == GMSEC_TRUE);
	}

	messageDestroy(&msg);
	statusDestroy(&status);
}


void test_message_all_fields()
{
	GMSEC_Status  status = statusCreate();
	GMSEC_Message msg    = createMessage();

	GMSEC_MessageFieldIterator iter = messageGetFieldIterator(msg, NULL);

	int fieldsFound = 0;

	while (messageFieldIteratorHasNext(iter, status) == GMSEC_TRUE)
	{
		testRequireBool("Expected to have another field available", statusHasError(status) == GMSEC_FALSE);

		GMSEC_Field field = messageFieldIteratorNext(iter, status);

		testRequireBool("Expected a non-NULL field", field != NULL);

		++fieldsFound;

		switch (fieldsFound)
		{
		case 1:
			testCheckBool("Field 1 has unexpected type", fieldGetType(field, NULL) == GMSEC_STRING_TYPE);
			testCheckBool("Field 1 has unexpected name", strcompare(fieldGetName(field, NULL), "FIELD-1") == 0);
			testCheckBool("Field 1 has unexpected value", strcompare(fieldGetStringValue(field, NULL), "1") == 0);
			break;

		case 2:
			testCheckBool("Field 2 has unexpected type", fieldGetType(field, NULL) == GMSEC_STRING_TYPE);
			testCheckBool("Field 2 has unexpected name", strcompare(fieldGetName(field, NULL), "FIELD-2") == 0);
			testCheckBool("Field 2 has unexpected value", strcompare(fieldGetStringValue(field, NULL), "2") == 0);
			break;

		case 3:
			testCheckBool("Field 3 has unexpected type", fieldGetType(field, NULL) == GMSEC_I32_TYPE);
			testCheckBool("Field 3 has unexpected name", strcompare(fieldGetName(field, NULL), "FIELD-3") == 0);
			testCheckBool("Field 3 has unexpected value", fieldGetI32Value(field, NULL) == 3);
			break;
		}
	}

	testCheckBool("Unexpected field count", fieldsFound == 3);

	messageDestroy(&msg);
	statusDestroy(&status);
}


int test_MessageFieldIterator()
{
	testSetDescription("MessageFieldIterator");

	test_messageFieldIteratorHasNext();
	test_messageFieldIteratorNext();
	test_messageFieldIteratorReset();

	test_message_all_fields();

	return 0;
}
TEST_DRIVER(test_MessageFieldIterator)
