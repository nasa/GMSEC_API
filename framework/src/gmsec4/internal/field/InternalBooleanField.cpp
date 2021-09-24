/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/field/InternalBooleanField.h>

#include <sstream>


namespace gmsec
{
namespace api
{
namespace internal
{

InternalBooleanField::InternalBooleanField(const char* name, bool value)
	: InternalField(name, Field::BOOL_TYPE),
	  m_value(value)
{
}


InternalBooleanField::~InternalBooleanField()
{
}


bool InternalBooleanField::getValue() const
{
	return m_value;
}


const char* InternalBooleanField::toXML() const
{
	if (m_xml.empty())
	{
		std::ostringstream oss;

		oss << "<FIELD NAME=\"" << getName() << "\" TYPE=\"BOOL\"" << (isHeader() ? " HEAD=\"T\"" : "") << ">"
			<< (getValue() ? "TRUE" : "FALSE")
			<< "</FIELD>";

		m_xml = oss.str();
	}

	return m_xml.c_str();
}


const char* InternalBooleanField::toJSON() const
{
	if (m_json.empty())
	{
		std::ostringstream oss;

		oss << "{\"NAME\":\"" << getName() << "\","
			<< (isHeader() ? "\"HEAD\":\"T\"," : "")
			<< "\"TYPE\":\"BOOL\","
			<< "\"VALUE\":\"" << (getValue() ? "TRUE" : "FALSE") << "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}

} //namespace internal
} //namespace api
} //namespace gmsec
