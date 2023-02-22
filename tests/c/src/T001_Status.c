#include "TestCase.h"


void test_statusCreate()
{
	GMSEC_INFO("Test statusCreate()");

	GMSEC_Status status = statusCreate();

	testCheckBool("Expected no error", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected GMSEC_NO_ERROR", statusGetClass(status) == GMSEC_NO_ERROR);
	testCheckBool("Expected GMSEC_NO_ERROR_CODE", statusGetCode(status) == GMSEC_NO_ERROR_CODE);
	testCheckBool("Expected custom code of 0", statusGetCustomCode(status) == 0);
	testCheckBool("Expected no reason", strcompare(statusGetReason(status), "") == 0);

	statusDestroy(&status);
}


void test_statusCreateWithValues()
{
	GMSEC_INFO("Test statusCreateWithValues()");

	const char* reason = "Some worthy status";

	GMSEC_Status status = statusCreateWithValues(1, 2, reason, 3);

	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Unexpected class", statusGetClass(status) == 1);
	testCheckBool("Unexpected code", statusGetCode(status) == 2);
	testCheckBool("Unexpected reason", strcompare(statusGetReason(status), reason) == 0);
	testCheckBool("Unexpected custom code", statusGetCustomCode(status) == 3);

	statusDestroy(&status);
}


void test_statusCreateCopy()
{
	GMSEC_INFO("Test statusCreateCopy()");

	const char* reason = "Some worthy status";

	GMSEC_Status status1 = statusCreateWithValues(1, 2, reason, 3);
	GMSEC_Status status2 = statusCreateCopy(status1);

	testCheckBool("Expected unique handles", status1 != status2);
	testCheckBool("Unexpected error", statusHasError(status1) == statusHasError(status2));
	testCheckBool("Unexpected class", statusGetClass(status1) == statusGetClass(status2));
	testCheckBool("Unexpected code", statusGetCode(status1) == statusGetCode(status2));
	testCheckBool("Unexpected reason", strcompare(statusGetReason(status1), statusGetReason(status2)) == 0);
	testCheckBool("Unexpected custom code", statusGetCustomCode(status1) == statusGetCustomCode(status2));

	statusDestroy(&status1);
	statusDestroy(&status2);
}


void test_statusDestroy()
{
	GMSEC_INFO("Test statusDestroy()");

	GMSEC_Status status = statusCreate();
	testCheckBool("Expected a GMSEC_Status handle", status != NULL);

	statusDestroy(&status);
	testCheckBool("Expected GMSEC_Status handle to be destroyed", status == NULL);
}


void test_statusGet()
{
	GMSEC_INFO("Test statusGet()");

	GMSEC_Status status = statusCreateWithValues(1, 2, "Some worthy reason", 3);

	testCheckBool("Unexpected get string", strcompare(statusGet(status), "[1,2,3] : Some worthy reason") == 0);

	statusDestroy(&status);
}


void test_statusSet()
{
	GMSEC_INFO("Test statusSet()");

	const char* reason = "Some worthy reason";

	GMSEC_Status status = statusCreate();

	statusSet(status, 1, 2, reason, 3);

	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Unexpected class", statusGetClass(status) == 1);
	testCheckBool("Unexpected code", statusGetCode(status) == 2);
	testCheckBool("Unexpected reason", strcompare(statusGetReason(status), reason) == 0);
	testCheckBool("Unexpected custom code", statusGetCustomCode(status) == 3);

	statusDestroy(&status);
}


void test_statusSetClass()
{
	GMSEC_INFO("Test statusSetClass()");

	GMSEC_Status status = statusCreate();

	statusSetClass(status, 0);
	testCheckBool("Expected no error", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected class", statusGetClass(status) == 0);
	testCheckBool("Unexpected code", statusGetCode(status) == 0);
	testCheckBool("Unexpected reason", strcompare(statusGetReason(status), "") == 0);
	testCheckBool("Unexpected custom code", statusGetCustomCode(status) == 0);

	statusSetClass(status, 1);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Unexpected class", statusGetClass(status) == 1);
	testCheckBool("Unexpected code", statusGetCode(status) == 0);
	testCheckBool("Unexpected reason", strcompare(statusGetReason(status), "") == 0);
	testCheckBool("Unexpected custom code", statusGetCustomCode(status) == 0);

	statusDestroy(&status);
}


void test_statusSetCode()
{
	GMSEC_INFO("Test statusSetCode()");

	GMSEC_Status status = statusCreate();

	statusSetCode(status, 0);
	testCheckBool("Expected no error", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected class", statusGetClass(status) == 0);
	testCheckBool("Unexpected code", statusGetCode(status) == 0);
	testCheckBool("Unexpected reason", strcompare(statusGetReason(status), "") == 0);
	testCheckBool("Unexpected custom code", statusGetCustomCode(status) == 0);

	statusSetCode(status, 1);
	testCheckBool("Expected no error", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected class", statusGetClass(status) == 0);
	testCheckBool("Unexpected code", statusGetCode(status) == 1);
	testCheckBool("Unexpected reason", strcompare(statusGetReason(status), "") == 0);
	testCheckBool("Unexpected custom code", statusGetCustomCode(status) == 0);

	statusDestroy(&status);
}


void test_statusSetReason()
{
	GMSEC_INFO("Test statusSetReason()");

	const char* reason = "Some worthy reason";

	GMSEC_Status status = statusCreate();

	statusSetReason(status, "");
	testCheckBool("Expected no error", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected class", statusGetClass(status) == 0);
	testCheckBool("Unexpected code", statusGetCode(status) == 0);
	testCheckBool("Unexpected reason", strcompare(statusGetReason(status), "") == 0);
	testCheckBool("Unexpected custom code", statusGetCustomCode(status) == 0);

	statusSetReason(status, reason);
	testCheckBool("Expected no error", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected class", statusGetClass(status) == 0);
	testCheckBool("Unexpected code", statusGetCode(status) == 0);
	testCheckBool("Unexpected reason", strcompare(statusGetReason(status), reason) == 0);
	testCheckBool("Unexpected custom code", statusGetCustomCode(status) == 0);

	statusDestroy(&status);
}


void test_statusSetCustomCode()
{
	GMSEC_INFO("Test statusSetCustomCode()");

	GMSEC_Status status = statusCreate();

	statusSetCustomCode(status, 0);
	testCheckBool("Expected no error", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected class", statusGetClass(status) == 0);
	testCheckBool("Unexpected code", statusGetCode(status) == 0);
	testCheckBool("Unexpected reason", strcompare(statusGetReason(status), "") == 0);
	testCheckBool("Unexpected custom code", statusGetCustomCode(status) == 0);

	statusSetCustomCode(status, 3);
	testCheckBool("Expected no error", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected class", statusGetClass(status) == 0);
	testCheckBool("Unexpected code", statusGetCode(status) == 0);
	testCheckBool("Unexpected reason", strcompare(statusGetReason(status), "") == 0);
	testCheckBool("Unexpected custom code", statusGetCustomCode(status) == 3);

	statusDestroy(&status);
}


void test_statusReset()
{
	GMSEC_INFO("Test statusReset()");

	GMSEC_Status status = statusCreateWithValues(1, 2, "foobar", 3);

	statusReset(status);
	testCheckBool("Expected no error", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected class", statusGetClass(status) == 0);
	testCheckBool("Unexpected code", statusGetCode(status) == 0);
	testCheckBool("Unexpected reason", strcompare(statusGetReason(status), "") == 0);
	testCheckBool("Unexpected custom code", statusGetCustomCode(status) == 0);

	statusDestroy(&status);
}


int test_Status()
{
	testSetDescription("Test Status");

	test_statusCreate();
	test_statusCreateWithValues();
	test_statusCreateCopy();
	test_statusDestroy();
	test_statusGet();
	test_statusSet();
	test_statusSetClass();
	test_statusSetCode();
	test_statusSetReason();
	test_statusSetCustomCode();
	test_statusReset();

	return 0;
}
TEST_DRIVER(test_Status)
