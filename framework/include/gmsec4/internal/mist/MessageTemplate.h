/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalMessageTemplate.h
 *
 *  @brief This file contains a class define a Message type
 *
**/

#ifndef GMSEC_MIST_INTERNAL_MESSAGE_TEMPLATE_H
#define GMSEC_MIST_INTERNAL_MESSAGE_TEMPLATE_H

#include <gmsec4/Message.h>

#include <gmsec4/internal/mist/FieldTemplate.h>

#include <gmsec4/util/DataList.h>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Field;

namespace mist
{
	// Forward declaration(s)
	namespace internal
	{
		class InternalSpecification;
	}

namespace internal
{

class GMSEC_API MessageTemplate
{
public:
	MessageTemplate();
	MessageTemplate(const char* schemaID, const gmsec::api::util::DataList<FieldTemplate>& inputFields);
	MessageTemplate(const MessageTemplate& other);

	~MessageTemplate();
	
	//Looks for a FieldTemplate with a matching name and returns it as a Field*
	Field* getField(const char* name);

	//Looks for a FieldTemplate with a matching name and returns it as a FieldTemplate
	const FieldTemplate& getFieldTemplate(const char* name);
	const FieldTemplate& getFieldTemplate(size_t index);

	const char* getID() const;

	 //Returns the template's FieldTemplates as a GMSEC DataList
	const gmsec::api::util::DataList<FieldTemplate>& listFieldTemplates() const;

	//returns an xml that can put used for a message data constructor
	const char* toXML(const char* subject);

private:
	//helper function, takes a referenced list of FieldTemplates and stores them as a copy.
	//the template is also assigned a new ID.
	void setFieldTemplates(const char* schemaID, const gmsec::api::util::DataList<FieldTemplate>& inputFields);

	//helper function, figure out the kind form the schema ID
	Message::MessageKind findKind(const char* schemaID);

	typedef gmsec::api::util::DataList<FieldTemplate> FieldTemplateList;

	FieldTemplateList m_fieldTemplates;
	std::string		  m_id;
	std::string		  m_xml;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
