/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





// Preprocessor definition to ignore Visual Studio warnings
// indicating that *_s functions should be used instead.
#define _CRT_SECURE_NO_DEPRECATE

#include <gmsec/util/String.h>
#include <cstring>
#ifdef __sun
#include <string.h>
#endif


namespace gmsec
{
namespace util
{

String::String()
	: str_len(0),
	data(new char[str_len + 1])
{
	if(data)
	{
		data[str_len] = '\0';
	}
}

String::String(const String& instring)
	: str_len(instring.str_len),
	data(new char[str_len + 1])
{
	if(data)
	{
		std::strncpy(data, instring.c_str(), str_len);
		data[str_len] = '\0';
	}
}

String::String(const char* instring)
	: str_len(0),
	data(0)
{
	if(instring != 0)
	{
		str_len = std::strlen(instring);
	}

	data = new char[str_len + 1];
	
	if(data)
	{
		std::strncpy(data, instring, str_len);
		data[str_len] = '\0';
	}
}

String::~String()
{
	if(data)
	{
		delete[] data;
	}
}

String& String::operator=(const String& s)
{
	if(this != &s)
	{
		str_len = s.str_len;
		const char* newstring = s.c_str();

		delete[] data;

		data = new char[str_len + 1];
		if(data)
		{
			for(size_t i = 0; i < (str_len + 1); i++)
			{
				data[i] = newstring[i];
			}
		}
	}

	return *this;
}

String& String::operator+=(const String& s)
{
	size_t orig_str_len          = str_len;
	char*  orig_string           = data;

	size_t      str_len_to_append = s.str_len;
	const char* string_to_append = s.c_str();

	str_len += str_len_to_append;
	data = new char[str_len + 1];

	if(data)
	{
		size_t i = 0;
		while (i < orig_str_len)
		{
			data[i] = orig_string[i];
			i++;
		}
		while (i < str_len)
		{
			data[i] = string_to_append[i-orig_str_len];
			i++;
		}

		data[str_len] = '\0';

	}

	delete[] orig_string;

	return *this;
}

bool String::operator==(const String& s) const
{
	return std::strncmp(data, s.c_str(), str_len) == 0;
}

bool String::operator!=(const String& s) const
{
	return std::strncmp(data, s.c_str(), str_len) != 0;
}

size_t String::size() const
{
	return str_len;
}	

size_t String::length() const
{
	return str_len;
}

void String::clear()
{
	delete[] data;

	str_len = 0;
	data = new char[str_len + 1];
	
	if(data)
	{
		data[str_len] = '\0';
	}
}

bool String::empty() const
{
	return data[0] == '\0';
}

const char* String::c_str() const
{
	return data;
}

void String::assign(const char* instring)
{
	str_len = std::strlen(instring);

	delete[] data;

	data = new char[str_len + 1];
	if(data)
	{
		for(size_t i = 0; i < (str_len + 1); i++)
		{
			data[i] = instring[i];
		}
	}
}

String String::substr(size_t startingIndex,
                      size_t length)
{
	String ret_string;

	delete[] ret_string.data; // Can operate on private member of same class

	ret_string.data    = new char[length+1];
	ret_string.str_len = length;

	strncpy(ret_string.data,
	        data+startingIndex,
                length);

	ret_string.data[length] = '\0';

	return ret_string; // Note:  ret_string is destroyed (since it goes
	                   //        out-of-scope) only *after* the return has
	                   //        occurred and the object value has
	                   //        been passed to the return assignment.

}

}//namespace util
}//namespace gmsec
