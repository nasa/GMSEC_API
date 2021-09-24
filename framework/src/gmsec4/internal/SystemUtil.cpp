/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file SystemUtil.cpp
 *
 * @brief Provide system utility functions.
 */


#include <gmsec4/internal/SystemUtil.h>

#include <gmsec4/internal/Rawbuf.h>
#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/util/Log.h>


#ifdef WIN32
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <cerrno>
#include <cstring>
#include <dlfcn.h>
#include <dirent.h>
#endif


namespace gmsec
{
namespace api
{
namespace util
{


int SystemUtil::getUserName(std::string& userName)
{
	static std::string cached;

	int code = 0;

	if (cached.empty())
	{
		char buffer[1024];

#ifdef WIN32
		DWORD stringSize = sizeof(buffer);
		int tmp = GetUserName(buffer, &stringSize);
		if (tmp == 0)
		{
			buffer[0] = 0;
			code = GetLastError();
			std::string errstr;
			SystemUtil::getErrorString(code, errstr);
			GMSEC_WARNING << "GetUserName failed: code=" << code << '=' << errstr.c_str();
		}

		cached = buffer;

#else

		errno = 0;
		uid_t me = getuid();

		struct passwd my_passwd;
		struct passwd *result = 0;

#if defined(__sun)

		result = getpwuid_r(me,&my_passwd,buffer,sizeof(buffer));

#else

		(void) getpwuid_r(me,&my_passwd,buffer,sizeof(buffer),&result);

#endif
		if (result!=NULL)
		{
			cached = my_passwd.pw_name;
		}
		else
		{
			code = errno;
			rawbuf raw(buffer, sizeof(buffer));
			std::ostream os(&raw);
			os << "uid=" << me << std::ends;
			cached = buffer;
		}
#endif
	}

	userName = cached;

	return code;
}


int SystemUtil::getHostName(std::string& hostName)
{
	static std::string cached;

	int code = 0;

	if (cached.empty())
	{
		char buffer[256];

		buffer[0] = '\0';

#ifdef WIN32

		DWORD nsize = sizeof(buffer);
		int tmp = GetComputerName(buffer, &nsize);
		if (tmp == 0)
		{
			code = GetLastError();
		}

#else

		int nsize = sizeof(buffer);
		code = gethostname(buffer, nsize);

#endif

		// replace dots with underscores
		size_t length = StringUtil::stringLength(buffer);
		for (size_t i = 0; i < length; ++i)
		{
			if (buffer[i] == '.')
			{
				buffer[i] = '_';
			}
		}

		// save in the object
		cached = buffer;
	}

	hostName = cached;

	return code;
}


bool SystemUtil::getErrorString(int code, std::string& errorString)
{
#ifdef GMSEC_VC6
	errorString = "";
#else
	errorString.clear();
#endif

	if (!code)
	{
		return true;
	}

#ifdef WIN32

	// This code is for retrieving the error text from the windows API
	char buffer[1024];
	buffer[0] = 0;
	LPVOID lpMsgBuff = buffer;
	int tmp = FormatMessage(
	              FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	              NULL,
	              code,
	              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	              (LPTSTR) lpMsgBuff,
	              sizeof(buffer),
	              NULL
	          );
	if (!tmp)
	{
		GMSEC_WARNING << "FormatMessage(" << code << ") error=" << GetLastError();
	}
	else
	{
		errorString = buffer;
	}

#else

	// use strerror on UNI*
	const char* p = strerror(code);
	if (p)
	{
		errorString = p;
	}

#endif

	// if that failed, just use the number
	if (errorString.empty())
	{
		char buffer[64] = {0};
		rawbuf raw(&buffer[0], sizeof(buffer));
		std::ostream os(&raw);
		os << "error[#" << code << ']' << std::ends;
		errorString = buffer;
	}

	return false;
}


int SystemUtil::getProcessID()
{
	int pid = -1;

#if defined (WIN32)

	pid = _getpid();

#else

	pid = getpid();

#endif /* WIN32 */

	return pid;
}


} // namespace util
} // namespace api
} // namespace gmsec
