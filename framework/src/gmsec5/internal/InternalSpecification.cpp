/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Disable the Visual Studio warning C4355
//   ('this' : used in base member initializer list)
#ifdef WIN32
#pragma warning(disable: 4355)
#endif

#include <gmsec5/internal/InternalSpecification.h>
#include <gmsec5/internal/FieldTemplate.h>
#include <gmsec5/internal/FileUtil.h>
#include <gmsec5/internal/InternalMessage.h>
#include <gmsec5/internal/InternalSchemaIDIterator.h>
#include <gmsec5/internal/MathUtil.h>
#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/MessageTemplate.h>
#include <gmsec5/internal/StringUtil.h>

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5/Config.h>
#include <gmsec5/ConfigOptions.h>
#include <gmsec5/Errors.h>
#include <gmsec5/Fields.h>
#include <gmsec5/FieldSpecification.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/Message.h>
#include <gmsec5/MessageSpecification.h>
#include <gmsec5/MessageValidator.h>

#include <gmsec5/util/cxx.h>
#include <gmsec5/util/Log.h>
#include <gmsec5/util/Mutex.h>
#include <gmsec5/util/StdUniquePtr.h>

#include <tinyxml2.h>

#include <limits>
#include <set>
#include <sstream>

#if defined(WIN32) || defined(__APPLE__) || (GCC_VERSION >= 40900)
#include <regex>
#endif


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


static Mutex g_mutex;

static const char* const NEWLINE_INDENT1 = "\n   ";
static const char* const NEWLINE_INDENT2 = "\n      ";
static const char* const INDENT1         = "   ";


static bool isXSDFile(const std::string& filename)
{
#if defined(WIN32) || defined(__APPLE__) || (GCC_VERSION >= 40900)
	std::regex ext(".*\\.[Xx][Ss][Dd]");
	return std::regex_match(filename, ext);
#else
	const char* ext[] = { "xsd", "XSD", "xsD", "xSd", "xSD", "Xsd", "XsD", "XSd", NULL };

	for (int i = 0; ext[i] != NULL; ++i)
	{
		if (filename.substr(filename.find_last_of('.') + 1) == ext[i])
		return true;
	}

	return false;
#endif
}


InternalSpecification::InternalSpecification(const Config& config)
	: m_config(config),
	  m_registry(),
	  m_templates(),
	  m_headers(),
	  m_directory(),
	  m_specVersionStr(),
	  m_specVersion(GMSEC_MSG_SPEC_CURRENT),
	  m_schemaLevel(Specification::SchemaLevel::LEVEL_0),
	  m_iterator(*this),
	  m_basePath(),
	  m_lastHeaderName(),
	  m_msgSpecs(),
	  m_templateXML()
{
	const char* path = m_config.getValue(GMSEC_SCHEMA_PATH);
	if (!path)
	{
		m_basePath = "";
	}
	else
	{
		m_basePath = path;
	}

	const char* version = m_config.getValue(GMSEC_MESSAGE_SPEC_VERSION);
	if (version)
	{
		try {
			m_specVersion = StringUtil::getValue<unsigned int>(version);
		}
		catch (...) {
			throw GmsecException(SPECIFICATION_ERROR, INVALID_CONFIG_VALUE, "\"GMSEC-SPECIFICATION-VERSION\" contains invalid value");
		}

		m_specVersionStr = version;
	}
	else
	{
		//default to the current version of the ISD
		std::ostringstream oss;
		oss << GMSEC_MSG_SPEC_CURRENT;

		m_specVersion    = GMSEC_MSG_SPEC_CURRENT;
		m_specVersionStr = oss.str();
	}

	const char* schemaLevel = m_config.getValue(GMSEC_SCHEMA_LEVEL);
	if (schemaLevel)
	{
		try {
			m_schemaLevel = (Specification::SchemaLevel) StringUtil::getValue<unsigned int>(schemaLevel);
		}
		catch (...) {
			throw GmsecException(SPECIFICATION_ERROR, INVALID_CONFIG_VALUE, "\"GMSEC-SCHEMA-LEVEL\" contains invalid value");
		}

		if (m_schemaLevel < Specification::SchemaLevel::LEVEL_0 || m_schemaLevel > Specification::SchemaLevel::LEVEL_6)
		{
			//while the user can conceivably layer as many addendums as they want over the ISD,
			//having more than 7 layered addendums is getting pretty ridiculous
			throw GmsecException(SPECIFICATION_ERROR, INVALID_CONFIG_VALUE, "\"GMSEC-SCHEMA-LEVEL\" contains out-of-range integer");
		}
	}

	// Load the schemas
	try
	{
		load();
	}
	catch (const GmsecException& e)
	{
		cleanup();
		throw e;
	}
	m_schemaIndex = m_templates.begin();

	// Capture the Message/Field Specifications from the loaded schema
	initMessageSpecifications();
}


InternalSpecification::InternalSpecification(const InternalSpecification& other)
	: m_config(other.m_config),
	  m_registry(),
	  m_templates(),
	  m_headers(),
	  m_specVersionStr(other.m_specVersionStr),
	  m_specVersion(other.m_specVersion),
	  m_schemaLevel(other.m_schemaLevel),
	  m_iterator(*this),
	  m_basePath(other.m_basePath),
	  m_lastHeaderName(),
	  m_msgSpecs(),
	  m_templateXML()
{
	for (MessageTemplates::const_iterator it = other.m_templates.begin(); it != other.m_templates.end(); ++it)
	{
		m_templates[it->first] = new MessageTemplate(*(it->second));
	}

	for (HeaderFieldTemplates::const_iterator it = other.m_headers.begin(); it != other.m_headers.end(); ++it)
	{
		m_headers[it->first] = cloneFieldTemplates(it->second);
	}

	for (SchemaTemplateList::const_iterator it = other.m_directory.begin(); it != other.m_directory.end(); ++it)
	{
		m_directory.push_back(new SchemaTemplate(*(*it)));
	}

	m_schemaIndex = m_templates.begin();

	// Capture the Message/Field Specifications from the loaded schema
	initMessageSpecifications();
}


InternalSpecification::~InternalSpecification()
{
	cleanup();
}


