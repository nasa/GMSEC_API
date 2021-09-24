/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MessageSubclassHelper.cpp
 *
 * @brief This file contains the implementation of the MIST Message 
 * subclass helper functions.
 */

#include <gmsec4/internal/mist/MessageSubclassHelper.h>

#include <gmsec4/Exception.h>

#include <sstream>


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::mist::internal;


std::string MessageSubclassHelper::extractStringField(const char* fieldName, 
                                                      const char* loggableInvocationPoint,
                                                      const InternalMessage& msg)
{
	try
	{
		const StringField& tmp_field = msg.getStringField(fieldName);
		return tmp_field.getValue();
	}
	catch (...)
	{
		std::ostringstream oss;

		oss << loggableInvocationPoint << ":  "
		    << fieldName
		    << " StringField could not be fetched from message";

		throw Exception(MIST_ERROR, FIELD_NOT_SET, oss.str().c_str());
	}
}


GMSEC_I16 MessageSubclassHelper::extractI16Field(const char* fieldName, 
                                                 const char* loggableInvocationPoint,
                                                 const InternalMessage& msg)
{
	try
	{
		const I16Field& i16_field = msg.getI16Field(fieldName);
		return i16_field.getValue();
	}
	catch (...)
	{
		std::ostringstream oss;

		oss << loggableInvocationPoint << ":  Error while fetching "
		    << fieldName << " from message; required field is missing";

		throw Exception(MIST_ERROR, MISSING_REQUIRED_FIELD, oss.str().c_str());
	}

}

GMSEC_U16 MessageSubclassHelper::extractU16Field(const char* fieldName, 
                                                 const char* loggableInvocationPoint,
                                                 const InternalMessage& msg)
{
	try
	{
		const U16Field& u16_field = msg.getU16Field(fieldName);
		return u16_field.getValue();
	}
	catch (...)
	{
		std::ostringstream oss;

		oss << loggableInvocationPoint << ":  Error while fetching "
		    << fieldName << " from message; required field is missing";

		throw Exception(MIST_ERROR, MISSING_REQUIRED_FIELD, oss.str().c_str());
	}

}

const Field* MessageSubclassHelper::extractField(const char* fieldName, 
                                                 const char* loggableInvocationPoint,
                                                 const InternalMessage& msg)
{
	const Field * ptr = msg.getField(fieldName);
	
	if (!ptr)
	{
		std::ostringstream oss;

		oss << loggableInvocationPoint << ":  Error while fetching "
			<< fieldName << " from message; required field is missing";

		throw Exception(MIST_ERROR, MISSING_REQUIRED_FIELD, oss.str().c_str());
	}

	return ptr;
}


void MessageSubclassHelper::checkStringField(const char* fieldName, 
                                             const char* loggableInvocationPoint,
                                             const char* expectedValue,
                                             const InternalMessage& msg)
{
	try
	{
		const StringField& string_field = msg.getStringField(fieldName);
		std::string message_type = string_field.getValue();
		if (message_type != expectedValue)
		{
			std::ostringstream oss;

			oss << loggableInvocationPoint << ":  Error while fetching "
			    << fieldName << " from message; value should be "
			    << expectedValue << " and instead it is " << message_type;

			throw Exception(MIST_ERROR, FIELD_NOT_SET, oss.str().c_str());
		}
	}
	catch (const Exception& excep)
	{
		if (excep.getErrorCode() == FIELD_NOT_SET)
		{
			throw excep; // re-throw as-is
		}
		else
		{
			std::ostringstream oss;

			oss << loggableInvocationPoint << ":  Error while fetching "
			    << fieldName << " from message; required field is missing";

			throw Exception(MIST_ERROR, MISSING_REQUIRED_FIELD, oss.str().c_str());
		}
	}
}


void MessageSubclassHelper::checkFloatField(const char* fieldName,
                                            const char* loggableInvocationPoint,
                                            GMSEC_F32 expectedValue,
                                            const InternalMessage& msg)
{
	try
	{
		const F32Field& f32_field = msg.getF32Field(fieldName);
		GMSEC_F32 header_version = f32_field.getValue();
		if (header_version != expectedValue)
		{
			std::ostringstream oss;

			oss << loggableInvocationPoint << ":  Error while fetching "
			    << fieldName << " from message; value should be " << expectedValue
			    << " and instead it is " << header_version;

			throw Exception(MIST_ERROR, FIELD_NOT_SET, oss.str().c_str());
		}
	}
	catch (const Exception& excep)
	{
		if (excep.getErrorCode() == FIELD_NOT_SET)
		{
			throw excep;  // re-throw as-is
		}
		else
		{
			std::ostringstream oss;

			oss << loggableInvocationPoint << ":  Error while fetching "
			    << fieldName << " from message; required field is missing";

			throw Exception(MIST_ERROR, MISSING_REQUIRED_FIELD, oss.str().c_str());
		}
	}
}


