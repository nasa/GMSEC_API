/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/field/InternalStringField.h>
#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/GmsecException.h>

#include <gmsec5/internal/Encoder.h>

#include <sstream>


using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


InternalStringField::InternalStringField(const char* name, const char* value, bool isHeader)
	: InternalField(name, Field::Type::STRING, isHeader),
	  m_value()
{
	if (!value)
	{
        throw GmsecException(FIELD_ERROR, INVALID_FIELD_VALUE, "StringField value cannot be NULL");
	}

	if (strlen(value) > static_cast<size_t>(GMSEC_STRING_LIMIT))
	{
		throw GmsecException(FIELD_ERROR, INVALID_FIELD_VALUE, "StringField value is too big");
	}

	m_value = value;
}


InternalStringField::~InternalStringField()
{
}


const char* InternalStringField::getValue() const
{
	return m_value.c_str();
}


const char* InternalStringField::toXML() const
{
	if (m_xml.empty())
	{
		std::ostringstream oss;

		oss << "<FIELD NAME=\"" << getName()
		    << "\" TYPE=\"STRING\"";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? " HEAD=\"T\"" : "");
		}

		oss << ">"
		    << StringUtil::toXML(getValue())
		    << "</FIELD>";

		m_xml = oss.str();
	}

	return m_xml.c_str();
}


const char* InternalStringField::toJSON() const
{
	if (m_json.empty())
	{
		std::ostringstream oss;

		oss << "{\"NAME\":\"" << getName() << "\","
		    << "\"TYPE\":\"STRING\",";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? "\"HEAD\":\"T\"," : "");
		}

		oss << "\"VALUE\":"
		    << StringUtil::toJSON(getValue())
		    << "}";

		m_json = oss.str();
	}

	return m_json.c_str();
}
