/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/field/InternalCharField.h>

#include <sstream>

namespace gmsec
{
namespace api
{
namespace internal
{

InternalCharField::InternalCharField(const char* name, GMSEC_CHAR value)
	: InternalField(name, Field::CHAR_TYPE),
	  m_value(value)
{
}


InternalCharField::~InternalCharField()
{
}


GMSEC_CHAR InternalCharField::getValue() const
{
	return m_value;
}


const char* InternalCharField::toXML() const
{
	if (m_xml.empty())
	{
		std::ostringstream oss;

		oss << "<FIELD NAME=\"" << getName() << "\" TYPE=\"CHAR\"" << (isHeader() ? " HEAD=\"T\"" : "") << ">";

		if (getValue() != 0)
		{
			oss << getValue();
		}

		oss << "</FIELD>";

		m_xml = oss.str();
	}

	return m_xml.c_str();
}


const char* InternalCharField::toJSON() const
{
	if (m_json.empty())
	{
		std::ostringstream oss;

		oss << "{\"NAME\":\"" << getName() << "\","
			<< (isHeader() ? "\"HEAD\":\"T\"," : "")
			<< "\"TYPE\":\"CHAR\","
			<< "\"VALUE\":\"" << getValue() << "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}

} //namespace internal
} //namespace api
} //namespace gmsec
