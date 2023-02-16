/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/util/InternalLog.h>

#include <gmsec5/internal/Rawbuf.h>
#include <gmsec5/internal/StringUtil.h>

#include <gmsec5/util/Log.h>
#include <gmsec5/util/LoggerStream.h>
#include <gmsec5/util/LogStream.h>
#include <gmsec5/util/LogHandler.h>
#include <gmsec5/util/TimeUtil.h>
#include <gmsec5/util/wdllexp.h>

#include <iostream>
#include <cstring>


namespace gmsec
{
namespace api5
{
namespace internal
{

/*
 * @brief This class represents the default log handler.
 */
class GMSEC_API DefaultHandler : public gmsec::api5::util::LogHandler
{
public:
	DefaultHandler(std::ostream* os = &std::cerr);

	void setStream(std::ostream* os);

	virtual void CALL_TYPE onMessage(const gmsec::api5::util::LogEntry& entry);

private:
	std::ostream* out;
};


/*
 * @brief This class connects the logging framework to the C API.
 */
class GMSEC_API CAPILogHandler : public gmsec::api5::util::LogHandler
{
public:
	CAPILogHandler(GMSEC_LogHandler* handler);

    virtual void CALL_TYPE onMessage(const gmsec::api5::util::LogEntry& entry);

private:
    GMSEC_LogHandler* fHandler;
};
}
}
}


using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;

static gmsec::api5::util::LogHandler* cxxHandlers[ static_cast<int>(LogLevel::logNLEVEL) ] = {0};
static gmsec::api5::util::LogHandler* ceeHandlers[ static_cast<int>(LogLevel::logNLEVEL) ] = {0};


// Initializes static variables for the Log class
// Set level to default (warning).
//
LogLevel InternalLog::s_maxLevel = LogLevel::logWARNING;

static gmsec::api5::internal::DefaultHandler s_defaultHandler(&std::cerr);


void InternalLog::setReportingLevel(LogLevel level)
{
	InternalLog::s_maxLevel = level;
}


void InternalLog::registerHandler(LogHandler* handler)
{
	for (int level = static_cast<int>(LogLevel::logERROR); level < static_cast<int>(LogLevel::logNLEVEL); ++level)
	{
		registerHandler(static_cast<LogLevel>(level), handler);
	}
}


void InternalLog::registerHandler(LogLevel level, LogHandler* handler)
{
	// If the callee attempts to register a NULL handler, then we will substitute the default handler in its place.
	if (handler)
	{
		GMSEC_DEBUG << "Registering custom handler " << (const void*) handler << " for level " << Log::toString(level);
		cxxHandlers[ static_cast<int>(level) ] = handler;
	}
	else
	{
		GMSEC_DEBUG << "Registering default handler for level " << Log::toString(level);
		LogHandler& logHandler = getDefaultHandler();
		cxxHandlers[ static_cast<int>(level) ] = &logHandler;
	}
}


void InternalLog::registerHandler(GMSEC_LogHandler* handler)
{
	for (int level = static_cast<int>(LogLevel::logERROR); level < static_cast<int>(LogLevel::logNLEVEL); ++level)
	{
		registerHandler(static_cast<LogLevel>(level), handler);
	}
}


void InternalLog::registerHandler(GMSEC_LogLevel level, GMSEC_LogHandler* handler)
{
	CAPILogHandler* ceeLogHandler = dynamic_cast<CAPILogHandler*>(ceeHandlers[ static_cast<int>(level) ]);

	if (ceeLogHandler)
	{
		delete ceeLogHandler;
	}

	if (handler == NULL || *handler == NULL)
	{
		GMSEC_DEBUG << "Registering default handler for level " << Log::toString(level);
		LogHandler& logHandler = getDefaultHandler();
		ceeHandlers[ static_cast<int>(level) ] = &logHandler;
	}
	else
	{
		GMSEC_DEBUG << "Registering custom handler " << (const void*) handler << " for level " << Log::toString(level);
		ceeHandlers[ static_cast<int>(level) ] = new CAPILogHandler(handler);
	}
}


