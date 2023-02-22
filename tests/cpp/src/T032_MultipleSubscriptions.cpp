#include "TestCase.h"

#include <string>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


class MyCallback : public Callback
{
public:
	MyCallback(const std::string& topic1, const std::string& topic2, const std::string& topic3)
		: topic1(topic1), topic2(topic2), topic3(topic3), rcvdCount1(0), rcvdCount2(0), rcvdCount3(0), errCount(0)
	{
	}

	void CALL_TYPE onMessage(Connection& conn, const Message& msg)
	{
		GMSEC_INFO << "Received message: " << msg.getSubject();

		if (topic1 == msg.getSubject())
		{
			++rcvdCount1;
		}
		else if (topic2 == msg.getSubject())
		{
			++rcvdCount2;
		}
		else if (topic3 == msg.getSubject())
		{
			++rcvdCount3;
		}
		else
		{
			++errCount;
		}
	}

	int getRcvdCount(int num) const
	{
		switch (num)
		{
		case 1: return rcvdCount1;
		case 2: return rcvdCount2;
		case 3: return rcvdCount3;
		}
		return 0;
	}

	int getErrCount() const
	{
		return errCount;
	}

private:
	std::string topic1;
	std::string topic2;
	std::string topic3;
	int         rcvdCount1;
	int         rcvdCount2;
	int         rcvdCount3;
	int         errCount;
};


int test_MultipleSubscriptions(Test& test)
{
    test.setDescription("Multiple Subscriptions");

	std::string topic1 = test.getSubject("1");
	std::string topic2 = test.getSubject("2");
	std::string topic3 = test.getSubject("3");

	MyCallback cb(topic1, topic2, topic3);

	int iterations = 2;

	StdUniquePtr<Connection> conn;

	try
	{
		conn.reset( new Connection(test.getConfig()) );

		conn->connect();

		conn->subscribe( topic1.c_str(), &cb );
		conn->subscribe( topic2.c_str(), &cb );
		conn->subscribe( topic3.c_str(), &cb );

		conn->startAutoDispatch();

		TimeUtil::millisleep( 2000 );   // give time for the auto-dispatcher to start

		Message msg1, msg2, msg3;
		msg1.setSubject( topic1.c_str() );
		msg2.setSubject( topic2.c_str() );
		msg3.setSubject( topic3.c_str() );

		for (int i = 0; i < iterations; ++i)
		{
			TimeUtil::millisleep( 5 );  // Add delay to mimic adding fields to the msg

			conn->publish( msg1 );

			TimeUtil::millisleep( 5 );  // Add delay to mimic adding fields to the msg

			conn->publish( msg2 );

			TimeUtil::millisleep( 5 );  // Add delay to mimic adding fields to the msg

			conn->publish( msg3 );

			TimeUtil::millisleep( 1000 );
		}

		conn->stopAutoDispatch();

		conn->disconnect();

		test.check("Unexpected topic1 recv count", cb.getRcvdCount(1) == iterations);
		test.check("Unexpected topic2 recv count", cb.getRcvdCount(2) == iterations);
		test.check("Unexpected topic3 recv count", cb.getRcvdCount(3) == iterations);
		test.check("Unexpected error count", cb.getErrCount() == 0);
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}

	return 0;
}

TEST_DRIVER(test_MultipleSubscriptions)
