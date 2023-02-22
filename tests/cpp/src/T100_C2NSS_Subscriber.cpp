#include "TestCase.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <string>
#include <vector>
#include <algorithm>   // for std::transform
#include <cstring>     // for memset()
#include <ctype.h>     // for toupper()


using namespace gmsec::api5;
using namespace gmsec::api5::util;


std::string getTopic(Test& test, const Config& config)
{
    static int testNum = 1;

    std::string hostname = test.getHostname();

    const char* mw = config.getValue("mw-id");
    if (!mw)
    {
        mw = config.getValue("connectionType");
    }

    std::stringstream ss;
    ss << "GMSEC.T100." << hostname.c_str() << "." << (mw ? mw : "mw-unknown") << "." << (testNum++);

    std::string topic = ss.str();

    std::transform(topic.begin(), topic.end(), topic.begin(), ::toupper);

    return topic;
}


void checkReadiness(const char* cookie)
{
	int tries = 0;

	while (++tries < 30)
	{
		if (std::fstream(cookie, std::fstream::in))
		{
			std::remove(cookie);
			break;
		}

		TimeUtil::millisleep(1000);
	}
}


void test_ValDigest(Test& test)
{
	Config config = test.getConfig();   // make a copy!

	config.addValue("sec-policy",           "c2_nss");
	config.addValue("sec-nss-dir",          test.getDataFile("nssdb").c_str());
	config.addValue("sec-nss-key",          "x");
	config.addValue("sec-auth-name",        "GIS1");
	config.addValue("sec-val-digest",       "true");
	config.addValue("sec-accept-encrypted", "true");

	std::string topic = getTopic(test, config);

	StdUniquePtr<Connection> conn(0);

	try
	{
		conn.reset( new Connection(config) );

		set_standard_fields( conn->getMessageFactory() );

		conn->connect();

		GMSEC_INFO << "Subscribing to: " << topic.c_str();
		conn->subscribe( topic.c_str() );

		checkReadiness( topic.c_str() );

		Message* msg = conn->receive(10000);

		if (msg)
		{
			GMSEC_VERBOSE << "Received Message:\n" << msg->toXML();

			test.check("Message is missing MESSAGE-TYPE", msg->hasField("MESSAGE-TYPE"));
			test.check("Message is missing MESSAGE-SUBTYPE", msg->hasField("MESSAGE-SUBTYPE"));
			test.check("Message is missing PUB-RATE", msg->hasField("PUB-RATE"));

			Message::destroy(msg);
		}
		else
		{
			test.check("Failed to receive a message", false);
		}

		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}
}


void test_SignerKey(Test& test)
{
	Config config = test.getConfig();   // make a copy!

	config.addValue("sec-policy",           "c2_nss");
	config.addValue("sec-nss-dir",          test.getDataFile("nssdb").c_str());
	config.addValue("sec-nss-key",          "x");
	config.addValue("sec-nss-sym-prefix",   "SKEY-");
	config.addValue("sec-auth-name",        "GIS1");
	config.addValue("sec-signer-key",       "GIS2");

	std::string topic = getTopic(test, config);

	StdUniquePtr<Connection> conn(0);

	try
	{
		conn.reset( new Connection(config) );

		set_standard_fields( conn->getMessageFactory() );

		conn->connect();

		GMSEC_INFO << "Subscribing to: " << topic.c_str();
		conn->subscribe( topic.c_str() );

		checkReadiness( topic.c_str() );

		Message* msg = conn->receive(10000);

		if (msg)
		{
			GMSEC_VERBOSE << "Received Message:\n" << msg->toXML();

			test.check("Message is missing MESSAGE-TYPE", msg->hasField("MESSAGE-TYPE"));
			test.check("Message is missing MESSAGE-SUBTYPE", msg->hasField("MESSAGE-SUBTYPE"));
			test.check("Message is missing PUB-RATE", msg->hasField("PUB-RATE"));

			Message::destroy(msg);
		}
		else
		{
			test.check("Failed to receive a message", false);
		}

		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}
}