LogLevel InternalLog::fromString(const char* level)
{
	using namespace gmsec::util;

	if (level)
	{
		if (StringUtil::stringEqualsIgnoreCase(level, "DEBUG"))
			return LogLevel::logDEBUG;
		if (StringUtil::stringEqualsIgnoreCase(level, "VERBOSE"))
			return LogLevel::logVERBOSE;
		if (StringUtil::stringEqualsIgnoreCase(level, "INFO"))
			return LogLevel::logINFO;
		if (StringUtil::stringEqualsIgnoreCase(level, "WARNING"))
			return LogLevel::logWARNING;
		if (StringUtil::stringEqualsIgnoreCase(level, "SECURE"))
			return LogLevel::logSECURE;
		if (StringUtil::stringEqualsIgnoreCase(level, "ERROR"))
			return LogLevel::logERROR;
		if (StringUtil::stringEqualsIgnoreCase(level, "NONE"))
			return LogLevel::logNONE;

		LoggerStream().get(LogLevel::logWARNING) << "Unknown logging level '" << level << "'. Returning INFO level as default.";
	}
	else
	{
		LoggerStream().get(LogLevel::logWARNING) << "Logging level is NULL. Returning INFO level as default.";
	}

	return LogLevel::logINFO;
}


const char* InternalLog::toString(LogLevel level)
{
	static const char* buffer[] = {
		"NONE",
		"ERROR",
		"SECURE",
		"WARNING",
		"INFO",
		"VERBOSE",
		"DEBUG"
	};

	if (static_cast<int>(level) >= 0 && static_cast<int>(level) < static_cast<int>(LogLevel::logNLEVEL))
		return buffer[ static_cast<int>(level) ];

	return "INVALID";
}


void InternalLog::setDefaultStream(std::ostream* s)
{
	getDefaultHandler().setStream(s);
}


std::string InternalLog::prepareLogMessage(const LogEntry& entry)
{
	char timeBuffer[GMSEC_TIME_BUFSIZE];
	TimeUtil::formatTime(entry.time, timeBuffer);

	std::string file = (entry.file ? entry.file : "unknown");
	std::string msg  = (entry.message ? entry.message : "");

#ifdef WIN32
	file = file.substr(file.rfind("\\") + 1);
#else
	file = file.substr(file.rfind("/") + 1);
#endif

	// The constant 128 has been arbitrarily chosen; it should (hopefully) be sufficient to
	// store the ancillary data associated with a log message that has not already been
	// accounted for using the sizes of known strings or buffers.
	const size_t bufSize = sizeof(timeBuffer) + file.length() + msg.length() + 128;

	char* buffer = new char[bufSize];

	rawbuf x(buffer, bufSize);

	std::ostream os(&x);

	size_t pos = msg.find("\n");
	while (pos != std::string::npos)
	{
		msg.replace(pos, 1, "\n\t");

		pos = msg.find("\n", pos+2);
	}

	pos = msg.find("\r");
	while (pos != std::string::npos)
	{
		msg.replace(pos, 1, "\n\t");

		pos = msg.find("\r", pos+2);
	}

	os << timeBuffer
	   << " [" << Log::toString(entry.level) << "]"
	   << " [" << file << ":" << entry.line << "] "
	   << msg << "\n" << std::ends;

	std::string logMsg = buffer;

	delete [] buffer;

	return logMsg;
}


DefaultHandler& getDefaultHandler()
{
	return s_defaultHandler;
}


LogHandler* getHandler(LogLevel level)
{
	if (ceeHandlers[ static_cast<int>(level) ] != NULL)
	{
		return ceeHandlers[ static_cast<int>(level) ];
	}
	return cxxHandlers[ static_cast<int>(level) ];
}


DefaultHandler::DefaultHandler(std::ostream* os)
	: out(os)
{
	Log::registerHandler(this);
}


void DefaultHandler::setStream(std::ostream* ostr)
{
	if (out != &std::cout && out != &std::cerr)
	{
		delete out;
	}
	out = ostr;
}


void DefaultHandler::onMessage(const LogEntry& entry)
{
	if (!out)
	{
		return;
	}

	(*out) << InternalLog::prepareLogMessage(entry) << std::endl;
}


CAPILogHandler::CAPILogHandler(GMSEC_LogHandler* handler)
{
	fHandler = handler;
}

void CAPILogHandler::onMessage(const LogEntry& entry)
{
	GMSEC_LogEntry ceeEntry;

	ceeEntry.file    = entry.file;
	ceeEntry.line    = entry.line;
	ceeEntry.level   = entry.level;
	ceeEntry.time    = entry.time;
	ceeEntry.message = entry.message;

	if (NULL != fHandler)
	{
		(*fHandler)(&ceeEntry);
	}
}
