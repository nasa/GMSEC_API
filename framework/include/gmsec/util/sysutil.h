/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef gmsec_util_sysutil_h
#define gmsec_util_sysutil_h


#include <gmsec/util/Deprecated.h>
#include <gmsec/util/String.h>
#include <gmsec/util/wdllexp.h>

#include <string>


namespace gmsec
{
namespace util
{

/**
* \fn millisleep (int milliseconds)
* \brief Sleeps for the specified number of milliseconds (at least 1).
*/
GMSEC_DEPRECATED GMSEC_API int CALL_TYPE millisleep(int milliseconds);

/**
* \fn getUserName (string &name)
* \brief Determines the user name.
* \return If there is an error, the return value will be non-zero
*/
GMSEC_DEPRECATED GMSEC_API int CALL_TYPE getUserName(std::string &s);

/**
* \fn getUserName (gmsec::util::String &name)
* \brief Determines the user name.
* \return If there is an error, the return value will be non-zero
*/
GMSEC_DEPRECATED GMSEC_API int CALL_TYPE getUserName(gmsec::util::String &s);

/**
* \fn getHostName (string &name)
* \brief Determines the host name.
* \return If there is an error, the return value will be non-zero
*/
GMSEC_DEPRECATED GMSEC_API int CALL_TYPE getHostName(std::string &s);

/**
* \fn getHostName (gmsec::util::String &name)
* \brief Determines the host name.
* \return If there is an error, the return value will be non-zero
*/
GMSEC_DEPRECATED GMSEC_API int CALL_TYPE getHostName(gmsec::util::String &s);

/**
* \fn getErrorString (int code, string &name)
* @brief Determines the string associated with an error code.
* @Returns true if successful.
*/
GMSEC_DEPRECATED GMSEC_API bool CALL_TYPE getErrorString(int code, std::string &s);

/**
* \fn getErrorString (int code, gmsec::util::String &name)
* @brief Determines the string associated with an error code.
* @Returns true if successful.
*/
GMSEC_DEPRECATED GMSEC_API bool CALL_TYPE getErrorString(int code, gmsec::util::String &s);

/**
* \fn getProcessID ()
* \brief Determines the user name.
* \return The ID of the current process.
*/
GMSEC_DEPRECATED GMSEC_API int CALL_TYPE getProcessID();


/**
* \fn SysUtilDummyFunc ()
* \brief A no-op function whose only purpose is to fetch a function
* address from the currently-running DLL, in a call to Linux dladdr().
*
*/
GMSEC_DEPRECATED GMSEC_API const void CALL_TYPE SysUtilDummyFunc();

} // namespace util
} // namespace gmsec


#endif /* gmsec_util_sysutil_h */
