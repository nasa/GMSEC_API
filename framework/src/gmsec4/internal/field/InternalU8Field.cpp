/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/field/InternalU8Field.h>

#include <sstream>


namespace gmsec
{
namespace api
{
namespace internal
{

InternalU8Field::InternalU8Field(const char* name, GMSEC_U8 value)
	: InternalField(name, Field::U8_TYPE),
	  m_value(value)
{
}


InternalU8Field::~InternalU8Field()
{
}


GMSEC_U8 InternalU8Field::getValue() const
{
	return m_value;
}


const char* InternalU8Field::toXML() const
{
	if (m_xml.empty())
	{
		std::ostringstream oss;

		oss << "<FIELD NAME=\"" << getName() << "\" TYPE=\"U8\"" << (isHeader() ? " HEAD=\"T\"" : "") << ">"
			<< (unsigned int) getValue()
			<< "</FIELD>";

		m_xml = oss.str();
	}

	return m_xml.c_str();
}


const char* InternalU8Field::toJSON() const
{
	if (m_json.empty())
	{
		std::ostringstream oss;

		oss << "{\"NAME\":\"" << getName() << "\","
			<< (isHeader() ? "\"HEAD\":\"T\"," : "")
			<< "\"TYPE\":\"U8\","
			<< "\"VALUE\":\"" << (unsigned int) getValue() << "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}

} //namespace internal
} //namespace api
} //namespace gmsec
