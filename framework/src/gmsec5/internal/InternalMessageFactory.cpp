/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalMessageFactory.cpp
 *
 * @brief This file contains the internal interface for the MessageFactory.
 */

#include <gmsec5/internal/InternalMessageFactory.h>

#include <gmsec5/internal/ComplianceValidator.h>
#include <gmsec5/internal/FieldTemplate.h>
#include <gmsec5/internal/InternalConnection.h>
#include <gmsec5/internal/MessageAggregationToolkit.h>
#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/MessageTemplate.h>
#include <gmsec5/internal/SpecificationBuddy.h>
#include <gmsec5/internal/StringUtil.h>

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5/Config.h>
#include <gmsec5/ConfigOptions.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/MessageValidator.h>

#include <gmsec5/field/Field.h>

#include <gmsec5/util/Log.h>
#include <gmsec5/util/StdUniquePtr.h>

#include <map>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


InternalMessageFactory::InternalMessageFactory()
	: m_msgConfig(),
	  m_spec(new Specification()),
	  m_validateMsgs(false),
	  m_standardFields(),
	  m_customMessageValidator(0),
	  m_ceeMessageValidator(),
	  m_mtCache()
{
}


InternalMessageFactory::InternalMessageFactory(const Config& config)
	: m_msgConfig(),
	  m_spec(new Specification(config)),
	  m_validateMsgs(false),
	  m_standardFields(),
	  m_customMessageValidator(0),
	  m_ceeMessageValidator(),
	  m_mtCache()
{
	const char* validationOption[] = { GMSEC_MSG_CONTENT_VALIDATE,
	                                   GMSEC_MSG_CONTENT_VALIDATE_ALL,
	                                   GMSEC_MSG_CONTENT_VALIDATE_RECV,
	                                   GMSEC_MSG_CONTENT_VALIDATE_SEND };

	for (size_t i = 0; i < sizeof(validationOption) / sizeof(const char*); ++i)
	{
		const char* value = config.getValue( validationOption[i] );

		if (value)
		{
			m_validateMsgs = StringUtil::stringEqualsIgnoreCase(value, "true");
			break;
		}
	}

	// Determine message-related configuration options from the given connection config
	{
		Message          tmp;
		InternalMessage& internalMsg = MessageBuddy::getInternal(tmp);

		const char* name    = 0;
		const char* value   = 0;
		bool        hasNext = config.getFirst(name, value);

		while (hasNext)
		{
			if (internalMsg.processConfigValue(name, value))
			{
				m_msgConfig.addValue(StringUtil::stringToUpper(name).c_str(), value);
			}

			hasNext = config.getNext(name, value);
		}

		if (!m_msgConfig.getValue(GMSEC_MESSAGE_SPEC_VERSION, NULL))
		{
			std::ostringstream oss;
			oss << getSpecification().getVersion();
			m_msgConfig.addValue(GMSEC_MESSAGE_SPEC_VERSION, oss.str().c_str());
		}
		if (!m_msgConfig.getValue(GMSEC_SCHEMA_LEVEL, NULL))
		{
			std::ostringstream oss;
			oss << static_cast<int>( getSpecification().getSchemaLevel() );
			m_msgConfig.addValue(GMSEC_SCHEMA_LEVEL, oss.str().c_str());
		}
	}
}


InternalMessageFactory::~InternalMessageFactory()
{
	clearStandardFields();

	for (MessageTemplateCache::iterator it = m_mtCache.begin(); it != m_mtCache.end(); ++it)
	{
		delete it->second;
	}
	m_mtCache.clear();
}


void InternalMessageFactory::setStandardFields(const List<Field*>& standardFields)
{
	clearStandardFields();

	AutoTicket lock(m_mutex);   // ensure the following is thread-safe

	for (List<Field*>::const_iterator it = standardFields.begin(); it != standardFields.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			m_standardFields.push_back(InternalField::makeFieldCopy(*field));
		}
		else
		{
			GMSEC_WARNING << "Field within List of standard fields is NULL";
		}
	}
}


void InternalMessageFactory::clearStandardFields()
{
	AutoTicket lock(m_mutex);   // ensure the following is thread-safe

	for (List<Field*>::iterator it = m_standardFields.begin(); it != m_standardFields.end(); ++it)
	{
		delete *it;
	}

	m_standardFields.clear();
}


