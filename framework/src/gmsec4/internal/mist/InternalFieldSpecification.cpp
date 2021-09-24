/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec4/internal/mist/InternalFieldSpecification.h>

using namespace gmsec::api;
using namespace gmsec::api::mist::internal;


InternalFieldSpecification::InternalFieldSpecification(const std::string& name, const std::string& type, const std::string& mode, const std::string& clazz, const std::string& value, const std::string& desc)
	: m_name(name),
	  m_type(type),
	  m_mode(mode),
	  m_class(clazz),
	  m_value(value),
	  m_desc(desc)
{
}


const char* InternalFieldSpecification::getName() const
{
	return m_name.c_str();
}


const char* InternalFieldSpecification::getType() const
{
	return m_type.c_str();
}


const char* InternalFieldSpecification::getMode() const
{
	return m_mode.c_str();
}


const char* InternalFieldSpecification::getClassification() const
{
	return m_class.c_str();
}


const char* InternalFieldSpecification::getValue() const
{
	return m_value.c_str();
}


const char* InternalFieldSpecification::getDescription() const
{
	return m_desc.c_str();
}
