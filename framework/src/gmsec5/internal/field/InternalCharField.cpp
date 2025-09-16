/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/field/InternalCharField.h>

#include <sstream>


using namespace gmsec::api5::internal;


InternalCharField::InternalCharField(const char* name, GMSEC_CHAR value, bool isHeader)
	: InternalField(name, Field::Type::CHAR, isHeader),
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

		oss << "<FIELD NAME=\"" << getName()
		    << "\" TYPE=\"CHAR\"";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? " HEAD=\"T\"" : "");
		}

		oss << ">";

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
			<< "\"TYPE\":\"CHAR\",";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? "\"HEAD\":\"T\"," : "");
		}

		oss << "\"VALUE\":\"" << getValue() << "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}
