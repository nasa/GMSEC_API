#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


void test_constructor_1(Test& test)
{
	GMSEC_INFO << "Test no-arg constructor...";

	const char* name  = NULL;
	const char* value = NULL;

	Config c1;

	test.check("Expected no config entries", c1.getFirst(name, value) == false);
	test.check("Expected no name", name == NULL);
	test.check("Expected no value", value == NULL);
}


void test_constructor_2(Test& test)
{
	GMSEC_INFO << "Test cmd-line arg constructor...";

	int argc = 2;
	char* argv[] = { (char*) "mw-id=bolt", (char*) "server=localhost", NULL };

	const char* name  = NULL;
	const char* value = NULL;

	Config c1(argc, argv);

	test.check("Expected config entry", c1.getFirst(name, value));
	test.check("Expected mw-id", std::string("mw-id") == name);
	test.check("Expected bolt", std::string("bolt") == value);

	test.check("Expected config entry", c1.getNext(name, value));
	test.check("Expected mw-id", std::string("server") == name);
	test.check("Expected bolt", std::string("localhost") == value);

	test.check("Expected no additional config entries", c1.getNext(name, value) == false);
}


void test_constructor_3(Test& test)
{
	GMSEC_INFO << "Test data constructor...";

	const char* name  = NULL;
	const char* value = NULL;

	// Using key=value pairs
	Config c1("mw-id=bolt server=localhost", DataType::KEY_VALUE_DATA);

	test.check("Expected config entry", c1.getFirst(name, value));
	test.check("Expected mw-id", std::string("mw-id") == name);
	test.check("Expected bolt", std::string("bolt") == value);

	test.check("Expected config entry", c1.getNext(name, value));
	test.check("Expected mw-id", std::string("server") == name);
	test.check("Expected bolt", std::string("localhost") == value);

	test.check("Expected no additional config entries", c1.getNext(name, value) == false);

	// Using XML
	Config c2(c1.toXML(), DataType::XML_DATA);

	test.check("Expected config entry", c2.getFirst(name, value));
	test.check("Expected mw-id", std::string("mw-id") == name);
	test.check("Expected bolt", std::string("bolt") == value);

	test.check("Expected config entry", c2.getNext(name, value));
	test.check("Expected mw-id", std::string("server") == name);
	test.check("Expected bolt", std::string("localhost") == value);

	test.check("Expected no additional config entries", c2.getNext(name, value) == false);

	// Using JSON
	Config c3(c1.toJSON(), DataType::JSON_DATA);

	test.check("Expected config entry", c3.getFirst(name, value));
	test.check("Expected mw-id", std::string("mw-id") == name);
	test.check("Expected bolt", std::string("bolt") == value);

	test.check("Expected config entry", c3.getNext(name, value));
	test.check("Expected mw-id", std::string("server") == name);
	test.check("Expected bolt", std::string("localhost") == value);

	test.check("Expected no additional config entries", c3.getNext(name, value) == false);


	// Using lowercase XML
	std::string lowerXML  = StringUtil::stringToLower(c1.toXML());

	Config c4(lowerXML.c_str(), DataType::XML_DATA);

	test.check("Expected config entry", c4.getFirst(name, value));
	test.check("Expected mw-id", std::string("mw-id") == name);
	test.check("Expected bolt", std::string("bolt") == value);

	test.check("Expected config entry", c4.getNext(name, value));
	test.check("Expected mw-id", std::string("server") == name);
	test.check("Expected bolt", std::string("localhost") == value);

	test.check("Expected no additional config entries", c4.getNext(name, value) == false);

	// Using lowercase JSON
	std::string lowerJSON = StringUtil::stringToLower(c1.toJSON());

	Config c5(lowerJSON.c_str(), DataType::JSON_DATA);

	test.check("Expected config entry", c5.getFirst(name, value));
	test.check("Expected mw-id", std::string("mw-id") == name);
	test.check("Expected bolt", std::string("bolt") == value);

	test.check("Expected config entry", c5.getNext(name, value));
	test.check("Expected mw-id", std::string("server") == name);
	test.check("Expected bolt", std::string("localhost") == value);

	test.check("Expected no additional config entries", c5.getNext(name, value) == false);
}


