#include "TestCase.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <string>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


struct ConfigOption
{
	const char* option;
	const char* value;
};


// Subscriber Configuration Options
ConfigOption LEGACY_subConfigOptions[] =
{
    { "MW-KEYSTOREPASSWORD",   "1234ABcd" },
    { "MW-TRUSTSTOREPASSWORD", "1234ABcd" }
};

ConfigOption subConfigOptions[] =
{
    { "MW-KEYSTORE-PASSWORD",   "1234ABcd" },
    { "MW-TRUSTSTORE-PASSWORD", "1234ABcd" }
};

const char* SSL_KEYSTORE   = "T104/client/client.ks.pem";
const char* SSL_TRUSTSTORE = "T104/client/client.ts.pem";


static void setupConfiguration(Test& test, Config& config, bool useLegacyOptions)
{
	// Set additional connection configuration options for the subscriber.
	if (useLegacyOptions)
	{
		for (size_t i = 0; i < sizeof(LEGACY_subConfigOptions) / sizeof(ConfigOption); ++i)
		{
			config.addValue(LEGACY_subConfigOptions[i].option, LEGACY_subConfigOptions[i].value);
		}
	}
	else
	{
		for (size_t i = 0; i < sizeof(subConfigOptions) / sizeof(ConfigOption); ++i)
		{
			config.addValue(subConfigOptions[i].option, subConfigOptions[i].value);
		}
	}

	// Add the keystore location to the configuration
	config.addValue("MW-KEYSTORE", test.getDataFile(SSL_KEYSTORE).c_str());

	//o Test case 1: Connect and subscribe to the ActiveMQ server using the correct SSL keystore.
	// Add the correct truststore location to the configuration
	config.addValue("MW-TRUSTSTORE", test.getDataFile(SSL_TRUSTSTORE).c_str());
}


int test_ActiveMQ_SSL(Test& test)
{
	test.setDescription("ActiveMQ SSL");

	Config config = test.getConfig();  // make a copy

	StdUniquePtr<Connection> conn;

	//o Nominal test (test both new and legacy configuration options)
	for (int i = 0; i < 2; ++i)
	{
		setupConfiguration(test, config, i == 0);

		try
		{
			conn.reset( new Connection(config) );

			conn->connect();

			test.check("Connected!", true);

			std::string subject = test.getSubject("ACTIVEMQ.SSL.TEST");

			conn->subscribe( subject.c_str() );

			Message msg;
			msg.setSubject( subject.c_str() );

			conn->publish(msg);

			Message* rcvd = conn->receive(5000);

			if (rcvd)
			{
				test.check("Received the message", true);

				Message::destroy(rcvd);
			}
			else
			{
				test.check("Failed to receive message", false);
			}

			conn->disconnect();
		}
		catch (const GmsecException& e)
		{
			test.require(e.what(), false);
		}
	}

	return 0;
}

TEST_DRIVER(test_ActiveMQ_SSL)
