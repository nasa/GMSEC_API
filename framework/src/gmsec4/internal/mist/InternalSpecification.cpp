/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalSpecification.cpp
 *
 *  @brief This file contains the InternalSpecification class.
 */

#include <gmsec4/internal/mist/InternalSpecification.h>

#include <gmsec4/internal/mist/AbstractFieldDescriptor.h>
#include <gmsec4/internal/mist/VariableFieldDescriptor.h>
#include <gmsec4/internal/mist/InternalContext.h>
#include <gmsec4/internal/mist/DefaultMessageSchemas.h>
#include <gmsec4/internal/mist/MessageTemplate.h>
#include <gmsec4/internal/mist/SimpleFieldDescriptor.h>
#include <gmsec4/internal/mist/InternalSimpleFieldDescriptor.h>
#include <gmsec4/internal/mist/CompoundFieldDescriptor.h>
#include <gmsec4/internal/mist/InternalCompoundFieldDescriptor.h>

#include <gmsec4/internal/FileUtil.h>
#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/Errors.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>
#include <gmsec4/Message.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/Log.h>

#include <tinyxml2.h>

#include <cstdlib>

#ifdef __sun
	#include <string.h>
#else
	#include <cstring>
#endif


using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


InternalSpecification::InternalSpecification()
{
}


InternalSpecification::~InternalSpecification()
{
	for (Templates::iterator it = m_templates.begin(); it != m_templates.end(); ++it)
	{
		delete it->second;
	}
	m_templates.clear();

	for (HeapList::iterator it = m_heapList.begin(); it != m_heapList.end(); ++it)
	{
		StringUtil::stringDestroy((const char*)(*it));
	}
	m_heapList.clear();
}


Status InternalSpecification::load(const char* basePath, unsigned int version)
{
	Status                 status;
	std::string            load_dir;
	std::list<std::string> schema_files;

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

	//
	// Get major, minor version number strings
	//
	StringUtil::stringFormat(major_version_str, sizeof(major_version_str), "%04u", (unsigned int)version/100);
	StringUtil::stringFormat(minor_version_str, sizeof(minor_version_str), "%02u", (unsigned int)version-(((unsigned int)version/100)*100));
	m_specificationId = major_version_str;
	m_specificationId.append(".");
	m_specificationId.append(minor_version_str);

	//
	// Load paths
	//
	if (basePath == NULL)
	{
		std::string shared_object_path;

		if (FileUtil::getCurrentSharedObjectPath(shared_object_path) == false)
		{
			status = loadDefaultTemplatesFromMemory(version);

			if(status.isError()){
				return status;
			}

			std::string err_msg = "Could not get system-specified base path.";

			status.set(MIST_ERROR, TEMPLATE_DIR_NOT_FOUND, err_msg.c_str());

			GMSEC_WARNING << err_msg.c_str();
		}
		else
		{
			size_t dirMarker = shared_object_path.rfind(FileUtil::PATH_SEPARATOR);
	
			if (dirMarker != std::string::npos)
			{
				load_dir = shared_object_path.substr(0, dirMarker) + FileUtil::PATH_SEPARATOR + ".." + FileUtil::PATH_SEPARATOR + "templates";
			}
			else
			{
				status = loadDefaultTemplatesFromMemory(version);

				if(status.isError()){
					return status;
				}
	
				std::string err_msg = "Could not list files in template directory ";
				err_msg += load_dir;

				status.set(MIST_ERROR, TEMPLATE_DIR_NOT_FOUND, err_msg.c_str());
	
				GMSEC_WARNING << err_msg.c_str();
			}
		}
	}
	else
	{
		load_dir = basePath;
	}

	if (!status.isError())
	{
		load_dir += FileUtil::PATH_SEPARATOR;
		load_dir += major_version_str;
		load_dir += ".";
		load_dir += minor_version_str;

		if (FileUtil::getFilesInDirectory(load_dir, schema_files) == false)
		{
			status = loadDefaultTemplatesFromMemory(version);

			if(status.isError()){
				return status;
			}

			std::string err_msg = "Could not list files in template directory ";
			err_msg += load_dir;

			status.set(MIST_ERROR, TEMPLATE_DIR_ERROR, err_msg.c_str());

			GMSEC_WARNING << err_msg.c_str();

		}
		else
		{
			for (std::list<std::string>::iterator it = schema_files.begin(); it != schema_files.end(); ++it)
			{
				std::string filename = *it;

				if (filename != ".." && filename != ".")
				{
					std::string path = load_dir;

					path += FileUtil::PATH_SEPARATOR;
					path += filename;

					status = loadTemplate(filename.c_str(), path.c_str());
				}

				if (status.isError())
				{
					GMSEC_WARNING << "Error loading template file " << filename.c_str();

					status = loadDefaultTemplatesFromMemory(version);

					if(status.isError()){
						return status;
					}
				}
			}
		}
	}

	return status;
}


