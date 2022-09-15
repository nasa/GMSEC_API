/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/util/InternalLog.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/LoggerStream.h>
#include <gmsec4/util/LogStream.h>
#include <gmsec4/util/LogHandler.h>
#include <gmsec4/util/TimeUtil.h>

#include <gmsec4/internal/Rawbuf.h>
#include <gmsec4/internal/StringUtil.h>

#include <iostream>
#include <cstring>


namespace gmsec
{
namespace api
{
namespace internal
{
/*
 * @brief This class represents the default log handler.
 */
class GMSEC_API DefaultHandler : public gmsec::api::util::LogHandler
{
public:
	DefaultHandler(std::ostream* os = &std::cerr);

	void setStream(std::ostream* os);

	virtual void CALL_TYPE onMessage(const gmsec::api::util::LogEntry& entry);

private:
	std::ostream* out;
};


/*
 * @brief This class connects the logging framework to the C API.
 */
class GMSEC_API CAPILogHandler : public gmsec::api::util::LogHandler
{
public:
	CAPILogHandler(GMSEC_LogHandler* handler);

    virtual void CALL_TYPE onMessage(const gmsec::api::util::LogEntry& entry);

private:
    GMSEC_LogHandler* fHandler;
};
}
}
}


using namespace gmsec::api::internal;
using namespace gmsec::api::util;

static gmsec::api::util::LogHandler* cxxHandlers[logNLEVEL];
static gmsec::api::util::LogHandler* ceeHandlers[logNLEVEL];


// Initializes static variables for the Log class
// Set level to default (none).
//
LogLevel InternalLog::s_maxLevel = logNONE;

static gmsec::api::internal::DefaultHandler s_defaultHandler(&std::cerr);


void InternalLog::setReportingLevel(LogLevel level)
{
	InternalLog::s_maxLevel = level;
}


void InternalLog::registerHandler(LogHandler* handler)
{
	for (int level = logERROR; level < logNLEVEL; ++level)
	{
		registerHandler((LogLevel) level, handler);
	}
}


void InternalLog::registerHandler(LogLevel level, LogHandler* handler)
{
	GMSEC_DEBUG << "Registering handler " << (const void*) handler << " for level " << Log::toString(level);

	// If the callee attempts to register a NULL handler, then we will substitute the default handler in its place.
	if (handler)
	{
		cxxHandlers[level] = handler;
	}
	else
	{
		LogHandler& logHandler = getDefaultHandler();
		cxxHandlers[level] = &logHandler;
	}
}


void InternalLog::registerHandler(GMSEC_LogHandler* handler)
{
	for (int level = logERROR; level < logNLEVEL; ++level)
	{
		registerHandler((LogLevel) level, handler);
	}
}


void InternalLog::registerHandler(GMSEC_LogLevel level, GMSEC_LogHandler* handler)
{
	GMSEC_DEBUG << "Registering handler " << (const void*) handler << " for level " << Log::toString(level);
	delete ceeHandlers[level];
	ceeHandlers[level] = new CAPILogHandler(handler);
}


LogLevel InternalLog::fromString(const char* level)
{
	using namespace gmsec::util;

	if (level)
	{
		if (StringUtil::stringEqualsIgnoreCase(level, "DEBUG"))
			return logDEBUG;
		if (StringUtil::stringEqualsIgnoreCase(level, "VERBOSE"))
			return logVERBOSE;
		if (StringUtil::stringEqualsIgnoreCase(level, "INFO"))
			return logINFO;
		if (StringUtil::stringEqualsIgnoreCase(level, "WARNING"))
			return logWARNING;
		if (StringUtil::stringEqualsIgnoreCase(level, "SECURE"))
			return logSECURE;
		if (StringUtil::stringEqualsIgnoreCase(level, "ERROR"))
			return logERROR;
		if (StringUtil::stringEqualsIgnoreCase(level, "NONE"))
			return logNONE;

		LoggerStream().get(logWARNING) << "Unknown logging level '" << level << "'. Returning INFO level as default.";
	}
	else
	{
		LoggerStream().get(logWARNING) << "Logging level is NULL. Returning INFO level as default.";
	}

	return logINFO;
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

	if (level >= 0 && level < logNLEVEL)
		return buffer[level];

	return "INVALID";
}


void InternalLog::setDefaultStream(std::ostream* s)
{
	getDefaultHandler().setStream(s);
}


std::string InternalLog::prepareLogMessage(const LogEntry& entry)
{
	char timeBuffer[GMSEC_TIME_BUFSIZE];
	gmsec::api::util::TimeUtil::formatTime(entry.time, timeBuffer);

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


gmsec::api::internal::DefaultHandler& getDefaultHandler()
{
	return s_defaultHandler;
}


gmsec::api::util::LogHandler* getHandler(LogLevel level)
{
	if (ceeHandlers[level] != NULL)
	{
		return ceeHandlers[level];
	}
	return cxxHandlers[level];
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


void DefaultHandler::onMessage(const gmsec::api::util::LogEntry& entry)
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

void CAPILogHandler::onMessage(const gmsec::api::util::LogEntry& entry)
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
