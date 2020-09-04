/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * \file StringUtil.cpp
 * \brief Provide string utility functions.
 */


#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/internal/Encoder.h>

#include <gmsec4/util/Log.h>

#include <sstream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <cerrno>
#include <iterator>
#include <string.h>


#ifdef __sun
#include <stdarg.h>
#include <stdio.h>
#else
#include <cstdarg>
#include <cstdio>
#endif

#if defined(_MSC_VER)
#define strtoll _strtoi64
#endif

#if defined(__GNUC__) && !defined(__hpux)
#define HAVE_EXECINFO_H 1
#endif


#if HAVE_EXECINFO_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <execinfo.h>

extern "C" void __cxa_pure_virtual()
{
	gmsec::api::util::StringUtil::logStack("pure virtual handler", 0, 0);
	while (1) {
		sleep(1);
	}
}

#endif /* HAVE_EXECINFO_H */


#include <miniz.h>
#define COMPRESS_HDR_SZB (sizeof(GMSEC_U32))


using gmsec::api::internal::Encoder;
using gmsec::api::internal::Decoder;


namespace gmsec {
namespace api {
namespace util {



// BEGIN StringConverter
//

StringConverter& StringConverter::instance()
{
	static StringConverter theInstance;
	return theInstance;
}


StringConverter::StringConverter()
	: m_mode(TO_UPPERCASE)
{
}


void StringConverter::setMode(Mode mode)
{
	m_mode = mode;
}


std::string StringConverter::convertString(const std::string& str)
{
	return convertString(str.c_str());
}


std::string StringConverter::convertString(const char* str)
{
	std::string data;

	switch (m_mode)
	{
	case TO_UPPERCASE:
		std::transform(str, str + strlen(str), std::back_inserter(data), ::toupper);
		break;
	case TO_LOWERCASE:
		std::transform(str, str + strlen(str), std::back_inserter(data), ::tolower);
		break;
	case NO_CONVERSION:
	default:
		data = str;
		break;
	}

	return data;
}


// BEGIN StringUtil
//

size_t StringUtil::stringLength(const char * s)
{
	return (s ? strlen(s) : 0);
}


bool StringUtil::stringEquals(const char * s1, const char *s2)
{
	return strcmp(s1, s2) == 0;
}


bool StringUtil::stringEqualsIgnoreCase(const char * s1, const char *s2)
{
#if defined (WIN32)
	return _stricmp(s1, s2) == 0;
#else
	return strcasecmp(s1, s2) == 0;
#endif
}


int StringUtil::stringCompareCount(const char * s1, const char *s2, int count)
{
	return strncmp(s1, s2, count);
}


char * StringUtil::stringNew(const char * s)
{
	char* p = 0;

	if (s)
	{
		const size_t length = stringLength(s);
		p = new char[length + 1];
		if (p)
		{
			copyBytes(p, s, length);
			p[length] = '\0';
		}
	}

	return p;
}


void StringUtil::stringDestroy(const char *s)
{
#ifdef GMSEC_VC6
	delete[] (char*) s;
#else
	delete[] s;
#endif
}


/*
	If the length of src is known, it is more efficient to call
	copyBytes directly.
*/
void StringUtil::stringCopy(char * dest, const char *src, size_t destSize)
{
	if (!dest || !src || destSize == 0)
		return;

	const size_t srcSize = stringLength(src);
	const size_t cpySize = (srcSize < destSize ? srcSize : destSize - 1);

#if defined(WIN32)
	strncpy_s(dest, destSize, src, cpySize);
#else
	strncpy(dest, src, cpySize);
#endif
	dest[cpySize] = 0;
}


void StringUtil::copyBytes(void * dest, const void *src, size_t count)
{
	if (!dest || !src || count == 0)
		return;

	memcpy(dest, src, count);
}


std::vector<std::string> StringUtil::split(const std::string& str, const char delimiter)
{
	std::vector<std::string> ret;

	std::string current = "";
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] != delimiter)
			current += str[i];
		else
		{
			ret.push_back(current);
			current = "";
		}
	}

	ret.push_back(current);
	return ret;
}

std::string StringUtil::trim(const std::string& str, const char chop)
{
	if (str.empty())
		return str;

	size_t first = str.find_first_not_of(chop);
	if (first == std::string::npos)
		return str.substr(0, 0);

	return str.substr(first, str.find_last_not_of(chop) + 1 - first);
}

