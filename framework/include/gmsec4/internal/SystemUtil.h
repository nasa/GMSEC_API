/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_SYSTEM_UTIL_H
#define GMSEC_API_INTERNAL_SYSTEM_UTIL_H


#include <gmsec4/util/wdllexp.h>

#include <string>


namespace gmsec
{
namespace api
{
namespace util
{

class GMSEC_API SystemUtil
{
public:
	/**
	 * @fn getUserName(std::string& userName)
	 *
	 * @brief Determines the user name.
	 *
	 * @return If there is an error, the return value will be non-zero
	 */
	 static int CALL_TYPE getUserName(std::string& userName);


	/**
	 * @fn getHostName(std::string& hostName)
	 *
	 * @brief Determines the host name.
	 *
	 * @return If there is an error, the return value will be non-zero
	 */
	static int CALL_TYPE getHostName(std::string& hostName);


	/**
	 * @fn getErrorString(int code, std::string& errorString)
	 *
	 * @brief Determines the string associated with an error code.
	 *
	 * @Returns true if successful.
	 */
	static bool CALL_TYPE getErrorString(int code, std::string& errorString);


	/**
	 * @fn getProcessID()
	 *
	 * @brief Determines the user name.
	 *
	 * @return The ID of the current process.
	 */
	static int CALL_TYPE getProcessID();


	/**
	 * @fn SysUtilDummyFunc()
	 *
	 * @brief A no-op function whose only purpose is to fetch a function
	 * address from the currently-running DLL, in a call to Linux dladdr().
	 */
	static const void CALL_TYPE SysUtilDummyFunc();
};


} // namespace util
} // namespace api
} // namespace gmsec

#endif
