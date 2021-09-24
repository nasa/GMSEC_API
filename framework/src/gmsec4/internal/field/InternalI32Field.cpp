/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/field/InternalI32Field.h>

#include <sstream>


namespace gmsec
{
namespace api
{
namespace internal
{

InternalI32Field::InternalI32Field(const char* name, GMSEC_I32 value)
	: InternalField(name, Field::I32_TYPE),
	  m_value(value)
{
}


InternalI32Field::~InternalI32Field()
{
}


GMSEC_I32 InternalI32Field::getValue() const
{
	return m_value;
}


const char* InternalI32Field::toXML() const
{
	if (m_xml.empty())
	{
		std::ostringstream oss;

		oss << "<FIELD NAME=\"" << getName() << "\" TYPE=\"I32\"" << (isHeader() ? " HEAD=\"T\"" : "") << ">"
			<< getValue()
			<< "</FIELD>";

		m_xml = oss.str();
	}

	return m_xml.c_str();
}


const char* InternalI32Field::toJSON() const
{
	if (m_json.empty())
	{
		std::ostringstream oss;

		oss << "{\"NAME\":\"" << getName() << "\","
			<< (isHeader() ? "\"HEAD\":\"T\"," : "")
			<< "\"TYPE\":\"I32\","
			<< "\"VALUE\":\"" << getValue() << "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}

} //namespace internal
} //namespace api
} //namespace gmsec