std::string InternalSpecification::loadXSD(const char* path, const char* fileName)
{
	tinyxml2::XMLDocument xsd;
	std::list<tinyxml2::XMLDocument*> included;

	//load XSD file
	std::string xsdFile = path + FileUtil::PATH_SEPARATOR + fileName;
	xsd.LoadFile(xsdFile.c_str());

	if (xsd.Error())
	{
		std::string msg = "loadXSD: Unparseable XML string given from file name: ";
		msg += fileName;
		throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, msg.c_str());
	}

	tinyxml2::XMLElement* schema = xsd.RootElement();

	if (schema == NULL)
	{
		std::string msg = "loadXSD: Loaded XML Schema is NULL";
		throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, "Loaded XML Schema is NULL");
	}

	//load included XSD files
	for (tinyxml2::XMLElement* element = schema->FirstChildElement();
		element != NULL;
		element = element->NextSiblingElement())
	{
		if (StringUtil::stringEquals(element->Value(), "xs:include"))
		{
			const char* name = element->Attribute("schemaLocation");

			if (name == NULL)
			{
				std::string err = fileName;
				err.append(": schemaLocation attribute missing");
				throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			std::string includePath = path + FileUtil::PATH_SEPARATOR + name;
			try
			{
				included.push_back(includeXSD(includePath.c_str()));
			}
			catch (const GmsecException& e)
			{
				std::string err = fileName;
				err.append(": failed to include schema ").append(name).append(": ").append(e.getErrorMessage());
				throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}
		}
	}

	//determine schema ID to find body of template

	//file name
	std::string filename = fileName;

	std::vector<std::string> parts = StringUtil::split(filename.c_str(), '_');

	//schema level (e.g. C2MS, GMSEC, etc.)
	std::string schemaLevelStr = parts.front();
	parts.erase(parts.begin());

	//specification ID (e.g. 2019.00.C2MS, 2019.00.GMSEC)
	std::string specID = m_specVersionStr;
	specID.insert(4, ".").append(".").append(schemaLevelStr);

	//short-hand schema ID (e.g. HEADER, MSG.LOG, etc.)
	//std::string short_id = StringUtil::split(filename.substr(0, filename.rfind('.')), '_').back();
	std::string short_id;
	for (std::vector<std::string>::const_iterator it = parts.begin(); it != parts.end(); ++it)
	{
		short_id += *it;
	}
	short_id = short_id.substr(0, short_id.rfind('.'));

	//fully-qualified schema ID (e.g. 2019.00.C2MS.MSG.LOG)
	std::string fullID = specID + "." + short_id;

	//try parsing the loaded XSD file
	std::string errorList;
	MessageTemplate::SubjectElementList subject;
	FieldTemplateList fields = parseXSD(schema, short_id, included, errorList, subject);

	//look up schema ID in directory to get the definition
	std::string definition;
	for (SchemaTemplateList::const_iterator it = m_directory.begin(); it != m_directory.end(); ++it)
	{
		SchemaTemplate* temp = *it;
		if (StringUtil::stringEquals(schemaLevelStr.c_str(), temp->getLevelName()) && StringUtil::stringEquals(short_id.c_str(), temp->getID()))
		{
			definition = temp->getDefinitionString();
		}
	}

	//specific definition not found, look for more general definition
	if (definition.empty() && short_id.rfind('.') != std::string::npos)
	{
		std::string id = short_id.substr(0, short_id.rfind('.'));
		while (!id.empty())
		{
			for (SchemaTemplateList::const_iterator it = m_directory.begin(); it != m_directory.end(); ++it)
			{
				SchemaTemplate* temp = *it;
				if (StringUtil::stringEquals(schemaLevelStr.c_str(), temp->getLevelName()) && StringUtil::stringEquals(id.c_str(), temp->getID()))
				{
					definition = temp->getDefinitionString();
				}
			}
			if (definition.empty() && id.rfind('.') != std::string::npos)
			{
				id = short_id.substr(0, short_id.rfind('.'));
			}
			else
			{
				id = "";
			}
		}
	}

	//general definition not found, use default definition for that schema level
	if (definition.empty())
	{
		for (SchemaTemplateList::const_iterator it = m_directory.begin(); it != m_directory.end(); ++it)
		{
			SchemaTemplate* temp = *it;
			if (StringUtil::stringEquals(schemaLevelStr.c_str(), temp->getLevelName()) && StringUtil::stringEquals("HEADER", temp->getID()))
			{
				definition = temp->getDefinitionString();
			}
		}
	}

	if (definition.empty())
	{
		errorList.append("Unable to find definition in directory file matching ID \"" + short_id + "\" at \"" + schemaLevelStr + "\" schema level").append(NEWLINE_INDENT1);
	}

	//load parsed field templates from XSD file into new FieldTemplate object
	if (StringUtil::stringEqualsIgnoreCase(short_id.c_str(), "HEADER"))
	{
		//remove version numbers from full ID to get proper header ID
		std::string headerID = StringUtil::stringToUpper(StringUtil::split(filename, '_').front());
		m_headers[headerID] = cloneFieldTemplates(fields);

		//This header is on the highest schema level and will be the default header
		if (StringUtil::stringEqualsIgnoreCase(StringUtil::split(specID, '.').back().append(".").append(short_id).c_str(), m_lastHeaderName.c_str()))
		{
			m_headers["DEFAULT"] = cloneFieldTemplates(fields);
			m_subjectHeader = subject;
		}
	}
	else
	{
		int schemaLevel = 0;
		for (SchemaTemplateList::iterator it = m_directory.begin(); it != m_directory.end(); ++it)
		{
			const SchemaTemplate& schemaTemp = **it;

			if (schemaLevelStr == schemaTemp.getLevelName())
			{
				schemaLevel = schemaTemp.getLevel();
				break;
			}
		}

		// The types of Schema IDs that the API (currently) supports.
		//
		// Schema ID Type 1: major.minor.schemaLevel.messagekind.messagetype.<optionalmessagesubtype>
		// Schema ID Type 2: messagekind.messagetype.<optionalmessagesubtype>

		// We store the Message Template using a Type 1 Schema ID (if not already stored)
		if (m_templates.find(fullID) == m_templates.end())
		{
			MessageTemplate* msgTemp = new MessageTemplate(fullID.c_str(), fields, schemaLevel);
			msgTemp->setSubject(subject);
			msgTemp->setDefinition(definition);
			m_templates[fullID] = msgTemp;
		}
		else
		{
			std::ostringstream errmsg;
			errmsg << "Duplicate Schema Name of " << fullID.c_str() << " found";

			throw GmsecException(SPECIFICATION_ERROR, TEMPLATE_DIR_ERROR, errmsg.str().c_str());
		}

		// We then store another copy of the Message Template using a Type 2 Schema ID (if not already stored)
		//
		// Check if Message Templates map has an entry for the short ID
		// If not:
		//     Add new Message Template to the map
		//
		// Else
		//     Iterate through Schema List; for each entry:
		//         Check each of the following:
		//             a) The schema level matches our desired level AND
		//             b) The schema ID is or part of our short ID AND
		//             c) The level name is the same as the addendum name
		//
		//         If the above conditions are all true, then:
		//             Delete existing Message Template
		//             Add new Message Template to the map
		MessageTemplates::iterator it = m_templates.find(short_id);

		if (it == m_templates.end())
		{
			// Not found; add new template with short-hand schema ID, but only if pertinent to configured schema level

			if (schemaLevel <= static_cast<int>(m_schemaLevel))
			{
				MessageTemplate* msgTemp = new MessageTemplate(fullID.c_str(), fields, schemaLevel);
				msgTemp->setSubject(subject);
				msgTemp->setDefinition(definition);

				m_templates[short_id] = msgTemp;

				if (Log::getReportingLevel() == LogLevel::logDEBUG)
				{
					std::ostringstream oss;
					oss << "Adding new Message Template:\n" << *msgTemp << std::endl;
					GMSEC_DEBUG << oss.str().c_str();
				}
			}
		}
		else
		{
			// Found; check if existing template should be replaced with one at a higher schema level

			if (it->second->getSchemaLevel() < static_cast<unsigned int>(schemaLevel) || m_schemaLevel == static_cast<Specification::SchemaLevel>(schemaLevel))
			{
				MessageTemplate* tmp = it->second;
				it->second = new MessageTemplate(fullID.c_str(), fields, schemaLevel);
				it->second->setSubject(tmp->getSubjectElements());
				it->second->setDefinition(definition);
				delete tmp;

				if (Log::getReportingLevel() == LogLevel::logDEBUG)
				{
					std::ostringstream oss;
					oss << "Replacing Message Template with:\n" << *(it->second) << std::endl;
					GMSEC_DEBUG << oss.str().c_str();
				}
			}
		}
	}

	xsd.Clear();

	for (std::list<tinyxml2::XMLDocument*>::iterator docIt = included.begin();
		docIt != included.end();
		++docIt)
	{
		delete *docIt;
	}
	included.clear();

	for (FieldTemplateList::iterator it = fields.begin(); it != fields.end(); ++it)
	{
		delete *it;
	}
	fields.clear();

	return errorList;
}


tinyxml2::XMLDocument* InternalSpecification::includeXSD(const char* path)
{
	tinyxml2::XMLDocument* xsd = new tinyxml2::XMLDocument;

	xsd->LoadFile(path);

	if (xsd->Error())
	{
		std::string err = "failed to load file";
		throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
	}

	return xsd;
}

