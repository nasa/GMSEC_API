/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */








/** @file Subject.h
 *
 *  @brief Utility functions for validating subjects.
 *
**/

#ifndef gmsec_internal_Subject_h
#define gmsec_internal_Subject_h


#include <gmsec/util/wdllexp.h>

#include <string>
#include <vector>


namespace gmsec {
namespace util {


class GMSEC_API Subject
{
public:

	static bool isValid (const std::string &subject, bool lenient = false);

	static bool isValidSubscription (const std::string &subject, bool lenient = false);

	static bool getElements (const std::string &subject,
			std::vector<std::string> &elements);

	static bool areValidElements (const std::vector<std::string> &elements,
			bool subscription, bool lenient = false);

	static bool isValidElement (const std::string &element,
			bool subscription, bool last, bool lenient = false);

	static bool doesSubjectMatchPattern(const std::string &subject,
			const std::string &pattern);

	static bool doesSubjectMatchPattern(const std::string &subject,
			const std::vector<std::string> &pattern);

	static bool doesSubjectMatchPattern(
			const std::vector<std::string> &subject,
			const std::string &pattern);

	static bool doesSubjectMatchPattern(
			const std::vector<std::string> &subject,
			const std::vector<std::string> &pattern);

};


} // namespace util
} // namespace gmsec


#endif /* gmsec_internal_Subject_h */
