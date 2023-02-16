/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalLoggerStream.h
 *
 * @brief Contains the structs and classes that support logging.
 */

#ifndef GMSEC_API5_INTERNAL_LOGGER_STREAM_H
#define GMSEC_API5_INTERNAL_LOGGER_STREAM_H


#include <gmsec5_defs.h>

#include <gmsec5/util/LogHandler.h>
#include <gmsec5/util/LogStream.h>
#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
namespace internal
{


class GMSEC_API InternalLoggerStream
{
public:
	InternalLoggerStream(const char* file = NULL, int line = 0);

	~InternalLoggerStream();

	util::LogStream& get(LogLevel level = LogLevel::logINFO);

private:
	InternalLoggerStream(const InternalLoggerStream&);
	InternalLoggerStream& operator =(const InternalLoggerStream&);

	util::LogEntry     m_entry;
	util::LogHandler*  m_handler;
	util::LogStream    m_logStream;
};


} //namespace interanl
} //namespace api5
} //namespace gmsec

#endif
