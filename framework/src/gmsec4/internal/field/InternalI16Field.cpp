/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/field/InternalI16Field.h>

#include <sstream>


namespace gmsec
{
namespace api
{
namespace internal
{

InternalI16Field::InternalI16Field(const char* name, GMSEC_I16 value)
	: InternalField(name, Field::I16_TYPE),
	  m_value(value)
{
}


InternalI16Field::~InternalI16Field()
{
}


GMSEC_I16 InternalI16Field::getValue() const
{
	return m_value;
}


const char* InternalI16Field::toXML() const
{
	if (m_xml.empty())
	{
		std::ostringstream oss;

		oss << "<FIELD NAME=\"" << getName() << "\" TYPE=\"I16\"" << (isHeader() ? " HEAD=\"T\"" : "") << ">"
			<< getValue()
			<< "</FIELD>";

		m_xml = oss.str();
	}

	return m_xml.c_str();
}


const char* InternalI16Field::toJSON() const
{
	if (m_json.empty())
	{
		std::ostringstream oss;

		oss << "{\"NAME\":\"" << getName() << "\","
			<< (isHeader() ? "\"HEAD\":\"T\"," : "")
			<< "\"TYPE\":\"I16\","
			<< "\"VALUE\":\"" << getValue() << "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}

} //namespace internal
} //namespace api
} //namespace gmsec
