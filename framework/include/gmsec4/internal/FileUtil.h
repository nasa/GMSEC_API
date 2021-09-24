/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_FILE_UTIL_H
#define GMSEC_API_INTERNAL_FILE_UTIL_H


#include <gmsec4/util/wdllexp.h>

#include <list>
#include <string>


namespace gmsec
{
namespace api
{
namespace util
{


class GMSEC_API FileUtil
{
public:
    static const std::string PATH_SEPARATOR;


	/**
	 * @fn getCurrentSharedObjectPath(std::string& path)
	 * @brief Determines the file path of the calling shared object (DLL).
	 * @return True if successful in fetching the file path of the calling shared object (DLL).
	 */
	static bool CALL_TYPE getCurrentSharedObjectPath(std::string& path);


	/**
	 * @fn getFilesInDirectory(const std::string& path, std::list<std::string>& filenames)
	 * @brief Get a list of filenames in a given directory path.
	 * @return True if successful in fetching filenames in the given directory path.
	 */
	static bool CALL_TYPE getFilesInDirectory(const std::string& path, std::list<std::string>& filenames);


	/**
	 * @fn sysUtilDummyFunc ()
	 * @brief A no-op function whose only purpose is to fetch a function
	 * address from the currently-running DLL, in a call to Linux dladdr().
	 */
	static const void CALL_TYPE sysUtilDummyFunc();
};


} // util
} // api
} // gmsec

#endif
