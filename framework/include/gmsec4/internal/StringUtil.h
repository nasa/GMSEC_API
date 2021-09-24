/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_STRING_UTIL_H
#define GMSEC_API_INTERNAL_STRING_UTIL_H

#include <gmsec4_defs.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/Mutex.h>
#include <gmsec4/util/Buffer.h>
#include <gmsec4/util/wdllexp.h>

#include <string>
#include <sstream>
#include <typeinfo>
#include <vector>

#include <cstdarg>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <cmath>


namespace gmsec
{
namespace api
{
namespace util
{

#ifdef __sun
using std::va_list;
#endif


class GMSEC_API StringConverter
{
public:
	enum Mode
	{
		NO_CONVERSION,
		TO_UPPERCASE,
		TO_LOWERCASE
	};

	static StringConverter& CALL_TYPE instance();

	void CALL_TYPE setMode(Mode mode);

	Mode CALL_TYPE getMode() const;

	std::string CALL_TYPE convertString(const std::string& str) const;

	std::string CALL_TYPE convertString(const char* str) const;

private:
	StringConverter();

	Mode m_mode;
};


class GMSEC_API StringUtil
{
public:
	enum STR2NUM_ERROR { STR2NUM_SUCCESS, STR2NUM_OVERFLOW, STR2NUM_UNDERFLOW, STR2NUM_INCONVERTIBLE };


	/**
	 * @fn stringLength(const char* s)
	 * @brief Returns the length of the string
	 */
	static size_t CALL_TYPE stringLength(const char* s);


	/**
	 * @fn stringEquals(const char* s1, const char* s2)
	 * @brief Returns true if the two strings are the same
	 */
	static bool CALL_TYPE stringEquals(const char* s1, const char* s2);


	/**
	 * @fn stringEqualsIgnoreCase(const char* s1, const char* s2)
	 * @brief Returns true if the two strings are the same ignoring case
	 */
	static bool CALL_TYPE stringEqualsIgnoreCase(const char* s1, const char* s2);


	/**
	 * @fn stringEqualsCount(const char* s1, const char* s2)
	 * @brief Returns true if the two strings are the same for count bytes.
	 */
	static int CALL_TYPE stringCompareCount(const char* s1, const char* s2, int count);


	/**
	 * @fn stringNew(const char* s)
	 * @brief Returns a C string copy of s allocated with new char[].
	 */
	static char* CALL_TYPE stringNew(const char* s);


	/**
	 * @fn stringDestroy(const char* s)
	 * @brief Deallocates a C string which was allocated with stringNew.
	 */
	static void CALL_TYPE stringDestroy(const char* s);


	/**
	 * @fn stringCopy(char* dest, const char* src, size_t destSize)
	 * @brief Copies up to destSize bytes from src to dest ensuring dest is null-terminated.
	 * If the size (including null terminator) of src is known, xStringCopy is more efficient.
	 */
	static void CALL_TYPE stringCopy(char* dest, const char* src, size_t destSize);


	/**
	 * @fn xStringCopy(char* dest, size_t destSize, const char* src, size_t srcSize)
	 * @brief Copies up to destSize bytes from src to dest ensuring dest is null-terminated.
	 * destSize and srcSize include the null terminator
	 */
	static void CALL_TYPE xStringCopy(char* dest, size_t destSize, const char* src, size_t srcSize);


	/**
	 * @fn copyBytes(char* dest, const char* src, size_t count)
	 * @brief Copies count bytes from src to dest.
	 */
	static void CALL_TYPE copyBytes(void* dest, const void* src, size_t count);

	/**
	 * @fn split(std::string str, char delimiter)
	 * @brief Splits a string and returns a vector of resulting strings.
	 */
	static std::vector<std::string> CALL_TYPE split(const std::string& str, const char delimiter);


	/**
	* @fn split(std::string str, char delimiter)
	* @brief Splits a string and returns a vector of resulting strings.
	*/
	static std::vector<std::string> CALL_TYPE split(const std::string& str, const std::string& delimiter);


	/**
	 * @fn ltrim(const std::string& str)
	 * @brief Strips the the left side of the given string of any white-space characters (spaces, \f, \n, \r, \t and \v).
	 */
	static std::string ltrim(const std::string& str);


	/**
	 * @fn rtrim(const std::string& str)
	 * @brief Strips the the right side of the given string of any white-space characters (spaces, \f, \n, \r, \t and \v).
	 */
	static std::string rtrim(const std::string& str);


