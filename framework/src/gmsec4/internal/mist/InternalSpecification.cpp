/*
 * Copyright 2007-2016 United States Government as represented by the
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
	const char* path = m_config.getValue("GMSEC-SCHEMA-PATH");
	if(!path)
	{
		m_basePath = "";
	}
	else
	{
		m_basePath = path;
	}

	const char* version = m_config.getValue("GMSEC-SPECIFICATION-VERSION");
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

	const char* valLevel = m_config.getValue("GMSEC-VALIDATION-LEVEL");
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

	const char* schemaLevel = m_config.getValue("GMSEC-SCHEMA-LEVEL");
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
	load();
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
	load();
}


InternalSpecification::~InternalSpecification()
{
	for(MessageTemplateList::const_iterator it = m_templates.begin(); it != m_templates.end(); ++it)
	{
		delete *it;
	}
	m_templates.clear();
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
			gmsec::api::util::DataList<FieldTemplate> fields;
			status = parser(schema, id, fields);

			if(!status.isError())
			{
				if(id.find("HEADER") != std::string::npos)
				{
					m_headers = fields;
				}
				else if(id != specID)
				{
					m_templates.add(new MessageTemplate(id.c_str(), fields));
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

		schemaID = registerTemplate(msg);

		if(schemaID == "")
		{//if registerTemplate returned a blank string, then that means a matching template wasn't found
			std::string err = "Failed to build Schema ID from message ";
			err.append(msg.getSubject());
			throw Exception(MIST_ERROR, TEMPLATE_ID_DOES_NOT_EXIST, err.c_str());
		}
	}

	//now grab the message template with the matching schemaID
	bool foundSchema = false;
	for(size_t i=0; i<m_templates.size(); i++)
	{
		if(m_templates.get(i)->getID() == schemaID)
		{
			foundSchema = true;
			//this list of fields will contain the combined header and contents
			gmsec::api::util::DataList<FieldTemplate> fields;

			//first prepare the header fields with the values we need to validate
			fields = prepHeaders(schemaID.c_str());

			for(size_t j=0; j<m_templates.get(i)->listFieldTemplates().size(); j++)
			{//add content fields
				fields.add(m_templates.get(i)->listFieldTemplates().get(j));
			}

			//compare the field list to the fields in the message
			status = compare(msg, fields);

			if(status.isError())
			{
				throw Exception(status);
			}
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


Status InternalSpecification::compare(const Message& msg, const gmsec::api::util::DataList<FieldTemplate>& fields)
{
	//current status
	Status status;
	//last recorded error
	Status lastError;

	//The accumulated error messages as a string
	std::string errorList = "";

	for(size_t fieldIndex=0; fieldIndex<fields.size(); fieldIndex++)
	{//iterating through all the fields to make sure they are all in the message and correct

		std::string mode = fields.get(fieldIndex).getMode();
		std::string name = fields.get(fieldIndex).getName();
		if(mode == "REQUIRED" && 
		   m_validationLevel > 0)
		{//Required fields must be validated if m_validation is set at all

			status = validate(msg, fields.get(fieldIndex));

			if(status.isError())
			{
				lastError = status;
				errorList.append("\n   ").append(lastError.get());
			}
		}
		else if(mode == "OPTIONAL" && 
			    m_validationLevel > 1)
		{//Optional fields must be validated if m_validation is set greater than 1

			status = validate(msg, fields.get(fieldIndex));

			if(status.isError())
			{
				lastError = status;
				errorList.append("\n   ").append(lastError.get());
			}
		}

		//TODO (MAV): Array control logic found below can be simplified into a single recursive call

		else if(mode == "CONTROL" && 
			    name == "ARRAY-START")
		{//we have found a control field indicating we have encountered fields 
		 //that may be used multiple times in a series

			bool skipArray = false;

			//this is the field template whose name will be modified by the array control(s)
			FieldTemplate f = fields.get(fieldIndex);
			std::ostringstream oss;

			//this is the character(s) we will replace with the arrayIndex
			std::string arrayChar = fields.get(fieldIndex).getValue();

			//the index for objects in messages start at 1, not 0
			//don't ask why, that's just how it is per GMSEC ISD
			size_t arrayIndex = 1;

			//now we need to figure out what the size of the array is
			size_t arraySize;
			try
			{
				arraySize = getIntValue(msg, fields.get(fieldIndex).getSize());
			}
			catch(Exception &e)
			{//we failed to determine the size of the array
				std::string name = fields.get(fieldIndex).getSize();
				if(e.getErrorClass() == MSG_ERROR && e.getErrorCode() == INVALID_FIELD_NAME)
				{//this catch occurred because size of the array is undefined due to the field being missing
				 //if that field is optional, then it means the array is optional, so we skip the array entirely
				 //if the array is required, we throw the error.
					for(size_t i=0; i<fields.size(); i++)
					{
						std::string fieldName = fields.get(i).getName();
						std::string fieldMode = fields.get(i).getMode();
						if(fieldName == name && fieldMode == "OPTIONAL")
						{//we have determined the array is optional and we will be skipping it
							skipArray = true;
							fieldIndex++;
							std::string skipName = fields.get(fieldIndex).getName();
							std::string skipMode = fields.get(fieldIndex).getMode();
							while(!(skipMode == "CONTROL" && skipName == "ARRAY-END"))
							{//skipping the index to the end of the array
								fieldIndex++;
								skipName = fields.get(fieldIndex).getName();
								skipMode = fields.get(fieldIndex).getMode();
							}
							break;
						}
					}
					if(!skipArray)
					{//This array is required and we couldn't determine the size, throw error
						throw Exception(e);
					}
				}
				else
				{//it's some other weird error, throw it
					throw Exception(e);
				}
			}

			if(!skipArray)
			{
				//move the field index up to get the next field
				fieldIndex++;
				
				//start of the array of fields 
				//we roll back to this index for each object we need to verify
				size_t arrayStartIndex = fieldIndex;
				size_t arrayEndIndex;

				for(; arrayIndex<=arraySize; arrayIndex++)
				{//the array of fields is repeated for each object the message has
					std::string controlMode = fields.get(fieldIndex).getMode();
					std::string controlName = fields.get(fieldIndex).getName();
					while(!(controlMode == "CONTROL" && 
						    controlName == "ARRAY-END"))
					{//we execute this logic until we encounter the end of the array

						controlMode = fields.get(fieldIndex).getMode();
						controlName = fields.get(fieldIndex).getName();
						if(controlMode == "CONTROL" && 
						   controlName == "ARRAY-START")
						{//we have found a nested array control field

							bool skipNestedArray = false;

							//this is the character(s) we will replace with the nested arrayIndex
							std::string nestedArrayChar = fields.get(fieldIndex).getValue();

							//the index for objects in messages start at 1, not 0
							size_t nestedArrayIndex = 1;

							//set up the field name with the current arrayIndex to get the nestedArraySize
							std::string nestedName = fields.get(fieldIndex).getSize();
							oss.clear();
							oss.str("");
							oss << arrayIndex;
							nestedName = nestedName.replace(nestedName.find(arrayChar),arrayChar.length(), oss.str());

							//now we need to figure out what the size of the nested array is
							size_t nestedArraySize;
							try
							{
								nestedArraySize = getIntValue(msg, nestedName.c_str());
							}
							catch(Exception &e)
							{//we failed to determine the size of the array
								if(e.getErrorClass() == MSG_ERROR && e.getErrorCode() == INVALID_FIELD_NAME)
								{//this catch occurred because size of the array is undefined due to the field being missing
								 //if that field is optional, then it means the array is optional, so we skip the array entirely
								 //if the array is required, we throw the error.
									for(size_t i=0; i<fields.size(); i++)
									{
										std::string fieldName = fields.get(i).getName();
										std::string fieldMode = fields.get(i).getMode();
										if(fieldName == nestedName && fieldMode == "OPTIONAL")
										{//we have determined the array is optional and we will be skipping it
											skipNestedArray = true;
											fieldIndex++;
											std::string skipName = fields.get(fieldIndex).getName();
											std::string skipMode = fields.get(fieldIndex).getMode();
											while(!(skipMode == "CONTROL" && skipName == "ARRAY-END"))
											{//skipping the index to the end of the array
												fieldIndex++;
												skipName = fields.get(fieldIndex).getName();
												skipMode = fields.get(fieldIndex).getMode();
											}
											break;
										}
									}
									if(!skipNestedArray)
									{//This array is required and we couldn't determine the size, throw error
										throw Exception(e);
									}
								}
								else
								{//it's some other weird error, throw it
									throw Exception(e);
								}
							}
							
							if(!skipNestedArray)
							{
								//move the field index up to get the next field
								fieldIndex++;
								controlMode = fields.get(fieldIndex).getMode();
								controlName = fields.get(fieldIndex).getName();

								//start of the array of nested fields 
								//we roll back to this index for each nested object we need to verify
								size_t nestedArrayStartIndex = fieldIndex;
								size_t nestedArrayEndIndex;

								for(; nestedArrayIndex<=nestedArraySize; nestedArrayIndex++)
								{//the array of fields is repeated for each nested object

									controlMode = fields.get(fieldIndex).getMode();
									controlName = fields.get(fieldIndex).getName();
									while(!(controlMode == "CONTROL" && 
										  controlName == "ARRAY-END"))
									{//we execute this logic until we encounter the end of the array

										//set up FieldTemplate that we will be validating against
										f = fields.get(fieldIndex);

										//set up the name for the FieldTemplate
										std::string str = f.getName();
										oss.clear();
										oss.str("");
										oss << arrayIndex;
										str.replace(str.find(arrayChar), arrayChar.length(), oss.str());
										oss.clear();
										oss.str("");
										oss << nestedArrayIndex;
										str.replace(str.find(nestedArrayChar), nestedArrayChar.length(), oss.str());

										//now insert the modified name back into the field template
										f.setName(str.c_str());

										if(controlMode == "REQUIRED" && 
										   m_validationLevel > 0)
										{//Required fields must be validated if m_validation is set at all

											status = validate(msg, f);

											if(status.isError())
											{
												lastError = status;
												errorList.append("\n   ").append(lastError.get());
											}
										}
										else if(controlMode == "OPTIONAL" && 
												m_validationLevel > 1)
										{//Optional fields must be validated if m_validation is set greater than 1

											status = validate(msg, f);

											if(status.isError())
											{
												lastError = status;
												errorList.append("\n   ").append(lastError.get());
											}
										}

										//moving on the the next field
										fieldIndex++;
										controlMode = fields.get(fieldIndex).getMode();
										controlName = fields.get(fieldIndex).getName();
										if(controlMode == "CONTROL" && controlName == "ARRAY-END")
										{
											nestedArrayEndIndex = fieldIndex;
										}

									}//finished with the nested array control loop

									//moving on the the next nested object, resetting field index
									if(nestedArrayIndex<nestedArraySize)
									{//there are still more objects, reset the field index to the start of the array
										fieldIndex = nestedArrayStartIndex;
										controlMode = fields.get(fieldIndex).getMode();
										controlName = fields.get(fieldIndex).getName();
									}
									else
									{//no more objects, reset the field index to the end of the array
										fieldIndex = nestedArrayEndIndex;
										controlMode = fields.get(fieldIndex).getMode();
										controlName = fields.get(fieldIndex).getName();
									}

								}//done with the nested objects in the current object of the message
							}

						}
						else
						{//normal field template within array, continue as normal

							//set up FieldTemplate that we will be validating against
							f = fields.get(fieldIndex);

							//set up the name for the FieldTemplate
							std::string str = f.getName();
							oss.clear();
							oss.str("");
							oss << arrayIndex;
							str.replace(str.find(arrayChar), arrayChar.length(), oss.str());

							//now insert the modified name back into the field template
							f.setName(str.c_str());
						}

						if(controlMode == "REQUIRED" && 
						   m_validationLevel > 0)
						{//Required fields must be validated if m_validation is set at all

							status = validate(msg, f);

							if(status.isError())
							{
								lastError = status;
								errorList.append("\n   ").append(lastError.get());
							}
						}
						else if(controlMode == "OPTIONAL" && 
								m_validationLevel > 1)
						{//Optional fields must be validated if m_validation is set greater than 1

							status = validate(msg, f);

							if(status.isError())
							{
								lastError = status;
								errorList.append("\n   ").append(lastError.get());
							}
						}
						
						//moving on the the next field
						fieldIndex++;
						controlMode = fields.get(fieldIndex).getMode();
						controlName = fields.get(fieldIndex).getName();
						if(controlMode == "CONTROL" && controlName == "ARRAY-END")
						{
							arrayEndIndex = fieldIndex ;
						}
					}//finished with the array control loop

					//moving on the the next object, resetting field index
					if(arrayIndex < arraySize)
					{//there are still more objects, reset the field index to the start of the array
						fieldIndex = arrayStartIndex;
						controlMode = fields.get(fieldIndex).getMode();
						controlName = fields.get(fieldIndex).getName();
					}
					else
					{//no more objects, reset the field index to the end of the array
						fieldIndex = arrayEndIndex;
						controlMode = fields.get(fieldIndex).getMode();
						controlName = fields.get(fieldIndex).getName();
					}
				}//done with all of the objects in the message
			}

		}//done validating the current field template
	}//done running through all of the field templates

	if(m_validationLevel > 2)
	{//No junk fields if validation is set to strict
		MessageFieldIterator& iter = msg.getFieldIterator();
		while(iter.hasNext())
		{//iterating through all the fields in the message
			const Field& field = iter.next();
			bool found = false;
			for(size_t i=0; i<fields.size(); i++)
			{//now look for a matching field in the template fields
				std::string fieldName = field.getName();
				std::string templateName = fields.get(i).getName();
				if(fieldName == templateName)
				{
					found = true;
					break;
				}
			}
			if(!found)
			{
				std::ostringstream err;
				err << "Message contains user-defined field " << field.getName() << ", which is disallowed under the current validation settings";
				GMSEC_ERROR << err.str().c_str();
				status.set(MIST_ERROR, NON_ALLOWED_FIELD, err.str().c_str());
				lastError = status;
				errorList.append("\n   ").append(lastError.get());
			}
		}
	}
	if(status.isError())
	{
		std::string err = msg.getSubject();
		err.append(": Validation Failed.");
		err.append("\n   ").append(lastError.get());
		return Status(MIST_ERROR, MESSAGE_FAILED_VALIDATION, err.c_str());
	}
	else if(lastError.isError())
	{
		std::string err = msg.getSubject();
		err.append(": Validation Failed.");
		err.append("\n   ").append(lastError.get());
		return Status(MIST_ERROR, MESSAGE_FAILED_VALIDATION, err.c_str());
	}
	else
	{
		GMSEC_INFO << msg.getSubject() << ": Validation Passed.";
		return status;
	}
}


Status InternalSpecification::validate(const Message& msg, const FieldTemplate& ftmp)
{
	Status status;

	//first convert the header field template into an actual field we can compare with
	std::auto_ptr<Field> tmpField(ftmp.toField());

	std::string mode = ftmp.getMode();
	std::string type = ftmp.getType();
	bool unset = false;

	std::string cumulativeError;

	if(type=="UNSET")
	{
		unset = true;
	}

	//now look for a field in the message with a matching name
	try
	{
		const Field* msgField = msg.getField(ftmp.getName());
		
		if(msgField)
		{//we found a matching field!

			if(!unset)
			{
				if(msgField->getType() != tmpField->getType())
				{//the field types need to match in order for validation to succeed
					std::stringstream err;
					err << msgField->getName()
						<< " has incorrect field type of " << getTypeStr(msgField->getType())
						<< ", expected type " << getTypeStr(tmpField->getType())
						<< ".  Field Description: " << ftmp.getDescription();
					//GMSEC_ERROR << err.str().c_str();
					status.set(MIST_ERROR, INCORRECT_FIELD_TYPE, err.str().c_str());
					cumulativeError.append("\n      ").append(status.get());
				}
			}

			std::string testValue = ftmp.getValue();
			if(testValue != "")
			{//if the template's value isn't blank, the message field's value needs to be validated as well

				//for simplicity's sake, we're comparing the values as strings, since we can pretty much turn any type into a string
				std::string msgValue = getValue(msg, msgField->getName());

				if(msgValue != testValue)
				{
					std::stringstream err;
					err << msgField->getName()
						<< " has incorrect value of " << msgValue
						<< ", expected a value of " << testValue
						<< ".  Field Description: " << ftmp.getDescription();
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
				err << tmpField->getName() 
					<< " is a required field, but is missing from message.  "
					<< "Field Description: " << ftmp.getDescription();
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
			err << tmpField->getName() 
				<< " is a required field, but is missing from message.  "
				<< "Field Description: " << ftmp.getDescription();
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


std::string InternalSpecification::registerTemplate(const Message& msg)
{
	std::string typeDef;
	std::string subTypeDef;

	//when searching for a match in the directory, we want to start schema level indicated by the user
	GMSEC_I64 schemaLevel = m_schemaLevel;
	std::string levelName;

	//This is the schemaID minus the spec version. We'll look this name up in the directory
	//to see if a more specific definition is available
	std::string name;

	//this will be marked true once we've figured out the most accurate definition for the message we can find
	bool found = false;

	while(!found && schemaLevel>=0)
	{
		for(size_t i=0; i<m_directory.size(); i++)
		{//now searching through the directory for the proper header definition
			std::string def = m_directory.get(i).getID();
			if(def == "HEADER" && m_directory.get(i).getLevel() == schemaLevel)
			{//found a matching header, now grab the default definition from it
				levelName = m_directory.get(i).getLevelName();
				typeDef = m_directory.get(i).getFirstDefinition();
				subTypeDef = m_directory.get(i).getNextDefinition();
			
				try
				{//add type and subtype to name
					name = msg.getStringField(typeDef.c_str()).getValue();
					name.append(".");
					name.append(msg.getStringField(subTypeDef.c_str()).getValue());
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
		for(size_t i=0; i<m_templates.size(); i++)
		{//now searching through list of schemas
			std::string id = m_templates.get(i)->getID();
			if(id == schemaID)
			{//found it!
				GMSEC_INFO << "Subject " << msg.getSubject() << " registered with Schema ID " << schemaID.c_str();
				m_registry.insert(std::pair<std::string, std::string>(msg.getSubject(), schemaID));
				return schemaID;
			}
		}
		schemaLevel--;
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

	while(oldName != name)
	{//so long as the name is updated it means there may be a more specific definition available
		oldName = name;
		for(size_t i=0;i<m_directory.size(); i++)
		{//now looking through directory
			std::string def = m_directory.get(i).getID();
			if(def == name && m_directory.get(i).getLevel() == (size_t) schemaLevel)
			{//looks like a more specific definition exists
			 //rebuild the name with the new definition
				found = true;
				try
				{
					std::string newName = msg.getStringField(m_directory.get(i).getFirstDefinition()).getValue();
					while(m_directory.get(i).hasNextDefinition())
					{
						newName.append(".");
						newName.append(msg.getStringField(m_directory.get(i).getNextDefinition()).getValue());
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


void InternalSpecification::load()
{
	if(!m_registry.empty())
	{//spec was previously loaded, wipe registry so messages can be reclassified with new templates
		m_registry.clear();
	}

	if(!m_templates.empty())
	{//spec was previously loaded, remove templates and reload
		for(MessageTemplateList::const_iterator it = m_templates.begin(); it != m_templates.end(); ++it)
		{
			delete *it;
		}
		m_templates.clear();
	}

	if(!m_headers.empty())
	{
		m_headers.clear();
	}

	if(!m_directory.empty())
	{
		m_directory.clear();
	}

	Status                 status;	  //This var will get overridden by the current status, error or non-error
	Status				   lastError; //This is the last error found by InternalSpecification::Load
									  //This is to avoid potential scenarios where an error status 
									  //is overridden by a non-error status
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
	StringUtil::stringFormat(major_version_str, sizeof(major_version_str), "%04u", getVersion()/100);
	StringUtil::stringFormat(minor_version_str, sizeof(minor_version_str), "%02u", getVersion()-((getVersion()/100)*100));

	//
	// Load paths
	//
	if (m_basePath.empty())
	{
		std::string shared_object_path;

		if (FileUtil::getCurrentSharedObjectPath(shared_object_path) == false)
		{
			if(status.isError())
			{
				throw Exception(status);
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
				if(status.isError())
				{
					throw Exception(status);
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
		load_dir = m_basePath;
	}

	if (!status.isError())
	{
		load_dir += FileUtil::PATH_SEPARATOR;
		load_dir += major_version_str;
		load_dir += ".";
		load_dir += minor_version_str;

		if (FileUtil::getFilesInDirectory(load_dir, schema_files) == false)
		{
			if(status.isError())
			{
				throw Exception(status);
			}

			std::string err_msg = "Could not list files in template directory ";
			err_msg += load_dir;

			status.set(MIST_ERROR, TEMPLATE_DIR_ERROR, err_msg.c_str());

			GMSEC_ERROR << err_msg.c_str();

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
			if (m_directory.empty())
			{
				status.set(MIST_ERROR, TEMPLATE_DIR_ERROR, "Template Directory information failed to load");
				throw Exception(status);
			}
		}
	}

	if(lastError.isError())
	{
		throw Exception(lastError);
	}
	else if(status.isError())
	{
		throw Exception(status);
	}
}


Status InternalSpecification::parser(tinyxml2::XMLElement* xmlSchema,
                                     std::string& schemaID,
                                     gmsec::api::util::DataList<FieldTemplate>& schemaFields)
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
			m_directory.add(schema);

		}//done running through the child elements

		//the directory must have at minimum a definition for a Level-0 HEADER
		bool found = false;
		for(size_t i=0; i<m_directory.size(); i++)
		{
			std::string name = m_directory.get(i).getID();
			if(name == "HEADER" && m_directory.get(i).getLevel() == 0)
			{
				found = true;
			}
		}

		if(!found)
		{
			status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, "DIRECTORY is missing definition for LEVEL-0 HEADER");
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

				if(StringUtil::stringEquals(fieldTemplate.getMode(), "CONTROL") && 
				   StringUtil::stringEquals(fieldTemplate.getName(), "ARRAY-START"))
				{//encountered ARRAY-START, set array control flag
					arrayControlActive++;
				}
				else if(StringUtil::stringEquals(fieldTemplate.getMode(), "CONTROL") && 
						StringUtil::stringEquals(fieldTemplate.getName(), "ARRAY-END"))
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
			else if(StringUtil::stringEquals(attribute->Name(), "VALUE"))
			{
				fieldTemplate.setValue(attribute->Value());
			}
			else if(StringUtil::stringEquals(attribute->Name(), "TYPE"))
			{
				fieldTemplate.setType(attribute->Value());
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

		if(StringUtil::stringEquals(fieldTemplate.getName(), ""))
		{//throw an error if f_name wasn't parsed
			oss.clear();
			oss.str("");
			oss << schemaID << ":  field template is missing NAME attribute";
			status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
			GMSEC_ERROR << status.get();
		}

		if(StringUtil::stringEquals(fieldTemplate.getMode(), ""))
		{//throw an error if f_mode wasn't parsed
			oss.clear();
			oss.str("");
			oss << schemaID << ": the field template " << fieldTemplate.getName() << " is missing MODE attribute";
			status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
			GMSEC_ERROR << status.get();
		}

		//TODO(MAV): Make parser check to make sure f_type is one of the following:
		//CHAR, BIN, BOOL, I8, I16, I32, I64, U8, U16, U32, U64, F32, F64, STRING

		//check to make sure all attributes are accounted for
		if(StringUtil::stringEquals(fieldTemplate.getMode(), "REQUIRED") || 
		   StringUtil::stringEquals(fieldTemplate.getMode(), "OPTIONAL"))
		{//checking attributes for regular field templates
			if(StringUtil::stringEquals(fieldTemplate.getClass(), ""))
			{
				oss.clear();
				oss.str("");
				oss << schemaID << ": the field template " << fieldTemplate.getName() << " is missing FIELD_CLASS attribute";
				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
				GMSEC_ERROR << status.get();
			}
			if(StringUtil::stringEquals(fieldTemplate.getType(), ""))
			{
				oss.clear();
				oss.str("");
				oss << schemaID << ": the field template " << fieldTemplate.getName() << " is missing TYPE attribute";
				status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
				GMSEC_ERROR << status.get();
			}
		}//done checking attributes for regular fields

		if(StringUtil::stringEquals(fieldTemplate.getMode(), "CONTROL"))
		{//checking control fields to make sure all attributes are present
			if(StringUtil::stringEquals(fieldTemplate.getName(), "ARRAY-START"))
			{
				if(StringUtil::stringEquals(fieldTemplate.getValue(), ""))
				{
					oss.clear();
					oss.str("");
					oss << schemaID << ": the control field " << fieldTemplate.getName() << " is missing VALUE attribute";
					status.set(MIST_ERROR, SCHEMA_FAILED_TO_PARSE, oss.str().c_str());
					GMSEC_ERROR << status.get();
				}
				if(StringUtil::stringEquals(fieldTemplate.getSize(), ""))
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
		schemaFields.add(fieldTemplate);
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


size_t InternalSpecification::getIntValue(const Message& msg, const char* name) const
{
	size_t value;
	Field::FieldType type;
	
	try
	{
		type = msg.getFieldType(name);
	}
	catch(Exception &e)
	{
		throw Exception(e);
	}

	//determine what type of value the field has, then grab the value
	if(type == Field::I16_TYPE)
	{
		I16Field field = msg.getI16Field(name);
		value = field.getValue();
	}
	else if(type == Field::I32_TYPE)
	{
		I32Field field = msg.getI32Field(name);
		value = field.getValue();
	}
	else if(type == Field::I64_TYPE)
	{
		I64Field field = msg.getI64Field(name);
		value = field.getValue();
	}
	else if(type == Field::I8_TYPE)
	{
		I8Field field = msg.getI8Field(name);
		value = field.getValue();
	}
	else if(type == Field::U16_TYPE)
	{
		U16Field field = msg.getU16Field(name);
		value = field.getValue();
	}
	else if(type == Field::U32_TYPE)
	{
		U32Field field = msg.getU32Field(name);
		value = field.getValue();
	}
	else if(type == Field::U64_TYPE)
	{
		U64Field field = msg.getU64Field(name);
		value = field.getValue();
	}
	else if(type == Field::U8_TYPE)
	{
		U8Field field = msg.getU8Field(name);
		value = field.getValue();
	}
	else
	{
		GMSEC_ERROR << "Size of array control returned an unrecognized type.";
		return 0;
	}

	return value;
}


gmsec::api::util::DataList<FieldTemplate> InternalSpecification::prepHeaders(const char* schemaID)
{
	std::string id = schemaID;
	std::string typeDef;
	std::string subTypeDef;

	//the major and minor revision isn't in the schema's name in the directory
	//so we need to trim these two parts off first
	id = id.substr(8,id.length()-8);

	for(size_t i=0; i<m_directory.size(); i++)
	{//look up schemaID in the directory
		std::string def = m_directory.get(i).getID();
		if(def.find(id) != std::string::npos)
		{//found it
			//the common headers only contain the first two definitions, so we only need the first two
			typeDef = m_directory.get(i).getFirstDefinition();
			subTypeDef = m_directory.get(i).getNextDefinition();
			break;
		}
	}

	if(typeDef == "")
	{//we couldn't find the schemaID in the directory, which means it isn't explicitly defined
	 //we'll instead search for a more generic definition for the schema
		std::string genID = id.substr(0, id.find_last_of("."));
		GMSEC_DEBUG << id.c_str() << " is not explicitly defined in directory, using definition " << genID.c_str();
		for(size_t i=0; i<m_directory.size(); i++)
		{
			std::string def = m_directory.get(i).getID();
			if(def == genID)
			{
				typeDef = m_directory.get(i).getFirstDefinition();
				subTypeDef = m_directory.get(i).getNextDefinition();
				break;
			}
		}
	}

	if(typeDef == "")
	{//We still don't have a definition for the schema
	 //that's ok, we'll just default to the generic one defined by HEADER
		std::string genID = id.substr(0, id.find_last_of("."));
		GMSEC_DEBUG << genID.c_str() << " is not explicitly defined in the directory, defaulting to generic schema definition";
		for(size_t i=0; i<m_directory.size(); i++)
		{
			std::string def = m_directory.get(i).getID();
			if(def == "HEADER")
			{
				typeDef = m_directory.get(i).getFirstDefinition();
				subTypeDef = m_directory.get(i).getNextDefinition();
				break;
			}
		}
	}

	gmsec::api::util::DataList<FieldTemplate> fields;

	//schema ID layout:
	//Major-Version.Minor-Version.Message-Type.Message-SubType.Additional-Types...
	//remember we have trimmed off the major and minor versions as the directory doesn't reference them

	//3rd element of the schemaID (Level-ID)
	size_t index1 = id.find(".");

	//4th element of schemaID (Message-Type)
	size_t index2 = id.find(".", index1+1);
	std::string type = id.substr(index1+1, index2-index1-1);

	//5th element of schemaID (Message-SubType)
	size_t index3 = id.find(".", index2+1);
	std::string subType;
	if(index3 == std::string::npos)
	{
		index3 = id.size() - 1;
		subType = id.substr(index2+1, index3-index2);
	}
	else
	{
		subType = id.substr(index2+1, index3-index2-1);
	}

	for(size_t i=0; i<m_headers.size(); i++)
	{
		fields.add(m_headers.get(i));
		std::string name = fields.get(i).getName();

		if(name == typeDef)
		{
			fields.get(i).setValue(type.c_str());
		}
		else if(name == subTypeDef)
		{
			fields.get(i).setValue(subType.c_str());
		}
	}

	return fields;
}


void InternalSpecification::resetSchemaIndex()
{	
	m_schemaIndex = 0;
}


const char* InternalSpecification::nextID()
{
	if(m_schemaIndex < m_templates.size())
	{
		size_t i = m_schemaIndex;
		m_schemaIndex++;
		return m_templates.get(i)->getID();
	}
	else
	{
		throw Exception(MIST_ERROR, INDEX_OUT_OF_RANGE, "next() has reached end of list");
	}
}


bool InternalSpecification::hasNextID() const
{
	if(m_schemaIndex < m_templates.size())
	{
		return true;
	}
	else
	{
		return false;
	}
}


SchemaIDIterator& InternalSpecification::getSchemaIDIterator()
{
	m_iterator.m_iter->reset();
	return m_iterator;
}


gmsec::api::util::DataList<std::string> InternalSpecification::splitString(const char* str)
{
	gmsec::api::util::DataList<std::string> list;
	size_t startIndex = 0;
	size_t endIndex;
	std::string s = str;
	while(s.find(".", startIndex) != std::string::npos)
	{
		endIndex = s.find(".", startIndex);
		list.add(s.substr(startIndex, endIndex));
		startIndex = endIndex + 1;
	}
	return list;
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
		load();
	}
	else
	{
		throw Exception(MIST_ERROR, INVALID_CONFIG, "Supplied specification version is not valid");
	}
}


MessageTemplate InternalSpecification::findTemplate(const char* schemaID)
{
	if (!schemaID || std::string(schemaID).empty())
	{
		throw Exception(MIST_ERROR, TEMPLATE_ID_DOES_NOT_EXIST, "Schema ID cannot be null or contain an empty string");
	}

	for(MessageTemplateList::const_iterator it = m_templates.begin(); it != m_templates.end(); ++it)
	{
		const MessageTemplate* temp = *it;

		if(StringUtil::stringEqualsIgnoreCase(temp->getID(), schemaID))
		{
			return *temp;
		}
	}

	//A message template was not returned, which means there wasn't a matching one in the list
	std::string err = "SchemaID \"";
	err.append(schemaID);
	err.append("\" could not be found in list of available schema.");
	throw Exception(MIST_ERROR, TEMPLATE_ID_DOES_NOT_EXIST, err.c_str());
}


const char* InternalSpecification::getTemplateXML(const char* subject, const char* schemaID)
{
	m_templateXML =  findTemplate(schemaID).toXML(subject);
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


const gmsec::api::util::DataList<SchemaTemplate>& InternalSpecification::getDirectory() const
{
	return m_directory;
}