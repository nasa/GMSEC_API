#include "TestCase.h"


int test_TimeUtil()
{
    testSetDescription("Time Utilities");

	//o Test timeUtilMillisleep()
	{
		GMSEC_TimeSpec time0 = timeUtilGetCurrentTime();

		timeUtilMillisleep(1);

		GMSEC_TimeSpec time1 = timeUtilGetCurrentTime();

		testCheckBool("Expected time1 to be greater than time 0", timeUtilGreaterThan(time1, time0) == GMSEC_TRUE);
	}

	//o Test timeUtilFormatTime()
	{
		//o Some arbitrary point in time
		GMSEC_TimeSpec ts = { 1641318286, 217233000 };
		char buffer[ GMSEC_TIME_BUFSIZE + 10 ] = {0};

		//o Test with default subs
		timeUtilFormatTime(ts, buffer);
		testCheckBool("1: Unexpected time", strcompare(buffer, "2022-004-17:44:46.217") == 0);

		//o Test with subs == 0
		timeUtilFormatTimeSubs(ts, 0, buffer);
		testCheckBool("2: Unexpected time", strcompare(buffer, "2022-004-17:44:46") == 0);

		//o Test with subs == 3
		timeUtilFormatTimeSubs(ts, 3, buffer);
		testCheckBool("3: Unexpected time", strcompare(buffer, "2022-004-17:44:46.217") == 0);

		//o Test with subs > 3
		timeUtilFormatTimeSubs(ts, 6, buffer);
		testCheckBool("4: Unexpected time", strcompare(buffer, "2022-004-17:44:46.217233") == 0);

		//o Test with subs < 0
		timeUtilFormatTimeSubs(ts, -1, buffer);
		testCheckBool("5: Unexpected time", strcompare(buffer, "2022-004-17:44:46.217") == 0);

		//o Test with subs > 9
		timeUtilFormatTimeSubs(ts, 10, buffer);
		testCheckBool("6: Unexpected time", strcompare(buffer, "2022-004-17:44:46.217") == 0);
	}

	//o Test timeUtilConvertTimeString()
	{
		GMSEC_Status status = statusCreate();

		GMSEC_TimeSpec ts = timeUtilConvertTimeString("2022-007-01:26:24.875", status);
		testCheckBool("Unexpected error converting time string", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected seconds", ts.seconds == 1641518784);
		testCheckBool("Unexpected nanoseconds", ts.nanoseconds == 875000000);

		(void) timeUtilConvertTimeString("This is an invalid timestamp", status);
		testCheckBool("Expected error converting time string", statusHasError(status) == GMSEC_TRUE);

		statusDestroy(&status);
	}

	//o Test timeUtilConvertTimeString_s()
	{
		GMSEC_Status   status = statusCreate();
		GMSEC_TimeSpec ts;
		double         time0  = timeUtilGetCurrentTime_s(&ts);
		char           buffer[ GMSEC_TIME_BUFSIZE ];

		timeUtilFormatTimeSubs(ts, 9, buffer);

		double time1 = timeUtilConvertTimeString_s(buffer, status);
		testCheckBool("Unexpected error converting time string", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Unexpected time conversion", time0 == time1);

		(void) timeUtilConvertTimeString_s("This is an invalid timestamp", status);
		testCheckBool("Expected error converting time string", statusHasError(status) == GMSEC_TRUE);

		statusDestroy(&status);
	}

	//o Test timeUtilDiffTime()
	{
		//o Some arbitrary points in time
		GMSEC_TimeSpec time0 = { 1641318286, 217233000 };
		GMSEC_TimeSpec time1 = { 1641318287, 217234000 };

		GMSEC_TimeSpec diff1 = timeUtilDiffTime(time1, time0);
		GMSEC_TimeSpec diff2 = timeUtilDiffTime(time0, time1);

		testCheckBool("1: Expected a delta of 1 second", diff1.seconds == 1);
		testCheckBool("1: Expected a delta of 1000 nanoseconds", diff1.nanoseconds == 1000);

		testCheckBool("2: Expected a delta of -1 second", diff2.seconds == -2);
		testCheckBool("2: Expected a delta of -1000 nanoseconds", diff2.nanoseconds == 999999000);
	}

	//o Test timeUtil comparator functions
	{
		//o Some arbitrary points in time
		GMSEC_TimeSpec time0 = { 1641318286, 217233000 };
		GMSEC_TimeSpec time1 = { 1641318286, 217233000 };
		GMSEC_TimeSpec time2 = { 1641318287, 217234000 };

		testCheckBool("Expected time0 to be the same as time1",  timeUtilEqualTo(time0, time1) == GMSEC_TRUE);
		testCheckBool("Expected time1 to be the same as time0",  timeUtilEqualTo(time1, time0) == GMSEC_TRUE);
		testCheckBool("Expected time0 to be less than time2",    timeUtilLesserThan(time0, time1) == GMSEC_FALSE);
		testCheckBool("Expected time0 to be less than time2",    timeUtilLesserThan(time1, time0) == GMSEC_FALSE);
		testCheckBool("Expected time0 to be less than time2",    timeUtilLesserThan(time0, time2) == GMSEC_TRUE);
		testCheckBool("Expected time0 to be less than time2",    timeUtilGreaterThan(time0, time1) == GMSEC_FALSE);
		testCheckBool("Expected time0 to be less than time2",    timeUtilGreaterThan(time1, time0) == GMSEC_FALSE);
		testCheckBool("Expected time2 to be greater than time1", timeUtilGreaterThan(time2, time1) == GMSEC_TRUE);
	}

    return 0;
}

TEST_DRIVER(test_TimeUtil)