bool MessageSubclassHelper::getOptionalString(const char* fieldName, const InternalMessage& msg, std::string& target)
{
	try
	{
		const StringField& tmp_field = msg.getStringField(fieldName);
		target = tmp_field.getValue();
		return true;
	}
	catch (...)
	{
		return false;
	}
}


bool MessageSubclassHelper::getOptionalI16(const char* fieldName, const InternalMessage& msg, GMSEC_I16& target)
{
	try
	{
		const I16Field& tmp_field = msg.getI16Field(fieldName);
		target = tmp_field.getValue();
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool MessageSubclassHelper::getOptionalI32(const char* fieldName,
                                         const gmsec::api::internal::InternalMessage& msg,
                                         GMSEC_I32& target)
{
	try
	{
		const I32Field& tmp_field = msg.getI32Field(fieldName);
		target = tmp_field.getValue();
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool MessageSubclassHelper::getOptionalU32(const char* fieldName,
                                         const gmsec::api::internal::InternalMessage& msg,
                                         GMSEC_U32& target)
{
	try
	{
		const U32Field& tmp_field = msg.getU32Field(fieldName);
		target = tmp_field.getValue();
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool MessageSubclassHelper::getOptionalBool(const char* fieldName,
                                         const gmsec::api::internal::InternalMessage& msg,
										 bool& target)
{
	try
	{
		const BooleanField& tmp_field = msg.getBooleanField(fieldName);
		target = tmp_field.getValue();
		return true;
	}
	catch (...)
	{
		return false;
	}
}


void MessageSubclassHelper::addRenamedFieldToMessage(const Field& valueField, InternalMessage& msg, const char* newFieldName)
{
	//TODO - code ported from InternalDeviceMessage. Cleanup and simpilfy.
	switch (valueField.getType())
	{
		case Field::BOOL_TYPE:
		{
			const BooleanField* boolean_field = dynamic_cast<const BooleanField*>(&valueField);

			if (boolean_field != NULL)
			{
				msg.addField(newFieldName, boolean_field->getValue());
			}
			else
			{
				throw Exception(MIST_ERROR,
				                MISSING_REQUIRED_FIELD,
				                "MessageSubclassHelper::addRenamedFieldToMessage:  Boolean field casting error.");
			}
		}
		break;

		case Field::CHAR_TYPE:
		{
			const CharField* char_field = dynamic_cast<const CharField*>(&valueField);

			if (char_field != NULL)
			{
				msg.addField(newFieldName, char_field->getValue());
			}
			else
			{
				throw Exception(MIST_ERROR,
				                MISSING_REQUIRED_FIELD,
				                "MessageSubclassHelper::addRenamedFieldToMessage:  Char field casting error.");
			}
		}
		break;

		case Field::I8_TYPE:
		{
			const I8Field* i8_field = dynamic_cast<const I8Field*>(&valueField);

			if (i8_field != NULL)
			{
				msg.addField(newFieldName, i8_field->getValue());
			}
			else
			{
				throw Exception(MIST_ERROR,
				                MISSING_REQUIRED_FIELD,
				                "MessageSubclassHelper::addRenamedFieldToMessage:  I8 field casting error.");
			}
		}
		break;

		case Field::I16_TYPE:
		{
			const I16Field* i16_field = dynamic_cast<const I16Field*>(&valueField);

			if (i16_field != NULL)
			{
				msg.addField(newFieldName, i16_field->getValue());
			}
			else
			{
				throw Exception(MIST_ERROR,
				                MISSING_REQUIRED_FIELD,
				                "MessageSubclassHelper::addRenamedFieldToMessage:  I16 field casting error.");
			}
		}
		break;

		case Field::I32_TYPE:
		{
			const I32Field* i32_field = dynamic_cast<const I32Field*>(&valueField);

			if (i32_field != NULL)
			{
				msg.addField(newFieldName, i32_field->getValue());
			}
			else
			{
				throw Exception(MIST_ERROR,
				                MISSING_REQUIRED_FIELD,
				                "MessageSubclassHelper::addRenamedFieldToMessage:  I32 field casting error.");
			}
		}
		break;

		case Field::I64_TYPE:
		{
			const I64Field* i64_field = dynamic_cast<const I64Field*>(&valueField);

			if (i64_field != NULL)
			{
				msg.addField(newFieldName, i64_field->getValue());
			}
			else
			{
				throw Exception(MIST_ERROR,
				                MISSING_REQUIRED_FIELD,
				                "MessageSubclassHelper::addRenamedFieldToMessage:  I64 field casting error.");
			}
		}
		break;

		case Field::F32_TYPE:
		{
			const F32Field* f32_field = dynamic_cast<const F32Field*>(&valueField);

			if (f32_field != NULL)
			{
				msg.addField(newFieldName, f32_field->getValue());
			}
			else
			{
				throw Exception(MIST_ERROR,
				                MISSING_REQUIRED_FIELD,
				                "MessageSubclassHelper::addRenamedFieldToMessage:  F32 field casting error.");
			}
		}
		break;

		case Field::F64_TYPE:
		{
			const F64Field* f64_field = dynamic_cast<const F64Field*>(&valueField);

			if (f64_field != NULL)
			{
				msg.addField(newFieldName, f64_field->getValue());
			}
			else
			{
				throw Exception(MIST_ERROR,
				                MISSING_REQUIRED_FIELD,
				                "MessageSubclassHelper::addRenamedFieldToMessage:  F64 field casting error.");
			}
		}
		break;

		case Field::STRING_TYPE:
		{
			const StringField* string_field = dynamic_cast<const StringField*>(&valueField);

			if (string_field != NULL)
			{
				msg.addField(newFieldName, string_field->getValue());
			}
			else
			{
				throw Exception(MIST_ERROR,
				                MISSING_REQUIRED_FIELD,
				                "MessageSubclassHelper::addRenamedFieldToMessage:  String field casting error.");
			}
		}
		break;

		case Field::U8_TYPE:
		{
			const U8Field* u8_field = dynamic_cast<const U8Field*>(&valueField);

			if (u8_field != NULL)
			{
				msg.addField(newFieldName, u8_field->getValue());
			}
			else
			{
				throw Exception(MIST_ERROR,
				                MISSING_REQUIRED_FIELD,
				                "MessageSubclassHelper::addRenamedFieldToMessage:  U8 field casting error.");
			}
		}
		break;

		case Field::U16_TYPE:
		{
			const U16Field* u16_field = dynamic_cast<const U16Field*>(&valueField);

			if (u16_field != NULL)
			{
				msg.addField(newFieldName, u16_field->getValue());
			}
			else
			{
				throw Exception(MIST_ERROR,
				                MISSING_REQUIRED_FIELD,
				                "MessageSubclassHelper::addRenamedFieldToMessage:  U16 field casting error.");
			}
		}
		break;

		case Field::U32_TYPE:
		{
			const U32Field* u32_field = dynamic_cast<const U32Field*>(&valueField);

			if (u32_field != NULL)
			{
				msg.addField(newFieldName, u32_field->getValue());
			}
			else
			{
				throw Exception(MIST_ERROR,
				                MISSING_REQUIRED_FIELD,
				                "MessageSubclassHelper::addRenamedFieldToMessage:  U32 field casting error.");
			}
		}
		break;

		case Field::U64_TYPE:
		{
			const U64Field* u64_field = dynamic_cast<const U64Field*>(&valueField);

			if (u64_field != NULL)
			{
				msg.addField(newFieldName, u64_field->getValue());
			}
			else
			{
				throw Exception(MIST_ERROR,
				                MISSING_REQUIRED_FIELD,
				                "MessageSubclassHelper::addRenamedFieldToMessage:  U64 field casting error.");
			}
		}
		break;

		  
		case Field::BIN_TYPE:
		{
			const BinaryField* binary_field = dynamic_cast<const BinaryField*>(&valueField);

			if (binary_field != NULL)
			{
				msg.addField(newFieldName, binary_field->getValue(), binary_field->getLength());
			}
			else
			{
				throw Exception(MIST_ERROR,
				                MISSING_REQUIRED_FIELD,
				                "MessageSubclassHelper::addRenamedFieldToMessage:  Binary field casting error.");
			}
		}
		break;
	}
}
