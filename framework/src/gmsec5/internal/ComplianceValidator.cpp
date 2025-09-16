/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ComplianceValidator.h
 *
 * @brief Utility class used to determine if GMSEC Message is compliant with the specification in use.
 */


#include <gmsec5/internal/ComplianceValidator.h>

#include <gmsec5/internal/CustomMessageValidator.h>
#include <gmsec5/internal/FieldTemplate.h>
#include <gmsec5/internal/InternalMessage.h>
#include <gmsec5/internal/MathUtil.h>
#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/MessageTemplate.h>
#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/internal/Subject.h>

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5/ConfigOptions.h>
#include <gmsec5/Message.h>

#include <gmsec5/util/cxx.h>
#include <gmsec5/util/Log.h>

#include <sstream>
#include <string>

#if defined(WIN32) || defined(__APPLE__) || (GCC_VERSION >= 40900)
#include <regex>
#endif


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


static const char* const INDENT1         = "   ";
static const char* const NEWLINE_INDENT1 = "\n   ";
static const char* const NEWLINE_INDENT2 = "\n      ";


ComplianceValidator::ComplianceValidator(const Config& config)
	: m_configTracking(),
	  m_customMessageValidator(0),
	  m_ceeMessageValidator()
{
	m_configTracking = TrackingDetails::initialize(config);
}


ComplianceValidator::ComplianceValidator(const ComplianceValidator& other)
	: m_configTracking(other.m_configTracking),
	  m_customMessageValidator(other.m_customMessageValidator),
	  m_ceeMessageValidator()
{
	if (other.m_ceeMessageValidator.get() != NULL)
	{
		registerMessageValidator(other.m_ceeMessageValidator->getValidator());
	}
}


ComplianceValidator::~ComplianceValidator()
{
}


void ComplianceValidator::updateConfig(const Config& config)
{
	m_configTracking = TrackingDetails::initialize(config);
}


Status ComplianceValidator::validateMessage(const Message& msg)
{
	Status status;

	const InternalMessage& intMsg = MessageBuddy::getInternal(msg);

	if (m_customMessageValidator != NULL)
	{
		status = m_customMessageValidator->validateMessage(msg);
	}

	if (!status.hasError())
	{
		if (intMsg.getTemplate() == NULL)
		{
			std::ostringstream err;
			std::string subject = msg.getSubject();
			err << "No template available; unable to validate message ["
			    << (subject.empty() ? "no message subject available" : subject)
			    << "]";
			status.set(MSG_ERROR, MESSAGE_FAILED_VALIDATION, err.str().c_str());
		}
		else
		{
			const FieldTemplateCollection& fieldTemplates = intMsg.getTemplate()->getFieldTemplates();
			std::string prefix;
			std::string validFieldNames;

			status = compare(intMsg, fieldTemplates, prefix, validFieldNames);

			const char* subject = intMsg.getSubject();

			std::string subjectValResult = Subject::isValid(subject, &intMsg);

			if (!status.hasError() && !subjectValResult.empty())
			{
				std::string err = subject;
				err.append(": Invalid Message Subject: ").append(subjectValResult);
				status.set(MSG_ERROR, INVALID_SUBJECT_NAME, err.c_str());
			}
			else if (status.hasError())
			{
				std::string err = subject;
				err.append(": Message Validation Failed.");
				err.append(NEWLINE_INDENT1).append(status.getReason());

				status.setReason(err.c_str());
			}
			else
			{
				// Check if tracking fields need to be validated
				bool checkTrackingFields = false;
				intMsg.getDetails().getBoolean(GMSEC_CHECK_TRACKING_FIELDS, checkTrackingFields);

				if (checkTrackingFields)
				{
					//The accumulated error messages (if any)
					std::string errorList;

					const TrackingDetails& msgTracking = MessageBuddy::getInternal(msg).getTracking();

					MessageFieldIterator& iter = msg.getFieldIterator();

					while (iter.hasNext())
					{
						const Field& field = iter.next();

						//When message is being sent, disallow tracking fields (these are reserved for use by the API)
						Status status = checkTrackingField(field.getName(), m_configTracking, msgTracking);

						if (status.hasError())
						{
							errorList.append(status.getReason()).append(NEWLINE_INDENT1);
						}
					}
					if (!errorList.empty())
					{
						size_t pos = errorList.rfind("\n");
						errorList = errorList.substr(0, pos);

						std::string err = subject;
						err.append(": Message Validation Failed.");
						err.append(NEWLINE_INDENT1).append(errorList);

						status = Status(MSG_ERROR, MESSAGE_FAILED_VALIDATION, err.c_str());
					}
				}
			}
		}
	}

	return status;
}


void ComplianceValidator::registerMessageValidator(MessageValidator* validator)
{
    m_customMessageValidator = validator;
}


void ComplianceValidator::registerMessageValidator(GMSEC_MessageValidator* validator)
{
    m_ceeMessageValidator.reset(new CustomMessageValidator(validator));

    registerMessageValidator(m_ceeMessageValidator.get());
}


