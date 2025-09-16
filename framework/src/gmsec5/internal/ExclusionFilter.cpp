/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/ExclusionFilter.h>
#include <gmsec5/internal/Subject.h>

#include <gmsec5/GmsecException.h>
#include <gmsec5/Message.h>

#include <algorithm>

using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


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
