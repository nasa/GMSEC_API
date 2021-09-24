/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/util/InternalLoggerStream.h>
#include <gmsec4/internal/util/InternalLog.h>

#include <gmsec4/util/TimeUtil.h>

#include <string>
#include <iostream>


using namespace gmsec::api::internal;
using namespace gmsec::api::util;


InternalLoggerStream::InternalLoggerStream(const char* file, int line)
	: m_handler(NULL)
{
	m_entry.file             = file;
	m_entry.line             = line;
	m_entry.time.seconds     = 0L;
	m_entry.time.nanoseconds = 0L;
	m_entry.level            = logNLEVEL;
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
