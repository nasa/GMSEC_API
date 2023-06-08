/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5/internal/field/InternalBinaryField.h>
#include <gmsec5/internal/field/InternalBooleanField.h>
#include <gmsec5/internal/field/InternalCharField.h>
#include <gmsec5/internal/field/InternalF32Field.h>
#include <gmsec5/internal/field/InternalF64Field.h>
#include <gmsec5/internal/field/InternalI16Field.h>
#include <gmsec5/internal/field/InternalI32Field.h>
#include <gmsec5/internal/field/InternalI64Field.h>
#include <gmsec5/internal/field/InternalI8Field.h>
#include <gmsec5/internal/field/InternalStringField.h>
#include <gmsec5/internal/field/InternalU16Field.h>
#include <gmsec5/internal/field/InternalU32Field.h>
#include <gmsec5/internal/field/InternalU64Field.h>
#include <gmsec5/internal/field/InternalU8Field.h>

#include <gmsec5/internal/MathUtil.h>
#include <gmsec5/internal/StringUtil.h>

#include <gmsec5/Fields.h>
#include <gmsec5/GmsecException.h>

#include <gmsec5/util/Log.h>
#include <gmsec5/util/StdUniquePtr.h>

#include <tinyxml2.h>
#include <json.h>

#include <algorithm>
#include <iterator>
#include <limits>
#include <sstream>
#include <cmath>    // for fabs
#include <cstring>  // for strlen
#include <cctype>   // for toupper and isalpha
#include <sstream>  // for ostringstream
#include <cstdio>   // for sscanf
#include <cstdlib>  // for sscanf


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


bool InternalField::s_identifyHeaderField = false;


static bool isFieldNameCompliant(const char* name)
{
	static const int EMPTY = 0x00;
	static const int HYPHEN = 0x2D;
	static const int PERIOD = 0x2E;
	static const int UNDERSCORE = 0x5F;

	if (!name || *name == EMPTY)
	{
		return false;
	}

	const char* s = name;
	while (*s)
	{
		// If NOT one of these, then error
		if (!( (isalpha(*s) && isupper(*s))
		        || isdigit(*s)
		        || *s == HYPHEN
		        || *s == PERIOD
		        || *s == UNDERSCORE ))
		{
			return false;
		}

		++s;
	}

	return true;
}


InternalField::InternalField(const char* name, Field::Type type, bool isHeader)
	: m_name(),
	  m_type(type),
	  m_header(isHeader)
{
	if (!isFieldNameCompliant(name))
	{
		std::ostringstream oss;
		oss << "Field name is not compliant [" << (name ? name : "NULL") << "]";
		throw GmsecException(FIELD_ERROR, INVALID_FIELD_NAME, oss.str().c_str());
	}

	m_name = name;
}


InternalField::~InternalField()
{
}


const char* InternalField::getName() const
{
	return m_name.c_str();
}


Field::Type InternalField::getType() const
{
	return m_type;
}


bool InternalField::isHeader() const
{
	return m_header;
}


void InternalField::identifyHeaderField(bool flag)
{
	s_identifyHeaderField = flag;
}


bool InternalField::identifyHeaderField()
{
	return s_identifyHeaderField;
}


Field* InternalField::fromXML(tinyxml2::XMLElement* element)
{
	if (!element)
	{
		throw GmsecException(FIELD_ERROR, XML_PARSE_ERROR, "Invalid XML -- FIELD element is NULL");
	}

	// verify that our root node is "FIELD"
	const char* mename = element->Name();
	if (!mename || !StringUtil::stringEqualsIgnoreCase(mename, "FIELD"))
	{
		throw GmsecException(FIELD_ERROR, XML_PARSE_ERROR, "Invalid XML -- not a FIELD");
	}

	const char* type = NULL;
	const char* name = NULL;
	const char* value = NULL;
	const char* head = NULL;
	const char* bits = NULL;

	//find field attributes
	for (const tinyxml2::XMLAttribute* attr = element->FirstAttribute(); attr; attr = attr->Next())
	{
		const char* caname  = attr->Name();
		const char* cavalue = attr->Value();

		if (!caname)
		{
			// no attribute
		}
		else if (StringUtil::stringEqualsIgnoreCase(caname, "TYPE"))
		{
			type = cavalue;
		}
		else if (StringUtil::stringEqualsIgnoreCase(caname, "NAME"))
		{
			name = cavalue;
		}
		else if (StringUtil::stringEqualsIgnoreCase(caname, "HEAD"))
		{
			head = cavalue;
		}
		else if (StringUtil::stringEqualsIgnoreCase(caname, "BITS"))
		{
			bits = cavalue;
		}
	}

	// find value
	for (tinyxml2::XMLNode* node = element->FirstChild(); node; node = node->NextSibling())
	{
		tinyxml2::XMLText* text = node->ToText();
		if (text)
		{
			value = text->Value();
		}
	}

	return createField(name, type, value, bits, head);
}


