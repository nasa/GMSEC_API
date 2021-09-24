
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file BaseSpecification.cpp
 *
 *  @brief This file contains the BaseSpecification class.
 *
**/

#include <gmsec/internal/mist/BaseSpecification.h>

#include <gmsec/mist/MIST_errors.h>

#include <gmsec/internal/mist/Context.h>
#include <gmsec/internal/mist/MessageTemplate.h>
#include <gmsec/internal/mist/SimpleFieldDescriptor.h>
#include <gmsec/internal/mist/CompoundFieldDescriptor.h>
#include <gmsec/internal/mist/BaseSimpleFieldDescriptor.h>
#include <gmsec/internal/mist/BaseCompoundFieldDescriptor.h>
#include <gmsec/internal/mist/DefaultMessageSchemas.h>

#include <gmsec/Field.h>
#include <gmsec/Message.h>

#include <gmsec/util/Log.h>
#include <gmsec/internal/fileutil.h>
#include <gmsec/internal/strutil.h>   // to stringToUpper()

#include <cstdlib>

#ifdef __sun
	#include <string.h>
#else
	#include <cstring>
#endif


using namespace gmsec;
using namespace gmsec::internal;
using namespace gmsec::mist::internal;
using namespace gmsec::util;
using namespace std;


typedef std::map<std::string, gmsec::mist::internal::MessageTemplate*>::iterator TemplateListIt;

BaseSpecification::BaseSpecification()
{
}


BaseSpecification::~BaseSpecification()
{
	for(TemplateListIt it = templates.begin(); it != templates.end(); it++)
	{
		delete it->second;
	}

	for (int i = 0; i < m_heapList.size(); i++)
	{
		unsigned char* delete_node;

		m_heapList.get(i, delete_node);

		gmsec::util::stringDestroy((char* ) delete_node);

	}
}


Status BaseSpecification::Load(const char* basePath, unsigned int version)
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
	stringFormat(major_version_str, sizeof(major_version_str), "%04u", (unsigned int)version/100);
	stringFormat(minor_version_str, sizeof(minor_version_str), "%02u", (unsigned int)version-(((unsigned int)version/100)*100));
	specificationId = major_version_str;
	specificationId.append(".");
	specificationId.append(minor_version_str);

	//
	// Load paths
	//
	if (basePath == NULL)
	{
		std::string shared_object_path;

		if (getCurrentSharedObjectPath(shared_object_path) == false)
		{
			loadDefaultTemplatesFromMemory();

			std::string err_msg = "Could not get system-specified base path.";

			status.Set(GMSEC_GENERAL_MIST_ERROR,
			           GMSEC_SYSTEM_TEMPLATE_DIRECTORY_NOT_FOUND,
			           err_msg.c_str());

			LOG_WARNING << err_msg.c_str();
		}
		else
		{
			size_t dirMarker = shared_object_path.rfind(PATH_SEPARATOR);
	
			if (dirMarker != std::string::npos)
			{
				load_dir = shared_object_path.substr(0, dirMarker) + PATH_SEPARATOR + ".." + PATH_SEPARATOR + "templates";
			}
			else
			{
				loadDefaultTemplatesFromMemory();
	
				std::string err_msg = "Could not list files in template directory ";
				err_msg += load_dir;

				status.Set(GMSEC_GENERAL_MIST_ERROR,
				           GMSEC_COULD_NOT_FIND_TEMPLATE_DIRECTORY,
				           err_msg.c_str());
	
				LOG_WARNING << err_msg.c_str();
			}
		}
	}
	else
	{
		load_dir = basePath;
	}

	if (!status.IsError())
	{
		load_dir += PATH_SEPARATOR;
		load_dir += major_version_str;
		load_dir += ".";
		load_dir += minor_version_str;

		if (getFilesInDirectory(load_dir, schema_files) == false)
		{
			loadDefaultTemplatesFromMemory();

			std::string err_msg = "Could not list files in template directory ";
			err_msg += load_dir;

			status.Set(GMSEC_GENERAL_MIST_ERROR,
			           GMSEC_COULD_NOT_LIST_TEMPLATE_DIRECTORY,
			           err_msg.c_str());

			LOG_WARNING << err_msg.c_str();

		}
		else
		{
			for (std::list<std::string>::iterator it = schema_files.begin(); it != schema_files.end(); ++it)
			{
				std::string filename = *it;

				if (filename != ".." && filename != ".")
				{
					std::string path = load_dir;

					path += PATH_SEPARATOR;
					path += filename;

					status = LoadTemplate(filename.c_str(), path.c_str());
				}

				if (status.IsError())
				{
					LOG_WARNING << "Error loading template file " << filename.c_str();

					loadDefaultTemplatesFromMemory();
				}
			}
		}
	}

	return status;
}