std::string ComplianceValidator::getTypeStr(Field::Type type)
{
	try
	{
		return InternalField::lookupTypeStr(type);
	}
	catch (...)
	{
		return "UNDEFINED";
	}
}


Status ComplianceValidator::compare(const InternalMessage& msg, const FieldTemplateCollection& fields, const std::string& prefix, std::string& validFieldNames)
{
	//Overall status (that will be returned)
	Status status;

	//The accumulated error messages as a string
	std::string errorList;

	//flag that determines whether the optional array control we are in should be skipped or not
	bool skipArray = false;

	//iterating through all the fields to make sure they are all in the message and correct
	for (size_t fieldIndex = 0; (fieldIndex < fields.size()); ++fieldIndex)
	{
		//the current iteration's field template we will be validating against
		FieldTemplate* temp = fields[fieldIndex];

		//check to see if the current field template is a control field
		if (skipArray)
		{
			continue;
		}

		if (StringUtil::stringEqualsIgnoreCase(temp->getMode().c_str(), "CONTROL") && temp->hasChildren())
		{//encountered control field which contains an array of fields
			//determine size of the array
			std::string arraySizeName = prefix + "NUM-OF-" + temp->getName();
			size_t size = 0;
			if (msg.hasField(arraySizeName.c_str()))
			{
				size = msg.getI32Value(arraySizeName.c_str());
			}
			else
			{
				//the size of the array is undefined due to the field being missing
				//if that field is optional, then it means the array is optional, so we skip the array entirely
				//if the array is required, we throw an error
				for (FieldTemplateCollection::const_iterator it2 = fields.begin(); it2 != fields.end(); ++it2)
				{
					std::string fieldName = prefix + (*it2)->getName();
					if (StringUtil::stringEqualsIgnoreCase(fieldName.c_str(), arraySizeName.c_str()))
					{
						(*it2)->checkDependencies(msg);
						if (StringUtil::stringEqualsIgnoreCase((*it2)->getMode().c_str(), "OPTIONAL"))
						{
							(*it2)->resetCheckedDependencies();
							skipArray = true;
							break;
						}
						(*it2)->resetCheckedDependencies();
					}
					if (StringUtil::stringEqualsIgnoreCase(fieldName.c_str(), arraySizeName.c_str()) &&
						(StringUtil::stringEqualsIgnoreCase((*it2)->getMode().c_str(), "OPTIONAL") ||
						StringUtil::stringEqualsIgnoreCase((*it2)->getMode().c_str(), "TRACKING")))
					{
						//we have determined the array is optional and we will be skipping it
						skipArray = true;
						break;
					}
				}

				if (!skipArray)
				{
					// Undefined size of required array; we're done.
					std::ostringstream err;
					err << msg.getSubject() << " : Message Validation Failed." << "\n   "
						<< "Message contains array of objects whose size is undefined: "
						<< arraySizeName.c_str() << " cannot be found.";
					status.set(MSG_ERROR, MESSAGE_FAILED_VALIDATION, err.str().c_str());
				}
				skipArray = false;

				continue;
			}

			for (size_t i = 1; i <= size; ++i)
			{
				std::ostringstream newPrefix;
				newPrefix << prefix << temp->getPrefix() << "." << i << ".";

				status = compare(msg, temp->getChildren(), newPrefix.str(), validFieldNames);

				if (status.hasError())
				{
					errorList.append(status.getReason()).append(NEWLINE_INDENT1);
				}
			}
		}

		//modify the name of the field template as necessary
		if (!StringUtil::stringEqualsIgnoreCase(temp->getMode().c_str(), "CONTROL"))
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
		if (temp->getMode() == "REQUIRED" ||
			temp->getMode() == "OPTIONAL" ||
			temp->getMode() == "TRACKING")
		{
			temp->checkDependencies(msg);
			Status valStatus = validate(msg, *temp);

			if (valStatus.hasError())
			{
				errorList.append(valStatus.getReason()).append(NEWLINE_INDENT1);
			}
			temp->resetCheckedDependencies();
		}

		// Regardless of validation level, always validate content of the message fields
		Status status = validateContent(msg, *temp);
		if (status.hasError())
		{
			errorList.append(status.getReason()).append(NEWLINE_INDENT1);
		}
	}//loop until we're done running through all the field templates

	bool checkTrackingFields = false;
	msg.getDetails().getBoolean(GMSEC_CHECK_TRACKING_FIELDS, checkTrackingFields);

	TrackingDetails& msgTracking = const_cast<InternalMessage&>(msg).getTracking();

	const MessageFieldIterator& iter = const_cast<InternalMessage&>(msg).getFieldIterator();

	//Iterate through all the fields in the message, check for API reserved and, if necessary, extraneous fields
	while (iter.hasNext())
	{
		const Field& field = iter.next();

		//Disallow extraneous fields
		std::string fieldName = " ";
		fieldName.append(field.getName()).append(" ");
		if (fieldName.find(prefix) != std::string::npos && validFieldNames.find(field.getName()) == std::string::npos)
		{
			std::ostringstream err;
			err << "Message contains user-defined field " << field.getName();
			Status error(MSG_ERROR, NON_ALLOWED_FIELD, err.str().c_str());
			if (errorList.find(err.str()) == std::string::npos) errorList.append(error.getReason()).append(NEWLINE_INDENT1);
		}
		else if (checkTrackingFields)
		{
			Status status = checkTrackingField(fieldName.c_str(), m_configTracking, msgTracking);

			if (status.hasError())
			{
				errorList.append(status.getReason()).append(NEWLINE_INDENT1);
			}
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

		status = Status(MSG_ERROR, MESSAGE_FAILED_VALIDATION, errorList.c_str());
	}

	return status;
}


std::string ComplianceValidator::prepFieldName(const std::string& name, const std::list<std::string>& charList, const std::list<size_t>& indexList)
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


Status ComplianceValidator::validate(const InternalMessage& msg, const FieldTemplate& ftmp)
{
	Status      status;
	std::string cumulativeError;
	
	const std::string& mode = ftmp.getMode();

	//now look for a field in the message with a matching name
	const Field* msgField = msg.getField(ftmp.getModifiedName().c_str());

	if (msgField)
	{//we found the field

		//validate type
		if (!ftmp.isTypeVariable())
		{
			std::list<Field::Type> types = ftmp.getTypes();

			//get the list of possible valid types the field could have

			bool valid = false;
			for (std::list<Field::Type>::const_iterator it = types.begin(); it != types.end(); ++it)
			{//running through the list of types the field template has

				Field::Type type = *it;
					
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
				status.set(MSG_ERROR, INCORRECT_FIELD_TYPE, err.str().c_str());
				cumulativeError.append(err.str().c_str()).append(NEWLINE_INDENT1);
			}
		}
		
		//validate value
		if (!ftmp.getValues().empty())
		{
			//if the template has values in its list, the message field's value needs to be validated as well
			//for the field to be valid, its value must match at least one value in the field template's value list

			bool valid = false;

			Field::Type type = msgField->getType();

			const std::list<std::string>& values = ftmp.getValues();

			switch (type)
			{
			case Field::Type::CHAR:
			case Field::Type::STRING:
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

			case Field::Type::I8:
			case Field::Type::I16:
			case Field::Type::I32:
			case Field::Type::I64:
			{
				for (std::list<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
				{
					GMSEC_I64 msgValue = msgField->getI64Value();

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

			case Field::Type::U8:
			case Field::Type::U16:
			case Field::Type::U32:
			case Field::Type::U64:
			{
				for (std::list<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
				{
					GMSEC_U64 msgValue = msgField->getU64Value();

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

			case Field::Type::F32:
			case Field::Type::F64:
			{
				for (std::list<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
				{
					GMSEC_F64 msgValue = msgField->getF64Value();

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

			case Field::Type::BOOL:
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

			case Field::Type::BINARY:
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
				status.set(MSG_ERROR, FIELD_NOT_SET, err.str().c_str());
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
			status.set(MSG_ERROR, MISSING_REQUIRED_FIELD, err.str().c_str());
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

		status = Status(MSG_ERROR, FIELD_FAILED_VALIDATION, cumulativeError.c_str());
	}

	return status;
}


Status ComplianceValidator::validateContent(const InternalMessage& msg, const FieldTemplate& ftmp)
{
	Status status;

	const Field* msgField = msg.getField(ftmp.getModifiedName().c_str());

	if (msgField != NULL && ftmp.getMode() != "TRACKING")
	{
		if ((ftmp.getPattern() == "HEADER_STRING_Type") && !StringUtil::isValidHeaderString(msgField->getStringValue()))
		{
			std::stringstream err;
			err << msgField->getName() << " has illegal Header String value of '" << msgField->getStringValue() << "'";
			status.set(MSG_ERROR, FIELD_NOT_SET, err.str().c_str());
		}
		else if (ftmp.isHeader() != msgField->isHeader())
		{
			std::stringstream err;
			err << msgField->getName();
			if (!ftmp.isHeader() && msgField->isHeader()) {
				err << " is improperly identified as a header field";
			}
			else {
				err << " is not identified as a header field";
			}
			status.set(MSG_ERROR, FIELD_NOT_SET, err.str().c_str());
		}
		else if ((ftmp.getPattern() == "TIME_Type") && !StringUtil::isValidTimestamp(msgField->getStringValue()).isValid())
		{
			std::stringstream err;
			err << msgField->getName() << " has illegal Timestamp value of '" << msgField->getStringValue() << "'";
			status.set(MSG_ERROR, FIELD_NOT_SET, err.str().c_str());
		}
		else if ((ftmp.getPattern() == "IP_ADDRESS_Type") && !StringUtil::isValidIpAddress(msgField->getStringValue()))
		{
			std::stringstream err;
			err << msgField->getName() << " has illegal IP Address value of '" << msgField->getStringValue() << "'";
			status.set(MSG_ERROR, FIELD_NOT_SET, err.str().c_str());
		}
	}

	return status;
}


Status ComplianceValidator::checkTrackingField(const char* fieldName, const TrackingDetails& configTracking, const TrackingDetails& msgTracking)
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
		status.set(MSG_ERROR, NON_ALLOWED_FIELD, err.str().c_str());
	}

	return status;
}
