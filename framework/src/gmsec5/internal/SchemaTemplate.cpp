/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec5/internal/SchemaTemplate.h>

#include <gmsec5/util/Log.h>

#include <list>


using namespace gmsec::api5::internal;


SchemaTemplate::SchemaTemplate()
{
	m_index = m_definitions.begin();
}


SchemaTemplate::SchemaTemplate(const SchemaTemplate& other)
	: m_id(other.m_id),
	  m_description(other.m_description),
	  m_levelName(other.m_levelName),
	  m_level(other.m_level)
{
	for (std::list<std::string>::const_iterator it = other.m_definitions.begin(); it != other.m_definitions.end(); ++it)
	{
		m_definitions.push_back(*it);
	}
	m_index = m_definitions.begin();
}


SchemaTemplate::~SchemaTemplate()
{
}


const char* SchemaTemplate::getID() const
{
	return m_id.c_str();
}


const char* SchemaTemplate::getDescription() const
{
	return m_description.c_str();
}

const std::list<std::string>& SchemaTemplate::getDefinitions() const
{
	return m_definitions;
}

std::string SchemaTemplate::getDefinitionString() const
{
	std::string definitionString;
	for (std::list<std::string>::const_iterator it = m_definitions.begin(); it != m_definitions.end(); ++it)
	{
		if (!definitionString.empty()) definitionString.append(".");
		definitionString.append(*it);
	}
	return definitionString;
}


const char* SchemaTemplate::getFirstDefinition()
{
	m_index = m_definitions.begin();

	if (m_definitions.empty())
	{
		GMSEC_WARNING << m_id.c_str() << ": Definition list is empty! What have you done???";
		return "";
	}

	return m_definitions.front().c_str();
}


const char* SchemaTemplate::getNextDefinition()
{
	if (m_index != m_definitions.end())
	{
		++m_index;

		return m_index->c_str();
	}

	GMSEC_WARNING << "SchemaTemplate::getNextDefinition: end of list reached";
	return "";
}


bool SchemaTemplate::hasNextDefinition() const
{
	return (m_index != m_definitions.end());
}


int SchemaTemplate::getLevel() const
{
	return m_level;
}


const char* SchemaTemplate::getLevelName() const
{
	return m_levelName.c_str();
}


void SchemaTemplate::setID(const char* id)
{
	m_id = id;
}


void SchemaTemplate::setDescription(const char* description)
{
	m_description = description;
}


void SchemaTemplate::setLevel(int level)
{
	m_level = level;
}


void SchemaTemplate::setLevelName(const char* name)
{
	m_levelName = name;
}


void SchemaTemplate::addDefinition(const char* def)
{
	m_definitions.push_back(def);
}


bool SchemaTemplate::isDefinitionEmpty() const
{
	return m_definitions.empty();
}
