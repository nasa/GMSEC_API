/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file LoggerStream.cpp
 *
 * @brief Contains the structs and classes that support logging.
 */

#include <gmsec4/util/LoggerStream.h>

#include <gmsec4/internal/util/InternalLoggerStream.h>


using namespace gmsec::api::util;
using namespace gmsec::api::internal;


LoggerStream::LoggerStream(const char* file, int line)
	: m_internal(new InternalLoggerStream(file, line))
{
}


LoggerStream::~LoggerStream()
{
	delete m_internal;
}


LogStream& LoggerStream::get(LogLevel level)
{
	return m_internal->get(level);
}
