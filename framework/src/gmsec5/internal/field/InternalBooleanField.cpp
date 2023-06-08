/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/field/InternalBooleanField.h>

#include <sstream>


using namespace gmsec::api5::internal;


InternalBooleanField::InternalBooleanField(const char* name, bool value, bool isHeader)
	: InternalField(name, Field::Type::BOOL, isHeader),
	  m_value(value)
{
}


InternalBooleanField::~InternalBooleanField()
{
}


bool InternalBooleanField::getValue() const
{
	return m_value;
}


const char* InternalBooleanField::toXML() const
{
	if (m_xml.empty())
	{
		std::ostringstream oss;

		oss << "<FIELD NAME=\"" << getName()
		    << "\" TYPE=\"BOOL\"";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? " HEAD=\"T\"" : "");
		}

		oss << ">"
			<< (getValue() ? "TRUE" : "FALSE")
			<< "</FIELD>";

		m_xml = oss.str();
	}

	return m_xml.c_str();
}


const char* InternalBooleanField::toJSON() const
{
	if (m_json.empty())
	{
		std::ostringstream oss;

		oss << "{\"NAME\":\"" << getName() << "\","
			<< "\"TYPE\":\"BOOL\",";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? "\"HEAD\":\"T\"," : "");
		}

		oss << "\"VALUE\":\"" << (getValue() ? "TRUE" : "FALSE") << "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}