Field* InternalField::fromJSON(const Json::Value& root)
{
	// Required attributes
	std::string name;
	std::string type;
	std::string value;

	// Non-required attributes
	std::string head;
	std::string bits;

	std::ostringstream oss;

	if (root.isMember("NAME") || root.isMember("name"))
	{
		name = (root.isMember("NAME") ? root["NAME"].asCString() : root["name"].asCString());
	}
	else
	{
		throw GmsecException(FIELD_ERROR, JSON_PARSE_ERROR, "Invalid JSON Field format - no NAME provided");
	}

	if (root.isMember("TYPE") || root.isMember("type"))
	{
		type = (root.isMember("TYPE") ? root["TYPE"].asCString() : root["type"].asCString());
	}
	else
	{
		throw GmsecException(FIELD_ERROR, JSON_PARSE_ERROR, "Invalid JSON Field format - no TYPE provided");
	}

	if (root.isMember("VALUE") || root.isMember("value"))
	{
		try
		{
			value = (root.isMember("VALUE") ? root["VALUE"].asCString() : root["value"].asCString());
		}
		catch (...)
		{
			double tmpValue = (root.isMember("VALUE") ? root["VALUE"].asDouble() : root["value"].asDouble());
			oss.str("");
			oss << tmpValue;
			value = oss.str();
		}
	}
	else
	{
		if (lookupType(type.c_str()) != Field::Type::STRING)
		{
			throw GmsecException(FIELD_ERROR, JSON_PARSE_ERROR, "Invalid JSON Field format - no VALUE provided");
		}
	}

	if (root.isMember("HEAD") || root.isMember("head"))
	{
		head = (root.isMember("HEAD") ? root["HEAD"].asCString() : root["head"].asCString());
	}

	if (root.isMember("BITS") || root.isMember("bits"))
	{
		bits = (root.isMember("BITS") ? root["BITS"].asCString() : root["bits"].asCString());
	}

	return createField(name.c_str(), type.c_str(), value.c_str(), bits.c_str(), head.c_str());
}


const char* InternalField::getStringValue() const
{
	std::ostringstream oss;

	switch (getType())
	{
		case Field::Type::CHAR:
			oss << dynamic_cast<const InternalCharField*>(this)->getValue();
			break;

		case Field::Type::BOOL:
			oss << (dynamic_cast<const InternalBooleanField*>(this)->getValue() ? "true" : "false");
			break;

		case Field::Type::I16:
			oss << dynamic_cast<const InternalI16Field*>(this)->getValue();
			break;

		case Field::Type::I32:
			oss << dynamic_cast<const InternalI32Field*>(this)->getValue();
			break;

		case Field::Type::I64:
			oss << dynamic_cast<const InternalI64Field*>(this)->getValue();
			break;

		case Field::Type::I8:
			oss << static_cast<int>(dynamic_cast<const InternalI8Field*>(this)->getValue());
			break;

		case Field::Type::U16:
			oss << dynamic_cast<const InternalU16Field*>(this)->getValue();
			break;

		case Field::Type::U32:
			oss << dynamic_cast<const InternalU32Field*>(this)->getValue();
			break;

		case Field::Type::U64:
			oss << dynamic_cast<const InternalU64Field*>(this)->getValue();
			break;

		case Field::Type::U8:
			oss << static_cast<unsigned int>(dynamic_cast<const InternalU8Field*>(this)->getValue());
			break;

		case Field::Type::F32:
			oss << dynamic_cast<const InternalF32Field*>(this)->getValue();
			break;

		case Field::Type::F64:
			oss << dynamic_cast<const InternalF64Field*>(this)->getValue();
			break;

		case Field::Type::STRING:
			oss << dynamic_cast<const InternalStringField*>(this)->getValue();
			break;

		case Field::Type::BINARY:
			oss << dynamic_cast<const InternalBinaryField*>(this)->getStringValue().c_str();
			break;
	}

	m_string = oss.str();

	return m_string.c_str();
}


