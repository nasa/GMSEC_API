/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file StringUtil.cpp
 * @brief Provide string utility functions.
 */


#include <gmsec5/internal/StringUtil.h>

#include <gmsec5/internal/Encoder.h>

#include <gmsec5/util/Log.h>

#include <algorithm>
#include <functional> 
#include <iomanip>
#include <iostream>
#include <iterator>
#include <locale>
#include <regex>

#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>


#if defined(_MSC_VER)
#define strtoll _strtoi64
#endif

#if defined(__GLIBC__) && !defined(__hpux)
#define HAVE_EXECINFO_H 1
#endif


#if HAVE_EXECINFO_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <execinfo.h>

extern "C" void __cxa_pure_virtual()
{
	gmsec::api5::util::StringUtil::logStack("pure virtual handler", 0, 0);
	while (1) {
		sleep(1);
	}
}

#endif /* HAVE_EXECINFO_H */


#include <json.h>
#include <tinyxml2.h>
#include <miniz.h>
#define COMPRESS_HDR_SZB (sizeof(GMSEC_U32))


using gmsec::api5::internal::Encoder;
using gmsec::api5::internal::Decoder;


namespace gmsec {
namespace api5 {
namespace util {


// BEGIN StringUtil
//

static const char GMSEC_HYPHEN     = '-';
static const char GMSEC_UNDERSCORE = '_';
static const char GMSEC_PERIOD     = '.';
static const char GMSEC_DOT        = GMSEC_PERIOD;
static const char GMSEC_COLON      = ':';
static const char GMSEC_PLUS       = '+';
static const char GMSEC_MINUS      = GMSEC_HYPHEN;


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
		copyBytes(p, s, length);
		p[length] = '\0';
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
	std::string delimitersStr;

	delimitersStr += delimiter;