// path will hold XML message definitions of the form above
Status BaseSpecification::LoadTemplate(const char* id, const char * path)
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
			List<AbstractFieldDescriptor*> schema_fields;

			status = parser(xml_schema, id_string, id_field_names,schema_fields);

			if (!status.IsError())
			{
				//
				// Note:  A copy of a MessageTemplate is *not*
				//        made in the Register() function.
				//
				m_idFieldNames.insert(std::pair<std::string, std::string>(id_string, id_field_names));

				MessageTemplate* mt = new MessageTemplate(schema_fields, true);

				status = Register(id_string.c_str(), mt);

				if (status.IsError())
				{
					delete mt;
				}
			}

			xml_doc.Clear();
		}
		else
		{
			String err_msg = "xml_schema NULL";

			LOG_WARNING << "BaseSpecification::LoadTemplate() : " << err_msg.c_str();

			status.Set(GMSEC_GENERAL_MIST_ERROR,
			           GMSEC_SCHEMA_FAILED_TO_PARSE,
			           err_msg.c_str());
		}
	}
	else
	{
		String err_msg = "Unparseable XML string given";

		LOG_WARNING << "BaseSpecification::LoadTemplate() : " << err_msg.c_str();

		status.Set(GMSEC_GENERAL_MIST_ERROR,
		           GMSEC_SCHEMA_FAILED_TO_PARSE,
		           err_msg.c_str());
	}

	return status;
}


// Update message (already created) according to template <id>
Status BaseSpecification::Apply(Message *message, Context &context) const
{
	Status status;
	const char * id;
	status = context.GetTemplateId(id);
	if(status.isError())
	{
		return status;
	}

	std::string sid = id;

	// Clean up the const char* allocated by BaseContext::GetTemplateId()
	gmsec::util::stringDestroy(id);

	Templates::const_iterator it = templates.find(sid);

	if (it == templates.end())
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_TEMPLATE_ID_DOES_NOT_EXIST, 
			"Unregistered template id requested");
		return status;
	}

	MessageTemplate *t = it->second;
	status = t->Apply(message, context);

	return status;
}


Status BaseSpecification::Instantiate(Message *message, Context &context) const
{
	Status status;
	const char * id;

	GMSEC_I32 count;
	message->GetFieldCount(count);
	if(count != 0)
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_REQUIRED_BLANK_MESSAGE, 
			"Empty message required for instantiate");
		return status;
	}

	status = context.GetTemplateId(id);
	if(status.isError())
	{
		return status;
	}

	std::string sid = id;

	// Clean up the const char* allocated by BaseContext::GetTemplateId()
	gmsec::util::stringDestroy(id);

	Templates::const_iterator it = templates.find(sid);

	if (it == templates.end())
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_TEMPLATE_ID_DOES_NOT_EXIST, 
			"Unregistered template id requested");
		return status;
	}

	MessageTemplate *t = it->second;
	status = t->Instantiate(message, context);

	return status;
}


// validate message against template
Status BaseSpecification::Validate(Message *message, Context &context) const
{
	Status status;
	const char* id;

	status = context.GetTemplateId(id);
	if(status.isError())
	{
		return status;
	}

	std::string sid = id;

	// Clean up the const char* allocated by BaseContext::GetTemplateId()
	gmsec::util::stringDestroy(id);

	Templates::const_iterator it = templates.find(sid);

	if (it == templates.end())
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_TEMPLATE_ID_DOES_NOT_EXIST, 
			"Unregistered template id requested");
		return status;
	}

	MessageTemplate *t = it->second;
	status = t->Validate(message, context);

	return status;
}


