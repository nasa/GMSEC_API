/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/field/InternalU32Field.h>

#include <sstream>


namespace gmsec
{
namespace api
{
namespace internal
{

InternalU32Field::InternalU32Field(const char* name, GMSEC_U32 value)
	: InternalField(name, Field::U32_TYPE),
	  m_value(value)
{
}


InternalU32Field::~InternalU32Field()
{
}


GMSEC_U32 InternalU32Field::getValue() const
{
	return m_value;
}


const char* InternalU32Field::toXML() const
{
	if (m_xml.empty())
	{
		std::ostringstream oss;

		oss << "<FIELD NAME=\"" << getName() << "\" TYPE=\"U32\"" << (isHeader() ? " HEAD=\"T\"" : "") << ">"
			<< getValue()
			<< "</FIELD>";

		m_xml = oss.str();
	}

	return m_xml.c_str();
}


const char* InternalU32Field::toJSON() const
{
	if (m_json.empty())
	{
		std::ostringstream oss;

		oss << "{\"NAME\":\"" << getName() << "\","
			<< (isHeader() ? "\"HEAD\":\"T\"," : "")
			<< "\"TYPE\":\"U32\","
			<< "\"VALUE\":\"" << getValue() << "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}

} //namespace internal
} //namespace api
} //namespace gmsec
