/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/field/InternalBinaryField.h>

#include <gmsec5/internal/Encoder.h>

#include <gmsec5/GmsecException.h>

#include <sstream>
#include <cstring>   // for memcpy()


using namespace gmsec::api5::internal;


InternalBinaryField::InternalBinaryField(const char* name, const GMSEC_U8* blob, size_t length, bool isHeader)
	: InternalField(name, Field::Type::BINARY, isHeader),
	  m_blob(0)
{
	if (length > static_cast<size_t>(GMSEC_BIN_LIMIT))
	{
		throw GmsecException(FIELD_ERROR, INVALID_FIELD_VALUE, "BinaryField blob is too big");
	}

	m_length = (!blob ? 0 : length);

	if (m_length > 0)
	{
		try
		{
			m_blob = new unsigned char[m_length];

			std::memcpy(m_blob, blob, m_length);
		}
		catch (const std::bad_alloc& e)
		{
			throw GmsecException(FIELD_ERROR, INVALID_FIELD_VALUE, e.what());
		}
	}
}


InternalBinaryField::~InternalBinaryField()
{
	delete [] m_blob;
}


GMSEC_U8 const* InternalBinaryField::getValue() const
{
	return m_blob;
}


size_t InternalBinaryField::getLength() const
{
	return m_length;
}


std::string InternalBinaryField::getStringValue() const
{
	std::ostringstream oss;

	static const char* hex = "0123456789ABCDEF";

	const GMSEC_U8* tmp = (GMSEC_U8*) m_blob;
	for (size_t i = 0; i < m_length; ++i)
	{
		int upper = unsigned(tmp[i]) / 16;
		int lower = unsigned(tmp[i]) % 16;

		oss << hex[upper] << hex[lower];
	}

	return oss.str();
}


const char* InternalBinaryField::toXML() const
{
	if (m_xml.empty())
	{
		std::ostringstream oss;

		oss << "<FIELD NAME=\"" << getName()
		    << "\" TYPE=\"BIN\"";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? " HEAD=\"T\"" : "");
		}

		oss << ">"
		    << getStringValue().c_str()
		    << "</FIELD>";

		m_xml = oss.str();
	}

	return m_xml.c_str();
}


const char* InternalBinaryField::toJSON() const
{
	if (m_json.empty())
	{
		std::ostringstream oss;

		oss << "{\"NAME\":\"" << getName() << "\","
			<< "\"TYPE\":\"BIN\",";

		if (identifyHeaderField())
		{
		    oss << (isHeader() ? "\"HEAD\":\"T\"," : "");
		}

		oss << "\"VALUE\":\""
			<< getStringValue().c_str()
			<< "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}
