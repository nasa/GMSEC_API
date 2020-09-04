/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec4/internal/mist/FieldTemplate.h>

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4/field/Field.h>

#include <gmsec4/Exception.h>

#include <gmsec4/util/Log.h>

#include <sstream>

#include <tinyxml2.h>


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


FieldTemplate::FieldTemplate()
	 :  m_xml(),
		m_name(),
		m_mode(),
		m_class(),
		m_value(),
		m_type(),
		m_description(),
		m_size()
{
}


FieldTemplate::FieldTemplate(const char* fieldName, 
							 const char* fieldMode, 
							 const char* fieldClass, 
							 const char* fieldValue, 
							 const char* fieldType, 
							 const char* fieldDescription)
							 :  m_xml(),
								m_name(fieldName),
								m_mode(fieldMode),
								m_class(fieldClass),
								m_value(fieldValue),
								m_type(fieldType),
								m_description(fieldDescription),
								m_size()
{
}


FieldTemplate::~FieldTemplate()
{
}


const char* FieldTemplate::getName() const
{
	return m_name.c_str();
}


const char* FieldTemplate::getMode() const
{
	return m_mode.c_str();
}


const char* FieldTemplate::getClass() const
{
	return m_class.c_str();
}


const char* FieldTemplate::getValue() const
{
	return m_value.c_str();
}


const char* FieldTemplate::getType() const
{
	return m_type.c_str();
}


const char* FieldTemplate::getDescription() const
{
	return m_description.c_str();
}


const char* FieldTemplate::getSize() const
{
	return m_size.c_str();
}


void FieldTemplate::setName(const char* name)
{
	m_name = name;
}


void FieldTemplate::setMode(const char* mode)
{
	m_mode = mode;
}


void FieldTemplate::setClass(const char* value)
{
	m_class = value;
}


void FieldTemplate::setValue(const char* value)
{
	m_value = value;
}


void FieldTemplate::setType(const char* type)
{
	m_type = type;
}


void FieldTemplate::setSize(const char* size)
{
	m_size = size;
}


void FieldTemplate::setDescription(const char* description)
{
	m_description = description;
}


bool FieldTemplate::isHeader() const
{
	if(m_class == "HEADER")
		return true;
	else
		return false;
}


const char* FieldTemplate::toXML() const
{

	if(m_mode == "CONTROL")
	{//control fields can't be converted to standard fields
		throw Exception(MIST_ERROR, INVALID_TYPE_CONVERSION, "FieldTemplate::toXML(): Control fields cannot be converted to standard fields");
	}

	std::ostringstream oss;

	oss << "<FIELD TYPE=\"" << (m_type=="UNSET" ? "STRING" : m_type) << "\" NAME=\"" << m_name << "\"" 
		<< (isHeader() ? " HEAD=\"T\"" : "") << ">"
		<< m_value
		<< "</FIELD>";

	m_xml = oss.str();

	return m_xml.c_str();
}


Field* FieldTemplate::toField() const
{
	tinyxml2::XMLDocument doc;
	doc.Parse(toXML());

	tinyxml2::XMLElement* element = doc.RootElement();

	//parsing the root element into a field
	return InternalField::fromXML(element);
}
