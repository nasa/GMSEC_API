/*
 * Copyright 2007-2016 United States Government as represented by the
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

#include <gmsec4/internal/mist/FieldTemplate.h>
#include <gmsec4/internal/mist/SchemaTemplate.h>

#include <gmsec4/internal/tinyxml2.h>

#include <gmsec4/field/Field.h>

#include <gmsec4/mist/SchemaIDIterator.h>

#include <gmsec4/util/DataList.h>

#include <gmsec4/Status.h>

#include <map>
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
namespace internal
{
	// Forward declaration(s)
	class MessageTemplate;


class GMSEC_API InternalSpecification
{
public:
	InternalSpecification(const Config& config);


	InternalSpecification(const InternalSpecification& other);


	~InternalSpecification();


	Status CALL_TYPE loadTemplate(const char* path);


	void CALL_TYPE validateMessage(const Message& msg);


	SchemaIDIterator& getSchemaIDIterator();


	unsigned int getVersion() const;


	void setVersion(unsigned int version);


	const char* getTemplateXML(const char* subject, const char* schemaID);

	
	//INTERNAL USE ONLY helper function, look up the MessageTemplate with the matching schemaID
	MessageTemplate findTemplate(const char* schemaID);


	bool checkValidSpec(unsigned int specVersionInt);

	const gmsec::api::util::DataList<SchemaTemplate>& getDirectory() const;
	

	//INTERNAL USE ONLY helper function, copy the header field templates and populate them with the expected values given the message schema
	gmsec::api::util::DataList<FieldTemplate> CALL_TYPE prepHeaders(const char* schemaID);


private:
	friend class gmsec::api::mist::internal::InternalSchemaIDIterator;

	void CALL_TYPE load();


	Status CALL_TYPE parser(tinyxml2::XMLElement* xmlSchema, 
	                        std::string& schemaID, 
	                        gmsec::api::util::DataList<FieldTemplate>& schemaFields);


	//helper function, get the int value from a field in a given message
	size_t CALL_TYPE getIntValue(const Message& msg, const char* name) const;


	//helper function, get the string value from a field in a given message
	std::string CALL_TYPE getValue(const Message& msg, const char* name) const;


	//helper function, convert enumerated field type to string a human can actually read
	const char* CALL_TYPE getTypeStr(Field::FieldType type);


	//helper function, takes a delimited string and splits it into a list of strings
	gmsec::api::util::DataList<std::string> CALL_TYPE splitString(const char* str);


	//internal helper functions for checkValidation
	std::string CALL_TYPE registerTemplate(const Message& msg);
	Status CALL_TYPE compare(const Message& msg, const gmsec::api::util::DataList<FieldTemplate>& fields);
	Status CALL_TYPE validate(const Message& msg, const FieldTemplate& fmtp);


	//helper function for registerTemplate
	bool CALL_TYPE findDefinition(size_t schemaLevel, std::string& name, const Message& msg);


	//iterator functions
	void CALL_TYPE resetSchemaIndex();
	const char* CALL_TYPE nextID();
	bool CALL_TYPE hasNextID() const;


	typedef std::map<std::string, std::string>			 SchemaRegistry; //message subject, schema ID
	typedef gmsec::api::util::DataList<MessageTemplate*> MessageTemplateList;
	typedef gmsec::api::util::DataList<FieldTemplate>	 FieldTemplateList;
	typedef gmsec::api::util::DataList<SchemaTemplate>	 SchemaTemplateList;

	const Config&		m_config;
	SchemaRegistry		m_registry;	
	MessageTemplateList m_templates;
	FieldTemplateList	m_headers;	
	SchemaTemplateList  m_directory;
	std::string			m_specificationId; //version
	unsigned int		m_validationLevel;
	unsigned int		m_schemaLevel;
	unsigned int		m_schemaIndex;
	SchemaIDIterator	m_iterator;
	std::string			m_basePath;

	//holding string for getTemplateXML, without it the function produces undefined behavior
	std::string			m_templateXML;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
