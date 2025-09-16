/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file utility.h
 *
 * @brief Defines common utility functions for C example programs.
 */

#ifndef GMSEC_C_EXAMPLES_UTILITY_H
#define GMSEC_C_EXAMPLES_UTILITY_H

#include <gmsec5/c/config.h>
#include <gmsec5/c/util/log.h>

void initializeLogLevel(GMSEC_Config config)
{
	LogLevel level;

	if (config == NULL)
	{
		level = logINFO;
	}
	else
	{
		level = logLevelFromString( configGetValueOrDefault(config, "loglevel", "info", NULL) );
	}
	logSetReportingLevel(level);
}

#endif
