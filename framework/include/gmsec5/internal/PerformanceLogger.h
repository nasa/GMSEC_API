/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file PerformanceLogger.h
 *
 * @author Matt Handy
 * @date July 25, 2014
 *
 * @brief This file contains the class definition for the Performance Logger.
 */

#ifndef GMSEC_API5_INTERNAL_PERFORMANCE_LOGGER_H
#define GMSEC_API5_INTERNAL_PERFORMANCE_LOGGER_H


#include <gmsec5/util/BoundedQueue.h>
#include <gmsec5/util/CountDownLatch.h>
#include <gmsec5/util/StdSharedPtr.h>

#include <string>
#include <cstdio>


namespace gmsec
{
namespace api5
{
namespace internal
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
	gmsec::api5::util::BoundedQueue<std::string> m_logEntryQueue;
	std::FILE* m_file;
	gmsec::api5::util::CountDownLatch* m_deathlatch;
};


/**
 * @fn runPerformanceLoggerThread
 *
 * @brief This is the body of the Performance Logger Thread.
 */
void runPerformanceLoggerThread(gmsec::api5::util::StdSharedPtr<PerformanceLogger> shared);


}  //end namespace internal
}  //end namespace api5
}  //end namespace gmsec

#endif
