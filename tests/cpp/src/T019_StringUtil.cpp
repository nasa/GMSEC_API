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
		const size_t      str_width = 45;
		const size_t      exp_width = 10;
		const size_t      act_width = 10;
		const size_t      num_flds  = 3;
		const std::string sep       = "|";
		const std::string spc       = " ";
		const size_t      tot_width = str_width + exp_width + act_width + ((sep.size() + spc.size()) * num_flds);
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

		std::ostringstream oss;

		oss << std::left << "\n" << line << "\n" << sep
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

			oss << std::left << sep
			    << spc << std::setw(str_width) << tc[i].str << sep
			    << spc << std::setw(exp_width) << (tc[i].expectedToBeValid ? "true" : "false") << sep
			    << spc << std::setw(act_width) << (result ? "true" : "false") << sep
			    << (error ? "\t<-- Error!" : "")
			    << "\n";

			test.check("Invalid entry found", error == false);
		}

		oss << line << "\n\n" << std::endl;

		GMSEC_DEBUG << oss.str().c_str();
	}
};


void test_string_copy(Test& test)
{
	GMSEC_INFO << "Test stringCopy()";

	const char* src = "Testing is no fun at all";
	char dest[256] = {0};

	//o No copying for the following scenarios:
	StringUtil::stringCopy(NULL, src, 0);
	test.check("Unexpected modification of dest buffer", *dest == '\0');
	StringUtil::stringCopy(dest, NULL, 0);
	test.check("Unexpected modification of dest buffer", *dest == '\0');
	StringUtil::stringCopy(dest, src, 0);
	test.check("Unexpected modification of dest buffer", *dest == '\0');

	//o Source length is less than destination size
	StringUtil::stringCopy(dest, src, sizeof(dest));
	test.check("Unexpected content", StringUtil::stringEquals(dest, src));
	*dest='\0';

	//o Source length is more than destination size
	StringUtil::stringCopy(dest, src, 8);
	test.check("Unexpected content", StringUtil::stringEquals(dest, "Testing"));
	*dest='\0';
}


void test_trim(Test& test)
{
	GMSEC_INFO << "Test trim()";

	std::string str = "LLLfooLLL";

	//o Empty string for the source
	std::string res = StringUtil::trim("", 'L');
	test.check("Unexpected trim result 1", res == "");

	//o String with no characters to trim away
	res = StringUtil::trim(str, 'M');
	test.check("Unexpected trim result 2", res == str);

	//o String with characters to trim away
	res = StringUtil::trim(str, 'L');
	test.check("Unexpected trim result 3", res == "foo");
}


void test_get_time_from_string(Test& test)
{
	GMSEC_INFO << "Test getTimeFromString()";

	double curTime = TimeUtil::getCurrentTime_s();

	//o Test future time
	double theTime = StringUtil::getTimeFromString("+00:00:10.1");
	test.check("Unexpected future time", theTime == static_cast<int>(curTime) + 10.1);

	//o Test past time
	theTime = StringUtil::getTimeFromString("-00:00:10.1");
	test.check("Unexpected past time", theTime == static_cast<int>(curTime) - 10.1);
}


void test_string_parse_f32(Test& test)
{
	GMSEC_INFO << "Test stringParseF32()";

	const char* s1 = NULL;
	const char* s2 = "3.14";
	const char* s3 = "";

	bool result;
	GMSEC_F32 out = 0.0f;

	//o Input string is NULL
	result = StringUtil::stringParseF32(s1, out);
	test.check("Unexpected F32 convert result", result == false);
	test.check("Unexpected F32 value", out == 0.0f);
	out = 0.0f;

	//o Input string is legit
	result = StringUtil::stringParseF32(s2, out);
	test.check("Unexpected F32 convert result", result);
	test.check("Unexpected F32 value", out == 3.14f);
	out = 0.0f;

	//o Input string is not legit
	result = StringUtil::stringParseF32(s3, out);
	test.check("Unexpected F32 convert result", result == false);
	test.check("Unexpected F32 value", out == 0.0f);
	out = 0.0f;
}


