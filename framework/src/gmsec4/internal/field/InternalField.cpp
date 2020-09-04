/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4/internal/field/InternalBinaryField.h>
#include <gmsec4/internal/field/InternalBooleanField.h>
#include <gmsec4/internal/field/InternalCharField.h>
#include <gmsec4/internal/field/InternalF32Field.h>
#include <gmsec4/internal/field/InternalF64Field.h>
#include <gmsec4/internal/field/InternalI16Field.h>
#include <gmsec4/internal/field/InternalI32Field.h>
#include <gmsec4/internal/field/InternalI64Field.h>
#include <gmsec4/internal/field/InternalI8Field.h>
#include <gmsec4/internal/field/InternalStringField.h>
#include <gmsec4/internal/field/InternalU16Field.h>
#include <gmsec4/internal/field/InternalU32Field.h>
#include <gmsec4/internal/field/InternalU64Field.h>
#include <gmsec4/internal/field/InternalU8Field.h>

#include <gmsec4/internal/MathUtil.h>
#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/field/BinaryField.h>
#include <gmsec4/field/BooleanField.h>
#include <gmsec4/field/CharField.h>
#include <gmsec4/field/F32Field.h>
#include <gmsec4/field/F64Field.h>
#include <gmsec4/field/I16Field.h>
#include <gmsec4/field/I32Field.h>
#include <gmsec4/field/I64Field.h>
#include <gmsec4/field/I8Field.h>
#include <gmsec4/field/StringField.h>
#include <gmsec4/field/U16Field.h>
#include <gmsec4/field/U32Field.h>
#include <gmsec4/field/U64Field.h>
#include <gmsec4/field/U8Field.h>

#include <gmsec4/Exception.h>

#include <gmsec4/util/Log.h>

#include <tinyxml2.h>
#include <json.h>

#include <algorithm>
#include <iterator>
#include <limits>
#include <cstring>  // for strlen
#include <cctype>   // for toupper
#include <sstream>  // for ostringstream
#include <cstdio>   // for sscanf


using namespace gmsec::api::util;


