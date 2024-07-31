/*
 * Copyright 2007-2024 United States Government as represented by the
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

#include <gmsec5/internal/InternalMessage.h>

#include <gmsec5/internal/ComplianceValidator.h>
#include <gmsec5/internal/ConnectionInterface.h>
#include <gmsec5/internal/Encoder.h>
#include <gmsec5/internal/InternalConfig.h>
#include <gmsec5/internal/InternalMessageFieldIterator.h>
#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/MiddlewareInfo.h>
#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/internal/Subject.h>
#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5/ConfigOptions.h>
#include <gmsec5/Fields.h>
#include <gmsec5/GmsecException.h>

#include <gmsec5/util/Buffer.h>
#include <gmsec5/util/Log.h>

#include <tinyxml2.h>
#include <json.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

#include <cstring>   // for strlen()

#ifdef WIN32
    #ifdef max
    	#undef max
    #endif
    #ifdef min
    	#undef min
    #endif
#endif


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


static const char*  TREE_TYPE                = "tree";
static const char*  HASH_TYPE                = "hash";
static const size_t MSG_FLD_STORAGE_ROLLOVER = 50;


InternalMessage::InternalMessage(Message& parent)
	: m_parent(parent),
	  m_config(),
	  m_fields(MsgFieldMap::BINARY_TREE_MAP, MSG_FLD_STORAGE_ROLLOVER),
	  m_fieldIterator(*this),
	  m_details(new ValueMap()),
	  m_valid(false),
	  m_valueBuffer(),
	  m_template(),
	  m_msgVersion(GMSEC_MSG_SPEC_CURRENT),
	  m_msgSchemaLevel(Specification::SchemaLevel::LEVEL_0),
	  m_kind(Message::Kind::PUBLISH),
	  m_subject(),
	  m_responseTopic(),
	  m_xml(),
	  m_json(),
	  m_subjectSet(false),
	  m_kindSet(false),
	  m_showMsgConfig(false),
	  m_subjectElementValues(),
	  m_mwInfo(0),
	  m_complianceValidator(new ComplianceValidator(m_config))
{
}


InternalMessage::InternalMessage(Message& parent, const InternalMessage& other)
	: m_parent(parent),
	  m_config(other.m_config),
	  m_fields(other.m_fields.getStorageType(), other.m_fields.getRolloverLimit()),
	  m_fieldIterator(*this),
	  m_details(new ValueMap()),
	  m_valid(other.m_valid),
	  m_valueBuffer(),
	  m_template(),
	  m_msgVersion(other.m_msgVersion),
	  m_msgSchemaLevel(other.m_msgSchemaLevel),
	  m_kind(other.m_kind),
	  m_subject(other.m_subject),
	  m_responseTopic(other.m_responseTopic),
	  m_xml(),
	  m_json(),
	  m_subjectSet(other.m_subjectSet),
	  m_kindSet(other.m_kindSet),
	  m_showMsgConfig(other.m_showMsgConfig),
	  m_subjectElementValues(other.m_subjectElementValues),
	  m_mwInfo(other.m_mwInfo),
	  m_complianceValidator(new ComplianceValidator(*(other.m_complianceValidator.get())))
{
	processConfig(other.m_config);

	// copy fields (if any)
	other.copyFields(*this);

	// copy meta-data (if any)
	getDetails().cloneValues(other.getDetails());

	if (other.getTemplate())
	{
		m_template.reset( new MessageTemplate(*(other.getTemplate())) );
	}
}


InternalMessage::~InternalMessage()
{
	clearFields();
}


void InternalMessage::acknowledge()
{
	if (m_mwInfo.get() && m_mwInfo->m_mwConnIf != NULL)
	{
		m_mwInfo->m_mwConnIf->mwAcknowledge(m_mwInfo);
	}
}


void InternalMessage::destroy(Message*& msg)
{
	delete msg;
	msg = 0;
}


const char* InternalMessage::getSchemaID() const
{
	if (m_template.get() == NULL)
	{
		GMSEC_WARNING << "Message does not have a schema message template";
		return "";
	}

	if (std::string(m_template->getSchemaID()).empty())
	{
		GMSEC_WARNING << "Schema message template does not have an ID";
	}

	return m_template->getSchemaID();
}


Status InternalMessage::isCompliant() const
{
	return m_complianceValidator->validateMessage(m_parent);
}


void InternalMessage::registerMessageValidator(MessageValidator* validator)
{
    if (!validator)
    {
        throw GmsecException(MSG_ERROR, UNINITIALIZED_OBJECT, "The MessageValidator cannot be null.");
    }

	m_complianceValidator->registerMessageValidator(validator);
}


void InternalMessage::registerMessageValidator(GMSEC_MessageValidator* validator)
{
    if (validator == NULL || *validator == NULL)
    {
        throw GmsecException(MSG_ERROR, UNINITIALIZED_OBJECT, "MessageValidator cannot be null.");
    }

	m_complianceValidator->registerMessageValidator(validator);
}


bool InternalMessage::setFieldValue(const char* fieldName, const char* value, bool convert)
{
	if (!fieldName || std::string(fieldName).empty())
	{
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, "Field name cannot be NULL or empty string");
	}
	if (!value)
	{
		throw GmsecException(MSG_ERROR, INVALID_FIELD_VALUE, "Field value cannot be NULL");
	}

	const FieldTemplate* fieldTemp = NULL;

	try
	{
		// attempt to find the field template using the field name
		fieldTemp = &(findFieldTemplate(fieldName));
	}
	catch (...)
	{
		// ignore error; template not found
	}

	Field::Type type;
	bool isHeader = false;

	if (!fieldTemp || fieldTemp->getType().empty() || fieldTemp->getType() == "UNSET" || fieldTemp->getType() == "VARIABLE")
	{
		// if the template was not found, or has no discernable type, default to STRING
		type = Field::Type::STRING;
	}
	else
	{
		// use the type associated with the field template
		type = InternalField::lookupType( fieldTemp->getType().c_str() );
		isHeader = fieldTemp->isHeader();
	}

	StdUniquePtr<Field> field;

	switch (type)
	{
	case Field::Type::STRING:
		{
			field.reset(new StringField(fieldName, value, isHeader));
		}
		break;

	case Field::Type::I16:
		{
			GMSEC_I64 numValue;
			try
			{
				numValue = StringUtil::getValue<GMSEC_I64>(value);
			}
			catch (...)
			{
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I16, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}

			if (numValue >= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_I16>::min()) &&
			    numValue <= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_I16>::max()))
			{
				field.reset(new I16Field(fieldName, static_cast<GMSEC_I16>(numValue), isHeader));
			}
			else
			{//int is too big to fit into I16, which can lead to undefined behavior
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I16, but value \"" << value << "\" is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::U16:
		{
			if (std::string(value).find("-") != std::string::npos)
			{
				//we cannot allow negative numbers
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U64, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}

			GMSEC_I64 numValue;
			try
			{
				numValue = StringUtil::getValue<GMSEC_I64>(value);
			}
			catch (...)
			{
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U16, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}

			if (numValue >= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_U16>::min()) &&
			    numValue <= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_U16>::max()))
			{
				field.reset(new U16Field(fieldName, static_cast<GMSEC_U16>(numValue), isHeader));
			}
			else
			{//int is too big to fit into U16, which can lead to undefined behavior
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U16, but value \"" << value << "\" is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::I32:
		{
			GMSEC_I64 numValue;
			try
			{
				numValue = StringUtil::getValue<GMSEC_I64>(value);
			}
			catch (...)
			{
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I32, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}

			if (numValue >= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_I32>::min()) &&
			    numValue <= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_I32>::max()))
			{
				field.reset(new I32Field(fieldName, static_cast<GMSEC_I32>(numValue), isHeader));
			}
			else
			{//int is too big to fit into I32, which can lead to undefined behavior
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I32, but value \"" << value << "\" is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::U32:
		{
			GMSEC_I64 numValue;
			try
			{
				numValue = StringUtil::getValue<GMSEC_I64>(value);
			}
			catch (...)
			{
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U32, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}

			if (numValue < 0)
			{// we disallow negative numbers
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U32, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}

			if (numValue <= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_U32>::max()))
			{
				field.reset(new U32Field(fieldName, static_cast<GMSEC_U32>(numValue), isHeader));
			}
			else
			{//int is too big to fit into U32, which can lead to undefined behavior
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U32, but value \"" << value << "\" is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::F32:
		{
			try
			{
				GMSEC_F32 fValue = StringUtil::getValue<GMSEC_F32>(value);
		
				field.reset(new F32Field(fieldName, fValue, isHeader));
			}
			catch (...)
			{
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_F32, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}
		}
		break;

	case Field::Type::F64:
		{
			try
			{
				GMSEC_F64 fValue = StringUtil::getValue<GMSEC_F64>(value);

				field.reset(new F64Field(fieldName, fValue, isHeader));
			}
			catch (...)
			{
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_F64, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}
		}
		break;

	case Field::Type::CHAR:
		{
			// we can convert string to char, but only if the string is one character long
			try
			{
				GMSEC_CHAR chrValue = StringUtil::getValue<GMSEC_CHAR>(value);

				field.reset(new CharField(fieldName, chrValue, isHeader));
			}
			catch (...)
			{
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_CHAR, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}
		}
		break;

	case Field::Type::BOOL:
		{
			//We can accept 4 values here: "true", "false", "0", or "1"
			if (StringUtil::stringEqualsIgnoreCase(value, "true"))
			{
				field.reset(new BooleanField(fieldName, true, isHeader));
			}
			else if (StringUtil::stringEqualsIgnoreCase(value, "false"))
			{
				field.reset(new BooleanField(fieldName, false, isHeader));
			}
			else
			{
				GMSEC_U32 numValue;
				try
				{
					numValue = StringUtil::getValue<GMSEC_U32>(value);
				}
				catch (...)
				{//this value is worthless, we can't do anything with it
					std::ostringstream err;
					err << "Field template \"" << fieldName << "\" calls for field type GMSEC_BOOL, but value \"" << value << "\" cannot be converted";
					throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
				}

				if (numValue <= 1)
				{
					field.reset(new BooleanField(fieldName, numValue == 1, isHeader));
				}
				else
				{//it is not 0 or 1
					std::ostringstream err;
					err << "Field template \"" << fieldName << "\" calls for field type GMSEC_BOOL, but value \"" << value << "\" cannot be converted";
					throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
				}
			}
		}
		break;

	case Field::Type::BINARY:
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

			field.reset(new BinaryField(fieldName, const_cast<GMSEC_U8*>(blob.data()), blob.length(), isHeader));
		}
		break;

	case Field::Type::I8:
		{
			GMSEC_I64 numValue;
			try
			{
				numValue = StringUtil::getValue<GMSEC_I64>(value);
			}
			catch (...)
			{
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I8, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}

			if (numValue >= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_I8>::min()) &&
			    numValue <= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_I8>::max()))
			{
				field.reset(new I8Field(fieldName, static_cast<GMSEC_I8>(numValue), isHeader));
			}
			else
			{//int is too big to fit into I8, which can lead to undefined behavior
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I8, but value \"" << value << "\" is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::U8:
		{
			if (std::string(value).find("-") != std::string::npos)
			{//we cannot allow negative numbers
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U64, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}

			GMSEC_I64 numValue;
			try
			{
				numValue = StringUtil::getValue<GMSEC_I64>(value);
			}
			catch (...)
			{
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U8, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}

			if (numValue >= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_U8>::min()) &&
			    numValue <= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_U8>::max()))
			{
				field.reset(new U8Field(fieldName, static_cast<GMSEC_U8>(numValue), isHeader));
			}
			else
			{//int is too big to fit into U8, which can lead to undefined behavior
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U8, but value \"" << value << "\" is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::I64:
		{
			try
			{
				GMSEC_I64 numValue = StringUtil::getValue<GMSEC_I64>(value);
		
				field.reset(new I64Field(fieldName, numValue, isHeader));
			}
			catch (...)
			{
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I64, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}
		}
		break;

	case Field::Type::U64:
		{
			if (std::string(value).find("-") != std::string::npos)
			{
				//we cannot allow negative numbers
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U64, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}

			try
			{
				GMSEC_U64 numValue = StringUtil::getValue<GMSEC_U64>(value);
	
				field.reset(new U64Field(fieldName, numValue, isHeader));
			}
			catch (...)
			{
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U64, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}
		}
		break;
	}

	return this->addField(*(field.release()), false);
}


bool InternalMessage::setFieldValue(const char* fieldName, GMSEC_I16 value)
{
	return setFieldValue(fieldName, static_cast<GMSEC_I64>(value), Field::Type::I16);
}


bool InternalMessage::setFieldValue(const char* fieldName, GMSEC_U16 value)
{
	return setFieldValue(fieldName, static_cast<GMSEC_I64>(value), Field::Type::U16);
}


bool InternalMessage::setFieldValue(const char* fieldName, GMSEC_I32 value)
{
	return setFieldValue(fieldName, static_cast<GMSEC_I64>(value), Field::Type::I32);
}


bool InternalMessage::setFieldValue(const char* fieldName, GMSEC_U32 value)
{
	return setFieldValue(fieldName, static_cast<GMSEC_I64>(value), Field::Type::U32);
}


bool InternalMessage::setFieldValue(const char* fieldName, GMSEC_I64 value, Field::Type desiredFieldType)
{
	if (!fieldName || std::string(fieldName).empty())
	{
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, "Field name cannot be NULL or empty string");
	}

	const FieldTemplate* fieldTemp = NULL;

	try
	{
		// attempt to find the field template using the field name
		fieldTemp = &(findFieldTemplate(fieldName));
	}
	catch (...)
	{
		// ignore error; template not found
	}

	Field::Type type;
	bool isHeader = false;

	if (!fieldTemp || fieldTemp->getType().empty() || fieldTemp->getType() == "UNSET" || fieldTemp->getType() == "VARIABLE")
	{
		// if the template was not found, or has no discernable type, default to desired field type
		type = desiredFieldType;
	}
	else
	{
		// use the type associated with the field template
		type = InternalField::lookupType( fieldTemp->getType().c_str() );
		isHeader = fieldTemp->isHeader();
	}

	StdUniquePtr<Field> field;

	switch (type)
	{
	case Field::Type::STRING:
		{
			std::ostringstream oss;
			oss << value;
			field.reset(new StringField(fieldName, oss.str().c_str(), isHeader));
		}
		break;

	case Field::Type::I16:
		{
			if (value >= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_I16>::min()) && 
			    value <= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_I16>::max()))
			{
				field.reset(new I16Field(fieldName, static_cast<GMSEC_I16>(value), isHeader));
			}
			else
			{//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I16, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::U16:
		{
			if (value >= 0 && 
			    value <= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_U16>::max()))
			{
				field.reset(new U16Field(fieldName, static_cast<GMSEC_U16>(value), isHeader));
			}
			else
			{//Tharr value be overflowin', Captain!
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U16, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::I32:
		{
			if (value >= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_I32>::min()) && 
			    value <= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_I32>::max()))
			{
				field.reset(new I32Field(fieldName, static_cast<GMSEC_I32>(value), isHeader));
			}
			else
			{//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I32, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::U32:
		{
			if (value >= 0 &&
			    value <= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_U32>::max()))
			{
				field.reset(new U32Field(fieldName, static_cast<GMSEC_U32>(value), isHeader));
			}
			else
			{
				//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U32, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::F32:
		{
			field.reset(new F32Field(fieldName, static_cast<GMSEC_F32>(value), isHeader));
		}
		break;

	case Field::Type::F64:
		{
			field.reset(new F64Field(fieldName, static_cast<GMSEC_F64>(value), isHeader));
		}
		break;

	case Field::Type::BOOL:
		{//we'll only accept 0 or 1 as values
			if (value >= 0 && value <= 1)
			{
				field.reset(new BooleanField(fieldName, value == 1, isHeader));
			}
			else
			{
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_BOOL" << ", but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}
		}
		break;

	case Field::Type::BINARY:
		{
			try
			{
				StringUtil::Data blob = StringUtil::I64_toBinary(value);

				field.reset(new BinaryField(fieldName, const_cast<GMSEC_U8*>(blob.data()), blob.length(), isHeader));
			}
			catch (...)
			{
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U8*, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}
		}
		break;

	case Field::Type::I8:
		{
			if (value >= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_I8>::min()) && 
			    value <= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_I8>::max()))
			{
				field.reset(new I8Field(fieldName, static_cast<GMSEC_I8>(value), isHeader));
			}
			else
			{//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I8, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::U8:
		{
			if (value >= 0 && 
			    value <= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_U8>::max()))
			{
				field.reset(new U8Field(fieldName, static_cast<GMSEC_U8>(value), isHeader));
			}
			else
			{//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U8, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::I64:
		{
			field.reset(new I64Field(fieldName, value, isHeader));
		}
		break;

	case Field::Type::U64:
		{
			if (value >= 0)
			{
				field.reset(new U64Field(fieldName, static_cast<GMSEC_U64>(value), isHeader));
			}
			else
			{//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U64, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::CHAR:
		{
			if (value >= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_CHAR>::min()) && 
			    value <= static_cast<GMSEC_I64>(std::numeric_limits<GMSEC_CHAR>::max()))
			{
				field.reset(new CharField(fieldName, static_cast<GMSEC_CHAR>(value), isHeader));
			}
			else
			{//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_CHAR, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;
	}

	return this->addField(*(field.release()), false);
}


bool InternalMessage::setFieldValue(const char* fieldName, GMSEC_U64 value)
{
	return setFieldValue(fieldName, static_cast<GMSEC_F64>(value), Field::Type::U64);
}


bool InternalMessage::setFieldValue(const char* fieldName, GMSEC_F64 value, Field::Type desiredFieldType)
{
	if (!fieldName || std::string(fieldName).empty())
	{
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, "Field name cannot be NULL or empty string");
	}

	const FieldTemplate* fieldTemp = NULL;

	try
	{
		// attempt to find the field template using the field name
		fieldTemp = &(findFieldTemplate(fieldName));
	}
	catch (...)
	{
		// ignore error; template not found
	}

	Field::Type type;
	bool isHeader = false;

	if (!fieldTemp || fieldTemp->getType().empty() || fieldTemp->getType() == "UNSET" || fieldTemp->getType() == "VARIABLE")
	{
		// if the template was not found, or has no discernable type, default to desired field type
		type = desiredFieldType;
	}
	else
	{
		// use the type associated with the field template
		type = InternalField::lookupType( fieldTemp->getType().c_str() );
		isHeader = fieldTemp->isHeader();
	}

	StdUniquePtr<Field> field;

	switch (type)
	{
	case Field::Type::STRING:
		{
			std::ostringstream oss;
			oss << value;
			field.reset(new StringField(fieldName, oss.str().c_str(), isHeader));
		}
		break;

	case Field::Type::I16:
		{
			if (value >= static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_I16>::min()) && 
			    value <= static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_I16>::max()))
			{
				GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_I16', possible loss of data";
				field.reset(new I16Field(fieldName, static_cast<GMSEC_I16>(value), isHeader));
			}
			else
			{//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I16, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::U16:
		{
			if (value >= 0 && 
			    value <= static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_U16>::max()))
			{
				GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_U16', possible loss of data";
				field.reset(new U16Field(fieldName, static_cast<GMSEC_U16>(value), isHeader));
			}
			else
			{//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U16, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::I32:
		{
			if (value >= static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_I32>::min()) && 
			    value <= static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_I32>::max()))
			{
				GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_I32', possible loss of data";
				field.reset(new I32Field(fieldName, static_cast<GMSEC_I32>(value), isHeader));
			}
			else
			{//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I32, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::U32:
		{
			if (value >= 0 && 
			    value <= static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_U32>::max()))
			{
				GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_U32', possible loss of data";
				field.reset(new U32Field(fieldName, static_cast<GMSEC_U32>(value), isHeader));
			}
			else
			{
				//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U32, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::F32:
		{
			field.reset(new F32Field(fieldName, static_cast<GMSEC_F32>(value), isHeader));
		}
		break;

	case Field::Type::F64:
		{
			field.reset(new F64Field(fieldName, value, isHeader));
		}
		break;

	case Field::Type::BINARY:
		{
			StringUtil::Data blob = StringUtil::F64_toBinary(value);

			field.reset(new BinaryField(fieldName, const_cast<GMSEC_U8*>(blob.data()), blob.length(), isHeader));
		}
		break;

	case Field::Type::BOOL:
		{//we'll only accept 0 or 1 as values
			if (static_cast<GMSEC_I32>(value) == 0)
			{
				field.reset(new BooleanField(fieldName, false, isHeader));
			}
			else if (static_cast<GMSEC_I32>(value) == 1)
			{
				field.reset(new BooleanField(fieldName, true, isHeader));
			}
			else
			{
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_BOOL, but value \"" << value << "\" cannot be converted";
				throw GmsecException(MSG_ERROR, INVALID_TYPE_CONVERSION, err.str().c_str());
			}
		}
		break;

	case Field::Type::I8:
		{
			if (value >= static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_I8>::min()) && 
			    value <= static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_I8>::max()))
			{
				GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_I8', possible loss of data";
				field.reset(new I8Field(fieldName, static_cast<GMSEC_I8>(value), isHeader));
			}
			else
			{//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I8, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::U8:
		{
			if (value >= 0 && 
			    value <= static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_U8>::max()))
			{
				GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_U8', possible loss of data";
				field.reset(new U8Field(fieldName, static_cast<GMSEC_U8>(value), isHeader));
			}
			else
			{//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U8, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::I64:
		{
			if (value >= static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_I64>::min()) && 
			    value <= static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_I64>::max()))
			{
				GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_I64', possible loss of data";
				field.reset(new I64Field(fieldName, static_cast<GMSEC_I64>(value), isHeader));
			}
			else
			{//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_I64, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::U64:
		{
			if (value >= 0 && 
			    value <= static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_U64>::max()))
			{
				GMSEC_WARNING << "conversion from 'GMSEC_F64' to 'GMSEC_U64', possible loss of data";
				field.reset(new U64Field(fieldName, static_cast<GMSEC_U64>(value), isHeader));
			}
			else
			{//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_U64, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;

	case Field::Type::CHAR:
		{
			if (value >= static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_CHAR>::min()) && 
			    value <= static_cast<GMSEC_F64>(std::numeric_limits<GMSEC_CHAR>::max()))
			{
				field.reset(new CharField(fieldName, static_cast<GMSEC_CHAR>(value), isHeader));
			}
			else
			{//can't cast because it's outside the limits
				std::ostringstream err;
				err << "Field template \"" << fieldName << "\" calls for field type GMSEC_CHAR, but value " << value << " is outside limits";
				throw GmsecException(MSG_ERROR, VALUE_OUT_OF_RANGE, err.str().c_str());
			}
		}
		break;
	}

	return this->addField(*(field.release()), false);
}


void InternalMessage::setConfig(const Config& config)
{
	processConfig(config);

	m_complianceValidator->updateConfig(m_config);
}


Config& InternalMessage::getConfig()
{
	return m_config;
}


const Config& InternalMessage::getConfig() const
{
	return m_config;
}


void InternalMessage::setSubject(const char* subject)
{
	if (!subject || *subject == '\0')
	{
		throw GmsecException(MSG_ERROR, INVALID_SUBJECT_NAME, "Subject cannot be NULL, nor an empty string");
	}

	std::string result = Subject::isValid(subject);

	if (!result.empty())
	{
		std::ostringstream oss;
		oss << "Invalid message subject: " << subject << result;
		throw GmsecException(MSG_ERROR, INVALID_SUBJECT_NAME, oss.str().c_str());
	}

	m_subject    = subject;
	m_subjectSet = true;
}


const char* InternalMessage::getSubject() const
{
	if (!m_subjectSet)
	{
		m_subject = buildSubject();
	}

	return m_subject.c_str();
}


void InternalMessage::setSubjectElement(const char* name, const char* value)
{
	if (!name || std::string(name).empty())
	{
		throw GmsecException(MSG_ERROR, INVALID_SUBJECT_NAME, "Subject element name cannot be NULL or empty string");
	}

	std::string newValue;

	if (!value || std::string(value).empty())
	{
		newValue = "FILL";
	}
	else
	{
		newValue = value;
	}

	if (m_template.get())
	{
		bool found = false;
		const SubjectElementList& elements = m_template->getSubjectElements();
		for (size_t i = 0; i < elements.size() && !found; i++)
		{
			if (StringUtil::stringEquals(name, elements[i].first.c_str()))
			{
				found = true;
				m_subjectElementValues[i] = StringUtil::stringToUpper(newValue);
			}
		}

		if (!found)
		{
			std::string err;
			err.append("Message does not have a subject element named ").append(name);
			throw GmsecException(MSG_ERROR, OTHER_ERROR_CODE, err.c_str());
		}
	}
	else
	{
		throw GmsecException(MSG_ERROR, OTHER_ERROR_CODE, "Message does not have a message template");
	}
}


const char* CALL_TYPE InternalMessage::getResponseTopic(bool useWildcards) const
{
	if (getKind() != Message::Kind::REQUEST)
	{
		throw GmsecException(MSG_ERROR, INVALID_MSG, "Cannot use non-REQUEST kind message");
	}

	m_responseTopic = buildSubject(true, useWildcards);

	if (m_responseTopic.empty())
	{
		// Request Directive Message does not have a template; cobble together best guess for the response topic
		m_responseTopic = m_subject;

		size_t pos = m_responseTopic.find(".REQ.");

		if (pos != std::string::npos)
		{
			m_responseTopic = m_responseTopic.replace(pos, strlen(".REQ."), ".RESP.");

			pos = m_responseTopic.find(".RESP.");

			m_responseTopic = m_responseTopic.substr(0, pos + strlen(".RESP."));

			m_responseTopic += (hasField("MESSAGE-SUBTYPE") ? StringUtil::stringToUpper(getStringValue("MESSAGE-SUBTYPE")) : "FILL");
			m_responseTopic += ".";
			m_responseTopic += (hasField("COMPONENT") ? StringUtil::stringToUpper(getStringValue("COMPONENT")) : "FILL");
			m_responseTopic += ".+";
		}
		else
		{
			m_responseTopic.clear();
		}
	}
	else
	{
		size_t pos = m_responseTopic.find(".REQ.");

		if (pos != std::string::npos)
		{
			m_responseTopic.replace(pos, strlen(".REQ."), ".RESP.");
		}

		pos = m_responseTopic.find("DESTINATION-COMPONENT");

		if (pos != std::string::npos)
		{
			m_responseTopic.replace(pos, strlen("DESTINATION-COMPONENT"), (hasField("COMPONENT") ? StringUtil::stringToUpper(getStringValue("COMPONENT")) : "FILL"));
		}

		m_responseTopic += ".+";
	}

	return m_responseTopic.c_str();
}


void InternalMessage::setKind(Message::Kind kind)
{
	m_kind    = kind;
	m_kindSet = true;
}


Message::Kind InternalMessage::getKind() const
{
	if (!m_kindSet)
	{
		// see if we can deduce the message kind
		try
		{
			std::string typeDef;
			if (m_template.get() && !m_template->getDefinition().empty())
			{
				typeDef = StringUtil::split(m_template->getDefinition(), '.').front();
			}
			else
			{
				typeDef = "MESSAGE-TYPE";
			}

			std::string msgType = getStringValue(typeDef.c_str());

			if (msgType == "MSG")  m_kind = Message::Kind::PUBLISH;
			if (msgType == "REQ")  m_kind = Message::Kind::REQUEST;
			if (msgType == "RESP") m_kind = Message::Kind::REPLY;
		}
		catch (...)
		{
			// ignore error (unknown message type)
			m_kind = Message::Kind::PUBLISH;
		}
	}

	return m_kind;
}


bool InternalMessage::addField(const Field& field, bool makeCopy)
{
	return m_fields.addField(field, makeCopy);
}


bool InternalMessage::addField(const char* name, const GMSEC_U8* blob, size_t len)
{
	Field* field = new BinaryField(name, blob, len);
	return addField(*field, false);
}


bool InternalMessage::addField(const char* name, bool value)
{
	Field* field = new BooleanField(name, value);
	return addField(*field, false);
}


bool InternalMessage::addField(const char* name, GMSEC_CHAR value)
{
	Field* field = new CharField(name, value);
	return addField(*field, false);
}


bool InternalMessage::addField(const char* name, GMSEC_I8 value)
{
	Field* field = new I8Field(name, value);
	return addField(*field, false);
}


bool InternalMessage::addField(const char* name, GMSEC_I16 value)
{
	Field* field = new I16Field(name, value);
	return addField(*field, false);
}


bool InternalMessage::addField(const char* name, GMSEC_I32 value)
{
	Field* field = new I32Field(name, value);
	return addField(*field, false);
}


bool InternalMessage::addField(const char* name, GMSEC_I64 value)
{
	Field* field = new I64Field(name, value);
	return addField(*field, false);
}


bool InternalMessage::addField(const char* name, GMSEC_F32 value)
{
	Field* field = new F32Field(name, value);
	return addField(*field, false);
}


bool InternalMessage::addField(const char* name, GMSEC_F64 value)
{
	Field* field = new F64Field(name, value);
	return addField(*field, false);
}


bool InternalMessage::addField(const char* name, const char* value)
{
	Field* field = new StringField(name, value);
	return addField(*field, false);
}


bool InternalMessage::addField(const char* name, GMSEC_U8 value)
{
	Field* field = new U8Field(name, value);
	return addField(*field, false);
}


bool InternalMessage::addField(const char* name, GMSEC_U16 value)
{
	Field* field = new U16Field(name, value);
	return addField(*field, false);
}


bool InternalMessage::addField(const char* name, GMSEC_U32 value)
{
	Field* field = new U32Field(name, value);
	return addField(*field, false);
}


bool InternalMessage::addField(const char* name, GMSEC_U64 value)
{
	Field* field = new U64Field(name, value);
	return addField(*field, false);
}


bool InternalMessage::addFields(const List<Field*>& fields)
{
	bool fieldReplaced = false;

	for (List<Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
	{
		if (*it != NULL)
		{
			fieldReplaced |= addField(*(*it));
		}
	}

	return fieldReplaced;
}


void InternalMessage::clearFields()
{
	m_fields.clearFields();
}


bool InternalMessage::clearField(const char* name)
{
	validateFieldName(name);

	return m_fields.clearField(name);
}


bool InternalMessage::hasField(const char* name) const
{
	if (name == NULL || *name == '\0')
	{
		return false;
	}

	return (getField(name) != NULL);
}


const char* InternalMessage::getStringValue(const char* fieldName) const
{
	const Field* field = getField(fieldName);

	if (field)
	{
		return field->getStringValue();
	}

	std::string errmsg = "Message does not contain field with name: ";
	errmsg += fieldName;
	throw GmsecException(MSG_ERROR, INVALID_FIELD, errmsg.c_str());
}


bool InternalMessage::getBooleanValue(const char* fieldName) const
{
	const Field* field = getField(fieldName);

	if (field)
	{
		return field->getBooleanValue();
	}

	std::string errmsg = "Message does not contain field with name: ";
	errmsg += fieldName;
	throw GmsecException(MSG_ERROR, INVALID_FIELD, errmsg.c_str());
}


GMSEC_I16 InternalMessage::getI16Value(const char* fieldName) const
{
	const Field* field = getField(fieldName);

	if (field)
	{
		return field->getI16Value();
	}

	std::string errmsg = "Message does not contain field with name: ";
	errmsg += fieldName;
	throw GmsecException(MSG_ERROR, INVALID_FIELD, errmsg.c_str());
}


GMSEC_I32 InternalMessage::getI32Value(const char* fieldName) const
{
	const Field* field = getField(fieldName);

	if (field)
	{
		return field->getI32Value();
	}

	std::string errmsg = "Message does not contain field with name: ";
	errmsg += fieldName;
	throw GmsecException(MSG_ERROR, INVALID_FIELD, errmsg.c_str());
}


GMSEC_I64 InternalMessage::getI64Value(const char* fieldName) const
{
	const Field* field = getField(fieldName);

	if (field)
	{
		return field->getI64Value();
	}

	std::string errmsg = "Message does not contain field with name: ";
	errmsg += fieldName;
	throw GmsecException(MSG_ERROR, INVALID_FIELD, errmsg.c_str());
}


GMSEC_U16 InternalMessage::getU16Value(const char* fieldName) const
{
	const Field* field = getField(fieldName);

	if (field)
	{
		return field->getU16Value();
	}

	std::string errmsg = "Message does not contain field with name: ";
	errmsg += fieldName;
	throw GmsecException(MSG_ERROR, INVALID_FIELD, errmsg.c_str());
}


GMSEC_U32 InternalMessage::getU32Value(const char* fieldName) const
{
	const Field* field = getField(fieldName);

	if (field)
	{
		return field->getU32Value();
	}

	std::string errmsg = "Message does not contain field with name: ";
	errmsg += fieldName;
	throw GmsecException(MSG_ERROR, INVALID_FIELD, errmsg.c_str());
}


GMSEC_U64 InternalMessage::getU64Value(const char* fieldName) const
{
	const Field* field = getField(fieldName);

	if (field)
	{
		return field->getU64Value();
	}

	std::string errmsg = "Message does not contain field with name: ";
	errmsg += fieldName;
	throw GmsecException(MSG_ERROR, INVALID_FIELD, errmsg.c_str());
}


GMSEC_F64 InternalMessage::getF64Value(const char* fieldName) const
{
	const Field* field = getField(fieldName);

	if (field)
	{
		return field->getF64Value();
	}

	std::string errmsg = "Message does not contain field with name: ";
	errmsg += fieldName;
	throw GmsecException(MSG_ERROR, INVALID_FIELD, errmsg.c_str());
}


const Field* InternalMessage::getField(const char* name) const
{
	validateFieldName(name);

	return m_fields.getField(name);
}


Field::Type InternalMessage::getFieldType(const char* name) const
{
	const Field* field = getField(name);

	if (!field)
	{
		std::string errmsg = "Message does not contain field with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return field->getType();
}


const BinaryField& InternalMessage::getBinaryField(const char* name) const
{
	validateFieldName(name);

	const BinaryField* field = dynamic_cast<const BinaryField*>(m_fields.getField(name));

	if (!field)
	{
		std::string errmsg = "Message does not contain BinaryField with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return *field;
}


const BooleanField& InternalMessage::getBooleanField(const char* name) const
{
	validateFieldName(name);

	const BooleanField* field = dynamic_cast<const BooleanField*>(m_fields.getField(name));

	if (!field)
	{
		std::string errmsg = "Message does not contain BooleanField with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return *field;
}


const CharField& InternalMessage::getCharField(const char* name) const
{
	validateFieldName(name);

	const CharField* field = dynamic_cast<const CharField*>(m_fields.getField(name));

	if (!field)
	{
		std::string errmsg = "Message does not contain CharField with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return *field;
}


const F32Field& InternalMessage::getF32Field(const char* name) const
{
	validateFieldName(name);

	const F32Field* field = dynamic_cast<const F32Field*>(m_fields.getField(name));

	if (!field)
	{
		std::string errmsg = "Message does not contain F32Field with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return *field;
}


const F64Field& InternalMessage::getF64Field(const char* name) const
{
	validateFieldName(name);

	const F64Field* field = dynamic_cast<const F64Field*>(m_fields.getField(name));

	if (!field)
	{
		std::string errmsg = "Message does not contain F64Field with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return *field;
}


const I8Field& InternalMessage::getI8Field(const char* name) const
{
	validateFieldName(name);

	const I8Field* field = dynamic_cast<const I8Field*>(m_fields.getField(name));

	if (!field)
	{
		std::string errmsg = "Message does not contain I8Field with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return *field;
}


const I16Field& InternalMessage::getI16Field(const char* name) const
{
	validateFieldName(name);

	const I16Field* field = dynamic_cast<const I16Field*>(m_fields.getField(name));

	if (!field)
	{
		std::string errmsg = "Message does not contain I16Field with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return *field;
}


const I32Field& InternalMessage::getI32Field(const char* name) const
{
	validateFieldName(name);

	const I32Field* field = dynamic_cast<const I32Field*>(m_fields.getField(name));

	if (!field)
	{
		std::string errmsg = "Message does not contain I32Field with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return *field;
}


const I64Field& InternalMessage::getI64Field(const char* name) const
{
	validateFieldName(name);

	const I64Field* field = dynamic_cast<const I64Field*>(m_fields.getField(name));

	if (!field)
	{
		std::string errmsg = "Message does not contain I64Field with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return *field;
}


const U8Field& InternalMessage::getU8Field(const char* name) const
{
	validateFieldName(name);

	const U8Field* field = dynamic_cast<const U8Field*>(m_fields.getField(name));

	if (!field)
	{
		std::string errmsg = "Message does not contain U8Field with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return *field;
}


const U16Field& InternalMessage::getU16Field(const char* name) const
{
	validateFieldName(name);

	const U16Field* field = dynamic_cast<const U16Field*>(m_fields.getField(name));

	if (!field)
	{
		std::string errmsg = "Message does not contain U16Field with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return *field;
}


const U32Field& InternalMessage::getU32Field(const char* name) const
{
	validateFieldName(name);

	const U32Field* field = dynamic_cast<const U32Field*>(m_fields.getField(name));

	if (!field)
	{
		std::string errmsg = "Message does not contain U32Field with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return *field;
}


const U64Field& InternalMessage::getU64Field(const char* name) const
{
	validateFieldName(name);

	const U64Field* field = dynamic_cast<const U64Field*>(m_fields.getField(name));

	if (!field)
	{
		std::string errmsg = "Message does not contain U64Field with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return *field;
}


const StringField& InternalMessage::getStringField(const char* name) const
{
	validateFieldName(name);

	const StringField* field = dynamic_cast<const StringField*>(m_fields.getField(name));

	if (!field)
	{
		std::string errmsg = "Message does not contain StringField with name: ";
		errmsg += name;
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
	}

	return *field;
}


size_t InternalMessage::getFieldCount() const
{
	return m_fields.getFieldCount();
}


void InternalMessage::copyFields(InternalMessage& toMsg) const
{
	toMsg.m_fields = m_fields;
}


const char* InternalMessage::toXML() const
{
	std::ostringstream oss;

	if (!m_subjectSet)
	{
		m_subject = buildSubject();
	}

	oss << "<MESSAGE SUBJECT=\"" << m_subject << "\" ";

	switch (getKind())
	{
		case Message::Kind::PUBLISH:
			oss << "KIND=\"PUBLISH\"";
			break;

		case Message::Kind::REQUEST:
			oss << "KIND=\"REQUEST\"";
			break;

		case Message::Kind::REPLY:
			oss << "KIND=\"REPLY\"";
			break;
	}

	const char* cfgXML = NULL;
	const char* fldXML = NULL;

	if (m_showMsgConfig)
	{
		cfgXML = m_config.toXML();
	}

	if (m_fields.getFieldCount() > 0)
	{
		fldXML = m_fields.toXML();
	}

	if (cfgXML)
	{
		oss << ">" << "\n";

		// Indent the config string to ensure overall output is pretty
		std::string strCfgXML = cfgXML;
		size_t pos = 0;

		strCfgXML.insert(pos, "\t");

		while ((pos = strCfgXML.find('\n', pos)) != std::string::npos)
		{
			strCfgXML.replace(pos, 1, "\n\t");
			pos += 2;
		}

		oss << strCfgXML << "\n";
	}
	if (fldXML)
	{
		if (!cfgXML)
		{
			oss << ">" << "\n";
		}

		oss << fldXML;
	}
	if (cfgXML || fldXML)
	{
		oss << "</MESSAGE>";
	}
	else
	{
		oss << "/>";
	}

	m_xml = oss.str();

	return m_xml.c_str();
}


const char* InternalMessage::toJSON() const
{
	// Note: See Message.h dox for JSON structure
	std::ostringstream jsonStream;

	if (!m_subjectSet)
	{
		m_subject = buildSubject();
	}

	jsonStream << "{\"MESSAGE\":{";

	jsonStream << "\"SUBJECT\":\"" << m_subject << "\",";

	switch (getKind())
	{
		case Message::Kind::PUBLISH:
			jsonStream << "\"KIND\":\"PUBLISH\",";
			break;

		case Message::Kind::REQUEST:
			jsonStream << "\"KIND\":\"REQUEST\",";
			break;

		case Message::Kind::REPLY:
			jsonStream << "\"KIND\":\"REPLY\",";
			break;
	}

	if (m_showMsgConfig)
	{
		InternalConfig internalConfig(m_config.toJSON(), DataType::JSON_DATA);

		std::string configJSON = internalConfig.toJSON(false);

		jsonStream << configJSON;

		if (m_fields.getFieldCount() > 0)
		{
			jsonStream << ",";
		}
	}

	if (m_fields.getFieldCount() > 0)
	{
		jsonStream << "\"FIELD\":[";

		jsonStream << m_fields.toJSON();

		jsonStream << "]";
	}

	jsonStream << "}}";

	m_json = jsonStream.str();

	return m_json.c_str();
}


size_t InternalMessage::getSize(const Message& msg) const
{
	DataBuffer buffer;
	MessageEncoder encoder;
	encoder.encode(msg, buffer);
	return buffer.size();
}


MessageFieldIterator& InternalMessage::getFieldIterator(MessageFieldIterator::Selector selector)
{
	m_fieldIterator.m_iter->setSelector(selector);
	m_fieldIterator.m_iter->reset();

	return m_fieldIterator;
}


ValueMap& InternalMessage::getDetails() const
{
	return *m_details;
}


void InternalMessage::setTracking(const TrackingDetails& tracking)
{
	m_tracking = tracking;
}


TrackingDetails& InternalMessage::getTracking()
{
	return m_tracking;
}


void InternalMessage::processConfig(const Config& config)
{
	const char* name  = 0;
	const char* value = 0;

	bool hasNext = config.getFirst(name, value);

	while (hasNext)
	{
		if (processConfigValue(name, value))
		{
			m_config.addValue(name, value);
		}

		hasNext = config.getNext(name, value);
	}

	m_tracking = TrackingDetails::initialize(config);
}


bool InternalMessage::processConfigValue(const char* name, const char* value)
{
	if (StringUtil::stringEqualsIgnoreCase(name, GMSEC_MSG_FLD_STORAGE_TYPE))
	{
		if (StringUtil::stringEqualsIgnoreCase(value, TREE_TYPE))
		{
			m_fields.setStorageType(MsgFieldMap::BINARY_TREE_MAP);
			return true;
		}
		else if (StringUtil::stringEqualsIgnoreCase(value, HASH_TYPE))
		{
			m_fields.setStorageType(MsgFieldMap::HASH_MAP);
			return true;
		}
		else
		{
			std::stringstream ss;
			ss << GMSEC_MSG_FLD_STORAGE_TYPE << " must be either '" << TREE_TYPE
			   << "' or '" << HASH_TYPE << "'; got '" << value << "'.";
			throw GmsecException(MSG_ERROR, UNUSED_CONFIG_ITEM, ss.str().c_str());
		}
	}
	else if (StringUtil::stringEqualsIgnoreCase(name, GMSEC_MSG_FLD_STORAGE_SIZE))
	{
		try
		{
			int limit = StringUtil::getValue<int>(value);

			m_fields.setRolloverLimit(limit);

			return true;
		}
		catch (...)
		{
			std::stringstream ss;
			ss << GMSEC_MSG_FLD_STORAGE_SIZE << " value must be a number; got '" << value << "'.";
			throw GmsecException(MSG_ERROR, UNUSED_CONFIG_ITEM, ss.str().c_str());
		}
	}
	else if (StringUtil::stringEqualsIgnoreCase(name, GMSEC_SORT_MSG_FIELDS))
	{
		if (StringUtil::stringEqualsIgnoreCase(value, "true"))
		{
			m_fields.setFieldDisplay(MsgFieldMap::SORTED);
			return true;
		}
		else if (StringUtil::stringEqualsIgnoreCase(value, "false"))
		{
			m_fields.setFieldDisplay(MsgFieldMap::UNSORTED);
			return true;
		}
		else
		{
			std::stringstream ss;
			ss << GMSEC_SORT_MSG_FIELDS << " value must be either be true or false.";
			throw GmsecException(MSG_ERROR, UNUSED_CONFIG_ITEM, ss.str().c_str());
		}
	}
	else if (StringUtil::stringEqualsIgnoreCase(name, GMSEC_MESSAGE_SPEC_VERSION)
	         || StringUtil::stringEqualsIgnoreCase(name, GMSEC_SCHEMA_PATH)
	         || StringUtil::stringEqualsIgnoreCase(name, GMSEC_SCHEMA_LEVEL))
	{
		return true;
	}
	// Ignore tracking fields; these are processed elsewhere
	else if (StringUtil::stringEqualsIgnoreCase(name, GMSEC_TRACKING)
	         || StringUtil::stringEqualsIgnoreCase(name, GMSEC_TRACKING_NODE)
	         || StringUtil::stringEqualsIgnoreCase(name, GMSEC_TRACKING_PROCESS_ID)
	         || StringUtil::stringEqualsIgnoreCase(name, GMSEC_TRACKING_USERNAME)
	         || StringUtil::stringEqualsIgnoreCase(name, GMSEC_TRACKING_CONNECTION_ID)
	         || StringUtil::stringEqualsIgnoreCase(name, GMSEC_TRACKING_PUBLISH_TIME)
	         || StringUtil::stringEqualsIgnoreCase(name, GMSEC_TRACKING_UNIQUE_ID)
	         || StringUtil::stringEqualsIgnoreCase(name, GMSEC_TRACKING_MW_INFO)
	         || StringUtil::stringEqualsIgnoreCase(name, GMSEC_TRACKING_ACTIVE_SUBSCRIPTIONS)
	         || StringUtil::stringEqualsIgnoreCase(name, GMSEC_TRACKING_CONNECTION_ENDPOINT))
	{
		return true;
	}
	else if (StringUtil::stringEqualsIgnoreCase(name, GMSEC_SHOW_MSG_CONFIG))
	{
		if (StringUtil::stringEqualsIgnoreCase(value, "true"))
		{
			m_showMsgConfig = true;
			return true;
		}
		else if (StringUtil::stringEqualsIgnoreCase(value, "false"))
		{
			m_showMsgConfig = false;
			return true;
		}
		else
		{
			std::stringstream ss;
			ss << GMSEC_SHOW_MSG_CONFIG << " value must be either be true or false.";
			throw GmsecException(MSG_ERROR, UNUSED_CONFIG_ITEM, ss.str().c_str());
		}
	}
	else if (StringUtil::stringEqualsIgnoreCase(name, GMSEC_REMOVE_TRACKING_FIELDS))
	{
		if (StringUtil::stringEqualsIgnoreCase(value, "true") || StringUtil::stringEqualsIgnoreCase(value, "false"))
		{
			return true;
		}
		else
		{
			std::stringstream ss;
			ss << GMSEC_REMOVE_TRACKING_FIELDS << " value must be either be true or false.";
			throw GmsecException(MSG_ERROR, UNUSED_CONFIG_ITEM, ss.str().c_str());
		}
	}

	return false;
}


void InternalMessage::resetFieldIterator()
{
	m_fields.initIterator();
}


bool InternalMessage::hasNextField() const
{
	return m_fields.hasNext();
}


const Field& InternalMessage::nextField()
{
	return m_fields.next();
}


void InternalMessage::setMiddlewareInfo(MWInfo& mwInfo)
{
	m_mwInfo = mwInfo;
}


InternalMessage::MWInfo& InternalMessage::getMiddlewareInfo()
{
	return m_mwInfo;
}


void InternalMessage::setVersion(unsigned int version)
{
	m_msgVersion = version;
}


unsigned int InternalMessage::getVersion() const
{
	return m_msgVersion;
}


void InternalMessage::setSchemaLevel(Specification::SchemaLevel level)
{
	m_msgSchemaLevel = level;
}


Specification::SchemaLevel InternalMessage::getSchemaLevel() const
{
	return m_msgSchemaLevel;
}


void InternalMessage::setTemplate(MessageTemplate* msgTemplate)
{
	m_template.reset(msgTemplate);
}


const MessageTemplate* InternalMessage::getTemplate() const
{
	return m_template.get();
}


void InternalMessage::init()
{
	if (m_template.get())
	{
		const MessageTemplate::FieldTemplateList& fieldTemplates = m_template->getFieldTemplates();

		for (MessageTemplate::FieldTemplateList::const_iterator it = fieldTemplates.begin(); it != fieldTemplates.end(); ++it)
		{
			FieldTemplate* temp = *it;

			if (!StringUtil::stringEqualsIgnoreCase(temp->getMode().c_str(), "CONTROL") && temp->hasExplicitType() && temp->hasExplicitValue())
			{
				//the field template has an explicitly defined type and value, so we'll add the field to the message
				StdUniquePtr<Field> field(temp->toField(temp->getType()));

				if (field.get())
				{
					addField(*(field.release()), false);
				}
			}
		}

		if (!m_template->getSubjectElements().empty())
		{
			const SubjectElementList& elements = m_template->getSubjectElements();
			for (SubjectElementList::const_iterator it = elements.begin(); it != elements.end(); ++it)
			{
				m_subjectElementValues.push_back("FILL");
			}
		}

		// TODO: Make the following optional (i.e., configurable)
		//
		// Normalize the fields to fit the message specication in use by
		// leveraging the usage of the setFieldValue() method.
		//
		// Caveat #1: Copying of fields is an 'expensive' operation.
		//
		MessageFieldIterator& iter = getFieldIterator( MessageFieldIterator::Selector::NON_HEADER_FIELDS );

		List<const Field*> fields;
		while (iter.hasNext())
		{
			const Field& field = iter.next();

			try {
				const FieldTemplate& fieldTemp = findFieldTemplate(field.getName());

				if (fieldTemp.hasExplicitType())
				{
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

		for (List<const Field*>::iterator it = fields.begin(); it != fields.end(); ++it)
		{
			const Field* field = *it;

			// Convert field, if necessary, for the given Specification.
			setFieldValue(field->getName(), field->getStringValue(), (field->getType() != Field::Type::BINARY ? true : false));
		}

		// If necessary, remove old-style field.
		if (getVersion() >= GMSEC_MSG_SPEC_2019_00)
		{
			(void) clearField("C2CX-SUBTYPE");
		}
		else if (getVersion() < GMSEC_MSG_SPEC_2018_00)
		{
			(void) clearField("SPECIFICATION");
		}
	}
}


std::string InternalMessage::buildSubject(bool forTopic, bool useWildcards) const
{
	std::string subject;

	// If no template is available, or no subject within the template, then we cannot generate/build a subject.
	if (m_template.get() && !m_template->getSubjectElements().empty())
	{
		const SubjectElementList& elements = m_template->getSubjectElements();

		for (size_t i = 0; i < elements.size(); ++i)
		{
			std::string element = elements[i].second;

			bool required = true;

			//determine if element is required/optional and remove character flag
			if (element.find("!") != std::string::npos)
			{
				if (forTopic) continue;
				//if (elements[i].second.find("ME2") != std::string::npos) continue;
				required = false;
				element = element.substr(1);
			}

			std::string value = (useWildcards ? (required ? "*" : "!*") : (required ? m_subjectElementValues[i] : "!" + m_subjectElementValues[i]));

			//element can have multiple associated fields, split into sub elements and look for values
			std::vector<std::string> subElements = StringUtil::split(element, ",");
			for (size_t j = 0; j < subElements.size(); ++j)
			{
				if (hasField(subElements[j].c_str()))
				{
					if (forTopic && subElements[j] == "DESTINATION-COMPONENT")
						value = subElements[j];
					else
						value = getStringValue(subElements[j].c_str());
					break;
				}
				else if (subElements[j] == "SPECIFICATION" || matchDefinitionElement(subElements[j]))
				{
					value = "FILL";
					break;
				}
			}

			//if element is empty then fill is expected by user, no need for warning.
			if (value == "FILL" && !element.empty())
			{
				std::ostringstream oss;
				oss << "Subject element " << elements[i].first.c_str() <<  " missing; requires field(s): " << element;
				GMSEC_DEBUG << oss.str().c_str();
			}

			subject += value;
			subject += ".";
		}

		if (!subject.empty())
		{
			// Erase trailing . (period char)
			subject.erase(subject.length() - 1, 1);

			// Remove trailing .!FILL or !* elements (i.e. unset miscellaneous elements)
			size_t pos;
			while ((pos = subject.rfind(".")) != std::string::npos)
			{
				if (subject.substr(pos) == ".!FILL")
				{
					subject.erase(pos, 6);
				}
				else if (subject.substr(pos) == ".!*")
				{
					subject.erase(pos, 3);
				}
				else
				{
					break;
				}
			}

			// Remove any remaining exclamation ('!') marks from optional subject elements
			while ((pos = subject.rfind("!")) != std::string::npos)
			{
				subject.erase(pos, 1);
			}

			// Ensure only uppercase characters are used in the subject
			subject = StringUtil::stringToUpper(subject);
		}
	}
	return subject;
}

bool InternalMessage::matchDefinitionElement(const std::string& element) const
{
	std::vector<std::string> definition = StringUtil::split(m_template->getDefinition(), '.');

	for (std::vector<std::string>::const_iterator it = definition.begin(); it != definition.end(); ++it)
	{
		if (element == *it) return true;
	}

	return false;
}


void InternalMessage::processData(const char* data, DataType type)
{
	if (!data || std::string(data).empty())
	{
		throw GmsecException(MSG_ERROR, PARSE_ERROR, "Data string cannot be NULL, nor empty");
	}

	switch (type)
	{
	case DataType::XML_DATA:
		{
			tinyxml2::XMLDocument doc;
			tinyxml2::XMLError code = doc.Parse(data);

			if (code != tinyxml2::XML_NO_ERROR)
			{
				throw GmsecException(MSG_ERROR, XML_PARSE_ERROR, StringUtil::xmlErrorToString(code));
			}

			tinyxml2::XMLElement* element = doc.RootElement();

			if (!element)
			{
				throw GmsecException(MSG_ERROR, XML_PARSE_ERROR, "Invalid XML format -- no root element");
			}

			fromXML(element);
		}
		break;

	case DataType::JSON_DATA:
		{
			Json::Reader reader;
			Json::Value root;
			bool success = reader.parse(data, root);

			if (!success || !root)
			{
				throw GmsecException(MSG_ERROR, JSON_PARSE_ERROR, "Invalid JSON message format -- invalid JSON string data");
			}

			try
			{
				fromJSON(root);
			}
			catch (...)
			{
				throw GmsecException(MSG_ERROR, JSON_PARSE_ERROR, "Invalid JSON message format -- invalid JSON string data");
			}
		}
		break;

	case DataType::KEY_VALUE_DATA:
		throw GmsecException(MSG_ERROR, OTHER_ERROR, "Cannot generate Message from key=value pair(s)");
		break;
	}
}


void InternalMessage::fromXML(tinyxml2::XMLElement* element)
{
	if (!element)
	{
		throw GmsecException(MSG_ERROR, XML_PARSE_ERROR, "Invalid XML -- element is NULL");
	}

	/* verify that our root node is "MESSAGE" */
	const char* mename = element->Name();
	if (!mename || !StringUtil::stringEqualsIgnoreCase(mename, "MESSAGE"))
	{
		throw GmsecException(MSG_ERROR, XML_PARSE_ERROR, "Invalid XML -- MESSAGE element not found");
	}

	const char* subject = NULL;
	const char* kind    = NULL;

	for (const tinyxml2::XMLAttribute *attr = element->FirstAttribute(); attr; attr = attr->Next())
	{
		const char* caname = attr->Name();

		if (caname && StringUtil::stringEqualsIgnoreCase(caname, "SUBJECT"))
		{
			subject = attr->Value();
		}
		else if (caname && StringUtil::stringEqualsIgnoreCase(caname, "KIND"))
		{
			kind = attr->Value();
		}
		else if (caname && StringUtil::stringEqualsIgnoreCase(caname, "TYPE") && (kind == NULL))
		{
			kind = attr->Value();
		}
	}

	if (subject)
	{
		std::string result = Subject::isValid(subject);

		if (!result.empty())
		{
			std::ostringstream oss;
			oss << "Invalid message subject: " << subject << result;

			throw GmsecException(MSG_ERROR, INVALID_SUBJECT_NAME, oss.str().c_str());
		}

		m_subject = subject;
		m_subjectSet = true;
	}

	// set kind
	if (kind)
	{
		if (StringUtil::stringEqualsIgnoreCase(kind, "PUBLISH"))
		{
			m_kind = Message::Kind::PUBLISH;
		}
		else if (StringUtil::stringEqualsIgnoreCase(kind, "REQUEST"))
		{
			m_kind = Message::Kind::REQUEST;
		}
		else if (StringUtil::stringEqualsIgnoreCase(kind, "REPLY"))
		{
			m_kind = Message::Kind::REPLY;
		}
		else
		{
			throw GmsecException(MSG_ERROR, XML_PARSE_ERROR, "Invalid XML -- MESSAGE element contains invalid KIND/TYPE definition");
		}
	}

	// fields (child nodes)
	for (tinyxml2::XMLElement* node = element->FirstChildElement(); node; node = node->NextSiblingElement())
	{
		//field nodes only
		const char* cnname = node->Name();
		if (cnname && StringUtil::stringEqualsIgnoreCase(cnname, "FIELD"))
		{
			Field* field = InternalField::fromXML(node);

			if (field)
			{
				addField(*field, false);
			}
		}
		// configuration node(s)
		else if (cnname && StringUtil::stringEqualsIgnoreCase(cnname, "CONFIG"))
		{
			try
			{
				// only the InternalConfig has the means to ingest an XML document node
				InternalConfig internalConfig;

				internalConfig.fromXML(node);

				Config config(internalConfig.toXML(), DataType::XML_DATA);

				// apply the config to our message
				processConfig(config);
			}
			catch (const GmsecException& e)
			{
				throw GmsecException(MSG_ERROR, XML_PARSE_ERROR, e.what());
			}
		}
	}
}