bool InternalField::getBooleanValue() const
{
	if (this->getType() == Field::Type::BOOL)
	{
		return dynamic_cast<const InternalBooleanField*>(this)->getValue();
	}
	else if (this->getType() == Field::Type::STRING)
	{
		const InternalStringField* strField = dynamic_cast<const InternalStringField*>(this);

		if (StringUtil::stringEqualsIgnoreCase(strField->getValue(), "true"))
		{
			return true;
		}
		else if (StringUtil::stringEqualsIgnoreCase(strField->getValue(), "false"))
		{
			return false;
		}
	}

	// If we reach here, all other plausible cases have been considered and they yielded no results.
	try
	{
		return (this->getI64Value() != 0);
	}
	catch (const GmsecException& e)
	{
		throw GmsecException(e.getErrorClass(), e.getErrorCode(), e.getCustomCode(), "Field cannot be represented as a boolean");
	}
}


GMSEC_I16 InternalField::getI16Value() const
{
	GMSEC_F64 value = 0;

	if (this->getType() == Field::Type::I16)
	{
		value = static_cast<GMSEC_F64>( dynamic_cast<const InternalI16Field*>(this)->getValue() );
	}
	else if (this->getType() == Field::Type::BINARY)
	{
		StringUtil::Data blob( dynamic_cast<const InternalBinaryField*>(this)->getValue(), dynamic_cast<const InternalBinaryField*>(this)->getLength() );

		if (blob.length() > sizeof(GMSEC_I16))
		{
			throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_I16");
		}

		value = static_cast<GMSEC_F64>(StringUtil::I32_fromBinary(blob));
	}
	else
	{
		try
		{
			value = getF64Value();
		}
		catch (...)
		{
			throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_I16");
		}
	}

	if ((value < static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_I16>::min())) || (value > static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_I16>::max())))
	{
		throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_I16");
	}

	return static_cast<GMSEC_I16>(value);
}


GMSEC_I32 InternalField::getI32Value() const
{
	GMSEC_F64 value = 0;

	if (this->getType() == Field::Type::I32)
	{
		value = static_cast<GMSEC_F64>( dynamic_cast<const InternalI32Field*>(this)->getValue() );
	}
	else if (this->getType() == Field::Type::BINARY)
	{
		StringUtil::Data blob( dynamic_cast<const InternalBinaryField*>(this)->getValue(), dynamic_cast<const InternalBinaryField*>(this)->getLength() );

		if (blob.length() > sizeof(GMSEC_I32))
		{
			throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_I32");
		}

		value = static_cast<GMSEC_F64>(StringUtil::I32_fromBinary(blob));
	}
	else
	{
		try
		{
			value = getF64Value();
		}
		catch (...)
		{
			throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_I32");
		}
	}

	if ((value < static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_I32>::min())) || (value > static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_I32>::max())))
	{
		throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_I32");
	}

	return static_cast<GMSEC_I32>(value);
}


GMSEC_I64 InternalField::getI64Value() const
{
	GMSEC_F64 value = 0;

	if (this->getType() == Field::Type::I64)
	{
		value = static_cast<GMSEC_F64>( dynamic_cast<const InternalI64Field*>(this)->getValue() );
	}
	else if (this->getType() == Field::Type::BINARY)
	{
		StringUtil::Data blob( dynamic_cast<const InternalBinaryField*>(this)->getValue(), dynamic_cast<const InternalBinaryField*>(this)->getLength() );

		if (blob.length() > sizeof(GMSEC_I64))
		{
			throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_I64");
		}

		value = static_cast<GMSEC_F64>(StringUtil::I64_fromBinary(blob));
	}
	else
	{
		try
		{
			value = getF64Value();
		}
		catch (...)
		{
			throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_I64");
		}
	}

	if ((value < static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_I64>::min())) || (value > static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_I64>::max())))
	{
		throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_I64");
	}

	return static_cast<GMSEC_I64>(value);
}


