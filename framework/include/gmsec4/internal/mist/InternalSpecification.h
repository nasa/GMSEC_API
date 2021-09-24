/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalSpecification.h
 *
 *  @brief This file contains a class handle MessageTemplates and Message validation
 */

#ifndef GMSEC_MIST_INTERNAL_SPECIFICATION_H
#define GMSEC_MIST_INTERNAL_SPECIFICATION_H

#include <gmsec4/internal/mist/SchemaTemplate.h>

#include <gmsec4/internal/tinyxml2.h>
#include <gmsec4/internal/TrackingDetails.h>

#include <gmsec4/field/Field.h>

#include <gmsec4/mist/SchemaIDIterator.h>
#include <gmsec4/mist/Specification.h>

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
	class CustomMessageValidator;
	class FieldSpecification;
	class MessageSpecification;
	class MessageValidator;

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

	const char* const HEADER_STRING = "GMSEC_HEADER_STRING_PATTERN";
	const char* const TIME_STRING = "GMSEC_TIME_STRING_PATTERN";
	const char* const IP_ADDRESS = "GMSEC_IP_ADDRESS_PATTERN";


class InternalSpecification
{
public:
	typedef std::vector<FieldTemplate*> FieldTemplateList;
	typedef std::list<SchemaTemplate>   SchemaTemplateList;

	InternalSpecification(const Config& config);


	InternalSpecification(const InternalSpecification& other);


	~InternalSpecification();

	//if m_legacyTemplates is true, source is a path to be loaded
	//if m_legacyTemplates is false, source is an xml-formatted string to be parsed
	Status loadTemplate(const char* source, const char* fileName);
	std::string loadXSD(const char* path, const char* fileName);
	tinyxml2::XMLDocument* includeXSD(const char* path);


	void validateMessage(const Message& msg);


	SchemaIDIterator& getSchemaIDIterator();


	unsigned int getVersion() const;


	Specification::SchemaLevel getSchemaLevel() const;


	void setVersion(unsigned int version);


	const char* getTemplateXML(const char* subject, const char* schemaID);

	
	//INTERNAL USE ONLY helper function, look up the MessageTemplate with the matching schemaID
	MessageTemplate& findTemplate(const char* schemaID);


	const SchemaTemplateList& getDirectory() const;
	

	//INTERNAL USE ONLY helper function, copy the header field templates and populate them with the expected values given the message schema
	FieldTemplateList prepHeaders(const char* schemaID);


	const util::DataList<MessageSpecification*>& getMessageSpecifications() const;


	void registerMessageValidator(MessageValidator* validator);


	void registerMessageValidator(GMSEC_MessageValidator* validator);


private:
	friend class gmsec::api::mist::internal::InternalSchemaIDIterator;

	void cleanup();


	void load();


	void initMessageSpecifications();

	Status parser(tinyxml2::XMLElement* xmlSchema, std::string& schemaID, FieldTemplateList& schemaFields);

	FieldTemplateList parseXSD(const tinyxml2::XMLElement* xmlSchema, const std::string& schemaID, const std::list<tinyxml2::XMLDocument*>& included, std::string& errorList);
	FieldTemplate* parseField(const tinyxml2::XMLElement* schema, const std::string& schemaID, const std::list<tinyxml2::XMLDocument*>& included, const tinyxml2::XMLElement* element, std::string& errorList);
	bool parseEnumerations(std::list<std::string>& enumerations, std::string& pattern, const std::string& name, const std::string& schemaID, const tinyxml2::XMLElement* schema, const std::list<tinyxml2::XMLDocument*>& included);

	//helper function, checks a given value against the types defined in the field template
	//to make sure that the value is valid for all types given
	bool checkValue(const std::list<Field::FieldType>& types, const std::string& value) const;

	//helper function, get the string value from a field in a given message
	std::string getValue(const Message& msg, const char* name) const;


	//helper function, convert enumerated field type to string a human can actually read
	std::string getTypeStr(Field::FieldType type);

	//internal helper functions for checkValidation
	std::string registerTemplate(const Message& msg, GMSEC_I64 level);
	Status compare(const Message& msg, const FieldTemplateList& fields, const std::string& prefix, std::string& validFieldNames);
	Status validate(const Message& msg, const FieldTemplate& ftmp);
	Status validateContent(const Message& msg, const FieldTemplate& ftmp);
	std::string prepFieldName(const char* name, const std::list<std::string>& charList, const std::list<size_t>& indexList);


	//helper function for registerTemplate
	bool findDefinition(size_t schemaLevel, std::string& name, const Message& msg);


	//iterator functions
	void resetSchemaIndex();
	const char* nextID();
	bool hasNextID() const;


	FieldTemplateList cloneFieldTemplates(const FieldTemplateList& fields) const;


	bool isReservedField(const char* fieldName);
	Status checkTrackingField(const char* fieldName, const gmsec::api::internal::TrackingDetails& configTracking, const gmsec::api::internal::TrackingDetails& msgTracking);


	typedef std::map<std::string, std::string>       SchemaRegistry;       // message subject, schema ID
	typedef std::map<std::string, MessageTemplate*>  MessageTemplates;     // schema ID, MessageTemplate
	typedef std::map<std::string, FieldTemplateList> HeaderFieldTemplates; // header ID, FieldTemplateList
	typedef util::DataList<MessageSpecification*>    MessageSpecifications;

	const Config&						m_config;
	SchemaRegistry						m_registry;	
	MessageTemplates					m_templates;
	HeaderFieldTemplates				m_headers;	
	SchemaTemplateList					m_directory;
	std::string							m_specVersionStr;   //version as a string
	unsigned int                        m_specVersion;      //version as a number
	unsigned int						m_validationLevel;
	Specification::SchemaLevel          m_schemaLevel;
	bool								m_legacyTemplates;
	MessageTemplates::const_iterator	m_schemaIndex;
	SchemaIDIterator					m_iterator;
	std::string							m_basePath;
	std::string							m_lastHeaderName;

	gmsec::api::internal::TrackingDetails m_configTracking;

	MessageSpecifications               m_msgSpecs;

	MessageValidator*                   m_customMessageValidator;
	CustomMessageValidator*             m_ceeMessageValidator;

	//holding string for getTemplateXML, without it the function produces undefined behavior
	std::string							m_templateXML;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
