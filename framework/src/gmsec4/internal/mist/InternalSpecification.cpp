/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Disable the Visual Studio warning C4355
//   ('this' : used in base member initializer list)
#ifdef WIN32
#pragma warning(disable: 4355)
#endif

#include <gmsec4/internal/mist/CustomMessageValidator.h>
#include <gmsec4/internal/mist/FieldTemplate.h>
#include <gmsec4/internal/mist/InternalSpecification.h>
#include <gmsec4/internal/mist/MessageTemplate.h>
#include <gmsec4/internal/mist/InternalSchemaIDIterator.h>

#include <gmsec4/internal/mist/message/InternalMistMessage.h>

#include <gmsec4/internal/FileUtil.h>
#include <gmsec4/internal/InternalMessage.h>
#include <gmsec4/internal/MathUtil.h>
#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4/mist/message/MistMessage.h>

#include <gmsec4/mist/FieldSpecification.h>
#include <gmsec4/mist/MessageSpecification.h>
#include <gmsec4/mist/MessageValidator.h>
#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/ConfigOptions.h>
#include <gmsec4/Errors.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>
#include <gmsec4/Message.h>
#include <gmsec4/Config.h>

#include <gmsec4/util/cxx.h>
#include <gmsec4/util/Log.h>
#include <gmsec4/util/Mutex.h>
#include <gmsec4/util/StdUniquePtr.h>

#include <tinyxml2.h>

#include <limits>
#include <set>
#include <sstream>

#if GMSEC_CXX_11_AVAILABLE == 1
#include <regex>
#endif


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message::internal;


static Mutex g_mutex;

static const char* const NEWLINE_INDENT1 = "\n   ";
static const char* const NEWLINE_INDENT2 = "\n      ";
static const char* const INDENT1 = "   ";


static bool isXMLFile(const std::string& filename)
{
	//TODO MAV: Replace code below with usage of std::regex when C++11 is adopted by GMSEC.

	const char* xml_ext[] = { "xml", "XML", "xmL", "xMl", "xML", "Xml", "XmL", "XMl", NULL };

	for (int i = 0; xml_ext[i] != NULL; ++i)
	{
		if (filename.substr(filename.find_last_of('.')+1) == xml_ext[i])
			return true;
	}

	return false;
}

static bool isXSDFile(const std::string& filename)
{
	//TODO MAV: Replace code below with usage of std::regex when C++11 is adopted by GMSEC.

	const char* xml_ext[] = { "xsd", "XSD", "xsD", "xSd", "xSD", "Xsd", "XsD", "XSd", NULL };

	for (int i = 0; xml_ext[i] != NULL; ++i)
	{
		if (filename.substr(filename.find_last_of('.') + 1) == xml_ext[i])
			return true;
	}

	return false;
}