// path will hold XML message definitions of the form above
Status InternalSpecification::loadTemplate(const char* id, const char* path)
{
	Status status;

	tinyxml2::XMLDocument xml_doc;

	if (xml_doc.LoadFile(path) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement* xml_schema = xml_doc.RootElement();

		if (xml_schema != NULL)
		{
			std::string id_string;
			std::string id_field_names;
			std::vector<AbstractFieldDescriptor*> schema_fields;

			status = parser(xml_schema, id_string, id_field_names, schema_fields);

			if (!status.isError())
			{
				m_idFieldNames.insert(std::pair<std::string, std::string>(id_string.c_str(), id_field_names.c_str()));

				// Note: registerTemplate() takes ownership of the allocated MesssageTemplate object.
				status = registerTemplate(id_string.c_str(), new MessageTemplate(schema_fields, true));
			}

			xml_doc.Clear();
		}
		else
		{
			std::string err_msg = "xml_schema NULL";

			GMSEC_WARNING << "InternalSpecification::LoadTemplate() : " << err_msg.c_str();

			status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err_msg.c_str());
		}
	}
	else
	{
		std::string err_msg = "Unparseable XML string given";

		GMSEC_WARNING << "InternalSpecification::LoadTemplate() : " << err_msg.c_str();

		status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err_msg.c_str());
	}

	return status;
}


// Update message (already created) according to template <id>
Status InternalSpecification::apply(Message& message, Context& context) const
{
	Status status;

	const char* id = context.getTemplateId();

	Templates::const_iterator it = m_templates.find(id);

	if (it == m_templates.end())
	{
		status.set(MIST_ERROR, TEMPLATE_ID_DOES_NOT_EXIST, "Unregistered template id requested");
		return status;
	}

	MessageTemplate* msgTemplate = it->second;

	return msgTemplate->apply(message, context);
}


Status InternalSpecification::instantiate(Message& message, Context& context) const
{
	Status status;

	if (message.getFieldCount() != 0)
	{
		status.set(MIST_ERROR, REQUIRED_EMPTY_MESSAGE, "Empty message required for instantiate");
		return status;
	}

	const char* id = context.getTemplateId();

	Templates::const_iterator it = m_templates.find(id);

	if (it == m_templates.end())
	{
		status.set(MIST_ERROR, TEMPLATE_ID_DOES_NOT_EXIST, "Unregistered template id requested");
		return status;
	}

	MessageTemplate* msgTemplate = it->second;

	return msgTemplate->instantiate(message, context);
}


// validate message against template
Status InternalSpecification::validate(Message& message, Context& context) const
{
	Status status;

	const char* id = context.getTemplateId();

	Templates::const_iterator it = m_templates.find(id);

	if (it == m_templates.end())
	{
		status.set(MIST_ERROR, TEMPLATE_ID_DOES_NOT_EXIST, "Unregistered template id requested");
		return status;
	}

	MessageTemplate* msgTemplate = it->second;

	return msgTemplate->validate(message, context);
}


// programmatic definition
Status InternalSpecification::registerTemplate(const char* id, MessageTemplate* newTemplate)
{
	Status status;

	if (!id)
	{
		status.set(MIST_ERROR, UNINITIALIZED_OBJECT, "Template ID is invalid.");
		return status;
	}

	std::string newId = std::string(id);
	m_templates.insert(std::pair<std::string, MessageTemplate*>(newId, newTemplate));

	return status;
}


