/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/util/InternalLoggerStream.h>
#include <gmsec5/internal/util/InternalLog.h>

#include <gmsec5/util/TimeUtil.h>

#include <string>
#include <iostream>


using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


InternalLoggerStream::InternalLoggerStream(const char* file, int line)
	: m_handler(NULL)
{
	m_entry.file             = file;
	m_entry.line             = line;
	m_entry.time.seconds     = 0L;
	m_entry.time.nanoseconds = 0L;
	m_entry.level            = LogLevel::logNLEVEL;
	m_entry.message          = NULL;
}


InternalLoggerStream::~InternalLoggerStream()
{
	try
	{
		std::string tmp = m_logStream.getText();
		m_entry.message = tmp.c_str();

		if (m_handler != NULL)
		{
			m_handler->onMessage(m_entry);
		}
	}
	catch (...)
	{
		std::cerr << "~InternalLoggerStream: There was an error displaying a log message" << std::endl;
	}
}


LogStream& InternalLoggerStream::get(LogLevel level)
{
	m_handler     = getHandler(level);
	m_entry.time  = TimeUtil::getCurrentTime();
	m_entry.level = level;

	return m_logStream;
}