GMSEC_U16 InternalField::getU16Value() const
{
	GMSEC_U16 value = 0;

	if (this->getType() == Field::Type::U16)
	{
		value = dynamic_cast<const InternalU16Field*>(this)->getValue();
	}
	else if (this->getType() == Field::Type::BINARY)
	{
		StringUtil::Data blob( dynamic_cast<const InternalBinaryField*>(this)->getValue(), dynamic_cast<const InternalBinaryField*>(this)->getLength() );

		if (blob.length() > sizeof(GMSEC_U16))
		{
			throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_U16");
		}

		value = static_cast<GMSEC_U16>( StringUtil::U32_fromBinary(blob) );
	}
	else
	{
		try
		{
			GMSEC_F64 tmp = getF64Value();

			if (tmp < 0) {
				GMSEC_WARNING << "Undefined behavior: converting negative GMSEC_F64 value to GMSEC_U16 value";
				value = static_cast<GMSEC_U16>(std::numeric_limits<GMSEC_U16>::max() - static_cast<GMSEC_U16>(std::fabs(tmp)) + 1);
			}
			else {
				value = static_cast<GMSEC_U16>(tmp);
			}
		}
		catch (...)
		{
			throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_U16");
		}
	}

	return value;
}


GMSEC_U32 InternalField::getU32Value() const
{
	GMSEC_U32 value = 0;

	if (this->getType() == Field::Type::U32)
	{
		value = dynamic_cast<const InternalU32Field*>(this)->getValue();
	}
	else if (this->getType() == Field::Type::BINARY)
	{
		StringUtil::Data blob( dynamic_cast<const InternalBinaryField*>(this)->getValue(), dynamic_cast<const InternalBinaryField*>(this)->getLength() );

		if (blob.length() > sizeof(GMSEC_U32))
		{
			throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_U32");
		}

		value = StringUtil::U32_fromBinary(blob);
	}
	else
	{
		try
		{
			GMSEC_F64 tmp = getF64Value();

			if (tmp < 0) {
				GMSEC_WARNING << "Undefined behavior: converting negative GMSEC_F64 value to GMSEC_U32 value";
				value = static_cast<GMSEC_U32>(std::numeric_limits<GMSEC_U32>::max() - static_cast<GMSEC_U32>(std::fabs(tmp)) + 1);
			}
			else {
				value = static_cast<GMSEC_U32>(tmp);
			}
		}
		catch (...)
		{
			throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_U32");
		}
	}

	return value;
}


GMSEC_U64 InternalField::getU64Value() const
{
	GMSEC_U64 value = 0;

	if (this->getType() == Field::Type::U64)
	{
		value = dynamic_cast<const InternalU64Field*>(this)->getValue();
	}
	else if (this->getType() == Field::Type::BINARY)
	{
		StringUtil::Data blob( dynamic_cast<const InternalBinaryField*>(this)->getValue(), dynamic_cast<const InternalBinaryField*>(this)->getLength() );

		if (blob.length() > sizeof(GMSEC_U64))
		{
			throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_U64");
		}

		value = StringUtil::U64_fromBinary(blob);
	}
	else
	{
		try
		{
			GMSEC_F64 tmp = getF64Value();

			if (tmp < 0) {
				GMSEC_WARNING << "Undefined behavior: converting negative GMSEC_F64 value to GMSEC_U64 value";
				value = static_cast<GMSEC_U64>(std::numeric_limits<GMSEC_U64>::max() - static_cast<GMSEC_U64>(std::fabs(tmp)) + 1);
			}
			else {
				value = static_cast<GMSEC_U64>(tmp);
			}
		}
		catch (...)
		{
			throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_U64");
		}
	}

	return value;
}