	return split(str, delimitersStr);
}

std::vector<std::string> StringUtil::split(const std::string& str, const std::string& delimiters)
{
	std::vector<std::string> tokens;

	for (size_t begin = 0, pos = 0; pos != std::string::npos; begin = pos + 1)
	{
		pos = str.find_first_of(delimiters, begin);

		std::string token = str.substr(begin, pos - begin);

		if (!token.empty())  // ignore empty tokens
		{
			tokens.push_back(token);
		}
	}

	return tokens;
}


std::string StringUtil::ltrim(const std::string& str)
{
	std::string tmp = str;
	tmp.erase(tmp.begin(), std::find_if(tmp.begin(), tmp.end(), [](int i) { return !std::isspace(i); }));
    return tmp;
}


std::string StringUtil::rtrim(const std::string& str)
{
	std::string tmp = str;
    tmp.erase(std::find_if(tmp.rbegin(), tmp.rend(), [](int i) { return !std::isspace(i); }).base(), tmp.end());
    return tmp;
}


std::string StringUtil::trim(const std::string& str)
{
	return ltrim(rtrim(str));
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


double StringUtil::getTimeFromString(const std::string& timeString)
{
	DateTime dt = isValidTimestamp(timeString);

	if (!dt.m_valid)
	{
		std::ostringstream err;
		err << "Invalid time string: " << timeString;
		throw GmsecException(OTHER_ERROR, PARSE_ERROR, err.str().c_str());
	}

	time_t now = time(NULL);
	struct tm utc;

#ifdef WIN32
		gmtime_s(&utc, &now);
#else
		gmtime_r(&now, &utc);
#endif

	time_t utcTime = mktime(&utc);
	struct tm locTime;

#ifdef WIN32
	localtime_s(&locTime, &now);
#else
	localtime_r(&now, &locTime);
#endif

	time_t local = mktime(&locTime);
	time_t localTimeOffsetSeconds = local - utcTime;

	int leapYearIndex = (isLeapYear(dt.m_year) ? 1 : 0);
	int month = 0;
	int dayOfMonth = 0;
	for (int mon = 0; mon < 12; mon++) {
		if (dt.m_doy <= gmsec_mon_yday[leapYearIndex][mon+1]) {
			month = mon;
			dayOfMonth = dt.m_doy - gmsec_mon_yday[leapYearIndex][mon];
			break;
		}
	}

	struct tm newtime;
	memset(&newtime, 0, sizeof(struct tm));

	if (dt.m_relative)
	{
		time_t now = time(NULL);
		struct tm utc;

#ifdef WIN32
		gmtime_s(&utc, &now);
#else
		gmtime_r(&now, &utc);
#endif

		if (dt.m_future) {
			newtime.tm_year = utc.tm_year + dt.m_year;
			newtime.tm_mon  = utc.tm_mon  + month;
			newtime.tm_mday = utc.tm_mday + dayOfMonth;
			newtime.tm_hour = utc.tm_hour + dt.m_hour;
			newtime.tm_min  = utc.tm_min  + dt.m_min;
			newtime.tm_sec  = utc.tm_sec  + dt.m_sec;
		}
		else {
			newtime.tm_year = utc.tm_year - dt.m_year;
			newtime.tm_mon  = utc.tm_mon  - month;
			newtime.tm_mday = utc.tm_mday - dayOfMonth;
			newtime.tm_hour = utc.tm_hour - dt.m_hour;
			newtime.tm_min  = utc.tm_min  - dt.m_min;
			newtime.tm_sec  = utc.tm_sec  - dt.m_sec;

			dt.m_milli *= -1;   // later this will be added to the offsetted time
		}
	}
	else
	{
		newtime.tm_year = dt.m_year - 1900;
		newtime.tm_mon  = month;
		newtime.tm_mday = dayOfMonth;
		newtime.tm_hour = dt.m_hour;
		newtime.tm_min  = dt.m_min;
		newtime.tm_sec  = dt.m_sec;
	}

	newtime.tm_isdst = 0; //This works since publish time is UTC

	time_t reversed = mktime(&newtime);
	reversed += localTimeOffsetSeconds;
	double time = reversed + (dt.m_milli / dt.m_milli_div);

	return time;
}


bool StringUtil::stringIsTrue(const char * s)
{
	return (s ? stringEquals(s, "1") || stringEqualsIgnoreCase(s, "TRUE") || stringEqualsIgnoreCase(s, "YES") : false);
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


bool StringUtil::stringParseF32(const char * s, GMSEC_F32 &out)
{
	bool flag = false;

	if (s)
	{
		GMSEC_F32 x = 0;
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

	// note: we do not compress the header area; only the data that follows it.
	int err = mz_compress(compressedBuf + COMPRESS_HDR_SZB, &compressedSize, in.get(), inSize);

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

		mz_ulong uncompressedSize = temp;   // conversion necessary!
		mz_ulong inSize = (mz_ulong) in.size();

		// note: we only uncompress the data sans the header.
		int err = mz_uncompress(uncompressed, &uncompressedSize, inData + COMPRESS_HDR_SZB, inSize - COMPRESS_HDR_SZB);

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
		// data is not compressed; treat it as raw data.
		out.copy(in.get(), in.size());
	}

	return status;
}


bool StringUtil::getBoolean(const char* value)
{
	if (value == NULL)
	{
		throw GmsecException(OTHER_ERROR, PARSE_ERROR, "Value cannot be NULL");
	}

	std::string tmpValue = StringUtil::trim(value);

	if (tmpValue.empty())
	{
		throw GmsecException(OTHER_ERROR, PARSE_ERROR, "Unable to convert value");
	}

	return StringUtil::stringIsTrue(tmpValue.c_str());
}


std::string StringUtil::toXML(const char* data)
{
	if (data == NULL)
	{
		throw GmsecException(OTHER_ERROR, PARSE_ERROR, "Cannot convert data value to XML; data is NULL");
	}

	std::ostringstream oss;

	for (const char* c = data; *c; ++c)
	{
		// Convert the data value if necessary to its XML equivalent;
		// otherwise leave it as is.
		//
		switch (*c)
		{
		case 0x0a:  // newline
			oss << "&#10;";
			break;
		case 0x0d:  // carriage return
			oss << "&#13;";
			break;
		case 0x22:  // double quote
			oss << "&quot;";
			break;
		case 0x26:  // ampersand
			oss << "&amp;";
			break;
		case 0x27:  // single quote
			oss << "&apos;";
			break;
		case 0x3C:  // less than
			oss << "&lt;";
			break;
		case 0x3E:  // greater than
			oss << "&gt;";
			break;
		default:
			oss << *c;
			break;
		}
	}

	return oss.str();
}


std::string StringUtil::toJSON(const char* data)
{
	if (data == NULL)
	{
		throw GmsecException(OTHER_ERROR, PARSE_ERROR, "Cannot convert data value to JSON; data is NULL");
	}

	return Json::valueToQuotedString(data);
}


bool StringUtil::isLeapYear(unsigned int year)
{
	return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}


bool StringUtil::isValidHeaderString(const std::string& fieldValue)
{
	if (fieldValue.empty())
	{
		return false;
	}

	std::regex ext(".[A-Z0-9_-]*");
	return std::regex_match(fieldValue, ext);
}


bool getTime(const std::string& time, StringUtil::DateTime& dt, bool mustHaveAllFields = false)
{
	dt.m_valid = true;

	int result1 = 0, result2 = 0, result3 = 0, result4 = 0;

	std::vector<std::string> elems = StringUtil::split(time, ':');

	if (mustHaveAllFields && elems.size() != 3)
	{
		dt.m_valid = false;
	}
	else
	{
		std::string secs;

		switch (elems.size())
		{
		case 1:
			secs       = elems[0];
			elems      = StringUtil::split(secs, ".");
			result1    = sscanf(elems[0].c_str(), "%02d", &dt.m_sec);
			if (elems.size() > 1) {
				result2    = sscanf(elems[1].c_str(), "%09d", &dt.m_milli);
				dt.m_milli_div = std::pow(static_cast<double>(10), static_cast<double>(elems[1].length()));
			}
			dt.m_valid = (result1 == 1 && result2 >= 0);
			break;
		case 2:
			result1    = sscanf(elems[0].c_str(), "%02d", &dt.m_min);
			secs       = elems[1];
			elems      = StringUtil::split(secs, ".");
			result2    = sscanf(elems[0].c_str(), "%02d", &dt.m_sec);
			if (elems.size() > 1) {
				result3    = sscanf(elems[1].c_str(), "%09d", &dt.m_milli);
				dt.m_milli_div = std::pow(static_cast<double>(10), static_cast<double>(elems[1].length()));
			}
			dt.m_valid = (result1 == 1 && result2 == 1 && result3 >= 0);
			break;
		case 3:
			result1    = sscanf(elems[0].c_str(), "%02d", &dt.m_hour);
			result2    = sscanf(elems[1].c_str(), "%02d", &dt.m_min);
			secs       = elems[2];
			elems      = StringUtil::split(secs, ".");
			result3    = sscanf(elems[0].c_str(), "%02d", &dt.m_sec);
			if (elems.size() > 1) {
				if (elems[1] != "") {
					result4    = sscanf(elems[1].c_str(), "%09d", &dt.m_milli);
					dt.m_milli_div = std::pow(static_cast<double>(10), static_cast<double>(elems[1].length()));
				}
				else {
					dt.m_valid = false;
				}
			}
			dt.m_valid = (result1 == 1 && result2 == 1 && result3 == 1 && result4 >= 0);
			break;
		default:
			dt.m_valid = false;
		}

		if (dt.m_valid && secs[ secs.length() - 1 ] == GMSEC_DOT)
		{
			dt.m_valid = false;
		}
	}

	return dt.m_valid;
}


StringUtil::DateTime StringUtil::isValidTimestamp(const std::string& fieldValue)
{
	DateTime dt;

	std::string fldValue = fieldValue;

	if (!isdigit(fldValue[0]))
	{
		dt.m_valid    = (fldValue[0] == GMSEC_PLUS || fldValue[0] == GMSEC_MINUS);
		dt.m_relative = dt.m_valid;
		dt.m_future   = (fldValue[0] == GMSEC_PLUS);

		fldValue = fieldValue.substr(1);
	}

	if (!dt.m_valid)
	{
		return dt;
	}

	std::vector<std::string> elems = StringUtil::split(fldValue, '-');

	if (elems.size() == 1 && dt.m_relative)
	{
		// only time given
		getTime(fldValue, dt);
	}
	else if (elems.size() == 2 && dt.m_relative)
	{
		// only day and time given
		int result = sscanf(elems[0].c_str(), "%03d", &dt.m_doy);
		dt.m_valid = ((result == 1) && getTime(elems[1], dt, true));
	}
	else if (elems.size() == 3)
	{
		// year, day and time given
		int result1 = sscanf(elems[0].c_str(), "%04d", &dt.m_year);
		int result2 = sscanf(elems[1].c_str(), "%03d", &dt.m_doy);

		if (result1 != 1 || elems[0].length() != 4)
		{
			dt.m_valid = false;
		}
		else if (result2 != 1 || elems[1].length() != 3)
		{
			dt.m_valid = false;
		}
		else
		{
			getTime(elems[2], dt, true);
		}
	}
	else
	{
		dt.m_valid = false;
	}

	if (dt.m_valid)
	{
		int dayMin = (dt.m_relative ? 0 : 1);
		int dayMax = (StringUtil::isLeapYear(dt.m_year) ? 366 : 365);

		if (dt.m_valid && (dt.m_year > 9999)) dt.m_valid = false;
		if (dt.m_valid && ((dt.m_doy < dayMin) || (dt.m_doy > dayMax))) dt.m_valid = false;
		if (dt.m_valid && (dt.m_hour > 23)) dt.m_valid = false;
		if (dt.m_valid && (dt.m_min > 59)) dt.m_valid = false;
		if (dt.m_valid && (dt.m_sec > 60)) dt.m_valid = false;  // 60 is to allow for leap-seconds
	}

	return dt;
}


bool StringUtil::isValidIpAddress(const std::string& address)
{
	bool valid = false;

	if (address.find('.') != std::string::npos)
	{
		// IPv4 style address
		unsigned int quad1, quad2, quad3, quad4;

		int result = sscanf(address.c_str(), "%03u.%03u.%03u.%03u", &quad1, &quad2, &quad3, &quad4);

		valid = (result == 4);
		valid = (valid && quad1 < 256);
		valid = (valid && quad2 < 256);
		valid = (valid && quad3 < 256);
		valid = (valid && quad4 < 256);
	}
	else if (address.find(':') != std::string::npos)
	{
		// IPv6 style address
		std::vector<std::string> quads = StringUtil::split(address, ':');

		valid = (quads.size() == 8);

		for (size_t i = 0; i < quads.size() && valid; ++i)
		{
			valid = !(quads[i].empty());

			if (valid)
			{
				char junk;
				int ret = sscanf(quads[i].c_str(), "%*[A-F0-9]%c", &junk);
				valid = (ret == -1);
			}
		}
	}

	return valid;
}


StringUtil::Data StringUtil::removeLeadingZeros(const Data& data)
{
	Data::const_iterator it = data.begin();
	size_t i = 0;

	for (; it != data.end(); ++it, ++i)
	{
		if (*it != 0) break;
	}

	// If the data only contains zeros, preserve at least one of them.
	if (i == data.length()) --i;

	return Data(data.begin() + i, data.end());
}


StringUtil::Data StringUtil::padWithLeadingZeros(const Data& data, size_t numZeros)
{
	Data padded = data;

	for (size_t i = 0; i < numZeros; ++i)
	{
		padded.insert(static_cast<size_t>(0), static_cast<size_t>(1), 0x00);
	}

	return padded;
}


StringUtil::Data StringUtil::string_toBinary(const char* value)
{
	if (value == NULL)
	{
		throw GmsecException(OTHER_ERROR, INVALID_STRING_PARAM, "Value cannot be NULL");
	}

	return StringUtil::Data((const unsigned char*) value, std::string(value).length());
}


StringUtil::Data StringUtil::binaryString_toBinary(const char* value)
{
	StringUtil::Data blob;

	size_t blob_len = std::string(value).length();

	for (size_t i = 0; i < blob_len; i += 2)
	{
		unsigned int ch;
		char hex[3] = { value[i], value[i+1], 0 };
#ifndef WIN32
		std::sscanf(hex, "%02X", &ch);
#else
		sscanf_s(hex, "%02X", &ch);
#endif

		blob.push_back((unsigned char) ch);
	}

	return blob;
}


StringUtil::Data StringUtil::I8_toBinary(GMSEC_I8 value)
{
	unsigned char bytes[sizeof(value)];
	char* bytesTmp = reinterpret_cast<char*>(bytes);

	Encoder::getEncoder()->putI8(&value, bytesTmp);

	return Data(reinterpret_cast<GMSEC_U8*>(bytes), sizeof(bytes));
}


StringUtil::Data StringUtil::I16_toBinary(GMSEC_I16 value)
{
	unsigned char bytes[sizeof(value)];
	char* bytesTmp = reinterpret_cast<char*>(bytes);

	Encoder::getEncoder()->putI16(&value, bytesTmp);

	return removeLeadingZeros( Data(reinterpret_cast<GMSEC_U8*>(bytes), sizeof(bytes)) );
}


StringUtil::Data StringUtil::I32_toBinary(GMSEC_I32 value)
{
	unsigned char bytes[sizeof(value)];
	char* bytesTmp = reinterpret_cast<char*>(bytes);

	Encoder::getEncoder()->putI32(&value, bytesTmp);

	return removeLeadingZeros( Data(reinterpret_cast<GMSEC_U8*>(bytes), sizeof(bytes)) );
}


StringUtil::Data StringUtil::I64_toBinary(GMSEC_I64 value)
{
	unsigned char bytes[sizeof(value)];
	char* bytesTmp = reinterpret_cast<char*>(bytes);

	Encoder::getEncoder()->putI64(&value, bytesTmp);

	return removeLeadingZeros( Data(reinterpret_cast<GMSEC_U8*>(bytes), sizeof(bytes)) );
}


StringUtil::Data StringUtil::U8_toBinary(GMSEC_U8 value)
{
	unsigned char bytes[sizeof(value)];
	char* bytesTmp = reinterpret_cast<char*>(bytes);

	Encoder::getEncoder()->putU8(&value, bytesTmp);

	return Data(reinterpret_cast<GMSEC_U8*>(bytes), sizeof(bytes));
}


StringUtil::Data StringUtil::U16_toBinary(GMSEC_U16 value)
{
	unsigned char bytes[sizeof(value)];
	char* bytesTmp = reinterpret_cast<char*>(bytes);

	Encoder::getEncoder()->putU16(&value, bytesTmp);

	return removeLeadingZeros( Data(reinterpret_cast<GMSEC_U8*>(bytes), sizeof(bytes)) );
}


StringUtil::Data StringUtil::U32_toBinary(GMSEC_U32 value)
{
	unsigned char bytes[sizeof(value)];
	char* bytesTmp = reinterpret_cast<char*>(bytes);

	Encoder::getEncoder()->putU32(&value, bytesTmp);

	return removeLeadingZeros( Data(reinterpret_cast<GMSEC_U8*>(bytes), sizeof(bytes)) );
}


StringUtil::Data StringUtil::U64_toBinary(GMSEC_U64 value)
{
	unsigned char bytes[sizeof(value)];
	char* bytesTmp = reinterpret_cast<char*>(bytes);

	Encoder::getEncoder()->putU64(&value, bytesTmp);

	return removeLeadingZeros( Data(reinterpret_cast<GMSEC_U8*>(bytes), sizeof(bytes)) );
}


StringUtil::Data StringUtil::F32_toBinary(GMSEC_F32 value)
{
	unsigned char bytes[sizeof(value)];
	char* bytesTmp = reinterpret_cast<char*>(bytes);

	Encoder::getEncoder()->putF32(&value, bytesTmp);

	return Data(reinterpret_cast<GMSEC_U8*>(bytes), sizeof(bytes));
}


StringUtil::Data StringUtil::F64_toBinary(GMSEC_F64 value)
{
	unsigned char bytes[sizeof(value)];
	char* bytesTmp = reinterpret_cast<char*>(bytes);

	Encoder::getEncoder()->putF64(&value, bytesTmp);

	return Data(reinterpret_cast<GMSEC_U8*>(bytes), sizeof(bytes));
}


GMSEC_I32 StringUtil::I32_fromBinary(const Data& data)
{
	Data temp = data;

	if (data.length() < sizeof(GMSEC_I32))
	{
		temp = padWithLeadingZeros(data, sizeof(GMSEC_I32) - data.length());
	}

	const char* blob = (const char*) temp.data();
	GMSEC_I32   value;

	Decoder::getDecoder()->getI32(blob, &value);

	return value;
}


GMSEC_I64 StringUtil::I64_fromBinary(const Data& data)
{
	Data temp = data;

	if (data.length() < sizeof(GMSEC_I64))
	{
		temp = padWithLeadingZeros(data, sizeof(GMSEC_I64) - data.length());
	}

	const char* blob = (const char*) temp.data();
	GMSEC_I64   value;

	Decoder::getDecoder()->getI64(blob, &value);

	return value;
}


GMSEC_U32 StringUtil::U32_fromBinary(const Data& data)
{
	Data temp = data;

	if (data.length() < sizeof(GMSEC_U32))
	{
		temp = padWithLeadingZeros(data, sizeof(GMSEC_U32) - data.length());
	}

	const char* blob = (const char*) temp.data();
	GMSEC_U32   value;

	Decoder::getDecoder()->getU32(blob, &value);

	return value;
}


GMSEC_U64 StringUtil::U64_fromBinary(const Data& data)
{
	Data temp = data;

	if (data.length() < sizeof(GMSEC_U64))
	{
		temp = padWithLeadingZeros(data, sizeof(GMSEC_U64) - data.length());
	}

	const char* blob = (const char*) temp.data();
	GMSEC_U64   value;

	Decoder::getDecoder()->getU64(blob, &value);

	return value;
}


GMSEC_F64 StringUtil::F64_fromBinary(const Data& data)
{
	Data temp = data;

	if (data.length() < sizeof(GMSEC_F64))
	{
		GMSEC_WARNING << "Binary data does not represent a GMSEC_F64 value";
		temp = padWithLeadingZeros(data, sizeof(GMSEC_F64) - data.length());
	}

	const char* blob = (const char*) temp.data();
	GMSEC_F64   value;

	Decoder::getDecoder()->getF64(blob, &value);

	return value;
}


const char* StringUtil::xmlErrorToString(tinyxml2::XMLError error)
{
	switch (error)
	{
	case tinyxml2::XML_NO_ERROR: return "No error";
	case tinyxml2::XML_NO_ATTRIBUTE: return "No attribute";
	case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE: return "Wrong attribute type";
	case tinyxml2::XML_ERROR_FILE_NOT_FOUND: return "File not found";
	case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED: return "File could not be opened";
	case tinyxml2::XML_ERROR_FILE_READ_ERROR: return "File read error";
	case tinyxml2::XML_ERROR_ELEMENT_MISMATCH: return "Element mismatch";
	case tinyxml2::XML_ERROR_PARSING_ELEMENT: return "Error parsing element";
	case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE: return "Error parsing attribute";
	case tinyxml2::XML_ERROR_IDENTIFYING_TAG: return "Error identifying tag";
	case tinyxml2::XML_ERROR_PARSING_TEXT: return "Error parsing text";
	case tinyxml2::XML_ERROR_PARSING_CDATA: return "Error parsing CDATA";
	case tinyxml2::XML_ERROR_PARSING_COMMENT: return "Error parsing comment";
	case tinyxml2::XML_ERROR_PARSING_DECLARATION: return "Error parsing declaration";
	case tinyxml2::XML_ERROR_PARSING_UNKNOWN: return "Error parsing unknown";
	case tinyxml2::XML_ERROR_EMPTY_DOCUMENT: return "Empty document";
	case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT: return "Mismatched element";
	case tinyxml2::XML_ERROR_PARSING: return "Error parsing";
	case tinyxml2::XML_CAN_NOT_CONVERT_TEXT: return "Can not convert text";
	case tinyxml2::XML_NO_TEXT_NODE: return "No text node";
	}

	return "Unknown error";
}

} // namespace util
} // namespace api5
} // namespace gmsec
