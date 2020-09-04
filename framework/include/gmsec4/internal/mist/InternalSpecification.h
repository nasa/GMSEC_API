/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalSpecification.h
 *
 *  @brief This file contains a class handle MessageTemplates and Message validation
 *
**/

#ifndef GMSEC_MIST_INTERNAL_SPECIFICATION_H
#define GMSEC_MIST_INTERNAL_SPECIFICATION_H

#include <gmsec4/internal/mist/SchemaTemplate.h>

#include <gmsec4/internal/tinyxml2.h>

#include <gmsec4/field/Field.h>

#include <gmsec4/mist/SchemaIDIterator.h>

#include <gmsec4/util/DataList.h>

#include <gmsec4/Status.h>

#include <map>
#include <list>
#include <vector>
#include <string>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Config;
	class Message;

namespace mist
{
	// Forward declaration(s)
	class MessageSpecification;

namespace internal
{
	// Forward declaration(s)
	class FieldTemplate;
	class MessageTemplate;

	const char* const DIRECTORY_FILE = ".DIRECTORY.xml";
	const char* const HEADER_FILE = "Header.xsd";
	const char* const DEFAULTS_FILE = "Defaults.xsd";
	const char* const FIELDS_FILE = "Fields.xsd";
	const char* const TRANSFORM_FILE = "Stylesheet.xsl";


class GMSEC_API InternalSpecification
{
public:
	typedef std::vector<FieldTemplate*> FieldTemplateList;
	typedef std::list<SchemaTemplate>   SchemaTemplateList;

	InternalSpecification(const Config& config);


	InternalSpecification(const InternalSpecification& other);


	~InternalSpecification();

	//if m_legacyTemplates is true, source is a path to be loaded
	//if m_legacyTemplates is false, source is an xml-formatted string to be parsed
	Status CALL_TYPE loadTemplate(const char* source, const char* fileName);


	void CALL_TYPE validateMessage(const Message& msg);


	SchemaIDIterator& getSchemaIDIterator();


	unsigned int getVersion() const;


	void setVersion(unsigned int version);


	const char* getTemplateXML(const char* subject, const char* schemaID);

	
	//INTERNAL USE ONLY helper function, look up the MessageTemplate with the matching schemaID
	MessageTemplate& findTemplate(const char* schemaID);


	bool checkValidSpec(unsigned int specVersionInt);

	const SchemaTemplateList& getDirectory() const;
	

	//INTERNAL USE ONLY helper function, copy the header field templates and populate them with the expected values given the message schema
	FieldTemplateList CALL_TYPE prepHeaders(const char* schemaID);


	const util::DataList<MessageSpecification*>& getMessageSpecifications() const;


private:
	friend class gmsec::api::mist::internal::InternalSchemaIDIterator;

	void CALL_TYPE cleanup();


	void CALL_TYPE load();


	void initMessageSpecifications();


	Status CALL_TYPE parser(tinyxml2::XMLElement* xmlSchema, 
	                        std::string& schemaID, 
							FieldTemplateList& schemaFields);

	//helper function, checks a given value against the types defined in the field template
	//to make sure that the value is valid for all types given
	bool checkValue(std::list<Field::FieldType> types, std::string value);

	//helper function, get the string value from a field in a given message
	std::string CALL_TYPE getValue(const Message& msg, const char* name) const;


	//helper function, convert enumerated field type to string a human can actually read
	std::string CALL_TYPE getTypeStr(Field::FieldType type);

	//internal helper functions for checkValidation
	std::string CALL_TYPE registerTemplate(const Message& msg, GMSEC_I64 level);
	Status CALL_TYPE compare(const Message& msg, const FieldTemplateList& fields);
	Status CALL_TYPE validate(const Message& msg, const FieldTemplate& fmtp);
	std::string prepFieldName(const char* name, const std::list<std::string>& charList, const std::list<size_t>& indexList);


	//helper function for registerTemplate
	bool CALL_TYPE findDefinition(size_t schemaLevel, std::string& name, const Message& msg);


	//iterator functions
	void CALL_TYPE resetSchemaIndex();
	const char* CALL_TYPE nextID();
	bool CALL_TYPE hasNextID() const;


	FieldTemplateList cloneFieldTemplates(const FieldTemplateList& fields) const;


	typedef std::map<std::string, std::string>       SchemaRegistry;       // message subject, schema ID
	typedef std::map<std::string, MessageTemplate*>  MessageTemplates;     // schema ID, MessageTemplate
	typedef std::map<std::string, FieldTemplateList> HeaderFieldTemplates; // header ID, FieldTemplateList
	typedef util::DataList<MessageSpecification*>    MessageSpecifications;

	const Config&						m_config;
	SchemaRegistry						m_registry;	
	MessageTemplates					m_templates;
	HeaderFieldTemplates				m_headers;	
	SchemaTemplateList					m_directory;
	std::string							m_specificationId; //version
	unsigned int						m_validationLevel;
	unsigned int						m_schemaLevel;
	bool								m_legacyTemplates;
	MessageTemplates::const_iterator	m_schemaIndex;
	SchemaIDIterator					m_iterator;
	std::string							m_basePath;
	std::string							m_lastHeaderName;

	MessageSpecifications               m_msgSpecs;

	//holding string for getTemplateXML, without it the function produces undefined behavior
	std::string							m_templateXML;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