GMSEC_F64 InternalField::getF64Value() const
{
	GMSEC_F64 value     = 0;
	bool      converted = true;

	switch (getType())
	{
		case Field::Type::CHAR:
			value = static_cast<GMSEC_F64>(dynamic_cast<const InternalCharField*>(this)->getValue());
			break;

		case Field::Type::BOOL:
			value = static_cast<GMSEC_F64>((dynamic_cast<const InternalBooleanField*>(this)->getValue() ? 1 : 0));
			break;

		case Field::Type::I16:
			value = static_cast<GMSEC_F64>(dynamic_cast<const InternalI16Field*>(this)->getValue());
			break;

		case Field::Type::I32:
			value = static_cast<GMSEC_F64>(dynamic_cast<const InternalI32Field*>(this)->getValue());
			break;

		case Field::Type::I64:
			value = static_cast<GMSEC_F64>(dynamic_cast<const InternalI64Field*>(this)->getValue());
			break;

		case Field::Type::I8:
			value = static_cast<GMSEC_F64>(dynamic_cast<const InternalI8Field*>(this)->getValue());
			break;

		case Field::Type::U16:
			value = static_cast<GMSEC_F64>(dynamic_cast<const InternalU16Field*>(this)->getValue());
			break;

		case Field::Type::U32:
			value = static_cast<GMSEC_F64>(dynamic_cast<const InternalU32Field*>(this)->getValue());
			break;

		case Field::Type::U64:
			value = static_cast<GMSEC_F64>(dynamic_cast<const InternalU64Field*>(this)->getValue());
			break;

		case Field::Type::U8:
			value = static_cast<GMSEC_F64>(dynamic_cast<const InternalU8Field*>(this)->getValue());
			break;

		case Field::Type::F32:
			// We convert the field value to a string, and then to a 64-bit float in order to
			// achieve better precision. Going from a 32-bit float directly to a 64-bit float
			// can result in an imprecise number being returned.
			value = StringUtil::getValue<GMSEC_F64>( this->getStringValue() );
			break;

		case Field::Type::F64:
			value = dynamic_cast<const InternalF64Field*>(this)->getValue();
			break;

		case Field::Type::STRING:
			{
				std::stringstream oss(dynamic_cast<const InternalStringField*>(this)->getValue());
				oss >> value;
				if (oss.fail() || !oss.eof())
				{
					converted = false;
				}
			}
			break;

		case Field::Type::BINARY:
			{
				StringUtil::Data blob( dynamic_cast<const InternalBinaryField*>(this)->getValue(), dynamic_cast<const InternalBinaryField*>(this)->getLength() );

				if (blob.length() > sizeof(GMSEC_F64))
				{
					throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_F64");
				}

				value = static_cast<GMSEC_F64>(StringUtil::F64_fromBinary(blob));
			}
			break;
	}

	if (!converted)
	{
		throw GmsecException(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to a GMSEC_F64");
	}

	return value;
}


Field* InternalField::createField(const char* name, const char* type, const char* value, const char* bits, const char* head)
{
	if (!name || *name == '\0')
	{
		throw GmsecException(FIELD_ERROR, INVALID_FIELD_NAME, "Field name was not specified");
	}
	if (!type || *type == '\0')
	{
		throw GmsecException(FIELD_ERROR, UNKNOWN_FIELD_TYPE, "Field type was not specified");
	}
	if (!value)
	{
		value = "";
	}

	Field* field = NULL;

	if (type != NULL && name != NULL)
	{
		Field::Type ftype;

		try {
			ftype = lookupType(type);
		}
		catch (...) {
			std::ostringstream oss;
			oss << "Field " << name << " has illegal type ['" << type << "']";
			throw GmsecException(FIELD_ERROR, UNKNOWN_FIELD_TYPE, oss.str().c_str());
		}

		bool isHeader = false;
		if (head && head[0] == 'T')
		{
			isHeader = true;
		}

		try
		{
			// If here, then ftype has been initialized.
			switch (ftype)
			{
			case Field::Type::BINARY:
				{
					std::basic_string<unsigned char> blob;
					size_t blob_len = std::string(value).length();

					for (size_t i = 0; i < blob_len; i += 2)
					{
						unsigned int ch;
						char hex[3] = { value[i], value[i+1], 0 };
#ifndef WIN32
						std::sscanf(hex, "%02X", &ch);
#else
						sscanf_s(hex, "%02X", &ch);
#endif

						blob.push_back((unsigned char) ch);
					}

					field = new BinaryField(name, reinterpret_cast<const GMSEC_U8*>(blob.data()), blob.length(), isHeader);
				}
				break;

			case Field::Type::BOOL:
				field = new BooleanField(name, StringUtil::getBoolean(value), isHeader);
				break;

			case Field::Type::CHAR:
				field = new CharField(name, StringUtil::getValue<GMSEC_CHAR>(value), isHeader);
				break;

			case Field::Type::I8:
				// To ensure proper conversion to an I8 value, we need to
				// pretend we are dealing with an I16 value.  Otherwise
				// if we were to specify I8, the value would be converted
				// to the ASCII equivalent of the character in value.
				field = new I8Field(name, static_cast<GMSEC_I8>(StringUtil::getValue<GMSEC_I16>(value)), isHeader);
				break;

			case Field::Type::I16:
				field = new I16Field(name, static_cast<GMSEC_I16>(StringUtil::getValue<GMSEC_I16>(value)), isHeader);
				break;

			case Field::Type::I32:
				field = new I32Field(name, static_cast<GMSEC_I32>(StringUtil::getValue<GMSEC_I32>(value)), isHeader);
				break;

			case Field::Type::I64:
				field = new I64Field(name, static_cast<GMSEC_I64>(StringUtil::getValue<GMSEC_I64>(value)), isHeader);
				break;

			case Field::Type::F32:
				if (bits && !std::string(bits).empty())
				{
					field = new F32Field(name, MathUtil::extractF32(bits), isHeader);
				}
				else
				{
					field = new F32Field(name, StringUtil::getValue<GMSEC_F32>(value), isHeader);
				}
				break;

			case Field::Type::F64:
				if (bits && !std::string(bits).empty())
				{
					field = new F64Field(name, MathUtil::extractF64(bits), isHeader);
				}
				else
				{
					field = new F64Field(name, StringUtil::getValue<GMSEC_F64>(value), isHeader);
				}
				break;

			case Field::Type::STRING:
				field = new StringField(name, value, isHeader);
				break;

			case Field::Type::U8:
				// To ensure proper conversion to an U8 value, we need to
				// pretend we are dealing with an U16 value.  Otherwise
				// if we were to specify U8, the value would be converted
				// to the ASCII equivalent of the character in value.
				field = new U8Field(name, static_cast<GMSEC_U8>(StringUtil::getValue<GMSEC_U16>(value)), isHeader);
				break;

			case Field::Type::U16:
				field = new U16Field(name, static_cast<GMSEC_U16>(StringUtil::getValue<GMSEC_U16>(value)), isHeader);
				break;

			case Field::Type::U32:
				field = new U32Field(name, static_cast<GMSEC_U32>(StringUtil::getValue<GMSEC_U32>(value)), isHeader);
				break;

			case Field::Type::U64:
				field = new U64Field(name, StringUtil::getValue<GMSEC_U64>(value), isHeader);
				break;
			}
		}
		catch (...)
		{
			std::ostringstream oss;
			oss << "Field " << name << " has illegal value ['" << StringUtil::trim(value) << "']";
			throw GmsecException(FIELD_ERROR, INVALID_FIELD_VALUE, oss.str().c_str());
		}
	}

	return field;
}


Field* InternalField::makeFieldCopy(const Field& other)
{
	StdUniquePtr<Field> copy;

	switch (other.getType())
	{
		case Field::Type::BINARY:
			copy.reset( new BinaryField(dynamic_cast<const BinaryField&>(other)) );
		break;

		case Field::Type::BOOL:
			copy.reset( new BooleanField(dynamic_cast<const BooleanField&>(other)) );
		break;

		case Field::Type::CHAR:
			copy.reset( new CharField(dynamic_cast<const CharField&>(other)) );
		break;

		case Field::Type::I8:
			copy.reset( new I8Field(dynamic_cast<const I8Field&>(other)) );
		break;

		case Field::Type::I16:
			copy.reset( new I16Field(dynamic_cast<const I16Field&>(other)) );
		break;

		case Field::Type::I32:
			copy.reset( new I32Field(dynamic_cast<const I32Field&>(other)) );
		break;

		case Field::Type::I64:
			copy.reset( new I64Field(dynamic_cast<const I64Field&>(other)) );
		break;

		case Field::Type::F32:
			copy.reset( new F32Field(dynamic_cast<const F32Field&>(other)) );
		break;

		case Field::Type::F64:
			copy.reset( new F64Field(dynamic_cast<const F64Field&>(other)) );
		break;

		case Field::Type::STRING:
			copy.reset( new StringField(dynamic_cast<const StringField&>(other)) );
		break;

		case Field::Type::U8:
			copy.reset( new U8Field(dynamic_cast<const U8Field&>(other)) );
		break;

		case Field::Type::U16:
			copy.reset( new U16Field(dynamic_cast<const U16Field&>(other)) );
		break;

		case Field::Type::U32:
			copy.reset( new U32Field(dynamic_cast<const U32Field&>(other)) );
		break;

		case Field::Type::U64:
			copy.reset( new U64Field(dynamic_cast<const U64Field&>(other)) );
		break;
	}

	return copy.release();
}


Field::Type InternalField::lookupType(const char* typeString)
{
	if (!typeString || *typeString == '\0')
	{
		throw GmsecException(FIELD_ERROR, UNKNOWN_FIELD_TYPE, "Field type string cannot be NULL, nor an empty string");
	}

	Field::Type type;

	if (StringUtil::stringEqualsIgnoreCase(typeString, "BOOL") || StringUtil::stringEqualsIgnoreCase(typeString, "BOOLEAN"))
	{
		type = Field::Type::BOOL;
	}
	else if (StringUtil::stringEqualsIgnoreCase(typeString, "BIN") || StringUtil::stringEqualsIgnoreCase(typeString, "BINARY") ||
	         StringUtil::stringEqualsIgnoreCase(typeString, "BLOB"))
	{
		type = Field::Type::BINARY;
	}
	else if (StringUtil::stringEqualsIgnoreCase(typeString, "CHAR"))
	{
		type = Field::Type::CHAR;
	}
	else if (StringUtil::stringEqualsIgnoreCase(typeString, "I8"))
	{
		type = Field::Type::I8;
	}
	else if (StringUtil::stringEqualsIgnoreCase(typeString, "I16") || StringUtil::stringEqualsIgnoreCase(typeString, "SHORT"))
	{
		type = Field::Type::I16;
	}
	else if (StringUtil::stringEqualsIgnoreCase(typeString, "I32") || StringUtil::stringEqualsIgnoreCase(typeString, "LONG"))
	{
		type = Field::Type::I32;
	}
	else if (StringUtil::stringEqualsIgnoreCase(typeString, "I64"))
	{
		type = Field::Type::I64;
	}
	else if (StringUtil::stringEqualsIgnoreCase(typeString, "F32") || StringUtil::stringEqualsIgnoreCase(typeString, "FLOAT"))
	{
		type = Field::Type::F32;
	}
	else if (StringUtil::stringEqualsIgnoreCase(typeString, "F64") || StringUtil::stringEqualsIgnoreCase(typeString, "DOUBLE"))
	{
		type = Field::Type::F64;
	}
	else if (StringUtil::stringEqualsIgnoreCase(typeString, "STRING"))
	{
		type = Field::Type::STRING;
	}
	else if (StringUtil::stringEqualsIgnoreCase(typeString, "U8"))
	{
		type = Field::Type::U8;
	}
	else if (StringUtil::stringEqualsIgnoreCase(typeString, "U16") || StringUtil::stringEqualsIgnoreCase(typeString, "USHORT"))
	{
		type = Field::Type::U16;
	}
	else if (StringUtil::stringEqualsIgnoreCase(typeString, "U32") || StringUtil::stringEqualsIgnoreCase(typeString, "ULONG"))
	{
		type = Field::Type::U32;
	}
	else if (StringUtil::stringEqualsIgnoreCase(typeString, "U64"))
	{
		type = Field::Type::U64;
	}
	else
	{
		std::ostringstream oss;
		oss << "Unsupported Field TYPE: " << typeString;
		throw GmsecException(FIELD_ERROR, UNKNOWN_FIELD_TYPE, oss.str().c_str());
	}

	return type;
}


std::string InternalField::lookupTypeStr(Field::Type type)
{
	std::string ret_string;

	switch (type)
	{
	  case Field::Type::BOOL:
		ret_string = "BOOL";
		break;

	  case Field::Type::BINARY:
		ret_string = "BIN";
		break;

	  case Field::Type::CHAR:
		ret_string = "CHAR";
		break;

	  case Field::Type::I8:
		ret_string = "I8";
		break;

	  case Field::Type::I16:
		ret_string = "I16";
		break;

	  case Field::Type::I32:
		ret_string = "I32";
		break;

	  case Field::Type::I64:
		ret_string = "I64";
		break;

	  case Field::Type::F32:
		ret_string = "F32";
		break;

	  case Field::Type::F64:
		ret_string = "F64";
		break;

	  case Field::Type::STRING:
		ret_string = "STRING";
		break;

	  case Field::Type::U8:
		ret_string = "U8";
		break;

	  case Field::Type::U16:
		ret_string = "U16";
		break;

	  case Field::Type::U32:
		ret_string = "U32";
		break;

	  case Field::Type::U64:
		ret_string = "U64";
		break;
	}

	return ret_string;
}