	/**
	 * @fn trim(const std::string& str)
	 * @brief Strips both sides of the given string of any white-space characters (spaces, \f, \n, \r, \t and \v).
	 */
	static std::string trim(const std::string& str);


	/**
	 * @fn trim(std::string str, char trim)
	 * @brief Trims leading and trailing instances of a character from a string and returns the result.
	 */
	static std::string CALL_TYPE trim(const std::string& str, const char trim);


	/**
	 * @fn stringFormat(char* buffer, int size, const char* format, ...);
	 * @brief Applies format to variable argument list putting the result to buffer of specified size.
	 */
	static void CALL_TYPE stringFormat(char* dest, int size, const char* format, ...);


	/**
	 * @fn stringFormat(char* buffer, int size, const char* format, va_list args);
	 * @brief Similar to stringFormat but takes a va_list as the input argument
	 */
	static void CALL_TYPE stringFormatV(char* dest, int space, const char* format, va_list args);


	/**
	 * @fn getTimeFromString(const char* timeString);
	 * @brief Returns a double representing the seconds since the Epoch from a string
	 */
	static double CALL_TYPE getTimeFromString(const std::string& timeString);


	/**
	 * @fn stringIsTrue(const char* s)
	 * @brief Determines if the input string corresponds to GMSEC_TRUE.
	 * If the input is "TRUE" or "YES" ignoring case, the result is GMSEC_TRUE.
	 */
	static bool CALL_TYPE stringIsTrue(const char* s);


	/**
	 * @fn stringParseI32(const char* in, GMSEC_I32& out)
	 * @brief Parses an I32 from the input.
	 * Returns true if successful.
	 *
	 * @deprecated - Use getSignedValue() instead.
	 */
	static bool CALL_TYPE stringParseI32(const char* in, GMSEC_I32& out);


	/**
	 * \fn stringParseF32(const char* in, GMSEC_F32& out)
	 * \brief Parses an F32 from the input.
	 * Returns true if successful.
	 *
	 * @deprecated - Use getFloat() instead.
	 */
	static bool CALL_TYPE stringParseF32(const char* in, GMSEC_F32& out);


	/**
	 * \fn stringParseF64(const char* in, GMSEC_F64& out)
	 * \brief Parses an F64 from the input.
	 * Returns true if successful.
	 *
	 * @deprecated - Use getDouble() instead.
	 */
	static bool CALL_TYPE stringParseF64(const char* in, GMSEC_F64& out);


	/**
	 * @fn stringToLower(const std::string& in);
	 * @brief Convert a string to lower case.
	 */
	static std::string CALL_TYPE stringToLower(const std::string& in);


	/**
	 * @fn stringToUpper(const std::string& in);
	 * @brief Convert a string to upper case.
	 */
	static std::string CALL_TYPE stringToUpper(const std::string& in);


	/**
	 * \fn stripWhiteSpace(const string& in);
	 * \brief Strips away leading and trailing white-space characters from the given string.
	 */
	static std::string CALL_TYPE stripWhiteSpace(const std::string& in);


	/**
	 * @fn str2int(int& i, const char* s, int base)
	 * @brief Convert a c-style string to an int
	 *
	 * @deprecated - Use getSignedValue() instead.
	 */
	static StringUtil::STR2NUM_ERROR str2int(int& i, char const* s, int base = 0);


	/**
	 * @fn str2longlong(long long& l, const char* s, int base)
	 * @brief Convert a c-style string to a long long
	 *
	 * @deprecated - Use getUnsignedValue() instead.
	 */
	static StringUtil::STR2NUM_ERROR str2longlong(long long& l, char const* s, int base = 0);


	/**
	 * @fn convertBase16ToBlob(const std::string& in, DataBuffer& out);
	 * @brief Convert a base 16 string to a blob.
	 */
	static bool CALL_TYPE convertBase16ToBlob(const std::string& in, DataBuffer& out);


	/**
	 * @fn convertBlobToBase16(const DataBuffer& in, std::string& out);
	 * @brief Convert a blob to a base 16 string.
	 */
	static bool CALL_TYPE convertBlobToBase16(const DataBuffer& in, std::string& out);


	/**
	 * @fn getSharedMutex ()
	 * Access a mutex for quick initialization purposes.
	 * The function needs a proper home.
	 */
	static gmsec::api::util::Mutex& CALL_TYPE getSharedMutex();


