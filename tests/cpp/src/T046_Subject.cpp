#include "TestCase.h"

#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/internal/Subject.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


struct Samples
{
	std::string subject;
	std::string result;
};

Samples subjectSamples[] =
{
	{ "GMSEC.FOO.BAR",  "" },
	{ "GMSEC.foo.BAR",  "element contains illegal character" },
	{ "GMSEC.FOO%BAR",  "element contains illegal character" },
	{ "GMSEC.FOO..BAR", " -- Subject has '..' (is it missing an element?)" },
	{ "",               " -- Subject cannot be empty" },
};


Samples subscriptionSamples[] =
{
	{ "GMSEC.>",              "" },
	{ "GMSEC.+",              "" },
	{ "GMSEC.*.*.MSG.HB.+",   "" },
	{ "GMSEC..>",             " -- Subject has '..' (is it missing an element?)" },
	{ "gmsec.>",              "element contains illegal character" },
	{ "GMSEC.FOO%BAR.>",      "element contains illegal character" },
	{ "",                     " -- Subject cannot be empty" },
};


void test_isValid_1(Test& test)
{
	GMSEC_INFO << "Test isValid(std::string&, bool)";

	// Test with no lenience
	for (size_t i = 0; i < sizeof(subjectSamples)/sizeof(Samples); ++i)
	{
		std::string result = Subject::isValid(subjectSamples[i].subject, false);
		bool        okay   = (result.find(subjectSamples[i].result) != std::string::npos);

		if (!okay)
		{
			GMSEC_WARNING << "Result for " << i << " is: " << result.c_str();
		}

		test.check("Unexpected result", okay);
	}

	// Test with lenience
	std::string result = Subject::isValid("gmsec.foo.bar", true);
	bool        okay   = (result == "");

	test.check("Unexpected lenience result", okay);
}


void test_isValid_2(Test& test)
{
	GMSEC_INFO << "Test isValid(std::string&, const InternalMessage*, bool)";

	MessageFactory factory;
	set_standard_fields(factory);

	Message msg = factory.createMessage("HB");

	// Test with no lenience
	{
		std::string result = Subject::isValid(msg.getSubject(), &MessageBuddy::getInternal(msg), false);
		bool        okay   = (result == "");

		test.check("Unexpected no lenience result", okay);
	}

	// Note: Cannot test with lenience because the API disallows
	// setting a message subject with lowercase character(s).
}


void test_isValidSubscription(Test& test)
{
	GMSEC_INFO << "Test isValidSubscription()";

	// Test with no lenience
	for (size_t i = 0; i < sizeof(subscriptionSamples)/sizeof(Samples); ++i)
	{
		std::string result = Subject::isValidSubscription(subscriptionSamples[i].subject, false);
		bool        okay   = (result.find(subscriptionSamples[i].result) != std::string::npos);

		if (!okay)
		{
			GMSEC_WARNING << "Result for " << i << " is: " << result.c_str();
		}

		test.check("Unexpected result", okay);
	}

	// Test with lenience
	std::string result = Subject::isValidSubscription("gmsec.foo.bar.>", true);
	bool        okay   = (result == "");

	test.check("Unexpected lenience result", okay);
}


void test_getElements(Test& test)
{
	GMSEC_INFO << "Test getElements()";

	std::vector<std::string> elements;
	bool result;

	// Nominal cases
	result = Subject::getElements("GMSEC.FOO.BAR", elements);
	test.check("Unexpected result", result);
	test.check("Unexpected elements", elements.size() == 3);
	elements.clear();

	result = Subject::getElements("GMSEC.FOO.BAR.>", elements);
	test.check("Unexpected result", result);
	test.check("Unexpected elements", elements.size() == 4);
	elements.clear();

	// Off-nominal case
	result = Subject::getElements("", elements);
	test.check("Unexpected result", result == false);
	test.check("Unexpected elements", elements.size() == 0);
	elements.clear();
}


void test_doesSubjectMatchPattern(Test& test)
{
	GMSEC_INFO << "Test doesSubjectMatchPattern(std::string&, std::string&)";

	test.check("Unexpected result", Subject::doesSubjectMatchPattern("GMSEC.FOO.BAR", "GMSEC.>"));
	test.check("Unexpected result", Subject::doesSubjectMatchPattern("GMSEC.FOO.BAR", "C2MS.>") == false);
	test.check("Unexpected result", Subject::doesSubjectMatchPattern("", "C2MS.>") == false);
	test.check("Unexpected result", Subject::doesSubjectMatchPattern("GMSEC.FOO.BAR", "") == false);
	test.check("Unexpected result", Subject::doesSubjectMatchPattern("gmsec.foo.bar", "GMSEC.>") == false);
}


int test_Subject(Test& test)
{
	test.setDescription("Subject");

	test_isValid_1(test);
	test_isValid_2(test);
	test_isValidSubscription(test);
	test_getElements(test);
	test_doesSubjectMatchPattern(test);

	return 0;
}

TEST_DRIVER(test_Subject)
