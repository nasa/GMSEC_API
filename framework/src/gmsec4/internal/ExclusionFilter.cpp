/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/ExclusionFilter.h>
#include <gmsec4/internal/Subject.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>

#include <algorithm>

using namespace gmsec::api::util;


namespace gmsec {
namespace api {
namespace internal {


ExclusionFilter::ExclusionFilter()
	: m_excludedSubjects(),
	  m_mutex()
{
}


ExclusionFilter::~ExclusionFilter()
{
	m_excludedSubjects.clear();
}


void ExclusionFilter::excludeSubject(const char* subject)
{
	AutoMutex lock(m_mutex);

	// If the list of excluded subjects does not already contain the given subject
	if (std::find(m_excludedSubjects.begin(), m_excludedSubjects.end(), subject) == m_excludedSubjects.end())
	{
		m_excludedSubjects.push_back(subject);
	}
}


void ExclusionFilter::removeExcludedSubject(const char* subject)
{
	AutoMutex lock(m_mutex);

	m_excludedSubjects.remove(subject);
}


bool ExclusionFilter::checkForExclusion(Message* message)
{
	return isSubjectExcluded(std::string(message->getSubject()));
}


bool ExclusionFilter::isSubjectExcluded(const std::string& subject) const
{
	bool isExcluded = false;

	AutoMutex lock(m_mutex);

	for (std::list<std::string>::const_iterator i = m_excludedSubjects.begin(); i != m_excludedSubjects.end(); i++)
	{
		if (Subject::doesSubjectMatchPattern(subject, *i))
		{
			isExcluded = true;
			break;
		}
	}

	return isExcluded;
}

} // namespace internal
} // namespace api
} // namespace gmsec