	/**
	 * @fn logStack(const char* label, const char* file, int line)
	 */
	static void logStack(const char* label, const char* file, int line);


	/**
	 * @fn gmsec_compress(const gmsec::api::util::DataBuffer& in, gmsec::api::util::DataBuffer& out)
	 * @brief Compress the data in the given input buffer and place the result in the output buffer.
	 */
	static Status gmsec_compress(const gmsec::api::util::DataBuffer& in, gmsec::api::util::DataBuffer& out);


	/**
	 * @fn gmsec_uncompress(const gmsec::api::util::DataBuffer& in, gmsec::api::util::DataBuffer& out)
	 * @brief Uncompress the data in the given input buffer and place the result in the output buffer.
	 */
	static Status gmsec_uncompress(const gmsec::api::util::DataBuffer& in, gmsec::api::util::DataBuffer& out);


	/**
	 * @fn getBoolean(const char* value)
	 * @brief Validates and converts (if possible) the given value to a bool value.
	 * @throws An Exception is thrown if the value is NULL, or if it does not represent a bool
	 * value.
	 */
	static bool getBoolean(const char* value);


	/**
	 * @fn getValue(const char* value)
	 * @brief Converts the given value, if possible, into the desired type T.
	 * @throws An Exception is thrown if the data value cannot be converted.
	 */
	template<typename T>
	static T getValue(const char* value)
	{
		if (value == NULL)
		{
			throw Exception(OTHER_ERROR, PARSE_ERROR, "Value cannot be NULL");
		}
		else
		{
			T result = T();

			std::istringstream iss(trim(value));

			iss >> result;    // attempt to convert string to desired type T

#if defined(__APPLE__)
			if ((typeid(T) == typeid(GMSEC_F32) || typeid(T) == typeid(GMSEC_F64)) && !std::isfinite(result))
			{
				throw Exception(OTHER_ERROR, PARSE_ERROR, "Unable to convert value");
			}
#endif

			if (iss.fail())
			{
				throw Exception(OTHER_ERROR, PARSE_ERROR, "Unable to convert value");
			}

			if (!iss.eof())
			{
				// Failed to convert; check if we are dealing with a GMSEC_CHAR type.
				// If so, handle as special case.
				if (typeid(T) == typeid(GMSEC_CHAR))
				{
					iss.unget();

					if (iss.str().length() == 1)
					{
						result = iss.str()[0];
					}
					else
					{
						throw Exception(OTHER_ERROR, PARSE_ERROR, "Unable to convert value");
					}
				}
				else
				{
					throw Exception(OTHER_ERROR, PARSE_ERROR, "Unable to convert value");
				}
			}

			return result;
		}
	}


	/**
	 * @fn std::string toXML(const char* data)
	 * @brief Converts the data string to a legal XML representation.
	 * @param data - null-terminated string containing raw data
	 * @throws An Exception is thrown if the data value is NULL.
	 */
	static std::string toXML(const char* data);


	/**
	 * @fn std::string toJSON(const char* data)
	 * @brief Converts the data string to a legal JSON representation.
	 * @param data - null-terminated string containing raw data
	 * @throws An Exception is thrown if the data value is NULL.
	 */
	static std::string toJSON(const char* data);


	/**
	 * @fn bool isLeapYear(unsigned int year)
	 * @brief Reports whether the given year is a leap-year or not.
	 */
	static bool isLeapYear(unsigned int year);


	/**
	 * @fn bool isValidHeaderString(const std::string& fieldValue)
	 * @brief Reports whether the given string is a valid header string.
	 * @param value - the string to check
	 */
	static bool isValidHeaderString(const std::string& value);


	/**
	 * @fn bool isValidTimestamp(const std::string& fieldValue)
	 * @brief Reports whether the given time string is valid.
	 * @param value - the string to check
	 */
	static bool isValidTimestamp(const std::string& value);


	/**
	 * @fn bool isValidIpAddress(const std::string& fieldValue)
	 * @brief Reports whether the given string contains a valid IPv4 or IPv6 address.
	 * @param value - the string to check
	 */
	static bool isValidIpAddress(const std::string& value);


	/**
	 * @brief Typedef to represent raw data.
	 */
	typedef std::basic_string<unsigned char> Data;


