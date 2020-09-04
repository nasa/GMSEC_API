/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalMessageTemplate.cpp
 *
 *  @brief This file contains the InternalMessageTemplate class.
 *
**/

#include <gmsec4/internal/mist/MessageTemplate.h>

#include <gmsec4/Config.h>
#include <gmsec4/Errors.h>
#include <gmsec4/Fields.h>
#include <gmsec4/MessageFieldIterator.h>

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/util/Log.h>

#include <memory>
#include <string>
#include <sstream>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


MessageTemplate::MessageTemplate()
	: m_fieldTemplates(),
  	  m_id()
{
}

MessageTemplate::MessageTemplate(const char* schemaID, const gmsec::api::util::DataList<FieldTemplate>& inputFields)
	: m_fieldTemplates(inputFields),
	  m_id(schemaID)
{
}

MessageTemplate::MessageTemplate(const MessageTemplate& other)
	: m_fieldTemplates(other.listFieldTemplates()),
	  m_id(other.getID())
{
}

MessageTemplate::~MessageTemplate()
{
}

void MessageTemplate::setFieldTemplates(const char* schemaID, const gmsec::api::util::DataList<FieldTemplate>& inputFields)
{
	m_id = schemaID;
	m_fieldTemplates.clear();
	m_fieldTemplates = inputFields;
}

Field* MessageTemplate::getField(const char* name)
{
	Field* field = NULL;

	for(size_t i=0; i<m_fieldTemplates.size(); i++)
	{
		if(m_fieldTemplates.get(i).getName() == name)
		{
			field = m_fieldTemplates.get(i).toField();
			break;
		}
	}

	if(!field)
	{
		GMSEC_DEBUG << "The Message template " << m_id.c_str() 
					<< " could not find a field template with name " << name;
	}

	return field;
}

const FieldTemplate& MessageTemplate::getFieldTemplate(const char* name)
{
	for(FieldTemplateList::const_iterator it = m_fieldTemplates.begin(); it != m_fieldTemplates.end(); ++it)
	{
		const FieldTemplate& temp = *it;
		
		if(temp.getName() == name)
		{
			return temp;
		}
	}

	std::ostringstream err;
	err << "The Message template \"" << m_id.c_str() << "\" could not find a field template with name \"" << name << "\"";
	throw Exception(MIST_ERROR, FIELD_TEMPLATE_NOT_FOUND, err.str().c_str());
}

const FieldTemplate& MessageTemplate::getFieldTemplate(size_t index)
{
	if(index < m_fieldTemplates.size())
	{
		return m_fieldTemplates.get(index);
	}
	else
	{
		throw Exception(MIST_ERROR, INDEX_OUT_OF_RANGE, "Index is out of range");
	}
}

const char* MessageTemplate::getID() const
{
	return m_id.c_str();
}

const gmsec::api::util::DataList<FieldTemplate>& MessageTemplate::listFieldTemplates() const
{
	return m_fieldTemplates;
}

Message::MessageKind MessageTemplate::findKind(const char* schemaID)
{
	std::vector<std::string> id = StringUtil::split(schemaID, '.');
	std::string kind;

	for(std::vector<std::string>::const_iterator it = id.begin(); it != id.end(); ++it)
	{
		kind = *it;
		if(kind ==  "MSG")
		{
			return Message::PUBLISH;
		}
		else if(kind == "REQ")
		{
			return Message::REQUEST;
		}
		else if(kind == "RESP")
		{
			return Message::REPLY;
		}
	}

	std::ostringstream err;
	err << "MessageTemplate::findKind(): unable to determine the MessageKind \"" << kind.c_str() << "\"";
	throw Exception(MSG_ERROR, UNKNOWN_MSG_TYPE, err.str().c_str());

}

const char* MessageTemplate::toXML(const char* subject)
{
	//making a dummy message to leverage its toXML function
	Message msg(subject, findKind(m_id.c_str()));

	//field templates within an array control don't get added because the array size has not been defined yet.
	int arrayControlActive = 0;

	for(FieldTemplateList::const_iterator it = m_fieldTemplates.begin(); it != m_fieldTemplates.end(); ++it)
	{//add fields
		FieldTemplate fieldTemplate = *it;

		if(StringUtil::stringEquals(fieldTemplate.getMode(),  "CONTROL") && StringUtil::stringEquals(fieldTemplate.getName(), "ARRAY-START"))
		{
			arrayControlActive++;
		}
		else if(StringUtil::stringEquals(fieldTemplate.getMode(), "CONTROL") && StringUtil::stringEquals(fieldTemplate.getName(), "ARRAY-END"))
		{
			arrayControlActive--;
		}

		if(arrayControlActive == 0 && !StringUtil::stringEquals(fieldTemplate.getValue(), ""))
		{
			try
			{
				std::auto_ptr<Field> field(fieldTemplate.toField());

				msg.addField(*(field.get()));
			}
			catch(...)
			{//ignore control fields, do nothing
			}
		}
	}

	m_xml = msg.toXML();

	return m_xml.c_str();

}
