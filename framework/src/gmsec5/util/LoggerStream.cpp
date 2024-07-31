/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file LoggerStream.cpp
 *
 * @brief Contains the structs and classes that support logging.
 */

#include <gmsec5/util/LoggerStream.h>

#include <gmsec5/internal/util/InternalLoggerStream.h>


using namespace gmsec::api5::util;
using namespace gmsec::api5::internal;


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