void test_copy_constructor(Test& test)
{
	GMSEC_INFO << "Test copy constructor...";

	const char* name  = NULL;
	const char* value = NULL;

	Config c1("mw-id=bolt server=localhost", DataType::KEY_VALUE_DATA);
	Config c2(c1);

	test.check("Expected config entry", c2.getFirst(name, value));
	test.check("Expected mw-id", std::string("mw-id") == name);
	test.check("Expected bolt", std::string("bolt") == value);

	test.check("Expected config entry", c2.getNext(name, value));
	test.check("Expected mw-id", std::string("server") == name);
	test.check("Expected bolt", std::string("localhost") == value);

	test.check("Expected no additional config entries", c2.getNext(name, value) == false);
}


void test_assignment_operator(Test& test)
{
	GMSEC_INFO << "Test assignment operator...";

	const char* name  = NULL;
	const char* value = NULL;

	Config c1("mw-id=bolt server=localhost", DataType::KEY_VALUE_DATA);
	Config c2;

	c2 = c1;

	test.check("Expected config entry", c2.getFirst(name, value));
	test.check("Expected mw-id", std::string("mw-id") == name);
	test.check("Expected bolt", std::string("bolt") == value);

	test.check("Expected config entry", c2.getNext(name, value));
	test.check("Expected mw-id", std::string("server") == name);
	test.check("Expected bolt", std::string("localhost") == value);

	test.check("Expected no additional config entries", c2.getNext(name, value) == false);
}


void test_addValue(Test& test)
{
	GMSEC_INFO << "Test addValue...";

	const char* name  = NULL;
	const char* value = NULL;

	Config c1;
	c1.addValue("mw-id", "bolt");

	test.check("Expected config entry", c1.getFirst(name, value));
	test.check("Expected mw-id", std::string("mw-id") == name);
	test.check("Expected bolt", std::string("bolt") == value);

	test.check("Expected no additional config entries", c1.getNext(name, value) == false);
}


void test_clearValue(Test& test)
{
	GMSEC_INFO << "Test clearValue...";

	const char* name  = NULL;
	const char* value = NULL;

	Config c1("mw-id=bolt", DataType::KEY_VALUE_DATA);

	test.check("Expected false when clearing non-existing key", c1.clearValue("server") == false);
	test.check("Expected true when clearing existing key", c1.clearValue("mw-id"));

	test.check("Expected no config entries", c1.getNext(name, value) == false);
}


void test_getValue(Test& test)
{
	GMSEC_INFO << "Test getValue...";

	Config c1("mw-id=bolt", DataType::KEY_VALUE_DATA);

	test.check("Unexpected value", std::string("bolt") == c1.getValue("mw-id"));
	test.check("Unexpected result for non-existent key", c1.getValue("server") == NULL);

	test.check("Unexpected result for NULL key", c1.getValue(NULL) == NULL);
	test.check("Unexpected result for empty key", c1.getValue("") == NULL);

	test.check("Unexpected default value", std::string("mb") == c1.getValue("connectionType", "mb"));
	test.check("Unexpected default value", std::string("mb") == c1.getValue(NULL, "mb"));
}


void test_getBooleanValue(Test& test)
{
	GMSEC_INFO << "Test getBooleanValue...";

	Config c1("gmsec-rocks=true", DataType::KEY_VALUE_DATA);
	Config c2("gmsec-rocks=sure", DataType::KEY_VALUE_DATA);

	test.check("Expected to find gmsec-rocks", c1.getBooleanValue("gmsec-rocks"));

	try {
		c1.getBooleanValue(NULL);
		test.check("Expected getBooleanValue() to throw exception", false);
	}
	catch (...) {
		test.check("Okay", true);
	}

	try {
		c2.getBooleanValue("gmsec-rocks");
		test.check("Expected getBooleanValue() to throw exception", false);
	}
	catch (...) {
		test.check("Okay", true);
	}

	test.check("Expected default value of false", c1.getBooleanValue("gmsec-missing", false) == false);
	test.check("Expected default value of true",  c1.getBooleanValue("gmsec-missing", true));
	test.check("Expected default value of true",  c2.getBooleanValue("gmsec-rocks", true));
	test.check("Expected default value of true",  c2.getBooleanValue("", true));
	test.check("Expected default value of false", c2.getBooleanValue(NULL, false) == false);
}