InternalSpecification::InternalSpecification(const Config& config)
	: m_config(config),
	  m_registry(),
	  m_templates(),
	  m_headers(),
	  m_directory(),
	  m_specVersionStr(),
	  m_specVersion(GMSEC_ISD_CURRENT),
	  m_validationLevel(1),
	  m_schemaLevel(Specification::LEVEL_2),
	  m_legacyTemplates(false),
	  m_iterator(*this),
	  m_basePath(),
	  m_lastHeaderName(),
	  m_configTracking(),
	  m_msgSpecs(),
	  m_customMessageValidator(0),
	  m_ceeMessageValidator(0),
	  m_templateXML()
{
	const char* path = m_config.getValue(GMSEC_SCHEMA_PATH);
	if(!path)
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
			throw Exception(MIST_ERROR, INVALID_CONFIG_VALUE, "\"GMSEC-SPECIFICATION-VERSION\" contains invalid value");
		}

		m_specVersionStr = version;
	}
	else
	{//default to the current version of the ISD
		std::ostringstream oss;
		oss << GMSEC_ISD_CURRENT;

		m_specVersion    = GMSEC_ISD_CURRENT;
		m_specVersionStr = oss.str();
	}

	const char* valLevel = m_config.getValue(GMSEC_VALIDATION_LEVEL);

	if (valLevel)
	{
		if (StringUtil::stringEqualsIgnoreCase(valLevel, "NO_ENFORCEMENT"))
		{//no validation
			m_validationLevel = 0;
		}
		else if (StringUtil::stringEqualsIgnoreCase(valLevel, "ENFORCE_REQUIRED"))
		{//validate required fields and ensure they are present in the message
			m_validationLevel = 1;
		}
		else if (StringUtil::stringEqualsIgnoreCase(valLevel, "ENFORCE_OPTIONAL"))
		{//validate required fields and ensure they are present in the message
		 //validate existing optional fields in the message, but they do not need to be present
			m_validationLevel = 2;
		}
		else if (StringUtil::stringEqualsIgnoreCase(valLevel, "ENFORCE_STRICT"))
		{//validate required fields and ensure they are present in the message
		 //validate existing optional fields in the message, but they do not need to be present
		 //custom fields not defined by template are not allowed
			m_validationLevel = 3;
		}
		else
		{//user may have supplied an integer, but we only accept values 0-3
			try {
				m_validationLevel = StringUtil::getValue<unsigned int>(valLevel);
			}
			catch (...) {
				throw Exception(MIST_ERROR, INVALID_CONFIG_VALUE, "\"GMSEC-VALIDATION-LEVEL\" contains invalid value");
			}

			if (m_validationLevel > 3)
			{
				throw Exception(MIST_ERROR, INVALID_CONFIG_VALUE, "\"GMSEC-VALIDATION-LEVEL\" contains out-of-range integer");
			}
		}
	}
	else
	{//default to 1, ENFORCE_REQUIRED
		m_validationLevel = 1;
	}

	const char* schemaLevel = m_config.getValue(GMSEC_SCHEMA_LEVEL);

	if (schemaLevel)
	{
		try {
			m_schemaLevel = (Specification::SchemaLevel) StringUtil::getValue<unsigned int>(schemaLevel);
		}
		catch (...) {
			throw Exception(MIST_ERROR, INVALID_CONFIG_VALUE, "\"GMSEC-SCHEMA-LEVEL\" contains invalid value");
		}

		if (m_schemaLevel < Specification::LEVEL_0 || m_schemaLevel > Specification::LEVEL_6)
		{//while the user can conceivably layer as many addendums as they want over the ISD,
		 //having more than 7 layered addendums is getting pretty ridiculous
			throw Exception(MIST_ERROR, INVALID_CONFIG_VALUE, "\"GMSEC-SCHEMA-LEVEL\" contains out-of-range integer");
		}
	}
	else
	{//set default schema level, depending on message specification in use
		if (getVersion() <= GMSEC_ISD_2016_00)
		{
			m_schemaLevel = Specification::LEVEL_0;
		}
		else if (getVersion() == GMSEC_ISD_2018_00)
		{
			m_schemaLevel = Specification::LEVEL_1;
		}
		else
		{
			m_schemaLevel = Specification::LEVEL_2;
		}
	}

	const char* legacyTemplates = m_config.getValue(GMSEC_LEGACY_SCHEMA_FILES);

	if (legacyTemplates)
	{//if there's a value and it's true, use XML files
		m_legacyTemplates = StringUtil::stringEqualsIgnoreCase(legacyTemplates, "true");
	}
	else
	{//default to false (use XSD) if using 201800 spec or greater
		m_legacyTemplates = (getVersion() < GMSEC_ISD_2018_00);
	}


	m_configTracking = TrackingDetails::initialize(m_config);


	// Load the schemas
	try {
		load();
	}
	catch (const Exception& e) {
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
	  m_directory(other.m_directory),
	  m_specVersionStr(other.m_specVersionStr),
	  m_specVersion(other.m_specVersion),
	  m_validationLevel(other.m_validationLevel),
	  m_schemaLevel(other.m_schemaLevel),
	  m_legacyTemplates(other.m_legacyTemplates),
	  m_iterator(*this),
	  m_basePath(other.m_basePath),
	  m_lastHeaderName(),
	  m_configTracking(other.m_configTracking),
	  m_msgSpecs(),
	  m_customMessageValidator(0),
	  m_ceeMessageValidator(0),
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
		throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, msg.c_str());
	}

	tinyxml2::XMLElement* schema = xsd.RootElement();

	if (schema == NULL)
	{
		std::string msg = "loadXSD: Loaded XML Schema is NULL";
		throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, "Loaded XML Schema is NULL");
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
				throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			std::string includePath = path + FileUtil::PATH_SEPARATOR + name;
			try
			{
				included.push_back(includeXSD(includePath.c_str()));
			}
			catch (Exception& e)
			{
				std::string err = fileName;
				err.append(": failed to include schema ").append(name).append(": ").append(e.getErrorMessage());
				throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}
		}
	}

	//determine schema ID to find body of template

	//file name
	std::string filename = fileName;

	//schema level (e.g. C2MS, GMSEC, etc.)
	std::string schemaLevelStr = StringUtil::split(filename, '_').front();

	//specification ID (e.g. 2019.00.C2MS, 2019.00.GMSEC)
	std::string specID = m_specVersionStr;
	specID.insert(4, ".").append(".").append(schemaLevelStr);

	//short-hand schema ID (e.g. HEADER, MSG.LOG, etc.)
	std::string short_id = StringUtil::split(filename.substr(0, filename.rfind('.')), '_').back();

	//fully-qualified schema ID (e.g. 2019.00.C2MS.MSG.LOG)
	std::string fullID = specID + "." + short_id;

	//try parsing the loaded XSD file
	std::string       errorList;
	FieldTemplateList fields = parseXSD(schema, short_id, included, errorList);

	//load parsed field templates from XSD file into new FieldTemplate object
	if (StringUtil::stringEqualsIgnoreCase(short_id.c_str(), "HEADER"))
	{
		//remove version numbers from full ID to get proper header ID
		std::string headerID = StringUtil::stringToUpper(StringUtil::split(filename, '_').front() + "." + short_id);
		m_headers[headerID] = cloneFieldTemplates(fields);

		//This header is on the highest schema level and will be the default header
		if (StringUtil::stringEqualsIgnoreCase(StringUtil::split(specID, '.').back().append(".").append(short_id).c_str(), m_lastHeaderName.c_str()))
		{
			m_headers["DEFAULT"] = cloneFieldTemplates(fields);
		}
	}
	else
	{
		unsigned int schemaLevel = 0;
		for (SchemaTemplateList::iterator it = m_directory.begin(); it != m_directory.end(); ++it)
		{
			const SchemaTemplate& schemaTemp = *it;

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
			m_templates[fullID] = new MessageTemplate(fullID.c_str(), fields, schemaLevel);
		}
		else
		{
			std::ostringstream errmsg;
			errmsg << "Duplicate Schema Name of " << fullID.c_str() << " found";

			throw Exception(MIST_ERROR, TEMPLATE_DIR_ERROR, errmsg.str().c_str());
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

			if (schemaLevel <= (unsigned int) m_schemaLevel)
			{
				MessageTemplate* msgTemp = new MessageTemplate(fullID.c_str(), fields, schemaLevel);

				m_templates[short_id] = msgTemp;

				if (Log::getReportingLevel() == logDEBUG)
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

			if (it->second->getSchemaLevel() < schemaLevel || m_schemaLevel == schemaLevel)
			{
				delete it->second;

				it->second = new MessageTemplate(fullID.c_str(), fields, schemaLevel);

				if (Log::getReportingLevel() == logDEBUG)
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
		throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
	}

	return xsd;
}

InternalSpecification::FieldTemplateList InternalSpecification::parseXSD(const tinyxml2::XMLElement* schema, const std::string& schemaID, const std::list<tinyxml2::XMLDocument*>& included, std::string& errorList)
{
	FieldTemplateList fields;

	if (schema->FirstChildElement("xs:complexType") && schema->FirstChildElement("xs:complexType")->Attribute("name", "HEADER"))
	{//parsed header with correct leading tag
		const tinyxml2::XMLElement* body = schema->FirstChildElement("xs:complexType");

		body = body->FirstChildElement("xs:all");
		if (body == NULL)
		{
			std::string err = schemaID;
			err.append(": schema missing <xs:all> tag");
			throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
		}

		//review field elements and create fieldTemplates
		for (const tinyxml2::XMLElement* element = body->FirstChildElement("xs:element");
			element != NULL;
			element = element->NextSiblingElement("xs:element"))
		{
			fields.push_back(parseField(schema, schemaID, included, element, errorList));
		}

		return fields;
	}
	else if (schema->LastChildElement("xs:element") && schema->LastChildElement("xs:element")->Attribute("name", schemaID.c_str()))
	{//parsed message template with correct leading tag
		const tinyxml2::XMLElement* body = schema->LastChildElement("xs:element");
		
		const tinyxml2::XMLElement* content = NULL;

		//get content of body
		if (body->Attribute("name", schemaID.c_str()))
		{
			content = body->FirstChildElement("xs:complexType");
			if (content == NULL)
			{
				std::string err = schemaID;
				err.append(": schema missing <xs:complexType> tag");
				throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			content = content->FirstChildElement("xs:all");
			if (content == NULL)
			{
				std::string err = schemaID;
				err.append(": schema missing <xs:all> tag");
				throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			content = content->LastChildElement("xs:element");
			if (content == NULL || !StringUtil::stringEquals(content->FirstAttribute()->Value(), "CONTENT"))
			{
				std::string err = schemaID;
				err.append("missing <xs:element> named CONTENT");
				throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			content = content->FirstChildElement("xs:complexType");
			if (content == NULL)
			{
				std::string err = schemaID;
				err.append(": content missing <xs:complexType> tag");
				throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			content = content->FirstChildElement("xs:all");
			if (content == NULL)
			{
				std::string err = schemaID;
				err.append(": content missing <xs:all> tag");
				throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
			}

			//review field elements and create fieldTemplates
			for (const tinyxml2::XMLElement* element = content->FirstChildElement("xs:element");
				element != NULL;
				element = element->NextSiblingElement("xs:element"))
			{
				fields.push_back(parseField(schema, schemaID, included, element, errorList));
			}
		}
		else
		{
			std::string err = schemaID;
			err.append(": last <xs:element> found is not named \"").append(schemaID.c_str()).append("\"");
			throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
		}

		return fields;
	}
	else
	{//can't properly identify xsd
		if (schemaID == "HEADER")
		{
			std::string err = schemaID;
			err.append(": schema missing <xs:complexType> tag");
			throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
		}
		else
		{
			std::string err = schemaID;
			err.append(": schema missing <xs:element> tag with attribute name=\"").append(schemaID.c_str()).append("\"");
			throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
		}
	}
}

FieldTemplate* InternalSpecification::parseField(const tinyxml2::XMLElement* schema, const std::string& schemaID, const std::list<tinyxml2::XMLDocument*>& included, const tinyxml2::XMLElement* element, std::string& errorList)
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
		throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
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
			throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
		}

		field->setPattern(extension->Attribute("base"));

		//find attribute tag to determine types
		const tinyxml2::XMLElement* attribute = extension->FirstChildElement("xs:attribute");
		if (attribute == NULL || attribute->Attribute("name", "TYPE") == NULL || attribute->Attribute("type") == NULL)
		{
			std::string err = schemaID;
			err.append(": Malformed XSD: field element \"").append(field->getName()).append("\" missing or incomplete <xs:attribute> tag");
			throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
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
					throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
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
							gmsec::api::internal::InternalField::lookupType((*it).c_str());
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
			throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
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
				field->addDependency(dependency->Attribute("name"), dependency->Attribute("valueEquals"), dependency->Attribute("valueGreaterThan"), dependency->Attribute("valueLessThan"), dependency->Attribute("use"), pattern.c_str(), values, types);
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
			throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
		}

		std::string refName = refElement->Attribute("ref");
		field->setPrefix(refName);

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
					throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
				}

				reference = reference->FirstChildElement("xs:all");
				if (reference == NULL)
				{
					std::string err = schemaID;
					err.append(": Malformed XSD: reference element \"").append(refName.c_str()).append("\" missing <xs:all> tag");
					throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
				}


				FieldTemplateList children;

				//review child fields...
				for (const tinyxml2::XMLElement* child = reference->FirstChildElement("xs:element"); 
					child != NULL; 
					child = child->NextSiblingElement("xs:element"))
				{
					children.push_back(parseField(schema, schemaID, included, child, errorList));
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
				throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
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
				throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
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
						throw Exception(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err.c_str());
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

	if (m_legacyTemplates || StringUtil::stringEqualsIgnoreCase(fileName, DIRECTORY_FILE))
	{//loading XML file located at path
		xmlError = xml.LoadFile(source);
	}
	else
	{//parsing XML-formatted string
		xmlError = xml.Parse(source);
	}

	if (xmlError != tinyxml2::XML_NO_ERROR)
	{
		std::string msg = "Unparseable XML string given from file name: ";
		msg += fileName;
		return Status(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, msg.c_str());
	}

	tinyxml2::XMLElement* schema = xml.RootElement();

	if (schema == NULL)
	{
		std::string msg = "Loaded XML schema is NULL";
		return Status(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, "Loaded XML Schema is NULL");
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

	if (!m_legacyTemplates && !StringUtil::stringEqualsIgnoreCase(fileName, DIRECTORY_FILE))
	{
		//XSD files only have shorthand schema ID, append revisions and addendum

		if (id == "HEADER")
		{
			//appending addendum to header id
			std::string fullID;
			fullID.append(StringUtil::split(fileName, '_').front()).append(".").append(id);

			id = fullID;
		}
		else
		{
			//appending version and addendum to schema id
			std::string fullID = m_specVersionStr;
			fullID.insert(4, ".");
			fullID.append(".").append(StringUtil::split(fileName, '_').front()).append(".").append(id);

			id = fullID;
		}
	}

	if (!status.isError())
	{
		if (id.find(".HEADER") != std::string::npos)
		{
			m_headers[id] = cloneFieldTemplates(fields);

			if (id == m_lastHeaderName)
			{
				m_headers["DEFAULT"] = cloneFieldTemplates(fields);
			}
		}
		else if (id != specID && id != "C2MS.HEADER")
		{
			unsigned int schemaLevel = 0;
			for (SchemaTemplateList::iterator it = m_directory.begin(); it != m_directory.end(); ++it)
			{
				const SchemaTemplate& schemaTemp = *it;

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
			if (m_templates.find(id) == m_templates.end())
			{
				m_templates[id] = new MessageTemplate(id.c_str(), fields, schemaLevel);
			}
			else
			{
				std::ostringstream errmsg;
				errmsg << "Duplicate Schema Name of " << id << " found";

				return Status(MIST_ERROR, TEMPLATE_DIR_ERROR, errmsg.str().c_str());
			}

			// We then store another copy of the Message Template using a Type 2 Schema ID (if not already stored)
			std::vector<std::string> elements = StringUtil::split(id, '.');
			if (elements.size() >= 5)
			{
				// Have Major and Minor number, and presumably, addendum name (e.g. GMSEC or COMPATC2)
				std::string short_id;
				for (size_t i = 3; i < elements.size(); ++i)
				{
					short_id.append(elements.at(i)).append(".");
				}
				short_id.erase(short_id.length() - 1);  // erase trailing period


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
				//
				MessageTemplates::iterator it = m_templates.find(short_id);

				if (it == m_templates.end())
				{
					// Not found; add new template with short-hand schema ID

					if (schemaLevel <= (unsigned int) m_schemaLevel)
					{
						MessageTemplate* msgTemp = new MessageTemplate(id.c_str(), fields, schemaLevel);

						m_templates[short_id] = msgTemp;

						if (Log::getReportingLevel() == logDEBUG)
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

					if (it->second->getSchemaLevel() < schemaLevel || m_schemaLevel == schemaLevel)
					{
						delete it->second;

						it->second = new MessageTemplate(id.c_str(), fields, schemaLevel);

						if (Log::getReportingLevel() == logDEBUG)
						{
							std::ostringstream oss;
							oss << "Replacing Message Template with:\n" << *(it->second) << std::endl;
							GMSEC_DEBUG << oss.str().c_str();
						}
					}
				}
			}
		}
	}
	xml.Clear();

	for (FieldTemplateList::iterator it = fields.begin(); it != fields.end(); ++it)
	{
		delete *it;
	}
	fields.clear();
	

	return status;
}


void InternalSpecification::validateMessage(const Message& msg)
{
	if (m_validationLevel == 0)
	{
		GMSEC_DEBUG << "Specification configured to validation level 0 (NO_ENFORCEMENT). Skipping validation.";
		return;
	}

	Status status;

	if (m_customMessageValidator != NULL)
	{
		status = m_customMessageValidator->validateMessage(msg);

		if (status.isError())
		{
			throw Exception(status);
		}
	}

	std::string schemaID;
	const MessageTemplate* temp = NULL;
	bool foundSchema = false;
	bool needHeaders = true;

	try {
		// If the Message is a MistMessage, then we can readily acquire the MessageTemplate.

		const InternalMessage& intMsg = MessageBuddy::getInternal(msg);

		const InternalMistMessage& intMistMsg = dynamic_cast<const InternalMistMessage&>(intMsg);

		temp = &(intMistMsg.getTemplate());

		schemaID = temp->getID();

		foundSchema = true;
		needHeaders = false;
	}
	catch (...) {
		foundSchema = false;
	}

	if (!foundSchema)
	{
		// If here, we have to lookup the MessageTemplate.

		//TODO MAV: Different messages may share the same message subject. For example,
		//          telemetry messages may have an invalid subject that do not contain
		//          elements ME1-ME4, but nevertheless contain valid fields (that indicate FORMAT).
		//          If a CCSDS-Packet TLM message and subsequently a CCSDS-Frame TLM message,
		//          each using the same subject, are received, then the second message will be confused
		//          with the first.
		//
		//          In summary, we need to validate subjects before ever checking the registry.
		//          This will ensure that only valid/unique subjects are registered within the registry.
		//
		SchemaRegistry::const_iterator it = m_registry.find(msg.getSubject());

		if (it != m_registry.end())
		{
			//a matching message subject was found in the registry!
			//grabbing the schemaID registered to the message subject
			schemaID = it->second;
		}
		else
		{
			//the message subject is not registered with a schemaID yet
	 		//we'll have to build a schemaID for the message and register it
			schemaID = registerTemplate(msg, m_schemaLevel);

			if (schemaID.empty())
			{
				//if registerTemplate returned a blank string, then that means a matching template wasn't found
				std::string err = "Failed to build Schema ID from message ";
				err.append(msg.getSubject());
				throw Exception(MIST_ERROR, TEMPLATE_ID_DOES_NOT_EXIST, err.c_str());
			}
		}

		//now grab the message template with the matching schemaID
		MessageTemplates::const_iterator it2 = m_templates.find(schemaID);

		if (it2 != m_templates.end())
		{
			temp = it2->second;

			foundSchema = true;
		}
	}

	if (foundSchema)
	{
		//this list of fields will contain the combined header and contents
		//first prepare the header fields with the values we need to validate
		FieldTemplateList fields;

		if (needHeaders)
		{
			fields = prepHeaders(schemaID.c_str());
		}

		const FieldTemplateList& msgFieldTemplates = temp->getFieldTemplates();

		for (FieldTemplateList::const_iterator it = msgFieldTemplates.begin(); it != msgFieldTemplates.end(); ++it)
		{
			//add content fields
			fields.push_back(new FieldTemplate(*(*it)));
		}

		if (Log::getReportingLevel() == logVERBOSE)
		{
			std::ostringstream oss;
			oss << "Validating using Message Template:\n" << *temp << std::endl;
			GMSEC_VERBOSE << oss.str().c_str();
		}

		//compare the field list to the fields in the message
		std::string prefix;
		std::string validFieldNames;
		status = compare(msg, fields, prefix, validFieldNames);

		for (FieldTemplateList::iterator it = fields.begin(); it != fields.end(); ++it)
		{
			delete *it;
		}
		fields.clear();
	}
	else
	{
		std::string err = "SchemaID ";
		err.append(schemaID);
		err.append(" could not be found in list of available schema.");
		status.set(MIST_ERROR, MSG_LOOKUP_FAILURE, err.c_str());
	}

	if (status.isError())
	{
		std::string err = msg.getSubject();
		err.append(": Message Validation Failed.");
		err.append(NEWLINE_INDENT1).append(status.getReason());

		throw Exception(MIST_ERROR, MESSAGE_FAILED_VALIDATION, err.c_str());
	}
}


Status InternalSpecification::compare(const Message& msg, const FieldTemplateList& fields, const std::string& prefix, std::string& validFieldNames)
{
	//TODO MAV: Drop support for legacy templates so we can finally ditch the array control fields spaghetti code (container control fields does it better)

	//Overall status (that will be returned)
	Status status;

	//The accumulated error messages as a string
	std::string errorList;

	//flag that indicates how many array controls we are currently inside of
	size_t arrayLevel = 0;

	//flag that determines whether the optional array control we are in should be skipped or not
	bool skipArray = false;

	//If we are to skip an array of values, this will tell us at what level of the array control we are skipping
	//skipping an array control at a specified level means skipping all the levels above it
	size_t skipArrayLevel = 0;

	//list of placeholder strings that are to be replaced by index numbers in the field's name
	//push a placeholder at the end of the list as an array control is opened
	//pop a placeholder at the end of the list as an array control is closed
	//always grab the value from the end of the list
	std::list<std::string> arrayCharList;

	//list of the sizes of the currently open array controls
	std::list<size_t> arraySizeList;

	//list of the current index in the currently open array controls
	//this number will replace the string being held in arrayCharList
	//Index starts at 1 per GMSEC ISD
	std::list<size_t> arrayIndexList;

	//list of the start position of each open array control in the list of field templates
	std::list<size_t> arrayStartList;

	//iterating through all the fields to make sure they are all in the message and correct
	for (size_t fieldIndex = 0; (fieldIndex < fields.size()); ++fieldIndex)
	{
		//the current iteration's field template we will be validating against
		FieldTemplate* temp = fields[fieldIndex];

		//check to see if the current field template is a control field
		if (temp->getMode() == "CONTROL" && temp->getName() == "ARRAY-START")
		{
			++arrayLevel;

			skipArrayLevel = arrayLevel;

			//now we need to figure out details of the array
			try
			{
				//index for objects in messages start at 1 per GMSEC ISD
				arrayIndexList.push_back(1);

				//push the value of the control field (which is the index placeholder) onto the list
				arrayCharList.push_back(temp->getArrayControlValue());

				//determine the size of the array
				if (arrayLevel > 1)
				{
					//array is nested, the field name that contains the size of the nested array has an index placeholder
					//index placeholder needs to be replaced with the index of the object in the array
					std::string name = prepFieldName(temp->getSize().c_str(), arrayCharList, arrayIndexList);
					size_t size = msg.getIntegerValue(name.c_str());
					arraySizeList.push_back(size);
				}
				else
				{
					size_t size = msg.getIntegerValue(temp->getSize().c_str());
					arraySizeList.push_back(size);
				}

				//starting index for this array control, bookmarked so we can loop back to it later
				arrayStartList.push_back(fieldIndex);
				continue;
			}
			catch (const Exception &e)
			{
				if (e.getErrorClass() == MSG_ERROR && e.getErrorCode() == INVALID_FIELD)
				{
					//this catch occurred because the size of the array is undefined due to the field being missing
					//if that field is optional, then it means the array is optional, so we skip the array entirely
					//if the array is required, we throw the error
					for (FieldTemplateList::const_iterator it2 = fields.begin(); it2 != fields.end(); ++it2)
					{
						if ((*it2)->getName() == temp->getSize() && (*it2)->getMode() == "OPTIONAL")
						{
							//we have determined the array is optional and we will be skipping it
							skipArray = true;
							break;
						}
					}

					if (!skipArray)
					{
						// Major validation error; we're done.
						std::ostringstream err;
						err << msg.getSubject() << " : Message Validation Failed." << "\n   "
						    << "Message contains array of objects whose size is undefined: "
						    << e.what();
						status.set(MIST_ERROR, MESSAGE_FAILED_VALIDATION, err.str().c_str());
					}
				}
				else
				{
					// Unknown/unexpected error; we're done.
					status.set(MIST_ERROR, MESSAGE_FAILED_VALIDATION, e.what());
				}

				continue;
			}
		}
		else if (temp->getMode() == "CONTROL" && temp->getName() == "ARRAY-END")
		{
			if (skipArray && skipArrayLevel == arrayLevel)
			{
				//we've reached the end of the array that is being skipped
				skipArray = false;
				skipArrayLevel = 0;  // MAV -- is this correct, or --skipArrayLevel???
				if (arrayIndexList.size() > 0) {
					arrayIndexList.pop_back();
				}
				if (arrayCharList.size() > 0) {
					arrayCharList.pop_back();
				}
				if (arraySizeList.size() > 0) {
					arraySizeList.pop_back();
				}
				if (arrayStartList.size() > 0) {
					arrayStartList.pop_back();
				}
				--arrayLevel;
				continue;
			}

			if (arrayIndexList.back() < arraySizeList.back())
			{
				//there are still more objects in the array to validate
				//reset fieldIndex to the beginning of the array control
				//increase the listed index by 1
				if (arrayStartList.size() > 0) {
					fieldIndex = arrayStartList.back();
					size_t index = arrayIndexList.back();
					arrayIndexList.pop_back();
					arrayIndexList.push_back(++index);
				}
				continue;
			}
			else
			{
				//all objects have been processed, close the array control
				if (arrayIndexList.size() > 0) {
					arrayIndexList.pop_back();
				}
				if (arrayCharList.size() > 0) {
					arrayCharList.pop_back();
				}
				if (arraySizeList.size() > 0) {
					arraySizeList.pop_back();
				}
				if (arrayStartList.size() > 0) {
					arrayStartList.pop_back();
				}
				--arrayLevel;
				continue;
			}
		}
		else if (skipArray)
		{
			continue;
		}

		if (StringUtil::stringEqualsIgnoreCase(temp->getMode().c_str(), "CONTROL") && temp->hasChildren())
		{//encountered control field which contains an array of fields
			//determine size of the array
			std::string arraySizeName = prefix + "NUM-OF-" + temp->getName();
			size_t size = 0;
			//TODO MAV: refactor code to not involve throwing internal exception
			try
			{
				size = msg.getIntegerValue(arraySizeName.c_str());
			}
			catch (const Exception& e)
			{
				if (e.getErrorClass() == MSG_ERROR && e.getErrorCode() == INVALID_FIELD)
				{
					//this catch occurred because the size of the array is undefined due to the field being missing
					//if that field is optional, then it means the array is optional, so we skip the array entirely
					//if the array is required, we throw the error
					for (FieldTemplateList::const_iterator it2 = fields.begin(); it2 != fields.end(); ++it2)
					{
						std::string fieldName = prefix + (*it2)->getName();
						if (StringUtil::stringEqualsIgnoreCase(fieldName.c_str(), arraySizeName.c_str()))
						{
							(*it2)->checkDependencies(msg);
							if (StringUtil::stringEqualsIgnoreCase((*it2)->getMode().c_str(), "OPTIONAL"))
							{
								(*it2)->resetDependencies();
								skipArray = true;
								break;
							}
							(*it2)->resetDependencies();
						}
						if (StringUtil::stringEqualsIgnoreCase(fieldName.c_str(), arraySizeName.c_str()) && StringUtil::stringEqualsIgnoreCase((*it2)->getMode().c_str(), "OPTIONAL"))
						{
							//we have determined the array is optional and we will be skipping it
							skipArray = true;
							break;
						}
					}

					if (!skipArray)
					{
						// Major validation error; we're done.
						std::ostringstream err;
						err << msg.getSubject() << " : Message Validation Failed." << "\n   "
							<< "Message contains array of objects whose size is undefined: "
							<< e.what();
						status.set(MIST_ERROR, MESSAGE_FAILED_VALIDATION, err.str().c_str());
					}
					skipArray = false;
				}
				else
				{
					// Unknown/unexpected error; we're done.
					status.set(MIST_ERROR, MESSAGE_FAILED_VALIDATION, e.what());
				}

				continue;
			}

			for (size_t i = 1; i <= size; ++i)
			{
				std::ostringstream newPrefix;
				newPrefix << prefix << temp->getPrefix() << "." << i << ".";

				status = compare(msg, temp->getChildren(), newPrefix.str(), validFieldNames);

				if (status.isError())
				{
					errorList.append(status.getReason()).append(NEWLINE_INDENT1);
				}
			}
		}

		//modify the name of the field template as necessary
		if (arrayLevel > 0 && (arraySizeList.size() > 0 && arraySizeList.front() > 0))
		{
			std::string name = prepFieldName(temp->getName().c_str(), arrayCharList, arrayIndexList);

			temp->setModifiedName(name.c_str());
		}
		else if (!StringUtil::stringEqualsIgnoreCase(temp->getMode().c_str(), "CONTROL"))
		{
			std::string name = prefix + temp->getName();
			temp->setModifiedName(name.c_str());
		}

		//keep record of valid field names
		if (!StringUtil::stringEqualsIgnoreCase(temp->getMode().c_str(), "CONTROL"))
		{
			validFieldNames.append(" ").append(temp->getModifiedName()).append(" ");
		}

		//begin validation process for the field
		if (temp->getMode() == "REQUIRED" && m_validationLevel > 0)
		{
			//Required fields must be validated if m_validation is set at all
			temp->checkDependencies(msg);
			Status valStatus = validate(msg, *temp);

			if (valStatus.isError())
			{
				errorList.append(valStatus.getReason()).append(NEWLINE_INDENT1);
			}
			temp->resetDependencies();
		}
		else if (temp->getMode() == "OPTIONAL" && m_validationLevel > 1)
		{
			//Optional fields must be validated if m_validation is set greater than 1
			temp->checkDependencies(msg);
			Status valStatus = validate(msg, *temp);

			if (valStatus.isError())
			{
				errorList.append(valStatus.getReason()).append(NEWLINE_INDENT1);
			}
			temp->resetDependencies();
		}

		// Regardless of validation level, always validate content of the message fields
		Status status = validateContent(msg, *temp);
		if (status.isError())
		{
			errorList.append(status.getReason()).append(NEWLINE_INDENT1);
		}
	}//loop until we're done running through all the field templates

	// Check if tracking fields need to be validated.
	bool msgBeingSent = false;
	MessageBuddy::getInternal(msg).getDetails().getBoolean(GMSEC_MSG_BEING_SENT, msgBeingSent);

	const TrackingDetails& msgTracking = MessageBuddy::getInternal(msg).getTracking();
	const MessageFieldIterator& iter = msg.getFieldIterator();

	//Iterate through all the fields in the message, check for API reserved and, if necessary, extraneous fields
	while (iter.hasNext())
	{
		const Field& field = iter.next();

		//When message is being sent, disallow tracking fields (these are reserved for use by the API).
		if (msgBeingSent && isReservedField(field.getName()))
		{
			Status status = checkTrackingField(field.getName(), m_configTracking, msgTracking);

			if (status.isError())
			{
				errorList.append(status.getReason()).append(NEWLINE_INDENT1);
			}
		}

		//Disallow extraneous fields (test is only applicable if validation level is 3 or more)
		std::string fieldName = " ";
		fieldName.append(field.getName()).append(" ");

		if (m_validationLevel > 2 && fieldName.find(prefix) != std::string::npos && validFieldNames.find(fieldName) == std::string::npos)
		{
			std::ostringstream err;
			err << "Message contains user-defined field " << field.getName() 
				<< ", which is disallowed under the current validation settings";
			Status error(MIST_ERROR, NON_ALLOWED_FIELD, err.str().c_str());
			if (errorList.find(err.str()) == std::string::npos) errorList.append(error.getReason()).append(NEWLINE_INDENT1);
		}
	}


	if (errorList.empty())
	{
		GMSEC_VERBOSE << msg.getSubject() << ": Validation Passed.";
	}
	else
	{
		size_t pos = errorList.rfind("\n");
		errorList = errorList.substr(0, pos);

		status = Status(MIST_ERROR, MESSAGE_FAILED_VALIDATION, errorList.c_str());
	}

	return status;
}


std::string InternalSpecification::prepFieldName(const char* name, const std::list<std::string>& charList, const std::list<size_t>& indexList)
{
	std::string newName = name;

	std::list<std::string>::const_iterator arrayChar = charList.begin();

	for (std::list<size_t>::const_iterator arrayIndex = indexList.begin();
	     arrayIndex != indexList.end() && arrayChar != charList.end();
	     ++arrayIndex, ++arrayChar)
	{
		//iterating through arrayCharList and arrayIndexList concurrently
		std::ostringstream oss;
		
		oss << *arrayIndex;

		if (newName.find(*arrayChar) != std::string::npos)
		{
			newName.replace(newName.find(*arrayChar), arrayChar->length(), oss.str());
		}
	}

	return newName;
}

Status InternalSpecification::validate(const Message& msg, const FieldTemplate& ftmp)
{
	Status      status;
	std::string cumulativeError;
	
	const std::string& mode = ftmp.getMode();

	//now look for a field in the message with a matching name
	const Field* msgField = msg.getField(ftmp.getModifiedName().c_str());

	if (msgField)
	{//we found the field

		if (!ftmp.isTypeVariable())
		{
			std::list<Field::FieldType> types = ftmp.getTypes();

			//get the list of possible valid types the field could have

			bool valid = false;
			for (std::list<Field::FieldType>::const_iterator it = types.begin(); it != types.end(); ++it)
			{//running through the list of types the field template has

				Field::FieldType type = *it;
					
				if (msgField->getType() == type)
				{//the field's type needs to match one of the types in the list
					valid = true;
					break;
				}
			}

			if (!valid)
			{
				std::stringstream err;
				err << msgField->getName()
					<< " has incorrect field type of " << getTypeStr(msgField->getType())
					<< ".  Valid type(s): " << ftmp.getConcatenatedTypes().c_str();
				status.set(MIST_ERROR, INCORRECT_FIELD_TYPE, err.str().c_str());
				cumulativeError.append(err.str().c_str()).append(NEWLINE_INDENT1);
			}
		}
			
		if (!ftmp.getValues().empty())
		{
			//if the template has values in its list, the message field's value needs to be validated as well
			//for the field to be valid, its value must match at least one value in the field template's value list

			bool valid = false;

			Field::FieldType type = msgField->getType();

			const std::list<std::string>& values = ftmp.getValues();

			switch (type)
			{
			case Field::CHAR_TYPE:
			case Field::STRING_TYPE:
			{
				for (std::list<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
				{
					//checking message value against the list of values in the field template
					//the message value must match one value in the list to be valid
					const std::string& testValue = *it;

					if (testValue == msgField->getStringValue())
					{
						valid = true;
						//message value found a match, no need to compare to the rest of the values
						break;
					}
				}
				break;
			}

			case Field::I8_TYPE:
			case Field::I16_TYPE:
			case Field::I32_TYPE:
			case Field::I64_TYPE:
			{
				for (std::list<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
				{
					GMSEC_I64 msgValue = msgField->getIntegerValue();

					//Check test value ranges
					const std::string& value = *it;
					if (value.find("..") != std::string::npos)
					{
						//finite range

						//split the range into its upper and lower bounds, then check the bounds
						std::vector<std::string> range = StringUtil::split(value, "..");

						GMSEC_I64 lowerBound = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(range.at(0).c_str());
						GMSEC_I64 upperBound = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(range.at(1).c_str());

						if (msgValue >= lowerBound && msgValue <= upperBound)
						{
							valid = true;
							break;
						}
					}
					else if (StringUtil::stringEquals(&value.at(value.length() - 1), "+"))
					{
						//lower bound

						GMSEC_I64 lowerBound = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value.substr(0, value.length() - 1).c_str());

						if (msgValue >= lowerBound)
						{
							valid = true;
							break;
						}
					}
					else if (StringUtil::stringEquals(&value.at(value.length() - 1), "-"))
					{
						//Upper bound

						GMSEC_I64 upperBound = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value.substr(0, value.length() - 1).c_str());

						if (msgValue <= upperBound)
						{
							valid = true;
							break;
						}
					}
					else
					{
						//absolute value

						GMSEC_I64 testValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value.c_str());

						if (msgValue == testValue)
						{
							valid = true;
							break;
						}
					}
				}
				break;
			}

			case Field::U8_TYPE:
			case Field::U16_TYPE:
			case Field::U32_TYPE:
			case Field::U64_TYPE:
			{
				for (std::list<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
				{
					GMSEC_U64 msgValue = msgField->getUnsignedIntegerValue();

					//Check test value ranges
					const std::string& value = *it;

					if (value.find("..") != std::string::npos)
					{
						//finite range

						//split the range into its upper and lower bounds, then check the bounds
						std::vector<std::string> range = StringUtil::split(value, "..");

						GMSEC_U64 lowerBound = StringUtil::getValue<GMSEC_U64>(range.at(0).c_str());
						GMSEC_U64 upperBound = StringUtil::getValue<GMSEC_U64>(range.at(1).c_str());

						if (msgValue >= lowerBound && msgValue <= upperBound)
						{
							valid = true;
							break;
						}
					}
					else if (StringUtil::stringEquals(&value.at(value.length() - 1), "+"))
					{
						//lower bound

						GMSEC_U64 lowerBound = StringUtil::getValue<GMSEC_U64>(value.substr(0, value.length() - 1).c_str());

						if (msgValue >= lowerBound)
						{
							valid = true;
							break;
						}
					}
					else if (StringUtil::stringEquals(&value.at(value.length() - 1), "-"))
					{
						//Upper bound

						GMSEC_U64 upperBound = StringUtil::getValue<GMSEC_U64>(value.substr(0, value.length() - 1).c_str());

						if (msgValue <= upperBound)
						{
							valid = true;
							break;
						}
					}
					else
					{
						//absolute value

						GMSEC_U64 testValue = StringUtil::getValue<GMSEC_U64>(value.c_str());

						if (msgValue == testValue)
						{
							valid = true;
							break;
						}
					}
				}
				break;
			}

			case Field::F32_TYPE:
			case Field::F64_TYPE:
			{
				for (std::list<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
				{
					GMSEC_F64 msgValue = msgField->getDoubleValue();

					//Check test value ranges
					const std::string& value = *it;

					if (value.find("..") != std::string::npos)
					{
						//finite range

						//split the range into its upper and lower bounds, then check the bounds
						std::vector<std::string> range = StringUtil::split(value, "..");

						GMSEC_F64 lowerBound = StringUtil::getValue<GMSEC_F64>(range.at(0).c_str());
						GMSEC_F64 upperBound = StringUtil::getValue<GMSEC_F64>(range.at(1).c_str());

						if (msgValue >= lowerBound && msgValue <= upperBound)
						{
							valid = true;
							break;
						}
					}
					else if (StringUtil::stringEquals(&value.at(value.length() - 1), "+"))
					{
						//lower bound

						GMSEC_F64 lowerBound = StringUtil::getValue<GMSEC_F64>(value.substr(0, value.length() - 1).c_str());

						if (msgValue > lowerBound || MathUtil::doubleEquals(msgValue, lowerBound))
						{
							valid = true;
							break;
						}
					}
					else if (StringUtil::stringEquals(&value.at(value.length() - 1), "-"))
					{
						//Upper bound

						GMSEC_F64 upperBound = StringUtil::getValue<GMSEC_F64>(value.substr(0, value.length() - 1).c_str());

						if (msgValue < upperBound || MathUtil::doubleEquals(msgValue, upperBound))
						{
							valid = true;
							break;
						}
					}
					else
					{
						//absolute value

						GMSEC_F64 testValue = StringUtil::getValue<GMSEC_F64>(value.c_str());

						if (MathUtil::doubleEquals(msgValue, testValue))
						{
							valid = true;
							break;
						}
					}
				}
				break;
			}

			case Field::BOOL_TYPE:
			{
				for (std::list<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
				{
					bool msgValue          = msg.getBooleanField(msgField->getName()).getValue();
					const std::string& str = *it; 
					bool testValue         = (str == "1" || StringUtil::stringEqualsIgnoreCase(str.c_str(), "true"));

					if (msgValue == testValue)
					{
						valid = true;
						break;
					}
				}
				break;
			}

			case Field::BIN_TYPE:
			{
				//Nothing to check here
				break;
			}
			}//end switch

			if (!valid)
			{
				std::stringstream err;
				err << msgField->getName()
			 		<< " has incorrect value of " << msgField->getStringValue()
			 		<< ", acceptable values: " << ftmp.getConcatenatedValues();
				status.set(MIST_ERROR, FIELD_NOT_SET, err.str().c_str());
				cumulativeError.append(err.str().c_str()).append(NEWLINE_INDENT1);
			}
		}
	}
	else
	{//we couldn't find a matching field
		if (mode == "REQUIRED")
		{//validation fails at this point if the field is required
			std::stringstream err;
			err << ftmp.getModifiedName() << " is a required field, but is missing from message";
			status.set(MIST_ERROR, MISSING_REQUIRED_FIELD, err.str().c_str());
			cumulativeError.append(err.str().c_str()).append(NEWLINE_INDENT1);
		}
	}

	if (cumulativeError.empty())
	{
		status = Status();
	}
	else
	{
		size_t pos = cumulativeError.rfind("\n");
		cumulativeError = cumulativeError.substr(0, pos);

		status = Status(MIST_ERROR, FIELD_FAILED_VALIDATION, cumulativeError.c_str());
	}

	return status;
}

Status InternalSpecification::validateContent(const Message& msg, const FieldTemplate& ftmp)
{
	Status status;

	const Field* msgField = msg.getField(ftmp.getModifiedName().c_str());

	if (msgField != NULL && !isReservedField(msgField->getName()))
	{
		if ((ftmp.getPattern() == "HEADER_STRING_Type") && !StringUtil::isValidHeaderString(msgField->getStringValue()))
		{
			std::stringstream err;
			err << msgField->getName() << " has illegal Header String value of '" << msgField->getStringValue() << "'";
			status.set(MIST_ERROR, FIELD_NOT_SET, err.str().c_str());
		}
		else if ((ftmp.getPattern() == "TIME_Type") && !StringUtil::isValidTimestamp(msgField->getStringValue()))
		{
			std::stringstream err;
			err << msgField->getName() << " has illegal Timestamp value of '" << msgField->getStringValue() << "'";
			status.set(MIST_ERROR, FIELD_NOT_SET, err.str().c_str());
		}
		else if ((ftmp.getPattern() == "IP_ADDRESS_Type") && !StringUtil::isValidIpAddress(msgField->getStringValue()))
		{
			std::stringstream err;
			err << msgField->getName() << " has illegal IP Address value of '" << msgField->getStringValue() << "'";
			status.set(MIST_ERROR, FIELD_NOT_SET, err.str().c_str());
		}
	}

	return status;
}

std::string InternalSpecification::registerTemplate(const Message& msg, GMSEC_I64 schemaLevel)
{
	std::string typeDef;
	std::string subTypeDef;

	//when searching for a match in the directory, we want to start schema level indicated by the user
	std::string levelName;

	//This is the schemaID minus the spec version. We'll look this name up in the directory
	//to see if a more specific definition is available
	std::string schemaID;

	//this will be marked true once we've figured out the most accurate definition for the message we can find
	bool found = false;

	while(!found && schemaLevel>=0)
	{
		for(SchemaTemplateList::iterator it = m_directory.begin(); it != m_directory.end(); ++it)
		{//now searching through the directory for the proper header definition
			SchemaTemplate& temp = *it;

			if(StringUtil::stringEquals(temp.getID(), "HEADER") && temp.getLevel() == schemaLevel)
			{//found a matching header, now grab the default definition from it
				levelName = temp.getLevelName();
				typeDef = temp.getFirstDefinition();
				subTypeDef = temp.getNextDefinition();
			
				try
				{//add type and subtype to name
					schemaID = msg.getStringValue(typeDef.c_str());
					schemaID.append(".").append(msg.getStringValue(subTypeDef.c_str()));
				}
				catch(const Exception& e)
				{//MESSAGE-TYPE or MESSAGE-SUBTYPE field was not found in message, validation automatically fails (not good)
					GMSEC_ERROR << e.what();
					return "";
				}	
				break;
			}	
		}
		//now we look for a more specific definition that matches up with the message
		found = findDefinition(schemaLevel, schemaID, msg);

		if(!found)
		{
			schemaLevel--;
		}
	}
		
	//Now look for matching schemaID in list of templates
	while(schemaLevel >=0)
	{//searching on the highest layer first, as defined by the user
		MessageTemplates::const_iterator it = m_templates.find(schemaID);
		if (it != m_templates.end())
		{
			GMSEC_VERBOSE << "Subject " << msg.getSubject() << " registered with Schema ID " << schemaID.c_str();
			m_registry.insert(std::pair<std::string, std::string>(msg.getSubject(), schemaID));
			return schemaID;
		}

		//If here, we did not find the schemaID; try a lookup at the next lowest level
		return registerTemplate(msg, --schemaLevel);
	}

	//if this return statement was reached then it means that there was 
	//no matching schemaID in the list of templates
	GMSEC_ERROR << "Schema ID " << schemaID.c_str() << " does not exist in templates";
	return "";
}


bool InternalSpecification::findDefinition(size_t schemaLevel, std::string& name, const Message& msg)
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
			SchemaTemplate& temp = *it;
			if(StringUtil::stringEquals(temp.getID(), name.c_str()) && temp.getLevel() == (size_t) schemaLevel)
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

	m_directory.clear();
	m_registry.clear();

	for (MessageSpecifications::iterator it = m_msgSpecs.begin(); it != m_msgSpecs.end(); ++it)
	{
		delete *it;
	}
	m_msgSpecs.clear();

	delete m_ceeMessageValidator;
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
			throw Exception(MIST_ERROR, TEMPLATE_DIR_NOT_FOUND, "Could not get the system-specified base path.");
		}
		size_t dirMarker = shared_object_path.rfind(FileUtil::PATH_SEPARATOR);

		if (dirMarker == std::string::npos)
		{
			std::string err_msg = "Could not list files in template directory ";
			err_msg += template_dir;
			throw Exception(MIST_ERROR, TEMPLATE_DIR_NOT_FOUND, err_msg.c_str());
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
		throw Exception(MIST_ERROR, TEMPLATE_DIR_ERROR, err_msg.c_str());
	}

	// Always load the DIRECTORY file first...
	std::string path = template_dir;
	path += FileUtil::PATH_SEPARATOR;
	path += DIRECTORY_FILE;

	Status result = loadTemplate(path.c_str(), DIRECTORY_FILE);

	if (m_directory.empty() || result.isError())
	{
		std::string err = "Template Directory information failed to load: ";
		err += result.get();
		status.set(MIST_ERROR, TEMPLATE_DIR_ERROR, err.c_str());
		throw Exception(status);
	}
			
	// Then other files...
	for (std::list<std::string>::iterator it = schema_files.begin(); it != schema_files.end(); ++it)
	{
		const std::string& filename = *it;

		if (m_legacyTemplates)
		{//Specification is configured to take in legacy XML message templates

			// We do not care about directories (current or above), or the DIRECTORY file (which has
			// already been processed)
			if (filename != ".." && 
				filename != "." && 
				!StringUtil::stringEquals(filename.c_str(), DIRECTORY_FILE) &&
				isXMLFile(filename))
			{
				std::string levelID = filename.substr(0, filename.find('.'));
				std::string schemaID = filename.substr(filename.find('.')+1, filename.length());
				schemaID = schemaID.substr(0, schemaID.find_last_of('.'));
				bool skip = false;

				for (SchemaTemplateList::iterator it = m_directory.begin(); it != m_directory.end(); ++it)
				{
					if (schemaID.find(it->getID()) != std::string::npos && StringUtil::stringEquals(levelID.c_str(), it->getLevelName()) && it->getLevel() > (unsigned int)m_schemaLevel)
					{
						skip = true;
						break;
					}
				}

				if (skip) continue;

				path = template_dir;
				path += FileUtil::PATH_SEPARATOR;
				path += filename;

				//passing a path to an XML file to loadTemplate()
				result = loadTemplate(path.c_str(), filename.c_str());

				if (result.isError())
				{
					oss.clear();
					oss.str("");
					oss << "Failed to parse " << filename.c_str() << "\n" << result.getReason() << "\n" << (status.isError() ? status.get() : "");
					status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
				}
			}
		}
		else
		{//Specification is configured to take in XSD message templates
			if (filename != ".." && 
				filename != "." && 
				!StringUtil::stringEquals(filename.c_str(), DIRECTORY_FILE) && 
				!StringUtil::stringEquals(filename.c_str(), FIELDS_FILE) && 
				!StringUtil::stringEquals(filename.c_str(), DEFAULTS_FILE) && 
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
					if (schemaID.find(it->getID()) != std::string::npos && StringUtil::stringEquals(levelID.c_str(), it->getLevelName()) && it->getLevel() > (unsigned int)m_schemaLevel)
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
						oss << "Failed to parse " << filename.c_str() << NEWLINE_INDENT1 << errors.c_str() << (status.isError() ? "\n" : "") << (status.isError() ? status.get() : "");
						status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
					}
				}
				catch (const Exception& e)
				{
					oss.clear();
					oss.str("");
					oss << "Failed to parse " << filename.c_str() << "\n" << e.getErrorMessage() << (status.isError() ? "\n" : "") << (status.isError() ? status.get() : "");
					status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
				}
			}
		}
	}

	if (status.isError())
	{
		throw Exception(status);
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
		
		DataList<FieldSpecification*> fieldSpecs;

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
		m_msgSpecs.push_back(new MessageSpecification(schemaID.c_str(), fieldSpecs));
	}
}

Status InternalSpecification::parser(tinyxml2::XMLElement* xmlSchema, std::string& schemaID, FieldTemplateList& schemaFields)
{
	Status status;
	std::ostringstream oss;

	//When the parser encounters an ARRAY-START control field, this flag will set
	//an error will be thrown if the parser encounters an ARRAY-END control field and the flag is not set
	//the number goes up as more array controls are opened, down as array controls are closed
	int arrayControlActive = 0;

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
		status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, 
				   "The schema read did not have a template id NAME specified.");
		return status;
	}
	else if(schemaID == id)
	{//This isn't a regular schema file we're parsing, this is the directory file
	 //that calls for a different operation
		for(tinyxml2::XMLElement* element = xmlSchema->FirstChildElement();
			element != NULL && !status.isError();
			element = element->NextSiblingElement())
		{//run through all the child elements and set them up as SchemaTemplates
			
			//setting up schema we'll load into the directory
			SchemaTemplate schema;

			for(tinyxml2::XMLAttribute* attribute = (tinyxml2::XMLAttribute*) element->FirstAttribute();
				attribute !=NULL;
				attribute = (tinyxml2::XMLAttribute*) attribute->Next())
			{//Now we'll parse the attributes and load them into the SchemaTemplate
				if(StringUtil::stringEqualsIgnoreCase(attribute->Name(), "NAME"))
				{
					schema.setID(attribute->Value());
				}
				else if(StringUtil::stringEqualsIgnoreCase(attribute->Name(), "DEFINITION"))
				{
					std::vector<std::string> definitions = StringUtil::split(attribute->Value(), '.');

					for(std::vector<std::string>::const_iterator it = definitions.begin(); it != definitions.end(); ++it)
					{
						schema.addDefinition(it->c_str());
					}
				}
				else if(std::string(attribute->Name()).find("LEVEL-") == 0)
				{
					std::string level = std::string(attribute->Name()).substr(6, std::string(attribute->Name()).length()-6);
					int value;
					if(StringUtil::STR2NUM_SUCCESS == StringUtil::str2int(value, level.c_str()))
					{
						schema.setLevel(value);
						schema.setLevelName(attribute->Value());
					}
					else
					{
						oss.clear();
						oss.str("");
						oss << schema.getID() << " is referenced in directory but schema level LEVEL-" << level.c_str() << " can't be parsed";
						status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
						errorList.append(INDENT1).append(status.get()).append("\n");
					}
				}
				else if(StringUtil::stringEqualsIgnoreCase(attribute->Name(), "DESCRIPTION"))
				{
					schema.setDescription(attribute->Value());
				}
			}//done parsing attributes for the schema

			//now check to make sure all attributes are accounted for
			if(StringUtil::stringEquals(schema.getID(), ""))
			{
				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, "Directory references a schema with no name");
				errorList.append(INDENT1).append(status.get()).append("\n");
			}

			if(schema.isDefinitionEmpty())
			{
				oss.clear();
				oss.str("");
				oss << schema.getID() << " is referenced in directory but contains no definition";
				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
				errorList.append(INDENT1).append(status.get()).append("\n");
			}

			if(StringUtil::stringEquals(schema.getLevelName(), ""))
			{
				oss.clear();
				oss.str("");
				oss << schema.getID() << " is referenced in directory but contains an unnamed level";
				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
				errorList.append(INDENT1).append(status.get()).append("\n");
			}

			if ((schema.getLevel() == m_schemaLevel) && StringUtil::stringEqualsIgnoreCase(schema.getID(), "HEADER"))
			{
				m_lastHeaderName = std::string(schema.getLevelName()) + ".HEADER";
			}

			//now we load the schema into the directory
			m_directory.push_back(schema);

		}//done running through the child elements

		//the directory must have a HEADER entry for each schema level in the directory
		for(unsigned int i = 0; i <= (unsigned int)m_schemaLevel; ++i)
		{
			bool found = false;
			for(SchemaTemplateList::const_iterator it = m_directory.begin(); it != m_directory.end(); ++it)
			{
				std::string id = it->getID();
				if(StringUtil::stringEqualsIgnoreCase(it->getID(), "HEADER") && it->getLevel() == i)
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
				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
				errorList.append(INDENT1).append(status.get()).append("\n");
				break;
			}
		}

		return status;
	}//done parsing the directory file
	
	//Now we'll run through all of the child elements
	//and set them up as FieldTemplates
	for (tinyxml2::XMLElement* field = xmlSchema->FirstChildElement();
	     field != NULL;
	     field = field->NextSiblingElement())
	{
		StdUniquePtr<FieldTemplate> fieldTemplate(new FieldTemplate());

		//Now that we have an element, we'll run through all of the attributes
		//and save them in their appropriate strings
		for (tinyxml2::XMLAttribute* attribute = (tinyxml2::XMLAttribute*) field->FirstAttribute();
			 attribute != NULL;
			 attribute = (tinyxml2::XMLAttribute*) attribute->Next())
		{
			if (StringUtil::stringEqualsIgnoreCase(attribute->Name(), "NAME"))
			{
				fieldTemplate->setName(attribute->Value());
				fieldTemplate->setModifiedName(attribute->Value());
			}
			else if (StringUtil::stringEqualsIgnoreCase(attribute->Name(), "MODE"))
			{
				fieldTemplate->setMode(attribute->Value());

				if (StringUtil::stringEqualsIgnoreCase(fieldTemplate->getMode().c_str(), "CONTROL") && 
					StringUtil::stringEqualsIgnoreCase(fieldTemplate->getName().c_str(), "ARRAY-START"))
				{
					//encountered ARRAY-START, set array control flag
					arrayControlActive++;
				}
				else if (StringUtil::stringEqualsIgnoreCase(fieldTemplate->getMode().c_str(), "CONTROL") &&
						 StringUtil::stringEqualsIgnoreCase(fieldTemplate->getName().c_str(), "ARRAY-END"))
				{
					//encountered ARRAY-END, clear array control flag if it is set or throw error
					if (arrayControlActive > 0)
					{
						//clear flag
						arrayControlActive--;
					}
					else
					{
						//error, we found an ARRAY-END control before we found an ARRAY-START
						status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, "Unexpected ARRAY-END control before ARRAY-START control.");
						errorList.append(INDENT1).append(status.get()).append("\n");
					}
				}
			}
			else if (StringUtil::stringEqualsIgnoreCase(attribute->Name(), "FIELD_CLASS"))
			{
				fieldTemplate->setClass(attribute->Value());
			}
			else if (StringUtil::stringEqualsIgnoreCase(attribute->Name(), "TYPE"))
			{
				std::vector<std::string> list = StringUtil::split(attribute->Value(),',');
				std::list<std::string>   types;

				for (std::vector<std::string>::const_iterator it = list.begin(); it != list.end(); ++it)
				{
					types.push_back(StringUtil::trim(*it));
				}

				fieldTemplate->setTypes(types);
			}
			else if (StringUtil::stringEqualsIgnoreCase(attribute->Name(), "VALUE"))
			{
				const std::string& temp = attribute->Value();

				//list of values to be added to the field template
				std::list<std::string> formattedValues;

				if (!temp.empty() && StringUtil::stringEqualsIgnoreCase(fieldTemplate->getMode().c_str(), "CONTROL"))
				{
					//Control field value
					formattedValues.push_back(temp);
				}
				else if (!temp.empty())
				{
					//regular set of Values to validate against

					//split variable into list (~~ delimiter)
					std::vector<std::string> values = StringUtil::split(temp, "~~");

					for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
					{
						//trim each value, then check to make sure it is properly formatted
						std::string value = StringUtil::stripWhiteSpace(*it);

						if (value.find("..") != std::string::npos)
						{
							//finite range

							//split the range into its upper and lower bounds, then check the bounds
							if (StringUtil::split(value, "..").size() == 2)
							{
								formattedValues.push_back(value);
							}
							else
							{
								oss.clear();
								oss.str("");
								oss << schemaID << ": The field template " << fieldTemplate->getName() << " contains an invalid range \"" << value << "\"";
								errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
								status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
							}
						}
						else
						{
							formattedValues.push_back(value);
						}
					}
				}

				fieldTemplate->setValues(formattedValues);
			}
			else if (StringUtil::stringEqualsIgnoreCase(attribute->Name(), "DESCRIPTION"))
			{
				fieldTemplate->setDescription(attribute->Value());
			}
			else if (StringUtil::stringEqualsIgnoreCase(attribute->Name(), "SIZE"))
			{
				//ARRAY-START control fields have their own attribute called SIZE

				fieldTemplate->setSize(attribute->Value());
			}
			else
			{
				oss.clear();
				oss.str("");
				oss << schemaID << ": The field template " << fieldTemplate->getName() << " contains an unrecognized attribute " << attribute->Name();
				errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
			}
		}//done parsing the attributes

		if (fieldTemplate->getName().empty())
		{
			//throw an error if f_name wasn't parsed
			oss.clear();
			oss.str("");
			oss << schemaID << ": field template is missing NAME attribute";
			errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
			status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
		}
		
		if (fieldTemplate->getMode().empty() || 
			!(StringUtil::stringEqualsIgnoreCase(fieldTemplate->getMode().c_str(), "OPTIONAL") ||
			StringUtil::stringEqualsIgnoreCase(fieldTemplate->getMode().c_str(), "REQUIRED") ||
			StringUtil::stringEqualsIgnoreCase(fieldTemplate->getMode().c_str(), "CONTROL")))
		{
			//throw an error if f_mode wasn't parsed
			oss.clear();
			oss.str("");
			oss << schemaID << ": the field template " << fieldTemplate->getName() << " has invalid MODE attribute";
			errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
			status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
		}

		//check to make sure all attributes are accounted for
		if (!StringUtil::stringEqualsIgnoreCase(fieldTemplate->getMode().c_str(), "CONTROL"))
		{
			//checking attributes for regular field templates
			if (fieldTemplate->getClass().empty() ||
				!(StringUtil::stringEqualsIgnoreCase(fieldTemplate->getClass().c_str(), "HEADER") ||
				  StringUtil::stringEqualsIgnoreCase(fieldTemplate->getClass().c_str(), "STANDARD")))
			{
				oss.clear();
				oss.str("");
				oss << schemaID << ": the field template " << fieldTemplate->getName() << " has invalid FIELD_CLASS attribute";
				errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
			}
			
			//check to make sure all types are valid
			const std::list<std::string>& types = fieldTemplate->getTypeStrings();

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
							gmsec::api::internal::InternalField::lookupType((*it).c_str());
						}
						catch (...) 
						{
							oss.clear();
							oss.str("");
							oss << schemaID << ": the field template " << fieldTemplate->getName() << " contains unrecognized TYPE \"" << (*it).c_str() << "\"";
							errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
							status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
						}
					}
				}
			}
			else
			{
				oss.clear();
				oss.str("");
				oss << schemaID << ": the field template " << fieldTemplate->getName() << " is missing TYPE";
				errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
			}

			//check types against the values to make sure they work
			const std::list<std::string>& values = fieldTemplate->getValues();

			for (std::list<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
			{
				if (!fieldTemplate->isTypeVariable())
				{
					const std::string& value = *it;

					//type is not variable, so check to make sure the values are valid for the given types(s)

					if (StringUtil::stringEquals(&value.at(value.length() - 1), "+"))
					{
						//lower bound

						if (!checkValue(fieldTemplate->getTypes(), value.substr(0, value.length() - 1)))
						{
							oss.clear();
							oss.str("");
							oss << schemaID << ": The field template " << fieldTemplate->getName() << " contains an invalid value \"" << value << "\"";
							errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
							status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
						}
					}
					else if (StringUtil::stringEquals(&value.at(value.length() - 1), "-"))
					{
						//Upper bound

						if (!checkValue(fieldTemplate->getTypes(), value.substr(0, value.length() - 1)))
						{
							oss.clear();
							oss.str("");
							oss << schemaID << ": The field template " << fieldTemplate->getName() << " contains an invalid value \"" << value << "\"";
							errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
							status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
						}
					}
					else if (value.find("..") != std::string::npos)
					{
						//finite range

						//split the range into its upper and lower bounds, then check the bounds
						std::vector<std::string> range = StringUtil::split(value, "..");

						//range should have exactly 2 elements, an upper and lower bound
						if (range.size() != 2)
						{
							oss.clear();
							oss.str("");
							oss << schemaID << ": The field template " << fieldTemplate->getName() << " contains an invalid range \"" << value << "\"";
							errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
							status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
						}
						else if (!(checkValue(fieldTemplate->getTypes(), range.at(0)) && checkValue(fieldTemplate->getTypes(), range.at(1))))
						{
							oss.clear();
							oss.str("");
							oss << schemaID << ": The field template " << fieldTemplate->getName() << " contains an invalid range \"" << value << "\"";
							errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
							status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
						}
					}
					else
					{
						//absolute value

						if (!checkValue(fieldTemplate->getTypes(), value))
						{
							oss.clear();
							oss.str("");
							oss << schemaID << ": The field template " << fieldTemplate->getName() << " contains an invalid value \"" << value << "\"";
							errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
							status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
						}
					}
				}
			}
		}//done checking attributes for regular fields

		if (StringUtil::stringEqualsIgnoreCase(fieldTemplate->getMode().c_str(), "CONTROL"))
		{
			//checking control fields to make sure all attributes are present
			if (StringUtil::stringEqualsIgnoreCase(fieldTemplate->getName().c_str(), "ARRAY-START"))
			{
				if (fieldTemplate->getArrayControlValue().empty())
				{
					oss.clear();
					oss.str("");
					oss << schemaID << ": the control field " << fieldTemplate->getName() << " is missing VALUE attribute";
					errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
					status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
				}
				if (fieldTemplate->getSize().empty())
				{
					oss.clear();
					oss.str("");
					oss << schemaID << ": the control field " << fieldTemplate->getName() << " is missing SIZE attribute";
					errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
					status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
				}
			}
		}//done checking CONTROL fields

		//And finally, load the FieldTemplate into schemaFields
		schemaFields.push_back(fieldTemplate.release());

	}//finished running through all child elements
	
	//If this flag is still set at this point then it means we have an open-ended array control
	if (arrayControlActive > 0)
	{
		oss.clear();
		oss.str("");
		oss << schemaID << ":  Expected ARRAY-END control not found";
		errorList.append(INDENT1).append(oss.str().c_str()).append("\n");
		status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
	}

	if (!errorList.empty() && errorList[errorList.length()-1] == '\n')
	{
		errorList.erase(errorList.length() - 1);
		status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, errorList.c_str());
	}

	return status;
}