void InternalMessage::fromJSON(const Json::Value& origRoot)
{
	Json::Value root;
	if (origRoot.isMember("MESSAGE") || origRoot.isMember("message"))
	{
		root = (origRoot.isMember("MESSAGE") ? origRoot["MESSAGE"] : origRoot["message"]);
	}
	else
	{
		throw GmsecException(MSG_ERROR, JSON_PARSE_ERROR, "Invalid JSON Message format -- no MESSAGE root element");
	}

	const char* subject = NULL;
	if (root.isMember("SUBJECT") || root.isMember("subject"))
	{
		subject = (root.isMember("SUBJECT") ? root["SUBJECT"].asCString() : root["subject"].asCString());
	}
	else
	{
		throw GmsecException(MSG_ERROR, JSON_PARSE_ERROR, "Invalid JSON Message format - no SUBJECT provided");
	}

	const char* kind = NULL;
	if (root.isMember("KIND") || root.isMember("kind"))
	{
		kind = (root.isMember("KIND") ? root["KIND"].asCString() : root["kind"].asCString());
	}
	else
	{
		throw GmsecException(MSG_ERROR, JSON_PARSE_ERROR, "Invalid JSON Message format - no KIND provided");
	}

	// Set subject
	m_subject = subject;
	m_subjectSet = true;

	// Set kind
	if (kind != NULL)
	{
		if (StringUtil::stringEqualsIgnoreCase(kind, "PUBLISH"))
		{
			m_kind = Message::Kind::PUBLISH;
		}
		else if (StringUtil::stringEqualsIgnoreCase(kind, "REQUEST"))
		{
			m_kind = Message::Kind::REQUEST;
		}
		else if (StringUtil::stringEqualsIgnoreCase(kind, "REPLY"))
		{
			m_kind = Message::Kind::REPLY;
		}
		else
		{
			throw GmsecException(MSG_ERROR, XML_PARSE_ERROR, "Invalid JSON Message format -- invalid KIND provided");
		}
	}

	if (root.isMember("FIELD") || root.isMember("field"))
	{
		const Json::Value fields = (root.isMember("FIELD") ? root["FIELD"] : root["field"]);

		// Fields
		for (unsigned int i = 0; i < fields.size(); i++)
		{
			Field* field = InternalField::fromJSON(fields[i]); 
			if (field)
			{
				addField(*field, false);
			}
		}
	}

	if (root.isMember("CONFIG") || root.isMember("config"))
	{
		// only the InternalConfig has the means to ingest a JSON Value object
		InternalConfig internalConfig;

		internalConfig.fromJSON(root);

		Config config(internalConfig.toJSON(), DataType::JSON_DATA);

		// apply the config to our message
		processConfig(config);
	}
}


