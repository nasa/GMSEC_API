/*
 * Copyright 2007-2017 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Disable the Visual Studio warning C4355
//   ('this' : used in base member initializer list)
//   Found in the Connection(Server s, const Options &o) constructor
#ifdef WIN32
#pragma warning(disable: 4355)
#endif

#include <gmsec4/internal/mist/InternalSpecification.h>
#include <gmsec4/internal/mist/MessageTemplate.h>
#include <gmsec4/internal/mist/InternalSchemaIDIterator.h>
#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/ConfigOptions.h>

#include <gmsec4/internal/FileUtil.h>
#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/Errors.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>
#include <gmsec4/Message.h>
#include <gmsec4/Config.h>

#include <gmsec4/util/Log.h>

#include <tinyxml2.h>

#include <memory>
#include <sstream>


using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


InternalSpecification::InternalSpecification(const Config& config)
	: m_config(config),
	  m_iterator(*this)
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
	if(version)
	{//it exists
		int versionInt;
		StringUtil::STR2NUM_ERROR result = StringUtil::str2int(versionInt, version);
		if(result == StringUtil::STR2NUM_SUCCESS)
		{//it's a number
			if(checkValidSpec(versionInt))
			{//it's a valid number!
				m_specificationId = version;
			}
			else
			{
				throw Exception(MIST_ERROR, INVALID_CONFIG_VALUE, "\"GMSEC-SPECIFICATION-VERSION\" contains invalid version");
			}
		}
		else
		{
			throw Exception(MIST_ERROR, INVALID_CONFIG_VALUE, "\"GMSEC-SPECIFICATION-VERSION\" contains invalid value");
		}
	}
	else
	{//default to the current version of the ISD
		std::ostringstream oss;
		oss << GMSEC_ISD_CURRENT;
		m_specificationId = oss.str();
	}

	const char* valLevel = m_config.getValue(GMSEC_VALIDATION_LEVEL);
	if(valLevel)
	{
		if(StringUtil::stringEqualsIgnoreCase(valLevel, "NO_ENFORCEMENT"))
		{//no validation
			m_validationLevel = 0;
		}
		else if(StringUtil::stringEqualsIgnoreCase(valLevel, "ENFORCE_REQUIRED"))
		{//validate required fields and ensure they are present in the message
			m_validationLevel = 1;
		}
		else if(StringUtil::stringEqualsIgnoreCase(valLevel, "ENFORCE_OPTIONAL"))
		{//validate required fields and ensure they are present in the message
		 //validate existing optional fields in the message, but they do not need to be present
			m_validationLevel = 2;
		}
		else if(StringUtil::stringEqualsIgnoreCase(valLevel, "ENFORCE_STRICT"))
		{//validate required fields and ensure they are present in the message
		 //validate existing optional fields in the message, but they do not need to be present
		 //custom fields not defined by template are not allowed
			m_validationLevel = 3;
		}
		else
		{//user may have supplied an integer, but we only accept values 0-3
			int valLevelInt;
			StringUtil::STR2NUM_ERROR result = StringUtil::str2int(valLevelInt, valLevel);
			if(result == StringUtil::STR2NUM_SUCCESS)
			{
				if(valLevelInt >=0 && valLevelInt <=3)
				{
					m_validationLevel = (unsigned int) valLevelInt;
				}
				else
				{
					throw Exception(MIST_ERROR, INVALID_CONFIG_VALUE, "\"GMSEC-VALIDATION-LEVEL\" contains out-of-range integer");
				}
			}
			else
			{//parse failed, not an integer
				throw Exception(MIST_ERROR, INVALID_CONFIG_VALUE, "\"GMSEC-VALIDATION-LEVEL\" contains invalid value");
			}
		}
	}
	else
	{//default to 1, ENFORCE_REQUIRED
		m_validationLevel = 1;
	}

	const char* schemaLevel = m_config.getValue(GMSEC_SCHEMA_LEVEL);
	if(schemaLevel)
	{
		int schemaLevelInt;
		StringUtil::STR2NUM_ERROR result = StringUtil::str2int(schemaLevelInt, schemaLevel);
		if(result == StringUtil::STR2NUM_SUCCESS)
		{
			if(schemaLevelInt >=0 && schemaLevelInt <=10)
			{//while the user can conceivably layer as many addendums as they want over the ISD,
			 //having more than 10 layered addendums is getting pretty ridiculous
				m_schemaLevel = (unsigned int) schemaLevelInt;
			}
			else
			{
				throw Exception(MIST_ERROR, INVALID_CONFIG_VALUE, "\"GMSEC-VALIDATION-LEVEL\" contains out-of-range integer");
			}
		}
		else
		{//parse failed, not an integer
			throw Exception(MIST_ERROR, INVALID_CONFIG_VALUE, "\"GMSEC-VALIDATION-LEVEL\" contains invalid value");
		}
	}
	else
	{//default to 0, GMSEC_ISD
		m_schemaLevel = 0;
	}


	// Finally, attempt to load the schemas
	try {
		load();
	}
	catch (const Exception& e) {
		cleanup();
		throw e;
	}

	m_schemaIndex = m_templates.begin();
}


InternalSpecification::InternalSpecification(const InternalSpecification& other)
	: m_config(other.m_config),
	  m_specificationId(other.m_specificationId),
	  m_validationLevel(other.m_validationLevel),
	  m_schemaLevel(other.m_schemaLevel),
	  m_iterator(*this),
	  m_basePath(other.m_basePath)
{
	// Finally, attempt to load the schemas
	try {
		load();
	}
	catch (const Exception& e) {
		cleanup();
		throw e;
	}
	m_schemaIndex = m_templates.begin();
}


InternalSpecification::~InternalSpecification()
{
	cleanup();
}


Status InternalSpecification::loadTemplate(const char* path)
{
	Status status;

	tinyxml2::XMLDocument xml;
	if(xml.LoadFile(path) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement* schema = xml.RootElement();

		if(schema != NULL)
		{
			std::string id;
			std::string specID = m_specificationId;
			specID.insert(4, ".");
			std::list<FieldTemplate> fields;
			status = parser(schema, id, fields);

			if(!status.isError())
			{
				if(id.find("HEADER") != std::string::npos)
				{
					m_headers = fields;
				}
				else if(id != specID)
				{
					// The types of Schema IDs that the API (currently) supports.
					//
					// Schema ID Type 1: major.minor.schemaLevel.messagekind.messagetype.<optionalmessagesubtype>
					// Schema ID Type 2: messagekind.messagetype.<optionalmessagesubtype>

					// We store the Message Template using a Type 1 Schema ID (if not already stored)
					if (m_templates.find(id) == m_templates.end())
					{
						m_templates[id] = new MessageTemplate(id.c_str(), fields);
					}
					else
					{
						std::ostringstream errmsg;
						errmsg <<  "Duplicate Schema Name of " << id << " found in: " << path;

						throw Exception(MIST_ERROR, TEMPLATE_DIR_ERROR, errmsg.str().c_str());
					}

					// We then store another copy of the Message Template using a Type 2 Schema ID (if not already stored)
					std::vector<std::string> elements = StringUtil::split(id, '.');
					if (elements.size() >= 5)
					{
						// have Major and Minor number, and presumably, addendum name (e.g. GMSEC or COMPATC2)
						std::ostringstream oss;
						for (size_t i = 3; i < elements.size(); ++i)
						{
							oss << elements.at(i) << ".";
						}
						std::string short_id = oss.str();
						short_id.erase(short_id.length() - 1);  // erase trailing period


						//o Check if Message Templates map has an entry for the short ID
						//o     If not
						//o			1. Add new Message Template to the map
						//o
						//o     Else
						//o 		1. Iterate through Schema List; for each entry,
						//o					Check if the schema level matches our desired level AND the schema ID is or part of our short ID; if so,
						//o						Stow the schema level name
						//o
						//o			2. Check if the schema name that was found is the same as our addendum name (that comes from the Specification ID)
						//o					If so, delete existing Message Template
						//o					Add new Message Template to the map
						//o
						if (m_templates.find(short_id) == m_templates.end())
						{
							m_templates[short_id] = new MessageTemplate(id.c_str(), fields);
						}
						else
						{
							std::string  levelName;
							for (SchemaTemplateList::iterator it = m_directory.begin(); it != m_directory.end(); ++it)
							{
								if ((it->getLevel() == m_schemaLevel) && (short_id.find(it->getID()) != std::string::npos))
								{
									levelName = it->getLevelName();
								}
							}

							if (levelName == elements.at(2))
							{
								delete m_templates[short_id];
								m_templates[short_id] = new MessageTemplate(id.c_str(), fields);
							}
						}
					}
				}
			}
			xml.Clear();
		}
		else
		{
			std::string msg = "Loaded XML schema is NULL";
			GMSEC_WARNING << "InternalSpecification::LoadTemplate() : " << msg.c_str();
			status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, msg.c_str());
		}
	}
	else
	{
		std::string msg = "Unparseable XML string given";
		GMSEC_WARNING << "InternalSpecification::LoadTemplate() : " << msg.c_str();
		status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, msg.c_str());
	}

	return status;
}


void InternalSpecification::validateMessage(const Message& msg)
{
	Status status;

	std::string schemaID;

	SchemaRegistry::const_iterator it = m_registry.find(msg.getSubject());

	if(it != m_registry.end())
	{//a matching message subject was found in the registry!

		//grabbing the schemaID registered to the message subject
		schemaID = it->second;
	}
	else
	{//the message subject is not registered with a schemaID yet
	 //we'll have to build a schemaID for the message and register it

		schemaID = registerTemplate(msg, m_schemaLevel);

		if(schemaID == "")
		{//if registerTemplate returned a blank string, then that means a matching template wasn't found
			std::string err = "Failed to build Schema ID from message ";
			err.append(msg.getSubject());
			throw Exception(MIST_ERROR, TEMPLATE_ID_DOES_NOT_EXIST, err.c_str());
		}
	}

	//now grab the message template with the matching schemaID
	bool foundSchema = false;
	MessageTemplates::const_iterator it2 = m_templates.find(schemaID);

	if (it2 != m_templates.end())
	{
		const MessageTemplate* temp = it2->second;

		foundSchema = true;
		//this list of fields will contain the combined header and contents
		std::list<FieldTemplate> fields;

		//first prepare the header fields with the values we need to validate
		fields = prepHeaders(schemaID.c_str());

		for(FieldTemplateList::const_iterator it3 = temp->listFieldTemplates().begin(); it3 != temp->listFieldTemplates().end(); ++it3)
		{//add content fields
			fields.push_back(*it3);
		}

		//compare the field list to the fields in the message
		status = compare(msg, fields);

		if(status.isError())
		{
			throw Exception(status);
		}
	}

	if(!foundSchema)
	{
		std::string err = "SchemaID ";
		err.append(schemaID);
		err.append("could not be found in list of available schema.");
		status.set(MIST_ERROR, MSG_LOOKUP_FAILURE, err.c_str());
	}

	if(status.isError())
	{
		throw Exception(status);
	}
}


Status InternalSpecification::compare(const Message& msg, const std::list<FieldTemplate>& fields)
{
	//current status
	Status status;
	//last recorded error
	Status lastError;

	//The accumulated error messages as a string
	std::string errorList = "";

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
	std::list<FieldTemplateList::const_iterator> arrayStartList;

	std::list<std::string> modifiedFieldNames;

	for(FieldTemplateList::const_iterator fieldIndex = fields.begin(); fieldIndex != fields.end(); ++fieldIndex)
	{//iterating through all the fields to make sure they are all in the message and correct

		//the current iteration's field template we will be validating against
		FieldTemplate temp = *fieldIndex;

		//check to see if the current field template is a control field
		if(StringUtil::stringEquals(temp.getMode().c_str(), "CONTROL") &&
		   StringUtil::stringEquals(temp.getName().c_str(), "ARRAY-START"))
		{
			arrayLevel++;
			skipArrayLevel = arrayLevel;

			//now we need to figure out details of the array
			try
			{
				//index for objects in messages start at 1 per GMSEC ISD
				arrayIndexList.push_back(1);

				//push the value of the control field (which is the index placeholder) onto the list
				arrayCharList.push_back(temp.getValue());

				//determine the size of the array
				if(arrayLevel>1)
				{//array is nested, the field name that contains the size of the nested array has an index placeholder
				 //index placeholder needs to be replaced with the index of the object in the array
					std::string name = prepFieldName(temp.getSize().c_str(),arrayCharList, arrayIndexList);
					size_t size = msg.getIntegerValue(name.c_str());
					arraySizeList.push_back(size);
				}
				else
				{
					size_t size = msg.getIntegerValue(temp.getSize().c_str());
					arraySizeList.push_back(size);
				}


				//starting index for this array control, bookmarked so we can loop back to it later
				arrayStartList.push_back(fieldIndex);
				continue;
			}
			catch(Exception &e)
			{
				if(e.getErrorClass() == MSG_ERROR && e.getErrorCode() == INVALID_FIELD)
				{//this catch occurred because the size of the array is undefined due to the field being missing
				 //if that field is optional, then it means the array is optional, so we skip the array entirely
				 //if the array is required, we throw the error
					for(FieldTemplateList::const_iterator it2 = fields.begin(); it2 != fields.end(); ++it2)
					{
						if(StringUtil::stringEquals(it2->getName().c_str(), temp.getSize().c_str()) &&
						   StringUtil::stringEquals(it2->getMode().c_str(), "OPTIONAL"))
						{//we have determined the array is optional and we will be skipping it
							skipArray = true;
							break;
						}
					}

					if(!skipArray)
					{//This array is required and we couldn't determine the size, throw error
						status.set(MIST_ERROR, MISSING_REQUIRED_FIELD, "Message contains array of objects whose size is undefined: ");
						std::ostringstream err;
						err << msg.getSubject() << " : Validation Failed." << "\n   " 
							<< status.get() << e.what();
						throw Exception(MIST_ERROR, MESSAGE_FAILED_VALIDATION, err.str().c_str());
					}
					else
					{
						continue;
					}
				}
				else
				{//it's some other weird error, throw it
					throw e;
				}
			}
		}
		else if(StringUtil::stringEquals(temp.getMode().c_str(), "CONTROL") &&
				StringUtil::stringEquals(temp.getName().c_str(), "ARRAY-END"))
		{
			if(skipArray && skipArrayLevel == arrayLevel)
			{//we've reached the end of the array that is being skipped
				skipArray = false;
				skipArrayLevel = 0;
				arrayIndexList.pop_back();
				arrayCharList.pop_back();
				arrayLevel--;
				continue;
			}

			if(arrayIndexList.back() < arraySizeList.back())
			{//there are still more objects in the array to validate
			 //reset fieldIndex to the beginning of the array control
			 //increase the listed index by 1
				fieldIndex = arrayStartList.back();
				size_t index = arrayIndexList.back();
				index++;
				arrayIndexList.pop_back();
				arrayIndexList.push_back(index);
				continue;
			}
			else
			{//all objects have been processed, close the array control
				arraySizeList.pop_back();
				arrayCharList.pop_back();
				arrayStartList.pop_back();
				arrayIndexList.pop_back();
				arrayLevel--;
				continue;
			}
		}
		else if(skipArray)
		{
			continue;
		}
		

		//modify the name of the field template as necessary
		if(arrayLevel > 0)
		{
			std::string name = prepFieldName(temp.getName().c_str(), arrayCharList, arrayIndexList);
			modifiedFieldNames.push_back(name);
			temp.setName(name.c_str());
		}

		//begin validation process for the field
		if(StringUtil::stringEquals(temp.getMode().c_str(), "REQUIRED") && 
		   m_validationLevel > 0)
		{//Required fields must be validated if m_validation is set at all

			status = validate(msg, temp);

			if(status.isError())
			{
				lastError = status;
				errorList.append("\n   ").append(lastError.get());
			}
		}
		else if(StringUtil::stringEquals(temp.getMode().c_str(), "OPTIONAL") && 
			    m_validationLevel > 1)
		{//Optional fields must be validated if m_validation is set greater than 1

			status = validate(msg, temp);

			if(status.isError())
			{
				lastError = status;
				errorList.append("\n   ").append(lastError.get());
			}
		}

	}//loop until we're done running through all the field templates

	if(m_validationLevel > 2)
	{//No junk fields if validation is set to strict
		MessageFieldIterator& iter = msg.getFieldIterator();
		while(iter.hasNext())
		{//iterating through all the fields in the message
			const Field& field = iter.next();
			bool found = false;
			for(FieldTemplateList::const_iterator it = fields.begin(); it != fields.end(); ++it)
			{//now look for a matching field in the template fields
				if(StringUtil::stringEquals(field.getName(), it->getName().c_str()))
				{
					found = true;
					break;
				}
			}
			if(!found)
			{
				for(std::list<std::string>::const_iterator it = modifiedFieldNames.begin(); it != modifiedFieldNames.end(); ++it)
				{
					std::string modifiedName = *it;
					if(StringUtil::stringEquals(field.getName(), modifiedName.c_str()))
					{
						found = true;
						break;
					}
				}
				if(!found)
				{
					std::ostringstream err;
					err << "Message contains user-defined field " << field.getName() 
						<< ", which is disallowed under the current validation settings";
					GMSEC_ERROR << err.str().c_str();
					status.set(MIST_ERROR, NON_ALLOWED_FIELD, err.str().c_str());
					lastError = status;
					errorList.append("\n   ").append(lastError.get());
				}
			}
		}
	}

	if(status.isError())
	{
		std::string err = msg.getSubject();
		err.append(": Validation Failed.");
		err.append("\n   ").append(errorList.c_str());
		return Status(MIST_ERROR, MESSAGE_FAILED_VALIDATION, err.c_str());
	}
	else if(lastError.isError())
	{
		std::string err = msg.getSubject();
		err.append(": Validation Failed.");
		err.append("\n   ").append(errorList.c_str());
		return Status(MIST_ERROR, MESSAGE_FAILED_VALIDATION, err.c_str());
	}

	GMSEC_VERBOSE << msg.getSubject() << ": Validation Passed.";
	return status;
}

std::string InternalSpecification::prepFieldName(const char* name, const std::list<std::string>& charList, const std::list<size_t>& indexList)
{
	std::string newName = name;

	std::list<std::string>::const_iterator arrayChar = charList.begin();

	for(std::list<size_t>::const_iterator arrayIndex = indexList.begin();
		arrayIndex != indexList.end(); ++arrayIndex)
	{//iterating through arrayCharList and arrayIndexList concurrently
		std::ostringstream oss;

		oss << *arrayIndex;

		if(newName.find(*arrayChar) != std::string::npos)
		{
			newName.replace(newName.find(*arrayChar), arrayChar->length(), oss.str());
		}

		++arrayChar;
	}

	return newName;
}

Status InternalSpecification::validate(const Message& msg, const FieldTemplate& ftmp)
{
	Status status;

	std::string mode = ftmp.getMode();

	std::string cumulativeError;

	//convert the field template into an actual field we can compare with
	//std::auto_ptr<Field> tmpField(ftmp.toField());

	try
	{
		//now look for a field in the message with a matching name
		const Field* msgField = msg.getField(ftmp.getName().c_str());
		
		if(msgField)
		{//we found a matching field!

			if(!ftmp.isTypeVariable())
			{
				//get the list of possible valid types the field could have
				std::list<Field::FieldType> types = ftmp.getTypes();

				bool valid = false;
				for (std::list<Field::FieldType>::const_iterator it = types.begin(); it != types.end(); ++it)
				{//running through the list of types the field template has

					Field::FieldType type = *it;

					if(msgField->getType() == type)
					{//the field's type needs to match one of the types in the list
						valid = true;
					}
				}

				if(!valid)
				{
					std::stringstream err;
					err << msgField->getName()
						<< " has incorrect field type of " << getTypeStr(msgField->getType())
						<< ".  Valid type(s): ";
					for(std::list<Field::FieldType>::const_iterator it = types.begin(); it != types.end(); ++it)
					{
						Field::FieldType type = *it;
						err << getTypeStr(type) << " ";
					}
					//GMSEC_ERROR << err.str().c_str();
					status.set(MIST_ERROR, INCORRECT_FIELD_TYPE, err.str().c_str());
					cumulativeError.append("\n      ").append(status.get());
				}
			}
			
			//TODO MAV:
			//EXPAND THIS BLOCK FOR MULTIPLE VALUES
			if(ftmp.hasExplicitValue())
			{//if the template's value isn't blank, the message field's value needs to be validated as well

				//for simplicity's sake, we're comparing the values as strings, since we can pretty much turn any type into a string
				std::string msgValue = getValue(msg, msgField->getName());
				std::string testValue = ftmp.getValue();

				if(msgValue != testValue)
				{
					std::stringstream err;
					err << msgField->getName()
						<< " has incorrect value of " << msgValue
						<< ", expected a value of " << testValue;
					//GMSEC_ERROR << err.str().c_str();
					status.set(MIST_ERROR, FIELD_NOT_SET, err.str().c_str());
					cumulativeError.append("\n      ").append(status.get());
				}
			}

		}
		else
		{//we couldn't find a matching field
			if(mode == "REQUIRED")
			{//validation fails at this point if the field is required
				std::stringstream err;
				err << ftmp.getName() 
					<< " is a required field, but is missing from message.  ";
				//GMSEC_ERROR << err.str().c_str();
				status.set(MIST_ERROR, MISSING_REQUIRED_FIELD, err.str().c_str());
				cumulativeError.append("\n      ").append(status.get());
			}
		}
	}
	catch(...)
	{//getField threw an error because there is no existing field by that name
		if(mode == "REQUIRED")
		{//validation fails at this point if the field is required
			std::stringstream err;
			err << ftmp.getName() 
				<< " is a required field, but is missing from message.  ";
			//GMSEC_ERROR << err.str().c_str();
			status.set(MIST_ERROR, MISSING_REQUIRED_FIELD, err.str().c_str());
			cumulativeError.append("\n      ").append(status.get());
		}
	}

	if (cumulativeError.empty())
	{
		return Status();
	}

	std::string err = "Field is not valid: ";
	err.append(cumulativeError);
	return Status(MIST_ERROR, FIELD_FAILED_VALIDATION, err.c_str());
}


std::string InternalSpecification::registerTemplate(const Message& msg, GMSEC_I64 schemaLevel)
{
	std::string typeDef;
	std::string subTypeDef;

	//when searching for a match in the directory, we want to start schema level indicated by the user
	std::string levelName;

	//This is the schemaID minus the spec version. We'll look this name up in the directory
	//to see if a more specific definition is available
	std::string name;

	//this will be marked true once we've figured out the most accurate definition for the message we can find
	bool found = false;

	while(!found && schemaLevel>=0)
	{
		for(SchemaTemplateList::const_iterator it = m_directory.begin(); it != m_directory.end(); ++it)
		{//now searching through the directory for the proper header definition
			SchemaTemplate temp = *it;
			if(StringUtil::stringEquals(temp.getID(), "HEADER") && temp.getLevel() == schemaLevel)
			{//found a matching header, now grab the default definition from it
				levelName = temp.getLevelName();
				typeDef = temp.getFirstDefinition();
				subTypeDef = temp.getNextDefinition();
			
				try
				{//add type and subtype to name
					name = msg.getStringValue(typeDef.c_str());
					name.append(".").append(msg.getStringValue(subTypeDef.c_str()));
				}
				catch(Exception &e)
				{//MESSAGE-TYPE or MESSAGE-SUBTYPE field was not found in message, validation automatically fails (not good)
					GMSEC_ERROR << e.what();
					return "";
				}	
				break;
			}	
		}
		//now we look for a more specific definition that matches up with the message
		found = findDefinition(schemaLevel, name, msg);

		if(!found)
		{
			schemaLevel--;
		}
	}

	//now that we have the name set up, append it to the schema ID
	std::string schemaID = m_specificationId;
	schemaID.insert(4, ".");
	schemaID.append(".");
	schemaID.append(levelName);
	schemaID.append(".");
	schemaID.append(name);
	
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
		for(SchemaTemplateList::const_iterator it = m_directory.begin(); it != m_directory.end(); ++it)
		{//now looking through directory
			SchemaTemplate temp = *it;
			if(StringUtil::stringEquals(temp.getID(), name.c_str()) && temp.getLevel() == (size_t) schemaLevel)
			{//looks like a more specific definition exists
			 //rebuild the name with the new definition
				found = true;
				try
				{
					std::string newName = msg.getStringField(temp.getFirstDefinition()).getValue();
					while(temp.hasNextDefinition())
					{
						newName.append(".");
						newName.append(msg.getStringField(temp.getNextDefinition()).getValue());
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
	m_headers.clear();
	m_directory.clear();
	m_registry.clear();
}


void InternalSpecification::load()
{
	Status status;    //This var will get overridden by the current status, error or non-error
	Status lastError; //This is the last error found by InternalSpecification::Load
	                  //This is to avoid potential scenarios where an error status 
	                  //is overridden by a non-error status

	std::string template_dir;

	// Deduce template directory path
	if (m_basePath.empty())
	{
		std::string shared_object_path;

		if (FileUtil::getCurrentSharedObjectPath(shared_object_path) == false)
		{
			std::string err_msg = "Could not get system-specified base path.";

			status.set(MIST_ERROR, TEMPLATE_DIR_NOT_FOUND, err_msg.c_str());

			GMSEC_WARNING << err_msg.c_str();
		}
		else
		{
			size_t dirMarker = shared_object_path.rfind(FileUtil::PATH_SEPARATOR);
	
			if (dirMarker != std::string::npos)
			{
				template_dir = shared_object_path.substr(0, dirMarker) + FileUtil::PATH_SEPARATOR + ".." + FileUtil::PATH_SEPARATOR + "templates";
			}
			else
			{
				std::string err_msg = "Could not list files in template directory ";
				err_msg += template_dir;

				status.set(MIST_ERROR, TEMPLATE_DIR_NOT_FOUND, err_msg.c_str());
	
				GMSEC_WARNING << err_msg.c_str();
			}
		}
	}
	else
	{
		template_dir = m_basePath;
	}

	if (!status.isError())
	{
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

		//TODO: Allow for filtering of specific file types (e.g. those with an xml extension).
		if (FileUtil::getFilesInDirectory(template_dir, schema_files) == false)
		{
			std::string err_msg = "Could not list files in template directory ";
			err_msg += template_dir;

			status.set(MIST_ERROR, TEMPLATE_DIR_ERROR, err_msg.c_str());

			GMSEC_ERROR << err_msg.c_str();
		}
		else
		{
			static std::string DIRECTORY_FILE = ".DIRECTORY.xml";

			// Always load the DIRECTORY file first...
			std::string path = template_dir;
			path += FileUtil::PATH_SEPARATOR;
			path += DIRECTORY_FILE;

			status = loadTemplate(path.c_str());

			if (m_directory.empty() || status.isError())
			{
				status.set(MIST_ERROR, TEMPLATE_DIR_ERROR, "Template Directory information failed to load");
				throw Exception(status);
			}

			// Then other files...
			for (std::list<std::string>::iterator it = schema_files.begin(); it != schema_files.end(); ++it)
			{
				std::string filename = *it;

				// We do not care about directories (current or above), or the DIRECTORY file (which has
				// already been processed)
				if (filename != ".." && filename != "." && filename != DIRECTORY_FILE)
				{
					path  = template_dir;
					path += FileUtil::PATH_SEPARATOR;
					path += filename;

					status = loadTemplate(path.c_str());

					if (status.isError())
					{
						std::string err = "Error loading template file ";
						err.append(filename.c_str());
						status.set(MIST_ERROR, TEMPLATE_DIR_ERROR, err.c_str());

						GMSEC_ERROR << status.get();

						lastError = status;
					}
				}
			}
		}
	}

	if (lastError.isError())
	{
		throw Exception(lastError);
	}
	else if (status.isError())
	{
		throw Exception(status);
	}
}


Status InternalSpecification::parser(tinyxml2::XMLElement* xmlSchema,
                                     std::string& schemaID,
                                     std::list<FieldTemplate>& schemaFields)
{
	Status status;
	std::ostringstream oss;

	//When the parser encounters an ARRAY-START control field, this flag will set
	//an error will be thrown if the parser encounters an ARRAY-END control field and the flag is not set
	//the number goes up as more array controls are opened, down as array controls are closed
	int arrayControlActive = 0;

	if(xmlSchema->FirstAttribute() != NULL)
	{
		std::string name = xmlSchema->FirstAttribute()->Name();
		if(name == "NAME")
		{//grabbing the name of the schema we are going to parse
			schemaID = xmlSchema->FirstAttribute()->Value();
		}
	}

	//delimiter is inserted after major revision (a four digit number)
	std::string id = m_specificationId;
	id.insert(4, ".");

	if(schemaID == "")
	{//We failed to set schemaID, which likely means it failed to parse
		status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, 
				   "The schema read did not have a template id NAME specified.");
		GMSEC_ERROR << status.get();
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
				if(StringUtil::stringEquals(attribute->Name(), "NAME"))
				{
					schema.setID(attribute->Value());
				}
				else if(StringUtil::stringEquals(attribute->Name(), "DEFINITION"))
				{
					std::vector<std::string> definitions = StringUtil::split(attribute->Value(), '.');

					for(std::vector<std::string>::const_iterator it = definitions.begin(); it != definitions.end(); ++it)
					{
						schema.addDefinition(std::string(*it).c_str());
					}
				}
				else if(std::string(attribute->Name()).find("LEVEL-") == 0)
				{
					std::string level = std::string(attribute->Name()).substr(6, std::string(attribute->Name()).length()-6);
					int value;
					StringUtil::STR2NUM_ERROR result = util::StringUtil::str2int(value, level.c_str());
					if(result == StringUtil::STR2NUM_SUCCESS)
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
						GMSEC_ERROR << status.get();
					}
				}
				else if(StringUtil::stringEquals(attribute->Name(), "DESCRIPTION"))
				{
					schema.setDescription(attribute->Value());
				}
			}//done parsing attributes for the schema

			//now check to make sure all attributes are acounted for
			if(StringUtil::stringEquals(schema.getID(), ""))
			{
				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, "Directory references a schema with no name");
				GMSEC_ERROR << status.get();
			}

			if(schema.isDefinitionEmpty())
			{
				oss.clear();
				oss.str("");
				oss << schema.getID() << " is referenced in directory but contains no definition";
				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
				GMSEC_ERROR << status.get();
			}

			if(StringUtil::stringEquals(schema.getLevelName(), ""))
			{
				oss.clear();
				oss.str("");
				oss << schema.getID() << " is referenced in directory but contains an unnamed level";
				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
				GMSEC_ERROR << status.get();
			}

			//now we load the schema into the directory
			m_directory.push_back(schema);

		}//done running through the child elements

		//the directory must have a HEADER entry for each schema level in the directory
		for(unsigned int i = 0; i <= m_schemaLevel; ++i)
		{
			bool found = false;
			for(SchemaTemplateList::const_iterator it = m_directory.begin(); it != m_directory.end(); ++it)
			{
				std::string id = it->getID();
				if(StringUtil::stringEquals(it->getID(), "HEADER") && it->getLevel() == i)
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
				break;
			}
		}

		return status;
	}//done parsing the directory file
	
	//Now we'll run through all of the child elements
	//and set them up as FieldTemplates
	for(tinyxml2::XMLElement* field = xmlSchema->FirstChildElement();
		field != NULL && !status.isError();
		field = field->NextSiblingElement())
	{
		FieldTemplate fieldTemplate;

		//Now that we have an element, we'll run through all of the attributes
		//and save them in their appropriate strings
		for(tinyxml2::XMLAttribute* attribute = (tinyxml2::XMLAttribute*) field->FirstAttribute();
			attribute != NULL;
			attribute = (tinyxml2::XMLAttribute*) attribute->Next())
		{
			if(StringUtil::stringEquals(attribute->Name(), "NAME"))
			{
				fieldTemplate.setName(attribute->Value());
			}
			else if(StringUtil::stringEquals(attribute->Name(), "MODE"))
			{
				fieldTemplate.setMode(attribute->Value());

				if(StringUtil::stringEquals(fieldTemplate.getMode().c_str(), "CONTROL") && 
				   StringUtil::stringEquals(fieldTemplate.getName().c_str(), "ARRAY-START"))
				{//encountered ARRAY-START, set array control flag
					arrayControlActive++;
				}
				else if(StringUtil::stringEquals(fieldTemplate.getMode().c_str(), "CONTROL") && 
						StringUtil::stringEquals(fieldTemplate.getName().c_str(), "ARRAY-END"))
				{//encountered ARRAY-END, clear array control flag if it is set or throw error
					if(arrayControlActive > 0)
					{//clear flag
						arrayControlActive--;
					}
					else
					{//error, we found an ARRAY-END control before we found an ARRAY-START
						status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE,
								   "Unexpected ARRAY-END control before ARRAY-START control.");
						GMSEC_ERROR << status.get();
					}
				}
			}
			else if(StringUtil::stringEquals(attribute->Name(), "FIELD_CLASS"))
			{
				fieldTemplate.setClass(attribute->Value());
			}
			else if(StringUtil::stringEquals(attribute->Name(), "TYPE"))
			{
				std::string value = attribute->Value();
				std::list<std::string> types;

				std::vector<std::string> list = StringUtil::split(value,',');

				for(std::vector<std::string>::const_iterator it = list.begin(); it != list.end(); ++it)
				{
					std::string type = StringUtil::trim(*it);
					types.push_back(type);
				}

				fieldTemplate.setTypes(types);
			}
			else if(StringUtil::stringEquals(attribute->Name(), "VALUE"))
			{
				fieldTemplate.setValue(attribute->Value());
	
				//TODO MAV:
				//EXPANDED VALUE PARSING
				
				//take attribute->value into temp variable
				//split variable into list (comma delimiter)

				//if only one item exists in the list then the field template has an immutable value (CONTENT-VERSION)
				//we should make note of this in the field template
				//auto-population should be able to automatically set this value when adding the field

				//now iterate through the list and for each value:
				
					//if the type is a string:
						
						//the first char may be a !, which indicates a NOT operator
						//save the value as string in list m_values (fieldTemplate.addValue?)
						//"VALUE" - valid field value
						//"!VALUE" - field must NOT have this value

					//if the type is a number:

						//the first char may be a !, which indicates a NOT operator
						//if value is a range (contains "..")

							//split value into list with ".." delimiter

							//values in this list should be able to parse into numbers

							//first value in list is stored as lower limit, second as upper limit
							//"firstValue.SecondValue.INCLUDE"
						
						//if item starts with "<" or ">":

							//check if it is preceded by a "="

							//for "<", lower bound is lowest possible value for given type
							//"MIN_VALUE.value.EXCLUDE"
							//if "=" is present, "min.value.INCLUDE"

							//for ">", upper bound is highest possible value for given type
							//"value.MAX_VALUE.EXCLUDE"
							//if "=" is present, "value.max.INCLUDE"
							
					//if type is a char:
						//make sure only one character is in each string value read in
						//..throw warning when truncating string?

					//if type is boolean:
						//no action needed?  the values the field accepts is already pretty strict

					//if type is binary blob:
						//PANIC
			}
			else if(StringUtil::stringEquals(attribute->Name(), "DESCRIPTION"))
			{
				fieldTemplate.setDescription(attribute->Value());
			}
			else if(StringUtil::stringEquals(attribute->Name(), "SIZE"))
			{//ARRAY-START control fields have their own attribute called SIZE
				fieldTemplate.setSize(attribute->Value());
			}
			else
			{
				oss.clear();
				oss.str("");
				oss << schemaID << ": The field template " << fieldTemplate.getName() << "contains an unrecognized attribute " << attribute->Name();
				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
				GMSEC_ERROR << status.get();
			}
		}//done parsing the attributes

		if(StringUtil::stringEquals(fieldTemplate.getName().c_str(), ""))
		{//throw an error if f_name wasn't parsed
			oss.clear();
			oss.str("");
			oss << schemaID << ":  field template is missing NAME attribute";
			status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
			GMSEC_ERROR << status.get();
		}

		if(StringUtil::stringEquals(fieldTemplate.getMode().c_str(), ""))
		{//throw an error if f_mode wasn't parsed
			oss.clear();
			oss.str("");
			oss << schemaID << ": the field template " << fieldTemplate.getName() << " is missing MODE attribute";
			status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
			GMSEC_ERROR << status.get();
		}

		//check to make sure all attributes are accounted for
		if(StringUtil::stringEquals(fieldTemplate.getMode().c_str(), "REQUIRED") || 
		   StringUtil::stringEquals(fieldTemplate.getMode().c_str(), "OPTIONAL"))
		{//checking attributes for regular field templates
			if(StringUtil::stringEquals(fieldTemplate.getClass().c_str(), ""))
			{
				oss.clear();
				oss.str("");
				oss << schemaID << ": the field template " << fieldTemplate.getName() << " is missing FIELD_CLASS attribute";
				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
				GMSEC_ERROR << status.get();
			}
			std::list<std::string> types = fieldTemplate.getTypeStrings();
			for(std::list<std::string>::const_iterator it = types.begin(); it != types.end(); ++it)
			{
				std::string type = *it;
				if(!(StringUtil::stringEquals(type.c_str(), "CHAR")			|| 
					 StringUtil::stringEquals(type.c_str(), "BIN")			||
					 StringUtil::stringEquals(type.c_str(), "BINARY")		||
					 StringUtil::stringEquals(type.c_str(), "BLOB")			||
					 StringUtil::stringEquals(type.c_str(), "BOOL")			||
					 StringUtil::stringEquals(type.c_str(), "BOOLEAN")		||
					 StringUtil::stringEquals(type.c_str(), "I8")			||
					 StringUtil::stringEquals(type.c_str(), "I16")			||
					 StringUtil::stringEquals(type.c_str(), "SHORT")		||
					 StringUtil::stringEquals(type.c_str(), "I32")			||
					 StringUtil::stringEquals(type.c_str(), "LONG")			||
					 StringUtil::stringEquals(type.c_str(), "I64")			||
					 StringUtil::stringEquals(type.c_str(), "U8")			||
					 StringUtil::stringEquals(type.c_str(), "U16")			||
					 StringUtil::stringEquals(type.c_str(), "USHORT")		||
					 StringUtil::stringEquals(type.c_str(), "U32")			||
					 StringUtil::stringEquals(type.c_str(), "ULONG")		||
					 StringUtil::stringEquals(type.c_str(), "U64")			||
					 StringUtil::stringEquals(type.c_str(), "F32")			||
					 StringUtil::stringEquals(type.c_str(), "FLOAT")		||
					 StringUtil::stringEquals(type.c_str(), "F64")			||
					 StringUtil::stringEquals(type.c_str(), "DOUBLE")		||
					 StringUtil::stringEquals(type.c_str(), "STRING")		||
					 StringUtil::stringEquals(type.c_str(), "VARIABLE")		||
					 StringUtil::stringEquals(type.c_str(), "UNSET")))
				{
					oss.clear();
					oss.str("");
					oss << schemaID << ": the field template " << fieldTemplate.getName() << " contains unrecognized type \"" << fieldTemplate.getType() << "\"";
					status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
					GMSEC_ERROR << status.get();
				}
			}
		}//done checking attributes for regular fields

		if(StringUtil::stringEquals(fieldTemplate.getMode().c_str(), "CONTROL"))
		{//checking control fields to make sure all attributes are present
			if(StringUtil::stringEquals(fieldTemplate.getName().c_str(), "ARRAY-START"))
			{
				if(StringUtil::stringEquals(fieldTemplate.getValue().c_str(), ""))
				{
					oss.clear();
					oss.str("");
					oss << schemaID << ": the control field " << fieldTemplate.getName() << " is missing VALUE attribute";
					status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
					GMSEC_ERROR << status.get();
				}
				if(StringUtil::stringEquals(fieldTemplate.getSize().c_str(), ""))
				{
					oss.clear();
					oss.str("");
					oss << schemaID << ": the control field " << fieldTemplate.getName() << " is missing SIZE attribute";
					status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
					GMSEC_ERROR << status.get();
				}
			}
		}//done checking CONTROL fields

		//And finally, load the FieldTemplate into schemaFields
		schemaFields.push_back(fieldTemplate);
	}//finished running through all child elements


	if(arrayControlActive > 0)
	{//If this flag is still set at this point then it means we have an open-ended array control
		oss.clear();
		oss.str("");
		oss << schemaID << ":  Expected ARRAY-END control not found";
		status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
		GMSEC_ERROR << status.get();
	}

	return status;
}


std::list<FieldTemplate> InternalSpecification::prepHeaders(const char* schemaID)
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

	for(SchemaTemplateList::const_iterator it = m_directory.begin(); it != m_directory.end(); ++it)
	{//look up schemaID in the directory
		SchemaTemplate temp = *it;
		std::string def = temp.getID();
		if(def.find(id) != std::string::npos)
		{//found it
			//the common headers only contain the first two definitions, so we only need the first two
			typeDef = temp.getFirstDefinition();
			subtypeDef = temp.getNextDefinition();
			break;
		}
	}

	if(typeDef == "")
	{//we couldn't find the schemaID in the directory, which means it isn't explicitly defined
	 //we'll instead search for a more generic definition for the schema
		std::string genID = id.substr(0, id.find_last_of("."));
		GMSEC_DEBUG << id.c_str() << " is not explicitly defined in directory, using definition " << genID.c_str();
		for(SchemaTemplateList::const_iterator it = m_directory.begin(); it != m_directory.end(); ++it)
		{
			SchemaTemplate temp = *it;
			if(StringUtil::stringEquals(temp.getID(), genID.c_str()))
			{
				typeDef = temp.getFirstDefinition();
				subtypeDef = temp.getNextDefinition();
				break;
			}
		}
	}

	if(typeDef == "")
	{//We still don't have a definition for the schema
	 //that's ok, we'll just default to the generic one defined by HEADER
		std::string genID = id.substr(0, id.find_last_of("."));
		GMSEC_DEBUG << genID.c_str() << " is not explicitly defined in the directory, defaulting to generic schema definition";
		for(SchemaTemplateList::const_iterator it = m_directory.begin(); it != m_directory.end() ; ++it)
		{
			SchemaTemplate temp = *it;
			if(StringUtil::stringEquals(temp.getID(), "HEADER"))
			{
				typeDef = temp.getFirstDefinition();
				subtypeDef = temp.getNextDefinition();
				break;
			}
		}
	}

	FieldTemplateList fields;

	for(FieldTemplateList::const_iterator it = m_headers.begin(); it != m_headers.end(); ++it)
	{
		FieldTemplate temp = *it;
		if(StringUtil::stringEquals(temp.getName().c_str(), typeDef.c_str()))
		{
			temp.setValue(type.c_str());
		}
		else if(StringUtil::stringEquals(temp.getName().c_str(), subtypeDef.c_str()))
		{
			temp.setValue(subtype.c_str());
		}

		fields.push_back(temp);
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


const char* InternalSpecification::getTypeStr(Field::FieldType type)
{
	switch(type)
	{
	case 1:
		return "CHAR";
	case 2:
		return "BOOL";
	case 3:
		return "I16";
	case 4:
		return "U16";
	case 5:
		return "I32";
	case 6:
		return "U32";
	case 7:
		return "F32";
	case 8:
		return "F64";
	case 9:
		return "STRING";
	case 10:
		return "BIN";
	case 20:
		return "I8";
	case 21:
		return "U8";
	case 22:
		return "I64";
	case 23:
		return "U64";
	default:
		return "UNDEFINED";
	}
}


std::string InternalSpecification::getValue(const Message& msg, const char* name) const
{
	std::ostringstream value;
	Field::FieldType type;
	
	type = msg.getFieldType(name);
	
	//determine what type of value the field has, then grab the value
	if(type == Field::BIN_TYPE)
	{
		BinaryField field = msg.getBinaryField(name);
		value << field.getValue();
	}
	else if(type == Field::BOOL_TYPE)
	{
		BooleanField field = msg.getBooleanField(name);
		value << field.getValue();
	}
	else if(type == Field::CHAR_TYPE)
	{
		CharField field = msg.getCharField(name);
		value << field.getValue();
	}
	else if(type == Field::F32_TYPE)
	{
		F32Field field = msg.getF32Field(name);
		value << field.getValue();
		if(value.str().find(".") == std::string::npos)
		{
			value << ".0";
		}
	}
	else if(type == Field::F64_TYPE)
	{
		F64Field field = msg.getF64Field(name);
		value << field.getValue();
		if(value.str().find(".") == std::string::npos)
		{
			value << ".0";
		}
	}
	else if(type == Field::I16_TYPE)
	{
		I16Field field = msg.getI16Field(name);
		value << field.getValue();
	}
	else if(type == Field::I32_TYPE)
	{
		I32Field field = msg.getI32Field(name);
		value << field.getValue();
	}
	else if(type == Field::I64_TYPE)
	{
		I64Field field = msg.getI64Field(name);
		value << field.getValue();
	}
	else if(type == Field::I8_TYPE)
	{
		I8Field field = msg.getI8Field(name);
		value << field.getValue();
	}
	else if(type == Field::STRING_TYPE)
	{
		StringField field = msg.getStringField(name);
		value << field.getValue();
	}
	else if(type == Field::U16_TYPE)
	{
		U16Field field = msg.getU16Field(name);
		value << field.getValue();
	}
	else if(type == Field::U32_TYPE)
	{
		U32Field field = msg.getU32Field(name);
		value << field.getValue();
	}
	else if(type == Field::U64_TYPE)
	{
		U64Field field = msg.getU64Field(name);
		value << field.getValue();
	}
	else if(type == Field::U8_TYPE)
	{
		U8Field field = msg.getU8Field(name);
		value << field.getValue();
	}
	else
	{//since we can conceivably turn anything into a string, we shouldn't be here
		std::stringstream err;
		err << "Unable to determine FieldType " << type
			<< " for field " << name;
		GMSEC_ERROR << err.str().c_str();
		throw Exception(MIST_ERROR, INCORRECT_FIELD_TYPE, err.str().c_str());
	}
	
	return value.str();
}


unsigned int InternalSpecification::getVersion() const
{
	int version;

	StringUtil::str2int(version, m_specificationId.c_str());

	return (unsigned int)version;
}


void InternalSpecification::setVersion(unsigned int version)
{
	if(checkValidSpec(version))
	{
		std::ostringstream oss;
		oss << version;
		m_specificationId = oss.str();

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
	else
	{
		throw Exception(MIST_ERROR, INVALID_CONFIG, "Supplied specification version is not valid");
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


bool InternalSpecification::checkValidSpec(unsigned int specVersionInt)
{
	switch(specVersionInt)
	{
		case GMSEC_ISD_2014_00:
		case GMSEC_ISD_2016_00:
			return true;
		default:
			return false;
	}
}


const std::list<SchemaTemplate>& InternalSpecification::getDirectory() const
{
	return m_directory;
}