void InternalMessageFactory::setMessageConfig(const Config& msgConfig)
{
	AutoTicket lock(m_mutex);   // ensure the following is thread-safe

	m_msgConfig = msgConfig;
}


Message InternalMessageFactory::createMessage() const
{
	Message msg;

	AutoTicket lock(m_mutex);   // ensure the following is thread-safe

	msg.setConfig( m_msgConfig );
	MessageBuddy::getInternal(msg).setVersion( m_spec->getVersion() );
	MessageBuddy::getInternal(msg).setSchemaLevel( static_cast<Specification::SchemaLevel>(m_spec->getSchemaLevel()) );

	for (List<Field*>::const_iterator it = m_standardFields.begin(); it != m_standardFields.end(); ++it)
	{
		const Field* field = *it;

		msg.addField(*field);
	}

	if (m_customMessageValidator != NULL)
	{
		msg.registerMessageValidator(m_customMessageValidator);
	}

	return msg;
}


Message InternalMessageFactory::createMessage(const char* schemaID)
{
	if (schemaID == NULL || *schemaID == '\0')
	{
		throw GmsecException(MSG_ERROR, UNKNOWN_MSG_TYPE, "SchemaID cannot be NULL, nor be an empty string");
	}

	AutoTicket lock(m_mutex);   // ensure the following is thread-safe

	Message msg;

	MessageBuddy::getInternal(msg).setTemplate( createMessageTemplate(schemaID) );
	MessageBuddy::getInternal(msg).setConfig( m_msgConfig );
	MessageBuddy::getInternal(msg).setVersion( m_spec->getVersion() );
	MessageBuddy::getInternal(msg).setSchemaLevel( static_cast<Specification::SchemaLevel>(m_spec->getSchemaLevel()) );

	for (List<Field*>::const_iterator it = m_standardFields.begin(); it != m_standardFields.end(); ++it)
	{
		const Field* field = *it;

		msg.addField(*field);
	}

	// Normalize the message for the specification in use
	MessageBuddy::getInternal(msg).init();

	if (m_customMessageValidator != NULL)
	{
		msg.registerMessageValidator(m_customMessageValidator);
	}

	return msg;
}


Message* InternalMessageFactory::newMessage() const
{
	Message* msg = new Message();

	AutoTicket lock(m_mutex);   // ensure the following is thread-safe

	MessageBuddy::getInternal(*msg).setConfig( m_msgConfig );
	MessageBuddy::getInternal(*msg).setTemplate(0);
	MessageBuddy::getInternal(*msg).setVersion( m_spec->getVersion() );
	MessageBuddy::getInternal(*msg).setSchemaLevel( static_cast<Specification::SchemaLevel>(m_spec->getSchemaLevel()) );

	if (m_customMessageValidator != NULL)
	{
		msg->registerMessageValidator(m_customMessageValidator);
	}

	return msg;
}


const Config& InternalMessageFactory::getMessageConfig() const
{
	return m_msgConfig;
}


Message InternalMessageFactory::fromData(const char* data, DataType type)
{
	Message msg;

	MessageBuddy::getInternal(msg).processData(data, type);

	// Merge the connection configuration options (supplied to the MessageFactory) into
	// the message configuration (if any). The connection configuration options will
	// trump over anything provided by the message.
	mergeConfig(msg, m_msgConfig);

	// TODO: Should we force the version? Message may have different version.
	MessageBuddy::getInternal(msg).setVersion( m_spec->getVersion() );
	MessageBuddy::getInternal(msg).setSchemaLevel( static_cast<Specification::SchemaLevel>(m_spec->getSchemaLevel()) );

	// Attempt to deduce the appropriate message template for the ingested message
	addMessageTemplate(msg);

	// Normalize the message for the specification in use
	MessageBuddy::getInternal(msg).init();

	if (m_customMessageValidator != NULL)
	{
		msg.registerMessageValidator(m_customMessageValidator);
	}

	return msg;
}


const Specification& InternalMessageFactory::getSpecification() const
{
	return *m_spec.get();
}