	/**
	 * @fn Data removeLeadingZeros(const Data& data)
	 * @brief Removes leading zeros (at the byte boundary) from the given data.
	 * @param data - the data from which to strip leading zero byte values.
	 */
	static Data removeLeadingZeros(const Data& data);


	/**
	 * @fn Data padWithLeadingZeros(const Data& data, size_t numZeros)
	 * @brief Places leading zeros (at the byte boundary) within a copy of the given data.
	 * @param data - the original data to which to add zeros
	 * @param numZeros - the number of zeros to add
	 */
	static Data padWithLeadingZeros(const Data& data, size_t numZeros);


	/**
	 * @fn Data string_toBinary(const char* value)
	 * @brief Converts string value to raw data blob.
	 * @param value - string to convert
	 */
	static Data string_toBinary(const char* value);


	/**
	 * @fn Data binaryString_toBinary(const char* value)
	 * @brief Converts string representing a binary value to raw data blob.
	 * @param value - binary string to convert
	 */
	static Data binaryString_toBinary(const char* value);


	/**
	 * @fn Data I8_toBinary(GMSEC_I8 value)
	 * @brief Converts a GMSEC_I8 value to raw data blob.
	 * @param value - number to convert
	 */
	static Data I8_toBinary(GMSEC_I8 value);


	/**
	 * @fn Data I16_toBinary(GMSEC_I16 value)
	 * @brief Converts a GMSEC_I16 value to raw data blob.
	 * @param value - number to convert
	 */
	static Data I16_toBinary(GMSEC_I16 value);


	/**
	 * @fn Data I32_toBinary(GMSEC_I32 value)
	 * @brief Converts a GMSEC_I32 value to raw data blob.
	 * @param value - number to convert
	 */
	static Data I32_toBinary(GMSEC_I32 value);


	/**
	 * @fn Data I64_toBinary(GMSEC_I64 value)
	 * @brief Converts a GMSEC_I64 value to raw data blob.
	 * @param value - number to convert
	 */
	static Data I64_toBinary(GMSEC_I64 value);


	/**
	 * @fn Data U8_toBinary(GMSEC_U8 value)
	 * @brief Converts a GMSEC_U8 value to raw data blob.
	 * @param value - number to convert
	 */
	static Data U8_toBinary(GMSEC_U8 value);


	/**
	 * @fn Data U16_toBinary(GMSEC_U16 value)
	 * @brief Converts a GMSEC_U16 value to raw data blob.
	 * @param value - number to convert
	 */
	static Data U16_toBinary(GMSEC_U16 value);


	/**
	 * @fn Data U32_toBinary(GMSEC_U32 value)
	 * @brief Converts a GMSEC_U32 value to raw data blob.
	 * @param value - number to convert
	 */
	static Data U32_toBinary(GMSEC_U32 value);


	/**
	 * @fn Data U64_toBinary(GMSEC_U64 value)
	 * @brief Converts a GMSEC_U64 value to raw data blob.
	 * @param value - number to convert
	 */
	static Data U64_toBinary(GMSEC_U64 value);


	/**
	 * @fn Data F32_toBinary(GMSEC_F32 value)
	 * @brief Converts a GMSEC_F32 value to raw data blob.
	 * @param value - number to convert
	 */
	static Data F32_toBinary(GMSEC_F32 value);


	/**
	 * @fn Data F64_toBinary(GMSEC_F64 value)
	 * @brief Converts a GMSEC_F64 value to raw data blob.
	 * @param value - number to convert
	 */
	static Data F64_toBinary(GMSEC_F64 value);


	/**
	 * @fn GMSEC_I64 I64_fromBinary(const Data& data)
	 * @brief Converts the given data blob into a GMSEC_I64 value.
	 * @param data - Binary data blob
	 */
	static GMSEC_I64 I64_fromBinary(const Data& data);


	/**
	 * @fn GMSEC_U64 U64_fromBinary(const Data& data)
	 * @brief Converts the given data blob into a GMSEC_U64 value.
	 * @param data - Binary data blob
	 */
	static GMSEC_U64 U64_fromBinary(const Data& data);


	/**
	 * @fn GMSEC_F64 F64_fromBinary(const Data& data)
	 * @brief Converts the given data blob into a GMSEC_F64 value.
	 * @param data - Binary data blob
	 */
	static GMSEC_F64 F64_fromBinary(const Data& data);
};


} // namespace util
} // namespace api
} // namespace gmsec


#endif
