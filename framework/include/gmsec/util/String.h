/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file String.h
 *
 *  @brief This file contains a class for cross platform List support
**/

#ifndef gmsec_util_String_h
#define gmsec_util_String_h

#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>
#include <stddef.h>

namespace gmsec
{
namespace util
{

/**	@class String
 *
 *	@brief This provides a basic String type, with an API like the C++ std::string type
 *
 *
**/
class GMSEC_API String 
{
public:

	/* @fn String()
	*
	*	@brief Standard constructor. 
	*
	*/
	GMSEC_DEPRECATED String();

	/* @fn String(const String instring)
 	*
 	* 	@brief String constructor 
 	*
 	* 	@param instring - The String that will be copied into the new String
 	*/
	GMSEC_DEPRECATED String(const String& instring);

	/* @fn String(const char* instring)
 	*
 	* 	@brief const char* constructor 
 	*
 	* 	@param instring - The string that will be copied into the new String
 	*/
	GMSEC_DEPRECATED String(const char* instring);

	/* @fn ~String()
 	*
 	* 	@brief destructor 
 	*
 	*/
	~String();

	/* @fn operator=(const String& s)
 	* 
 	* 	@brief Assigns a new value to the String, replacing its value
 	*
 	*	@param s - The String value being assigned to this String
 	*
 	*/ 
	String& operator=(const String& s);
	
	/* @fn operator+=(const String& s)
 	* 
 	* 	@brief Appends a string to the String, replacing its value.
 	*
 	*	@param s - The String value being appended to this String
 	*
 	*/ 
	String& operator+=(const String& s);

	/* @fn operator==(const String& s)
 	* 
 	* 	@brief Compares this String with another String, returns a bool
 	* 	value based on the result
 	*
 	*	@param s - The String value being compared to this String
 	*
 	*/ 
	bool operator==(const String& s) const;

	/* @fn operator!=(const String& s)
 	* 
 	* 	@brief Compares this String with another String, returns a bool
 	* 	       which is the opposite of operater==() (operator==()
 	* 	       returns true if the strings have equal content;
 	* 	       operator!=() returns true if the string do not).
 	*
 	*	@param s - The String value being compared to this String
 	*
 	*/ 
	bool operator!=(const String& s) const;

	/* @fn size()
 	* 
 	* 	@brief This function returns an integer representation of the length of the String
 	*
 	*/ 
	size_t size() const;

	/* @fn length()
 	* 
 	* 	@brief This function returns an integer representation of the length of the String
 	*
 	*/ 
	size_t length() const;

	/* @fn clear()
 	* 
 	* 	@brief This function erases the contents of the String
 	*
 	*/ 
	void clear();

	/* @fn empty()
 	* 
 	* 	@brief This function tests if the String is empty and returns a bool value of
 	* 	true if its length is 0, false if it is not
 	*
 	*/ 
	bool empty() const;

	/* @fn c_str()
 	* 
 	* 	@brief This function returns a pointer to a null-terminated sequence of characters representing
 	* 	the current value of the String
 	*
 	*/ 
	const char* c_str() const;

	/* @fn assign(const char* instring)
 	* 
 	*	@brief This function takes in a const char* value to assign as the String value.
 	*
 	*	@param instring - The const char* value that will be assigned as the new String value.
 	*
 	*/
	void assign(const char* instring);

	/* @fn substr(size_t startingIndex, size_t length)
 	* 
 	*	@brief This function returns a gmsec::util::String containing
 	*	a substring of this String, specified by the startingIndex
 	*	and length.
 	*
 	*	@param startingIndex - Position (starting at index 0 like
 	*	                       an ordinary C string) to start the
 	*	                       substring.
 	*
 	*	@param length - Length of the substring to return.
 	*
 	*/
	String substr(size_t startingIndex, size_t length);

private:
	size_t str_len;
	char* data;
};

}//namespace util
}//namespace gmsec
#endif