// Fetches list of template IDs
std::list<std::string> InternalSpecification::getTemplateIds() const
{
	std::list<std::string> ids;

	for (Templates::const_iterator it = m_templates.begin(); it != m_templates.end(); ++it)
	{
		ids.push_back(it->first);
	}

	return ids;
}


// Checks if a given message matches a template in this InternalSpecification
std::string InternalSpecification::lookupTemplate(const Message& message)
{
	std::string templateID;

	//
	// This function will search for whether a message type, and  message
	// subtypes (there *can* be more than one subtype), match
	// an id key in the templates list for a template spec.
	//

	std::string msgID;
	std::string schemaID;
	bool        templateFound = false;

	FieldNames::iterator fieldNamesIter = m_idFieldNames.begin();
	Templates::iterator  templatesIter  = m_templates.begin();

	while (fieldNamesIter != m_idFieldNames.end() && templatesIter != m_templates.end() && !templateFound)
	{
		msgID = "";

		std::vector<std::string> idFieldNames = StringUtil::split(fieldNamesIter->second, '.');

		for (std::vector<std::string>::iterator idIter = idFieldNames.begin(); idIter != idFieldNames.end(); ++idIter)
		{
			const std::string fieldName = *idIter;

			const Field* field = message.getField(fieldName.c_str());

			if (field)
			{
				char               msgValueBuffer[128];
				const char*        msgValue    = msgValueBuffer;
				// In the usage below, most often msgValue will just be
				// a destination buffer for gmsec::util::stringFormat() which
				// is a safe sprintf() function that works across different
				// platforms basically.  There is one case where msgValue
				// is actually allocated by StringField::getValue() (when the
				// relevant field is a GMSEC_STR field); in that case the
				// msgValue char* gets re-assigned a pointer to memory
				// allocated by Field itself, and apparently there is no need
				// to de-allocate that by the user; the de-allocation is
				// handled by the Field destructor.

				const I8Field*     i8Field     = NULL;
				const I16Field*    i16Field    = NULL;
				const I32Field*    i32Field    = NULL;
				const I64Field*    i64Field    = NULL;
				const U8Field*     u8Field     = NULL;
				const U16Field*    u16Field    = NULL;
				const U32Field*    u32Field    = NULL;
				const U64Field*    u64Field    = NULL;
				const StringField* strField    = NULL;

				bool               validField  = false;

				switch (field->getType())
				{
				  case Field::I8_TYPE:
					i8Field = dynamic_cast<const I8Field*>(field);
					if (i8Field)
					{
						StringUtil::stringFormat(msgValueBuffer, sizeof(msgValueBuffer), "%hhd", i8Field->getValue());
						validField = true;
					}
					break;

				  case Field::I16_TYPE:
					i16Field = dynamic_cast<const I16Field*>(field);
					if (i16Field)
					{
						StringUtil::stringFormat(msgValueBuffer, sizeof(msgValueBuffer), "%hd", i16Field->getValue());
						validField = true;
					}
					break;

				  case Field::I32_TYPE:
					i32Field = dynamic_cast<const I32Field*>(field);
					if (i32Field)
					{
						StringUtil::stringFormat(msgValueBuffer, sizeof(msgValueBuffer), "%ld", i32Field->getValue());
						validField = true;
					}
					break;

				  case Field::I64_TYPE:
					i64Field = dynamic_cast<const I64Field*>(field);
					if (i64Field)
					{
						StringUtil::stringFormat(msgValueBuffer, sizeof(msgValueBuffer), "%lld", i64Field->getValue());
						validField = true;
					}
					break;

				  case Field::U8_TYPE:
					u8Field = dynamic_cast<const U8Field*>(field);
					if (u8Field)
					{
						StringUtil::stringFormat(msgValueBuffer, sizeof(msgValueBuffer), "%hhu", u8Field->getValue());
						validField = true;
					}
					break;

				  case Field::U16_TYPE:
					u16Field = dynamic_cast<const U16Field*>(field);
					if (u16Field)
					{
						StringUtil::stringFormat(msgValueBuffer, sizeof(msgValueBuffer), "%hu", u16Field->getValue());
						validField = true;
					}
					break;

				  case Field::U32_TYPE:
					u32Field = dynamic_cast<const U32Field*>(field);
					if (u32Field)
					{
						StringUtil::stringFormat(msgValueBuffer, sizeof(msgValueBuffer), "%lu", u32Field->getValue());
						validField = true;
					}
					break;

				  case Field::U64_TYPE:
					u64Field = dynamic_cast<const U64Field*>(field);
					if (u64Field)
					{
						StringUtil::stringFormat(msgValueBuffer, sizeof(msgValueBuffer), "%llu", u64Field->getValue());
						validField = true;
					}
					break;

				  case Field::STRING_TYPE:
					strField = dynamic_cast<const StringField*>(field);
					if (strField)
					{
						msgValue = strField->getValue();
						validField = true;
					}
					break;

				  default:
					break;

				}

				if (validField)
				{
					msgID += ".";
					msgID += msgValue;
				}
				else
				{
					std::string error("Unexpected field type for ");
					error.append(field->getName());
					GMSEC_WARNING << error.c_str();
					throw Exception(MIST_ERROR, INCORRECT_FIELD_TYPE, error.c_str());
				}

			}
		}

		schemaID = m_specificationId;
		schemaID.append(msgID);

		if (StringUtil::stringEqualsIgnoreCase(templatesIter->first.c_str(), schemaID.c_str()))
		{
			templateFound = true;
		}

		++fieldNamesIter;
		++templatesIter;
	}

	if (templateFound)
	{
		templateID = schemaID.c_str();
	}
	else
	{
		throw Exception(MIST_ERROR, TEMPLATE_ID_DOES_NOT_EXIST,
			"The message passed into lookupTemplate() does not match any loaded message template.");
	}

	return templateID;
}


