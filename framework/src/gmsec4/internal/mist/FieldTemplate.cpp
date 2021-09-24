/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec4/internal/mist/FieldTemplate.h>

#include <gmsec4/internal/mist/FieldTemplateDependency.h>

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4/Message.h>

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
	  m_modifiedMode(),
	  m_class(),
	  m_values(),
	  m_modifiedValues(),
	  m_pattern(),
	  m_modifiedPattern(),
	  m_types(),
	  m_modifiedTypes(),
	  m_description(),
	  m_size(),
	  m_prefix(),
	  m_children(),
	  m_dependencies()
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
	  m_modifiedMode(),
	  m_class(fieldClass),
	  m_values(fieldValues),
	  m_modifiedValues(),
	  m_pattern(),
	  m_modifiedPattern(),
	  m_types(fieldTypes),
	  m_modifiedTypes(),
	  m_description(fieldDescription),
	  m_size(),
	  m_prefix(),
	  m_children(),
	  m_dependencies()
{
}


FieldTemplate::FieldTemplate(const FieldTemplate& other)
	: m_xml(other.m_xml),
	  m_name(other.m_name),
	  m_modifiedName(other.m_modifiedName),
	  m_mode(other.m_mode),
	  m_modifiedMode(other.m_modifiedMode),
	  m_class(other.m_class),
	  m_values(other.m_values),
	  m_modifiedValues(other.m_modifiedValues),
	  m_pattern(other.m_pattern),
	  m_modifiedPattern(other.m_modifiedPattern),
	  m_types(other.m_types),
	  m_modifiedTypes(other.m_modifiedTypes),
	  m_description(other.m_description),
	  m_size(other.m_size),
	  m_prefix(other.m_prefix),
	  m_children(),
	  m_dependencies()
{
	// We need to make a deep-copy of the child FieldTemplate(s)
	for (FieldTemplateList::const_iterator it = other.m_children.begin(); it != other.m_children.end(); ++it)
	{
		m_children.push_back(new FieldTemplate(*(*it)));
	}

	for (FieldTemplateDependencyList::const_iterator it = other.m_dependencies.begin(); it != other.m_dependencies.end(); ++it)
	{
		m_dependencies.push_back(new FieldTemplateDependency(*(*it)));
	}
}


FieldTemplate::~FieldTemplate()
{
	for (FieldTemplateList::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		delete *it;
	}
	m_children.clear();

	for (FieldTemplateDependencyList::iterator it = m_dependencies.begin(); it != m_dependencies.end(); ++it)
	{
		delete *it;
	}
	m_dependencies.clear();
}


const std::string& FieldTemplate::getName() const
{
	return m_name;
}


const std::string& FieldTemplate::getModifiedName() const
{
	return m_modifiedName;
}


const std::string& FieldTemplate::getMode() const
{
	return (m_modifiedMode.empty() ? m_mode : m_modifiedMode);
}


const std::string& FieldTemplate::getClass() const
{
	return m_class;
}


const std::list<std::string>& FieldTemplate::getValues() const
{
	return (m_modifiedValues.empty() ? m_values : m_modifiedValues);
}

const std::string& FieldTemplate::getPattern() const
{
	return (m_modifiedPattern.empty() ? m_pattern : m_modifiedPattern);
}


const std::list<std::string>& FieldTemplate::getTypeStrings() const
{
	return (m_modifiedTypes.empty() ? m_types : m_modifiedTypes);
}

std::list<Field::FieldType> FieldTemplate::getTypes() const
{
	std::list<Field::FieldType> list;

	const std::list<std::string>& types = (m_modifiedTypes.empty() ? m_types : m_modifiedTypes);

	for (std::list<std::string>::const_iterator it = types.begin(); it != types.end(); ++it)
	{
		std::string type = *it;

		if (StringUtil::stringEquals(type.c_str(), "CHAR"))
		{
			list.push_back(Field::CHAR_TYPE);
		}
		else if (StringUtil::stringEquals(type.c_str(), "BIN") ||
			StringUtil::stringEquals(type.c_str(), "BINARY") ||
			StringUtil::stringEquals(type.c_str(), "BLOB"))
		{
			list.push_back(Field::BIN_TYPE);
		}
		else if (StringUtil::stringEquals(type.c_str(), "BOOL") ||
			StringUtil::stringEquals(type.c_str(), "BOOLEAN"))
		{
			list.push_back(Field::BOOL_TYPE);
		}
		else if (StringUtil::stringEquals(type.c_str(), "I8"))
		{
			list.push_back(Field::I8_TYPE);
		}
		else if (StringUtil::stringEquals(type.c_str(), "I16") ||
			StringUtil::stringEquals(type.c_str(), "SHORT"))
		{
			list.push_back(Field::I16_TYPE);
		}
		else if (StringUtil::stringEquals(type.c_str(), "I32") ||
			StringUtil::stringEquals(type.c_str(), "LONG"))
		{
			list.push_back(Field::I32_TYPE);
		}
		else if (StringUtil::stringEquals(type.c_str(), "I64"))
		{
			list.push_back(Field::I64_TYPE);
		}
		else if (StringUtil::stringEquals(type.c_str(), "U8"))
		{
			list.push_back(Field::U8_TYPE);
		}
		else if (StringUtil::stringEquals(type.c_str(), "U16") ||
			StringUtil::stringEquals(type.c_str(), "USHORT"))
		{
			list.push_back(Field::U16_TYPE);
		}
		else if (StringUtil::stringEquals(type.c_str(), "U32") ||
			StringUtil::stringEquals(type.c_str(), "ULONG"))
		{
			list.push_back(Field::U32_TYPE);
		}
		else if (StringUtil::stringEquals(type.c_str(), "U64"))
		{
			list.push_back(Field::U64_TYPE);
		}
		else if (StringUtil::stringEquals(type.c_str(), "F32") ||
			StringUtil::stringEquals(type.c_str(), "FLOAT"))
		{
			list.push_back(Field::F32_TYPE);
		}
		else if (StringUtil::stringEquals(type.c_str(), "F64") ||
			StringUtil::stringEquals(type.c_str(), "DOUBLE"))
		{
			list.push_back(Field::F64_TYPE);
		}
		else if (StringUtil::stringEquals(type.c_str(), "STRING"))
		{
			list.push_back(Field::STRING_TYPE);
		}
	}

	return list;
}


