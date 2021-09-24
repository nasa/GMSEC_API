/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_LOGGING_LEVEL_NET_H
#define GMSEC_API_LOGGING_LEVEL_NET_H


namespace GMSEC
{
namespace API
{
namespace UTIL
{


/// <summary>
/// Logging level definition
/// </summary>


public enum class LoggingLevel
{
	NONE    = 0,
	ERROR   = 1,
	SECURE  = 2,
	WARNING = 3,
	INFO    = 4,
	VERBOSE = 5,
	DEBUG   = 6
};


} // end namespace UTIL
} // end namespace API
} // end namespace GMSEC


#endif