void StringUtil::stringFormatV(char * dest, int space, const char *format, va_list args)
{
	if (!dest || !format)
		return;

#ifdef WIN32
#ifdef GMSEC_VC6
	_vsnprintf(dest, space, format, args);
#else
	vsnprintf_s(dest, space, space - 1, format, args);
#endif /* GMSEC_VC6 */
#else
	vsnprintf(dest, space, format, args);
#endif /* WIN32 */
}

void StringUtil::stringFormat(char * dest, int space, const char *format, ...)
{
	if (!dest || !format)
		return;

	va_list args;
	va_start(args, format);

	stringFormatV(dest, space, format, args);

	va_end(args);
}

//pulled from glibc and renamed to GMSEC namespace for portability
int gmsec_mon_yday[2][13] =
{
    /* Normal years.  */
    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
    /* Leap years.  */
    { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};
static inline int gmsec_leapyear (int year)
{
  return
    ((year & 3) == 0
     && (year % 100 != 0
         || ((year / 100) & 3) == (- (1900 / 100) & 3)));
}

double StringUtil::getTimeFromString(const std::string& timeString)
{
	std::vector<std::string> comps = split(timeString, '-');
	if(comps.size() == 3)
	{
		int year = atoi(comps.at(0).c_str());
		int doy = atoi(comps.at(1).c_str());
		std::vector<std::string> subDayComps = split(comps.at(2), ':');
		if(subDayComps.size() == 3)
		{
			int hour = atoi(subDayComps.at(0).c_str());
			int minute = atoi(subDayComps.at(1).c_str());
			std::vector<std::string> subSecondComps = split(subDayComps.at(2), '.');
			if(subSecondComps.size() == 2)
			{
				int second = atoi(subSecondComps.at(0).c_str());
				double milli = atoi(subSecondComps.at(1).c_str());

				size_t subs      = subSecondComps.at(1).length();
				double milli_div = std::pow((double) 10, (double) subs);

				time_t now = time(NULL);
				struct tm utc;

#ifdef WIN32
#ifdef GMSEC_VC6
				static Mutex mutex;
				AutoMutex hold(mutex);
				utc = *gmtime_r(&now,&utc);
#else
				gmtime_s(&utc, &now);
#endif /* GMSEC_VC6 */
#else
				gmtime_r(&now, &utc);
#endif

				time_t utcTime = mktime(&utc);

				struct tm locTime;
				#ifdef WIN32
#ifdef GMSEC_VC6
				static Mutex mutex;
				AutoMutex hold(mutex);
				locTime = *localtime(&now);
#else
				localtime_s(&locTime, &now);
#endif /* GMSEC_VC6 */
#else
				localtime_r(&now, &locTime);
#endif
				time_t local = mktime(&locTime);
				time_t localTimeOffsetSeconds = local - utcTime;

				int leapYearIndex = gmsec_leapyear(year) ? 1 : 0;
				int month = 0;
				int dayOfMonth = 0;
				for (int mon = 0; mon < 12; mon++) { //12 months in year
					if (doy <= gmsec_mon_yday[leapYearIndex][mon+1]) {
						month = mon;
						dayOfMonth = doy - gmsec_mon_yday[leapYearIndex][mon];
						break;
					}
				}

				struct tm newtime;
				memset(&newtime, 0, sizeof(struct tm));
				newtime.tm_year = year - 1900;
				newtime.tm_hour = hour;
				newtime.tm_min = minute;
				newtime.tm_sec = second;
				newtime.tm_isdst = 0; //This works since publish time is UTC
				newtime.tm_mon = month;
				newtime.tm_mday = dayOfMonth;

				time_t reversed = mktime(&newtime);
				reversed += localTimeOffsetSeconds;
				double time = reversed + (milli / milli_div);
				return time;

			}
		}
	}

	return 0;
}

bool StringUtil::stringIsTrue(const char * s)
{
	return (s ? stringEqualsIgnoreCase(s, "TRUE") || stringEqualsIgnoreCase(s, "YES") : false);
}


// this allows leading space but not trailing...
bool StringUtil::stringParseI32(const char * s, GMSEC_I32 &out)
{
	bool flag = false;

	if (s)
	{
		GMSEC_I32 x = 0;
		std::istringstream in(s);
		in.unsetf(std::ios::skipws);
		in >> x;
		if (in && in.eof())
		{
			out = x;
			flag = true;
		}
	}

	return flag;
}


bool StringUtil::stringParseF64(const char * s, GMSEC_F64 &out)
{
	bool flag = false;

	if (s)
	{
		GMSEC_F64 x = 0;
		std::istringstream in(s);
		in.unsetf(std::ios::skipws);
		in >> x;
		if (in && in.eof())
		{
			out = x;
			flag = true;
		}
	}

	return flag;
}


std::string StringUtil::stringToLower(const std::string &str)
{
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}


std::string StringUtil::stringToUpper(const std::string &str)
{
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), ::toupper);
	return result;
}