namespace gmsec
{
namespace api
{
namespace internal
{


InternalField::InternalField(const char* name, Field::FieldType type)
	: m_name(),
	  m_type(type),
	  m_header(false)
{
	if (!name || StringUtil::stringLength(name) == 0)
	{
		throw Exception(FIELD_ERROR, INVALID_FIELD_NAME,
			"Field name cannot be NULL, nor an empty string");
	}

	m_name = StringConverter::instance().convertString(name);
}


InternalField::~InternalField()
{
}


const char* InternalField::getName() const
{
	return m_name.c_str();
}


void InternalField::setName(const char* name)
{
	m_name = name;
}


Field::FieldType InternalField::getType() const
{
	return m_type;
}


void InternalField::setType(Field::FieldType type)
{
	m_type = type;
}


bool InternalField::isHeader() const
{
	return m_header;
}


void InternalField::isHeader(bool isHeader)
{
	m_header = isHeader;
}


Field* InternalField::fromXML(tinyxml2::XMLElement* element)
{
	if (!element)
	{
		throw Exception(FIELD_ERROR, XML_PARSE_ERROR,
				"Invalid XML -- FIELD element is NULL");
	}

	// verify that our root node is "FIELD"
	const char* mename = element->Name();
	if (!mename || !StringUtil::stringEquals(mename, "FIELD"))
	{
		throw Exception(FIELD_ERROR, XML_PARSE_ERROR,
			"Invalid XML -- not a FIELD");
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
		else if (StringUtil::stringEquals(caname, "TYPE"))
		{
			type = cavalue;
		}
		else if (StringUtil::stringEquals(caname, "NAME"))
		{
			name = cavalue;
		}
		else if (StringUtil::stringEquals(caname, "HEAD"))
		{
			head = cavalue;
		}
		else if (StringUtil::stringEquals(caname, "BITS"))
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
	Status result;

	// Required attributes
	const char* name = NULL;
	const char* type = NULL;
	const char* value = NULL;
	// Non-required attributes
	const char* head = NULL;
	const char* bits = NULL;

	if (root.isMember(std::string("NAME")))
	{
		name = root["NAME"].asCString();
	}
	else
	{
		throw Exception(FIELD_ERROR, JSON_PARSE_ERROR,
			"Invalid JSON Field format - no NAME provided");
	}

	if (root.isMember(std::string("TYPE")))
	{
		type = root["TYPE"].asCString();
	}
	else
	{
		throw Exception(FIELD_ERROR, JSON_PARSE_ERROR,
			"Invalid JSON Field format - no TYPE provided");
	}

	if (root.isMember(std::string("VALUE")))
	{
		value = root["VALUE"].asCString();
	}
	else
	{
		if (lookupType(type) == Field::STRING_TYPE)
		{
			value = "";
		}
		else
		{
			throw Exception(FIELD_ERROR, JSON_PARSE_ERROR,
				"Invalid JSON Field format - no VALUE provided");
		}
	}

	if (root.isMember(std::string("HEAD")))
	{
		head = root["HEAD"].asCString();
	}

	if (root.isMember(std::string("BITS")))
	{
		bits = root["BITS"].asCString();
	}

	return createField(name, type, value, bits, head);
}


GMSEC_I64 InternalField::getIntegerValue() const
{
	const GMSEC_F64 dbl = getDoubleValue();

	if ((dbl < GMSEC_F64(std::numeric_limits<GMSEC_I64>::min())) || (dbl > GMSEC_F64(std::numeric_limits<GMSEC_I64>::max())))
	{
		throw Exception(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to an integer");
	}

	return (GMSEC_I64) dbl;
}


GMSEC_U64 InternalField::getUnsignedIntegerValue() const
{
	const GMSEC_F64 dbl = getDoubleValue();

	if ((dbl < GMSEC_F64(std::numeric_limits<GMSEC_U64>::min())) || (dbl > GMSEC_F64(std::numeric_limits<GMSEC_U64>::max())))
	{
		throw Exception(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to an unsigned integer");
	}

	return (GMSEC_U64) dbl;
}


GMSEC_F64 InternalField::getDoubleValue() const
{
	GMSEC_F64 value     = 0;
	bool      converted = true;

	switch (getType())
	{
		case Field::CHAR_TYPE:
			value = (GMSEC_F64) dynamic_cast<const InternalCharField*>(this)->getValue();
			break;

		case Field::BOOL_TYPE:
			value = (GMSEC_F64) (dynamic_cast<const InternalBooleanField*>(this)->getValue() ? 1 : 0);
			break;

		case Field::I16_TYPE:
			value = (GMSEC_F64) dynamic_cast<const InternalI16Field*>(this)->getValue();
			break;

		case Field::I32_TYPE:
			value = (GMSEC_F64) dynamic_cast<const InternalI32Field*>(this)->getValue();
			break;

		case Field::I64_TYPE:
			value = (GMSEC_F64) dynamic_cast<const InternalI64Field*>(this)->getValue();
			break;

		case Field::I8_TYPE:
			value = (GMSEC_F64) dynamic_cast<const InternalI8Field*>(this)->getValue();
			break;

		case Field::U16_TYPE:
			value = (GMSEC_F64) dynamic_cast<const InternalU16Field*>(this)->getValue();
			break;

		case Field::U32_TYPE:
			value = (GMSEC_F64) dynamic_cast<const InternalU32Field*>(this)->getValue();
			break;

		case Field::U64_TYPE:
			value = (GMSEC_F64) dynamic_cast<const InternalU64Field*>(this)->getValue();
			break;

		case Field::U8_TYPE:
			value = (GMSEC_F64) dynamic_cast<const InternalU8Field*>(this)->getValue();
			break;

		case Field::F32_TYPE:
			value = (GMSEC_F64) dynamic_cast<const InternalF32Field*>(this)->getValue();
			break;

		case Field::F64_TYPE:
			value = dynamic_cast<const InternalF64Field*>(this)->getValue();
			break;

		case Field::STRING_TYPE:
			{
				std::stringstream oss(dynamic_cast<const InternalStringField*>(this)->getValue());
				oss >> value;
				if (oss.fail() || !oss.eof())
				{
					converted = false;
				}
			}
			break;

		default:
			converted = false;
			break;
	}

	if (!converted)
	{
		throw Exception(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to double");
	}

	return value;
}


const char* InternalField::getStringValue() const
{
	bool converted = true;
	std::ostringstream oss;

	switch (getType())
	{
		case Field::CHAR_TYPE:
			oss << dynamic_cast<const InternalCharField*>(this)->getValue();
			break;

		case Field::BOOL_TYPE:
			oss << (dynamic_cast<const InternalBooleanField*>(this)->getValue() ? "true" : "false");
			break;

		case Field::I16_TYPE:
			oss << dynamic_cast<const InternalI16Field*>(this)->getValue();
			break;

		case Field::I32_TYPE:
			oss << dynamic_cast<const InternalI32Field*>(this)->getValue();
			break;

		case Field::I64_TYPE:
			oss << dynamic_cast<const InternalI64Field*>(this)->getValue();
			break;

		case Field::I8_TYPE:
			oss << dynamic_cast<const InternalI8Field*>(this)->getValue();
			break;

		case Field::U16_TYPE:
			oss << dynamic_cast<const InternalU16Field*>(this)->getValue();
			break;

		case Field::U32_TYPE:
			oss << dynamic_cast<const InternalU32Field*>(this)->getValue();
			break;

		case Field::U64_TYPE:
			oss << dynamic_cast<const InternalU64Field*>(this)->getValue();
			break;

		case Field::U8_TYPE:
			oss << dynamic_cast<const InternalU8Field*>(this)->getValue();
			break;

		case Field::F32_TYPE:
			oss << dynamic_cast<const InternalF32Field*>(this)->getValue();
			break;

		case Field::F64_TYPE:
			oss << dynamic_cast<const InternalF64Field*>(this)->getValue();
			break;

		case Field::STRING_TYPE:
			oss << dynamic_cast<const InternalStringField*>(this)->getValue();
			break;

		default:
			converted = false;
			break;
	}

	if (!converted)
	{
		throw Exception(FIELD_ERROR, INVALID_FIELD, "Field cannot be converted to string");
	}

	m_string = oss.str();

	return m_string.c_str();
}


Field* InternalField::createField(const char* name, const char* type, const char* value, const char* bits, const char* head)
{
	if (!name)
	{
		throw Exception(FIELD_ERROR, INVALID_FIELD_NAME, "Field name was not specified");
	}
	if (!type)
	{
		throw Exception(FIELD_ERROR, UNKNOWN_FIELD_TYPE, "Field type was not specified");
	}
	if (!value)
	{
		value = "";
	}

	Field* field = NULL;

	if (type != NULL && name != NULL)
	{
		switch (lookupType(type))
		{
			case Field::BIN_TYPE:
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
						sscanf_s(hex, "%02X", &ch, sizeof(ch));
#endif

						blob.push_back((unsigned char) ch);
					}

					field = new BinaryField(name, const_cast<GMSEC_BIN>(blob.data()), blob.length());
				}
				break;

			case Field::BOOL_TYPE:
				field = new BooleanField(name, StringUtil::stringEqualsIgnoreCase(value, "TRUE"));
				break;

			case Field::CHAR_TYPE:
				field = new CharField(name, getValue<char>(value));
				break;

			case Field::I8_TYPE:
				// To ensure proper conversion to an I8 value, we need to
				// pretend we are dealing with an I16 value.  Otherwise
				// if we were to specify I8, the value would be converted
				// to the ASCII equivalent of the character in value.
				field = new I8Field(name, (GMSEC_I8) getValue<GMSEC_I16>(value));
				break;

			case Field::I16_TYPE:
				field = new I16Field(name, getValue<GMSEC_I16>(value));
				break;

			case Field::I32_TYPE:
				field = new I32Field(name, getValue<GMSEC_I32>(value));
				break;

			case Field::I64_TYPE:
				field = new I64Field(name, getValue<GMSEC_I64>(value));
				break;

			case Field::F32_TYPE:
				if (bits)
				{
					field = new F32Field(name, MathUtil::extractF32(bits));
				}
				else
				{
					field = new F32Field(name, getValue<GMSEC_F32>(value));
				}
				break;

			case Field::F64_TYPE:
				if (bits)
				{
					field = new F64Field(name, MathUtil::extractF64(bits));
				}
				else
				{
					field = new F64Field(name, getValue<GMSEC_F64>(value));
				}
				break;

			case Field::STRING_TYPE:
				field = new StringField(name, value);
				break;

			case Field::U8_TYPE:
				// To ensure proper conversion to an U8 value, we need to
				// pretend we are dealing with an U16 value.  Otherwise
				// if we were to specify U8, the value would be converted
				// to the ASCII equivalent of the character in value.
				field = new U8Field(name, (GMSEC_U8) getValue<GMSEC_U16>(value));
				break;

			case Field::U16_TYPE:
				field = new U16Field(name, getValue<GMSEC_U16>(value));
				break;

			case Field::U32_TYPE:
				field = new U32Field(name, getValue<GMSEC_U32>(value));
				break;

			case Field::U64_TYPE:
				field = new U64Field(name, getValue<GMSEC_U64>(value));
				break;

			default:
				throw Exception(FIELD_ERROR, UNKNOWN_FIELD_TYPE, "Unknown Field type was specified");
		}

		if (field && head && head[0] == 'T')
		{
			field->isHeader(true);
		}
	}

	return field;
}


Field* InternalField::makeFieldCopy(const Field& other)
{
	Field* copy = 0;

	switch (other.getType())
	{
		case Field::BIN_TYPE:
		{
			const BinaryField& tmp = dynamic_cast<const BinaryField&>(other);
			copy = new BinaryField(tmp.getName(), tmp.getValue(), tmp.getLength());
		}
		break;

		case Field::BOOL_TYPE:
		{
			const BooleanField& tmp = dynamic_cast<const BooleanField&>(other);
			copy = new BooleanField(tmp.getName(), tmp.getValue());
		}
		break;

		case Field::CHAR_TYPE:
		{
			const CharField& tmp = dynamic_cast<const CharField&>(other);
			copy = new CharField(tmp.getName(), tmp.getValue());
		}
		break;

		case Field::I8_TYPE:
		{
			const I8Field& tmp = dynamic_cast<const I8Field&>(other);
			copy = new I8Field(tmp.getName(), tmp.getValue());
		}
		break;

		case Field::I16_TYPE:
		{
			const I16Field& tmp = dynamic_cast<const I16Field&>(other);
			copy = new I16Field(tmp.getName(), tmp.getValue());
		}
		break;

		case Field::I32_TYPE:
		{
			const I32Field& tmp = dynamic_cast<const I32Field&>(other);
			copy = new I32Field(tmp.getName(), tmp.getValue());
		}
		break;

		case Field::I64_TYPE:
		{
			const I64Field& tmp = dynamic_cast<const I64Field&>(other);
			copy = new I64Field(tmp.getName(), tmp.getValue());
		}
		break;

		case Field::F32_TYPE:
		{
			const F32Field& tmp = dynamic_cast<const F32Field&>(other);
			copy = new F32Field(tmp.getName(), tmp.getValue());
		}
		break;

		case Field::F64_TYPE:
		{
			const F64Field& tmp = dynamic_cast<const F64Field&>(other);
			copy = new F64Field(tmp.getName(), tmp.getValue());
		}
		break;

		case Field::STRING_TYPE:
		{
			const StringField& tmp = dynamic_cast<const StringField&>(other);
			copy = new StringField(tmp.getName(), tmp.getValue());
		}
		break;

		case Field::U8_TYPE:
		{
			const U8Field& tmp = dynamic_cast<const U8Field&>(other);
			copy = new U8Field(tmp.getName(), tmp.getValue());
		}
		break;

		case Field::U16_TYPE:
		{
			const U16Field& tmp = dynamic_cast<const U16Field&>(other);
			copy = new U16Field(tmp.getName(), tmp.getValue());
		}
		break;

		case Field::U32_TYPE:
		{
			const U32Field& tmp = dynamic_cast<const U32Field&>(other);
			copy = new U32Field(tmp.getName(), tmp.getValue());
		}
		break;

		case Field::U64_TYPE:
		{
			const U64Field& tmp = dynamic_cast<const U64Field&>(other);
			copy = new U64Field(tmp.getName(), tmp.getValue());
		}
		break;

		default:
			throw Exception(MSG_ERROR, UNKNOWN_FIELD_TYPE,
					"Error with adding Field with unknwon type");
	}

	if (copy)
	{
		copy->isHeader(other.isHeader());
	}

	return copy;
}


bool InternalField::testEquals(const Field& first, const Field& second)
{
	bool ret_val = false;

	if (first.getType() != second.getType())
	{
		std::ostringstream oss;
		oss << "InternalField::testEquals:  field type mistmatch " << lookupTypeStr(first.getType()) << " and " << lookupTypeStr(second.getType());
		throw Exception(MSG_ERROR,
		                FIELD_TYPE_MISMATCH,
		                oss.str().c_str());
	}

	switch (first.getType())
	{
		case Field::BIN_TYPE:
		{
			const BinaryField& tmp1 = dynamic_cast<const BinaryField&>(first);
			const BinaryField& tmp2 = dynamic_cast<const BinaryField&>(second);
			if (std::memcmp(tmp1.getValue(), tmp2.getValue(), tmp1.getLength()) == 0)
			{
				ret_val = true;
			}
		}
		break;

		case Field::BOOL_TYPE:
		{
			const BooleanField& tmp1 = dynamic_cast<const BooleanField&>(first);
			const BooleanField& tmp2 = dynamic_cast<const BooleanField&>(second);
			if (tmp1.getValue() == tmp2.getValue())
			{
				ret_val = true;
			}
		}
		break;

		case Field::CHAR_TYPE:
		{
			const CharField& tmp1 = dynamic_cast<const CharField&>(first);
			const CharField& tmp2 = dynamic_cast<const CharField&>(second);
			if (tmp1.getValue() == tmp2.getValue())
			{
				ret_val = true;
			}
		}
		break;

		case Field::I8_TYPE:
		{
			const I8Field& tmp1 = dynamic_cast<const I8Field&>(first);
			const I8Field& tmp2 = dynamic_cast<const I8Field&>(second);
			if (tmp1.getValue() == tmp2.getValue())
			{
				ret_val = true;
			}
		}
		break;

		case Field::I16_TYPE:
		{
			const I16Field& tmp1 = dynamic_cast<const I16Field&>(first);
			const I16Field& tmp2 = dynamic_cast<const I16Field&>(second);
			if (tmp1.getValue() == tmp2.getValue())
			{
				ret_val = true;
			}
		}
		break;

		case Field::I32_TYPE:
		{
			const I32Field& tmp1 = dynamic_cast<const I32Field&>(first);
			const I32Field& tmp2 = dynamic_cast<const I32Field&>(second);
			if (tmp1.getValue() == tmp2.getValue())
			{
				ret_val = true;
			}
		}
		break;

		case Field::I64_TYPE:
		{
			const I64Field& tmp1 = dynamic_cast<const I64Field&>(first);
			const I64Field& tmp2 = dynamic_cast<const I64Field&>(second);
			if (tmp1.getValue() == tmp2.getValue())
			{
				ret_val = true;
			}
		}
		break;

		case Field::F32_TYPE:
		{
			const F32Field& tmp1 = dynamic_cast<const F32Field&>(first);
			const F32Field& tmp2 = dynamic_cast<const F32Field&>(second);
			if (tmp1.getValue() == tmp2.getValue())
			{
				ret_val = true;
			}
		}
		break;

		case Field::F64_TYPE:
		{
			const F64Field& tmp1 = dynamic_cast<const F64Field&>(first);
			const F64Field& tmp2 = dynamic_cast<const F64Field&>(second);
			if (tmp1.getValue() == tmp2.getValue())
			{
				ret_val = true;
			}
		}
		break;

		case Field::STRING_TYPE:
		{
			const StringField& tmp1 = dynamic_cast<const StringField&>(first);
			const StringField& tmp2 = dynamic_cast<const StringField&>(second);
			if (StringUtil::stringEquals(tmp1.getValue(), tmp2.getValue()))
			{
				ret_val = true;
			}
		}
		break;

		case Field::U8_TYPE:
		{
			const U8Field& tmp1 = dynamic_cast<const U8Field&>(first);
			const U8Field& tmp2 = dynamic_cast<const U8Field&>(second);
			if (tmp1.getValue() == tmp2.getValue())
			{
				ret_val = true;
			}
		}
		break;

		case Field::U16_TYPE:
		{
			const U16Field& tmp1 = dynamic_cast<const U16Field&>(first);
			const U16Field& tmp2 = dynamic_cast<const U16Field&>(second);
			if (tmp1.getValue() == tmp2.getValue())
			{
				ret_val = true;
			}
		}
		break;

		case Field::U32_TYPE:
		{
			const U32Field& tmp1 = dynamic_cast<const U32Field&>(first);
			const U32Field& tmp2 = dynamic_cast<const U32Field&>(second);
			if (tmp1.getValue() == tmp2.getValue())
			{
				ret_val = true;
			}
		}
		break;

		case Field::U64_TYPE:
		{
			const U64Field& tmp1 = dynamic_cast<const U64Field&>(first);
			const U64Field& tmp2 = dynamic_cast<const U64Field&>(second);
			if (tmp1.getValue() == tmp2.getValue())
			{
				ret_val = true;
			}
		}
		break;

		default:
			throw Exception(MSG_ERROR, UNKNOWN_FIELD_TYPE,
					"Error with checking equality of fields with unknwon type");
		break;
	}

	return ret_val;

}


Field::FieldType InternalField::lookupType(const char* typeString)
{
	if (!typeString || StringUtil::stringLength(typeString) == 0)
	{
		throw Exception(FIELD_ERROR, UNKNOWN_FIELD_TYPE,
			"Field type string cannot be NULL, nor an empty string");
	}

	Field::FieldType type;

	if (StringUtil::stringEquals(typeString, "BOOL") || StringUtil::stringEquals(typeString, "BOOLEAN"))
	{
		type = Field::BOOL_TYPE;
	}
	else if (StringUtil::stringEquals(typeString, "BIN") || StringUtil::stringEquals(typeString, "BLOB"))
	{
		type = Field::BIN_TYPE;
	}
	else if (StringUtil::stringEquals(typeString, "CHAR"))
	{
		type = Field::CHAR_TYPE;
	}
	else if (StringUtil::stringEquals(typeString, "I8"))
	{
		type = Field::I8_TYPE;
	}
	else if (StringUtil::stringEquals(typeString, "I16") || StringUtil::stringEquals(typeString, "SHORT"))
	{
		type = Field::I16_TYPE;
	}
	else if (StringUtil::stringEquals(typeString, "I32") || StringUtil::stringEquals(typeString, "LONG"))
	{
		type = Field::I32_TYPE;
	}
	else if (StringUtil::stringEquals(typeString, "I64"))
	{
		type = Field::I64_TYPE;
	}
	else if (StringUtil::stringEquals(typeString, "F32") || StringUtil::stringEquals(typeString, "FLOAT"))
	{
		type = Field::F32_TYPE;
	}
	else if (StringUtil::stringEquals(typeString, "F64") || StringUtil::stringEquals(typeString, "DOUBLE"))
	{
		type = Field::F64_TYPE;
	}
	else if (StringUtil::stringEquals(typeString, "STRING"))
	{
		type = Field::STRING_TYPE;
	}
	else if (StringUtil::stringEquals(typeString, "U8"))
	{
		type = Field::U8_TYPE;
	}
	else if (StringUtil::stringEquals(typeString, "U16") || StringUtil::stringEquals(typeString, "USHORT"))
	{
		type = Field::U16_TYPE;
	}
	else if (StringUtil::stringEquals(typeString, "U32") || StringUtil::stringEquals(typeString, "ULONG"))
	{
		type = Field::U32_TYPE;
	}
	else if (StringUtil::stringEquals(typeString, "U64"))
	{
		type = Field::U64_TYPE;
	}
	else
	{
		std::ostringstream oss;
		oss << "Unsupported Field TYPE: " << typeString;
		throw Exception(FIELD_ERROR, UNKNOWN_FIELD_TYPE, oss.str().c_str());
	}

	return type;
}


std::string InternalField::lookupTypeStr(Field::FieldType type)
{
	std::string ret_string;

	switch (type)
	{
	  case Field::BOOL_TYPE:
		ret_string = "BOOL";
		break;

	  case Field::BIN_TYPE:
		ret_string = "BIN";
		break;

	  case Field::CHAR_TYPE:
		ret_string = "CHAR";
		break;

	  case Field::I8_TYPE:
		ret_string = "I8";
		break;

	  case Field::I16_TYPE:
		ret_string = "I16";
		break;

	  case Field::I32_TYPE:
		ret_string = "I32";
		break;

	  case Field::I64_TYPE:
		ret_string = "I64";
		break;

	  case Field::F32_TYPE:
		ret_string = "F32";
		break;

	  case Field::F64_TYPE:
		ret_string = "F64";
		break;

	  case Field::STRING_TYPE:
		ret_string = "STRING";
		break;

	  case Field::U8_TYPE:
		ret_string = "U8";
		break;

	  case Field::U16_TYPE:
		ret_string = "U16";
		break;

	  case Field::U32_TYPE:
		ret_string = "U32";
		break;

	  case Field::U64_TYPE:
		ret_string = "U64";
		break;

	  default:
		std::ostringstream oss;
		oss << "Unsupported Field TYPE: " << type;
		throw Exception(FIELD_ERROR, UNKNOWN_FIELD_TYPE, oss.str().c_str());

	}

	return ret_string;
}

} //namespace internal
} //namespace api
} //namespace gmsec