bool InternalSpecification::checkValue(const std::list<Field::FieldType>& types, const std::string& value) const
{
	bool valid = true;

	for (std::list<Field::FieldType>::const_iterator it = types.begin(); it != types.end(); ++it)
	{
		Field::FieldType type = *it;

		switch (type)
		{
		case Field::CHAR_TYPE:
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
		case Field::BOOL_TYPE:
		{//valid values: 0, 1, true, false
			try
			{
				GMSEC_U32 testValue = (GMSEC_U32) StringUtil::getValue<GMSEC_U32>(value.c_str());
				if (!(testValue == 0 || 
					  testValue == 1 || 
					  StringUtil::stringEqualsIgnoreCase(value.c_str(), "true") || 
					  StringUtil::stringEqualsIgnoreCase(value.c_str(), "false")))
				{
					//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_BOOL";
					valid = false;
				}
			}
			catch (...)
			{
				//GMSEC_ERROR << "XML Parse Error: " << value.c_str() << " cannot be converted to GMSEC_BOOL";
				valid = false;
			}

			break;
		}
		case Field::I16_TYPE:
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
		case Field::U16_TYPE:
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
		case Field::I32_TYPE:
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
		case Field::U32_TYPE:
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
		case Field::F32_TYPE:
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
		case Field::F64_TYPE:
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
		case Field::STRING_TYPE:
			//no checks needed, anything can be a string
			break;
		case Field::BIN_TYPE:
		{//BIN_TYPE field is used for data, and should not be predefining values
			//GMSEC_ERROR << "XML Parse Error: unable to evaluate BIN_TYPE values";
			valid = false;
			break;
		}
		case Field::I8_TYPE:
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
		case Field::U8_TYPE:
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
		case Field::I64_TYPE:
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
		case Field::U64_TYPE:
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
		SchemaTemplate&    temp = *it;
		const std::string& def  = temp.getID();

		if (def.find(id) != std::string::npos)
		{
			//found it

			//the common headers only contain the first two definitions, so we only need the first two
			typeDef    = temp.getFirstDefinition();
			subtypeDef = temp.getNextDefinition();
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
			SchemaTemplate& temp = *it;

			if (StringUtil::stringEquals(temp.getID(), genID.c_str()))
			{
				typeDef    = temp.getFirstDefinition();
				subtypeDef = temp.getNextDefinition();
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
			SchemaTemplate& temp = *it;

			if (StringUtil::stringEquals(temp.getID(), "HEADER"))
			{
				typeDef    = temp.getFirstDefinition();
				subtypeDef = temp.getNextDefinition();
				break;
			}
		}
	}

	// Look up the Field Template List that is associated with the Schema ID; if we cannot
	// find the list, then default to using the Field Template List that is presumably
	// associated with the highest schema level (i.e. the DEFAULT list).
	//
	std::string headerID = (elements.size() >= 5 ? elements.at(2) + ".HEADER" : "DEFAULT");

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
		return temp->getID();
	}

	throw Exception(MIST_ERROR, INDEX_OUT_OF_RANGE, "next() has reached end of list");
}


bool InternalSpecification::hasNextID() const
{
	return m_schemaIndex != m_templates.end();
}


SchemaIDIterator& InternalSpecification::getSchemaIDIterator()
{
	m_iterator.m_iter->reset();
	return m_iterator;
}


std::string InternalSpecification::getTypeStr(Field::FieldType type)
{
	try {
		return gmsec::api::internal::InternalField::lookupTypeStr(type);
	}
	catch (...) {
		return "UNDEFINED";
	}
}


std::string InternalSpecification::getValue(const Message& msg, const char* name) const
{
	std::ostringstream value;

	Field::FieldType type = msg.getFieldType(name);

	//determine what type of value the field has, then grab the value
	switch (type)
	{
	case Field::BIN_TYPE:
		value << msg.getBinaryField(name).getValue();
		break;

	case Field::BOOL_TYPE:
		value << msg.getBooleanField(name).getValue();
		break;

	case Field::CHAR_TYPE:
		value << msg.getCharField(name).getValue();
		break;

	case Field::F32_TYPE:
		value << msg.getF32Field(name).getValue();
		if(value.str().find(".") == std::string::npos)
		{
			value << ".0";
		}
		break;

	case Field::F64_TYPE:
		value << msg.getF64Field(name).getValue();
		if(value.str().find(".") == std::string::npos)
		{
			value << ".0";
		}
		break;

	case Field::I16_TYPE:
		value << msg.getI16Field(name).getValue();
		break;

	case Field::I32_TYPE:
		value << msg.getI32Field(name).getValue();
		break;

	case Field::I64_TYPE:
		value << msg.getI64Field(name).getValue();
		break;

	case Field::I8_TYPE:
		value << msg.getI8Field(name).getValue();
		break;

	case Field::STRING_TYPE:
		value << msg.getStringField(name).getValue();
		break;

	case Field::U16_TYPE:
		value << msg.getU16Field(name).getValue();
		break;

	case Field::U32_TYPE:
		value << msg.getU32Field(name).getValue();
		break;

	case Field::U64_TYPE:
		value << msg.getU64Field(name).getValue();
		break;

	case Field::U8_TYPE:
		value << msg.getU8Field(name).getValue();
		break;

	default:
		{//since we can conceivably turn anything into a string, we shouldn't be here
			std::stringstream err;
			err << "Unable to determine FieldType " << type << " for field " << name;
			GMSEC_ERROR << err.str().c_str();
			throw Exception(MIST_ERROR, INCORRECT_FIELD_TYPE, err.str().c_str());
		}
	}
	
	return value.str();
}


unsigned int InternalSpecification::getVersion() const
{
	return m_specVersion;
}


Specification::SchemaLevel InternalSpecification::getSchemaLevel() const
{
	return m_schemaLevel;
}


void InternalSpecification::setVersion(unsigned int version)
{
	std::ostringstream oss;
	oss << version;
	m_specVersionStr = oss.str();

	//the templates need to be reloaded since the spec has been changed
	cleanup();

	try {
		load();
	}
	catch (const Exception& e) {
		cleanup();
		throw e;
	}
}


MessageTemplate& InternalSpecification::findTemplate(const char* schemaID)
{
	if (!schemaID || std::string(schemaID).empty())
	{
		throw Exception(MIST_ERROR, TEMPLATE_ID_DOES_NOT_EXIST, "Schema ID cannot be null or contain an empty string");
	}

	MessageTemplates::const_iterator it = m_templates.find(schemaID);

	if (it != m_templates.end())
	{
		return *(it->second);
	}

	//A message template was not returned, which means there wasn't a matching one in the list
	std::string err = "SchemaID \"";
	err.append(schemaID);
	err.append("\" could not be found in list of available schema.");
	throw Exception(MIST_ERROR, TEMPLATE_ID_DOES_NOT_EXIST, err.c_str());
}


const char* InternalSpecification::getTemplateXML(const char* subject, const char* schemaID)
{
	m_templateXML = findTemplate(schemaID).toXML(subject);
	return m_templateXML.c_str();
}


const InternalSpecification::SchemaTemplateList& InternalSpecification::getDirectory() const
{
	return m_directory;
}


const util::DataList<MessageSpecification*>& InternalSpecification::getMessageSpecifications() const
{
	return m_msgSpecs;
}


void InternalSpecification::registerMessageValidator(MessageValidator* validator)
{
	if (!validator)
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "The MessageValidator cannot be null.");
	}

	m_customMessageValidator = validator;
}


