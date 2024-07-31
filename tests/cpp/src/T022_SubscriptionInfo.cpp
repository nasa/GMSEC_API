#include "TestCase.h"

#include <gmsec5/internal/InternalSubscriptionInfo.h>

using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


class MyCallback : public Callback
{
public:
	MyCallback() {}
	virtual ~MyCallback() {}
	void onMessage(Connection& conn, const Message& msg) {}
};


void MyCeeCallback(GMSEC_Connection conn, const GMSEC_Message msg)
{
}


int test_SubscriptionInfo(Test& test)
{
	test.setDescription("InternalSubscriptionInfo");

	const std::string topic  = "GMSEC.>";
	const Config&     config = test.getConfig();
	MyCallback        cb;
	void*             cee_cb = reinterpret_cast<void*>(&MyCeeCallback);

	StdUniquePtr<Connection> conn;

	try
	{
		//o Create connection
		conn.reset(new Connection(config));

		//o Test InternalSubscriptionInfo constructor
		InternalSubscriptionInfo info( conn.get(), topic.c_str(), config, &cb );

		//o Test setter(s)
		info.setCallbackFunction(cee_cb);

		//o Test getter(s)
		test.check("Unexpected topic", topic == info.getSubjectPattern());
		test.check("Unexpected config", StringUtil::stringEquals(config.toXML(), info.getConfig().toXML()));
		test.check("Unexpected callback", &cb == info.getCallback());
		test.check("Unexpected connection", conn.get() == info.getConnection());
		test.check("Unexpected C callback", cee_cb == info.getCallbackFunction());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	return 0;
}

TEST_DRIVER(test_SubscriptionInfo)
