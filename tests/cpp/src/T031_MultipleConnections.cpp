#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


int test_MultipleConnections(Test& test)
{
    test.setDescription("Multiple Connections");

	Log::setReportingLevel( LogLevel::logNONE );

	Connection* conn[100] = { NULL };

	try
	{
		for (int i = 0; i < 100; ++i)
		{
			conn[i] = new Connection( test.getConfig() );

			conn[i]->connect();
		}

		for (int i = 0; i < 100; ++i)
		{
			conn[i]->disconnect();

			delete conn[i];

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

TEST_DRIVER(test_MultipleConnections)
