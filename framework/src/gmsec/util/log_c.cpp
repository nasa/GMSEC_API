
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file log_c.cpp
 *
 *  @brief This file contains the C-wrapper for functions in the Logger object.
 *
 */

#include <gmsec/util/log_c.h>

#undef LOG_LEVEL_DEFINE
#undef LOG_ERROR
#undef LOG_WARNING
#undef LOG_INFO
#undef LOG_VERBOSE
#undef LOG_DEBUG
#undef LOG

#include <gmsec/internal/strutil.h>

#include <gmsec/internal/Log.h>


using namespace gmsec;
using namespace gmsec::util;


void CALL_TYPE gmsec_SetReportingLevel(enum LogLevel level)
{
	Log::SetReportingLevel(level);
}

enum LogLevel CALL_TYPE gmsec_GetReportingLevel()
{
	return Log::GetReportingLevel();
}

GMSEC_BOOL CALL_TYPE gmsec_IsLoggingEnabled(enum LogLevel level)
{
	if (level <= gmsec_GetReportingLevel())
	{
		return GMSEC_TRUE;
	}
	return GMSEC_FALSE;
}

void CALL_TYPE gmsec_RegisterLogHandler(GMSEC_LOGGER_HANDLER *handler)
{
	Log::RegisterHandler(handler);
}

void CALL_TYPE gmsec_RegisterLogHandlerByLevel(enum LogLevel level, GMSEC_LOGGER_HANDLER *handler)
{
	Log::RegisterHandler(level, handler);
}

const char* CALL_TYPE gmsec_LogLevelToString(enum LogLevel level)
{
	return Log::ToString(level);
}

enum LogLevel CALL_TYPE gmsec_LogLevelFromString(const char *level)
{
	return Log::FromString(level);
}

void CALL_TYPE gmsec_CAPILog(enum LogLevel level, char *fmt, ...)
{
	char buffer[2046];

	va_list args;
	va_start(args, fmt);
	stringFormatV(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	Logger().Get(level) << buffer;
}