// programmatic definition
Status BaseSpecification::Register(const char* id, MessageTemplate* newTemplate)
{
	Status status;

	if (id != NULL)
	{
		templates.insert(std::pair<std::string, MessageTemplate*>(id, newTemplate));
	}
	else
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_INVALID_VALUE, "Template ID is invalid.");
	}

	return status;
}


// Fetches list of template IDs
void BaseSpecification::GetTemplateIds(List<String>& idList)
{
	idList.clear();

	for(TemplateListIt it = templates.begin(); it != templates.end(); it++)
	{
		idList.add(it->first.c_str());
	}
}


// Checks if a given message matches a template in this BaseSpecification
Status BaseSpecification::LookupTemplate(Message* message, String& templateIdStr)
{
	//
	// This function will search for whether a message type, and  message
	// subtypes (there *can* be more than one subtype), match
	// an id key in the templates list for a template spec.
	//

	Field       message_field;
	char        message_value_str_buffer[128];
	GMSEC_STR   message_value_str = message_value_str_buffer;
	// GMSEC_STR is const char*.  In the usage
	// below, most often message_value_str will just be
	// a destination buffer for gmsec::util::stringFormat() which
	// is a safe sprintf() function that works across different
	// platforms basically.  There is one case where message_value_str
	// is actually allocated by Field::GetValue() (when the relevant
	// field is a GMSEC_STR field); in that case the message_value_str
	// char* gets re-assigned a pointer to memory allocated by Field
	// itself, and apparently there is no need to de-allocate that by the
	// user; the de-allocation is handled by the Field destructor.

	GMSEC_I16   tmp_i16;
	GMSEC_U16   tmp_u16;
	GMSEC_I32   tmp_i32;
	GMSEC_U32   tmp_u32;
	GMSEC_I8    tmp_i8;
	GMSEC_U8    tmp_u8;
	GMSEC_I64   tmp_i64;
#ifdef GMSEC_FIELD_U64
	GMSEC_U64   tmp_u64;
#endif

	std::string msg_id_str;

	std::string full_schema_id;

	Status      status;

	bool        template_found = false;

	if (message != NULL)
	{
		FieldNames::iterator id_field_names_it = m_idFieldNames.begin();

		TemplateListIt templates_it = templates.begin();

		while (id_field_names_it != m_idFieldNames.end() &&
		       templates_it != templates.end() &&
		       !status.IsError() && !template_found)
		{
			msg_id_str = "";

			vector<std::string> id_field_names = gmsec::util::split(id_field_names_it->second, '.');

			unsigned int id_field_name_index = 0;
			Status       temp_status;

			while (id_field_name_index < id_field_names.size() && !status.IsError())
			{
				temp_status =
				  message->GetField(id_field_names[id_field_name_index].c_str(), message_field);

				if (!temp_status.IsError())
				{
					GMSEC_TYPE field_type;

					message_field.GetType(field_type);

					message_value_str = message_value_str_buffer;

					switch (field_type)
					{
					  case GMSEC_TYPE_I16:
						temp_status = message_field.GetValue(tmp_i16);
						stringFormat(message_value_str_buffer,
						             sizeof(message_value_str_buffer),
						             "%hd",
						             tmp_i16);
						break;

					  case GMSEC_TYPE_U16:
						temp_status = message_field.GetValue(tmp_u16);
						stringFormat(message_value_str_buffer,
						             sizeof(message_value_str_buffer),
						             "%hu",
						             tmp_u16);
						break;

					  case GMSEC_TYPE_I32:
						temp_status = message_field.GetValue(tmp_i32);
						stringFormat(message_value_str_buffer,
						             sizeof(message_value_str_buffer),
						             "%ld",
						             tmp_i32);
						break;

					  case GMSEC_TYPE_U32:
						temp_status = message_field.GetValue(tmp_u32);
						stringFormat(message_value_str_buffer,
						             sizeof(message_value_str_buffer),
						             "%lu",
						             tmp_u32);
						break;

					  case GMSEC_TYPE_I8:
						temp_status = message_field.GetValue(tmp_i8);
						stringFormat(message_value_str_buffer,
						             sizeof(message_value_str_buffer),
						             "%hhd",
						             tmp_i8);
						break;

					  case GMSEC_TYPE_U8:
						temp_status = message_field.GetValue(tmp_u8);
						stringFormat(message_value_str_buffer,
						             sizeof(message_value_str_buffer),
						             "%hhu",
						             tmp_u8);
						break;

					  case GMSEC_TYPE_I64:
						temp_status = message_field.GetValue(tmp_i64);
						stringFormat(message_value_str_buffer,
						             sizeof(message_value_str_buffer),
						             "%lld",
						             tmp_i64);
						break;

#ifdef GMSEC_FIELD_U64
					  case GMSEC_TYPE_U64:
						temp_status = message_field.GetValue(tmp_u64);
						stringFormat(message_value_str_buffer,
						             sizeof(message_value_str_buffer),
						             "%llu",
						             tmp_u64);
						break;
#endif

					  case GMSEC_TYPE_STR:
					  default:
						message_value_str = NULL;
						temp_status = message_field.GetValue(message_value_str);
						break;

					}

					if (!temp_status.IsError())
					{
						msg_id_str += ".";
						msg_id_str += message_value_str;
					}
					else
					{
						status.Set(GMSEC_GENERAL_MIST_ERROR,
						           GMSEC_INCORRECT_FIELD_TYPE,
						           status.GetString());

						LOG_WARNING << status.GetString();

					}
				}

				id_field_name_index++;

			}

			full_schema_id = specificationId;

			full_schema_id.append(msg_id_str.c_str());

			std::string template_id_str = templates_it->first;

			if (gmsec::util::stringEqualsIgnoreCase(template_id_str.c_str(), full_schema_id.c_str()))
			{
				template_found = true;
			}

			id_field_names_it++;
			templates_it++;

		}
	}
	else
	{
		String err_str = "Message passed into BaseSpecification::LookupTemplate() was NULL";

		status.Set(GMSEC_GENERAL_MIST_ERROR,
		           GMSEC_MESSAGE_NULL,
		           err_str.c_str());

		LOG_WARNING << err_str.c_str();

	}

	if (template_found)
	{
		templateIdStr = full_schema_id.c_str();
	}
	else
	{
		templateIdStr = "";
		if (!status.IsError()) // no error was found before
		{
			status.Set(GMSEC_GENERAL_MIST_ERROR,
			           GMSEC_TEMPLATE_ID_DOES_NOT_EXIST,
			           "The message passed into "
			           "BaseSpecification::LookupTemplate() "
			           "does not match any loaded message "
			           "template.");
		}
	}

	return status;
}


