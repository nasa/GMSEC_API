/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file PerformanceLogger.cpp
*
*  @author Matt Handy
*  @date July 25, 2014
*
*  @brief This file contains the implementation for the Performance Logger.
*
**/

#include <gmsec4/internal/performance/PerformanceLogger.h>

#include <gmsec4/Exception.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/TimeUtil.h>

#include <sstream>
#include <iomanip>


namespace gmsec {
namespace api {
namespace internal {
namespace performance{

using namespace gmsec::api::util;


PerformanceLogger::PerformanceLogger(const char* logfile)
	: m_running(false),
	  m_logfile(logfile),
	  m_logEntryQueue(1000),
	  m_file(NULL),
	  m_deathlatch(NULL)
{
}


PerformanceLogger::~PerformanceLogger()
{
	this->shutdown();
}


void PerformanceLogger::run()
{
#ifdef WIN32
	fopen_s(&m_file, m_logfile.c_str(), "a+");
#else
	m_file = std::fopen(m_logfile.c_str(), "a+");
#endif

	if (!m_file)
	{
		std::string msg = "Unable to open file for performance logging: ";
		msg.append(m_logfile);
		GMSEC_WARNING << msg.c_str();
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_VALUE, msg.c_str());
	}
	else
	{
		m_running = true;
		m_deathlatch = new CountDownLatch(1);
	}

	while (m_running)
	{
		std::string msg;

		if (m_logEntryQueue.poll(100, msg))
		{
			msg.append("\n");
			std::fputs(msg.c_str(), m_file);
			std::fflush(m_file);
		}
	}

	if (m_deathlatch)
	{
		m_deathlatch->countDown();
	}
}


void PerformanceLogger::shutdown()
{
	m_running = false;

	if (m_deathlatch)
	{
		m_deathlatch->await(1000);
		delete m_deathlatch;
		m_deathlatch = NULL;
	}
	if (m_file)
	{
		std::fclose(m_file);
		m_file = NULL;
	}
}


void PerformanceLogger::dispatchLog(const std::string& subject, const std::string& publishTime)
{
	char tempBuffer[GMSEC_TIME_BUFSIZE];

	GMSEC_TimeSpec ts;

	double now = TimeUtil::getCurrentTime_s(&ts);

	TimeUtil::formatTime(ts, tempBuffer);
	
	std::ostringstream strm;

	strm << subject << ",";
	strm << std::setprecision(3) << (now - TimeUtil::getCharTime_s(publishTime.c_str())) * 1000  << " ms,";
	strm << publishTime << ",";
	strm << tempBuffer;

	if (!m_logEntryQueue.offer(strm.str(), 100))
	{
		GMSEC_WARNING << "Unable to log latency for message with subject " << subject.c_str() << ", logging queue is full";
	}
}


void runPerformanceLoggerThread(gmsec::api::util::StdSharedPtr<PerformanceLogger> shared)
{
	shared->run();
}


}  // namespace performance
}  // namespace internal
}  // namespace api
}  // namespace gmsec
