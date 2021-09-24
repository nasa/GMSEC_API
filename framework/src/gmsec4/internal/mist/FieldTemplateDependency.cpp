/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec4/internal/mist/FieldTemplateDependency.h>

#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/MathUtil.h>

#include <gmsec4/util/Log.h>

#include <sstream>

#include <tinyxml2.h>


using namespace gmsec::api::internal;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::util;


FieldTemplateDependency::FieldTemplateDependency()
	: m_name(),
	m_valueEquals(),
	m_valueGreaterThan(),
	m_valueLessThan(),
	m_newMode(),
	m_newValues(),
	m_newTypes()
{
}

FieldTemplateDependency::FieldTemplateDependency(const char* name, 
												 const char* valueEquals, 
												 const char* valueGreaterThan, 
												 const char* valueLessThan, 
												 const char* newMode, 
												 const char* newPattern,
												 const std::list<std::string>& newValues, 
												 const std::list<std::string>& newTypes)
	: m_name(name ? name : ""),
	m_valueEquals(valueEquals ? valueEquals : ""),
	m_valueGreaterThan(valueGreaterThan ? valueGreaterThan : ""),
	m_valueLessThan(valueLessThan ? valueLessThan : ""),
	m_newMode(newMode ? newMode : ""),
	m_newPattern(newPattern ? newPattern : ""),
	m_newValues(newValues),
	m_newTypes(newTypes)
{
}


FieldTemplateDependency::FieldTemplateDependency(const FieldTemplateDependency& other)
	: m_name(other.m_name),
	m_valueEquals(other.m_valueEquals),
	m_valueGreaterThan(other.m_valueGreaterThan),
	m_valueLessThan(other.m_valueLessThan),
	m_newMode(other.m_newMode),
	m_newValues(other.m_newValues),
	m_newTypes(other.m_newTypes)
{
}


FieldTemplateDependency::~FieldTemplateDependency()
{
}

const std::string& FieldTemplateDependency::getName() const
{
	return m_name;
}

const std::string& FieldTemplateDependency::getNewMode() const
{
	return m_newMode;
}

const std::string & gmsec::api::mist::internal::FieldTemplateDependency::getNewPattern() const
{
	return m_newPattern;
}

const std::list<std::string>& FieldTemplateDependency::getNewValues() const
{
	return m_newValues;
}

const std::list<std::string>& FieldTemplateDependency::getNewTypes() const
{
	return m_newTypes;
}

void FieldTemplateDependency::setName(const std::string& name)
{
	m_name = name;
}

void FieldTemplateDependency::setNewMode(const std::string& newMode)
{
	m_newMode = newMode;
}

void gmsec::api::mist::internal::FieldTemplateDependency::setNewPattern(const std::string & newPattern)
{
	m_newPattern = newPattern;
}

void gmsec::api::mist::internal::FieldTemplateDependency::setNewValues(const std::list<std::string>& newValues)
{
	m_newValues = newValues;
}

void gmsec::api::mist::internal::FieldTemplateDependency::setNewTypes(const std::list<std::string>& newTypes)
{
	m_newTypes = newTypes;
}

void gmsec::api::mist::internal::FieldTemplateDependency::setConditions(const std::string& valueEquals, const std::string& valueGreaterThan, const std::string& valueLessThan)
{
	m_valueEquals = valueEquals;
	m_valueGreaterThan = valueGreaterThan;
	m_valueLessThan = valueLessThan;
}

bool FieldTemplateDependency::checkConditions(const std::string& value) const
{
	if (m_valueEquals.empty()) return true; //no value needs to be check, field just needs to exist

	return m_valueEquals == value.c_str();
}

bool FieldTemplateDependency::checkConditions(GMSEC_I64 value) const
{
	GMSEC_I64 testValue;

	if (!m_valueEquals.empty())
	{
		testValue = StringUtil::getValue<GMSEC_I64>(m_valueEquals.c_str());
		if (value == testValue) return true;
	}
	if (!m_valueGreaterThan.empty())
	{
		testValue = StringUtil::getValue<GMSEC_I64>(m_valueGreaterThan.c_str());
		if (value > testValue) return true;
	}
	if (!m_valueLessThan.empty())
	{
		testValue = StringUtil::getValue<GMSEC_I64>(m_valueLessThan.c_str());
		if (value < testValue) return true;
	}

	return false;
}

bool FieldTemplateDependency::checkConditions(GMSEC_U64 value) const
{
	GMSEC_U64 testValue;

	if (!m_valueEquals.empty())
	{
		testValue = StringUtil::getValue<GMSEC_U64>(m_valueEquals.c_str());
		if (value == testValue) return true;
	}
	if (!m_valueGreaterThan.empty())
	{
		testValue = StringUtil::getValue<GMSEC_U64>(m_valueGreaterThan.c_str());
		if (value > testValue) return true;
	}
	if (!m_valueLessThan.empty())
	{
		testValue = StringUtil::getValue<GMSEC_U64>(m_valueLessThan.c_str());
		if (value < testValue) return true;
	}

	return false;
}

bool FieldTemplateDependency::checkConditions(GMSEC_F64 value) const
{
	GMSEC_F64 testValue;

	if (!m_valueEquals.empty())
	{
		testValue = StringUtil::getValue<GMSEC_F64>(m_valueEquals.c_str());
		if (MathUtil::doubleEquals(value, testValue)) return true;
	}
	if (!m_valueGreaterThan.empty())
	{
		testValue = StringUtil::getValue<GMSEC_F64>(m_valueGreaterThan.c_str());
		if (value > testValue) return true;
	}
	if (!m_valueLessThan.empty())
	{
		testValue = StringUtil::getValue<GMSEC_F64>(m_valueLessThan.c_str());
		if (value < testValue) return true;
	}

	return false;
}

bool FieldTemplateDependency::checkConditions(bool value) const
{
	if (m_valueEquals == "1" || m_valueEquals == "0")
	{
		return m_valueEquals == (value ? "1" : "0");
	}
	return StringUtil::stringEqualsIgnoreCase(m_valueEquals.c_str(), (value ? "true" : "false"));
}
