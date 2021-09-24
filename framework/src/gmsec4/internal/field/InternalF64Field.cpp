/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/field/InternalF64Field.h>

#include <gmsec4/internal/Encoder.h>

#include <gmsec4/internal/MathUtil.h>

#include <sstream>


using namespace gmsec::api;
using namespace gmsec::api::internal;


InternalF64Field::InternalF64Field(const char* name, GMSEC_F64 value)
	: InternalField(name, Field::F64_TYPE),
	  m_value(value)
{
}


InternalF64Field::~InternalF64Field()
{
}


GMSEC_F64 InternalF64Field::getValue() const
{
	return m_value;
}


const char* InternalF64Field::toXML() const
{
	if (m_xml.empty())
	{
		std::ostringstream oss;

		oss << "<FIELD NAME=\"" << getName() << "\" TYPE=\"F64\"" << (isHeader() ? " HEAD=\"T\"" : "")
		    << " BITS=\"";

		Encoder* encoder = Encoder::getEncoder();

		unsigned char bytes[8];
		char* bytesTmp = reinterpret_cast<char*>(bytes);
		encoder->putF64(&m_value, bytesTmp);

		const char* hex = "0123456789ABCDEF";
		for (int i = 0; i < 8; i++)
		{
			oss << hex[bytes[i] / 16];
			oss << hex[bytes[i] % 16];
		}

		oss << "\">";

		const char *p = MathUtil::realStringConstant(m_value);

		if (p)
		{
			oss << p;
		}
		else
		{
			oss << m_value;
		}

		oss << "</FIELD>";

		m_xml = oss.str();
	}

	return m_xml.c_str();
}


const char* InternalF64Field::toJSON() const
{
	if (m_json.empty())
	{
		std::ostringstream oss;

		oss << "{\"NAME\":\"" << getName() << "\","
			<< (isHeader() ? "\"HEAD\":\"T\"," : "")
			<< "\"TYPE\":\"F64\","
			<< "\"BITS\":\"";	// START BITS PROCESSING

		Encoder* encoder = Encoder::getEncoder();

		unsigned char bytes[8];
		char* bytesTmp = reinterpret_cast<char*>(bytes);
		encoder->putF64(&m_value, bytesTmp);

		const char* hex = "0123456789ABCDEF";
		for (int i = 0; i < 8; i++)
		{
			oss << hex[bytes[i] / 16];
			oss << hex[bytes[i] % 16];
		}

		oss << "\",";			// END BITS PROCESSING
		oss << "\"VALUE\":\"";

		const char* p = MathUtil::realStringConstant(m_value);

		if (p)
		{
			oss << p;
		}
		else
		{
			oss << m_value;
		}

		oss << "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}