std::string StringUtil::stripWhiteSpace(const std::string &in)
{
	std::string copy = in;

	// remove leading white-space
	size_t pos = copy.find_first_not_of(" \t");
	if (pos != std::string::npos)
	{
		copy = copy.substr(pos);
	}

	// remove trailing white-space
	pos = copy.find_last_not_of(" \t");
	if (pos!= std::string::npos)
	{
		copy = copy.substr(0, pos+1);
	}

	return copy;
}


StringUtil::STR2NUM_ERROR StringUtil::str2int (int &i, char const *s, int base)
{
	if (!s || *s == '\0')
	{
		return STR2NUM_INCONVERTIBLE;
	}
	char *end = 0;
	errno = 0;
	long tmp = strtol(s, &end, base);
	if ((errno == ERANGE && tmp == LONG_MAX) || tmp > INT_MAX)
	{
		return STR2NUM_OVERFLOW;
	}
	if ((errno == ERANGE && tmp == LONG_MIN) || tmp < INT_MIN)
	{
		return STR2NUM_UNDERFLOW;
	}
	if (end && *end != '\0')
	{
		return STR2NUM_INCONVERTIBLE;
	}
	i = tmp;
	return STR2NUM_SUCCESS;
}


StringUtil::STR2NUM_ERROR StringUtil::str2longlong (long long &l, char const *s, int base)
{
	if (!s || *s == '\0')
	{
		return STR2NUM_INCONVERTIBLE;
	}
	char *end = 0;
	errno = 0;
	long long tmp = strtoll(s, &end, base);
	if (end && *end != '\0')
	{
		return STR2NUM_INCONVERTIBLE;
	}
	l = tmp;
	return STR2NUM_SUCCESS;
}



static int lookupHexDigit(char digit)
{
	/* TODO: convert to lookup table */
	switch (digit)
	{
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		return (digit - 'a') + 10;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		return (digit - 'A') + 10;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return digit - '0';

	default:
		return -1;
	}
}


bool StringUtil::convertBlobToBase16 (const DataBuffer &in, std::string &out)
{
	const char *hex = "0123456789ABCDEF";

	out.resize(in.size() * 2);

	const DataBuffer::data_t * raw = in.get();

	for (size_t i = 0; i < in.size(); ++i)
	{
		DataBuffer::data_t tmp = raw[i];

		out[2*i] = hex[tmp / 16];
		out[2*i+1] = hex[tmp % 16];
	}

	return true;
}


bool StringUtil::convertBase16ToBlob (const std::string &in, DataBuffer &out)
{
	if (in.size() % 2)
		return false;

	const size_t n = in.size() / 2;
	out.resize(n);

	DataBuffer::data_t * raw = out.raw();
	const char * data = in.data();

	for (size_t i = 0; i < n; ++i)
	{
		int upper = lookupHexDigit(*data++);
		int lower = lookupHexDigit(*data++);
		if (upper < 0 || lower < 0)
                        return false;
		raw[i] = upper*16 + lower;
	}

	return true;
}


Mutex& StringUtil::getSharedMutex()
{
	static Mutex mutex;
	return mutex;
}