// Translate the XML specification in a TinyXML (which is a 3rd-party package)
// node into an ID string and list of MIST FieldDescriptors, which is
// intended to be inserted into a MIST MessageTemplate.
Status BaseSpecification::parser(tinyxml2::XMLElement* xmlSchema, std::string& id, std::string& idFieldNames, List<AbstractFieldDescriptor*>& schemaFields)
{
	Status status;

	const tinyxml2::XMLAttribute* xml_schema_attribute = xmlSchema->FirstAttribute();

	if (xml_schema_attribute != NULL)
	{
		if (gmsec::util::stringEqualsIgnoreCase(xml_schema_attribute->Name(), "NAME"))
		{
			id = gmsec::util::stringToUpper(xml_schema_attribute->Value());
		}
		
		xml_schema_attribute = xml_schema_attribute->Next();

		if (xml_schema_attribute != NULL)
		{
			if (gmsec::util::stringEqualsIgnoreCase(xml_schema_attribute->Name(), "ID_FIELD_NAMES"))
			{
				idFieldNames = gmsec::util::stringToUpper(xml_schema_attribute->Value());
			}
		}
	}

	if (id == "" || idFieldNames == "")
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR,
		           GMSEC_SCHEMA_FAILED_TO_PARSE,
		           "The schema read did not have a "
		           "template id NAME or ID_FIELD_NAMES specified.");
	}

	std::string field_name;
	std::string field_value;
	FieldMode   field_mode = F_OPTIONAL;
	FieldClass  field_class = F_NONE;
	std::string field_desc;
	AbstractFieldDescriptor* field_ptr = 0;

	//
	// Loop adding all fields to fields list parameter
	//
	for (tinyxml2::XMLElement* field = xmlSchema->FirstChildElement();
             field != NULL && !status.IsError();
             field = field->NextSiblingElement())
	{
		field_ptr = NULL;

		for (tinyxml2::XMLAttribute* field_attrib = (tinyxml2::XMLAttribute* ) field->FirstAttribute();
		     field_attrib != NULL;
			 field_attrib = (tinyxml2::XMLAttribute* ) field_attrib->Next())
		{
			//
			// NAME="..." field attribute
			//
			if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Name(), "NAME"))
			{
				field_name = gmsec::util::stringToUpper(field_attrib->Value());
			}
			//
			// MODE="..." field attribute
			//
			else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Name(), "MODE"))
			{
				if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "REQUIRED"))
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
			else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Name(), "FIELD_CLASS"))
			{
				if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "STANDARD"))
				{
					field_class = F_STANDARD;
				}
				else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "HEADER"))
				{
					field_class = F_HEADER;
				}
				else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "USER"))
				{
					field_class = F_USER;
				}
				else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "API"))
				{
					field_class = F_API;
				}
				else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "VARIABLE"))
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
			else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Name(), "VALUE"))
			{
				field_value = gmsec::util::stringToUpper(field_attrib->Value());
			}
			//
			// TYPE="..." field attribute
			//
			else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Name(), "TYPE"))
			{
				if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "CHAR"))
				{
					//
					// Question:  What does the fvalue
					// ultimately denote anyway in a 
					// SimpleFieldDescriptor?
					//

					GMSEC_CHAR ch = field_value.c_str()[0];

					field_ptr = new SimpleFieldDescriptor<GMSEC_CHAR>(ch);
				}
				else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "BOOL") ||
				         gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "BOOLEAN"))
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
				else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "I16") ||
				         gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "SHORT"))
				{
					GMSEC_I16 value = (GMSEC_I16)atoi(field_value.c_str());
					field_ptr = new SimpleFieldDescriptor<GMSEC_I16>(value);
				}
				else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "U16") ||
				         gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "USHORT"))
				{
					GMSEC_U16 value = (GMSEC_U16)atoi(field_value.c_str());
					field_ptr = new SimpleFieldDescriptor<GMSEC_U16>(value);
				}
				else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "I32") ||
				         gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "LONG"))
				{
					GMSEC_I32 value = (GMSEC_I32)atol(field_value.c_str());
					field_ptr = new SimpleFieldDescriptor<GMSEC_I32>(value);
				}
				else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "U32") ||
				         gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "ULONG"))
				{
					GMSEC_U32 value = (GMSEC_U32)atol(field_value.c_str());
					field_ptr = new SimpleFieldDescriptor<GMSEC_U32>(value);
				}
				else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "F32") ||
				         gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "FLOAT"))
				{
					GMSEC_F32 value = (GMSEC_F32)atof(field_value.c_str());
					field_ptr = new SimpleFieldDescriptor<GMSEC_F32>(value);
				}
				else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "F64") ||
				         gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "DOUBLE"))
				{
					GMSEC_F64 value = (GMSEC_F64)atof(field_value.c_str());
					field_ptr = new SimpleFieldDescriptor<GMSEC_F64>(value);
				}
				else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "STR") ||
				        gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "STRING"))
				{
					char* tmpstr = gmsec::util::stringNew(field_value.c_str());

					field_ptr = new SimpleFieldDescriptor<GMSEC_STRING>(tmpstr);

					m_heapList.add((unsigned char* ) tmpstr);
					// Memory on m_heapList is deleted
					// in the destructor to
					// BaseSpecification.

				}
				else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "BIN")   ||
				        gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "BINARY") ||
				        gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "BLOB")   ||
				        gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "UNSET"))
				{
					unsigned char* tmp_mem = NULL;

					if (field_value.size() > 0)
					{
						tmp_mem = new unsigned char[field_value.size()/2];

						// Binary values are expected to be
						// described in hex strings, with of
						// course 2 hex characters 
						// representing each byte; therefore
						// the size specified above.
	
						for (size_t mem_char_count = 0;
						     mem_char_count < field_value.size()/2;
						     mem_char_count++)
						{
							char          hex_str[3];
							unsigned int  mem_value;

							memcpy(hex_str, field_value.c_str()+mem_char_count*2, 2);
							hex_str[2] = '\0';
#ifndef WIN32
							sscanf(hex_str, "%02X", &mem_value);
#else
							sscanf_s(hex_str, "%02X", &mem_value);
#endif

							tmp_mem[mem_char_count]= (unsigned char) mem_value;

						}
	 
						m_heapList.add(tmp_mem);
						// Memory on m_heapList is deleted
						// in the destructor to
						// BaseSpecification.

			        	if (!gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "BLOB"))
						{
							field_ptr = new SimpleFieldDescriptor<GMSEC_BIN>(tmp_mem, (GMSEC_U32) sizeof(tmp_mem));
						}
						else
						{
							GMSEC_BLOB tmp_void_ptr = (GMSEC_BLOB ) tmp_mem;

							field_ptr = new SimpleFieldDescriptor<GMSEC_BLOB>(tmp_void_ptr, (GMSEC_U32) sizeof(tmp_mem));
						}
					}
					else
					{
						if (!gmsec::util::stringEqualsIgnoreCase(field_attrib->Value(), "BLOB"))
						{
							field_ptr = new SimpleFieldDescriptor<GMSEC_BIN>();
						}
						else
						{
							field_ptr = new SimpleFieldDescriptor<GMSEC_BLOB>();
						}
					}
				}
			}
			//
			// DESCRIPTION="..." field attribute
			//
			else if (gmsec::util::stringEqualsIgnoreCase(field_attrib->Name(), "DESCRIPTION"))
			{
				if (field_attrib->Value())
				{
					field_desc = gmsec::util::stringToUpper(field_attrib->Value());
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
			field_ptr->SetFieldVariables(field_name.c_str(),
			                             field_mode,
			                             field_class,
			                             field_desc.c_str());

			schemaFields.add(field_ptr);

		}
		else
		{
			delete field_ptr;

			const char* err_msg = "Could not parse XML schema successfully:  ";

			LOG_WARNING << err_msg << id.c_str() << ", " << field_name.c_str();

			status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_SCHEMA_FAILED_TO_PARSE, err_msg);
		}
	}

	return status;
}


