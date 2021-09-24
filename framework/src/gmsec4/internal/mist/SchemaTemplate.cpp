/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec4/internal/mist/SchemaTemplate.h>

#include <gmsec4/util/Log.h>

namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{


SchemaTemplate::SchemaTemplate()
{
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

const char* SchemaTemplate::getFirstDefinition()
{
	m_definitionIndex = 0;
	if(m_definitions.empty())
	{
		GMSEC_WARNING << m_id.c_str() << ": Definition list is empty! What have you done???";
		return "";
	}
	else
	{
		return m_definitions.get(m_definitionIndex).c_str();
	}
}

const char* SchemaTemplate::getNextDefinition()
{
	if(m_definitionIndex+1 < m_definitions.size())
	{
		m_definitionIndex++;
		return m_definitions.get(m_definitionIndex).c_str();
	}
	else
	{
		GMSEC_WARNING << "SchemaTemplate::getNextDefinition: end of list reached";
		return "";
	}
}

bool SchemaTemplate::hasNextDefinition() const
{
	if(m_definitionIndex+1 < m_definitions.size())
	{
		return true;
	}
	else
	{
		return false;
	}
}

unsigned int SchemaTemplate::getLevel() const
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

void SchemaTemplate::setLevel(unsigned int level)
{
	m_level = level;
}

void SchemaTemplate::setLevelName(const char* name)
{
	m_levelName = name;
}

void SchemaTemplate::addDefinition(const char* def)
{
	m_definitions.add(def);
}

bool SchemaTemplate::isDefinitionEmpty() const
{
	return m_definitions.empty();
}

}//namespace internal
}//namespace mist
}//namespace api
}//namespace gmsec
