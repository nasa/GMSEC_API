/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/field/InternalI8Field.h>

#include <sstream>


using namespace gmsec::api5::internal;


InternalI8Field::InternalI8Field(const char* name, GMSEC_I8 value, bool isHeader)
	: InternalField(name, Field::Type::I8, isHeader),
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

		oss << "<FIELD NAME=\"" << getName()
		    << "\" TYPE=\"I8\"";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? " HEAD=\"T\"" : "");
		}

		oss << ">"
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
			<< "\"TYPE\":\"I8\",";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? "\"HEAD\":\"T\"," : "");
		}

		oss << "\"VALUE\":\"" << (int) getValue() << "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}
