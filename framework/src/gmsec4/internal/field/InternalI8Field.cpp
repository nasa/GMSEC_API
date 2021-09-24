/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/field/InternalI8Field.h>

#include <sstream>


namespace gmsec
{
namespace api
{
namespace internal
{

InternalI8Field::InternalI8Field(const char* name, GMSEC_I8 value)
	: InternalField(name, Field::I8_TYPE),
	  m_value(value)
{
}


InternalI8Field::~InternalI8Field()
{
}


GMSEC_I8 InternalI8Field::getValue() const
{
	return m_value;
}


const char* InternalI8Field::toXML() const
{
	if (m_xml.empty())
	{
		std::ostringstream oss;

		oss << "<FIELD NAME=\"" << getName() << "\" TYPE=\"I8\"" << (isHeader() ? " HEAD=\"T\"" : "") << ">"
			<< (int) getValue()
			<< "</FIELD>";

		m_xml = oss.str();
	}

	return m_xml.c_str();
}


const char* InternalI8Field::toJSON() const
{
	if (m_json.empty())
	{
		std::ostringstream oss;

		oss << "{\"NAME\":\"" << getName() << "\","
			<< (isHeader() ? "\"HEAD\":\"T\"," : "")
			<< "\"TYPE\":\"I8\","
			<< "\"VALUE\":\"" << (int) getValue() << "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}

} //namespace internal
} //namespace api
} //namespace gmsec
