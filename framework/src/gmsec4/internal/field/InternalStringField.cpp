/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/field/InternalStringField.h>
#include <gmsec4/Exception.h>

#include <sstream>


namespace gmsec
{
namespace api
{
namespace internal
{

InternalStringField::InternalStringField(const char* name, const char* value)
	: InternalField(name, Field::STRING_TYPE),
	  m_value()
{
	if (!value)
	{
        throw Exception(FIELD_ERROR, INVALID_FIELD_VALUE,
				"StringField value cannot be NULL");
	}

	m_value = value;
}


InternalStringField::~InternalStringField()
{
}


const char* InternalStringField::getValue() const
{
	return m_value.c_str();
}


const char* InternalStringField::toXML() const
{
	if (m_xml.empty())
	{
		std::ostringstream oss;

		oss << "<FIELD TYPE=\"STRING\" NAME=\"" << getName() << "\"" << (isHeader() ? " HEAD=\"T\"" : "") << ">"
			<< getValue()
			<< "</FIELD>";

		m_xml = oss.str();
	}

	return m_xml.c_str();
}


const char* InternalStringField::toJSON() const
{
	if (m_json.empty())
	{
		std::ostringstream oss;

		oss << "{\"NAME\":\"" << getName() << "\","
			<< (isHeader() ? "\"HEAD\":\"T\"," : "")
			<< "\"TYPE\":\"STRING\","
			<< "\"VALUE\":\"" << getValue() << "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}

} //namespace internal
} //namespace api
} //namespace gmsec
