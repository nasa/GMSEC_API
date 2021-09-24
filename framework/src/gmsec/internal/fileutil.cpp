/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec/internal/fileutil.h>

#ifdef WIN32
	#include <windows.h>
	#include <process.h>
#else
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <unistd.h>
	#include <dirent.h>
	#include <dlfcn.h>
#endif


namespace gmsec
{
namespace util
{

#ifdef WIN32
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#endif
bool getCurrentSharedObjectPath(std::string& path)
{
	bool ret_val = false;

#ifndef WIN32
	Dl_info dl_info;
#if defined(__sun)
	if (dladdr(sysUtilDummyFunc, &dl_info) != 0)
#elif defined(__hpux)
	if (dladdr((void* )sysUtilDummyFunc, &dl_info) != 0)
#else
	if (dladdr((const void*) sysUtilDummyFunc, &dl_info) != 0)
#endif
	// dladdr returns 0 on error
	{
		path = dl_info.dli_fname;
		ret_val = true;
	}
#else
	char dll_path[MAX_PATH] = {0};
	if (GetModuleFileName((HINSTANCE)&__ImageBase, dll_path, MAX_PATH) != 0)
	{
		path = dll_path;
		ret_val = true;
	}
#endif

	return ret_val;
}


bool getFilesInDirectory(const std::string& path, std::list<std::string>& filenames)
{
#ifndef WIN32
	DIR* dir_ptr = opendir(path.c_str());

	if (dir_ptr != NULL)
	{
		struct dirent* dir_entry = 0;

		while ((dir_entry = readdir(dir_ptr)) != 0)
		{
			std::string filename = path + PATH_SEPARATOR + dir_entry->d_name;
			struct stat buf;

			if (lstat(filename.c_str(), &buf) == 0)
			{
				if (S_ISREG(buf.st_mode))
				{
					// Only added filename to the list, not the
					// entire path, because of expected usage.
					// Note that the user supplied the path 
					// as an input parameter to this function
					// anyway.
					filenames.push_back(dir_entry->d_name);
				}
			}
		}

		closedir(dir_ptr);
	}
#else
	std::string file_pattern_to_list = path;
	file_pattern_to_list += "\\*";

	WIN32_FIND_DATA file_result;
	HANDLE file_entry_handle = FindFirstFile(file_pattern_to_list.c_str(), &file_result);

	bool done = false;
	while (file_entry_handle != INVALID_HANDLE_VALUE && !done)
	{
		if (file_result.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{
			// Only added filename to the list, not the
			// entire path, because of expected usage.
			// Note that the user supplied the path 
			// as an input parameter to this function
			// anyway.
			filenames.push_back(file_result.cFileName);
		}

		if (FindNextFile(file_entry_handle, &file_result) == false)
		{
			done = true;
		}
	}
#endif

	return filenames.size() > 0;
}


const void sysUtilDummyFunc()
{
	// do nothing; just a placeholder function to lookup shared-object/DLL library.
}

}
}
