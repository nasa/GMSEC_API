#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


int test_TimeUtil(Test& test)
{
    test.setDescription("Time Utilities");

	//o Test TimeUtil::millisleep()
	{
		GMSEC_TimeSpec time0 = TimeUtil::getCurrentTime();

		TimeUtil::millisleep(0);

		GMSEC_TimeSpec time1 = TimeUtil::getCurrentTime();

		test.check("Expected time1 to be greater than time 0", time1 > time0);
	}

	//o Test TimeUtil::formatTime()
	{
		//o Some arbitrary point in time
		GMSEC_TimeSpec ts = { 1641318286, 217233000 };
		char buffer[ GMSEC_TIME_BUFSIZE + 10 ] = {0};

		//o Test with subs == 0
		TimeUtil::formatTime(ts, 0, buffer);
		test.check("1: Unexpected time", strcompare(buffer, "2022-004-17:44:46") == 0);

		//o Test with subs == 3
		TimeUtil::formatTime(ts, 3, buffer);
		test.check("2: Unexpected time", strcompare(buffer, "2022-004-17:44:46.217") == 0);

		//o Test with subs > 3
		TimeUtil::formatTime(ts, 6, buffer);
		test.check("3: Unexpected time", strcompare(buffer, "2022-004-17:44:46.217233") == 0);

		//o Test with subs < 0
		TimeUtil::formatTime(ts, -1, buffer);
		test.check("4: Unexpected time", strcompare(buffer, "2022-004-17:44:46.217") == 0);

		//o Test with subs > 9
		TimeUtil::formatTime(ts, 10, buffer);
		test.check("5: Unexpected time", strcompare(buffer, "2022-004-17:44:46.217") == 0);
	}

	//o Test TimeUtil::convertTimeString()
	{
		GMSEC_TimeSpec ts = TimeUtil::convertTimeString("2022-007-01:26:24.875");
		test.check("Unexpected seconds", ts.seconds == 1641518784);
		test.check("Unexpected nanoseconds", ts.nanoseconds == 875000000);

		try {
			(void) TimeUtil::convertTimeString("This is an invalid timestamp");
			test.check("Expected a time conversion error", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), true);
		}
	}

	//o Test TimeUtil::convertTimeString_s()
	{
		GMSEC_TimeSpec ts;
		double         time0  = TimeUtil::getCurrentTime_s(&ts);
		char           buffer[ GMSEC_TIME_BUFSIZE ];

		TimeUtil::formatTime(ts, 9, buffer);

		double time1 = TimeUtil::convertTimeString_s(buffer);
		test.check("Unexpected time conversion", time0 == time1);

		try {
			(void) TimeUtil::convertTimeString_s("This is an invalid timestamp");
			test.check("Expected an exception", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), true);
		}
	}

	//o Test TimeUtil::diffTime()
	{
		//o Some arbitrary points in time
		GMSEC_TimeSpec time0 = { 1641318286, 217233000 };
		GMSEC_TimeSpec time1 = { 1641318287, 217234000 };

		GMSEC_TimeSpec diff1 = TimeUtil::diffTime(time1, time0);
		GMSEC_TimeSpec diff2 = TimeUtil::diffTime(time0, time1);

		test.check("1: Expected a delta of 1 second", diff1.seconds == 1);
		test.check("1: Expected a delta of 1000 nanoseconds", diff1.nanoseconds == 1000);

		test.check("2: Expected a delta of -1 second", diff2.seconds == -2);
		test.check("2: Expected a delta of -1000 nanoseconds", diff2.nanoseconds == 999999000);
	}

	//o Test TimeUtil::operatorX()
	{
		//o Some arbitrary points in time
		GMSEC_TimeSpec time0 = { 1641318286, 217233000 };
		GMSEC_TimeSpec time1 = { 1641318286, 217233000 };
		GMSEC_TimeSpec time2 = { 1641318287, 217234000 };

		test.check("Expected time0 to be the same as time1",  (time0 == time1) == true);
		test.check("Expected time1 to be the same as time0",  (time1 == time0) == true);
		test.check("Expected time0 to be less than time2",    (time0 <  time1) == false);
		test.check("Expected time0 to be less than time2",    (time1 <  time0) == false);
		test.check("Expected time0 to be less than time2",    (time0 <  time2) == true);
		test.check("Expected time0 to be less than time2",    (time0 >  time1) == false);
		test.check("Expected time0 to be less than time2",    (time1 >  time0) == false);
		test.check("Expected time2 to be greater than time1", (time2 >  time1) == true);
	}

    return 0;
}

TEST_DRIVER(test_TimeUtil)
