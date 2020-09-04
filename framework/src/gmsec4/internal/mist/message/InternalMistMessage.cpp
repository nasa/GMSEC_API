/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec4/internal/mist/message/InternalMistMessage.h>

#include <gmsec4/internal/mist/MessageTemplate.h>
#include <gmsec4/internal/mist/FieldTemplate.h>
#include <gmsec4/internal/mist/SpecificationBuddy.h>
#include <gmsec4/internal/mist/InternalSpecification.h>

#include <gmsec4/internal/MessageBuddy.h>

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/mist/Specification.h>
#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/util/Log.h>

#include <limits>
#include <memory>
#include <sstream>

using namespace gmsec::api::internal;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


namespace gmsec
{
namespace api
{
namespace mist
{
namespace message
{
namespace internal
{

DataList<Field*> InternalMistMessage::m_standardFields;


InternalMistMessage::InternalMistMessage(const char* subject,
										 Message::MessageKind kind,
										 const char* schemaID,
										 const Specification& spec)
	: InternalMessage(subject, kind),
	  m_valid(false),
	  m_valueBuffer(),
	  m_spec(0),
	  m_template()
{
	std::auto_ptr<Specification> managedSpec(new Specification(spec));

	m_spec = managedSpec.get();

	registerTemplate(schemaID);

	managedSpec.release();

	init();
}


InternalMistMessage::InternalMistMessage(const char* subject,
										 Message::MessageKind kind,
										 const char* schemaID,
										 const gmsec::api::Config& config,
										 const Specification& spec)
	: InternalMessage(subject, kind, config),
	  m_valid(false),
	  m_valueBuffer(),
	  m_spec(0),
	  m_template()
{
	std::auto_ptr<Specification> managedSpec(new Specification(spec));

	m_spec = managedSpec.get();

	registerTemplate(schemaID);

	managedSpec.release();

	init();
}


InternalMistMessage::InternalMistMessage(const InternalMistMessage& other)
	: InternalMessage(other),
	  m_valid(false),
	  m_valueBuffer(),
	  m_spec(0),
	  m_template()
{
	std::auto_ptr<Specification> managedSpec(new Specification(*other.m_spec));

	m_spec = managedSpec.get();

	if(other.getTemplate().getID()!= NULL)
	{
		std::string id = other.getTemplate().getID();

		if(id != "")
		{
			m_template = MessageTemplate(other.getTemplate());
		}
	}

	managedSpec.release();

	init();
}


InternalMistMessage::InternalMistMessage(const Message& msg)
	: InternalMessage(msg.getSubject(), msg.getKind(), msg.getConfig()),
	  m_valid(false),
	  m_valueBuffer(),
	  m_spec(0),
	  m_template()
{
	convertMessage(msg, Config());
}


InternalMistMessage::InternalMistMessage(const Message& msg, const Config& specConfig)
	: InternalMessage(msg.getSubject(), msg.getKind(), msg.getConfig()),
	  m_valid(false),
	  m_valueBuffer(),
	  m_spec(0),
	  m_template()
{
	convertMessage(msg, specConfig);
}


InternalMistMessage::InternalMistMessage(const char* data)
	: InternalMessage(data),
	  m_valid(false),
	  m_valueBuffer(),
	  m_spec(0),
	  m_template()
{
	std::ostringstream oss;

	Config specCfg;
	try
	{
		unsigned int version = (unsigned int) getUnsignedIntegerValue("CONTENT-VERSION") * 100;

		//TODO: We need a better way to handle ISD versions (and not just here, but throughout MIST code).

		if (version <= mist::GMSEC_ISD_2014_00)
		{
			// 2014.00 is the oldest spec for which we have schemas/templates.
			GMSEC_DEBUG << "CONTENT-VERSION is " << version << "; defaulting to GMSEC_ISD_2014_00";

			oss << mist::GMSEC_ISD_2014_00;

			specCfg.addValue("gmsec-specification-version", oss.str().c_str());
			specCfg.addValue("gmsec-legacy-schema-files", "true");
		}
		else if (version <= mist::GMSEC_ISD_2016_00)
		{
			oss << mist::GMSEC_ISD_2016_00;

			specCfg.addValue("gmsec-specification-version", oss.str().c_str());
			specCfg.addValue("gmsec-legacy-schema-files", "true");
		}
		else if (version <= mist::GMSEC_ISD_2018_00)
		{
			oss << mist::GMSEC_ISD_2018_00;

			specCfg.addValue("gmsec-specification-version", oss.str().c_str());
			specCfg.addValue("gmsec-legacy-schema-files", "false");
		}
		else if (version != mist::GMSEC_ISD_2014_00 && version != mist::GMSEC_ISD_2016_00 && version != mist::GMSEC_ISD_2018_00)
		{
			GMSEC_WARNING << "Unknown value supplied with CONTENT-VERSION field (" << version << "); defaulting to GMSEC_ISD_CURRENT";

			oss << mist::GMSEC_ISD_CURRENT;

			specCfg.addValue("gmsec-specification-version", oss.str().c_str());
			specCfg.addValue("gmsec-legacy-schema-files", "false");
		}
	}
	catch (...)
	{
		GMSEC_WARNING << "CONTENT-VERSION field not available in data; unable to determine specification version. Defaulting to GMSEC_ISD_CURRENT.";

		oss << mist::GMSEC_ISD_CURRENT;

		specCfg.addValue("gmsec-specification-version", oss.str().c_str());
		specCfg.addValue("gmsec-legacy-schema-files", "false");
	}
	
	// If we have made it here, then we can finally instantiate our Specification object
	// but we guard it using an std::auto_ptr in case other anomalies occur.
	std::auto_ptr<Specification> managedSpec(new Specification(specCfg));

	m_spec = managedSpec.get();

	std::string schemaID = deduceSchemaID(*this);

	registerTemplate(schemaID.c_str());

	managedSpec.release();

	init();
}


InternalMistMessage::~InternalMistMessage()
{
	delete m_spec;
}


void InternalMistMessage::registerTemplate(const char* schemaID)
{
	try
	{
		std::string id = schemaID;   // note: schemaID has already been verified to be non-NULL

		const MessageTemplate& msgTemp = SpecificationBuddy::getInternal(*m_spec).findTemplate(id.c_str());
		const MessageTemplate::FieldTemplateList& msgFieldTemps = msgTemp.getFieldTemplates();

		//found the right template, now add the header fields
		MessageTemplate::FieldTemplateList fields = SpecificationBuddy::getInternal(*m_spec).prepHeaders(id.c_str());

		for (MessageTemplate::FieldTemplateList::const_iterator it = msgFieldTemps.begin(); it != msgFieldTemps.end(); ++it)
		{
			fields.push_back(new FieldTemplate(*(*it)));
		}

		m_template = MessageTemplate(id.c_str(), fields);

		for (MessageTemplate::FieldTemplateList::iterator it = fields.begin(); it != fields.end(); ++it)
		{
			delete *it;
		}
	}
	catch (const Exception& e)
	{
		GMSEC_ERROR << "Failed to create MistMessage: schemaID given is not valid";
		throw e;
	}
}


bool InternalMistMessage::isValid() const
{
	return m_valid;
}


void InternalMistMessage::validate()
{
	m_valid = true;
}


void InternalMistMessage::invalidate()
{
	m_valid = false;
}


void InternalMistMessage::setStandardFields(const DataList<Field*>& standardFields)
{
	destroyStandardFields();

	for (DataList<Field*>::const_iterator it = standardFields.begin(); it != standardFields.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			m_standardFields.push_back(InternalField::makeFieldCopy(*field));
		}
		else
		{
			GMSEC_WARNING << "Field within DataList of standard fields is NULL";
		}
	}
}


void InternalMistMessage::clearStandardFields()
{
	destroyStandardFields();
}


void InternalMistMessage::destroyStandardFields()
{
	for (DataList<Field*>::iterator it = m_standardFields.begin(); it != m_standardFields.end(); ++it)
	{
		delete *it;
	}

	m_standardFields.clear();
}


const char* InternalMistMessage::getSchemaID() const
{
	std::string id = m_template.getID();
	if(id == "")
	{
		GMSEC_WARNING << getSubject() << " does not have an attached schema template";
	}
	return m_template.getID();
}


const MessageTemplate& InternalMistMessage::getTemplate() const
{
	return m_template;
}


void InternalMistMessage::setValue(const char* fieldName, const char* value)
{
	if(!fieldName || std::string(fieldName).empty())
	{
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, "Field name cannot be NULL or empty string");
	}
	if(!value)
	{
		throw Exception(MSG_ERROR, INVALID_FIELD_VALUE, "Field value cannot be NULL");
	}