void test_string_parse_f64(Test& test)
{
	GMSEC_INFO << "Test stringParseF64()";

	const char* s1 = NULL;
	const char* s2 = "3.14";
	const char* s3 = "";

	bool result;
	GMSEC_F64 out = 0.0;

	//o Input string is NULL
	result = StringUtil::stringParseF64(s1, out);
	test.check("Unexpected F32 convert result", result == false);
	test.check("Unexpected F32 value", out == 0.0);
	out = 0.0;

	//o Input string is legit
	result = StringUtil::stringParseF64(s2, out);
	test.check("Unexpected F32 convert result", result);
	test.check("Unexpected F32 value", out == 3.14);
	out = 0.0;

	//o Input string is not legit
	result = StringUtil::stringParseF64(s3, out);
	test.check("Unexpected F32 convert result", result == false);
	test.check("Unexpected F32 value", out == 0.0);
	out = 0.0;
}


void test_strip_white_space(Test& test)
{
	GMSEC_INFO << "Test stripWhiteSpace()";

	//o Empty string
	test.check("Expected same string", StringUtil::stripWhiteSpace("") == "");

	//o Leading white-space
	test.check("Expected no white space", StringUtil::stripWhiteSpace(" \tABC") == "ABC");

	//o Trailing white-space
	test.check("Expected no white space", StringUtil::stripWhiteSpace("ABC \t") == "ABC");

	//o Leading and trailing white-space
	test.check("Expected no white space", StringUtil::stripWhiteSpace("  ABC \t") == "ABC");
}


void test_gmsec_compress_uncompress(Test& test)
{
	GMSEC_INFO << "Test gmsec_compress() and gmsec_uncompress()";

	DataBuffer in1(100), in2;
	DataBuffer out1, out2;

	std::memset(in1.raw(), 0xEB, in1.size());

	//o Compress
	Status result = StringUtil::gmsec_compress(in1, out1);
	test.check("Unexpected result when compressing", result.hasError() == false);

	//o Uncompress
	in2 = out1;
	result = StringUtil::gmsec_uncompress(in2, out2);
	test.check("Unexpected result when uncompressing", result.hasError() == false);

	test.check("Original and uncompressed are not the same", in1.equals(out2));
}


void test_to_xml(Test& test)
{
	GMSEC_INFO << "Test toXML()";

	const char* data = "FOO.\n.\".&.'.<.>.\r";

	std::string xml = StringUtil::toXML(data);

	test.check("Unexpected XML content", xml == "FOO.&#10;.&quot;.&amp;.&apos;.&lt;.&gt;.&#13;");
}
void test_timestamps(Test& test)
{
	GMSEC_INFO << "Test isValidTimestamp()";

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

	TestCase::invokeTest(test, timestamps, sizeof(timestamps) / sizeof(TestCase), TestCase::TIMESTAMP);
}


void test_ip_addresses(Test& test)
{
	GMSEC_INFO << "Test isValidIpAddress()";

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

	TestCase::invokeTest(test, addresses, sizeof(addresses) / sizeof(TestCase), TestCase::IP);
}


void test_leap_years(Test& test)
{
	GMSEC_INFO << "Test isLeapYear()";

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

	TestCase::invokeTest(test, years, sizeof(years) / sizeof(TestCase), TestCase::LEAP_YEAR);
}


void test_remove_leading_zeros(Test& test)
{
	GMSEC_INFO << "Test removeLeadingZeros";

	unsigned char blob[] = { 0x00, 0x00, 0x07 };
	size_t blobLen = sizeof(blob)/sizeof(unsigned char);

	StringUtil::Data data(blob, blobLen);

	StringUtil::Data out = StringUtil::removeLeadingZeros(data);

	test.check("Unexpected data output", out.size() == 1 && out[0] == 0x07);
}


void test_pad_with_leading_zeros(Test& test)
{
	GMSEC_INFO << "Test padWithLeadingZeros";

	unsigned char blob[] = { 0x07 };
	size_t blobLen = sizeof(blob)/sizeof(unsigned char);

	StringUtil::Data data(blob, blobLen);

	StringUtil::Data out = StringUtil::padWithLeadingZeros(data, 2);

	test.check("Unexpected data output", out.size() == 3 && out[0] == 0x00  && out[1] == 0x00  && out[2] == 0x07);
}


void test_headers(Test& test)
{
	GMSEC_INFO << "Test isValidHeaderString()";

	struct TestCase headers[] =
	{
		{ "MY-COMPONENT",   true  },
		{ "MY_COMPONENT",   true  },
		{ "MY_COMPONENT-6", true  },
		{ "MY*COMPONENT",   false },
		{ "MY COMPONENT",   false },
		{ "My-Component",   false },
		{ "",               false },
	};

	TestCase::invokeTest(test, headers, sizeof(headers) / sizeof(TestCase), TestCase::HEADER);
}