void InternalSpecification::registerMessageValidator(GMSEC_MessageValidator* validator)
{
	if (validator == NULL || *validator == NULL)
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "MessageValidator cannot be null.");
	}

	delete m_ceeMessageValidator;

	m_ceeMessageValidator = new CustomMessageValidator(validator);

	registerMessageValidator(m_ceeMessageValidator);
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


bool InternalSpecification::isReservedField(const char* fieldName)
{
	bool reserved = false;

#if GMSEC_CXX_11_AVAILABLE == 1

	static const char* reservedFields[] =
	{
		"CONNECTION-ID",
		"MW-INFO",
		"NODE",
		"PROCESS-ID",
		"PUBLISH-TIME",
		"UNIQUE-ID",
		"USER-NAME",
		"NUM-OF-SUBSCRIPTIONS",
		"SUBSCRIPTION.(.*).SUBJECT-PATTERN",
		"CONNECTION-ENDPOINT",
		"MW-CONNECTION-ENDPOINT",
		NULL
	};

	for (size_t i = 0; !reserved && reservedFields[i]; ++i)
	{
		reserved = std::regex_match(fieldName, std::regex(reservedFields[i]));
	}

#else

	//TODO: Remove once support for CentOS 6 is dropped
	static const char* reservedFields[] =
	{
		"CONNECTION-ID",
		"MW-INFO",
		"NODE",
		"PROCESS-ID",
		"PUBLISH-TIME",
		"UNIQUE-ID",
		"USER-NAME",
		"NUM-OF-SUBSCRIPTIONS",
		"SUBSCRIPTION.%d.SUBJECT-PATTERN",
		"CONNECTION-ENDPOINT",
		"MW-CONNECTION-ENDPOINT",
		NULL
	};

	for (size_t i = 0; !reserved && reservedFields[i]; ++i)
	{
		if ((std::string(fieldName).find("SUBSCRIPTION") != std::string::npos) && (std::string(fieldName).find("SUBJECT-PATTERN") != std::string::npos))
		{
			reserved = true;
		}
		else
		{
			reserved = (std::string(fieldName) == reservedFields[i]);
		}
	}

#endif

	return reserved;
}


