/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file MessageTemplate.cpp
 *
 *  @brief This file contains the InternalMessageTemplate class.
 */

#include <gmsec5/internal/MessageTemplate.h>

#include <gmsec5/internal/FieldTemplate.h>

#include <gmsec5/internal/StringUtil.h>

#include <gmsec5/Config.h>
#include <gmsec5/Errors.h>
#include <gmsec5/Fields.h>
#include <gmsec5/MessageFieldIterator.h>

#include <gmsec5/util/Log.h>
#include <gmsec5/util/StdUniquePtr.h>

#include <string>
#include <sstream>

using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


MessageTemplate::MessageTemplate(const char* schemaID, const FieldTemplateList& inputFields, unsigned int schemaLevel)
	: m_schemaID(schemaID),
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
	: m_schemaID(other.m_schemaID),
	  m_definition(other.m_definition),
	  m_subject(other.m_subject),
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

		m_schemaID = other.m_schemaID;
		m_subject = other.m_subject;
		m_definition = other.m_definition;

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
		GMSEC_DEBUG << "The Message template " << m_schemaID.c_str() 
					<< " could not find a field template with name " << name;
	}

	return field;
}


const FieldTemplate& MessageTemplate::getFieldTemplate(const char* name) const
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
	err << "The Message template \"" << m_schemaID.c_str() << "\" could not find a field template with name \"" << name << "\"";
	throw GmsecException(MSG_TEMPLATE_ERROR, FIELD_TEMPLATE_NOT_FOUND, err.str().c_str());
}


const char* MessageTemplate::getSchemaID() const
{
	return m_schemaID.c_str();
}


std::string MessageTemplate::getSubjectNames() const
{
	std::string subject;
	for (SubjectElementList::const_iterator it = m_subject.begin(); it != m_subject.end(); ++it)
	{
		if (it != m_subject.begin()) subject.append(".");
		subject.append(it->first);
	}
	return subject;
}


std::string MessageTemplate::getSubjectDefinition() const
{
	std::string subject;
	for (SubjectElementList::const_iterator it = m_subject.begin(); it != m_subject.end(); ++it)
	{
		if (it != m_subject.begin()) subject.append(".");
		subject.append(it->second);
	}
	return subject;
}


MessageTemplate::SubjectElementList MessageTemplate::getSubjectElements() const
{
	return m_subject;
}


void MessageTemplate::setSubject(const SubjectElementList& subject)
{
	m_subject = subject;
}


void MessageTemplate::addSubjectHeader(const SubjectElementList& header)
{
	m_subject.insert(m_subject.begin(), header.begin(), header.end());
}


const std::string MessageTemplate::getDefinition() const
{
	return m_definition.c_str();
}


void MessageTemplate::setDefinition(const std::string& definition)
{
	m_definition = definition;
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


Message::Kind MessageTemplate::findKind(const char* schemaID)
{
	std::vector<std::string> id = StringUtil::split(schemaID, '.');
	std::string kind;

	for (std::vector<std::string>::const_iterator it = id.begin(); it != id.end(); ++it)
	{
		kind = *it;

		if (kind ==  "MSG")
		{
			return Message::Kind::PUBLISH;
		}
		else if(kind == "REQ")
		{
			return Message::Kind::REQUEST;
		}
		else if(kind == "RESP")
		{
			return Message::Kind::REPLY;
		}
	}

	std::ostringstream err;
	err << "MessageTemplate::findKind(): unable to determine the Message Kind \"" << kind.c_str() << "\"";
	throw GmsecException(MSG_ERROR, UNKNOWN_MSG_TYPE, err.str().c_str());
}
