#include "TestCase.h"

using namespace gmsec::api5;


void test_constructor_1(Test& test)
{
	GMSEC_INFO << "Testing GmsecException constructor (3 args)...";

	std::string emsg1 = "Something bad happened";
	std::string emsg2;

	GmsecException e1(1, 2, emsg1.c_str());
	GmsecException e2(3, 4, NULL);

	test.check("Unexpected error class", 1 == e1.getErrorClass());
	test.check("Unexpected error code", 2 == e1.getErrorCode());
	test.check("Unexpected error custom code", 0 == e1.getCustomCode());
	test.check("Unexpected error message", emsg1 == e1.getErrorMessage());

	test.check("Unexpected error class", 3 == e2.getErrorClass());
	test.check("Unexpected error code", 4 == e2.getErrorCode());
	test.check("Unexpected error custom code", 0 == e2.getCustomCode());
	test.check("Unexpected error message", emsg2 == e2.getErrorMessage());
}


void test_constructor_2(Test& test)
{
	GMSEC_INFO << "Testing GmsecException constructor (4 args)...";

	std::string emsg1 = "Something bad happened";
	std::string emsg2;

	GmsecException e1(1, 2, 3, emsg1.c_str());
	GmsecException e2(4, 5, 6, NULL);

	test.check("Unexpected error class", 1 == e1.getErrorClass());
	test.check("Unexpected error code", 2 == e1.getErrorCode());
	test.check("Unexpected error custom code", 3 == e1.getCustomCode());
	test.check("Unexpected error message", emsg1 == e1.getErrorMessage());

	test.check("Unexpected error class", 4 == e2.getErrorClass());
	test.check("Unexpected error code", 5 == e2.getErrorCode());
	test.check("Unexpected error custom code", 6 == e2.getCustomCode());
	test.check("Unexpected error message", emsg2 == e2.getErrorMessage());
}


void test_constructor_3(Test& test)
{
	GMSEC_INFO << "Testing GmsecException constructor (w/ Status)...";

	std::string reason = "My status reason";

	Status status(1, 2, reason.c_str(), 3);

	GmsecException e1(status);

	test.check("Unexpected error class", 1 == e1.getErrorClass());
	test.check("Unexpected error code", 2 == e1.getErrorCode());
	test.check("Unexpected error custom code", 3 == e1.getCustomCode());
	test.check("Unexpected error message", reason == e1.getErrorMessage());
}


void test_copy_constructor(Test& test)
{
	GMSEC_INFO << "Testing GmsecException copy-constructor...";

	std::string emsg1 = "Something bad happened";

	GmsecException e1(1, 2, 3, emsg1.c_str());
	GmsecException e2(e1);

	test.check("Unexpected error class", e1.getErrorClass() == e2.getErrorClass());
	test.check("Unexpected error code", e1.getErrorCode() == e2.getErrorCode());
	test.check("Unexpected error custom code", e1.getCustomCode() == e2.getCustomCode());
	test.check("Unexpected error message", emsg1 == e2.getErrorMessage());
}


void test_assignment_operator(Test& test)
{
	GMSEC_INFO << "Testing GmsecException assignment-operator...";

	std::string emsg1 = "Something bad happened";

	GmsecException e1(1, 2, 3, emsg1.c_str());
	GmsecException e2(4, 5, 6, NULL);

	e2 = e1;

	test.check("Unexpected error class", e1.getErrorClass() == e2.getErrorClass());
	test.check("Unexpected error code", e1.getErrorCode() == e2.getErrorCode());
	test.check("Unexpected error custom code", e1.getCustomCode() == e2.getCustomCode());
	test.check("Unexpected error message", emsg1 == e2.getErrorMessage());
}


void test_what(Test& test)
{
	GMSEC_INFO << "Testing GmsecException what()...";

	std::string emsg1    = "Something bad happened";
	std::string expected = "[1,2,3] : Something bad happened";

	GmsecException e1(1, 2, 3, emsg1.c_str());

	test.check("Unexpected summarized error message", expected == e1.what());
}


int test_GmsecException(Test& test)
{
	test.setDescription("Test GmsecException");

	test_constructor_1(test);
	test_constructor_2(test);
	test_constructor_3(test);
	test_copy_constructor(test);
	test_assignment_operator(test);
	test_what(test);

	return 0;
}

TEST_DRIVER(test_GmsecException)
