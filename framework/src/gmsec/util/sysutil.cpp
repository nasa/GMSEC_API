
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#include <gmsec/util/sysutil.h>
#include <gmsec/internal/strutil.h>
#include <gmsec/internal/rawbuf.h>
#include <gmsec/internal/Log.h>


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

#include <algorithm> // for transform
#include <ctype.h>   // for toupper


/**
 * \file sysutil.cpp
 * \brief Provide system utility functions.
 */


using namespace std;


namespace gmsec
{
namespace util
{


int getUserName(string &s)
{
	static string cached;
	int code = 0;

	if (cached.length() < 1)
	{
		char buffer[1024];

#ifdef WIN32
		DWORD stringSize = sizeof(buffer);
		int tmp = GetUserName(buffer, &stringSize);
		if (tmp == 0)
		{
			buffer[0] = 0;
			code = GetLastError();
			string errstr;
			getErrorString(code, errstr);
			LOG_WARNING << "GetUserName failed: code=" << code
					<< '=' << errstr;
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
                           cached = my_passwd.pw_name;
		else
		{
			code = errno;
			rawbuf raw(buffer, sizeof(buffer));
			ostream os(&raw);
			os << "uid=" << me << ends;
			cached = buffer;
		}
#endif
               
	}

	s = cached;

	return code;
}

int getUserName(gmsec::util::String &s)
{
	std::string str;
	int code = getUserName(str);
	s.assign(str.c_str());
	return code;
}


int getHostName(string &s)
{
	static string cached;
	int code = 0;

	if (cached.length() < 1)
	{

		char buffer[256];

		buffer[0] = '\0';

#ifdef WIN32

		DWORD nsize = sizeof(buffer);
		int tmp = GetComputerName(buffer, &nsize);
		if (tmp == 0)
			code = GetLastError();

#else

		int nsize = sizeof(buffer);
		code = gethostname(buffer, nsize);

#endif

		// replace dots with underscores
		size_t length = stringLength(buffer);
		for (size_t i = 0; i < length; ++i)
			if (buffer[i] == '.')
				buffer[i] = '_';

		// save in the object
		cached = buffer;
	}

	s = cached;

	return code;
}

int getHostName(gmsec::util::String &s)
{
	std::string str;
	int code = getHostName(str);
	s.assign(str.c_str());
	return code;
}


bool getErrorString(int code, string &s)
{
#ifdef GMSEC_VC6
	s = "";
#else
	s.clear();
#endif

	if (!code)
		return true;

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
		LOG_WARNING << "FormatMessage(" << code << ") error=" << GetLastError();
	else
		s = buffer;

#else

	// use strerror on UNI*
	const char *p = strerror(code);
	if (p)
		s = p;

#endif

	// if that failed, just use the number
	if (s.empty())
	{
		char buffer[64] = {0};
		rawbuf raw(&buffer[0], sizeof(buffer));
		ostream os(&raw);
		os << "error[#" << code << ']' << std::ends;
		s = buffer;
	}

	return false;
}


bool getErrorString(int code, gmsec::util::String &s)
{
	std::string str;
	bool val = getErrorString(code, str);
	s.assign(str.c_str());
	return val;
}


int /* CALL_TYPE */ millisleep(int milliseconds)
{
	if (milliseconds < 1)
		milliseconds = 1;

#if defined (WIN32)

	Sleep(milliseconds);

#else

	const int micros_per_second = 1000 * 1000;

	int seconds = 0;
	int micros = milliseconds * 1000;
	if (micros >= micros_per_second)
	{
		seconds = micros / micros_per_second;
		micros %= micros_per_second;
	}

	struct timeval timeout = { seconds, micros };
	int code = select(0, 0, 0, 0, &timeout);
	if (code)
	{
		// TODO : log warning
	}

#endif /* WIN32 */

	return milliseconds;
}


int /* CALL_TYPE */ getProcessID()
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
} // namespace gmsec

