/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file ci_less.h
 *
 *  @brief This file contains a stl case insensitive less compare function.
 */

#ifndef GMSEC_API5_INTERNAL_CI_LESS_H
#define GMSEC_API5_INTERNAL_CI_LESS_H

#include <string>


namespace gmsec {
namespace api5 {
namespace internal {


/**
 * @struct ci_less
 *
 * @brief Case-independent (ci) string less_than
 *
 * @return Returns true if s1 < s2
 */
struct ci_less
{
	struct nocase_compare
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
		        nocase_compare());        // comparison
	}
};


}  // namespace internal
}  // namespace api5
}  // namespace gmsec

#endif
