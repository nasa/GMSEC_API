#include "TestCase.h"

#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/GmsecException.h>

#include <iomanip>
#include <iostream>
#include <string>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


struct TestCase
{
	enum TestCaseKind { HEADER, TIMESTAMP, IP, LEAP_YEAR };

	std::string str;
	bool expectedToBeValid;

	static void invokeTest(Test& test, TestCase* tc, size_t num_tc, TestCaseKind kind_tc)
	{
		const int         str_width = 45;
		const int         exp_width = 10;
		const int         act_width = 10;
		const int         num_flds  = 3;
		const std::string sep       = "|";
		const std::string spc       = " ";
		const int         tot_width = str_width + exp_width + act_width + ((sep.size() + spc.size()) * num_flds);
		const std::string line      = sep + std::string(tot_width-1, '-') + '|';

		const char* desc_tc;
		switch (kind_tc)
		{
			case HEADER:
				desc_tc = "Is Header String Valid?";
				break;
			case TIMESTAMP:
				desc_tc = "Is Timestamp Valid?";
				break;
			case IP:
				desc_tc = "Is IP Address Valid?";
				break;
			case LEAP_YEAR:
				desc_tc = "Is Leap Year?";
				break;
		}

		std::cerr << std::left << line << "\n" << sep
		          << spc << std::setw(str_width) << desc_tc << sep
		          << spc << std::setw(exp_width) << "Expected" << sep
		          << spc << std::setw(act_width) << "Actual" << sep
		          << "\n" << line << "\n";

		for (size_t i = 0; i < num_tc; ++i)
		{
			bool result;

			switch (kind_tc)
			{
			case HEADER:
				result = StringUtil::isValidHeaderString(tc[i].str);
				break;
			case TIMESTAMP:
				result = StringUtil::isValidTimestamp(tc[i].str).isValid();
				break;
			case IP:
				result = StringUtil::isValidIpAddress(tc[i].str);
				break;
			case LEAP_YEAR:
				{
					unsigned int year = StringUtil::getValue<unsigned int>(tc[i].str.c_str());
					result = StringUtil::isLeapYear(year);
				}
				break;
			}

			bool error = (tc[i].expectedToBeValid != result);

			std::cerr << std::left << sep
			          << spc << std::setw(str_width) << tc[i].str << sep
			          << spc << std::setw(exp_width) << (tc[i].expectedToBeValid ? "true" : "false") << sep
			          << spc << std::setw(act_width) << (result ? "true" : "false") << sep
			          << (error ? "\t<-- Error!" : "")
			          << "\n";

			test.check("Invalid entry found", error == false);
		}

		std::cerr << line << "\n\n" << std::endl;
	}
};


int test_StringUtil(Test& test)
{
	test.setDescription("StringUtil");

	struct TestCase headers[] =
	{
		{ "MY-COMPONENT",   true  },
		{ "MY_COMPONENT",   true  },
		{ "MY_COMPONENT-6", true  },
		{ "MY*COMPONENT",   false },
		{ "MY COMPONENT",   false },
		{ "My-Component",   false },
	};

	struct TestCase timestamps[] =
	{
		{ "2019-296-22:06:36",        true  },
		{ "2019-296-22:06:36.563",    true  },
		{ "9999-355-23:59:59.999999", true  },
		{ "2000-366-22:06:36.563",    true  },
		{ "2100-365-22:06:36.563",    true  },
		{ "2019-296-22:06:36.0",      true  },
		{ "2019-296-22:06:36.999999", true  },
		{ "2019-296-22:06:60.563",    true  },
		{ "+2019-296-22:06:36.9",     true  },
		{ "-2019-296-22:06:36.9",     true  },
		{ "-355-23:59:59.999999",     true  },
		{ "-23:59:59.9",              true  },
		{ "-59:59.9",                 true  },
		{ "-36.9",                    true  },
		{ "-59",                      true  },
		{ "-366-12:34:56.9",          true  },
		{ "-366-12:34:56",            true  },
		{ "2019--06-22:06:36.563",    false },
		{ "2019-06-22:06:36.563",     false },
		{ "2019-366-22:06:36.563",    false },
		{ "2100-366-22:06:36.563",    false },
		{ "2019_296-22:06:36.563",    false },
		{ "2019-496-22:06:36.563",    false },
		{ "2019-296-24:06:36.563",    false },
		{ "2019-296-22:60:36.563",    false },
		{ "2019-296-22:06:61.563",    false },
		{ "2019-296-22:06:36.",       false },
		{ "%2019-296-22:06:36.9",     false },
		{ "-123-06:36.9",             false },
		{ "-367-00:00:00",            false },
		{ "-24:06:36.9",              false },
		{ "-60:36.9",                 false },
		{ "-06:63.9",                 false },
		{ "-63.9999",                 false },
		{ "-59.",                     false },
		{ "-63",                      false },
	};

	struct TestCase addresses[] =
	{
		{ "192.168.1.12",                             true  },
		{ "192.168.1.123",                            true  },
		{ "123.6.9.012",                              true  },
		{ "255.255.255.255",                          true  },
		{ "256.168.1.123",                            false },
		{ "192.168.1.",                               false },
		{ "192.168.1",                                false },
		{ ".168.1.123",                               false },
		{ "FE80:CD00:0000:0CDE:1257:0000:211E:729C",  true  },
		{ "FE80:CD00:0:CDE:1257:0:211E:729C",         true  },
		{ "FE80:ZD00:0000:0CDE:1257:0000:211E:729C",  false },
		{ ":CD00:0:CDE:1257:0:211E:729C",             false },
		{ "ABCD:0:CDE:1257:0:211E:729C",              false },
		{ "Foobar",                                   false },
		{ "Hard stop.",                               false },
		{ "AC:DC",                                    false },
	};

	struct TestCase years[] =
	{
		{ "1996", true  },
		{ "2000", true  },
		{ "2003", false },
		{ "2004", true  },
		{ "2008", true  },
		{ "2010", false },
		{ "2016", true  },
		{ "2019", false },
		{ "2020", true  },
		{ "2100", false },
	};

	//o Header Strings
	TestCase::invokeTest(test, headers, sizeof(headers) / sizeof(TestCase), TestCase::HEADER);

	//o Timestamps
	TestCase::invokeTest(test, timestamps, sizeof(timestamps) / sizeof(TestCase), TestCase::TIMESTAMP);

	//o IP Addresses
	TestCase::invokeTest(test, addresses, sizeof(addresses) / sizeof(TestCase), TestCase::IP);

	//o (Leap) Years
	TestCase::invokeTest(test, years, sizeof(years) / sizeof(TestCase), TestCase::LEAP_YEAR);

	//o Off-nominal test(s)
	try {
		(void) StringUtil::getValue<int>(NULL);   // null value
		test.check("Expected exception from getValue()", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), true);
	}

	try {
		(void) StringUtil::getValue<GMSEC_CHAR>("FOO-BAR");  // string value, not a char value
		test.check("Expected exception from getValue()", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), true);
	}

	return 0;
}

TEST_DRIVER(test_StringUtil)