void InternalMessageFactory::registerMessageValidator(MessageValidator* validator)
{
	if (!validator)
	{
		throw GmsecException(MSG_FACTORY_ERROR, UNINITIALIZED_OBJECT, "The MessageValidator cannot be null.");
	}

	m_customMessageValidator = validator;
}


void InternalMessageFactory::registerMessageValidator(GMSEC_MessageValidator* validator)
{
	if (validator == NULL || *validator == NULL)
	{
		throw GmsecException(MSG_FACTORY_ERROR, UNINITIALIZED_OBJECT, "MessageValidator cannot be null.");
	}

	m_ceeMessageValidator.reset(new CustomMessageValidator(validator));

	registerMessageValidator(m_ceeMessageValidator.get());
}


void InternalMessageFactory::addMessageTemplate(Message& msg)
{
	if (MessageAggregationToolkit::isAggregatedMsg(&msg))
	{
		// ignore message; it is an aggregated message
		return;
	}

	try
	{
		AutoTicket lock(m_mutex);   // ensure the following is thread-safe

		std::string schemaID = deduceSchemaID(msg);

		MessageBuddy::getInternal(msg).setTemplate( createMessageTemplate(schemaID) );
	}
	catch (const GmsecException& e)
	{
		// Could not deduce/create message template; if we are validating messages
		// pass this error on now, as opposed to later (when/if an attempt is made
		// to send the message).
		if (m_validateMsgs)
		{
			throw e;
		}
	}
}


void InternalMessageFactory::identifyTrackingFields(Message& msg)
{
	//Accessor for the message template is in the internal interface for Message
	const MessageTemplate* mt = MessageBuddy::getInternal(msg).getTemplate();
	if (mt)
	{
#if 1
		MessageFieldIterator& iter = msg.getFieldIterator();

		while (iter.hasNext())
		{//iterate through each field in the message and check if it is a tracking field
			Field& field = const_cast<Field&>(iter.next());

			//for field arrays, elements of the field name are delimited by . and an index number (e.g. SUBSCRIPTION.n.NAME-PATTERN)
			std::vector<std::string> nameElements = StringUtil::split(field.getName(), '.');

			const MessageTemplate::FieldTemplateList& list = mt->getFieldTemplates();

			for (MessageTemplate::FieldTemplateList::const_iterator it = list.begin(); it != list.end(); ++it)
			{//iterate through the field templates to match a template to our message field
				FieldTemplate* ft = *it;

				if (ft->getName() == nameElements[0] && ft->getMode() == "TRACKING")
				{//match found, regular tracking field
					FieldBuddy::getInternal(field).isTracking(true);
					break;
				}
				else if (ft->getName().find(nameElements[0]) != std::string::npos && ft->getMode() == "CONTROL" && ft->hasChildren())
				{//match found, tracking field is nested in an array, need to match all elements in the arrayed name to prevent mismatch

					bool matched = false;

					MessageTemplate::FieldTemplateList children;
					ft->getAllChildren(children);

					for (MessageTemplate::FieldTemplateList::const_iterator it2 = children.begin(); it2 != children.end(); ++it2)
					{//check each child of the field array for a matching field name

						FieldTemplate* child = *it2;

						std::vector<std::string> templateElements = StringUtil::split(child->getModifiedName(), '.');

						//elements in the msg field name are matched to the respective elements in the field template name
						for (size_t i = 2; i < nameElements.size(); i+=2)
						{//indexes are the odd-numbered elements and can be skipped
							if (i < templateElements.size() && templateElements[i] == nameElements[i])
							{
								matched = true;
							}
							else
							{
								matched = false;
								break;
							}
						}

						if (matched)
						{//all elements match
							FieldBuddy::getInternal(field).isTracking(true);
							break;
						}
					}

					if (matched) break;
				}
			}
		}
#else
		//get the field templates from the message template, we will iterate through them and mark tracking fields
		const MessageTemplate::FieldTemplateList& list = mt->getFieldTemplates();

		for (MessageTemplate::FieldTemplateList::const_iterator it = list.begin(); it != list.end(); ++it)
		{
			FieldTemplate* ft = *it;

			if (ft->getMode() == "TRACKING" && msg.hasField(ft->getName().c_str()))
			{//tracking field template matches a field in the message (e.g. NODE), mark it as tracking
				FieldBuddy::getInternal(*msg.getField(ft->getName().c_str())).isTracking(true);
			}
			else if (ft->getMode() == "CONTROL" && ft->hasChildren())
			{//we've encountered a container field that defines an indexed array of fields (e.g. SUBSCRIPTION.n.NAME-PATTERN)
			 //check the contained field templates for any tracking fields.

				bool matched = false;

				MessageTemplate::FieldTemplateList children;
				ft->getAllChildren(children);				

				for (MessageTemplate::FieldTemplateList::const_iterator it2 = children.begin(); it2 != children.end(); ++it2)
				{
					FieldTemplate* child = *it2;

					if (child->getMode() == "TRACKING" && msg.hasField(child->getName().c_str()))
					{//we've found the first field in an indexed array (e.g. SUBSCRIPTION.1.NAME-PATTERN)
						
						FieldBuddy::getInternal(*msg.getField(child->getName().c_str())).isTracking(true);

						//we do not necessarily know how many elements are in the array, so we need to ensure we account for them all

						//elements in the array
						std::vector<std::string> nameElements = StringUtil::split(child->getName(),".1.");

						//TODO MAV:
						//Any kind of string surgery on the fieldnames to make this part work won't yeld any significant improvement over the original code block
						//a basic string is not a good enough container to handle a field names with multiple, incrementing index numbers embedded in it.
					}
				}
			}

		}
#endif
	}
}