// Translate the XML specification in a TinyXML (which is a 3rd-party package)
// node into an ID string and list of MIST FieldDescriptors, which is
// intended to be inserted into a MIST MessageTemplate.
Status InternalSpecification::parser(tinyxml2::XMLElement* xmlSchema,
                                     std::string& id,
                                     std::string& idFieldNames,
                                     std::vector<AbstractFieldDescriptor*>& schemaFields)
{
	Status status;

	const tinyxml2::XMLAttribute* xml_schema_attribute = xmlSchema->FirstAttribute();

	if (xml_schema_attribute != NULL)
	{
		if (StringUtil::stringEqualsIgnoreCase(xml_schema_attribute->Name(), "NAME"))
		{
			id = StringUtil::stringToUpper(xml_schema_attribute->Value());
		}
		
		xml_schema_attribute = xml_schema_attribute->Next();

		if (xml_schema_attribute != NULL)
		{
			if (StringUtil::stringEqualsIgnoreCase(xml_schema_attribute->Name(), "ID_FIELD_NAMES"))
			{
				idFieldNames = StringUtil::stringToUpper(xml_schema_attribute->Value());
			}
		}
	}

	if (id == "" || idFieldNames == "")
	{
		status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, "The schema read did not have a template id NAME or ID_FIELD_NAMES specified.");
	}

	std::string field_name;
	std::string field_value;
	FieldMode   field_mode;
	FieldClass  field_class;
	std::string field_desc;
	AbstractFieldDescriptor* field_ptr = 0;

	//
	// Loop adding all fields to fields list parameter
	//
	for (tinyxml2::XMLElement* field = xmlSchema->FirstChildElement();
             field != NULL && !status.isError();
             field = field->NextSiblingElement())
	{
		field_ptr = NULL;

		for (tinyxml2::XMLAttribute* field_attrib = (tinyxml2::XMLAttribute*) field->FirstAttribute();
		     field_attrib != NULL;
			 field_attrib = (tinyxml2::XMLAttribute*) field_attrib->Next())
		{
			//
			// NAME="..." field attribute
			//
			if (StringUtil::stringEqualsIgnoreCase(field_attrib->Name(), "NAME"))
			{
				field_name = StringUtil::stringToUpper(field_attrib->Value());
			}
			//
			// MODE="..." field attribute
			//
			else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Name(), "MODE"))
			{
				if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "REQUIRED"))
				{
					field_mode = F_REQUIRED;
				}
				else
				{
					field_mode = F_OPTIONAL;
				}
			}
			//
			// FIELD_CLASS="..." field attribute
			//
			else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Name(), "FIELD_CLASS"))
			{
				if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "STANDARD"))
				{
					field_class = F_STANDARD;
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "HEADER"))
				{
					field_class = F_HEADER;
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "USER"))
				{
					field_class = F_USER;
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "API"))
				{
					field_class = F_API;
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "VARIABLE"))
				{
					field_class = F_VARIABLE;
				}
				else
				{
					field_class = F_NONE;
				}
			}
			//
			// VALUE="..." field attribute
			//
			else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Name(), "VALUE"))
			{
				field_value = StringUtil::stringToUpper(field_attrib->Value());
			}
			//
			// TYPE="..." field attribute
			//
			else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Name(), "TYPE"))
			{
				if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "CHAR"))
				{
					//
					// Question:  What does the fvalue
					// ultimately denote anyway in a 
					// SimpleFieldDescriptor?
					//

					GMSEC_CHAR ch = field_value.c_str()[0];

					field_ptr = new SimpleFieldDescriptor<GMSEC_CHAR>(ch);
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "BOOL") ||
				         StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "BOOLEAN"))
				{
					GMSEC_BOOL init_bool_val;

					if (field_value == "TRUE")
					{
						init_bool_val = GMSEC_TRUE;
					}
					else
					{
						init_bool_val = GMSEC_FALSE;
					}

					field_ptr = new SimpleFieldDescriptor<GMSEC_BOOL>(init_bool_val);
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "I16") ||
				         StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "SHORT"))
				{
					GMSEC_I16 value = (GMSEC_I16) std::atoi(field_value.c_str());
					field_ptr = new SimpleFieldDescriptor<GMSEC_I16>(value);
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "U16") ||
				         StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "USHORT"))
				{
					GMSEC_U16 value = (GMSEC_U16) std::atoi(field_value.c_str());
					field_ptr = new SimpleFieldDescriptor<GMSEC_U16>(value);
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "I32") ||
				         StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "INT"))
				{
					GMSEC_I32 value = (GMSEC_I32) std::atol(field_value.c_str());
					field_ptr = new SimpleFieldDescriptor<GMSEC_I32>(value);
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "U32") ||
				         StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "UINT"))
				{
					GMSEC_U32 value = (GMSEC_U32) std::atol(field_value.c_str());
					field_ptr = new SimpleFieldDescriptor<GMSEC_U32>(value);
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "I64") ||
				         StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "LONG"))
				{
					GMSEC_I64 value = (GMSEC_I64) std::atol(field_value.c_str());
					field_ptr = new SimpleFieldDescriptor<GMSEC_I64>(value);
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "U64") ||
				         StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "ULONG"))
				{
					GMSEC_U64 value = (GMSEC_U32) std::atol(field_value.c_str());
					field_ptr = new SimpleFieldDescriptor<GMSEC_U64>(value);
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "F32") ||
				         StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "FLOAT"))
				{
					GMSEC_F32 value = (GMSEC_F32) std::atof(field_value.c_str());
					field_ptr = new SimpleFieldDescriptor<GMSEC_F32>(value);
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "F64") ||
				         StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "DOUBLE"))
				{
					GMSEC_F64 value = (GMSEC_F64) std::atof(field_value.c_str());
					field_ptr = new SimpleFieldDescriptor<GMSEC_F64>(value);
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "STR") ||
				         StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "STRING"))
				{
					char* tmpstr = StringUtil::stringNew(field_value.c_str());

					field_ptr = new SimpleFieldDescriptor<char*>(tmpstr);

					// Memory on m_heapList is deleted in the destructor to InternalSpecification.
					m_heapList.push_back((unsigned char*) tmpstr);
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "BIN")   ||
				         StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "BINARY") ||
				         StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "BLOB"))
				{
					unsigned char* tmp_mem = NULL;

					if (field_value.size() > 0)
					{
						tmp_mem = new unsigned char[field_value.size() / 2];

						// Binary values are expected to be
						// described in hex strings, with of
						// course 2 hex characters 
						// representing each byte; therefore
						// the size specified above.
	
						for (size_t mem_char_count = 0; mem_char_count < field_value.size() / 2; ++mem_char_count)
						{
							char          hex_str[3];
							unsigned int  mem_value;

							memcpy(hex_str, field_value.c_str() + mem_char_count * 2, 2);
							hex_str[2] = '\0';
#ifndef WIN32
							std::sscanf(hex_str, "%02X", &mem_value);
#else
							sscanf_s(hex_str, "%02X", &mem_value, sizeof(mem_value));
#endif

							tmp_mem[mem_char_count]= (unsigned char) mem_value;
						}
	 
						// Memory on m_heapList is deleted in the destructor to InternalSpecification.
						m_heapList.push_back(tmp_mem);

						field_ptr = new SimpleFieldDescriptor<GMSEC_BIN>(tmp_mem, sizeof(tmp_mem));
					}
					else
					{
						field_ptr = new SimpleFieldDescriptor<GMSEC_BIN>();
					}
				}
				else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "UNSET") ||
				         StringUtil::stringEqualsIgnoreCase(field_attrib->Value(), "VARIABLE"))
				{
					field_ptr = new VariableFieldDescriptor();
				}
			}
			
			//
			// DESCRIPTION="..." field attribute
			//
			else if (StringUtil::stringEqualsIgnoreCase(field_attrib->Name(), "DESCRIPTION"))
			{
				if (field_attrib->Value())
				{
					field_desc = StringUtil::stringToUpper(field_attrib->Value());
				}
			}
		}

		//
		// If a field appears to have been read properly,
		// add it to a list of AbstractFieldDescriptors that 
		// is the output of this function.
		//
		if (field_ptr != NULL && id != "")
		{
			field_ptr->setFieldVariables(field_name.c_str(),
			                             field_mode,
			                             field_class,
			                             field_desc.c_str());

			schemaFields.push_back(field_ptr);
		}
		else
		{
			std::string err_msg = "Could not parse XML schema successfully:  ";

			GMSEC_WARNING << err_msg.c_str() << id.c_str() << ", " << field_name.c_str();

			status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err_msg.c_str());
		}
	}

	return status;
}


