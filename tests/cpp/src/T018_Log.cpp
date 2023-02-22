#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


class CXX_LogHandler : public LogHandler
{
public:
	CXX_LogHandler() : m_called(false) {}

	void CALL_TYPE onMessage(const LogEntry& entry)
	{
		m_called = true;
	}

	bool wasCalled() const
	{
		return m_called;
	}
private:
	bool m_called;
};


GMSEC_BOOL g_called = GMSEC_BOOL::GMSEC_FALSE;

void C_LogHandler(const GMSEC_LogEntry* entry)
{
	g_called = GMSEC_BOOL::GMSEC_TRUE;
}


int test_Log(Test& test)
{
    test.setDescription("Log");

	//o getReportingLevel() and setReportingLevel()
	{
		LogLevel level = Log::getReportingLevel();

		Log::setReportingLevel(LogLevel::logDEBUG);
		test.check("Expected DEBUG log level", Log::getReportingLevel() == LogLevel::logDEBUG);

		// Restore logging level
		Log::setReportingLevel(level);
		test.check("Expected INFO log level", Log::getReportingLevel() == LogLevel::logINFO);
	}

	//o C++ Log Handler
	{
		CXX_LogHandler cxxLogHandler;
		Log::registerHandler(LogLevel::logVERBOSE, &cxxLogHandler);
		LogLevel level = Log::getReportingLevel();
		Log::setReportingLevel(LogLevel::logVERBOSE);
		GMSEC_VERBOSE << "This is a test";
		Log::setReportingLevel(level);
		Log::registerHandler(reinterpret_cast<LogHandler*>(NULL));

		test.check("CXX Log Handler was not called", cxxLogHandler.wasCalled());
	}

	//o C Log Handler
	{
		Log::registerHandler(LogLevel::logVERBOSE, &C_LogHandler);
		LogLevel level = Log::getReportingLevel();
		Log::setReportingLevel(LogLevel::logVERBOSE);
		GMSEC_VERBOSE << "This is a test";
		Log::setReportingLevel(level);
		Log::registerHandler(reinterpret_cast<GMSEC_LogHandler*>(NULL));

		test.check("C Log Handler was not called", g_called == GMSEC_BOOL::GMSEC_TRUE);
	}

	//o toString() and fromString()
	{
		test.check("Expected WARNING level", strcompare(Log::toString(LogLevel::logWARNING), "WARNING") == 0);

		test.check("Expected WARNING level", Log::fromString("WARNING") == LogLevel::logWARNING);
	}

	//o Test LogStream
	{
		LogStream ls;

		ls << static_cast<char>(49)
		   << static_cast<int>(1)
		   << static_cast<unsigned int>(1)
		   << static_cast<long>(1)
		   << static_cast<unsigned long>(1)
		   << static_cast<long long>(1)
		   << static_cast<unsigned long long>(1)
		   << static_cast<float>(1)
		   << static_cast<double>(1)
		   << reinterpret_cast<void*>(0x1111111111111111)
		   << ": Lots of ones!";

#ifdef WIN32
		test.check("Expected the jibberish noted above", strcompare(ls.getText(), "1111111111111111111111111: Lots of ones!") == 0);
#else
		test.check("Expected the jibberish noted above", strcompare(ls.getText(), "1111111110x1111111111111111: Lots of ones!") == 0);
#endif
	}

    return 0;
}

TEST_DRIVER(test_Log)
