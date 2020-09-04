/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file AbstractBaseFieldDescriptor.cpp
 *
 *  @brief This file contains the AbstractBaseFieldDescriptor class.
 *
**/

#include <gmsec4/internal/mist/AbstractBaseFieldDescriptor.h>

#include <gmsec4/internal/mist/Context.h>

#include <gmsec4/Errors.h>
#include <gmsec4/Exception.h>

#include <gmsec4/internal/StringUtil.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;



AbstractBaseFieldDescriptor::AbstractBaseFieldDescriptor()
	: m_name(),
	  m_mode(F_OPTIONAL),
	  m_class(F_NONE),
	  m_description()
{
}


AbstractBaseFieldDescriptor::~AbstractBaseFieldDescriptor()
{
}


const char* AbstractBaseFieldDescriptor::getName() const
{
	if (m_name.empty())
	{
		throw Exception(MIST_ERROR, FIELD_NOT_SET, "Name has not been set");
	}

	return m_name.c_str();
}


FieldMode AbstractBaseFieldDescriptor::getMode()const
{
	return m_mode;
}


FieldClass AbstractBaseFieldDescriptor::getClass() const
{
	return m_class;
}


const char* AbstractBaseFieldDescriptor::getDescription() const
{
	if (m_description.empty())
	{
		throw Exception(MIST_ERROR, FIELD_NOT_SET, "Description has not been set");
	}

	return m_description.c_str();
}


void AbstractBaseFieldDescriptor::setFieldVariables(const char* name, FieldMode mode, FieldClass clss, const char* description)
{
	if (!name || std::string(name).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "Name is null, or is empty string");
	}
	if (!description)
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "Description is null");
	}

	m_name        = name;
	m_mode        = mode;
	m_class       = clss;
	m_description = description;
}


void AbstractBaseFieldDescriptor::setName(const char* name)
{
	if (!name || std::string(name).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "Name is null, or is empty string");
	}

	m_name = name;
}

void AbstractBaseFieldDescriptor::setMode(FieldMode mode)
{
	m_mode = mode;
}

void AbstractBaseFieldDescriptor::setClass(FieldClass clss)
{
	m_class = clss;
}

void AbstractBaseFieldDescriptor::setDescription(const char*  description)
{
	if (!description || std::string(description).empty())
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "Description is null, or is empty string");
	}

	m_description = description;
}
