#include "TestCase.h"

using namespace gmsec::api5;


void test_constructor_1(Test& test)
{
	GMSEC_INFO << "Test no-arg constructor...";

	std::string reason;

	Status s1;
	test.check("Expected no error", s1.hasError() == false);
	test.check("Expected NO_ERROR", s1.getClass() == NO_ERROR_CLASS);
	test.check("Expected NO_ERROR_CODE", s1.getCode() == NO_ERROR_CODE);
	test.check("Expected custom code of 0", s1.getCustomCode() == 0);
	test.check("Expected no reason", reason == s1.getReason());
}


void test_constructor_2(Test& test)
{
	GMSEC_INFO << "Test 2-4 arg constructor...";

	std::string reason1 = "";
	std::string reason2 = "Some worthy status";

	Status s1(1, 2);
	test.check("Expected an error", s1.hasError());
	test.check("Unexpected class", s1.getClass() == 1);
	test.check("Unexpected code", s1.getCode() == 2);
	test.check("Unexpected custom code", s1.getCustomCode() == 0);
	test.check("Unexpected reason", reason1 == s1.getReason());

	Status s2(3, 4, reason2.c_str());
	test.check("Expected an error", s2.hasError());
	test.check("Unexpected class", s2.getClass() == 3);
	test.check("Unexpected code", s2.getCode() == 4);
	test.check("Unexpected custom code", s2.getCustomCode() == 0);
	test.check("Unexpected reason", reason2 == s2.getReason());

	Status s3(6, 7, reason2.c_str(), 8);
	test.check("Expected an error", s3.hasError());
	test.check("Unexpected class", s3.getClass() == 6);
	test.check("Unexpected code", s3.getCode() == 7);
	test.check("Unexpected custom code", s3.getCustomCode() == 8);
	test.check("Unexpected reason", reason2 == s3.getReason());
}


void test_exception_constructor(Test& test)
{
	GMSEC_INFO << "Test exception constructor...";

	std::string reason = "Some worthy exception";

	GmsecException e1(1, 2, 3, reason.c_str());

	Status s1(e1);
	test.check("Expected an error", s1.hasError());
	test.check("Unexpected class", s1.getClass() == 1);
	test.check("Unexpected code", s1.getCode() == 2);
	test.check("Unexpected custom code", s1.getCustomCode() == 3);
	test.check("Unexpected reason", reason == s1.getReason());
}


void test_copy_constructor(Test& test)
{
	GMSEC_INFO << "Test copy constructor...";

	std::string reason = "Some worthy exception";

	Status s1(1, 2, reason.c_str(), 3);
	Status s2(s1);

	test.check("Expected an error", s1.hasError() == s2.hasError());
	test.check("Unexpected class", s1.getClass() == s2.getClass());
	test.check("Unexpected code", s1.getCode() == s2.getCode());
	test.check("Unexpected custom code", s1.getCustomCode() == s2.getCustomCode());
	test.check("Unexpected reason", reason == s2.getReason());
}


void test_assignment_operator(Test& test)
{
	GMSEC_INFO << "Test assignment operator...";

	std::string reason = "Some worthy exception";

	Status s1(1, 2, reason.c_str(), 3);
	Status s2;

	s2 = s1;

	test.check("Expected an error", s1.hasError() == s2.hasError());
	test.check("Unexpected class", s1.getClass() == s2.getClass());
	test.check("Unexpected code", s1.getCode() == s2.getCode());
	test.check("Unexpected custom code", s1.getCustomCode() == s2.getCustomCode());
	test.check("Unexpected reason", reason == s2.getReason());
}


void test_get(Test& test)
{
	GMSEC_INFO << "Test get...";

	std::string reason = "Some worthy exception";

	Status s1(1, 2, reason.c_str(), 3);

	std::ostringstream oss;
	oss << "[1,2,3] : " << reason;

	test.check("Unexpected get string", oss.str() == s1.get());
}


