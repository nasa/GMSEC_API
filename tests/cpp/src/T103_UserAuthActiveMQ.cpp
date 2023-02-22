#include "TestCase.h"

#include <string>


using namespace gmsec::api5;
using namespace gmsec::api5::util;


/*
TEST DESCRIPTIONS:

Nominal Test Cases...
o	Connect to the server using the correct username and password

Off-Nominal Test Cases...
o	Attempt to connect to the server using an incorrect username
o	Attempt to connect to the server using an incorrect password
*/


struct ConfigOption
{
	const char* option;
	const char* value;
};


ConfigOption subConfigOptions[] = {
    { "MW-USERNAME", "nightrun" },
    { "MW-PASSWORD", "password" }
};

const char* BAD_USERNAME = "notauser";
const char* BAD_PASSWORD = "wrong";


int test_UserAuthActiveMQ(Test& test)
{
	test.setDescription("Active MQ User Authentication");

	Config config = test.getConfig();

	const char* mw = config.getValue("MW-ID");

	if (!mw)
	{
		test.excuse("Unable to deduce middleware type.");
		return -1;
	}
	else if (!strcontains(mw, "activemq"))
	{
		test.excuse("Test is only applicable to ActiveMQ clients");
		return -1;
	}

	// Set connection configuration options for the subscriber.
	for (unsigned int i = 0; i < sizeof(subConfigOptions) / sizeof(ConfigOption); ++i)
	{
		config.addValue(subConfigOptions[i].option, subConfigOptions[i].value);
	}

	StdUniquePtr<Connection> conn;


	//o Test Case 1: Connect to the server using the correct username and password
	try
	{
		// Connect to the WebSphere server
		conn.reset( new Connection(config) );

		conn->connect();

		test.check("Passed", true);

		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}


	//o Test Case 2: Attempt to connect to the server using an incorrect username
	// Reset the configuration parameters to a nominal state
	for (unsigned int i = 0; i < sizeof(subConfigOptions) / sizeof(ConfigOption); ++i)
	{
		config.addValue(subConfigOptions[i].option, subConfigOptions[i].value);
	}

	// Set the username to an incorrect value
	config.addValue("MW-USERNAME", BAD_USERNAME);

	try
	{
		conn.reset( new Connection(config) );

		try
		{
			conn->connect();
			test.check("Connection with BAD USERNAME should have failed", false);
		}
		catch (const GmsecException& e)
		{
			test.check("Passed", true);
		}
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}


	//o Test Case 3: Attempt to connect to the server using an incorrect password
	// Reset the configuration parameters to a nominal state
	for (unsigned int i = 0; i < sizeof(subConfigOptions) / sizeof(ConfigOption); ++i)
	{
		config.addValue(subConfigOptions[i].option, subConfigOptions[i].value);
	}

	// Set the password to an incorrect value
	config.addValue("MW-PASSWORD", BAD_PASSWORD);

	try
	{
		conn.reset( new Connection(config) );

		try
		{
			conn->connect();
			test.check("Connection with BAD PASSWORD should have failed", false);
		}
		catch (const GmsecException& e)
		{
			test.check("Passed", true);
		}
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	return 0;
};

TEST_DRIVER(test_UserAuthActiveMQ)
