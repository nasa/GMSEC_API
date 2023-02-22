#include "TestCase.h"

#include <cstring>
#include <iostream>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


class AsyncStatusCheckCallback : public EventCallback
{
private:
	Test& test;
	bool invoked;

public:
	AsyncStatusCheckCallback(Test& t)
		: test(t),
		  invoked(false)
	{
	}

	void CALL_TYPE onEvent(Connection& conn, const Status& status, Connection::Event event)
	{
		invoked = true;

		if (status.hasError())
		{
			std::ostringstream strm;
			strm << "There was an error or warning associated with publishing the message, Reason Code: " << status.getCustomCode();
			test.check(strm.str().c_str(), false);
		}
		else
		{
			test.check("AsyncStatusCheckCallback: unexpected event", event == Connection::Event::WSMQ_ASYNC_STATUS_CHECK_EVENT);
		}
	}

	bool wasInvoked() const
	{
		return invoked;
	}
};


/*
 * Test Case #1: Enable Asynchronous Publishing, set up an AsyncStatusCheckEvent Callback, tell the Connection to
 * proc the event after one message is published, then publish a message and check to see if the callback was invoked
 * and if the message was published successfully.
 */
int test_AsyncPublish(Test& test)
{
	test.setDescription("Asynchronous Publish");

	const char* mw = test.getConfig().getValue("mw-id");

	if (!mw)
	{
		test.excuse("Unable to deduce middleware type.");
		return -1;
	}

	if (!strcontains(mw, "ibmmq"))
	{
		test.excuse("Test is being excused for all middlewares except IBM MQ");
		return 0;
	}

	Config cfg = test.getConfig();
	cfg.addValue("MW-ASYNC-PUBLISH", "true");
	cfg.addValue("MW-ASYNC-STATUS-CHECK", "true");
	cfg.addValue("MW-ASYNC-STATUS-CHECK-MESSAGE-INTERVAL", "1");

	StdUniquePtr<Connection> conn;

	try
	{
		conn.reset( new Connection(cfg) );

		//o Register the AsyncStatusCheckCallback
		AsyncStatusCheckCallback asccb(test);
		conn->registerEventCallback(Connection::Event::WSMQ_ASYNC_STATUS_CHECK_EVENT, &asccb);

		conn->connect();

		Message msg;
		msg.setSubject( test.getSubject().c_str() );

		conn->publish(msg);

		// This check serves two purposes: That the Publish call succeeded and that the Callback was invoked.
		// Check to see if the AsyncStatusCheckCallback was invoked
		test.check("Callback not invoked", asccb.wasInvoked());

		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}

	return 0;
}

TEST_DRIVER(test_AsyncPublish)
