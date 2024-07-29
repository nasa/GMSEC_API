/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalSpecification.h
 *
 *  @brief This file contains a class that handles MessageTemplates
 */

#ifndef GMSEC_API5_INTERNAL_SPECIFICATION_H
#define GMSEC_API5_INTERNAL_SPECIFICATION_H

#include <gmsec5/internal/SchemaTemplate.h>
#include <gmsec5/internal/TrackingDetails.h>
#include <gmsec5/internal/tinyxml2.h>

#include <gmsec5/SchemaIDIterator.h>
#include <gmsec5/Specification.h>
#include <gmsec5/Status.h>

#include <gmsec5/field/Field.h>

#include <gmsec5/util/List.h>

#include <map>
#include <list>
#include <set>
#include <string>
#include <vector>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Config;
	class Message;
	class MessageValidator;
	class FieldSpecification;
	class MessageSpecification;

namespace internal
{
	// Forward declaration(s)
	class CustomMessageValidator;
	class FieldTemplate;
	class MessageTemplate;

	const char* const HIDDEN_DIRECTORY_FILE = ".DIRECTORY.xml";
	const char* const DIRECTORY_FILE        = "DIRECTORY.xml";
	const char* const HEADER_FILE           = "Header.xsd";
	const char* const DEFAULTS_FILE         = "Defaults.xsd";
	const char* const FIELDS_FILE           = "Fields.xsd";
	const char* const SUBJECTS_FILE			= "Subjects.xsd";
	const char* const TRANSFORM_FILE        = "Stylesheet.xsl";


class InternalSpecification
{
public:
	typedef std::vector<FieldTemplate*> FieldTemplateList;
	typedef std::list<SchemaTemplate*>   SchemaTemplateList;

	//SubjectElement
	//first string: name of the element
	//second string: value of the element (name of the field that contains the value)
	typedef std::pair<std::string, std::string> SubjectElement;
	typedef std::vector<SubjectElement> SubjectElementList;

	InternalSpecification(const Config& config);


	InternalSpecification(const InternalSpecification& other);


	~InternalSpecification();

	//if m_legacyTemplates is true, source is a path to be loaded
	//if m_legacyTemplates is false, source is an xml-formatted string to be parsed
	Status loadTemplate(const char* source, const char* fileName);
	std::string loadXSD(const char* path, const char* fileName);
	tinyxml2::XMLDocument* includeXSD(const char* path);


	SchemaIDIterator& getSchemaIDIterator() const;


	unsigned int getVersion() const;

	const util::List<const char*> getHeaderFieldNames(const char* schemaName) const;

	Specification::SchemaLevel getSchemaLevel() const;


	//INTERNAL USE ONLY helper function, look up the MessageTemplate with the matching schemaID
	MessageTemplate& findTemplate(const char* schemaID);


	const SchemaTemplateList& getDirectory() const;
	

	//INTERNAL USE ONLY helper function, copy the header field templates and populate them with the expected values given the message schema
	FieldTemplateList prepHeaders(const char* schemaID);


	const util::List<MessageSpecification*>& getMessageSpecifications() const;

private:

	friend class gmsec::api5::internal::InternalSchemaIDIterator;

	void cleanup();


	void load();


	void initMessageSpecifications();

	Status parser(tinyxml2::XMLElement* xmlSchema, std::string& schemaID, FieldTemplateList& schemaFields);

	FieldTemplateList parseXSD(const tinyxml2::XMLElement* xmlSchema, const std::string& schemaID, const std::list<tinyxml2::XMLDocument*>& included, std::string& errorList, SubjectElementList& subjectElements);
	FieldTemplate* parseField(const tinyxml2::XMLElement* schema, const std::string& schemaID, const std::list<tinyxml2::XMLDocument*>& included, const tinyxml2::XMLElement* element, std::string& errorList, std::string& refNameList);
	bool parseEnumerations(std::list<std::string>& enumerations, std::string& pattern, const std::string& name, const std::string& schemaID, const tinyxml2::XMLElement* schema, const std::list<tinyxml2::XMLDocument*>& included);

	//helper function, checks a given value against the types defined in the field template
	//to make sure that the value is valid for all types given
	bool checkValue(const std::list<Field::Type>& types, const std::string& value) const;

	//helper function, get the string value from a field in a given message
	std::string getValue(const Message& msg, const char* name) const;


	//helper function for registerTemplate
	bool findDefinition(int schemaLevel, std::string& name, const Message& msg);


	//iterator functions
	void resetSchemaIndex();
	const char* nextID();
	bool hasNextID() const;


	FieldTemplateList cloneFieldTemplates(const FieldTemplateList& fields) const;


	typedef std::map<std::string, std::string>       SchemaRegistry;         // message subject, schema ID
	typedef std::map<std::string, MessageTemplate*>  MessageTemplates;       // schema ID, MessageTemplate
	typedef std::map<std::string, FieldTemplateList> HeaderFieldTemplates;   // header ID, FieldTemplateList
	typedef util::List<MessageSpecification*>        MessageSpecifications;

	const Config&                    m_config;
	SchemaRegistry                   m_registry;   //list of registered message subjects and their schema IDs
	MessageTemplates                 m_templates;
	HeaderFieldTemplates             m_headers;
	SchemaTemplateList               m_directory; 
	std::string                      m_specVersionStr;   //version as a string
	unsigned int                     m_specVersion;      //version as a number
	Specification::SchemaLevel       m_schemaLevel;
	MessageTemplates::const_iterator m_schemaIndex;
	SchemaIDIterator                 m_iterator;
	std::string                      m_basePath;
	std::string                      m_lastHeaderName;
	SubjectElementList               m_subjectHeader;

	MessageSpecifications            m_msgSpecs;

	//holding string for getTemplateXML, without it the function produces undefined behavior
	std::string                      m_templateXML;
};

} //end namespace internal
} //end namespace api5
} //end namespace gmsec

#endif