void test_string_to_binary(Test& test)
{
	GMSEC_INFO << "Test string_toBinary()";

	const char* value = "1234";

	// TODO: What does string_toBinary() do? Is a utility method really
	// necessary to convert a const char* into an std::basic_string?
	//std::basic_string<unsigned char> data(reinterpret_cast<const unsigned char*>(value), std::string(value).length());

	StringUtil::Data data = StringUtil::string_toBinary(value);

	//o Off-nominal test
	try {
		StringUtil::string_toBinary(NULL);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Value cannot be NULL") != std::string::npos);
	}
}


void test_binary_string_to_binary(Test& test)
{
	GMSEC_INFO << "Test binaryString_toBinary()";

	const char* value = "44FC6000";

	StringUtil::Data data = StringUtil::binaryString_toBinary(value);

	BinaryField field("BIN-FIELD", data.data(), data.length());

	test.check("Unexpected Field content", StringUtil::stringEquals(field.toXML(),
	           "<FIELD NAME=\"BIN-FIELD\" TYPE=\"BIN\">44FC6000</FIELD>"));
}


void test_i8_to_binary(Test& test)
{
	GMSEC_INFO << "Test I8_toBinary()";

	GMSEC_I8 value = 64;

	StringUtil::Data data = StringUtil::I8_toBinary(value);
	test.check("Unexpected data length", data.length() == 1);
	test.check("Unexpected value", static_cast<GMSEC_I8>(StringUtil::I32_fromBinary(data)) == value);
}


void test_i16_to_binary(Test& test)
{
	GMSEC_INFO << "Test I16_toBinary()";

	GMSEC_I16 value = 64;

	StringUtil::Data data = StringUtil::I16_toBinary(value);
	test.check("Unexpected data length", data.length() == 1);
	test.check("Unexpected value", static_cast<GMSEC_I16>(StringUtil::I32_fromBinary(data)) == value);
}


void test_i32_to_binary(Test& test)
{
	GMSEC_INFO << "Test I32_toBinary()";

	GMSEC_I32 value = 256;

	StringUtil::Data data = StringUtil::I32_toBinary(value);
	test.check("Unexpected data length", data.length() == 2);
	test.check("Unexpected value", StringUtil::I32_fromBinary(data) == value);
}


void test_i64_to_binary(Test& test)
{
	GMSEC_INFO << "Test I64_toBinary()";

	GMSEC_I64 value = 1028;

	StringUtil::Data data = StringUtil::I64_toBinary(value);
	test.check("Unexpected data length", data.length() == 2);
	test.check("Unexpected value", StringUtil::I64_fromBinary(data) == value);
}


void test_u8_to_binary(Test& test)
{
	GMSEC_INFO << "Test U8_toBinary()";

	GMSEC_U8 value = 64;

	StringUtil::Data data = StringUtil::U8_toBinary(value);
	test.check("Unexpected data length", data.length() == 1);
	test.check("Unexpected value", static_cast<GMSEC_U8>(StringUtil::U32_fromBinary(data)) == value);
}


void test_u16_to_binary(Test& test)
{
	GMSEC_INFO << "Test U16_toBinary()";

	GMSEC_U16 value = 64;

	StringUtil::Data data = StringUtil::U16_toBinary(value);
	test.check("Unexpected data length", data.length() == 1);
	test.check("Unexpected value", static_cast<GMSEC_U16>(StringUtil::U32_fromBinary(data)) == value);
}


void test_u32_to_binary(Test& test)
{
	GMSEC_INFO << "Test U32_toBinary()";

	GMSEC_U32 value = 256;

	StringUtil::Data data = StringUtil::U32_toBinary(value);
	test.check("Unexpected data length", data.length() == 2);
	test.check("Unexpected value", StringUtil::U32_fromBinary(data) == value);
}


void test_u64_to_binary(Test& test)
{
	GMSEC_INFO << "Test U64_toBinary()";

	GMSEC_U64 value = 1028;

	StringUtil::Data data = StringUtil::U64_toBinary(value);
	test.check("Unexpected data length", data.length() == 2);
	test.check("Unexpected value", StringUtil::U64_fromBinary(data) == value);
}


