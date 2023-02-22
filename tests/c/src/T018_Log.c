#include "TestCase.h"


GMSEC_BOOL g_called = GMSEC_FALSE;

void C_LogHandler(const GMSEC_LogEntry* entry)
{
	g_called = GMSEC_TRUE;
}

void C_LogHandlerReset()
{
	g_called = GMSEC_FALSE;
}


int test_Log()
{
    testSetDescription("Time Log");

	//o logGetReportingLevel() and logSetReportingLevel()
	{
		GMSEC_LogLevel level = logGetReportingLevel();

		logSetReportingLevel(logDEBUG);
		testCheckBool("Expected DEBUG log level", logGetReportingLevel() == logDEBUG);

		// Restore logging level
		logSetReportingLevel(level);
		testCheckBool("Expected INFO log level", logGetReportingLevel() == logINFO);
	}

	//o logRegisterHandler()
	{
		logRegisterHandler(&C_LogHandler);

		GMSEC_WARNING("This is a test");
		testCheckBool("C Log Handler was not called", g_called == GMSEC_TRUE);

		// Restore default log handler
		logRegisterHandler(NULL);

		C_LogHandlerReset();
	}

	//o logRegisterHandlerByLevel()
	{
		GMSEC_LogLevel level = logGetReportingLevel();
		logSetReportingLevel(logVERBOSE);
		logRegisterHandlerByLevel(logVERBOSE, &C_LogHandler);

		GMSEC_VERBOSE("This is a test");
		testCheckBool("C Log Handler was not called", g_called == GMSEC_TRUE);

		// Restore default log handler and log level
		logRegisterHandler(NULL);
		logSetReportingLevel(level);

		C_LogHandlerReset();
	}

	//o logLevelToString() and logLevelFromString()
	{
		testCheckBool("Expected WARNING level", strcompare(logLevelToString(logWARNING), "WARNING") == 0);

		testCheckBool("Expected WARNING level", logLevelFromString("WARNING") == logWARNING);
	}

    return 0;
}

TEST_DRIVER(test_Log)