void test_getIntegerValue(Test& test)
{
	GMSEC_INFO << "Test getIntegerValue...";

	Config c1("gmsec-value=10", DataType::KEY_VALUE_DATA);
	Config c2("gmsec-value=NAN", DataType::KEY_VALUE_DATA);

	test.check("Unexpected integer value", c1.getIntegerValue("gmsec-value") == 10);

	try {
		c1.getIntegerValue(NULL);
		test.check("Expected getIntegerValue() to throw exception", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
	try {
		c2.getIntegerValue("gmsec-value");
		test.check("Expected getIntegerValue() to throw exception", false);
	}
	catch (...) {
		test.check("Okay", true);
	}

	test.check("Expected default value of 20", c1.getIntegerValue("gmsec-missing", 20));
	test.check("Expected default value of 30", c2.getIntegerValue("gmsec-value", 30));
	test.check("Expected default value of 10", c2.getIntegerValue("", 10));
	test.check("Expected default value of 10", c2.getIntegerValue(NULL, 10));
}


void test_getDoubleValue(Test& test)
{
	GMSEC_INFO << "Test getDoubleValue...";

	Config c1("gmsec-value=10.15", DataType::KEY_VALUE_DATA);
	Config c2("gmsec-value=NAN", DataType::KEY_VALUE_DATA);

	test.check("Unexpected double value", c1.getDoubleValue("gmsec-value") == 10.15);

	try {
		c1.getDoubleValue(NULL);
		test.check("Expected getDoubleValue() to throw exception", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
	try {
		c2.getDoubleValue("gmsec-value");
		test.check("Expected getDoubleValue() to throw exception", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	test.check("Expected default value of 20.22", c1.getDoubleValue("gmsec-missing", 20.22));
	test.check("Expected default value of 30.33", c2.getDoubleValue("gmsec-value", 30.33));
	test.check("Expected default value of 10.33", c2.getDoubleValue("", 10.33));
	test.check("Expected default value of 10.33", c2.getDoubleValue(NULL, 10.33));
}


void test_clear(Test& test)
{
	GMSEC_INFO << "Test clear...";

	const char* name  = NULL;
	const char* value = NULL;

	Config c1("mw-id=bolt server=localhost", DataType::KEY_VALUE_DATA);

	c1.clear();

	test.check("Unexpected result from getNext",  c1.getNext(name, value) == false);
	test.check("Unexpected result from getFirst", c1.getFirst(name, value) == false);
}


void test_merge(Test& test)
{
	GMSEC_INFO << "Test merge...";

	const char* name  = NULL;
	const char* value = NULL;

	Config c1("mw-id=bolt", DataType::KEY_VALUE_DATA);
	Config c2("mw-id=bolt", DataType::KEY_VALUE_DATA);
	Config c3("mw-id=mb server=localhost", DataType::KEY_VALUE_DATA);

	// Merge w/ overwrite
	c1.merge(c3, true);

	test.check("Expected to find first entry", c1.getFirst(name, value));
	test.check("Unexpected entry name", std::string("mw-id") == name);
	test.check("Unexpected entry value", std::string("mb") == value);

	test.check("Expected to find second entry", c1.getNext(name, value));
	test.check("Unexpected entry name", std::string("server") == name);
	test.check("Unexpected entry value", std::string("localhost") == value);

	test.check("Was not expected to find third entry", c1.getNext(name, value) == false);

	// Merge w/ no overwrite
	c2.merge(c3, false);

	test.check("Expected to find first entry", c2.getFirst(name, value));
	test.check("Unexpected entry name", std::string("mw-id") == name);
	test.check("Unexpected entry value", std::string("bolt") == value);

	test.check("Expected to find second entry", c2.getNext(name, value));
	test.check("Unexpected entry name", std::string("server") == name);
	test.check("Unexpected entry value", std::string("localhost") == value);

	test.check("Was not expected to find third entry", c2.getNext(name, value) == false);
}


void test_toXML(Test& test)
{
	GMSEC_INFO << "Test toXML...";

	std::string xml = "<CONFIG>\n"
	                  "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
	                  "\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
	                  "</CONFIG>";

	Config c1("mw-id=bolt server=localhost", DataType::KEY_VALUE_DATA);

	test.check("Unexpected XML content", xml == c1.toXML());
}


void test_fromXML(Test& test)
{
	GMSEC_INFO << "Test fromXML...";

	std::string xml = "<CONFIG>\n"
	                  "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
	                  "\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
	                  "</CONFIG>";


	Config c1;

	c1.fromXML(xml.c_str());

	test.check("Unexpected XML content", xml == c1.toXML());
}


void test_getFromFile(Test& test)
{
	GMSEC_INFO << "Test getFromFile...";

	// Nominal test
	try
	{
		Config config1 = Config::getFromFile( test.getDataFile("T002_Config_good.xml").c_str(), "Bolt" );
		test.check("Unexpected mw-id value", strcompare(config1.getValue("mw-id"), "bolt") == 0);
		test.check("Unexpected server value", strcompare(config1.getValue("server"), "localhost") == 0);

		Config config2 = Config::getFromFile( test.getDataFile("T002_Config_good.xml").c_str(), "ActiveMQ" );
		test.check("Unexpected mw-id value", strcompare(config2.getValue("mw-id"), "activemq39") == 0);
		test.check("Unexpected server value", strcompare(config2.getValue("server"), "tcp://localhost:61616") == 0);

		Config config3 = Config::getFromFile( test.getDataFile("T002_Config_good.xml").c_str(), NULL);
		test.check("Unexpected mw-id value", strcompare(config3.getValue("mw-id"), "bolt") == 0);
		test.check("Unexpected server value", strcompare(config3.getValue("server"), "localhost") == 0);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal tests
	try
	{
		// malformed configuration file
		Config::getFromFile( test.getDataFile("T002_Config_good.xml").c_str(), "FooBar" );
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), strcontains(e.what(), "Named configuration could not be found"));
	}

	try
	{
		// malformed configuration file
		Config::getFromFile( test.getDataFile("T002_Config_bad.xml").c_str(), "Bolt" );
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), strcontains(e.what(), "Invalid XML format -- parse error"));
	}

	try
	{
		// non-existent configuration file
		Config::getFromFile( "file-does-not-exist", "Bolt" );
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), strcontains(e.what(), "Configuration file could not be found or opened"));
	}

	try
	{
		// NULL configuration file name
		Config::getFromFile(NULL, "Bolt");
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), strcontains(e.what(), "Configuration file name cannot be NULL, nor be an empty string"));
	}

	try
	{
		// empty configuration file name
		Config::getFromFile("", "Bolt");
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), strcontains(e.what(), "Configuration file name cannot be NULL, nor be an empty string"));
	}
}


void test_toJSON(Test& test)
{
	GMSEC_INFO << "Test toJSON...";

	std::string json = "{\"CONFIG\":{\"PARAMETER\":[{\"NAME\":\"mw-id\",\"VALUE\":\"bolt\"},{\"NAME\":\"server\",\"VALUE\":\"localhost\"}]}}";

	Config c1("mw-id=bolt server=localhost", DataType::KEY_VALUE_DATA);

	test.check("Unexpected JSON content", json == c1.toJSON());
}


int test_Config(Test& test)
{
	test.setDescription("Test Config");

	test_constructor_1(test);
	test_constructor_2(test);
	test_constructor_3(test);
	test_copy_constructor(test);
	test_assignment_operator(test);
	test_addValue(test);
	test_clearValue(test);
	test_getValue(test);
	test_getBooleanValue(test);
	test_getIntegerValue(test);
	test_getDoubleValue(test);
	test_clear(test);
	test_merge(test);
	test_toXML(test);
	test_fromXML(test);
	test_getFromFile(test);
	test_toJSON(test);

	return 0;
}

TEST_DRIVER(test_Config)

