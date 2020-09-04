/*
 * Copyright 2007-2017 United States Government as represented by the
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
	 :  m_xml(),
		m_name(),
		m_mode(),
		m_class(),
		m_value(),
		m_types(),
		m_description(),
		m_size()
{
}


FieldTemplate::FieldTemplate(const std::string& fieldName, 
							 const std::string& fieldMode, 
							 const std::string& fieldClass, 
							 const std::string& fieldValue, 
							 const std::list<std::string>& fieldTypes, 
							 const std::string& fieldDescription)
							 :  m_xml(),
								m_name(fieldName),
								m_mode(fieldMode),
								m_class(fieldClass),
								m_value(fieldValue),
								m_types(fieldTypes),
								m_description(fieldDescription),
								m_size()
{
}


FieldTemplate::~FieldTemplate()
{
}


std::string FieldTemplate::getName() const
{
	return m_name.c_str();
}


std::string FieldTemplate::getMode() const
{
	return m_mode.c_str();
}


std::string FieldTemplate::getClass() const
{
	return m_class.c_str();
}


std::string FieldTemplate::getValue() const
{
	return m_value.c_str();
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
	return m_description.c_str();
}


std::string FieldTemplate::getSize() const
{
	return m_size.c_str();
}

std::string FieldTemplate::getType() const
{
	return *m_types.begin();
}

void FieldTemplate::setName(const std::string& name)
{
	m_name = name;
}


void FieldTemplate::setMode(const std::string& mode)
{
	m_mode = mode;
}


void FieldTemplate::setClass(const std::string& value)
{
	m_class = value;
}


void FieldTemplate::setValue(const std::string& value)
{
	m_value = value;
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
	if(m_types.size() == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool FieldTemplate::hasExplicitValue() const
{
	return !m_value.empty();
}

bool FieldTemplate::isTypeVariable() const
{
	bool variable = false;
	for(std::list<std::string>::const_iterator it = m_types.begin(); it != m_types.end(); ++it)
	{
		std::string type = *it;
		if(StringUtil::stringEquals(type.c_str(), "UNSET") || StringUtil::stringEquals(type.c_str(), "VARIABLE"))
		{
			variable = true;
		}
	}
	return variable;
}

bool FieldTemplate::isHeader() const
{
	if(m_class == "HEADER")
		return true;
	else
		return false;
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
		<< m_value
		<< "</FIELD>";

	m_xml = oss.str();

	return m_xml.c_str();
}


Field* FieldTemplate::toField(const std::string& type) const
{
	tinyxml2::XMLDocument doc;
	doc.Parse(toXML(type).c_str());

	tinyxml2::XMLElement* element = doc.RootElement();

	//parsing the root element into a field
	return InternalField::fromXML(element);
}
