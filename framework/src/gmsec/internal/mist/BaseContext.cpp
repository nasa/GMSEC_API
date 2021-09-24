
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file BaseContext.cpp
 *
 *  @brief This file contains the BaseContext class.
 *
**/


#include <gmsec/internal/mist/BaseContext.h>

#include <gmsec/mist/MIST_errors.h>

#include <gmsec/internal/mist/BaseMessageTemplate.h>

#include <gmsec/Field.h>
#include <gmsec/Message.h>
#include <gmsec/Config.h>

#include <gmsec/internal/Value.h>
#include <gmsec/internal/Log.h>

#include <gmsec/util/String.h>

using namespace gmsec;
using namespace gmsec::internal;
using namespace gmsec::mist::internal;

BaseContext::BaseContext()
	: templateId(std::string("unset")),
	validateOnApply(false)
{
}

BaseContext::BaseContext(const char * newTemplateId, bool newValidate)
	: templateId(std::string(newTemplateId)),
	validateOnApply(newValidate)
{
}

BaseContext::~BaseContext()
{
}

Status BaseContext::GetTemplateId(const char * &id) const
{
	Status status;

	if(templateId.compare(std::string("unset")) != 0)
	{
		id = util::stringNew(templateId.c_str());
	}
	else
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_UNSET_FIELD, "TemplateId has not been set yet");
	}

	return status;
}

Status BaseContext::Reset(const char * newTemplateId)
{
	Status status;

	if(newTemplateId == 0)
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "TemplateId is not a valid const char *");
		return status;
	}
	if(templateId.compare(std::string("unset")) != 0)
	{
		namePath.clear();
		visited.clear();
		statuses.clear();

		templateId = std::string(newTemplateId);
	}
	else
	{
		templateId = std::string(newTemplateId);
	}

	return status;
}

Status BaseContext::FullNamePath(std::string &fpname) const
{ 
	Status status;
	std::string name;

	for(std::list<std::string>::const_iterator it = namePath.begin(); it != namePath.end(); it++)
	{
		name += *it;
		name += ".";
	}

	fpname = name.substr(0, name.length() - 1);  // remove trailing ".'

	return status; 
}

Status BaseContext::AddVisited()
{
	Status status;
	std::string visit;
	FullNamePath(visit);
	visited.push_back(visit);
	return status;
}

Status BaseContext::IncrementDepth(std::string fieldName)
{
	Status status;
	namePath.push_back(std::string(fieldName));
	return status;
}

Status BaseContext::DecrementDepth()
{
	Status status;
	namePath.pop_back();
	return status;
}

Status BaseContext::AddStatus(const std::string &namePathString, int errorLevel, const std::string &description)
{
	Status result;
	ContextStatus status;

	status.fieldName = namePathString;
	status.errorLevel = errorLevel;
	status.description = description;

	statuses.push_back(status);

	return result;
}

Status BaseContext::ValidateBoolean(bool validate)
{
	Status status;
	validateOnApply = validate;
	return status;
}

bool BaseContext::GetValidate() const
{
	return validateOnApply;
}

bool BaseContext::Visited(const char * name) const
{
	if(name == 0)
	{
		return false;
	}

	for(std::list<std::string>::const_iterator it = visited.begin(); it != visited.end(); it++)
	{
		if(util::stringEquals(name,(*it).c_str()))
		{
			return true;
		}
	}

	return false;
}

size_t BaseContext::GetStatusCount() const
{
	return statuses.size();
}

Status BaseContext::GetStatusIndex(size_t index, const char * &name, const char * &desc, int &code)
{
	Status status;

	if(index >= statuses.size())
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_OUTSIDE_STATUS_VALUE, "Index beyond total number of statuses");
		return status;
	}

	name = util::stringNew(statuses.at(index).fieldName.c_str());
	desc = util::stringNew(statuses.at(index).description.c_str());
	code = statuses.at(index).errorLevel;

	return status;
}
