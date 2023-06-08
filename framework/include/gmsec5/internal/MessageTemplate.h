/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file MessageTemplate.h
 *
 *  @brief This file contains a class define a Message type
 */

#ifndef GMSEC_API5_INTERNAL_MESSAGE_TEMPLATE_H
#define GMSEC_API5_INTERNAL_MESSAGE_TEMPLATE_H

#include <gmsec5/internal/FieldTemplate.h>

#include <gmsec5/Message.h>

#include <gmsec5/util/wdllexp.h>

#include <iostream>
#include <string>
#include <vector>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Field;

namespace internal
{
	// Forward declaration(s)
	class InternalSpecification;

class GMSEC_API MessageTemplate
{
public:
	typedef std::vector<FieldTemplate*> FieldTemplateList;

	//SubjectElement
	//first string: name of the element
	//second string: value of the element (name of the field that contains the value)
	typedef std::pair<std::string, std::string> SubjectElement;
	typedef std::vector<SubjectElement>         SubjectElementList;

	MessageTemplate(const char* schemaID, const FieldTemplateList& inputFields, unsigned int schemaLevel);

	MessageTemplate(const MessageTemplate& other);

	~MessageTemplate();

	MessageTemplate& operator=(const MessageTemplate& other);

	//Looks for a FieldTemplate with a matching name and returns it as a Field*
	Field* getField(const char* name, const char* type);

	//Looks for a FieldTemplate with a matching name and returns it as a FieldTemplate
	const FieldTemplate& getFieldTemplate(const char* name);

	const char* getSchemaID() const;

	std::string getSubjectNames() const;
	std::string getSubjectDefinition() const;
	SubjectElementList getSubjectElements() const;
	void setSubject(const SubjectElementList& subject);
	void addSubjectHeader(const SubjectElementList& header);

	const std::string getDefinition() const;
	void setDefinition(const std::string& definition);

	const FieldTemplateList& getFieldTemplates() const;

	unsigned int getSchemaLevel() const;


	//convenience method to display the info associated with the FieldTemplate
	//usage: std::cout << ftmp << std::endl
	friend std::ostream& operator<<(std::ostream& os, const MessageTemplate& msgTemp)
	{
		os << "ID          : " << msgTemp.m_schemaID      << "\n"
		   << "ID Definition: " << msgTemp.m_definition << "\n"
		   << "Subject Element Names: " << msgTemp.getSubjectNames() << "\n"
		   << "Subject Definition: " << msgTemp.getSubjectDefinition() << "\n"
		   << "Schema Level: " << msgTemp.m_schemaLevel   << "\n";

		for (FieldTemplateList::const_iterator it = msgTemp.m_fieldTemplates.begin(); it != msgTemp.m_fieldTemplates.end(); ++it)
		{
			const FieldTemplate* tmp = *it;
			os << *tmp << "\n";
		}

		return os;
	}

private:
	// Defined, but not implemented.
	MessageTemplate();

	//helper method to detroy resources in FieldTemplateList.
	void cleanup();

	//helper function, figure out the kind from the schema ID
	Message::Kind findKind(const char* schemaID);

	std::string		   m_schemaID;
	std::string		   m_definition;
	SubjectElementList m_subject;
	FieldTemplateList  m_fieldTemplates;

	unsigned int       m_schemaLevel;
};

} //end namespace internal
} //end namespace api5
} //end namespace gmsec

#endif
