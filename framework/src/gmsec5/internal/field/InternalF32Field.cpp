/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/field/InternalF32Field.h>

#include <gmsec5/internal/Encoder.h>

#include <gmsec5/internal/MathUtil.h>

#include <sstream>


using namespace gmsec::api5::internal;


InternalF32Field::InternalF32Field(const char* name, GMSEC_F32 value, bool isHeader)
	: InternalField(name, Field::Type::F32, isHeader),
	  m_value(value)
{
}


InternalF32Field::~InternalF32Field()
{
}


GMSEC_F32 InternalF32Field::getValue() const
{
	return m_value;
}


const char* InternalF32Field::toXML() const
{
	if (m_xml.empty())
	{
		std::ostringstream oss;

		oss << "<FIELD NAME=\"" << getName()
		    << "\" TYPE=\"F32\"";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? " HEAD=\"T\"" : "");
		}

		oss << " BITS=\"";

		Encoder* encoder = Encoder::getEncoder();

		unsigned char bytes[4];
		char* bytesTmp = reinterpret_cast<char*>(bytes);
		encoder->putF32(&m_value, bytesTmp);

		const char* hex = "0123456789ABCDEF";
		for (int i = 0; i < 4; i++)
		{
			oss << hex[bytes[i] / 16];
			oss << hex[bytes[i] % 16];
		}

		oss << "\">";

		const char* p = MathUtil::realStringConstant(m_value);

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


const char* InternalF32Field::toJSON() const
{
	if (m_json.empty())
	{
		std::ostringstream oss;

		oss << "{\"NAME\":\"" << getName() << "\","
			<< "\"TYPE\":\"F32\",";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? "\"HEAD\":\"T\"," : "");
		}

		oss << "\"BITS\":\"";	// START BITS PROCESSING

		Encoder* encoder = Encoder::getEncoder();

		unsigned char bytes[4];
		char* bytesTmp = reinterpret_cast<char*>(bytes);
		encoder->putF32(&m_value, bytesTmp);

		const char* hex = "0123456789ABCDEF";
		for (int i = 0; i < 4; i++)
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