	const FieldTemplate* temp = NULL;

	try
	{//first find the template and see if it works
		temp = &(findFieldTemplate(fieldName));
	}
	catch (...)
	{//if fieldTemplate does not exist, add StringField to the message
		addField(fieldName, value);
		return;
	}

	const FieldTemplate fieldTemplate = *temp;

	//found the template, now determine the type
	std::string type = fieldTemplate.getType();

	if(type == "CHAR")
	{//we can convert string to char, but we'll only accept it if it's one character long
		try
		{
			GMSEC_CHAR chrValue = StringUtil::getValue<GMSEC_CHAR>(value);

			addField(fieldName, chrValue);
		}
		catch(...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_CHAR, but value \"" << value << "\" cannot be converted to GMSEC_CHAR";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type == "BOOLEAN" || type == "BOOL")
	{//We can accept 4 values here: "0", "1", "true", "false"
		try
		{
			if(StringUtil::stringEqualsIgnoreCase(value, "true"))
			{//the value is not a number, so we check to see if it is "true"
				addField(fieldName, true);
			}
			else if(StringUtil::stringEqualsIgnoreCase(value, "false"))
			{//not "true", check if it is "false"
				addField(fieldName, false);
			}
			else
			{
				GMSEC_U32 numValue = (GMSEC_U32) StringUtil::getValue<GMSEC_U32>(value);

				if (numValue <= 1)
				{
					addField(fieldName, numValue == 1);
				}
				else
				{//it is not 0 or 1
					std::ostringstream err;
					err << "Field template \"" << fieldName << "\" calls for field type GMSEC_BOOL, but value \"" << value << "\" cannot be converted to GMSEC_BOOL";
					throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
				}
			}
		}
		catch (...)
		{//this value is worthless, we can't do anything with it
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_BOOL, but value \"" << value << "\" cannot be converted to GMSEC_BOOL";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type == "I8")
	{
		try
		{
			GMSEC_I64 numValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value);

			if (numValue >= GMSEC_I64(std::numeric_limits<GMSEC_I8>::min()) &&
			    numValue <= GMSEC_I64(std::numeric_limits<GMSEC_I8>::max()))
			{
				addField(fieldName, (GMSEC_I8) numValue);
			}
			else
			{//int is too big to fit into I8, which can lead to undefined behavior
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I8, but value \"" << value << "\" is outside limits";
				throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I8, but value \"" << value << "\" cannot be converted to GMSEC_I8";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type == "I16")
	{
		try
		{
			GMSEC_I64 numValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value);

			// Handle special case.
			if (StringUtil::stringEqualsIgnoreCase(fieldName, "PROCESS-ID") && (numValue > std::numeric_limits<GMSEC_I16>::max()))
			{
				// Force PROCESS-ID value to a GMSEC_I16 value (which may make it negative)
				numValue = (GMSEC_I16) numValue;
			}

			if (numValue >= GMSEC_I64(std::numeric_limits<GMSEC_I16>::min()) &&
			    numValue <= GMSEC_I64(std::numeric_limits<GMSEC_I16>::max()))
			{
				addField(fieldName, (GMSEC_I16) numValue);
			}
			else
			{//int is too big to fit into I16, which can lead to undefined behavior
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I16, but value \"" << value << "\" is outside limits";
				throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I16, but value \"" << value << "\" cannot be converted to GMSEC_I16";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type == "I32")
	{
		try
		{
			GMSEC_I64 numValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value);

			if (numValue >= GMSEC_I64(std::numeric_limits<GMSEC_I32>::min()) &&
			    numValue <= GMSEC_I64(std::numeric_limits<GMSEC_I32>::max()))
			{
				addField(fieldName, (GMSEC_I32) numValue);
			}
			else
			{//int is too big to fit into I32, which can lead to undefined behavior
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I32, but value \"" << value << "\" is outside limits";
				throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I32, but value \"" << value << "\" cannot be converted to GMSEC_I32";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type == "I64")
	{
		try
		{
			GMSEC_I64 numValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value);
		
			addField(fieldName, numValue);
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I64, but value \"" << value << "\" cannot be converted to GMSEC_I64";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type == "U8")
	{
		if (std::string(value).find("-") != std::string::npos)
		{//we cannot allow negative numbers
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U64, but value \"" << value << "\" cannot be converted to GMSEC_U64";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}

		try
		{
			GMSEC_I64 numValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value);

			if (numValue >= GMSEC_I64(std::numeric_limits<GMSEC_U8>::min()) &&
			    numValue <= GMSEC_I64(std::numeric_limits<GMSEC_U8>::max()))
			{
				addField(fieldName, (GMSEC_U8) numValue);
			}
			else
			{//int is too big to fit into U8, which can lead to undefined behavior
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U8, but value \"" << value << "\" is outside limits";
				throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U8, but value \"" << value << "\" cannot be converted to GMSEC_U8";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type == "U16")
	{
		if (std::string(value).find("-") != std::string::npos)
		{//we cannot allow negative numbers
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U64, but value \"" << value << "\" cannot be converted to GMSEC_U64";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}

		try
		{
			GMSEC_I64 numValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value);

			if (numValue >= GMSEC_I64(std::numeric_limits<GMSEC_U16>::min()) &&
			    numValue <= GMSEC_I64(std::numeric_limits<GMSEC_U16>::max()))
			{
				addField(fieldName, (GMSEC_U16) numValue);
			}
			else
			{//int is too big to fit into U16, which can lead to undefined behavior
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U16, but value \"" << value << "\" is outside limits";
				throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U16, but value \"" << value << "\" cannot be converted to GMSEC_U16";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type == "U32")
	{
		try
		{
			GMSEC_I64 numValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value);

			// Handle special case.
			if (StringUtil::stringEqualsIgnoreCase(fieldName, "PROCESS-ID") && (numValue < 0))
			{
				// Assume we are translating a negative PROCESS-ID value.
				numValue = std::numeric_limits<GMSEC_I16>::max() + (std::numeric_limits<GMSEC_I16>::max() + numValue) + 2;
			}

			if (numValue < 0)
			{
				// we disallow negative numbers
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U32, but value \"" << value << "\" cannot be converted to GMSEC_U32";
				throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}

			if (numValue <= GMSEC_I64(std::numeric_limits<GMSEC_U32>::max()))
			{
				addField(fieldName, (GMSEC_U32) numValue);
			}
			else
			{
				//int is too big to fit into U32, which can lead to undefined behavior
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U32, but value \"" << value << "\" is outside limits";
				throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U32, but value \"" << value << "\" cannot be converted to GMSEC_U32";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type == "U64")
	{
		if (std::string(value).find("-") != std::string::npos)
		{//we cannot allow negative numbers
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U64, but value \"" << value << "\" cannot be converted to GMSEC_U64";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}

		try
		{
			GMSEC_U64 numValue = StringUtil::getValue<GMSEC_U64>(value);
	
			addField(fieldName, numValue);
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U64, but value \"" << value << "\" cannot be converted to GMSEC_U64";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type == "F32")
	{
		try
		{
			GMSEC_F32 fValue = StringUtil::getValue<GMSEC_F32>(value);
		
			addField(fieldName, fValue);
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_F32, but value \"" << value << "\" cannot be converted to GMSEC_F32";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type == "F64")
	{
		try
		{
			GMSEC_F64 fValue = StringUtil::getValue<GMSEC_F64>(value);

			addField(fieldName, fValue);
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_F64, but value \"" << value << "\" cannot be converted to GMSEC_F64";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type == "STRING")
	{//a string value can very easily be converted to a string value
		addField(fieldName, value);
	}
	else if(type == "UNSET")
	{//field template has a variable data type, meaning it can be whatever it wants
	 //today, it's a StringField
		addField(fieldName, value);
	}
	else
	{//any types not listed can't be converted
		std::ostringstream err;
		err << "Field template \"" << fieldName << "\" calls for field type " << type.c_str() << ", but value \"" << value << "\" cannot be converted to " << type.c_str();
		throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
	}
}


void InternalMistMessage::setValue(const char* fieldName, GMSEC_I64 value)
{
	if(!fieldName || std::string(fieldName).empty())
	{
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, "Field name cannot be NULL or empty string");
	}

	const FieldTemplate* temp = NULL;

	try
	{//first find the template and see if it works
		temp = &(findFieldTemplate(fieldName));
	}
	catch (...)
	{//if fieldTemplate does not exist, add StringField to the message
		addField(fieldName, value);
		return;
	}

	const FieldTemplate fieldTemplate = *temp;

	//field template found, now determine the type
	std::string type = fieldTemplate.getType();
	
	if(type == "BOOLEAN" || type == "BOOL")
	{//we'll only accept 0 or 1 as values
		if(value == 0)
		{
			addField(fieldName, false);
		}
		else if(value == 1)
		{
			addField(fieldName, true);
		}
		else
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_BOOL, but value " << value << " cannot be converted to GMSEC_BOOL";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type == "I8")
	{
		if(value >= GMSEC_I64(std::numeric_limits<GMSEC_I8>::min()) && 
		   value <= GMSEC_I64(std::numeric_limits<GMSEC_I8>::max()))
		{
			addField(fieldName, (GMSEC_I8) value);
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I8, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "I16")
	{
		// Handle special case.
		if (StringUtil::stringEqualsIgnoreCase(fieldName, "PROCESS-ID") && (value > std::numeric_limits<GMSEC_I16>::max()))
		{
			// Force PROCESS-ID value to a GMSEC_I16 value (which may make it negative)
			value = (GMSEC_I16) value;
		}

		if(value >= GMSEC_I64(std::numeric_limits<GMSEC_I16>::min()) && 
		   value <= GMSEC_I64(std::numeric_limits<GMSEC_I16>::max()))
		{
			addField(fieldName, (GMSEC_I16) value);
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I16, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "I32")
	{
		if(value >= GMSEC_I64(std::numeric_limits<GMSEC_I32>::min()) && 
		   value <= GMSEC_I64(std::numeric_limits<GMSEC_I32>::max()))
		{
			addField(fieldName, (GMSEC_I32) value);
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I32, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "I64")
	{
		addField(fieldName, (GMSEC_I64) value);
	}
	else if(type == "U8")
	{
		if(value >= 0 && 
		   value <= GMSEC_I64(std::numeric_limits<GMSEC_U8>::max()))
		{
			addField(fieldName, (GMSEC_U8) value);
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U8, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "U16")
	{
		if(value >= 0 && 
		   value <= GMSEC_I64(std::numeric_limits<GMSEC_U16>::max()))
		{
			addField(fieldName, (GMSEC_U16) value);
		}
		else
		{//Tharr value be overflowin', Captain!
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U16, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "U32")
	{
		// Handle special case.
		if (StringUtil::stringEqualsIgnoreCase(fieldName, "PROCESS-ID") && (value < 0))
		{
			// Assume we are translating a negative PROCESS-ID value.
			value = std::numeric_limits<GMSEC_I16>::max() + (std::numeric_limits<GMSEC_I16>::max() + value) + 2;
		}

		if (value >= 0 &&
		    value <= GMSEC_I64(std::numeric_limits<GMSEC_U32>::max()))
		{
			addField(fieldName, (GMSEC_U32) value);
		}
		else
		{
			//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U32, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "U64")
	{
		if(value >= 0)
		{
			addField(fieldName, (GMSEC_U64) value);
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U64, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "F32")
	{
		addField(fieldName, (GMSEC_F32) value);
	}
	else if(type == "F64")
	{
		addField(fieldName, (GMSEC_F64) value);
	}
	else if(type == "STRING")
	{
		std::ostringstream oss;
		oss << value;
		addField(fieldName, oss.str().c_str());
	}
	else if(type == "UNSET")
	{//field template has a variable data type, meaning it can be whatever it wants
	 //today, it's an I64Field
		addField(fieldName, (GMSEC_I64) value);
	}
	else
	{//any types not listed can't be converted
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type " << type.c_str() << ", but value " << value << " cannot be converted to " << type.c_str();
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
	}
}


void InternalMistMessage::setValue(const char* fieldName, GMSEC_F64 value)
{
	if(!fieldName || std::string(fieldName).empty())
	{
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, "Field name cannot be NULL or empty string");
	}

	const FieldTemplate* temp = NULL;

	try
	{//first find the template and see if it works
		temp = &(findFieldTemplate(fieldName));
	}
	catch (...)
	{//if fieldTemplate does not exist, add StringField to the message
		addField(fieldName, value);
		return;
	}

	const FieldTemplate fieldTemplate = *temp;

	std::string type = fieldTemplate.getType();
	
	if(type == "BOOLEAN" || type == "BOOL")
	{//we'll only accept 0 or 1 as values
		//TODO: Fix this. Comparing F64 to an integer!
		if(value == 0)
		{
			addField(fieldName, false);
		}
		else if(value == 1)
		{
			addField(fieldName, true);
		}
		else
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_BOOL, but value " << value << " cannot be converted to GMSEC_BOOL";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type == "I8")
	{
		if(value >= GMSEC_F64(std::numeric_limits<GMSEC_I8>::min()) && 
		   value <= GMSEC_F64(std::numeric_limits<GMSEC_I8>::max()))
		{
			GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_I8', possible loss of data";
			addField(fieldName, (GMSEC_I8) value);
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I8, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "I16")
	{
		// Handle special case.
		if (StringUtil::stringEqualsIgnoreCase(fieldName, "PROCESS-ID") && (value > std::numeric_limits<GMSEC_I16>::max()))
		{
			// Force PROCESS-ID value to a GMSEC_I16 value (which may make it negative)
			value = (GMSEC_I16) value;
		}

		if(value >= GMSEC_F64(std::numeric_limits<GMSEC_I16>::min()) && 
		   value <= GMSEC_F64(std::numeric_limits<GMSEC_I16>::max()))
		{
			GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_I16', possible loss of data";
			addField(fieldName, (GMSEC_I16) value);
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I16, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "I32")
	{
		if(value >= GMSEC_F64(std::numeric_limits<GMSEC_I32>::min()) && 
		   value <= GMSEC_F64(std::numeric_limits<GMSEC_I32>::max()))
		{
			GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_I32', possible loss of data";
			addField(fieldName, (GMSEC_I32) value);
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I32, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "I64")
	{
		if(value >= GMSEC_F64(std::numeric_limits<GMSEC_I64>::min()) && 
		   value <= GMSEC_F64(std::numeric_limits<GMSEC_I64>::max()))
		{
			GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_I64', possible loss of data";
			addField(fieldName, (GMSEC_I64) value);
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I64, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "U8")
	{
		if(value >= 0 && 
		   value <= GMSEC_F64(std::numeric_limits<GMSEC_U8>::max()))
		{
			GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_U8', possible loss of data";
			addField(fieldName, (GMSEC_U8) value);
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U8, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "U16")
	{
		if(value >= 0 && 
		   value <= GMSEC_F64(std::numeric_limits<GMSEC_U16>::max()))
		{
			GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_U16', possible loss of data";
			addField(fieldName, (GMSEC_U16) value);
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U16, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "U32")
	{
		// Handle special case.
		if (StringUtil::stringEqualsIgnoreCase(fieldName, "PROCESS-ID") && (value < 0))
		{
			// Assume we are translating a negative PROCESS-ID value.
			value = std::numeric_limits<GMSEC_I16>::max() + (std::numeric_limits<GMSEC_I16>::max() + value) + 2;
		}

		if (value >= 0 && 
		    value <= GMSEC_F64(std::numeric_limits<GMSEC_U32>::max()))
		{
			GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_U32', possible loss of data";
			addField(fieldName, (GMSEC_U32) value);
		}
		else
		{
			//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U32, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "U64")
	{
		if(value >= 0 && 
		   value <= GMSEC_F64(std::numeric_limits<GMSEC_U64>::max()))
		{
			GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_U64', possible loss of data";
			addField(fieldName, (GMSEC_U64) value);
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U64, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(type == "F32")
	{
		addField(fieldName, (GMSEC_F32) value);
	}
	else if(type == "F64")
	{
		addField(fieldName, (GMSEC_F64) value);
	}
	else if(type == "STRING")
	{
		std::ostringstream oss;
		oss << value;
		addField(fieldName, oss.str().c_str());
	}
	else if(type == "UNSET")
	{//field template has a variable data type, meaning it can be whatever it wants
	 //today, it's an F64Field
		addField(fieldName, value);
	}
	else
	{//any types not listed can't be converted
		std::ostringstream err;
		err << "Field template \"" << fieldName << "\" calls for field type " << type.c_str() << ", but value " << value << " cannot be converted to " << type.c_str();
		throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
	}
}


const FieldTemplate& InternalMistMessage::findFieldTemplate(const char* fieldName)
{
	//nested array controls have their placeholder value added to the end of the list
	std::list<std::string> placeHolders;

	size_t arrayControlActive = 0;

	const MessageTemplate::FieldTemplateList& msgFieldTemps = m_template.getFieldTemplates();

	for (MessageTemplate::FieldTemplateList::const_iterator ft = msgFieldTemps.begin(); ft != msgFieldTemps.end(); ++ft)
	{
		const FieldTemplate* temp = *ft;

		if (StringUtil::stringEqualsIgnoreCase(temp->getName().c_str(), "ARRAY-START") &&
		    StringUtil::stringEqualsIgnoreCase(temp->getMode().c_str(), "CONTROL"))
		{
			//we found an array control
			//grab the placeholder value for the array control
			arrayControlActive++;
			placeHolders.push_back(temp->getArrayControlValue());
		}
		else if (StringUtil::stringEqualsIgnoreCase(temp->getName().c_str(), "ARRAY-END") &&
				 StringUtil::stringEqualsIgnoreCase(temp->getMode().c_str(), "CONTROL"))
		{
			//we can remove the last last value since we're done with it
			arrayControlActive--;
		}

		if (arrayControlActive > 0)
		{
			//the names will need to be broken down into their elements and compared one by one, ignoring the index placeholders
			std::vector<std::string> fieldNameElements = StringUtil::split(fieldName, '.');
			std::vector<std::string> templateNameElements = StringUtil::split(temp->getName(), '.');
			bool found = false;

			//if they don't have the same amount of elements we can assume the names don't match
			if (fieldNameElements.size() == templateNameElements.size())
			{
				for (size_t i = 0; i < templateNameElements.size(); i++)
				{
					//compare each element of the field template's name at the specified index the field's element at the same index
					bool notPlaceHolder = true;

					for (std::list<std::string>::const_iterator placeHolder=placeHolders.begin(); placeHolder!=placeHolders.end(); ++placeHolder)
					{
						//check the current template element to make sure it isn't an index placeholder
						std::string value = *placeHolder;
						if (StringUtil::stringEquals(templateNameElements.at(i).c_str(), value.c_str()))
						{
							//it's an index placeholder, so the elements at this index won't be compared
							notPlaceHolder = false;
							break;
						}
					}

					if (notPlaceHolder)
					{
						//template element isn't an element, so compare the two elements at the index
						if (StringUtil::stringEquals(templateNameElements.at(i).c_str(), fieldNameElements.at(i).c_str()))
						{
							found = true;
						}
						else
						{
							found = false;
							break;
						}
					}
				}
			}
			if (found)
			{
				return *temp;
			}
		}
		else
		{
			//regular field
			if (StringUtil::stringEquals(temp->getModifiedName().c_str(), fieldName))
			{
				return *temp;
			}
		}
	}

	throw Exception(MIST_ERROR, FIELD_TEMPLATE_NOT_FOUND, "FieldTemplate not found in MessageTemplate");
}


std::string InternalMistMessage::buildSchemaID(Message::MessageKind kind, const char* type, const char* subtype, unsigned int version)
{
	std::string id;

	switch(kind)
	{
		case Message::PUBLISH:
			id.append("MSG");
			break;
		case Message::REQUEST:
			id.append("REQ");
			break;
		case Message::REPLY:
			id.append("RESP");
			break;
	}

	if(type && !std::string(type).empty())
	{
		id.append(".");
		id.append(type);
	}

	if(subtype && !std::string(subtype).empty())
	{
		id.append(".");
		id.append(subtype);
	}

	return id;
}


Specification InternalMistMessage::buildSpecification(unsigned int version)
{
	std::ostringstream versionStr;
	versionStr << version;

	Config config;
	config.addValue("GMSEC-SPECIFICATION-VERSION", versionStr.str().c_str());
	config.addValue("GMSEC-LEGACY-SCHEMA-FILES", "true");

	return Specification(config);
}


Message::MessageKind InternalMistMessage::findKind(const char* schemaID, unsigned int version)
{
	if (!schemaID || std::string(schemaID).empty())
	{
		throw Exception(MSG_ERROR, UNKNOWN_MSG_TYPE, "SchemaID cannot be NULL nor contain an empty string");
	}

	// The types of Schema IDs that the API (currently) supports.
	//
	// Schema ID Type 1: major.minor.schemaLevel.messagekind.messagetype.<optionalmessagesubtype>
	// Schema ID Type 2: messagekind.messagetype.<optionalmessagesubtype>

	std::vector<std::string> elements = StringUtil::split(schemaID, '.');
	std::string kind;

	if (elements.size() >= 5)        // Type 1
	{
		kind = elements.at(3);
	}
	else if (elements.size() >= 2)   // Type 2
	{
		kind = elements.at(0);
	}

	if(StringUtil::stringEqualsIgnoreCase(kind.c_str(), "MSG"))
	{
		return Message::PUBLISH;
	}
	else if(StringUtil::stringEqualsIgnoreCase(kind.c_str(), "REQ"))
	{
		return Message::REQUEST;
	}
	else if(StringUtil::stringEqualsIgnoreCase(kind.c_str(), "RESP"))
	{
		return Message::REPLY;
	}
	else
	{
		std::ostringstream err;
		err << "MistMessage::findKind(): unable to determine the MessageKind \"" << kind.c_str() << "\"";
		throw Exception(MSG_ERROR, UNKNOWN_MSG_TYPE, err.str().c_str());
	}
}


unsigned int InternalMistMessage::getSpecVersion() const
{
	return m_spec->getVersion();
}


void InternalMistMessage::setSpecVersion(unsigned int version)
{
	SpecificationBuddy::getInternal(*m_spec).setVersion(version);
}


void InternalMistMessage::init()
{	
	const MessageTemplate::FieldTemplateList& fieldTemplates = getTemplate().getFieldTemplates();

	for (MessageTemplate::FieldTemplateList::const_iterator it = fieldTemplates.begin(); it != fieldTemplates.end(); ++it)
	{
		FieldTemplate* temp = *it;

		if (temp->hasExplicitType() && temp->hasExplicitValue())
		{
			//the field template has an explicitly defined type and value, so we'll add the field to the message
			try
			{
				std::auto_ptr<Field> field(temp->toField(temp->getType()));

				addField(*(field.get()));
			}
			catch(...)
			{
				//ignore control fields, do nothing
			}
		}
	}

	// Add standard fields (if any) to the message
	//
	for (DataList<Field*>::const_iterator it = m_standardFields.begin(); it != m_standardFields.end(); ++it)
	{
		const Field* field = *it;

		addField(*field);
	}
}


std::string InternalMistMessage::deduceSchemaID(const InternalMessage& msg)
{
	std::string schemaID;

	//determine message type (MSG, REQ, RESP)
	std::string type;
	try
	{
		type = msg.getStringValue("MESSAGE-TYPE");
		schemaID.append(type);
	}
	catch(...)
	{
		throw Exception(MSG_ERROR, BAD_MSG_FORMAT, "MESSAGE-TYPE field not supplied, unable to determine message format");
	}

	//determine message subtype
	std::string subtype;
	try
	{
		subtype = msg.getStringValue("MESSAGE-SUBTYPE");
		schemaID.append(".").append(subtype);
	}
	catch(...)
	{
		throw Exception(MSG_ERROR, BAD_MSG_FORMAT, "MESSAGE-SUBTYPE field not supplied, unable to determine message format");
	}

	//determine if there is an additional subtype
	const std::list<SchemaTemplate>& directory = SpecificationBuddy::getInternal(*m_spec).getDirectory();

	for (std::list<SchemaTemplate>::const_iterator it = directory.begin(); it != directory.end(); ++it)
	{
		SchemaTemplate temp = *it;
		std::string name;
		name.append(type).append(".").append(subtype);

		if (StringUtil::stringEqualsIgnoreCase(name.c_str(), temp.getID()))
		{
			//we have the first two definitions already, we're interested in if there's a third definition
			temp.getFirstDefinition();
			temp.getNextDefinition();

			if (temp.hasNextDefinition())
			{
				schemaID.append(".").append(msg.getStringValue(temp.getNextDefinition()));
				break;
			}
		}
	}

	return schemaID;
}


void InternalMistMessage::convertMessage(const Message& msg, const Config& specConfig)
{
	std::auto_ptr<Specification> managedSpec(new Specification(specConfig));

	m_spec = managedSpec.get();

	std::string schemaID = deduceSchemaID(MessageBuddy::getInternal(msg));

	registerTemplate(schemaID.c_str());

	managedSpec.release();

	init();

	MessageFieldIterator& iter = msg.getFieldIterator();

	while (iter.hasNext())
	{
		const Field& field = iter.next();

		try
		{
			// Determine if this is a known field
			(void) findFieldTemplate(field.getName());

			// Set field (where conversion of field type may take place)
			setValue(field.getName(), field.getStringValue());
		}
		catch (...)
		{
			// Unknown field; add it directly as is (i.e. no conversion)
			addField(field);
		}
	}

	// copy meta-data (if any)
	getDetails().cloneValues(const_cast<InternalMessage&>(msg.getInternal()).getDetails());
}

} // namespace internal
} // namespace message
} // namespace mist
} // namespace api
} // namespace gmsec
