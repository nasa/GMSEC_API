/*
 * Copyright 2007-2021 United States Government as represented by the
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
#include <gmsec4/internal/Encoder.h>

#include <gmsec4/mist/Specification.h>
#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/StdUniquePtr.h>

#include <limits>
#include <sstream>

using namespace gmsec::api::internal;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::util;


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

// Static member declaration
DataList<Field*> InternalMistMessage::s_standardFields;
SchemaIdMapping  InternalMistMessage::s_schemaIdMap;


InternalMistMessage::InternalMistMessage(const char* subject,
										 Message::MessageKind kind,
										 const char* schemaID,
										 const Specification& spec)
	: InternalMessage(subject, kind),
	  m_valid(false),
	  m_valueBuffer(),
	  m_specVersion(spec.getVersion()),
	  m_template(0)
{
	registerTemplate(spec, schemaID);

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
	  m_specVersion(spec.getVersion()),
	  m_template(0)
{
	registerTemplate(spec, schemaID);

	init();
}


InternalMistMessage::InternalMistMessage(const InternalMistMessage& other)
	: InternalMessage(other),
	  m_valid(false),
	  m_valueBuffer(),
	  m_specVersion(other.getSpecVersion()),
	  m_template(0)
{
	if(other.getTemplate().getID()!= NULL)
	{
		std::string id = other.getTemplate().getID();

		if(id != "")
		{
			m_template.reset(new MessageTemplate(other.getTemplate()));
		}
	}

	init();
}


InternalMistMessage::InternalMistMessage(const Message& msg)
	: InternalMessage(msg.getSubject(), msg.getKind(), msg.getConfig()),
	  m_valid(false),
	  m_valueBuffer(),
	  m_specVersion(GMSEC_ISD_CURRENT),
	  m_template(0)
{
	convertMessage(msg, Config());
}


InternalMistMessage::InternalMistMessage(const Message& msg, const Config& specConfig)
	: InternalMessage(msg.getSubject(), msg.getKind(), msg.getConfig()),
	  m_valid(false),
	  m_valueBuffer(),
	  m_specVersion(GMSEC_ISD_CURRENT),
	  m_template(0)
{
	convertMessage(msg, specConfig);
}


InternalMistMessage::InternalMistMessage(const char* data)
	: InternalMessage(data),
	  m_valid(false),
	  m_valueBuffer(),
	  m_specVersion(GMSEC_ISD_CURRENT),
	  m_template(0)
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
		else if (version <= mist::GMSEC_ISD_2019_00)
		{
			oss << mist::GMSEC_ISD_2019_00;

			specCfg.addValue("gmsec-specification-version", oss.str().c_str());
			specCfg.addValue("gmsec-legacy-schema_files", "false");
		}
		else if (version <= mist::GMSEC_ISD_CURRENT)
		{
			oss << mist::GMSEC_ISD_CURRENT;

			specCfg.addValue("gmsec-specification-version", oss.str().c_str());
			specCfg.addValue("gmsec-legacy-schema_files", "false");
		}
		else
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
	
	Specification spec(specCfg);

	m_specVersion = spec.getVersion();

	std::string schemaID = deduceSchemaID(spec, *this);

	registerTemplate(spec, schemaID.c_str());

	init();
}


InternalMistMessage::InternalMistMessage(const Specification& spec, const char* data)
	: InternalMessage(data),
	  m_valid(false),
	  m_valueBuffer(),
	  m_specVersion(spec.getVersion()),
	  m_template(0)
{
	std::string schemaID = deduceSchemaID(spec, *this);

	registerTemplate(spec, schemaID.c_str());

	init();

	const MessageFieldIterator& iter = getFieldIterator();

	DataList<const Field*> fields;
	while (iter.hasNext())
	{
		const Field& field = iter.next();

		try {
			const FieldTemplate& ftmp = findFieldTemplate(field.getName());

			if (ftmp.hasExplicitType()) {
				// Found existing field with an explicit type; stow the field
				// and apply potentially new field type later after iterating
				// over existing fields. This will mitigate unexpected behavior
				// should the addition of a field cause a rollover to occur.
				fields.push_back(&field);
			}
		}
		catch (...) {
			// Ignore error
		}
	}

	for (DataList<const Field*>::iterator it = fields.begin(); it != fields.end(); ++it)
	{
		const Field* field = *it; 

		// Convert field, if necessary, for the given Specification.
		setValue(field->getName(), field->getStringValue(), (field->getType() != Field::BIN_TYPE ? true : false));
	}

	// If necessary, remove old-style field.
	if (spec.getVersion() >= GMSEC_ISD_2019_00)
	{
		(void) clearField("C2CX-SUBTYPE");
	}
	else if (spec.getVersion() < GMSEC_ISD_2018_00)
	{
		(void) clearField("SPECIFICATION");
	}
}


InternalMistMessage::~InternalMistMessage()
{
}


void InternalMistMessage::registerTemplate(const Specification& spec, const char* schemaID)
{
	// Check if were provided with an alias for the schema ID, or if it needs to be translated to another
	// value for use with the given message specification version.
	std::string id = s_schemaIdMap.getSchemaId(schemaID, spec.getVersion());

	try
	{
		const MessageTemplate& msgTemp = SpecificationBuddy::getInternal(spec).findTemplate(id.c_str());

		const MessageTemplate::FieldTemplateList& msgFieldTemps = msgTemp.getFieldTemplates();

		//found the right template, now add the header fields
		MessageTemplate::FieldTemplateList fields = SpecificationBuddy::getInternal(spec).prepHeaders(id.c_str());

		for (MessageTemplate::FieldTemplateList::const_iterator it = msgFieldTemps.begin(); it != msgFieldTemps.end(); ++it)
		{
			fields.push_back(new FieldTemplate(*(*it)));
		}

		m_template.reset(new MessageTemplate(id.c_str(), fields, msgTemp.getSchemaLevel()));

		for (MessageTemplate::FieldTemplateList::iterator it = fields.begin(); it != fields.end(); ++it)
		{
			delete *it;
		}
	}
	catch (const Exception& e)
	{
		GMSEC_ERROR << "Failed to create MistMessage: given schemaID is not valid";
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
			s_standardFields.push_back(InternalField::makeFieldCopy(*field));
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
	for (DataList<Field*>::iterator it = s_standardFields.begin(); it != s_standardFields.end(); ++it)
	{
		delete *it;
	}

	s_standardFields.clear();
}


const char* InternalMistMessage::getSchemaID() const
{
	if (std::string(m_template->getID()).empty())
	{
		GMSEC_WARNING << getSubject() << " does not have an attached schema template";
	}
	return m_template->getID();
}


const MessageTemplate& InternalMistMessage::getTemplate() const
{
	return *(m_template.get());
}


void InternalMistMessage::setValue(const char* fieldName, const char* value, bool convert)
{
	if(!fieldName || std::string(fieldName).empty())
	{
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, "Field name cannot be NULL or empty string");
	}
	if(!value)
	{
		throw Exception(MSG_ERROR, INVALID_FIELD_VALUE, "Field value cannot be NULL");
	}
	
	const FieldTemplate* fieldTemp = NULL;

	try
	{//first find the template and see if it works
		fieldTemp = &(findFieldTemplate(fieldName));
	}
	catch (...)
	{//if fieldTemplate does not exist, default to type given
		addField(fieldName, value);
		return;
	}

	//found the template, get the type
	const std::string& type = fieldTemp->getType();

	StdUniquePtr<Field> field;

	if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "CHAR"))
	{//we can convert string to char, but we'll only accept it if it's one character long
		try
		{
			GMSEC_CHAR chrValue = StringUtil::getValue<GMSEC_CHAR>(value);

			field.reset(new CharField(fieldName, chrValue));
		}
		catch(...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_CHAR, but value \"" << value << "\" cannot be converted to GMSEC_CHAR";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "BOOLEAN") || StringUtil::stringEqualsIgnoreCase(type.c_str(), "BOOL"))
	{//We can accept 4 values here: "0", "1", "true", "false"
		try
		{
			if(StringUtil::stringEqualsIgnoreCase(value, "true"))
			{//the value is not a number, so we check to see if it is "true"
				field.reset(new BooleanField(fieldName, true));
			}
			else if(StringUtil::stringEqualsIgnoreCase(value, "false"))
			{//not "true", check if it is "false"
				field.reset(new BooleanField(fieldName, false));
			}
			else
			{
				GMSEC_U32 numValue = (GMSEC_U32) StringUtil::getValue<GMSEC_U32>(value);

				if (numValue <= 1)
				{
					field.reset(new BooleanField(fieldName, numValue == 1));
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
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "I8"))
	{
		try
		{
			GMSEC_I64 numValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value);

			if (numValue >= GMSEC_I64(std::numeric_limits<GMSEC_I8>::min()) &&
			    numValue <= GMSEC_I64(std::numeric_limits<GMSEC_I8>::max()))
			{
				field.reset(new I8Field(fieldName, (GMSEC_I8) numValue));
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
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "I16"))
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
				field.reset(new I16Field(fieldName, (GMSEC_I16) numValue));
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
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "I32"))
	{
		try
		{
			GMSEC_I64 numValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value);

			if (numValue >= GMSEC_I64(std::numeric_limits<GMSEC_I32>::min()) &&
			    numValue <= GMSEC_I64(std::numeric_limits<GMSEC_I32>::max()))
			{
				field.reset(new I32Field(fieldName, (GMSEC_I32) numValue));
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
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "I64"))
	{
		try
		{
			GMSEC_I64 numValue = (GMSEC_I64) StringUtil::getValue<GMSEC_I64>(value);
		
			field.reset(new I64Field(fieldName, numValue));
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I64, but value \"" << value << "\" cannot be converted to GMSEC_I64";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "U8"))
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
				field.reset(new U8Field(fieldName, (GMSEC_U8) numValue));
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
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "U16"))
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
				field.reset(new U16Field(fieldName, (GMSEC_U16) numValue));
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
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "U32"))
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
				field.reset(new U32Field(fieldName, (GMSEC_U32) numValue));
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
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "U64"))
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
	
			field.reset(new U64Field(fieldName, (GMSEC_U64) numValue));
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U64, but value \"" << value << "\" cannot be converted to GMSEC_U64";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "F32"))
	{
		try
		{
			GMSEC_F32 fValue = StringUtil::getValue<GMSEC_F32>(value);
		
			field.reset(new F32Field(fieldName, fValue));
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_F32, but value \"" << value << "\" cannot be converted to GMSEC_F32";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "F64"))
	{
		try
		{
			GMSEC_F64 fValue = StringUtil::getValue<GMSEC_F64>(value);

			field.reset(new F64Field(fieldName, fValue));
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_F64, but value \"" << value << "\" cannot be converted to GMSEC_F64";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "STRING"))
	{//a string value can very easily be converted to a string value
		field.reset(new StringField(fieldName, value));
	}
	else if (StringUtil::stringEqualsIgnoreCase(type.c_str(), "BLOB") || StringUtil::stringEqualsIgnoreCase(type.c_str(), "BIN") || StringUtil::stringEqualsIgnoreCase(type.c_str(), "BINARY"))
	{
		StringUtil::Data blob;

		if (convert)
		{
			blob = StringUtil::string_toBinary(value);
		}
		else
		{
			blob = StringUtil::binaryString_toBinary(value);
		}

		field.reset(new BinaryField(fieldName, const_cast<GMSEC_BIN>(blob.data()), blob.length()));
	}
	else if(type.empty() || StringUtil::stringEqualsIgnoreCase(type.c_str(), "UNSET") || StringUtil::stringEqualsIgnoreCase(type.c_str(), "VARIABLE"))
	{
		GMSEC_WARNING << "setValue() is setting variable type field '" << fieldName << "' as a string field within the message";
		field.reset(new StringField(fieldName, value));
	}
	else
	{//any types not listed can't be converted
		std::ostringstream err;
		err << "Field template \"" << fieldName << "\" calls for field type " << type.c_str() << ", but value \"" << value << "\" cannot be converted to " << type.c_str();
		throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
	}

	field->isHeader(fieldTemp->isHeader());

	this->addField(*(field.release()), false);
}


void InternalMistMessage::setValue(const char* fieldName, GMSEC_I64 value)
{
	if(!fieldName || std::string(fieldName).empty())
	{
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, "Field name cannot be NULL or empty string");
	}

	const FieldTemplate* fieldTemp = NULL;

	try
	{//first find the template and see if it works
		fieldTemp = &(findFieldTemplate(fieldName));
	}
	catch (...)
	{//if fieldTemplate does not exist, default to type given
		addField(fieldName, value);
		return;
	}

	//found the template, get the type
	const std::string& type = fieldTemp->getType();

	StdUniquePtr<Field> field;

	if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "BOOLEAN") || StringUtil::stringEqualsIgnoreCase(type.c_str(), "BOOL"))
	{//we'll only accept 0 or 1 as values
		if(value == 0)
		{
			field.reset(new BooleanField(fieldName, false));
		}
		else if(value == 1)
		{
			field.reset(new BooleanField(fieldName, true));
		}
		else
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_BOOL, but value " << value << " cannot be converted to GMSEC_BOOL";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "I8"))
	{
		if(value >= GMSEC_I64(std::numeric_limits<GMSEC_I8>::min()) && 
		   value <= GMSEC_I64(std::numeric_limits<GMSEC_I8>::max()))
		{
			field.reset(new I8Field(fieldName, (GMSEC_I8) value));
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I8, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "I16"))
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
			field.reset(new I16Field(fieldName, (GMSEC_I16) value));
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I16, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "I32"))
	{
		if(value >= GMSEC_I64(std::numeric_limits<GMSEC_I32>::min()) && 
		   value <= GMSEC_I64(std::numeric_limits<GMSEC_I32>::max()))
		{
			field.reset(new I32Field(fieldName, (GMSEC_I32) value));
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I32, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "I64"))
	{
		field.reset(new I64Field(fieldName, value));
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "U8"))
	{
		if(value >= 0 && 
		   value <= GMSEC_I64(std::numeric_limits<GMSEC_U8>::max()))
		{
			field.reset(new U8Field(fieldName, (GMSEC_U8) value));
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U8, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "U16"))
	{
		if(value >= 0 && 
		   value <= GMSEC_I64(std::numeric_limits<GMSEC_U16>::max()))
		{
			field.reset(new U16Field(fieldName, (GMSEC_U16) value));
		}
		else
		{//Tharr value be overflowin', Captain!
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U16, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "U32"))
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
			field.reset(new U32Field(fieldName, (GMSEC_U32) value));
		}
		else
		{
			//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U32, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "U64"))
	{
		field.reset(new U64Field(fieldName, static_cast<GMSEC_U64>(value)));
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "F32"))
	{
		field.reset(new F32Field(fieldName, (GMSEC_F32) value));
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "F64"))
	{
		field.reset(new F64Field(fieldName, (GMSEC_F64) value));
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "STRING"))
	{
		std::ostringstream oss;
		oss << value;
		field.reset(new StringField(fieldName, oss.str().c_str()));
	}
	else if (StringUtil::stringEqualsIgnoreCase(type.c_str(), "BIN") || StringUtil::stringEqualsIgnoreCase(type.c_str(), "BINARY"))
	{
		try
		{
			StringUtil::Data blob = StringUtil::I64_toBinary(value);

			field.reset(new BinaryField(fieldName, const_cast<GMSEC_BIN>(blob.data()), blob.length()));
		}
		catch (...)
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_BIN, but value \"" << value << "\" cannot be converted to GMSEC_BIN";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(type.empty() || StringUtil::stringEqualsIgnoreCase(type.c_str(), "UNSET") || StringUtil::stringEqualsIgnoreCase(type.c_str(), "VARIABLE"))
	{
		GMSEC_WARNING << "setValue() is setting variable type field '" << fieldName << "' as an I64 field within the message";
		field.reset(new I64Field(fieldName, value));
	}
	else
	{//any types not listed can't be converted
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type " << type.c_str() << ", but value " << value << " cannot be converted to " << type.c_str();
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
	}

	field->isHeader(fieldTemp->isHeader());

	this->addField(*(field.release()), false);
}


void InternalMistMessage::setValue(const char* fieldName, GMSEC_F64 value)
{
	if(!fieldName || std::string(fieldName).empty())
	{
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, "Field name cannot be NULL or empty string");
	}

	const FieldTemplate* fieldTemp = NULL;

	try
	{//first find the template and see if it works
		fieldTemp = &(findFieldTemplate(fieldName));
	}
	catch (...)
	{//if fieldTemplate does not exist, default to type given
		addField(fieldName, value);
		return;
	}

	//found the template, get the type
	const std::string& type = fieldTemp->getType();

	StdUniquePtr<Field> field;

	if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "BOOLEAN") || StringUtil::stringEqualsIgnoreCase(type.c_str(), "BOOL"))
	{//we'll only accept 0 or 1 as values
		//TODO: Fix this. Comparing F64 to an integer!
		if(value == 0)
		{
			field.reset(new BooleanField(fieldName, false));
		}
		else if(value == 1)
		{
			field.reset(new BooleanField(fieldName, true));
		}
		else
		{
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_BOOL, but value " << value << " cannot be converted to GMSEC_BOOL";
			throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "I8"))
	{
		if(value >= GMSEC_F64(std::numeric_limits<GMSEC_I8>::min()) && 
		   value <= GMSEC_F64(std::numeric_limits<GMSEC_I8>::max()))
		{
			GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_I8', possible loss of data";
			field.reset(new I8Field(fieldName, (GMSEC_I8) value));
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I8, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "I16"))
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
			field.reset(new I16Field(fieldName, (GMSEC_I16) value));
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I16, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "I32"))
	{
		if(value >= GMSEC_F64(std::numeric_limits<GMSEC_I32>::min()) && 
		   value <= GMSEC_F64(std::numeric_limits<GMSEC_I32>::max()))
		{
			GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_I32', possible loss of data";
			field.reset(new I32Field(fieldName, (GMSEC_I32) value));
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I32, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "I64"))
	{
		if(value >= GMSEC_F64(std::numeric_limits<GMSEC_I64>::min()) && 
		   value <= GMSEC_F64(std::numeric_limits<GMSEC_I64>::max()))
		{
			GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_I64', possible loss of data";
			field.reset(new I64Field(fieldName, (GMSEC_I64) value));
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I64, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "U8"))
	{
		if(value >= 0 && 
		   value <= GMSEC_F64(std::numeric_limits<GMSEC_U8>::max()))
		{
			GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_U8', possible loss of data";
			field.reset(new U8Field(fieldName, (GMSEC_U8) value));
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U8, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "U16"))
	{
		if(value >= 0 && 
		   value <= GMSEC_F64(std::numeric_limits<GMSEC_U16>::max()))
		{
			GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_U16', possible loss of data";
			field.reset(new U16Field(fieldName, (GMSEC_U16) value));
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U16, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "U32"))
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
			field.reset(new U32Field(fieldName, (GMSEC_U32) value));
		}
		else
		{
			//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U32, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "U64"))
	{
		if(value >= 0 && 
		   value <= GMSEC_F64(std::numeric_limits<GMSEC_U64>::max()))
		{
			GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_U64', possible loss of data";
			field.reset(new U64Field(fieldName, (GMSEC_U64) value));
		}
		else
		{//can't cast because it's outside the limits
			std::ostringstream err;
			err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U64, but value " << value << " is outside limits";
			throw Exception(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
		}
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "F32"))
	{
		field.reset(new F32Field(fieldName, (GMSEC_F32) value));
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "F64"))
	{
		field.reset(new F64Field(fieldName, value));
	}
	else if(StringUtil::stringEqualsIgnoreCase(type.c_str(), "STRING"))
	{
		std::ostringstream oss;
		oss << value;
		field.reset(new StringField(fieldName, oss.str().c_str()));
	}
	else if (StringUtil::stringEqualsIgnoreCase(type.c_str(), "BIN") || StringUtil::stringEqualsIgnoreCase(type.c_str(), "BINARY"))
	{
		StringUtil::Data blob = StringUtil::F64_toBinary(value);

		field.reset(new BinaryField(fieldName, const_cast<GMSEC_BIN>(blob.data()), blob.length()));
	}
	else if(type.empty() || StringUtil::stringEqualsIgnoreCase(type.c_str(), "UNSET") || StringUtil::stringEqualsIgnoreCase(type.c_str(), "VARIABLE"))
	{
		GMSEC_WARNING << "setValue() is setting variable type field '" << fieldName << "' as an F64 field within the message";
		field.reset(new F64Field(fieldName, value));
	}
	else
	{//any types not listed can't be converted
		std::ostringstream err;
		err << "Field template \"" << fieldName << "\" calls for field type " << type.c_str() << ", but value " << value << " cannot be converted to " << type.c_str();
		throw Exception(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
	}

	field->isHeader(fieldTemp->isHeader());

	this->addField(*(field.release()), false);
}


const FieldTemplate& InternalMistMessage::findFieldTemplate(const char* fieldName)
{
	//nested array controls have their placeholder value added to the end of the list
	std::list<std::string> placeHolders;
	std::vector<std::string> elements = StringUtil::split(fieldName, '.');
	std::string prefixes;

	//recreate the field name without the index numbers (just the prefixes, the even numbered elements)
	for (size_t i = 0; i < elements.size(); ++i)
	{
		if (i % 2 == 0)
		{
			prefixes.append(elements.at(i));
		}
	}

	size_t arrayControlActive = 0;

	const MessageTemplate::FieldTemplateList& msgFieldTemps = m_template->getFieldTemplates();

	for (MessageTemplate::FieldTemplateList::const_iterator ft = msgFieldTemps.begin(); ft != msgFieldTemps.end(); ++ft)
	{
		FieldTemplate* temp = *ft;

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
		else if (StringUtil::stringEqualsIgnoreCase(temp->getMode().c_str(), "CONTROL") && temp->hasChildren())
		{
			if (StringUtil::stringEqualsIgnoreCase(elements.front().c_str(), temp->getPrefix().c_str()))
			{
				//found matching prefixes, field is a child of this container

				MessageTemplate::FieldTemplateList children;

				temp->getAllChildren(children);

				for (MessageTemplate::FieldTemplateList::const_iterator it = children.begin(); it != children.end(); ++it)
				{
					std::string name = (*it)->getModifiedName();
					std::vector<std::string> nameElements = StringUtil::split(name, '.');
					name = "";
					for (size_t i = 0; i < nameElements.size(); ++i)
					{
						if (i % 2 == 0)
						{
							name.append(nameElements.at(i));
						}
					}
					
					if (StringUtil::stringEqualsIgnoreCase(prefixes.c_str(), name.c_str()))
					{
						return *(*it);
					}
				}
			}
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

	// Check if were provided with an alias for the schema ID, or if it needs to be translated to another
	// value for use with the given message specification version.
	std::string id = s_schemaIdMap.getSchemaId(schemaID, version);

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
		err << "MistMessage::findKind(): unable to determine the MessageKind from schema ID '" << schemaID << "'";
		throw Exception(MSG_ERROR, UNKNOWN_MSG_TYPE, err.str().c_str());
	}
}


unsigned int InternalMistMessage::getSpecVersion() const
{
	return m_specVersion;
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
				StdUniquePtr<Field> field(temp->toField(temp->getType()));

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
	for (DataList<Field*>::const_iterator it = s_standardFields.begin(); it != s_standardFields.end(); ++it)
	{
		const Field* field = *it;

		try
		{
			const FieldTemplate& ftmp = findFieldTemplate(field->getName());

			// Make copy of the field, and identify whether it is a header field.
			Field* copy = InternalField::makeFieldCopy(*field);
			copy->isHeader(ftmp.isHeader());

			// Add standard field to this message, and ensure another copy is not made.
			this->addField(*copy, false);
		}
		catch (...)
		{
			// Standard field not in the templates; just add it as is to this message.
			this->addField(*field);
		}
	}
}


std::string InternalMistMessage::deduceSchemaID(const Specification& spec, const InternalMessage& msg)
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
		if (getSpecVersion() >= GMSEC_ISD_2019_00 && StringUtil::stringEqualsIgnoreCase(msg.getStringValue("MESSAGE-SUBTYPE"), "C2CX"))
		{
			subtype = msg.getStringValue("C2CX-SUBTYPE");
		}
		else
		{
			subtype = msg.getStringValue("MESSAGE-SUBTYPE");
		}
		schemaID.append(".").append(subtype);
	}
	catch(...)
	{
		throw Exception(MSG_ERROR, BAD_MSG_FORMAT, "MESSAGE-SUBTYPE field not supplied, unable to determine message format");
	}

	//determine if there is an additional subtype
	const std::list<SchemaTemplate>& directory = SpecificationBuddy::getInternal(spec).getDirectory();

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

	// When working with old spec versions, we may need to convert schema ID to old style that included ".C2CX"
	if (getSpecVersion() < GMSEC_ISD_2019_00 &&
	    (schemaID == "MSG.CFG" || schemaID == "MSG.CNTL" || schemaID == "MSG.DEV" || schemaID == "MSG.HB" || schemaID == "MSG.RSRC"))
	{
		schemaID.insert(3, ".C2CX");
	}

	return schemaID;
}


void InternalMistMessage::convertMessage(const Message& msg, const Config& specConfig)
{
	Specification spec(specConfig);

	m_specVersion = spec.getVersion();

	std::string schemaID = deduceSchemaID(spec, MessageBuddy::getInternal(msg));

	registerTemplate(spec, schemaID.c_str());

	init();

	const MessageFieldIterator& iter = msg.getFieldIterator();

	while (iter.hasNext())
	{
		const Field& field = iter.next();

		if (this->hasField(field.getName())) continue;

		try
		{
			// Determine if this is a known field
			(void) findFieldTemplate(field.getName());

			// Set field (where conversion of field type may take place)
			setValue(field.getName(), field.getStringValue(), false);
		}
		catch (...)
		{
			// Unknown field; add it directly as is (i.e. no conversion)
			addField(field);
		}
	}

	if (m_specVersion >= GMSEC_ISD_2019_00)
	{
		(void) clearField("C2CX-SUBTYPE");
	}
	else if (m_specVersion < GMSEC_ISD_2018_00)
	{
		(void) clearField("SPECIFICATION");
	}

	// copy meta-data (if any)
	getDetails().cloneValues(const_cast<InternalMessage&>(msg.getInternal()).getDetails());
}

} // namespace internal
} // namespace message
} // namespace mist
} // namespace api
} // namespace gmsec
