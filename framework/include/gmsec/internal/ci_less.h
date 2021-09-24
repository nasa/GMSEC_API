
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file ci_less.h
 *
 *  @brief This file contains a stl case insensitive less compare function.
 *
 *
**/
#ifndef ci_less_h
#define ci_less_h

#include <string>
#include <map>

// case-independent (ci) string less_than
// returns true if s1 < s2
struct ci_less : std::binary_function<std::string, std::string, bool>
{

	// case-independent (ci) compare_less binary function
	struct nocase_compare : public std::binary_function<unsigned char,unsigned char,bool>
	{
		bool operator()(const unsigned char& c1, const unsigned char& c2) const
		{
			return tolower(c1) < tolower(c2);
		}
	};

	bool operator()(const std::string & s1, const std::string & s2) const
	{

		return std::lexicographical_compare
		       (s1.begin(), s1.end(),     // source range
		        s2.begin(), s2.end(),     // dest range
		        nocase_compare());   // comparison
	}
}; // end of ci_less

#endif // ci_less_h
