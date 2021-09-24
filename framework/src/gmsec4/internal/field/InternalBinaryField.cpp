/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/field/InternalBinaryField.h>
#include <gmsec4/Exception.h>

#include <sstream>
#include <cstring>   // for memcpy()


namespace gmsec
{
namespace api
{
namespace internal
{

InternalBinaryField::InternalBinaryField(const char* name, const GMSEC_BIN blob, size_t length)
	: InternalField(name, Field::BIN_TYPE),
	  m_blob(0)
{
	m_length = (!blob ? 0 : length);

	if (m_length > 0)
	{
		m_blob = new unsigned char[m_length];

		std::memcpy(m_blob, blob, m_length);
	}
}


InternalBinaryField::~InternalBinaryField()
{
	delete [] m_blob;
}


GMSEC_BIN InternalBinaryField::getValue() const
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

		oss << "<FIELD NAME=\"" << getName() << "\" TYPE=\"BIN\"" << (isHeader() ? " HEAD=\"T\"" : "") << ">" << getStringValue().c_str() << "</FIELD>";

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
			<< (isHeader() ? "\"HEAD\":\"T\"," : "")
			<< "\"TYPE\":\"BIN\","
			<< "\"VALUE\":\""
			<< getStringValue().c_str()
			<< "\"}";

		m_json = oss.str();
	}

	return m_json.c_str();
}

} //namespace internal
} //namespace api
} //namespace gmsec
