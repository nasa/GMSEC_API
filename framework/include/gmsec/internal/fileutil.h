/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef gmsec_internal_fileutil_h
#define gmsec_internal_fileutil_h


#include <gmsec/util/wdllexp.h>

#include <list>
#include <string>


#ifdef WIN32
    static const std::string PATH_SEPARATOR = "\\";
#else
    static const std::string PATH_SEPARATOR = "/";
#endif


namespace gmsec
{
namespace util
{

/**
* \fn getCurrentSharedObjectPath ()
* \brief Determines the file path of the calling shared object (DLL).
* \return True if successful in fetching the file path of the calling 
*         shared object (DLL).
*/
GMSEC_API bool CALL_TYPE getCurrentSharedObjectPath(std::string& path);


/**
* \fn getFilesInDirectory ()
* \brief Get a list of filenames in a given directory path.
* \return True if successful in fetching filenames in the given directory path.
*/
GMSEC_API bool CALL_TYPE getFilesInDirectory(const std::string& path, std::list<std::string>& filenames);


/**
* \fn SysUtilDummyFunc ()
* \brief A no-op function whose only purpose is to fetch a function
* address from the currently-running DLL, in a call to Linux dladdr().
*
*/
GMSEC_API const void CALL_TYPE sysUtilDummyFunc();


} // util
} // gmsec

#endif