void StringUtil::logStack(const char *label, const char *file, int line)
{
	if (!label)
		label = "logStack";

	if (!file)
		file = "unknown";

	fprintf(stderr, "%s[%s:%d]\n", label, file, line);

#if HAVE_EXECINFO_H
	const size_t max_depth = 100;
	void *stack_addrs[max_depth];

	size_t stack_depth = backtrace(stack_addrs, max_depth);
	char **stack_strings = backtrace_symbols(stack_addrs, stack_depth);

	for (size_t i = 1; i < stack_depth; i++)
		fprintf(stderr, "\t%s\n", stack_strings[i]);

	free(stack_strings); // malloc()ed by backtrace_symbols
#endif
}


Status StringUtil::gmsec_compress(const DataBuffer& in, DataBuffer& out)
{
	using namespace gmsec_miniz;

	Status status;

	const mz_ulong inSize = (mz_ulong) in.size();

	mz_ulong compressedSize = mz_compressBound(inSize);
	unsigned char* compressedBuf = new unsigned char[COMPRESS_HDR_SZB + compressedSize];

	int err = MZ_OK;
	if (compressedBuf != 0)
	{
		// note: we do not compress the header area; only the data that follows it.
		err = mz_compress(compressedBuf + COMPRESS_HDR_SZB, &compressedSize, in.get(), inSize);

		if (err == MZ_OK)
		{
			const GMSEC_U32 afterSize = COMPRESS_HDR_SZB + compressedSize;

			if (afterSize < (GMSEC_U32) in.size())
			{
				// store length in buffer
				GMSEC_U32 temp = Encoder::encodeU32((GMSEC_U32) in.size());
				copyBytes(compressedBuf, &temp, sizeof(GMSEC_U32));
				out.copy(compressedBuf, afterSize);
			}
			else
			{
				status.set(NO_ERROR_CLASS, OTHER_ERROR_CODE, "Compressing message will not reduce size");
				GMSEC_VERBOSE << "Compressed buffer size is not smaller than original size of buffer.  Will send as raw binary.";
				out.copy(in.get(), in.size());
			}
		}
		else
		{
			status.set(OTHER_ERROR, CUSTOM_ERROR_CODE, "Error compressing message", err);
			const char *errstr = mz_error(err);
			if (!errstr) errstr = "unknown";
			GMSEC_WARNING << "Error compressing message (" << errstr << ").  Will send as raw binary.";
			out.copy(in.get(), in.size());
		}

		delete [] compressedBuf;
	}
	else
	{
		status.set(OTHER_ERROR, OUT_OF_MEMORY, "Error allocating memory for compressing message");
		GMSEC_WARNING << "Error occurred when attempting to compress message (unable to allocate memory).";
	}

	return status;
}


Status StringUtil::gmsec_uncompress(const DataBuffer& in, DataBuffer& out)
{
	using namespace gmsec_miniz;

	Status status;

	const unsigned char* inData = in.get();

	// Does the buffer contain compressed data?  Check the header...
	if (size_t(in.size()) >= COMPRESS_HDR_SZB)
	{
		// get size of uncompressed data
		GMSEC_U32 temp = 0;
		copyBytes(&temp, inData, sizeof(GMSEC_U32));
		temp = Decoder::decodeU32(temp);

		unsigned char* uncompressed = new unsigned char[temp];

		int err = MZ_OK;
		if (uncompressed != 0)
		{
			mz_ulong uncompressedSize = temp;   // conversion necessary!
			mz_ulong inSize = (mz_ulong) in.size();

			// note: we only uncompress the data sans the header.
			err = mz_uncompress(uncompressed, &uncompressedSize, inData + COMPRESS_HDR_SZB, inSize - COMPRESS_HDR_SZB);

			if (err == MZ_OK)
			{
				out.reset(uncompressed, (GMSEC_U32) uncompressedSize, true);
			}
			else
			{
				delete [] uncompressed;
				status.set(MSG_ERROR, BAD_MSG_FORMAT, "Error uncompressing message", err);
				const char *errstr = mz_error(err);
				if (!errstr) errstr = "unknown";
				GMSEC_WARNING << "Error uncompressing data (" << errstr << ")";
			}
		}
		else
		{
			status.set(OTHER_ERROR, OUT_OF_MEMORY, "Error allocating memory for uncompressing message");
			GMSEC_WARNING << status.get();
		}
	}
	else
	{
		// data is not compressed; treat it as raw data.
		out.copy(in.get(), in.size());
	}

	return status;
}


} // namespace util
} // namespace api
} // namespace gmsec