std::string InternalMessageFactory::deduceSchemaID(const Message& msg)
{
	std::string schemaID;
	std::string typeDef;    //name of the field holding the message type value
	std::string subTypeDef; //name of the field holding the message subtype value

	//start at the level defined by user
	int schemaLevel = static_cast<int>( m_spec->getSchemaLevel() );
	std::string levelName;

	const std::list<SchemaTemplate*>& directory = SpecificationBuddy::getInternal(*m_spec).getDirectory();

	//this will be marked true once we've figured out the most accurate definition for the message we can find
	bool found = false;

	while (!found && schemaLevel >= 0)
	{
		for (std::list<SchemaTemplate*>::const_iterator it = directory.begin(); it != directory.end(); ++it)
		{
			//now searching through the directory for the proper header definition
			const SchemaTemplate* temp = *it;

			if (StringUtil::stringEquals(temp->getID(), "HEADER") && temp->getLevel() == schemaLevel)
			{
				//found a matching header, now grab the default definition from it
				levelName = temp->getLevelName();
				std::list<std::string>::const_iterator index = temp->getDefinitions().begin();
				typeDef = (temp->getDefinitions().size() > 0 ? *index : "");
				if (index != temp->getDefinitions().end())
				{
					++index;
					subTypeDef = *index;
				}
				else
				{
					subTypeDef = "";
				}

				if (msg.hasField(typeDef.c_str()))
				{
					schemaID = msg.getStringValue(typeDef.c_str());
				}

				if (schemaID.empty() && msg.hasField(subTypeDef.c_str()))
				{
					schemaID = msg.getStringValue(subTypeDef.c_str());
				}
				else if (msg.hasField(subTypeDef.c_str()))
				{
					schemaID.append(".").append(msg.getStringValue(subTypeDef.c_str()));
				}
				break;
			}
		}

		//now we look for a more specific definition that matches up with the message
		//found = findDefinition(schemaLevel, schemaID, msg);
		std::string oldSchemaID;

		while (oldSchemaID != schemaID && !found)
		{
			//so long as the name is updated it means there may be a more specific definition available
			oldSchemaID = schemaID;

			for (std::list<SchemaTemplate*>::const_iterator it = directory.begin(); it != directory.end(); ++it)
			{
				//now looking through directory
				const SchemaTemplate* temp = *it;

				if ((schemaID == temp->getID()) && (temp->getLevel() == schemaLevel))
				{
					//looks like a more specific definition exists
					//rebuild the name with the new definition
					found = true;

					if (msg.hasField(temp->getDefinitions().begin()->c_str()))
					{
						//keep track of whether all of the definitions match within the message
						bool match = true;

						std::string newSchemaID;

						for (std::list<std::string>::const_iterator index = temp->getDefinitions().begin(); index != temp->getDefinitions().end(); ++index)
						{
							if (msg.hasField(index->c_str()))
							{
								if (!newSchemaID.empty()) newSchemaID.append(".");
								newSchemaID.append(msg.getStringValue(index->c_str()));
							}
							else
							{
								//The message contents doesn't match the more specific definition we found
								//so our definition is as specific it can get already, we're done here
								match = false;
								break;
							}
						}

						//if match is true then it means we have matched all values to fields in the message
						//that means we have ourselves a new definition that matches up with our message
						if (match) schemaID = newSchemaID;
					}
					break;
				}
			}
		}

		if (!found)
		{
			schemaLevel--;
		}
	}

	return schemaID;
}


