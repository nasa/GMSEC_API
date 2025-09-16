/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file utility.h
 *
 * @brief Defines common utility functions for C++ example programs.
 */

#ifndef GMSEC_CXX_EXAMPLES_UTILITY_H
#define GMSEC_CXX_EXAMPLES_UTILITY_H

#include <gmsec5/Config.h>
#include <gmsec5/util/Log.h>

using namespace gmsec::api5;
using namespace gmsec::api5::util;

void initializeLogLevel(const Config& config)
{

	LogLevel level = Log::fromString( config.getValue("loglevel", "info") );
	Log::setReportingLevel(level);
}

#endif