void InternalMessage::validateFieldName(const char* name) const
{
	if (!name || std::string(name).empty())
	{
		throw GmsecException(MSG_ERROR, INVALID_FIELD_NAME, "Field name cannot be NULL, nor an empty string");
	}
}


const FieldTemplate& InternalMessage::findFieldTemplate(const char* fieldName)
{
	if (m_template.get() == NULL)
	{
		throw GmsecException(MSG_ERROR, OTHER_ERROR_CODE, "Message does not have a message template");
	}

	// nested array controls have their placeholder value added to the end of the list
	std::vector<std::string> elements = StringUtil::split(fieldName, '.');
	std::string prefixes;

	// recreate the field name without the index numbers (just the prefixes, the even numbered elements)
	for (size_t i = 0; i < elements.size(); ++i)
	{
		if (i % 2 == 0)
		{
			prefixes.append(elements.at(i));
		}
	}

	const MessageTemplate::FieldTemplateList& msgFieldTemps = m_template->getFieldTemplates();

	for (MessageTemplate::FieldTemplateList::const_iterator ft = msgFieldTemps.begin(); ft != msgFieldTemps.end(); ++ft)
	{
		FieldTemplate* temp = *ft;

		if (StringUtil::stringEqualsIgnoreCase(temp->getMode().c_str(), "CONTROL") && temp->hasChildren())
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

		if (StringUtil::stringEquals(temp->getModifiedName().c_str(), fieldName))
		{
			return *temp;
		}
	}

	throw GmsecException(FIELD_TEMPLATE_ERROR, FIELD_TEMPLATE_NOT_FOUND, "FieldTemplate not found in MessageTemplate");
}