MessageTemplate* InternalMessageFactory::createMessageTemplate(const std::string& schemaID)
{
	// note: before this method is called, ensure a Mutex is used to prevent concurrent access

	std::string id = m_schemaIdMap.getSchemaId(schemaID, m_spec->getVersion());

	MessageTemplate* newMsgTemp;

	// check if template for ID already exists in the cache
	MessageTemplateCache::const_iterator msgIt = m_mtCache.find(id);

	if (msgIt != m_mtCache.end())
	{
		// return a copy of the message template (because it is not thread-safe)
		newMsgTemp = new MessageTemplate( *(msgIt->second) );
	}
	else
	{
		const MessageTemplate& msgTemp = SpecificationBuddy::getInternal(*m_spec.get()).findTemplate(id.c_str());

		const MessageTemplate::FieldTemplateList& msgFieldTemps = msgTemp.getFieldTemplates();

		MessageTemplate::FieldTemplateList fields = SpecificationBuddy::getInternal(*m_spec.get()).prepHeaders(id.c_str());

		for (MessageTemplate::FieldTemplateList::const_iterator it = msgFieldTemps.begin(); it != msgFieldTemps.end(); ++it)
		{
			fields.push_back(new FieldTemplate(*(*it)));
		}

		// create message template with prepped field templates
		newMsgTemp = new MessageTemplate(id.c_str(), fields, msgTemp.getSchemaLevel());

		newMsgTemp->setDefinition( msgTemp.getDefinition() );
		newMsgTemp->setSubject( msgTemp.getSubjectElements() );

		// create a copy of the message template and add to the cache
		m_mtCache[id] = new MessageTemplate( *newMsgTemp );

		for (MessageTemplate::FieldTemplateList::iterator it = fields.begin(); it != fields.end(); ++it)
		{
			delete* it;
		}
	}

	return newMsgTemp;
}


Message::Kind InternalMessageFactory::deduceMessageKind(const char* schemaID, unsigned int version)
{
	if (!schemaID || std::string(schemaID).empty())
	{
		return Message::Kind::PUBLISH;
	}

	// Check if were provided with an alias for the schema ID, or if it needs to be translated to another
	// value for use with the given message specification version.
	std::string id = m_schemaIdMap.getSchemaId(schemaID, version);

	// The types of Schema IDs that the API (currently) supports.
	//
	// Schema ID Type 1: major.minor.schemaLevel.messagekind.messagetype.<optionalmessagesubtype>
	// Schema ID Type 2: messagekind.messagetype.<optionalmessagesubtype>

	std::vector<std::string> elements = StringUtil::split(id, '.');
	std::string kind;

	if (elements.size() >= 5)        // Type 1
	{
		kind = elements.at(3);
	}
	else if (elements.size() >= 2)   // Type 2
	{
		kind = elements.at(0);
	}

	if (StringUtil::stringEqualsIgnoreCase(kind.c_str(), "MSG"))
	{
		return Message::Kind::PUBLISH;
	}
	else if (StringUtil::stringEqualsIgnoreCase(kind.c_str(), "REQ"))
	{
		return Message::Kind::REQUEST;
	}
	else if (StringUtil::stringEqualsIgnoreCase(kind.c_str(), "RESP"))
	{
		return Message::Kind::REPLY;
	}

	std::ostringstream err;
	err << "MessageFactory::deduceMessageKind(): Unable to determine the Message Kind from schema ID '" << schemaID << "'";
	throw GmsecException(MSG_ERROR, UNKNOWN_MSG_TYPE, err.str().c_str());
}

void InternalMessageFactory::mergeConfig(Message& msg, const Config& config)
{
	AutoTicket lock(m_mutex);

	msg.getConfig().merge(config);
}