InternalSpecification::FieldTemplateList InternalSpecification::parseXSD(const tinyxml2::XMLElement* schema, const std::string& schemaID, const std::list<tinyxml2::XMLDocument*>& included, std::string& errorList, SubjectElementList& subjectElements)
{
	FieldTemplateList fields;

	if (schema->FirstChildElement("xs:complexType") && schema->FirstChildElement("xs:complexType")->Attribute("name", "HEADER"))
	{//parsed header with correct leading tag
		const tinyxml2::XMLElement* body = schema->FirstChildElement("xs:complexType");
		const tinyxml2::XMLElement* subject = body->NextSiblingElement("xs:complexType");

		body = body->FirstChildElement("xs:all");
		if (body == NULL)
		{
			std::string err = schemaID;
			err.append(": schema missing <xs:all> tag inside HEADER");
			throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
		}

		//review field elements and create fieldTemplates
		for (const tinyxml2::XMLElement* element = body->FirstChildElement("xs:element");
			element != NULL;
			element = element->NextSiblingElement("xs:element"))
		{
			std::string list;
			fields.push_back(parseField(schema, schemaID, included, element, errorList, list));
		}

		//review subject elements and build header portion of subject
		if (subject != NULL)
		{
			subject = subject->FirstChildElement("xs:all");
			if (subject == NULL)
			{
				std::string err = schemaID;
				err.append(": schema missing <xs:all> tag inside SUBJECT");
				throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			for (const tinyxml2::XMLElement* element = subject->FirstChildElement("xs:element"); element != NULL; element = element->NextSiblingElement("xs:element"))
			{
				if (!element->Attribute("name") || StringUtil::stringEquals(element->Attribute("name"), ""))
				{
					std::string err = schemaID;
					err.append(": subject element missing name attribute");
					throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
				}

				std::string value;
				if (element->Attribute("minOccurs", "0")) value.append("!");
				if (element->Attribute("fixed")) value.append(element->Attribute("fixed"));

				subjectElements.push_back(SubjectElement(element->Attribute("name"), value));
			}
		}
	}
	else if (schema->LastChildElement("xs:element") && schema->LastChildElement("xs:element")->Attribute("name", schemaID.c_str()))
	{//parsed message template with correct leading tag
		const tinyxml2::XMLElement* body = schema->LastChildElement("xs:element");

		const tinyxml2::XMLElement* content = NULL;

		//get subject and content of body
		if (body->Attribute("name", schemaID.c_str()))
		{
			content = body->FirstChildElement("xs:complexType");
			if (content == NULL)
			{
				std::string err = schemaID;
				err.append(": schema missing <xs:complexType> tag");
				throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			content = content->FirstChildElement("xs:all");
			if (content == NULL)
			{
				std::string err = schemaID;
				err.append(": schema missing <xs:all> tag");
				throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			//get subject
			content = content->FirstChildElement("xs:element");
			if (content == NULL || !StringUtil::stringEquals(content->FirstAttribute()->Value(), "SUBJECT"))
			{
				std::string err = schemaID;
				err.append(": missing <xs:element> named SUBJECT");
				throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}
			else
			{
				//get name of subject template
				std::string type = content->Attribute("type");

				//look up matching complextype in message template
				for (const tinyxml2::XMLElement* complexType = schema->FirstChildElement("xs:complexType"); 
					complexType != NULL; 
					complexType = complexType->NextSiblingElement("xs:complexType")) 
				{
					if (complexType->Attribute("name", type.c_str()))
					{
						//found matching complexType

						const tinyxml2::XMLElement* subject = complexType->FirstChildElement("xs:all");

						if (subject != NULL)
						{
							for (const tinyxml2::XMLElement* element = subject->FirstChildElement("xs:element"); element != NULL; element = element->NextSiblingElement("xs:element"))
							{
								if (!element->Attribute("name") || StringUtil::stringEquals(element->Attribute("name"), ""))
								{
									std::string err = schemaID;
									err.append(": subject element missing name attribute");
									throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
								}

								std::string value;
								if (element->Attribute("minOccurs", "0")) value.append("!");
								if (element->Attribute("fixed")) value.append(element->Attribute("fixed"));

								subjectElements.push_back(SubjectElement(element->Attribute("name"), value));
							}
						}
						break;
					}
				}

				if (subjectElements.empty())
				{
					//no match found yet, look up complexType in included files
					for (std::list<tinyxml2::XMLDocument*>::const_iterator docIt = included.begin(); docIt != included.end(); ++docIt)
					{

						const tinyxml2::XMLDocument& doc = **docIt;
						const tinyxml2::XMLElement* includedSchema = doc.RootElement();

						if (includedSchema == NULL)
						{
							std::string err = schemaID;
							err.append(": Malformed XSD: included schema missing root element");
							throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
						}						
										
						//look up matching complextype in included doc
						for (const tinyxml2::XMLElement* complexType = includedSchema->FirstChildElement("xs:complexType");
							complexType != NULL;
							complexType = complexType->NextSiblingElement("xs:complexType"))
						{
							if (complexType->Attribute("name", type.c_str()))
							{
								//found matching complexType
								
								const tinyxml2::XMLElement* subject = complexType->FirstChildElement("xs:all");
								if (subject != NULL)
								{
									for (const tinyxml2::XMLElement* element = subject->FirstChildElement("xs:element"); element != NULL; element = element->NextSiblingElement("xs:element"))
									{
										if (!element->Attribute("name") || StringUtil::stringEquals(element->Attribute("name"), ""))
										{
											std::string err = schemaID;
											err.append(": subject element missing name attribute");
											throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
										}

										std::string value;
										if (element->Attribute("minOccurs", "0")) value.append("!");
										if (element->Attribute("fixed")) value.append(element->Attribute("fixed"));

										subjectElements.push_back(SubjectElement(element->Attribute("name"), value));
									}
								}
								break;
							}
						}
						if (!subjectElements.empty()) break;
					}
				}

			}

			//get content 
			content = content->NextSiblingElement("xs:element");
			if (content == NULL || !StringUtil::stringEquals(content->FirstAttribute()->Value(), "CONTENT"))
			{
				std::string err = schemaID;
				err.append(": missing <xs:element> named CONTENT");
				throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			content = content->FirstChildElement("xs:complexType");
			if (content == NULL)
			{
				std::string err = schemaID;
				err.append(": content missing <xs:complexType> tag");
				throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			content = content->FirstChildElement("xs:all");
			if (content == NULL)
			{
				std::string err = schemaID;
				err.append(": content missing <xs:all> tag");
				throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			//review field elements and create fieldTemplates
			for (const tinyxml2::XMLElement* element = content->FirstChildElement("xs:element");
				element != NULL;
				element = element->NextSiblingElement("xs:element"))
			{
				std::string list;
				fields.push_back(parseField(schema, schemaID, included, element, errorList, list));
			}
		}
		else
		{
			std::string err = schemaID;
			err.append(": last <xs:element> found is not named \"").append(schemaID.c_str()).append("\"");
			throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
		}
	}
	else
	{//can't properly identify xsd
		if (schemaID == "HEADER")
		{
			std::string err = schemaID;
			err.append(": schema missing <xs:complexType> tag named HEADER");
			throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
		}
		else
		{
			std::string err = schemaID;
			err.append(": schema missing <xs:element> tag with attribute name=\"").append(schemaID.c_str()).append("\"");
			throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
		}
	}

	return fields;
}

FieldTemplate* InternalSpecification::parseField(const tinyxml2::XMLElement* schema, const std::string& schemaID, const std::list<tinyxml2::XMLDocument*>& included, const tinyxml2::XMLElement* element, std::string& errorList, std::string& refNameList)
{
	StdUniquePtr<FieldTemplate> field(new FieldTemplate());
	
	//set field template's NAME
	if (element->Attribute("name"))
	{
		field->setName(element->Attribute("name"));
		field->setModifiedName(element->Attribute("name"));
	}
	else
	{
		std::string err = schemaID;
		err.append(": field element missing NAME attribute");
		errorList.append(err.c_str()).append(NEWLINE_INDENT1);
	}

	if (field->getName().empty())
	{
		std::string err = schemaID;
		err.append(": field template is missing NAME attribute");
		errorList.append(err.c_str()).append(NEWLINE_INDENT1);
	}
	   
	//set field templates's CLASS
	if (StringUtil::stringEquals(schemaID.c_str(), "HEADER"))
	{
		field->setClass("HEADER");
	}
	else
	{
		field->setClass("STANDARD");
	}

	//set field template's MODE
	if (element->Attribute("minOccurs", "0")) field->setMode("OPTIONAL");
	else field->setMode("REQUIRED");

	//set field template's DESCRIPTION
	const tinyxml2::XMLElement* documentation = element->FirstChildElement("xs:annotation");
	if (documentation != NULL)
	{
		documentation = documentation->FirstChildElement("xs:documentation");
		if (documentation != NULL)
		{
			if (documentation->GetText())
			{
				field->setDescription(documentation->GetText());
			}
			else
			{
				field->setDescription("");
			}
		}
	}

	//find field template extension tag to get types and values
	const tinyxml2::XMLElement* extension = element->FirstChildElement("xs:complexType");
	if (extension == NULL)
	{
		std::string err = schemaID;
		err.append(": Malformed XSD: field element \"").append(field->getName()).append("\" missing <xs:complexType> tag");
		throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
	}


	//determine if next tag is xs:simpleContent or xs:sequence
	if (extension->FirstChildElement("xs:simpleContent"))
	{
		extension = extension->FirstChildElement("xs:simpleContent");
		//standard field, finish finding extension
		extension = extension->FirstChildElement("xs:extension");
		if (extension == NULL || extension->Attribute("base") == NULL)
		{
			std::string err = schemaID;
			err.append(": Malformed XSD: field element \"").append(field->getName()).append("\" missing <xs:extension> tag or base attribute");
			throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
		}

		field->setPattern(extension->Attribute("base"));

		//find attribute tag to determine types
		const tinyxml2::XMLElement* attribute = extension->FirstChildElement("xs:attribute");
		if (attribute == NULL || attribute->Attribute("name", "TYPE") == NULL || attribute->Attribute("type") == NULL)
		{
			std::string err = schemaID;
			err.append(": Malformed XSD: field element \"").append(field->getName()).append("\" missing or incomplete <xs:attribute> tag");
			throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
		}
		else
		{
			std::list<std::string> types;
			if (attribute->Attribute("fixed"))
			{//field has a fixed value
				types.push_back(attribute->Attribute("fixed"));
				field->setTypes(types);

				if (field->getTypes().empty())
				{
					std::string err = schemaID;
					err.append(": field element ").append(field->getName()).append(" is missing TYPE");
					errorList.append(err.c_str()).append(NEWLINE_INDENT1);
				}
			}
			else
			{//field has a list of enumerated values
				if (attribute->Attribute("type") == NULL)
				{
					std::string err = schemaID;
					err.append(": Malformed XSD: field element \"").append(field->getName()).append("\" missing or incomplete <xs:attribute> tag");
					throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
				}

				std::string simpleType = attribute->Attribute("type");

				std::string pattern;
				bool foundSimpleType = parseEnumerations(types, pattern, simpleType, schemaID, schema, included);

				field->setTypes(types);

				if (!foundSimpleType)
				{
					std::string err = schemaID;
					err.append(": cannot find simpleType element \"").append(simpleType.c_str()).append("\"");
					errorList.append(err.c_str()).append(NEWLINE_INDENT1);
				}

				if (field->getTypes().empty())
				{
					std::string err = schemaID;
					err.append(": field element ").append(field->getName()).append(" is missing TYPE");
					errorList.append(err.c_str()).append(NEWLINE_INDENT1);
				}
			}

			//check to make sure all types are valid
			//const std::list<std::string>& types = field->getTypeStrings();

			if (!types.empty() && !(types.size() == 1 && types.front().empty()))
			{
				for (std::list<std::string>::const_iterator it = types.begin(); it != types.end(); ++it)
				{
					if (!(StringUtil::stringEqualsIgnoreCase(it->c_str(), "VARIABLE") ||
						StringUtil::stringEqualsIgnoreCase(it->c_str(), "UNSET") ||
						StringUtil::stringEqualsIgnoreCase(it->c_str(), "HEADER_STRING") ||
						StringUtil::stringEqualsIgnoreCase(it->c_str(), "TIME")))
					{
						try
						{
							InternalField::lookupType((*it).c_str());
						}
						catch (...)
						{
							std::string err = schemaID;
							err.append(": the field template ").append(field->getName()).append(" contains unrecognized TYPE \"").append((*it).c_str()).append("\"");
							errorList.append(err.c_str()).append(NEWLINE_INDENT1);
						}
					}
				}
			}
			else
			{
				std::string err = schemaID;
				err.append(": field element ").append(field->getName()).append(" is missing TYPE");
				errorList.append(err.c_str()).append(NEWLINE_INDENT1);
			}
		}

		//tracking fields will have another attribute tag
		attribute = extension->LastChildElement("xs:attribute");
		if (attribute)
		{
			if (attribute->Attribute("name", "TRACKING") != NULL)
			{
				field->setMode("TRACKING");
			}
		}

		//find values using base tag
		if (extension->Attribute("base"))
		{
			std::list<std::string> values;
			std::string pattern;
			std::string base = extension->Attribute("base");

			if (base.find("xs:") == std::string::npos)
			{//field has an enumerated list of values

				bool foundSimpleType = parseEnumerations(values, pattern, base, schemaID, schema, included);

				field->setValues(values);

				if (!foundSimpleType)
				{
					std::string err = schemaID;
					err.append(": cannot find simpleType element \"").append(base.c_str()).append("\"");
					errorList.append(err.c_str()).append(NEWLINE_INDENT1);
				}

				//check to make sure all values are valid
				std::list<std::string> values = field->getValues();
				for (std::list<std::string>::iterator it = values.begin(); it != values.end(); ++it)
				{
					std::string value = *it;
					if (!checkValue(field->getTypes(), value))
					{
						std::string err = schemaID;
						err.append(": The field template ").append(field->getName()).append(" contains an invalid value \"").append(value).append("\"");
						errorList.append(err.c_str()).append(NEWLINE_INDENT1);
					}
				}
			}
		}
		else
		{
			std::string err = schemaID;
			err.append(": field element \"").append(field->getName()).append("\" extension tag missing base attribute");
			errorList.append(err.c_str()).append(NEWLINE_INDENT1);
		}

		//find if any dependencies exist within the extension
		const tinyxml2::XMLElement* dependency = extension->FirstChildElement("api:dependency");
		if (dependency && dependency->Attribute("name") == NULL)
		{
			std::string err = schemaID;
			err.append(": Malformed XSD: field element \"").append(field->getName()).append("\" contains <api:dependency> tag with missing or empty name attribute");
			throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
		}
		else if(dependency)
		{//found dependencies

			for (dependency = extension->FirstChildElement("api:dependency"); dependency != NULL; dependency = dependency->NextSiblingElement("api:dependency"))
			{//run through the list of dependencies and parse

				if (dependency->Attribute("name") == NULL)
				{
					std::string err = schemaID;
					err.append(": Malformed XSD: field element \"").append(field->getName()).append("\" contains <api:dependency tag with missing name attribute");
					errorList.append(err.c_str()).append(NEWLINE_INDENT1);
				}

				std::string name = dependency->Attribute("name");

				if (name.empty())
				{
					std::string err = schemaID;
					err.append(": Malformed XSD: field element \"").append(field->getName()).append("\"contains <api:dependency tag with empty name attribute");
					errorList.append(err.c_str()).append(NEWLINE_INDENT1);
				}

				std::list<std::string> values;
				std::list<std::string> types;
				std::string pattern;

				//check if dependency modifies the field values
				if (dependency->Attribute("base"))
				{
					std::string base = dependency->Attribute("base");
					bool foundSimpleType = parseEnumerations(values, pattern, base, schemaID, schema, included);

					if (!foundSimpleType)
					{
						std::string err = schemaID;
						err.append(": cannot find simpleType element \"").append(base.c_str()).append("\"");
						errorList.append(err.c_str()).append(NEWLINE_INDENT1);
					}
				}

				//check if dependency modifies the types
				if (dependency->Attribute("type"))
				{
					std::string type = dependency->Attribute("type");
					std::string pattern;
					bool foundSimpleType = parseEnumerations(types, pattern, type, schemaID, schema, included);

					if (!foundSimpleType)
					{
						std::string err = schemaID;
						err.append(": cannot find simpleType element \"").append(type.c_str()).append("\"");
						errorList.append(err.c_str()).append(NEWLINE_INDENT1);
					}
				}

				//add dependency to field template
				field->initDependency(dependency->Attribute("name"), dependency->Attribute("valueEquals"), dependency->Attribute("valueGreaterThan"), dependency->Attribute("valueLessThan"), dependency->Attribute("use"), pattern.c_str(), values, types);
			}
		}
	}
	else if (extension->FirstChildElement("xs:sequence"))
	{
		//control field: container
		//contains an array of indexed field templates
		//not an actual message field
		extension = extension->FirstChildElement("xs:sequence");

		//change field MODE to CONTROL
		field->setMode("CONTROL");

		//sequence of fields, there is no extension, find reference element instead
		const tinyxml2::XMLElement* refElement = extension->FirstChildElement("xs:element");
		if (refElement == NULL || refElement->Attribute("ref") == NULL)
		{
			std::string err = schemaID;
			err.append(": Malformed XSD: field element \"").append(field->getName()).append("\" missing <xs:element> tag or base attribute");
			throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
		}

		std::string refName = refElement->Attribute("ref");
		field->setPrefix(refName);

		std::vector<std::string> list = StringUtil::split(refNameList, ' ');
		for (std::vector<std::string>::iterator it = list.begin(); it != list.end(); ++it)
		{
			const std::string name = *it;
			if (StringUtil::stringEqualsIgnoreCase(name.c_str(), refName.c_str()))
			{
				std::string err = schemaID;
				err.append(": Malformed XSD: reference element \"").append(refName.c_str()).append("\"cannot reference itself or parent element of same name");
				throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}
		}
		
		refNameList.append(" ").append(refName);

		bool foundSimpleType = false;

		//find element with name matching ref attribute in sequence tag
		for (const tinyxml2::XMLElement* match = schema->FirstChildElement("xs:element");
			match != NULL;
			match = match->NextSiblingElement("xs:element"))
		{
			if (match->Attribute("name", refName.c_str()))
			{
				foundSimpleType = true;
				const tinyxml2::XMLElement* reference = match->FirstChildElement("xs:complexType");

				if (reference == NULL)
				{
					std::string err = schemaID;
					err.append(": Malformed XSD: reference element \"").append(refName.c_str()).append("\" missing <xs:complexType> tag");
					throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
				}

				reference = reference->FirstChildElement("xs:all");
				if (reference == NULL)
				{
					std::string err = schemaID;
					err.append(": Malformed XSD: reference element \"").append(refName.c_str()).append("\" missing <xs:all> tag");
					throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
				}

				FieldTemplateList children;

				//review child fields...
				for (const tinyxml2::XMLElement* child = reference->FirstChildElement("xs:element"); 
					child != NULL; 
					child = child->NextSiblingElement("xs:element"))
				{
					children.push_back(parseField(schema, schemaID, included, child, errorList, refNameList));
				}
				field->setChildren(children);
				for (FieldTemplateList::iterator it = children.begin(); it != children.end(); ++it)
				{
					delete *it;
				}
				children.clear();

				break;
			}
		}

		if (!foundSimpleType)
		{
			std::string err = schemaID;
			err.append(": cannot find reference element \"").append(refName.c_str()).append("\"");
			errorList.append(err.c_str()).append(NEWLINE_INDENT1);
		}		

	}
	else
	{
		std::string err = schemaID;
		err.append(": field element \"").append(field->getName()).append("\" missing <xs:simpleContent> or <xs:sequence> tag");
		errorList.append(err.c_str()).append(NEWLINE_INDENT1);
	}

	return field.release();
}

bool InternalSpecification::parseEnumerations(std::list<std::string>& enumerations, std::string& pattern, const std::string& name, const std::string& schemaID, const tinyxml2::XMLElement* schema, const std::list<tinyxml2::XMLDocument*>& included)
{
	bool foundSimpleType = false;

	//search template for simpletype matching given name
	for (const tinyxml2::XMLElement* match = schema->FirstChildElement("xs:simpleType"); match != NULL; match = match->NextSiblingElement("xs:simpleType"))
	{
		if (match->Attribute("name", name.c_str()))
		{
			foundSimpleType = true;
			const tinyxml2::XMLElement* restriction = match->FirstChildElement("xs:restriction");

			if (restriction == NULL)
			{
				std::string err = schemaID;
				err.append(": Malformed XSD: simpleType \"").append(name.c_str()).append("\" missing <xs:restriction> tag");
				throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			for (const tinyxml2::XMLElement* enumeration = restriction->FirstChildElement("xs:enumeration"); enumeration != NULL; enumeration = enumeration->NextSiblingElement("xs:enumeration"))
			{
				enumerations.push_back(enumeration->Attribute("value"));
			}

			const tinyxml2::XMLElement* patternElement = restriction->FirstChildElement("xs:pattern");

			if (patternElement)
			{
				pattern = patternElement->Attribute("value");
			}

			break;
		}
	}

	//search included documents for simpleType matching given name if simpletype hasn't yet been found
	if (!foundSimpleType)
	{
		for (std::list<tinyxml2::XMLDocument*>::const_iterator docIt = included.begin(); docIt != included.end(); ++docIt)
		{
			const tinyxml2::XMLDocument& doc = **docIt;
			const tinyxml2::XMLElement* includedSchema = doc.RootElement();

			if (includedSchema == NULL)
			{
				std::string err = schemaID;
				err.append(": Malformed XSD: included schema missing root element");
				throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			for (const tinyxml2::XMLElement* match = includedSchema->FirstChildElement("xs:simpleType"); match != NULL; match = match->NextSiblingElement("xs:simpleType"))
			{
				if (match->Attribute("name", name.c_str()))
				{
					foundSimpleType = true;

					const tinyxml2::XMLElement* restriction = match->FirstChildElement("xs:restriction");

					if (restriction == NULL)
					{
						std::string err = schemaID;
						err.append(": Malformed XSD: simpleType \"").append(name.c_str()).append("\" in included document missing <xs:restriction> tag");
						throw GmsecException(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
					}

					for (const tinyxml2::XMLElement* enumeration = restriction->FirstChildElement("xs:enumeration"); enumeration != NULL; enumeration = enumeration->NextSiblingElement("xs:enumeration"))
					{
						enumerations.push_back(enumeration->Attribute("value"));
					}
					break;
				}
			}
			if (foundSimpleType) break;
		}
	}

	return foundSimpleType;
}


Status InternalSpecification::loadTemplate(const char* source, const char* fileName)
{
	Status status;

	tinyxml2::XMLDocument xml;
	tinyxml2::XMLError xmlError;

	xmlError = xml.LoadFile(source);

	if (xmlError != tinyxml2::XML_NO_ERROR)
	{
		std::string msg = "Unable to parse file; reason: ";
		msg += StringUtil::xmlErrorToString(xmlError);
		return Status(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, msg.c_str());
	}

	tinyxml2::XMLElement* schema = xml.RootElement();

	if (schema == NULL)
	{
		std::string msg = "Loaded XML schema is NULL";
		return Status(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, "Loaded XML Schema is NULL");
	}

	//file name
	std::string filename = fileName;

	//schema level (e.g. C2MS, GMSEC, etc.)
	std::string schemaLevelStr = StringUtil::split(filename, '.').front();

	std::string id;
	std::string specID = m_specVersionStr;
	specID.insert(4, ".");

	FieldTemplateList fields;
	status = parser(schema, id, fields);

	xml.Clear();

	for (FieldTemplateList::iterator it = fields.begin(); it != fields.end(); ++it)
	{
		delete *it;
	}
	fields.clear();
	

	return status;
}


bool InternalSpecification::findDefinition(int schemaLevel, std::string& name, const Message& msg)
{//first look for a schema definition on the same level as the header
 //if we can't find a matching schema, return false then go one level lower on the header and call this function again
 //if we find a matching schema, we'll search again on that same level for a more specific definition
 //and repeat until we can't find a more specific definition for the message
	
	bool found = false;
	std::string oldName;

	while(oldName != name && !found)
	{//so long as the name is updated it means there may be a more specific definition available

		oldName = name;

		for(SchemaTemplateList::iterator it = m_directory.begin(); it != m_directory.end(); ++it)
		{//now looking through directory
			SchemaTemplate& temp = **it;
			if(StringUtil::stringEquals(temp.getID(), name.c_str()) && temp.getLevel() == schemaLevel)
			{//looks like a more specific definition exists
			 //rebuild the name with the new definition
				found = true;
				try
				{
					std::string newName = msg.getStringValue(temp.getFirstDefinition());
					while(temp.hasNextDefinition())
					{
						newName.append(".");
						newName.append(msg.getStringValue(temp.getNextDefinition()));
					}
					//if we've gotten this far then it means we haven't thrown a field not found exception
					//that means we have ourselves a new definition that matches up with our message
					name = newName;
				}
				catch(...)
				{//The message contents doesn't match the more specific definition we found
				 //so our definition is as specific it can get already, so we're done here
					return true;
				}
				break;
			}
		}
	}

	return found;
}


void InternalSpecification::cleanup()
{
	for (MessageTemplates::iterator it = m_templates.begin(); it != m_templates.end(); ++it)
	{
		delete it->second;
	}
	m_templates.clear();

	for (HeaderFieldTemplates::iterator it = m_headers.begin(); it != m_headers.end(); ++it)
	{
		FieldTemplateList& fieldTemps = it->second;
		for (FieldTemplateList::iterator it2 = fieldTemps.begin(); it2 != fieldTemps.end(); ++it2)
		{
			delete *it2;
		}
		fieldTemps.clear();
	}
	m_headers.clear();

	for (SchemaTemplateList::iterator it = m_directory.begin(); it != m_directory.end(); ++it)
	{
		delete *it;
	}
	m_directory.clear();

	m_registry.clear();

	for (MessageSpecifications::iterator it = m_msgSpecs.begin(); it != m_msgSpecs.end(); ++it)
	{
		delete *it;
	}
	m_msgSpecs.clear();
}


void InternalSpecification::load()
{
	Status status;    
	std::ostringstream oss;

	std::string template_dir;

	// Deduce template directory path
	if (m_basePath.empty())
	{
		std::string shared_object_path;

		if (FileUtil::getCurrentSharedObjectPath(shared_object_path) == false)
		{
			throw GmsecException(SPECIFICATION_ERROR, TEMPLATE_DIR_NOT_FOUND, "Could not get the system-specified base path.");
		}
		size_t dirMarker = shared_object_path.rfind(FileUtil::PATH_SEPARATOR);

		if (dirMarker == std::string::npos)
		{
			std::string err_msg = "Could not list files in template directory ";
			err_msg += template_dir;
			throw GmsecException(SPECIFICATION_ERROR, TEMPLATE_DIR_NOT_FOUND, err_msg.c_str());
		}

		template_dir = shared_object_path.substr(0, dirMarker) + FileUtil::PATH_SEPARATOR + ".." + FileUtil::PATH_SEPARATOR + "templates";
	}
	else
	{
		template_dir = m_basePath;
	}

	char major_version_str[5]; // Major version comes from the year.
		                        // It is assumed that this code will
		                        // not be used beyond year 9999,
		                        // which has 4 digits (and we
		                        // add one for the end of a c-string,
		                        // to get max size 5)

	char minor_version_str[3]; // Minor versions are allowed
		                        // from 0 to 99, or 2 digits max
		                        // (and we add one for the end of
		                        // a c-string, to get max size 3

	// Get major, minor version number strings
	StringUtil::stringFormat(major_version_str, sizeof(major_version_str), "%04u", getVersion() / 100);
	StringUtil::stringFormat(minor_version_str, sizeof(minor_version_str), "%02u", getVersion() % 100);

	// Adjust template (load) directory path
	template_dir += FileUtil::PATH_SEPARATOR;
	template_dir += major_version_str;
	template_dir += ".";
	template_dir += minor_version_str;

	std::list<std::string> schema_files;

	if (FileUtil::getFilesInDirectory(template_dir, schema_files) == false)
	{
		std::string err_msg = "Could not list files in template directory ";
		err_msg += template_dir;
		throw GmsecException(SPECIFICATION_ERROR, TEMPLATE_DIR_ERROR, err_msg.c_str());
	}

	// Always load the DIRECTORY file first...
	std::string path = template_dir;
	path += FileUtil::PATH_SEPARATOR;
	path += DIRECTORY_FILE;

	Status result = loadTemplate(path.c_str(), DIRECTORY_FILE);

	if (m_directory.empty() || StringUtil::stringEqualsIgnoreCase(result.getReason(), "File not found"))
	{
		GMSEC_WARNING << "Failed to read/process " << path.c_str() << "; reason: " << result.get();
		GMSEC_INFO    << "Attempting to read " << HIDDEN_DIRECTORY_FILE;

		// Try hidden file using the legacy naming convention
		path = template_dir;
		path += FileUtil::PATH_SEPARATOR;
		path += HIDDEN_DIRECTORY_FILE;

		result = loadTemplate(path.c_str(), HIDDEN_DIRECTORY_FILE);

		if (m_directory.empty() || result.hasError())
		{
			std::ostringstream oss;
			oss << "Failed to load Template Directory " << HIDDEN_DIRECTORY_FILE << "; reason: " << result.getReason();
			throw GmsecException(SPECIFICATION_ERROR, TEMPLATE_DIR_ERROR, oss.str().c_str());
		}
	}
	else if (result.hasError())
	{
		std::ostringstream oss;
		oss << "Failed to load Template Directory " << DIRECTORY_FILE << "; reason: " << result.getReason();
		throw GmsecException(SPECIFICATION_ERROR, TEMPLATE_DIR_ERROR, oss.str().c_str());
	}
			
	// Then other files...
	for (std::list<std::string>::iterator it = schema_files.begin(); it != schema_files.end(); ++it)
	{
		const std::string& filename = *it;
		if (filename != ".." && 
			filename != "." && 
			!StringUtil::stringEquals(filename.c_str(), DIRECTORY_FILE) && 
			!StringUtil::stringEquals(filename.c_str(), HIDDEN_DIRECTORY_FILE) && 
			!StringUtil::stringEquals(filename.c_str(), FIELDS_FILE) && 
			!StringUtil::stringEquals(filename.c_str(), DEFAULTS_FILE) &&
			!StringUtil::stringEquals(filename.c_str(), SUBJECTS_FILE) &&
			isXSDFile(filename))
		{
			AutoMutex lock(g_mutex);

			std::string levelID = StringUtil::split(filename.c_str(), '_').front();
			std::string schemaID = StringUtil::split(filename.c_str(), '_').back();
			schemaID = schemaID.substr(0, schemaID.find_last_of('.'));
			bool skip = false;

			//skip loading templates that live on a schema level higher than the configured schema level, they will never be used
			for (SchemaTemplateList::iterator it = m_directory.begin(); it != m_directory.end(); ++it)
			{
				SchemaTemplate* schema = *it;
				if (schemaID.find(schema->getID()) != std::string::npos && StringUtil::stringEquals(levelID.c_str(), schema->getLevelName()) && static_cast<Specification::SchemaLevel>(schema->getLevel()) > m_schemaLevel)
				{
					skip = true;
					break;
				}
			}

			if (skip) continue;

			path = template_dir;
			path += FileUtil::PATH_SEPARATOR;
			try
			{
				std::string errors = loadXSD(path.c_str(), filename.c_str());
				if (!errors.empty())
				{
					size_t pos = errors.rfind("\n");
					errors = errors.substr(0, pos);
					oss.clear();
					oss.str("");
					oss << "Failed to parse " << filename.c_str() << NEWLINE_INDENT1 << errors.c_str() << (status.hasError() ? "\n" : "") << (status.hasError() ? status.get() : "");
					status.set(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
				}
			}
			catch (const GmsecException& e)
			{
				oss.clear();
				oss.str("");
				oss << "Failed to parse " << filename.c_str() << "\n" << e.getErrorMessage() << (status.hasError() ? "\n" : "") << (status.hasError() ? status.get() : "");
				status.set(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
			}
		}
	}

	//finalize subjects for loaded message templates
	for (MessageTemplates::iterator it = m_templates.begin(); it != m_templates.end(); ++it)
	{
		MessageTemplate* msgTemp = it->second;

		msgTemp->addSubjectHeader(m_subjectHeader);
	}

	if (status.hasError())
	{
		throw GmsecException(status);
	}
}


void InternalSpecification::initMessageSpecifications()
{
	for (MessageTemplates::iterator it = m_templates.begin(); it != m_templates.end(); ++it)
	{
		const std::string&     schemaID = it->first;;
		const MessageTemplate* msgTemp  = it->second;
		
		std::vector<std::string> parts = StringUtil::split(schemaID, '.');

		// We do not want duplicate entries in our list; thus skip fully-qualified
		// schema IDs, and work only with the shorthand ones.
		if (parts.size() >= 5) continue;

		// This list of fields will contain the combined header and contents
		FieldTemplateList fields = prepHeaders(schemaID.c_str());
		
		const FieldTemplateList& fldTemps = msgTemp->getFieldTemplates();
		
		for (FieldTemplateList::const_iterator it2 = fldTemps.begin(); it2 != fldTemps.end(); ++it2)
		{
			FieldTemplate* field = *it2;
			if (StringUtil::stringEqualsIgnoreCase(field->getMode().c_str(), "CONTROL") && field->hasChildren())
			{
				FieldTemplateList temps;
				field->getAllChildren(temps);
				for (FieldTemplateList::iterator it3 = temps.begin(); it3 != temps.end(); ++it3)
				{
					fields.push_back(new FieldTemplate(*(*it3)));
				}
			}
			else
			{
				fields.push_back(new FieldTemplate(*(*it2)));
			}
		}
		
		util::List<FieldSpecification*> fieldSpecs;

		for (FieldTemplateList::iterator it2 = fields.begin(); it2 != fields.end(); ++it2)
		{
			const FieldTemplate* fldTemp = *it2;

			const std::string& name = fldTemp->getModifiedName();
			
			//skip control fields
			if (!StringUtil::stringEqualsIgnoreCase(fldTemp->getMode().c_str(), "CONTROL"))
			{
				const std::string& type  = fldTemp->getConcatenatedTypes();
				const std::string& mode  = fldTemp->getMode();
				const std::string& clazz = fldTemp->getClass();
				const std::string& value = fldTemp->getConcatenatedValues();
				const std::string& desc  = fldTemp->getDescription();

				fieldSpecs.push_back(new FieldSpecification(name.c_str(), type.c_str(), mode.c_str(), clazz.c_str(), value.c_str(), desc.c_str()));
			}

			delete fldTemp;
		}
		fields.clear();

		//Note: MessageSpecification object will take ownership of the FieldSpecification object(s)
		m_msgSpecs.push_back(new MessageSpecification(schemaID.c_str(), msgTemp->getSubjectDefinition().c_str(), fieldSpecs));
	}
}

Status InternalSpecification::parser(tinyxml2::XMLElement* xmlSchema, std::string& schemaID, FieldTemplateList& schemaFields)
{
	Status status;
	std::ostringstream oss;

	//The accumulated error messages as a string
	std::string errorList;

	if(xmlSchema->FirstAttribute() != NULL)
	{
		std::string name = xmlSchema->FirstAttribute()->Name();
		if(name == "NAME")
		{//grabbing the name of the schema we are going to parse
			schemaID = xmlSchema->FirstAttribute()->Value();
		}
	}

	//delimiter is inserted after major revision (a four digit number)
	std::string id = m_specVersionStr;
	id.insert(4, ".");

	if(schemaID == "")
	{//We failed to set schemaID, which likely means it failed to parse
		status.set(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, 
				   "The schema read did not have a template id NAME specified.");
		return status;
	}

	for(tinyxml2::XMLElement* element = xmlSchema->FirstChildElement();
		element != NULL && !status.hasError();
		element = element->NextSiblingElement())
	{//run through all the child elements and set them up as SchemaTemplates
			
		//setting up schema we'll load into the directory
		SchemaTemplate* schema = new SchemaTemplate();

		for(tinyxml2::XMLAttribute* attribute = (tinyxml2::XMLAttribute*) element->FirstAttribute();
			attribute !=NULL;
			attribute = (tinyxml2::XMLAttribute*) attribute->Next())
		{//Now we'll parse the attributes and load them into the SchemaTemplate
			if(StringUtil::stringEqualsIgnoreCase(attribute->Name(), "NAME"))
			{
				schema->setID(attribute->Value());
			}
			else if(StringUtil::stringEqualsIgnoreCase(attribute->Name(), "DEFINITION"))
			{
				std::vector<std::string> definitions = StringUtil::split(attribute->Value(), '.');

				for(std::vector<std::string>::const_iterator it = definitions.begin(); it != definitions.end(); ++it)
				{
					schema->addDefinition(it->c_str());
				}
			}
			else if(std::string(attribute->Name()).find("LEVEL-") == 0)
			{
				std::string level = std::string(attribute->Name()).substr(6, std::string(attribute->Name()).length()-6);
				int value;
				if(StringUtil::STR2NUM_SUCCESS == StringUtil::str2int(value, level.c_str()))
				{
					schema->setLevel(value);
					schema->setLevelName(attribute->Value());
				}
				else
				{
					oss.clear();
					oss.str("");
					oss << schema->getID() << " is referenced in directory but schema level LEVEL-" << level.c_str() << " can't be parsed";
					status.set(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
					errorList.append(INDENT1).append(status.get()).append("\n");
				}
			}
			else if(StringUtil::stringEqualsIgnoreCase(attribute->Name(), "DESCRIPTION"))
			{
				schema->setDescription(attribute->Value());
			}
		}//done parsing attributes for the schema

		//now check to make sure all attributes are accounted for
		if(StringUtil::stringEquals(schema->getID(), ""))
		{
			status.set(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, "Directory references a schema with no name");
			errorList.append(INDENT1).append(status.get()).append("\n");
		}

		if(schema->isDefinitionEmpty())
		{
			oss.clear();
			oss.str("");
			oss << schema->getID() << " is referenced in directory but contains no definition";
			status.set(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
			errorList.append(INDENT1).append(status.get()).append("\n");
		}

		if(StringUtil::stringEquals(schema->getLevelName(), ""))
		{
			oss.clear();
			oss.str("");
			oss << schema->getID() << " is referenced in directory but contains an unnamed level";
			status.set(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
			errorList.append(INDENT1).append(status.get()).append("\n");
		}

		if ((static_cast<Specification::SchemaLevel>(schema->getLevel()) == m_schemaLevel) && StringUtil::stringEqualsIgnoreCase(schema->getID(), "HEADER"))
		{
			m_lastHeaderName = std::string(schema->getLevelName()) + ".HEADER";
		}

		//now we load the schema into the directory
		m_directory.push_back(schema);

	}//done running through the child elements

	//the directory must have a HEADER entry for each schema level in the directory
	for(int i = 0; i <= static_cast<int>(m_schemaLevel); ++i)
	{
		bool found = false;
		for(SchemaTemplateList::const_iterator it = m_directory.begin(); it != m_directory.end(); ++it)
		{
			SchemaTemplate* schema = *it;
			std::string id = schema->getID();
			if(StringUtil::stringEqualsIgnoreCase(schema->getID(), "HEADER") && schema->getLevel() == i)
			{
				found = true;
				break;
			}
		}

		if(!found)
		{
			oss.clear();
			oss.str("");
			oss << "DIRECTORY is missing definition for LEVEL-" << i << " HEADER";
			status.set(SPECIFICATION_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
			errorList.append(INDENT1).append(status.get()).append("\n");
			break;
		}
	}

	return status;
}


bool InternalSpecification::checkValue(const std::list<Field::Type>& types, const std::string& value) const
{
	bool valid = true;

	for (std::list<Field::Type>::const_iterator it = types.begin(); it != types.end(); ++it)
	{
		Field::Type type = *it;

		switch (type)
		{
		case Field::Type::CHAR:
		{//value can only be one character
			try
			{
				(void) StringUtil::getValue<GMSEC_CHAR>(value.c_str());
			}
			catch (...)
			{
				//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_CHAR";
				valid = false;
			}

			break;
		}
		case Field::Type::BOOL:
		{//valid values: 0, 1, true, false
			if (!(StringUtil::stringEqualsIgnoreCase(value.c_str(), "0") ||
				  StringUtil::stringEqualsIgnoreCase(value.c_str(), "1") ||
				  StringUtil::stringEqualsIgnoreCase(value.c_str(), "true") ||
				  StringUtil::stringEqualsIgnoreCase(value.c_str(), "false")))
			{
				//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_BOOL";
				valid = false;
			}

			break;
		}
		case Field::Type::I16:
		{
			try
			{
				GMSEC_I64 testValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value.c_str());

				if (!(testValue >= GMSEC_I64((std::numeric_limits<GMSEC_I16>::min)()) &&
					  testValue <= GMSEC_I64((std::numeric_limits<GMSEC_I16>::max)())))
				{
					//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " is outside limits of GMSEC_I16";
					valid = false;
				}
			}
			catch (...)
			{
				//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_I16";
				valid = false;
			}

			break;
		}
		case Field::Type::U16:
		{
			if (std::string(value).find("-") != std::string::npos)
			{//no negative numbers allowed
				//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_U16";
				valid = false;
			}

			try
			{
				GMSEC_I64 testValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value.c_str());

				if (!(testValue >= GMSEC_I64((std::numeric_limits<GMSEC_U16>::min)()) &&
					  testValue <= GMSEC_I64((std::numeric_limits<GMSEC_U16>::max)())))
				{
					//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " is outside limits of GMSEC_U16";
					valid = false;
				}
			}
			catch (...)
			{
				//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_U16";
				valid = false;
			}

			break;
		}
		case Field::Type::I32:
		{
			try
			{
				GMSEC_I64 testValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value.c_str());
				
				if (!(testValue >= GMSEC_I64((std::numeric_limits<GMSEC_I32>::min)()) &&
					  testValue <= GMSEC_I64((std::numeric_limits<GMSEC_I32>::max)())))
				{
					//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " is outside limits of GMSEC_I32";
					valid = false;
				}

			}
			catch (...)
			{
				//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_I32";
				valid = false;
			}

			break;
		}
		case Field::Type::U32:
		{
			if (std::string(value).find("-") != std::string::npos)
			{//no negative numbers allowed
				//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_U32";
				valid = false;
			}

			try
			{
				GMSEC_I64 testValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value.c_str());

				if (!(testValue >= GMSEC_I64((std::numeric_limits<GMSEC_U32>::min)()) &&
					  testValue <= GMSEC_I64((std::numeric_limits<GMSEC_U32>::max)())))
				{
					//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " is outside limits of GMSEC_U32";
					valid = false;
				}
			}
			catch (...)
			{
				//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_U32";
				valid = false;
			}

			break;
		}
		case Field::Type::F32:
		{
			try
			{
				(void) StringUtil::getValue<GMSEC_F32>(value.c_str());
			}
			catch (...)
			{
				//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_F32";
				valid = false;
			}

			break;
		}
		case Field::Type::F64:
		{
			try
			{
				(void) StringUtil::getValue<GMSEC_F64>(value.c_str());
			}
			catch (...)
			{
				//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_F64";
				valid = false;
			}

			break;
		}
		case Field::Type::STRING:
			//no checks needed, anything can be a string
			break;
		case Field::Type::BINARY:
		{//BINARY field is used for data, and should not be predefining values
			//GMSEC_ERROR << "XML Parse Error: unable to evaluate BINARY values";
			valid = false;
			break;
		}
		case Field::Type::I8:
		{
			try
			{
				GMSEC_I64 testValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value.c_str());

				if (!(testValue >= GMSEC_I64((std::numeric_limits<GMSEC_I8>::min)()) &&
					  testValue <= GMSEC_I64((std::numeric_limits<GMSEC_I8>::max)())))
				{
					GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " is outside limits of GMSEC_I8";
					valid = false;
				}
			}
			catch (...)
			{
				GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_I32";
				valid = false;
			}

			break;
		}
		case Field::Type::U8:
		{
			if (std::string(value).find("-") != std::string::npos)
			{//no negative numbers allowed
				GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_U8";
				valid = false;
			}

			try
			{
				GMSEC_I64 testValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value.c_str());

				if (!(testValue >= GMSEC_I64((std::numeric_limits<GMSEC_U8>::min)()) &&
					  testValue <= GMSEC_I64((std::numeric_limits<GMSEC_U8>::max)())))
				{
					GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " is outside limits of GMSEC_U8";
					valid = false;
				}
			}
			catch (...)
			{
				GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_U8";
				valid = false;
			}

			break;
		}
		case Field::Type::I64:
		{
			try
			{
				(void) StringUtil::getValue<GMSEC_I64>(value.c_str());
			}
			catch (...)
			{
				GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_I64";
				valid = false;
			}

			break;
		}
		case Field::Type::U64:
		{
			if (std::string(value).find("-") != std::string::npos)
			{//no negative numbers allowed
				GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_U64";
				valid = false;
			}

			try
			{
				(void) StringUtil::getValue<GMSEC_U64>(value.c_str());
			}
			catch (...)
			{
				GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_U64";
				valid = false;
			}
			break;
		}
		}
	}
	return valid;
}


InternalSpecification::FieldTemplateList InternalSpecification::prepHeaders(const char* schemaID)
{
	std::string id = schemaID;
	std::string typeDef;
	std::string subtypeDef;
	std::string type;
	std::string subtype;

	// The types of Schema IDs that the API (currently) supports.
	//
	// Schema ID Type 1: major.minor.schemaLevel.messagekind.messagetype.<optionalmessagesubtype>
	// Schema ID Type 2: messagekind.messagetype.<optionalmessagesubtype>
	
	std::vector<std::string> elements = StringUtil::split(id, '.');

	if (elements.size() >= 5)        // Type 1
	{
		//the major and minor revision isn't in the schema's name in the directory
		//so we need to trim these two parts off
		id = id.substr(8,id.length()-8);

		type    = elements.at(3);
		subtype = elements.at(4);
	}
	else if (elements.size() >= 2)   // Type 2
	{
		type    = elements.at(0);
		subtype = elements.at(1);
	}

	//look up schemaID in the directory
	for (SchemaTemplateList::iterator it = m_directory.begin(); it != m_directory.end(); ++it)
	{
		SchemaTemplate*    temp = *it;
		const std::string& def  = temp->getID();

		if (def.find(id) != std::string::npos)
		{
			//found it

			//the common headers only contain the first two definitions, so we only need the first two
			typeDef    = temp->getFirstDefinition();
			subtypeDef = temp->getNextDefinition();
			break;
		}
	}

	if (typeDef.empty())
	{
		//We couldn't find the schemaID in the directory, which means it isn't explicitly defined

		//we'll instead search for a more generic definition for the schema
		std::string genID = id.substr(0, id.find_last_of("."));

		GMSEC_DEBUG << id.c_str() << " is not explicitly defined in directory, using definition " << genID.c_str();

		for (SchemaTemplateList::iterator it = m_directory.begin(); it != m_directory.end(); ++it)
		{
			SchemaTemplate* temp = *it;

			if (StringUtil::stringEquals(temp->getID(), genID.c_str()))
			{
				typeDef    = temp->getFirstDefinition();
				subtypeDef = temp->getNextDefinition();
				break;
			}
		}
	}

	if (typeDef.empty())
	{
		//We still don't have a definition for the schema

		//that's ok, we'll just default to the generic one defined by HEADER
		std::string genID = id.substr(0, id.find_last_of("."));

		GMSEC_DEBUG << genID.c_str() << " is not explicitly defined in the directory, defaulting to generic schema definition";

		for (SchemaTemplateList::iterator it = m_directory.begin(); it != m_directory.end() ; ++it)
		{
			SchemaTemplate* temp = *it;

			if (StringUtil::stringEquals(temp->getID(), "HEADER"))
			{
				typeDef    = temp->getFirstDefinition();
				subtypeDef = temp->getNextDefinition();
				break;
			}
		}
	}

	// Look up the Field Template List that is associated with the Schema ID; if we cannot
	// find the list, then default to using the Field Template List that is presumably
	// associated with the highest schema level (i.e. the DEFAULT list).
	//
	std::string headerID = (elements.size() >= 5 ? elements.at(2) : "DEFAULT");

	HeaderFieldTemplates::const_iterator it = m_headers.find(headerID);

	if (it == m_headers.end())
	{
		it = m_headers.find("DEFAULT");
	}

	FieldTemplateList fields;

	if (it != m_headers.end())
	{
		const FieldTemplateList& headerFieldList = it->second;

		for (FieldTemplateList::const_iterator it2 = headerFieldList.begin(); it2 != headerFieldList.end(); ++it2)
		{
			FieldTemplate* temp = new FieldTemplate(*(*it2));

			if (temp->getName() == typeDef)
			{
				temp->setValue(type);
			}
			else if (temp->getName() == subtypeDef)
			{
				temp->setValue(subtype);
			}

			fields.push_back(temp);
		}
	}

	return fields;
}


void InternalSpecification::resetSchemaIndex()
{	
	m_schemaIndex = m_templates.begin();
}


const char* InternalSpecification::nextID()
{
	if(hasNextID())
	{
		const MessageTemplate* temp = m_schemaIndex->second;
		m_schemaIndex++;
		return temp->getSchemaID();
	}

	throw GmsecException(SPECIFICATION_ERROR, INDEX_OUT_OF_RANGE, "next() has reached end of list");
}


bool InternalSpecification::hasNextID() const
{
	return m_schemaIndex != m_templates.end();
}


SchemaIDIterator& InternalSpecification::getSchemaIDIterator() const
{
	const_cast<InternalSchemaIDIterator*>(m_iterator.m_internal)->reset();
	return const_cast<SchemaIDIterator&>(m_iterator);
}


std::string InternalSpecification::getValue(const Message& msg, const char* name) const
{
	std::ostringstream value;

	Field::Type type = msg.getFieldType(name);

	//determine what type of value the field has, then grab the value
	switch (type)
	{
	case Field::Type::BINARY:
		value << msg.getBinaryField(name).getValue();
		break;

	case Field::Type::BOOL:
		value << msg.getBooleanField(name).getValue();
		break;

	case Field::Type::CHAR:
		value << msg.getCharField(name).getValue();
		break;

	case Field::Type::F32:
		value << msg.getF32Field(name).getValue();
		if(value.str().find(".") == std::string::npos)
		{
			value << ".0";
		}
		break;

	case Field::Type::F64:
		value << msg.getF64Field(name).getValue();
		if(value.str().find(".") == std::string::npos)
		{
			value << ".0";
		}
		break;

	case Field::Type::I16:
		value << msg.getI16Field(name).getValue();
		break;

	case Field::Type::I32:
		value << msg.getI32Field(name).getValue();
		break;

	case Field::Type::I64:
		value << msg.getI64Field(name).getValue();
		break;

	case Field::Type::I8:
		value << msg.getI8Field(name).getValue();
		break;

	case Field::Type::STRING:
		value << msg.getStringField(name).getValue();
		break;

	case Field::Type::U16:
		value << msg.getU16Field(name).getValue();
		break;

	case Field::Type::U32:
		value << msg.getU32Field(name).getValue();
		break;

	case Field::Type::U64:
		value << msg.getU64Field(name).getValue();
		break;

	case Field::Type::U8:
		value << msg.getU8Field(name).getValue();
		break;

	default:
		{//since we can conceivably turn anything into a string, we shouldn't be here
			std::stringstream err;
			err << "Unable to determine FieldType " << static_cast<int>(type) << " for field " << name;
			GMSEC_ERROR << err.str().c_str();
			throw GmsecException(SPECIFICATION_ERROR, INCORRECT_FIELD_TYPE, err.str().c_str());
		}
	}
	
	return value.str();
}


unsigned int InternalSpecification::getVersion() const
{
	return m_specVersion;
}


const util::List<const char*> InternalSpecification::getHeaderFieldNames(const char* schemaName) const
{
	util::List<const char*> headerList;

	HeaderFieldTemplates::const_iterator it = m_headers.find((schemaName && !std::string(schemaName).empty()) ? schemaName : "DEFAULT");

	if (it == m_headers.end())
	{
		std::string err = "Schema name ";
		err.append(schemaName).append(" does not exist.");
		throw GmsecException(SPECIFICATION_ERROR, MSG_LOOKUP_FAILURE, err.c_str());
	}
	else
	{
		for (FieldTemplateList::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			headerList.add((*it2)->getName().c_str());
		}
	}

	return headerList;
}


Specification::SchemaLevel InternalSpecification::getSchemaLevel() const
{
	return m_schemaLevel;
}


MessageTemplate& InternalSpecification::findTemplate(const char* schemaID)
{
	if (!schemaID || std::string(schemaID).empty())
	{
		throw GmsecException(SPECIFICATION_ERROR, TEMPLATE_ID_DOES_NOT_EXIST, "Schema ID cannot be null or contain an empty string");
	}

	MessageTemplates::const_iterator it = m_templates.find(schemaID);

	if (it != m_templates.end())
	{
		return *(it->second);
	}

	//ID may be too specific, check for the base message
	std::string baseID = schemaID;
	do
	{
		baseID = baseID.substr(0, baseID.rfind('.')); //breaking off the trailing identifier
		MessageTemplates::const_iterator it = m_templates.find(baseID);
		if (it != m_templates.end())
		{
			return *(it->second);
		}
	}
	while (StringUtil::split(baseID, '.').size() > 2);

	//A message template was not returned, which means there wasn't a matching one in the list
	std::string err = "SchemaID \"";
	err.append(schemaID);
	err.append("\" could not be found in list of available schema.");
	throw GmsecException(SPECIFICATION_ERROR, TEMPLATE_ID_DOES_NOT_EXIST, err.c_str());
}


const InternalSpecification::SchemaTemplateList& InternalSpecification::getDirectory() const
{
	return m_directory;
}


const util::List<MessageSpecification*>& InternalSpecification::getMessageSpecifications() const
{
	return m_msgSpecs;
}


InternalSpecification::FieldTemplateList InternalSpecification::cloneFieldTemplates(const FieldTemplateList& fields) const
{
	FieldTemplateList fieldTemps;

	for (FieldTemplateList::const_iterator it = fields.begin(); it != fields.end(); ++it)
	{
		fieldTemps.push_back(new FieldTemplate(*(*it)));
	}

	return fieldTemps;
}
