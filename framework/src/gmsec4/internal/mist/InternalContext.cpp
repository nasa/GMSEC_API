/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalContext.cpp
 *
 *  @brief This file contains a class to store the results of a Message validation
 *
**/

#include <gmsec4/internal/mist/InternalContext.h>

#include <gmsec4/Exception.h>

using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


InternalContext::InternalContext()
	: m_templateId(),
	  m_statuses(),
	  m_visited(),
	  m_namePath(),
	  m_validateOnApply(false)
{
}


InternalContext::InternalContext(const char* templateId, bool validateOnApply)
	: m_templateId(),
	  m_statuses(),
	  m_visited(),
	  m_namePath(),
	  m_validateOnApply(validateOnApply)
{
	if (!templateId)
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "TemplateID cannot be NULL");
	}

	m_templateId = templateId;
}


InternalContext::~InternalContext()
{
}


const char* InternalContext::getTemplateId() const
{
	return m_templateId.c_str();
}


void InternalContext::reset(const char* templateId)
{
	if (!templateId)
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "Template ID is NULL.");
	}
	else
	{
		m_namePath.clear();
		m_visited.clear();
		m_statuses.clear();

		m_templateId = templateId;
	}
}


void InternalContext::validateOnApply(bool validate)
{
	m_validateOnApply = validate;
}


bool InternalContext::getValidate() const
{
	return m_validateOnApply;
}


std::string InternalContext::fullNamePath() const
{
	std::string name;

	for (std::list<std::string>::const_iterator it = m_namePath.begin(); it != m_namePath.end(); ++it)
	{
		name += *it;
		name += ".";
	}

	name = name.substr(0, name.length() - 1);  // remove trailing ".'

	return name;
}


void InternalContext::addVisited()
{
	m_visited.push_back(fullNamePath());
}


bool InternalContext::visited(const char* name) const
{
	if (!name)
	{
		return false;
	}

	for (std::list<std::string>::const_iterator it = m_visited.begin(); it != m_visited.end(); ++it)
	{
		if (*it == name)
		{
			return true;
		}
	}

	return false;
}


void InternalContext::incrementDepth(const std::string& fieldName)
{
	m_namePath.push_back(fieldName);
}


void InternalContext::decrementDepth()
{
	if (m_namePath.size() > 0)
	{
		m_namePath.pop_back();
	}
}


void InternalContext::addStatus(const ContextStatus& status)
{
	m_statuses.push_back(status);
}


size_t InternalContext::getStatusCount() const
{
	return m_statuses.size();
}


ContextStatus InternalContext::getStatus(size_t index)
{
	if (index >= m_statuses.size())
	{
		throw Exception(MIST_ERROR, INDEX_OUT_OF_RANGE, "Index is beyond range of available Context Statuses");
	}

	return m_statuses.at(index);
}
