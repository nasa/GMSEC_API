/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec/util/timeutil.h>

#include <gmsec/internal/Log.h>
#include <gmsec/internal/strutil.h>
#include <gmsec/internal/rawbuf.h>

#include <gmsec4/internal/util/InternalLog.h>

static gmsec::api::util::LogHandler* cxxAdapterHandlers[logNLEVEL] = {NULL};
static gmsec::api::util::LogHandler* ceeAdapterHandlers[logNLEVEL] = {NULL};


namespace gmsec
{
namespace util
{

/* Initializes static variables */
// Default logging level
LogLevel Log::fMaxLevel = logNONE;


// Provides interface between API 3.x LogHandler and an API 4.x LogHandler
class GMSEC_API API_4x_LogHandlerAdapter : public gmsec::api::util::LogHandler
{
public:
	API_4x_LogHandlerAdapter(gmsec::util::LogHandler* handler)
		: m_handler(handler)
	{
	}

	virtual void CALL_TYPE onMessage(const gmsec::api::util::LogEntry& entry)
	{
		if (m_handler)
		{
			GMSEC_LOG_ENTRY legacyEntry;

			legacyEntry.file    = entry.file;
			legacyEntry.line    = entry.line;
			legacyEntry.level   = entry.level;
			legacyEntry.time    = double(entry.time.seconds) + double(entry.time.nanoseconds) / 1E9;
			legacyEntry.message = entry.message;

			m_handler->OnMessage(legacyEntry);
		}
	}

private:
	gmsec::util::LogHandler* m_handler;
};


// Provides interface between API 3.x LogHandler and an API 4.x LogHandler
class GMSEC_API CAPI_4x_LogHandlerAdapter : public gmsec::api::util::LogHandler
{
public:
	CAPI_4x_LogHandlerAdapter(GMSEC_LOGGER_HANDLER* handler)
		: m_handler(handler)
	{
	}

	virtual void CALL_TYPE onMessage(const gmsec::api::util::LogEntry& entry)
	{
		if (m_handler)
		{
			GMSEC_LOG_ENTRY legacyEntry;

			legacyEntry.file    = entry.file;
			legacyEntry.line    = entry.line;
			legacyEntry.level   = entry.level;
			legacyEntry.time    = (double)(entry.time.seconds + (entry.time.nanoseconds / 1E9));
			legacyEntry.message = entry.message;

			(*m_handler)(&legacyEntry);
		}
	}

private:
	GMSEC_LOGGER_HANDLER* m_handler;
};




void Log::SetReportingLevel(LogLevel level)
{
	fMaxLevel = level;
	gmsec::api::util::Log::setReportingLevel(level);
}


void Log::RegisterHandler(LogHandler *handler)
{
	for (int level = logERROR; level < logNLEVEL; level++)
	{
		RegisterHandler((LogLevel) level, handler);
	}
}


void Log::RegisterHandler(GMSEC_LOGGER_HANDLER *handler)
{
	for (int level = logERROR; level < logNLEVEL; level++)
	{
		RegisterHandler((LogLevel) level, handler);
	}
}


void Log::RegisterHandler(LogLevel level, LogHandler *handler)
{
	if (cxxAdapterHandlers[level] != NULL)
	{
		delete cxxAdapterHandlers[level];
		cxxAdapterHandlers[level] = NULL; 
	}

	if (handler != NULL)
	{
		cxxAdapterHandlers[level] = new API_4x_LogHandlerAdapter(handler);
	}

	gmsec::api::util::Log::registerHandler(level, cxxAdapterHandlers[level]);
}


void Log::RegisterHandler(LogLevel level, GMSEC_LOGGER_HANDLER* handler)
{
	if (ceeAdapterHandlers[level] != NULL)
	{
		delete ceeAdapterHandlers[level];
		ceeAdapterHandlers[level] = NULL;
	}

	if (handler != NULL)
	{
		ceeAdapterHandlers[level] = new CAPI_4x_LogHandlerAdapter(handler);
	}

	gmsec::api::util::Log::registerHandler(level, ceeAdapterHandlers[level]);
}


LogLevel Log::FromString(const char *level)
{
	return gmsec::api::util::Log::fromString(level);
}


const char* Log::ToString(LogLevel level)
{
	return gmsec::api::util::Log::toString(level);
}


void Log::SetDefaultStream(std::ostream *s)
{
	gmsec::api::internal::InternalLog::setDefaultStream(s);
}


class GMSEC_API API_4x_LoggerAdapter : public LogHandler
{
public:
	API_4x_LoggerAdapter(const char* file = NULL, int line = 0)
		: defaultStream(),
		  oss(&defaultStream),
		  handler(NULL)
	{
		entry.file    = file;
		entry.line    = line;
		entry.time    = 0.0;
		entry.level   = logNLEVEL;
		entry.message = NULL;
	}

