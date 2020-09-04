/*
 * Copyright 2007-2019 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec4/internal/mist/FieldTemplate.h>

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4/field/Field.h>

#include <gmsec4/Exception.h>

#include <gmsec4/util/Log.h>

#include <gmsec4/internal/StringUtil.h>

#include <sstream>

#include <tinyxml2.h>


using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::internal;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


FieldTemplate::FieldTemplate()
	: m_xml(),
	  m_name(),
	  m_modifiedName(),
	  m_mode(),
	  m_class(),
	  m_values(),
	  m_types(),
	  m_description(),
	  m_size()
{
}


FieldTemplate::FieldTemplate(const std::string& fieldName, 
							 const std::string& fieldMode, 
							 const std::string& fieldClass, 
							 const std::list<std::string>& fieldValues, 
							 const std::list<std::string>& fieldTypes, 
							 const std::string& fieldDescription)
	: m_xml(),
	  m_name(fieldName),
	  m_modifiedName(fieldName),
	  m_mode(fieldMode),
	  m_class(fieldClass),
	  m_values(fieldValues),
	  m_types(fieldTypes),
	  m_description(fieldDescription),
	  m_size()
{
}


FieldTemplate::FieldTemplate(const FieldTemplate& other)
	: m_xml(other.m_xml),
	  m_name(other.m_name),
	  m_modifiedName(other.m_modifiedName),
	  m_mode(other.m_mode),
	  m_class(other.m_class),
	  m_values(other.m_values),
	  m_types(other.m_types),
	  m_description(other.m_description),
	  m_size(other.m_size)
{
}


FieldTemplate::~FieldTemplate()
{
}


std::string FieldTemplate::getName() const
{
	return m_name;
}


std::string FieldTemplate::getModifiedName() const
{
	return m_modifiedName;
}


std::string FieldTemplate::getMode() const
{
	return m_mode;
}


std::string FieldTemplate::getClass() const
{
	return m_class;
}


std::list<std::string> FieldTemplate::getValues() const
{
	return m_values;
}


std::list<std::string> FieldTemplate::getTypeStrings() const
{
	return m_types;
}

std::list<Field::FieldType> FieldTemplate::getTypes() const
{
	std::list<Field::FieldType> list;
	for (std::list<std::string>::const_iterator it = m_types.begin(); it != m_types.end(); ++it)
	{
		std::string type = *it;

		if(StringUtil::stringEquals(type.c_str(), "CHAR"))
		{
			list.push_back(Field::CHAR_TYPE);
		}
		else if(StringUtil::stringEquals(type.c_str(), "BIN") || 
				StringUtil::stringEquals(type.c_str(), "BINARY") ||
				StringUtil::stringEquals(type.c_str(), "BLOB"))
		{
			list.push_back(Field::BIN_TYPE);
		}
		else if(StringUtil::stringEquals(type.c_str(), "BOOL") || 
				StringUtil::stringEquals(type.c_str(), "BOOLEAN"))
		{
			list.push_back(Field::BOOL_TYPE);
		}
		else if(StringUtil::stringEquals(type.c_str(), "I8"))
		{
			list.push_back(Field::I8_TYPE);
		}
		else if(StringUtil::stringEquals(type.c_str(), "I16") || 
				StringUtil::stringEquals(type.c_str(), "SHORT"))
		{
			list.push_back(Field::I16_TYPE);
		}
		else if(StringUtil::stringEquals(type.c_str(), "I32") || 
				StringUtil::stringEquals(type.c_str(), "LONG"))
		{
			list.push_back(Field::I32_TYPE);
		}
		else if(StringUtil::stringEquals(type.c_str(), "I64"))
		{
			list.push_back(Field::I64_TYPE);
		}
		else if(StringUtil::stringEquals(type.c_str(), "U8"))
		{
			list.push_back(Field::U8_TYPE);
		}
		else if(StringUtil::stringEquals(type.c_str(), "U16") || 
				StringUtil::stringEquals(type.c_str(), "USHORT"))
		{
			list.push_back(Field::U16_TYPE);
		}
		else if(StringUtil::stringEquals(type.c_str(), "U32") || 
				StringUtil::stringEquals(type.c_str(), "ULONG"))
		{
			list.push_back(Field::U32_TYPE);
		}
		else if(StringUtil::stringEquals(type.c_str(), "U64"))
		{
			list.push_back(Field::U64_TYPE);
		}
		else if(StringUtil::stringEquals(type.c_str(), "F32") || 
				StringUtil::stringEquals(type.c_str(), "FLOAT"))
		{
			list.push_back(Field::F32_TYPE);
		}
		else if(StringUtil::stringEquals(type.c_str(), "F64") || 
				StringUtil::stringEquals(type.c_str(), "DOUBLE"))
		{
			list.push_back(Field::F64_TYPE);
		}
		else if(StringUtil::stringEquals(type.c_str(), "STRING"))
		{
			list.push_back(Field::STRING_TYPE);
		}
	}

	return list;
}


std::string FieldTemplate::getDescription() const
{
	return m_description;
}


std::string FieldTemplate::getSize() const
{
	return m_size;
}

std::string FieldTemplate::getType() const
{
	return hasExplicitType() ? *m_types.begin() : "";
}

std::string FieldTemplate::getValue() const
{
	return hasExplicitValue() ? *m_values.begin() : "";
}

std::string FieldTemplate::getConcatenatedTypes() const
{
	if (!m_types.empty())
	{
		std::string types;

		for (std::list<std::string>::const_iterator it = m_types.begin(); it != m_types.end(); ++it)
		{
			if (it != m_types.begin())
			{
				types += ", ";
			}
			types += *it;
		}

		return types;
	}
	else
	{
		return "";
	}
}

std::string FieldTemplate::getConcatenatedValues() const
{
	if (!m_values.empty())
	{
		std::string values;

		for (std::list<std::string>::const_iterator it = m_values.begin(); it != m_values.end(); ++it)
		{
			if (it != m_values.begin())
			{
				values += ", ";
			}
			values += *it;
		}

		return values;
	}
	else
	{
		return "";
	}
}

std::string FieldTemplate::getArrayControlValue() const
{
	return StringUtil::stringEqualsIgnoreCase(m_mode.c_str(), "CONTROL") ? *m_values.begin() : "";
}

void FieldTemplate::setName(const std::string& name)
{
	m_name = name;
}


void FieldTemplate::setModifiedName(const std::string& name)
{
	m_modifiedName = name;
}


void FieldTemplate::setMode(const std::string& mode)
{
	m_mode = mode;
}


void FieldTemplate::setClass(const std::string& value)
{
	m_class = value;
}


void FieldTemplate::setValues(const std::list<std::string>& values)
{
	m_values = values;
}

void FieldTemplate::setValue(const std::string& value)
{
	m_values.clear();
	m_values.push_back(value);
}


void FieldTemplate::setTypes(const std::list<std::string>& types)
{
	m_types = types;
}


void FieldTemplate::setSize(const std::string& size)
{
	m_size = size;
}

void FieldTemplate::setDescription(const std::string& description)
{
	m_description = description;
}

bool FieldTemplate::hasExplicitType() const
{
	return m_types.size() == 1 && !(StringUtil::stringEqualsIgnoreCase(m_types.front().c_str(), "VARIABLE")) && !(StringUtil::stringEqualsIgnoreCase(m_types.front().c_str(), "UNSET"));
}

bool FieldTemplate::hasExplicitValue() const
{//to have an explicitly defined value, there can only be one value present that is not a range and the type must be explicitly defined
	if (m_values.size() == 1 && hasExplicitType())
	{
		const std::string& value = *m_values.begin();
		if (value.find("..") != std::string::npos && !StringUtil::stringEqualsIgnoreCase(getType().c_str(), "STRING"))
			return false;
		else if ((StringUtil::stringEquals(&value.at(value.length() - 1), "+") ||
				  StringUtil::stringEquals(&value.at(value.length() - 1), "-")) &&
				  !StringUtil::stringEqualsIgnoreCase(value.c_str(), "STRING"))
			return false;
		else
			return true;
	}
	else return false;
}

bool FieldTemplate::isTypeVariable() const
{
	bool variable = false;
	for(std::list<std::string>::const_iterator it = m_types.begin(); it != m_types.end(); ++it)
	{
		if(StringUtil::stringEqualsIgnoreCase(it->c_str(), "UNSET") || StringUtil::stringEqualsIgnoreCase(it->c_str(), "VARIABLE"))
		{
			variable = true;
		}
	}
	return variable;
}

bool FieldTemplate::isHeader() const
{
	return StringUtil::stringEqualsIgnoreCase(m_class.c_str(), "HEADER");
}


std::string FieldTemplate::toXML(const std::string& type) const
{

	if(m_mode == "CONTROL")
	{//control fields can't be converted to standard fields
		throw Exception(MIST_ERROR, INVALID_TYPE_CONVERSION, "FieldTemplate::toXML(): Control fields cannot be converted to standard fields");
	}

	std::ostringstream oss;

	oss << "<FIELD TYPE=\"" << (type=="UNSET" || type=="VARIABLE" ? "STRING" : type) << "\" NAME=\"" << m_name << "\"" 
		<< (isHeader() ? " HEAD=\"T\"" : "") << ">"
		<< (hasExplicitType() ? getValue() : "")
		<< "</FIELD>";

	m_xml = oss.str();

	return m_xml.c_str();
}


Field* FieldTemplate::toField(const std::string& type) const
{
	tinyxml2::XMLDocument doc;

	doc.Parse(toXML(type).c_str());

	//parsing the root element into a field
	return InternalField::fromXML(doc.RootElement());
}