const std::string& FieldTemplate::getDescription() const
{
	return m_description;
}


const std::string& FieldTemplate::getSize() const
{
	return m_size;
}

const std::string& FieldTemplate::getPrefix() const
{
	return m_prefix;
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
	const std::list<std::string>& typeList = (m_modifiedTypes.empty() ? m_types : m_modifiedTypes);
	if (!typeList.empty())
	{
		std::string types;

		for (std::list<std::string>::const_iterator it = typeList.begin(); it != typeList.end(); ++it)
		{
			if (it != typeList.begin())
			{
				types += ", ";
			}
			types += *it;
		}

		return types;
	}

	return "";
}

std::string FieldTemplate::getConcatenatedValues() const
{
	const std::list<std::string>& valueList = (m_modifiedValues.empty() ? m_values : m_modifiedValues);
	if (!valueList.empty())
	{
		std::string values;

		for (std::list<std::string>::const_iterator it = valueList.begin(); it != valueList.end(); ++it)
		{
			if (it != valueList.begin())
			{
				values += ", ";
			}
			values += *it;
		}

		return values;
	}

	return "";
}

std::string FieldTemplate::getArrayControlValue() const
{
	return StringUtil::stringEqualsIgnoreCase(m_mode.c_str(), "CONTROL") ? *m_values.begin() : "";
}

const FieldTemplate::FieldTemplateList& FieldTemplate::getChildren() const
{
	return m_children;
}