// Fetch default message schemas, from static memory, into
// the private BaseSpecification::templates list.
Status BaseSpecification::loadDefaultTemplatesFromMemory()
{
	Status status;
	int    index = 0;

	while (defaultMessageTemplates[index][0] != '\0' && !status.IsError())
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
				List<AbstractFieldDescriptor*> schema_fields;

				status = parser(xml_schema, id, id_field_names, schema_fields);

				if (!status.IsError())
				{
					//
					// Note:  A copy of a MessageTemplate is *not*
					//        made in the Register() function.
					//
					m_idFieldNames.insert(std::pair<std::string, std::string>(id, id_field_names));

					MessageTemplate* mt = new MessageTemplate(schema_fields, true);

					status = Register(id.c_str(), mt);

					if (status.IsError())
					{
						delete mt;
					}
				}

				xml_doc.Clear();

				index++;
			}
			else
			{
				String err_msg = "xml_schema NULL";

				LOG_WARNING << "BaseSpecification::loadDefaultTemplatesFromMemory() : " << err_msg.c_str();

				status.Set(GMSEC_GENERAL_MIST_ERROR,
				           GMSEC_SCHEMA_FAILED_TO_PARSE,
				           err_msg.c_str());
			}
		}
		else
		{
			String err_msg = "Unparseable XML string given";

			LOG_WARNING << "BaseSpecification::loadDefaultTemplatesFromMemory() : " << err_msg.c_str();

			status.Set(GMSEC_GENERAL_MIST_ERROR,
			           GMSEC_SCHEMA_FAILED_TO_PARSE,
			           err_msg.c_str());
		}
	}

	return status;
}