void test_f32_to_binary(Test& test)
{
	GMSEC_INFO << "Test F32_toBinary()";

	GMSEC_F32 value = 1024.0f;

	StringUtil::Data data = StringUtil::F32_toBinary(value);
	test.check("Unexpected data length", data.length() == 4);
}


void test_f64_to_binary(Test& test)
{
	GMSEC_INFO << "Test F64_toBinary()";

	GMSEC_F64 value = 1024.0;

	StringUtil::Data data = StringUtil::F64_toBinary(value);
	test.check("Unexpected data length", data.length() == 8);
	test.check("Unexpected value", StringUtil::F64_fromBinary(data) == value);
}


void test_xml_error_to_string(Test& test)
{
	GMSEC_INFO << "Test xmlErrorToString()";

	struct Error {
		tinyxml2::XMLError error;
		const char*        errorMsg;
	};

	Error errors[21] = {
		{ tinyxml2::XML_NO_ERROR, "No error" },
		{ tinyxml2::XML_NO_ATTRIBUTE, "No attribute" },
		{ tinyxml2::XML_WRONG_ATTRIBUTE_TYPE, "Wrong attribute type" },
		{ tinyxml2::XML_ERROR_FILE_NOT_FOUND, "File not found" },
		{ tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED, "File could not be opened" },
		{ tinyxml2::XML_ERROR_FILE_READ_ERROR, "File read error" },
		{ tinyxml2::XML_ERROR_ELEMENT_MISMATCH, "Element mismatch" },
		{ tinyxml2::XML_ERROR_PARSING_ELEMENT, "Error parsing element" },
		{ tinyxml2::XML_ERROR_PARSING_ATTRIBUTE, "Error parsing attribute" },
		{ tinyxml2::XML_ERROR_IDENTIFYING_TAG, "Error identifying tag" },
		{ tinyxml2::XML_ERROR_PARSING_TEXT, "Error parsing text" },
		{ tinyxml2::XML_ERROR_PARSING_CDATA, "Error parsing CDATA" },
		{ tinyxml2::XML_ERROR_PARSING_COMMENT, "Error parsing comment" },
		{ tinyxml2::XML_ERROR_PARSING_DECLARATION, "Error parsing declaration" },
		{ tinyxml2::XML_ERROR_PARSING_UNKNOWN, "Error parsing unknown" },
		{ tinyxml2::XML_ERROR_EMPTY_DOCUMENT, "Empty document" },
		{ tinyxml2::XML_ERROR_MISMATCHED_ELEMENT, "Mismatched element" },
		{ tinyxml2::XML_ERROR_PARSING, "Error parsing" },
		{ tinyxml2::XML_CAN_NOT_CONVERT_TEXT, "Can not convert text" },
		{ tinyxml2::XML_NO_TEXT_NODE, "No text node" },
		{ static_cast<tinyxml2::XMLError>(42), "Unknown error" },
	};

	for (size_t i = 0; i < (sizeof(errors)/sizeof(Error)); ++i)
	{
		tinyxml2::XMLError error = errors[i].error;
		const char*        emsg  = errors[i].errorMsg;

		test.check("Unexpected error string", StringUtil::stringEquals(StringUtil::xmlErrorToString(error), emsg));
	}
}


void test_get_value(Test& test)
{
	GMSEC_INFO << "Test getValue<T>()";

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
}


int test_StringUtil(Test& test)
{
	test.setDescription("StringUtil");

	test_string_copy(test);
	test_trim(test);
	test_get_time_from_string(test);
	test_string_parse_f32(test);
	test_string_parse_f64(test);
	test_strip_white_space(test);
	test_gmsec_compress_uncompress(test);
	test_to_xml(test);
	test_headers(test);
	test_timestamps(test);
	test_ip_addresses(test);
	test_leap_years(test);
	test_remove_leading_zeros(test);
	test_pad_with_leading_zeros(test);
	test_string_to_binary(test);
	test_binary_string_to_binary(test);
	test_i8_to_binary(test);
	test_i16_to_binary(test);
	test_i32_to_binary(test);
	test_i64_to_binary(test);
	test_u8_to_binary(test);
	test_u16_to_binary(test);
	test_u32_to_binary(test);
	test_u64_to_binary(test);
	test_f32_to_binary(test);
	test_f64_to_binary(test);
	test_xml_error_to_string(test);
	test_get_value(test);

	return 0;
}

TEST_DRIVER(test_StringUtil)
