#include "TestCase.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <ctype.h>


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


void signalReadiness(const char* cookie)
{
	std::fstream file(cookie, std::fstream::out);
}


void test_ValDigest(Test& test)
{
	Config config = test.getConfig();   // make a copy!

	config.addValue("loglevel",             "error");

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

		Message msg = conn->getMessageFactory().createMessage("HB");
		msg.setSubject( topic.c_str() );
		msg.setFieldValue("PUB-RATE", "1");

		signalReadiness( topic.c_str() );

		TimeUtil::millisleep(5000);

		conn->publish(msg);

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

	config.addValue("loglevel",             "error");

	config.addValue("sec-policy",           "c2_nss");
	config.addValue("sec-nss-dir",          test.getDataFile("nssdb").c_str());
	config.addValue("sec-nss-key",          "x");
	config.addValue("sec-encrypt",          "true");
	config.addValue("sec-key-id",           "23");
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

		Message msg = conn->getMessageFactory().createMessage("HB");
		msg.setSubject( topic.c_str() );
		msg.setFieldValue("PUB-RATE", "1");

		signalReadiness( topic.c_str() );

		TimeUtil::millisleep(5000);

		conn->publish(msg);

		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}
}


int test_C2NSS_Publisher(Test& test)
{
    test.setDescription("Test C2-NSS");

	test_ValDigest(test);
	test_SignerKey(test);
	test_SignerKey(test);  // yes, this is intentional

    return 0;
}

TEST_DRIVER(test_C2NSS_Publisher)

