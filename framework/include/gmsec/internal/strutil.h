
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#ifndef gmsec_internal_strutil_h
#define gmsec_internal_strutil_h

#include <gmsec_defs.h>
#include <gmsec/Status.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec/util/Mutex.h>
#include <gmsec/util/Buffer.h>

#include <string.h>
#include <string>
#include <vector>
#include <cstdarg>


namespace gmsec
{
namespace util
{

enum STR2NUM_ERROR { STR2NUM_SUCCESS, STR2NUM_OVERFLOW, STR2NUM_UNDERFLOW, STR2NUM_INCONVERTIBLE };

#ifdef __sun
using std::va_list;
#endif


/**
* \fn stringLength (const char *s)
* \brief Returns the length of the string
*/
GMSEC_API size_t CALL_TYPE stringLength(const char *s);

/**
* \fn stringEquals (const char *s1, const char *s2)
* \brief Returns true if the two strings are the same
*/
GMSEC_API bool CALL_TYPE stringEquals(const char *s1, const char *s2);

/**
* \fn stringEqualsIgnoreCase (const char *s1, const char *s2)
* \brief Returns true if the two strings are the same ignoring case
*/
GMSEC_API bool CALL_TYPE stringEqualsIgnoreCase(const char *s1, const char *s2);

/**
* \fn stringEqualsCount (const char *s1, const char *s2)
* \brief Returns true if the two strings are the same for count bytes.
*/
GMSEC_API int CALL_TYPE stringCompareCount(const char *s1, const char *s2, int count);

/**
* \fn stringNew (const char *s)
* \brief Returns a C string copy of s allocated with new char[].
*/
GMSEC_API char * CALL_TYPE stringNew(const char *s);

/**
* \fn stringDestroy (const char *s)
* \brief Deallocates a C string which was allocated with stringNew.
*/
GMSEC_API void CALL_TYPE stringDestroy(const char *s);

/**
* \fn stringCopy (char *dest, const char *src, size_t destSize)
* \brief Copies up to destSize bytes from src to dest ensuring dest is null-terminated.
* If the size (including null terminator) of src is known, xStringCopy is more efficient.
*/
GMSEC_API void CALL_TYPE stringCopy(char *dest, const char *src, size_t destSize);

/**
* \fn xStringCopy (char *dest, size_t destSize, const char *src, size_t srcSize)
* \brief Copies up to destSize bytes from src to dest ensuring dest is null-terminated.
* @destSize and @srcSize include the null terminator
*/
GMSEC_API void CALL_TYPE xStringCopy(char *dest, size_t destSize, const char *src, size_t srcSize);


/**
* \fn copyBytes (char *dest, const char *src, size_t count)
* \brief Copies count bytes from src to dest.
*/
GMSEC_API void CALL_TYPE copyBytes(void *dest, const void *src, size_t count);

/**
* \fn split(string str, char delimiter)
* \brief Splits a string and returns a vector of resulting strings.
*/
GMSEC_API std::vector<std::string> CALL_TYPE split(const std::string& str, const char delimiter);

/**
* \fn trim(string str, char trim)
* \brief Trims leading and trailing instances of a character from a string and returns the result.
*/
GMSEC_API std::string CALL_TYPE trim(const std::string& str, const char chop = ' ');

/**
* \fn stringFormat (char *buffer, int size, const char *format, ...);
* \brief Applies format to variable argument list putting the result
* to buffer of specified size.
*/
GMSEC_API void CALL_TYPE stringFormat(char *dest, int size, const char *format, ...);

/**
* \fn stringFormat (char *buffer, int size, const char *format, va_list args);
* \brief Similar to stringFormat but takes a va_list as the input argument
*/
GMSEC_API void CALL_TYPE stringFormatV(char * dest, int space, const char *format, va_list args);

/**
* \fn getTimeFromString (const char * timeString);
* \brief Returns a double representing the seconds since the Epoch
* from a string
*/
GMSEC_API double CALL_TYPE getTimeFromString(const std::string& timeString);

/**
* \fn stringIsTrue (const char *s)
* \brief Determines if the input string corresponds to GMSEC_TRUE.
* If the input is "TRUE" or "YES" ignoring case, the result is GMSEC_TRUE.
*/
GMSEC_API bool CALL_TYPE stringIsTrue(const char * s);


/**
* \fn stringParseI32 (const char *in, GMSEC_I32 &out)
* \brief Parses an I32 from the input.
* Returns true if successful.
*/
GMSEC_API bool CALL_TYPE stringParseI32(const char * in, GMSEC_I32 &out);

/**
* \fn stringParseI32 (const char *in, GMSEC_I32 &out)
* \brief Parses an I32 from the input.
* Returns true if successful.
*/
GMSEC_API bool CALL_TYPE stringParseF64(const char * in, GMSEC_F64 &out);

/**
* \fn stringToLower (const string &in);
* \brief Convert a string to lower case.
*/
GMSEC_API std::string CALL_TYPE stringToLower(const std::string &in);

/**
* \fn stringToUpper (const string &in);
* \brief Convert a string to upper case.
*/
GMSEC_API std::string CALL_TYPE stringToUpper(const std::string &in);

/**
* \fn stripWhiteSpace(const string &in);
* \brief Strips away leading and trailing white-space characters from the given string.
*/
GMSEC_API std::string CALL_TYPE stripWhiteSpace(const std::string &in);

/**
 * \fn str2int (int &i, const char *s, int base)
 * \brief Convert a c-style string to an int
 */
GMSEC_API STR2NUM_ERROR str2int (int &i, char const *s, int base = 0);

/**
 * \fn str2longlong (long long &l, const char *s, int base)
 * \brief Convert a c-style string to a long long
 */
GMSEC_API STR2NUM_ERROR str2longlong (long long &l, char const *s, int base = 0);

/**
* \fn convertBase16ToBlob (const string &in, DataBuffer &out);
* \brief Convert a base 16 string to a blob.
*/
GMSEC_API bool CALL_TYPE convertBase16ToBlob(const std::string &in, DataBuffer &out);

/**
* \fn convertBlobToBase16 (const DataBuffer &in, string &out);
* \brief Convert a blob to a base 16 string.
*/
GMSEC_API bool CALL_TYPE convertBlobToBase16(const DataBuffer &in, std::string &out);

/**
 * @fn getSharedMutex ()
 * Access a mutex for quick initialization purposes.
 * The function needs a proper home.
 */
GMSEC_API gmsec::util::Mutex & CALL_TYPE getSharedMutex();

GMSEC_API void logStack(const char *label, const char *file, int line);

GMSEC_API Status gmsec_compress(const DataBuffer& in, DataBuffer& out);
GMSEC_API Status gmsec_uncompress(const DataBuffer& in, DataBuffer& out);

} // namespace util
} // namespace gmsec


#endif /* gmsec_internal_strutil_h */
