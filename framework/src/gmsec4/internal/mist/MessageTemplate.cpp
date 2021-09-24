/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec4/internal/mist/FieldTemplate.h>

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/Config.h>
#include <gmsec4/Errors.h>
#include <gmsec4/Fields.h>
#include <gmsec4/MessageFieldIterator.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/StdUniquePtr.h>

#include <string>
#include <sstream>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


MessageTemplate::MessageTemplate(const char* schemaID, const FieldTemplateList& inputFields, unsigned int schemaLevel)
	: m_id(schemaID),
	  m_fieldTemplates(),
	  m_schemaLevel(schemaLevel)
{
	// We need to make a deep-copy of the FieldTemplate(s)
	for (FieldTemplateList::const_iterator it = inputFields.begin(); it != inputFields.end(); ++it)
	{
		m_fieldTemplates.push_back(new FieldTemplate(*(*it)));
	}
}


MessageTemplate::MessageTemplate(const MessageTemplate& other)
	: m_id(other.m_id),
	  m_fieldTemplates(),
	  m_schemaLevel(other.m_schemaLevel)
{
	// We need to make a deep-copy of the FieldTemplate(s)
	for (FieldTemplateList::const_iterator it = other.m_fieldTemplates.begin(); it != other.m_fieldTemplates.end(); ++it)
	{
		m_fieldTemplates.push_back(new FieldTemplate(*(*it)));
	}
}


MessageTemplate::~MessageTemplate()
{
	cleanup();
}


MessageTemplate& MessageTemplate::operator=(const MessageTemplate& other)
{
	if (this != &other)
	{
		cleanup();

		m_id = other.m_id;

		// We need to make a deep-copy of the FieldTemplate(s)
		for (FieldTemplateList::const_iterator it = other.m_fieldTemplates.begin(); it != other.m_fieldTemplates.end(); ++it)
		{
			m_fieldTemplates.push_back(new FieldTemplate(*(*it)));
		}

		m_schemaLevel = other.m_schemaLevel;
	}

	return *this;
}


Field* MessageTemplate::getField(const char* name, const char* type)
{
	Field* field = NULL;

	for (FieldTemplateList::const_iterator it = m_fieldTemplates.begin(); it != m_fieldTemplates.end(); ++it)
	{
		FieldTemplate* temp = *it;

		if (StringUtil::stringEquals(temp->getName().c_str(), name))
		{
			field = temp->toField(type);
			break;
		}
	}

	if (!field)
	{
		GMSEC_DEBUG << "The Message template " << m_id.c_str() 
					<< " could not find a field template with name " << name;
	}

	return field;
}


const FieldTemplate& MessageTemplate::getFieldTemplate(const char* name)
{
	for (FieldTemplateList::const_iterator it = m_fieldTemplates.begin(); it != m_fieldTemplates.end(); ++it)
	{
		const FieldTemplate* temp = *it;
		
		if (temp->getName() == name)
		{
			return *temp;
		}
	}

	std::ostringstream err;
	err << "The Message template \"" << m_id.c_str() << "\" could not find a field template with name \"" << name << "\"";
	throw Exception(MIST_ERROR, FIELD_TEMPLATE_NOT_FOUND, err.str().c_str());
}


const char* MessageTemplate::getID() const
{
	return m_id.c_str();
}


const MessageTemplate::FieldTemplateList& MessageTemplate::getFieldTemplates() const
{
	return m_fieldTemplates;
}


unsigned int MessageTemplate::getSchemaLevel() const
{
	return m_schemaLevel;
}


void MessageTemplate::cleanup()
{
	for (FieldTemplateList::iterator it = m_fieldTemplates.begin(); it != m_fieldTemplates.end(); ++it)
	{
		delete *it;
	}
	m_fieldTemplates.clear();
}


Message::MessageKind MessageTemplate::findKind(const char* schemaID)
{
	std::vector<std::string> id = StringUtil::split(schemaID, '.');
	std::string kind;

	for (std::vector<std::string>::const_iterator it = id.begin(); it != id.end(); ++it)
	{
		kind = *it;

		if (kind ==  "MSG")
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

	for (FieldTemplateList::const_iterator it = m_fieldTemplates.begin(); it != m_fieldTemplates.end(); ++it)
	{
		//add fields
		FieldTemplate* fieldTemplate = *it;

		if (StringUtil::stringEquals(fieldTemplate->getMode().c_str(),  "CONTROL") && StringUtil::stringEquals(fieldTemplate->getName().c_str(), "ARRAY-START"))
		{
			arrayControlActive++;
		}
		else if (StringUtil::stringEquals(fieldTemplate->getMode().c_str(), "CONTROL") && StringUtil::stringEquals(fieldTemplate->getName().c_str(), "ARRAY-END"))
		{
			arrayControlActive--;
		}

		if (arrayControlActive == 0 && fieldTemplate->hasExplicitType() && fieldTemplate->hasExplicitValue())
		{
			try
			{
				StdUniquePtr<Field> field(fieldTemplate->toField(fieldTemplate->getType()));

				msg.addField(*(field.get()));
			}
			catch(...)
			{
				//ignore control fields, do nothing
			}
		}
	}

	m_xml = msg.toXML();

	return m_xml.c_str();
}
