/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalLoggerStream.h
 *
 * @brief Contains the structs and classes that support logging.
 */

#ifndef GMSEC_API_INTERNAL_LOGGER_STREAM_H
#define GMSEC_API_INTERNAL_LOGGER_STREAM_H


#include <gmsec4_defs.h>

#include <gmsec4/util/LogHandler.h>
#include <gmsec4/util/LogStream.h>
#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace internal
{


class GMSEC_API InternalLoggerStream
{
public:
	InternalLoggerStream(const char* file = NULL, int line = 0);

	~InternalLoggerStream();

	util::LogStream& get(LogLevel level = logINFO);

private:
	InternalLoggerStream(const InternalLoggerStream&);
	InternalLoggerStream& operator =(const InternalLoggerStream&);

	util::LogEntry     m_entry;
	util::LogHandler*  m_handler;
	util::LogStream    m_logStream;
};


} //namespace interanl
} //namespace api
} //namespace gmsec

#endif
