/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_EXCLUSION_FILTER_H
#define GMSEC_API_INTERNAL_EXCLUSION_FILTER_H

#include <gmsec4/util/Mutex.h>
#include <gmsec4/util/wdllexp.h>

#include <string>
#include <list>

namespace gmsec {
namespace api {

class Message;

namespace internal {

class GMSEC_API ExclusionFilter
{
public:
	ExclusionFilter();
	~ExclusionFilter();


	/**
	* @method excludeSubject(const char* subject) 
	*
	* @brief Adds a subject to the list of excluded subjects on a given
	* subscription.
	*
	* @param subject - the subject to be ExcludeSubjected.
	*
	* @sa Connection::excludeSubject(const char* subject)
	*/
	void excludeSubject(const char* subject);


	/**
	* @method removeExcludedSubject(const char* subject) 
	*
	* @brief Removes a subject to the list of excluded subjects on a given
	* subscription.
	*
	* @param subject - the subject to be RemoveExcludedSubject.
	*
	* @sa Connection::removeExcludedSubject(const char* subject)
	*/
	void removeExcludedSubject(const char* subject);


	/**
	* @method checkForExclusion(Message* message)
	*
	* @brief Compares the given subject against the list of excluded
	* subjects.
	*
	* @param subject - the subject which will be compared against
	*     the excluded subjects.
	*
	* @return true if the subject matches one of the rules, false otherwise.
	*/
	bool checkForExclusion(Message* message);


	/**
 	 * @method isSubjectExcluded(const std::string& subject) const
 	 *
 	 * @brief Checks to see if the provided subject is in the list of excluded subjects.
 	 *
 	 * @param subject - the subject which will be compared against the excluded subjects.
 	 *
 	 * @return true if the subject has been excluded, false otherwise.
 	 *
 	 */
	bool isSubjectExcluded(const std::string& subject) const;


private:
	// Declared, but not implemented.
	ExclusionFilter(const ExclusionFilter &);
	ExclusionFilter &operator=(const ExclusionFilter &);

	std::list<std::string>          m_excludedSubjects;
	mutable gmsec::api::util::Mutex m_mutex;
};

} // namespace internal
} // namespace api
} // namespace gmsec

#endif