// Fetch default message schemas, from static memory, into
// the private InternalSpecification::templates list.
Status InternalSpecification::loadDefaultTemplatesFromMemory(unsigned int version)
{
	Status status;

	if (version != mist::GMSEC_ISD_2014_00)
	{
		status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, "loadDefaultTemplatesFromMemory not available for schema other than GMSEC ISD 2014");
		return status;
	}

	int    index = 0;

	while (defaultMessageTemplates[index][0] != '\0' && !status.isError())
	// while not empty schema string
	{
		tinyxml2::XMLDocument xml_doc;

		if (xml_doc.Parse(defaultMessageTemplates[index].c_str()) == tinyxml2::XML_NO_ERROR)
		{
			tinyxml2::XMLElement* xml_schema = xml_doc.RootElement();

			if (xml_schema != NULL)
			{
				std::string id;
				std::string id_field_names;
				std::vector<AbstractFieldDescriptor*> schema_fields;

				status = parser(xml_schema, id, id_field_names, schema_fields);

				if (!status.isError())
				{
					m_idFieldNames.insert(std::pair<std::string, std::string>(id.c_str(), id_field_names.c_str()));

					//
					// Note:  A copy of a MessageTemplate is *not*
					//        made in the registerTemplate() function.
					//
					MessageTemplate* mt = new MessageTemplate(schema_fields, true);

					status = registerTemplate(id.c_str(), mt);
				}

				xml_doc.Clear();

				++index;
			}
			else
			{
				std::string err_msg = "xml_schema NULL";

				GMSEC_WARNING << "InternalSpecification::loadDefaultTemplatesFromMemory() : " << err_msg.c_str();

				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err_msg.c_str());
			}
		}
		else
		{
			std::string err_msg = "Unparseable XML string given";

			GMSEC_WARNING << "InternalSpecification::loadDefaultTemplatesFromMemory() : " << err_msg.c_str();

			status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, err_msg.c_str());
		}
	}

	return status;
}
