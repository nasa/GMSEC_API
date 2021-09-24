/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file PerformanceLogger.h
 *
 *  @author Matt Handy
 *  @date July 25, 2014
 *
 *  @brief This file contains the class definition for the Performance Logger.
 */

#ifndef GMSEC_API_INTERNAL_PERFORMANCE_LOGGER_H
#define GMSEC_API_INTERNAL_PERFORMANCE_LOGGER_H


#include <gmsec4/util/BoundedQueue.h>
#include <gmsec4/util/CountDownLatch.h>
#include <gmsec4/util/StdSharedPtr.h>
#include <gmsec4/util/wdllexp.h>

#include <string>
#include <cstdio>


namespace gmsec
{
namespace api
{
namespace internal
{
namespace performance
{


class PerformanceLogger
{
public:
	PerformanceLogger(const char* logfile);

	virtual ~PerformanceLogger();

	virtual void run();

	virtual void shutdown();

	void dispatchLog(const std::string& subject, const std::string& publishTime);

private:
	// Declared, but not implemented.
	PerformanceLogger(const PerformanceLogger&);
	PerformanceLogger& operator=(const PerformanceLogger&);

	bool m_running;
	std::string m_logfile;
	gmsec::api::util::BoundedQueue<std::string> m_logEntryQueue;
	std::FILE* m_file;
	gmsec::api::util::CountDownLatch* m_deathlatch;
};


/**
 * @fn runPerformanceLoggerThread
 *
 * @brief This is the body of the Performance Logger Thread.
 */
void runPerformanceLoggerThread(gmsec::api::util::StdSharedPtr<PerformanceLogger> shared);


}//end namespace performance
}//end namespace internal
}//end namespace api
}//end namespace gmsec

#endif