void test_Unauthorized(Test& test)
{
	Config config = test.getConfig();   // make a copy!

	config.clearValue("sec-policy");

	std::string topic = getTopic(test, config);

	StdUniquePtr<Connection> conn(0);

	try
	{
		conn.reset( new Connection(config) );

		set_standard_fields( conn->getMessageFactory() );

		conn->connect();

		GMSEC_INFO << "Subscribing to: " << topic.c_str();
		conn->subscribe( topic.c_str() );

		checkReadiness( topic.c_str() );

		Message* msg = conn->receive(10000);

		if (msg)
		{
			GMSEC_VERBOSE << "Received Message:\n" << msg->toXML();

			test.check("Message is missing SEC-CONTENT", msg->hasField("SEC-CONTENT"));
			test.check("Message is missing SEC-DIGEST", msg->hasField("SEC-DIGEST"));
			test.check("Message has unexpected SEC-ENCRYPT", msg->hasField("SEC-ENCRYPT") && msg->getBooleanValue("SEC-ENCRYPT"));
			test.check("Message has unexpected SEC-KEY-ID", msg->hasField("SEC-KEY-ID") && msg->getI32Value("SEC-KEY-ID") == 23);
			test.check("Message has unexpected SEC-SIGNER", msg->hasField("SEC-SIGNER") && strcompare(msg->getStringValue("SEC-SIGNER"), "GIS1") == 0);

			Message::destroy(msg);
		}
		else
		{
			test.check("Failed to receive a message", false);
		}

		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}
}


void getConfigValues(const Config& config, std::vector<std::string>& values)
{
	const char* name;
	const char* value;

	bool hasNext = config.getFirst(name, value);

	while (hasNext)
	{
		if (strcompare(name, "-signal") != 0)
		{
			values.push_back(std::string(name) + "=" + std::string(value));
		}
		hasNext = config.getNext(name, value);
	}
}


void startPublisherProcess(Test& test)
{
	std::vector<std::string> values;
	getConfigValues(test.getConfig(), values);

	std::string pub_exe = test.getDirectory() + "/T100_C2NSS_Publisher.exe";

	GMSEC_INFO << "Launching process " << pub_exe.c_str();

#ifdef WIN32

	STARTUPINFO si_pub;
	PROCESS_INFORMATION pi_pub;

	memset(&si_pub, 0, sizeof(si_pub));
	memset(&pi_pub, 0, sizeof(pi_pub));

	std::string pub_cmd = pub_exe;

	for (size_t i = 0; i < values.size(); ++i)
	{
		pub_cmd += " " + values[i];
	}

	char* pub_cmd_str = strduplicate(pub_cmd);

	if (!CreateProcess(NULL, pub_cmd_str, NULL, NULL, FALSE, 0, NULL, NULL, &si_pub, &pi_pub))
	{
		GMSEC_ERROR << "Failed to run " << pub_exe.c_str();
	}

#else

	if (fork() == 0)
	{
		// Running as child process...
		char** argv = new char*[values.size() + 2];

		argv[0] = strduplicate(pub_exe);

		for (size_t i = 0; i < values.size(); ++i)
		{
			argv[i+1] = strduplicate(values[i]);
		}
		argv[values.size() + 1] = NULL;

		execv(argv[0], argv);

		// This point should never be reached.
		GMSEC_ERROR << "Failed to run " << pub_exe.c_str();
	}

#endif
}


bool policyExists(Test& test)
{
	bool exists = true;

	Config config = test.getConfig();   // make a copy!
	config.addValue("sec-policy",           "c2_nss");
	config.addValue("sec-nss-dir",          test.getDataFile("nssdb").c_str());
	config.addValue("sec-nss-key",          "x");
	config.addValue("sec-nss-sym-prefix",   "SKEY-");
	config.addValue("sec-auth-name",        "GIS1");
	config.addValue("sec-signer-key",       "GIS2");

	StdUniquePtr<Connection> conn(0);
	try {
		conn.reset( new Connection(config) );
		conn->connect();
		conn->disconnect();
	}
	catch (...) {
		exists = false;
	}

	return exists;
}


int test_C2NSS_Subscriber(Test& test)
{
    test.setDescription("Test C2-NSS");

	std::string mwID = test.getConfig().getValue("mw-id", "unknown");

	if (mwID == "unknown")
	{
		test.excuse("Test is being excused because no mw-id was provided");
		return -1;
	}
	else if (mwID == "mb")
	{
		test.excuse("Test is being excused because MessageBus is not supported");
		return -1;
	}
	else if (!policyExists(test))
	{
		test.excuse("Test is being excused because C2-NSS policy is not supported");
		return -1;
	}

	startPublisherProcess(test);

	test_ValDigest(test);
	test_SignerKey(test);
	test_Unauthorized(test);

    return 0;
}

TEST_DRIVER(test_C2NSS_Subscriber)
