/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/field/InternalU32Field.h>

#include <sstream>


using namespace gmsec::api5::internal;


InternalU32Field::InternalU32Field(const char* name, GMSEC_U32 value, bool isHeader)
	: InternalField(name, Field::Type::U32, isHeader),
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

		oss << "<FIELD NAME=\"" << getName()
		    << "\" TYPE=\"U32\"";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? " HEAD=\"T\"" : "");
		}

		oss << ">"
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
			<< "\"TYPE\":\"U32\",";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? "\"HEAD\":\"T\"," : "");
		}

		oss << "\"VALUE\":\"" << getValue() << "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}
