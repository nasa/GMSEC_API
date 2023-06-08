/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Subject.h
 *
 * @brief Utility functions for validating subjects.
 */

#ifndef GMSEC_API5_INTERNAL_SUBJECT_H
#define GMSEC_API5_INTERNAL_SUBJECT_H

#include <gmsec5/util/wdllexp.h>

#include <string>
#include <vector>


namespace gmsec {
namespace api5 {
namespace internal {
	class InternalMessage;
}
namespace util {


class GMSEC_API Subject
{
public:
	//SubjectElement
	//first string: name of the element
	//second string: value of the element (name of the field that contains the value)
	typedef std::pair<std::string, std::string> SubjectElement;
	typedef std::vector<SubjectElement> SubjectElementList;

	static std::string CALL_TYPE isValid (const std::string &subject, bool lenient = false);

	static std::string CALL_TYPE isValid (const std::string &subject, const internal::InternalMessage* msg, bool lenient = false);

	static std::string CALL_TYPE isValidSubscription (const std::string &subject, bool lenient = false);

	static bool CALL_TYPE getElements (const std::string &subject, std::vector<std::string> &elements);

	static std::string CALL_TYPE areValidElements (const std::vector<std::string> &elements, const internal::InternalMessage* msg, bool subscription, bool lenient = false);

	static std::string CALL_TYPE isValidElement (const std::string &element, const std::string& fieldValue, bool required, bool subscription, bool last, bool lenient = false);

	static bool CALL_TYPE doesSubjectMatchPattern(const std::string &subject, const std::string &pattern);

	static bool CALL_TYPE doesSubjectMatchPattern(const std::string &subject, const std::vector<std::string> &pattern);

	static bool CALL_TYPE doesSubjectMatchPattern(const std::vector<std::string> &subject, const std::string &pattern);

	static bool CALL_TYPE doesSubjectMatchPattern(const std::vector<std::string> &subject, const std::vector<std::string> &pattern);
};


} // namespace util
} // namespace api5
} // namespace gmsec


#endif