Status InternalSpecification::checkTrackingField(const char* fieldName, const TrackingDetails& configTracking, const TrackingDetails& msgTracking)
{
	Status status;

	const int ON    = MESSAGE_TRACKINGFIELDS_ON;
	const int OFF   = MESSAGE_TRACKINGFIELDS_OFF;
	const int UNSET = MESSAGE_TRACKINGFIELDS_UNSET;

	bool addTracking = (configTracking.get() == ON && (msgTracking.get() == ON || msgTracking.get() == UNSET));
	bool haveError   = false;

	if (StringUtil::stringEquals(fieldName, "CONNECTION-ID"))
	{
		if ((addTracking || configTracking.getConnectionId() == ON || msgTracking.getConnectionId() == ON) &&
		    (configTracking.getConnectionId() != OFF && msgTracking.getConnectionId() != OFF))
		{
			haveError = true;
		}
	}
	else if (StringUtil::stringEquals(fieldName, "MW-INFO"))
	{
		if ((addTracking || configTracking.getMwInfo() == ON || msgTracking.getMwInfo() == ON) &&
		    (configTracking.getMwInfo() != OFF && msgTracking.getMwInfo() != OFF))
		{
			haveError = true;
		}
	}
	else if (StringUtil::stringEquals(fieldName, "NODE"))
	{
		if ((addTracking || configTracking.getNode() == ON || msgTracking.getNode() == ON) &&
		    (configTracking.getNode() != OFF && msgTracking.getNode() != OFF))
		{
			haveError = true;
		}
	}
	else if (StringUtil::stringEquals(fieldName, "PROCESS-ID"))
	{
		if ((addTracking || configTracking.getProcessId() == ON || msgTracking.getProcessId() == ON) &&
		    (configTracking.getProcessId() != OFF && msgTracking.getProcessId() != OFF))
		{
			haveError = true;
		}
	}
	else if (StringUtil::stringEquals(fieldName, "PUBLISH-TIME"))
	{
		if ((addTracking || configTracking.getPublishTime() == ON || msgTracking.getPublishTime() == ON) &&
		    (configTracking.getPublishTime() != OFF && msgTracking.getPublishTime() != OFF))
		{
			haveError = true;
		}
	}
	else if (StringUtil::stringEquals(fieldName, "UNIQUE-ID"))
	{
		if ((addTracking || configTracking.getUniqueId() == ON || msgTracking.getUniqueId() == ON) &&
		    (configTracking.getUniqueId() != OFF && msgTracking.getUniqueId() != OFF))
		{
			haveError = true;
		}
	}
	else if (StringUtil::stringEquals(fieldName, "USER-NAME"))
	{
		if ((addTracking || configTracking.getUserName() == ON || msgTracking.getUserName() == ON) &&
		    (configTracking.getUserName() != OFF && msgTracking.getUserName() != OFF))
		{
			haveError = true;
		}
	}
	else if (StringUtil::stringEquals(fieldName, "NUM-OF-SUBSCRIPTIONS"))
	{
		if ((addTracking || configTracking.getActiveSubscriptions() == ON || msgTracking.getActiveSubscriptions() == ON) &&
		    (configTracking.getActiveSubscriptions() != OFF && msgTracking.getActiveSubscriptions() != OFF))
		{
			haveError = true;
		}
	}
	else if (std::string(fieldName).find("SUBSCRIPTION.") != std::string::npos && std::string(fieldName).find(".SUBJECT-PATTERN") != std::string::npos)
	{
		if ((addTracking || configTracking.getActiveSubscriptions() == ON || msgTracking.getActiveSubscriptions() == ON) &&
		    (configTracking.getActiveSubscriptions() != OFF && msgTracking.getActiveSubscriptions() != OFF))
		{
			int val;
			int tmp = sscanf(fieldName, "SUBSCRIPTION.%d.SUBJECT-PATTERN", &val);

			haveError = (tmp == 1);
		}
	}
	else if (StringUtil::stringEquals(fieldName, "CONNECTION-ENDPOINT") || StringUtil::stringEquals(fieldName, "MW-CONNECTION-ENDPOINT"))
	{
		if ((addTracking || configTracking.getConnectionEndpoint() == ON || msgTracking.getConnectionEndpoint() == ON) &&
		    (configTracking.getConnectionEndpoint() != OFF && msgTracking.getConnectionEndpoint() != OFF))
		{
			haveError = true;
		}
	}

	if (haveError)
	{
		std::stringstream err;
		err << fieldName << " is a reserved tracking field for the GMSEC API";
		status.set(MIST_ERROR, NON_ALLOWED_FIELD, err.str().c_str());
	}

	return status;
}
