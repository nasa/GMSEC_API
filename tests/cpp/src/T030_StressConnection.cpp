#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


int test_StressConnection(Test& test)
{
    test.setDescription("Stress Connection");

	Log::setReportingLevel( LogLevel::logNONE );

	StdUniquePtr<Connection> conn;

	try
	{
		for (int i = 0; i < 10; ++i)
		{
			// Yes, do it twice... why??
			conn.reset( new Connection(test.getConfig()) );
			conn.reset( new Connection(test.getConfig()) );

			for (int j = 0; j < 10; ++j)
			{
				conn->connect();
				conn->disconnect();
			}

			conn.reset();

			test.check("All good", true);
		}
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	Log::setReportingLevel( LogLevel::logINFO );

    return 0;
}

TEST_DRIVER(test_StressConnection)