void test_set(Test& test)
{
	GMSEC_INFO << "Test set...";

	std::string reason = "Some worthy exception";

	Status s1;
	s1.set(0, 1);
	test.check("Expected an error", s1.hasError() == false);
	test.check("Unexpected class", s1.getClass() == 0);
	test.check("Unexpected code", s1.getCode() == 1);
	test.check("Unexpected custom code", s1.getCustomCode() == 0);
	test.check("Unexpected reason", std::string("") == s1.getReason());

	Status s2;
	s2.set(1, 2, reason.c_str());
	test.check("Expected an error", s2.hasError());
	test.check("Unexpected class", s2.getClass() == 1);
	test.check("Unexpected code", s2.getCode() == 2);
	test.check("Unexpected custom code", s2.getCustomCode() == 0);
	test.check("Unexpected reason", reason == s2.getReason());

	Status s3(1, 2, "foobar", 6);
	s3.set(1, 2, reason.c_str(), 3);
	test.check("Expected an error", s3.hasError());
	test.check("Unexpected class", s3.getClass() == 1);
	test.check("Unexpected code", s3.getCode() == 2);
	test.check("Unexpected custom code", s3.getCustomCode() == 3);
	test.check("Unexpected reason", reason == s3.getReason());
}


void test_setClass(Test& test)
{
	GMSEC_INFO << "Test setClass...";

	Status s1;
	s1.setClass(0);
	test.check("Expected an error", s1.hasError() == false);
	test.check("Unexpected class", s1.getClass() == 0);
	test.check("Unexpected code", s1.getCode() == 0);
	test.check("Unexpected custom code", s1.getCustomCode() == 0);
	test.check("Unexpected reason", std::string("") == s1.getReason());

	Status s2;
	s2.setClass(1);
	test.check("Expected an error", s2.hasError());
	test.check("Unexpected class", s2.getClass() == 1);
	test.check("Unexpected code", s2.getCode() == 0);
	test.check("Unexpected custom code", s2.getCustomCode() == 0);
	test.check("Unexpected reason", std::string("") == s2.getReason());
}


void test_setCode(Test& test)
{
	GMSEC_INFO << "Test setCode...";

	Status s1;
	s1.setCode(1);
	test.check("Expected an error", s1.hasError() == false);
	test.check("Unexpected class", s1.getClass() == 0);
	test.check("Unexpected code", s1.getCode() == 1);
	test.check("Unexpected custom code", s1.getCustomCode() == 0);
	test.check("Unexpected reason", std::string("") == s1.getReason());
}


void test_setReason(Test& test)
{
	GMSEC_INFO << "Test setReason...";

	std::string reason = "Some worthy exception";

	Status s1;
	s1.setReason(reason.c_str());
	test.check("Expected an error", s1.hasError() == false);
	test.check("Unexpected class", s1.getClass() == 0);
	test.check("Unexpected code", s1.getCode() == 0);
	test.check("Unexpected custom code", s1.getCustomCode() == 0);
	test.check("Unexpected reason", reason == s1.getReason());
}


void test_setCustomCode(Test& test)
{
	GMSEC_INFO << "Test setCustomCode...";

	Status s1;
	s1.setCustomCode(1);
	test.check("Expected an error", s1.hasError() == false);
	test.check("Unexpected class", s1.getClass() == 0);
	test.check("Unexpected code", s1.getCode() == 0);
	test.check("Unexpected custom code", s1.getCustomCode() == 1);
	test.check("Unexpected reason", std::string("") == s1.getReason());
}


void test_reset(Test& test)
{
	GMSEC_INFO << "Test reset...";

	Status s1(1, 2, "foobar", 3);
	s1.reset();
	test.check("Expected an error", s1.hasError() == false);
	test.check("Unexpected class", s1.getClass() == 0);
	test.check("Unexpected code", s1.getCode() == 0);
	test.check("Unexpected custom code", s1.getCustomCode() == 0);
	test.check("Unexpected reason", std::string("") == s1.getReason());
}


int test_Status(Test& test)
{
	test.setDescription("Test Status");

	test_constructor_1(test);
	test_constructor_2(test);
	test_exception_constructor(test);
	test_copy_constructor(test);
	test_assignment_operator(test);
	test_get(test);
	test_set(test);
	test_setClass(test);
	test_setCode(test);
	test_setReason(test);
	test_setCustomCode(test);
	test_reset(test);

	return 0;
}

TEST_DRIVER(test_Status)