void FieldTemplate::getAllChildren(FieldTemplateList& fields, const std::string& prefix)
{
	for (FieldTemplateList::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		FieldTemplate* child = *it;

		if (StringUtil::stringEqualsIgnoreCase(child->getMode().c_str(), "CONTROL") && child->hasChildren())
		{
			std::string newPrefix = prefix + m_prefix + ".1.";
			child->getAllChildren(fields,  newPrefix);
		}
		else
		{
			child->setModifiedName(prefix + m_prefix + ".1." + child->getName());
			fields.push_back(child);
		}
	}
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

void FieldTemplate::setPattern(const std::string & pattern)
{
	m_pattern = pattern;
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

void FieldTemplate::setPrefix(const std::string& prefix)
{
	m_prefix = prefix;
}

void FieldTemplate::setDescription(const std::string& description)
{
	m_description = description;
}

void FieldTemplate::setChildren(const FieldTemplateList& children)
{
	for (FieldTemplateList::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		delete *it;
	}

	// We need to make a deep-copy of the FieldTemplate(s)
	for (FieldTemplateList::const_iterator it = children.begin(); it != children.end(); ++it)
	{
		m_children.push_back(new FieldTemplate(*(*it)));
	}
}

void FieldTemplate::setDependencies(const FieldTemplateDependencyList& dependencies)
{
	for (FieldTemplateDependencyList::const_iterator it = m_dependencies.begin(); it != m_dependencies.end(); ++it)
	{
		delete *it;
	}

	for (FieldTemplateDependencyList::const_iterator it = dependencies.begin(); it != dependencies.end(); ++it)
	{
		m_dependencies.push_back(new FieldTemplateDependency(*(*it)));
	}
}

void FieldTemplate::addDependency(const char* name, const char* valueEquals, const char* valueGreaterThan, const char* valueLessThan, const char* newMode, const char* newPattern, const std::list<std::string>& newValues, const std::list<std::string>& newTypes)
{
	m_dependencies.push_back(new FieldTemplateDependency(name, valueEquals, valueGreaterThan, valueLessThan, newMode, newPattern, newValues, newTypes));
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

	return false;
}

bool FieldTemplate::isTypeVariable() const
{
	bool variable = false;
	for (std::list<std::string>::const_iterator it = m_types.begin(); it != m_types.end(); ++it)
	{
		if (StringUtil::stringEqualsIgnoreCase(it->c_str(), "UNSET") || StringUtil::stringEqualsIgnoreCase(it->c_str(), "VARIABLE"))
		{
			variable = true;
			break;
		}
	}
	return variable;
}

bool FieldTemplate::isHeader() const
{
	return StringUtil::stringEqualsIgnoreCase(m_class.c_str(), "HEADER");
}

bool FieldTemplate::hasChildren() const
{
	return !m_children.empty();
}

bool FieldTemplate::hasDependencies() const
{
	return !m_dependencies.empty();
}

void FieldTemplate::checkDependencies(const Message& message)
{
	for (FieldTemplateDependencyList::iterator it = m_dependencies.begin(); it != m_dependencies.end(); ++it)
	{
		FieldTemplateDependency* dependency = *it;

		std::string fieldName = dependency->getName();

		const Field* field = message.getField(fieldName.c_str());

		if (!field)
		{
			//field name not found as regular field, could be in array
			if (getModifiedName().find_last_of('.') != std::string::npos)
			{
				std::string prefix = getModifiedName().substr(0, getModifiedName().find_last_of('.'));

				fieldName = prefix + "." + dependency->getName();

				field = message.getField(fieldName.c_str());

				if (!field)
				{
					//check if field is in parent array
					while (prefix.find_last_of('.') != std::string::npos && prefix.substr(0, prefix.find_last_of('.')).find_last_of('.') != std::string::npos)
					{
						prefix = prefix.substr(0, prefix.find_last_of('.')); //remove nested index
						prefix = prefix.substr(0, prefix.find_last_of('.')); //remove nested name

						fieldName = prefix + "." + dependency->getName();

						field = message.getField(fieldName.c_str());
						
						if (field) break;
					}
				}
			}
		}

		if (field)
		{
			// If we have a field, dependency applies; otherwise nothing to do.

			switch (field->getType())
			{
			case Field::BIN_TYPE:
			case Field::CHAR_TYPE:
			case Field::STRING_TYPE:
				if (dependency->checkConditions(std::string(message.getStringValue(fieldName.c_str()))))
				{
					applyDependency(dependency);
				}
				break;

			case Field::I8_TYPE:
			case Field::I16_TYPE:
			case Field::I32_TYPE:
			case Field::I64_TYPE:
				if (dependency->checkConditions(message.getIntegerValue(fieldName.c_str())))
				{
					applyDependency(dependency);
				}
				break;

			case Field::U8_TYPE:
			case Field::U16_TYPE:
			case Field::U32_TYPE:
			case Field::U64_TYPE:
				if (dependency->checkConditions(message.getUnsignedIntegerValue(fieldName.c_str())))
				{
					applyDependency(dependency);
				}
				break;

			case Field::F32_TYPE:
			case Field::F64_TYPE:
				if (dependency->checkConditions(message.getDoubleValue(fieldName.c_str())))
				{
					applyDependency(dependency);
				}
				break;

			case Field::BOOL_TYPE:
				if (dependency->checkConditions(message.getBooleanField(fieldName.c_str()).getValue()))
				{
					applyDependency(dependency);
				}
				break;
			}
		}
	}
}

void FieldTemplate::applyDependency(const FieldTemplateDependency* dependency)
{
	if (!(dependency->getNewMode().empty()))
	{
		m_modifiedMode = dependency->getNewMode();
	}

	if (!(dependency->getNewPattern().empty()))
	{
		m_modifiedPattern = dependency->getNewPattern();
	}

	if (!(dependency->getNewTypes().empty()))
	{
		m_modifiedTypes = dependency->getNewTypes();
	}

	if (!(dependency->getNewValues().empty()))
	{
		m_modifiedValues = dependency->getNewValues();
	}

}

void FieldTemplate::resetDependencies()
{
	m_modifiedMode.clear();
	m_modifiedTypes.clear();
	m_modifiedValues.clear();
}


const std::string& FieldTemplate::toXML(const std::string& type) const
{

	if (StringUtil::stringEqualsIgnoreCase(m_mode.c_str(), "CONTROL"))
	{//control fields can't be converted to standard fields
		throw Exception(MIST_ERROR, INVALID_TYPE_CONVERSION, "FieldTemplate::toXML(): Control fields cannot be converted to standard fields");
	}

	std::ostringstream oss;

	oss << "<FIELD TYPE=\"" << (type=="UNSET" || type=="VARIABLE" ? "STRING" : type) << "\" NAME=\"" << m_name << "\"" 
		<< (isHeader() ? " HEAD=\"T\"" : "") << ">"
		<< (hasExplicitType() ? getValue() : "")
		<< "</FIELD>";

	m_xml = oss.str();

	return m_xml;
}


Field* FieldTemplate::toField(const std::string& type) const
{
	tinyxml2::XMLDocument doc;

	doc.Parse(toXML(type).c_str());

	//parsing the root element into a field
	return InternalField::fromXML(doc.RootElement());
}