	~API_4x_LoggerAdapter()
	{
		try
		{
			std::string msg;

			if (oss)
			{
				msg = oss->str();
			}

			entry.message = msg.c_str();

			if (handler != NULL)
			{
				gmsec::api::util::LogEntry newEntry;

				double fractionTime = entry.time - long(entry.time);

				newEntry.file             = entry.file;
				newEntry.line             = entry.line;
				newEntry.level            = entry.level;
				newEntry.time.seconds     = long(entry.time);
				newEntry.time.nanoseconds = long(fractionTime * 1E9) % 1000000000L;
				newEntry.message          = entry.message;

				handler->onMessage(newEntry);
			}
		}
		catch(...)
		{
			std::cerr << "There was an error displaying the message" << std::endl;
		}
	}

	std::ostream& get(LogLevel level)
	{
		setup(level);
		return *oss;
	}

	void setStream(std::ostringstream* o)
	{
		oss = o;
	}

	void setup(LogLevel level)
	{
		handler     = getHandler(level);
		entry.time  = getTime_s();
		entry.level = level;
	}

	virtual void CALL_TYPE OnMessage(const LogEntry& notUsed)
	{
		// we're not a real LogHandler, so do nothing
	}

private:
	std::ostringstream            defaultStream;
	std::ostringstream*           oss;
	gmsec::api::util::LogStream   logStream;
	gmsec::api::util::LogHandler* handler;
	LogEntry                      entry;
};



Logger::Logger()
	:
	handler(new API_4x_LoggerAdapter())
{
}


Logger::Logger(const char *file, int line)
	:
	handler(new API_4x_LoggerAdapter(file, line))
{
}


Logger::~Logger()
{
	API_4x_LoggerAdapter* logger = dynamic_cast<API_4x_LoggerAdapter*>(handler);
	delete logger;
}


std::ostream& Logger::Get(LogLevel level)
{
	API_4x_LoggerAdapter* logger = dynamic_cast<API_4x_LoggerAdapter*>(handler);
	return logger->get(level);
}


LoggerStream::LoggerStream()
	:
	handler(new API_4x_LoggerAdapter())
{
}


LoggerStream::LoggerStream(const char *file, int line)
	:
	handler(new API_4x_LoggerAdapter(file, line))
{
}


LoggerStream::~LoggerStream()
{
	API_4x_LoggerAdapter* logger = dynamic_cast<API_4x_LoggerAdapter*>(handler);
	delete logger;
}

LogStream& LoggerStream::Get(LogLevel level)
{
	std::ostringstream&   oss    = dynamic_cast<std::ostringstream&>(logStream.getStream());
	API_4x_LoggerAdapter* logger = dynamic_cast<API_4x_LoggerAdapter*>(handler);
	logger->setup(level);
	logger->setStream(&oss);
	return logStream;
}


LogStreamImp::LogStreamImp()
{
};

LogStreamImp::~LogStreamImp()
{
};

LogStream::LogStream()
{
	ptr = new LogStreamImp();
}

LogStream::~LogStream()
{
	delete ptr;
	ptr = 0;
}

LogStream &LogStream::operator<<(char x)
{
	ptr->os << x;
	return *this;
}

LogStream &LogStream::operator<<(int x)
{
	ptr->os << x;
	return *this;
}

LogStream &LogStream::operator<<(unsigned int x)
{
	ptr->os << x;
	return *this;
}

LogStream &LogStream::operator<<(long x)
{
	ptr->os << x;
	return *this;
}

LogStream &LogStream::operator<<(unsigned long x)
{
	ptr->os << x;
	return *this;
}

LogStream &LogStream::operator<<(long long x)
{
	ptr->os << x;
	return *this;
}

LogStream &LogStream::operator<<(unsigned long long x)
{
	ptr->os << x;
	return *this;
}

LogStream &LogStream::operator<<(const char *p)
{
	ptr->os << p;
	return *this;
}

LogStream &LogStream::operator<<(const void *p)
{
	ptr->os << p;
	return *this;
}

LogStream &LogStream::operator<<(float x)
{
	ptr->os << x;
	return *this;
}

LogStream &LogStream::operator<<(double x)
{
	ptr->os << x;
	return *this;
}

LogStream &LogStream::operator<<(const std::string &x)
{
	ptr->os << x;
	return *this;
}

std::string LogStream::getText()
{
	return ptr->os.str();
}

std::ostream &LogStream::getStream()
{
	return ptr->os;
}


}//namespace util
}//namespace gmsec
