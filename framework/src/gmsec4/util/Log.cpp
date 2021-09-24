/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/util/Log.h>

#include <gmsec4/internal/util/InternalLog.h>


using namespace gmsec::api::util;
using namespace gmsec::api::internal;


void Log::setReportingLevel(LogLevel level)
{
	InternalLog::setReportingLevel(level);
}


LogLevel Log::getReportingLevel()
{
	return InternalLog::getReportingLevel();
}


void Log::registerHandler(LogHandler* handler)
{
	InternalLog::registerHandler(handler);
}


void Log::registerHandler(LogLevel level, LogHandler* handler)
{
	InternalLog::registerHandler(level, handler);
}


void Log::registerHandler(GMSEC_LogHandler* handler)
{
	InternalLog::registerHandler(handler);
}


void Log::registerHandler(LogLevel level, GMSEC_LogHandler* handler)
{
	InternalLog::registerHandler(level, handler);
}


LogLevel Log::fromString(const char* level)
{
	return InternalLog::fromString(level);
}


const char* Log::toString(LogLevel level)
{
	return InternalLog::toString(level);
}
