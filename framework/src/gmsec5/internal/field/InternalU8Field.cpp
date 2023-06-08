/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/field/InternalU8Field.h>

#include <sstream>


using namespace gmsec::api5::internal;


InternalU8Field::InternalU8Field(const char* name, GMSEC_U8 value, bool isHeader)
	: InternalField(name, Field::Type::U8, isHeader),
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

		oss << "<FIELD NAME=\"" << getName()
		    << "\" TYPE=\"U8\"";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? " HEAD=\"T\"" : "");
		}

		oss << ">"
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
			<< "\"TYPE\":\"U8\",";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? "\"HEAD\":\"T\"," : "");
		}

		oss << "\"VALUE\":\"" << (unsigned int) getValue() << "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}
