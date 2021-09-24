/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file MessagePopulator.cpp
 *
 *  @brief The MessagePopulator is intended to add fields to messages
 *	based on the ISD version specified.
 *
**/

#include <gmsec4/Errors.h>
#include <gmsec4/Exception.h>
#include <gmsec4/internal/mist/MessagePopulator.h>
#include <gmsec4/internal/field/InternalField.h>
#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/field/Field.h>
#include <gmsec4/field/BinaryField.h>
#include <gmsec4/field/BooleanField.h>
#include <gmsec4/field/CharField.h>
#include <gmsec4/field/F32Field.h>
#include <gmsec4/field/F64Field.h>
#include <gmsec4/field/I16Field.h>
#include <gmsec4/field/I32Field.h>
#include <gmsec4/field/I64Field.h>
#include <gmsec4/field/I8Field.h>
#include <gmsec4/field/StringField.h>
#include <gmsec4/field/U16Field.h>
#include <gmsec4/field/U32Field.h>
#include <gmsec4/field/U64Field.h>
#include <gmsec4/field/U8Field.h>

#include <gmsec4/util/DataList.h>
#include <gmsec4/util/Log.h>
#include <gmsec4/internal/SystemUtil.h>
#include <gmsec4/util/TimeUtil.h>

#include <gmsec4/internal/StringUtil.h>

#include <cstring>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::internal;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;

static const char* const HEADER_VERSION_STRING    = "HEADER-VERSION";
static const char* const MESSAGE_TYPE_STRING      = "MESSAGE-TYPE";
static const char* const MESSAGE_SUBTYPE_STRING   = "MESSAGE-SUBTYPE";
static const char* const CONTENT_VERSION_STRING   = "CONTENT-VERSION";
static const char* const REQ_STRING               = "REQ";
static const char* const RESP_STRING              = "RESP";
static const char* const SERV_STRING              = "SERV";
static const char* const OPERATION_NAME_STRING    = "OPERATION-NAME";
static const char* const OPERATION_NUMBER_STRING  = "OPERATION-NUMBER";
static const char* const DIR_STRING               = "DIR";
static const char* const MSG_ID_STRING            = "MSG-ID";
static const char* const MSG_STRING               = "MSG";
static const char* const RESPONSE_STATUS_STRING   = "RESPONSE-STATUS";
static const char* const C2CX_STRING              = "C2CX";
static const char* const EVENT_TIME_STRING        = "EVENT-TIME";
static const char* const C2CX_SUBTYPE_STRING      = "C2CX-SUBTYPE";
static const char* const RSRC_STRING              = "RSRC";
static const char* const COUNTER_STRING           = "COUNTER";


MessagePopulator::MessagePopulator(unsigned int isdVersion)
	: m_specVersion(isdVersion),
	m_standardFieldsAllMsgs()
	  
{
}


MessagePopulator::~MessagePopulator()
{
	destroyFields(m_standardFieldsAllMsgs);
}


void MessagePopulator::populateSimpleServiceMessage(Message &msg, const char * opName, const Field& opNumber, const DataList<Field*>& fields, const DataList<ServiceParam*>& sParams)
{
	msg.addField(MESSAGE_TYPE_STRING, REQ_STRING);
	msg.addField(MESSAGE_SUBTYPE_STRING, SERV_STRING);

	if(m_specVersion == GMSEC_ISD_2014_00){
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2010);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2012);
		msg.addField("NUM-OF-PARAMS", (GMSEC_I16) sParams.size());
		msg.addField(MSG_ID_STRING, generateUniqueID().c_str());
	}else if(m_specVersion == GMSEC_ISD_2016_00){
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2010);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2016);
		msg.addField("NUM-OF-PARAMS", (GMSEC_U16) sParams.size());
	}
	else if (m_specVersion == GMSEC_ISD_2018_00) {
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2018);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32)2018);
		msg.addField("NUM-OF-PARAMS", (GMSEC_U16)sParams.size());
	}else{
		GMSEC_WARNING << "Specification version unknown: " << m_specVersion << ", unable to autopopulate CONTENT-VERSION"
			<< " and NUM-OF-PARAMS in Simple Service message";
	}

	if (opName && !std::string(opName).empty())
	{
		msg.addField(OPERATION_NAME_STRING, opName);
	}

	GMSEC_I16 i16_value = 0;

	try
	{
		i16_value = dynamic_cast<const I16Field&>(opNumber).getValue();
	}
	catch (...)
	{
		GMSEC_ERROR << "MessagePopulator::populateSimpleServiceMessage():  Expected I16Field and got something different in a dynamic_cast, for .OPERATION-NUMBER.";
	}
	msg.addField(OPERATION_NUMBER_STRING, i16_value);

	for (DataList<Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
	{
		const Field* valueField = *it;

		if (valueField)
		{
			msg.addField(*valueField);
		}
	}

	int s_idx = 0;
	for (DataList<ServiceParam*>::const_iterator it = sParams.begin(); it != sParams.end(); ++it)
	{
		ServiceParam* current = *it;

		if (current == NULL)
		{
			continue;
		}

		std::ostringstream ss;
		ss << "PARAM." << (s_idx++ + 1);
		std::string sampleCountStr = ss.str();

		std::string nameCount = sampleCountStr;
		nameCount.append(".NAME");
		msg.addField(nameCount.c_str(), current->getName());

		std::string valCount = sampleCountStr;
		valCount.append(".VALUE");

		const Field& valueField = current->getValue();

		if (!addFieldToMsg(valueField, msg, valCount.c_str()))
		{
			throw Exception(MIST_ERROR,
			                FIELD_NOT_ADDED,
			                "MessagePopulator::populateSimpleServiceMessage():  Error while adding service param to message.");
		}	
	}
}


void MessagePopulator::populateDirective(Message &msg, const Field& directiveString, const DataList<Field*>& fields)
{
	msg.addField(MESSAGE_TYPE_STRING, REQ_STRING);
	msg.addField(MESSAGE_SUBTYPE_STRING, DIR_STRING);

	if (m_specVersion <= GMSEC_ISD_2014_00)
	{
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2010);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 1.0);
		msg.addField(MSG_ID_STRING, generateUniqueID().c_str());
	}
	else if (m_specVersion <= GMSEC_ISD_2016_00)
	{
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2010);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2016);
	}
	else if (m_specVersion <= GMSEC_ISD_2018_00)
	{
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2018);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2018);
	}
	else
	{
		GMSEC_WARNING << "Specification version unknown: " << m_specVersion << ", unable to autopopulate CONTENT-VERSION"
		              << " in Directive message";
	}
	
	msg.addField(directiveString);

	for (DataList<Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
	{
		const Field* valueField = *it;

		if (valueField)
		{
			msg.addField(*valueField);
		}
	}
}


void MessagePopulator::setStandardFields(const DataList<Field*>& standardFields)
{
	destroyFields(m_standardFieldsAllMsgs);

	for (DataList<Field*>::const_iterator it = standardFields.begin(); it != standardFields.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			m_standardFieldsAllMsgs.push_back(InternalField::makeFieldCopy(*field));
		}
		else
		{
			GMSEC_WARNING << "Field within DataList of standard fields is NULL";
		}
	}
}


const gmsec::api::util::DataList<Field*>& MessagePopulator::getStandardFields() const
{
	return m_standardFieldsAllMsgs;
}


void MessagePopulator::addStandardFields(Message& msg) const
{
	for (FieldList::const_iterator it = m_standardFieldsAllMsgs.begin(); it != m_standardFieldsAllMsgs.end(); ++it)
    {
		const Field* field = *it;

		msg.addField(*field);
	}
}


bool MessagePopulator::addFieldToMsg(const Field& fld, Message &msg, const char * nameOverwrite)
{
	bool ret_val = true;

	if(!nameOverwrite)
	{
		nameOverwrite = fld.getName();
	}

	switch (fld.getType())
	{
	  case gmsec::api::Field::BIN_TYPE:
	  {
		const BinaryField* binField = dynamic_cast<const BinaryField*>(&fld);
		if (binField)
		{
			msg.addField(nameOverwrite, binField->getValue(), binField->getLength());
		}
		else
		{
			ret_val = false;
		}
	  }
	  break;

	  case gmsec::api::Field::BOOL_TYPE:
	  {
		const BooleanField* boolField = dynamic_cast<const BooleanField*>(&fld);
		if (boolField)
		{
			msg.addField(nameOverwrite, boolField->getValue());
		}
		else
		{
			ret_val = false;
		}
	  }
	  break;

	  case gmsec::api::Field::CHAR_TYPE:
	  {
		const CharField* charField = dynamic_cast<const CharField*>(&fld);
		if (charField)
		{
			msg.addField(nameOverwrite, charField->getValue());
		}
		else
		{
			ret_val = false;
		}
	  }
	  break;

	  case gmsec::api::Field::F32_TYPE:
	  {
		const F32Field* f32Field = dynamic_cast<const F32Field*>(&fld);
		if (f32Field)
		{
			msg.addField(nameOverwrite, f32Field->getValue());
		}
		else
		{
			ret_val = false;
		}
	  }
	  break;

	  case gmsec::api::Field::F64_TYPE:
	  {
		const F64Field* f64Field = dynamic_cast<const F64Field*>(&fld);
		if (f64Field)
		{
			msg.addField(nameOverwrite, f64Field->getValue());
		}
		else
		{
			ret_val = false;
		}
	  }
	  break;

	  case gmsec::api::Field::I16_TYPE:
	  {
		const I16Field* i16Field = dynamic_cast<const I16Field*>(&fld);
		if (i16Field)
		{
			msg.addField(nameOverwrite, i16Field->getValue());
		}
		else
		{
			ret_val = false;
		}
	  }
	  break;

	  case gmsec::api::Field::I32_TYPE:
	  {
		const I32Field* i32Field = dynamic_cast<const I32Field*>(&fld);
		if (i32Field)
		{
			msg.addField(nameOverwrite, i32Field->getValue());
		}
		else
		{
			ret_val = false;
		}
	  }
	  break;

	  case gmsec::api::Field::I64_TYPE:
	  {
		const I64Field* i64Field = dynamic_cast<const I64Field*>(&fld);
		if (i64Field)
		{
			msg.addField(nameOverwrite, i64Field->getValue());
		}
		else
		{
			ret_val = false;
		}
	  }
	  break;

	  case gmsec::api::Field::I8_TYPE:
	  {
		const I8Field* i8Field = dynamic_cast<const I8Field*>(&fld);
		if (i8Field)
		{
			msg.addField(nameOverwrite, i8Field->getValue());
		}
		else
		{
			ret_val = false;
		}
	  }
	  break;

	  case gmsec::api::Field::STRING_TYPE:
	  {
		const StringField* strField = dynamic_cast<const StringField*>(&fld);
		if (strField)
		{
			msg.addField(nameOverwrite, strField->getValue());
		}
		else
		{
			ret_val = false;
		}
	  }
	  break;

	  case gmsec::api::Field::U16_TYPE:
	  {
		const U16Field* u16Field = dynamic_cast<const U16Field*>(&fld);
		if (u16Field)
		{
			msg.addField(nameOverwrite, u16Field->getValue());
		}
		else
		{
			ret_val = false;
		}
	  }
	  break;
	
	  case gmsec::api::Field::U32_TYPE:
	  {
		const U32Field* u32Field = dynamic_cast<const U32Field*>(&fld);
		if (u32Field)
		{
			msg.addField(nameOverwrite, u32Field->getValue());
		}
		else
		{
			ret_val = false;
		}
	  }
	  break;

	  case gmsec::api::Field::U64_TYPE:
	  {
		const U64Field* u64Field = dynamic_cast<const U64Field*>(&fld);
		if (u64Field)
		{
			msg.addField(nameOverwrite, u64Field->getValue());
		}
		else
		{
			ret_val = false;
		}
	  }
	  break;

	  case gmsec::api::Field::U8_TYPE:
	  {
		const U8Field* u8Field = dynamic_cast<const U8Field*>(&fld);
		if (u8Field)
		{
			msg.addField(nameOverwrite, u8Field->getValue());
		}
		else
		{
			ret_val = false;
		}
	  }
	  break;

	  default:
	  {
		ret_val = false;
	  }

	}// end switch

	return ret_val;

}


void MessagePopulator::destroyFields(FieldList& flist)
{
	for (FieldList::iterator it = flist.begin(); it != flist.end(); ++it)
	{
		delete *it;
	}

	flist.clear();
}


//DEPRECATED with API 4.5
#if 0
void MessagePopulator::populateLogMessage(Message &msg, const DataList<Field*>& logFields, FieldList &standardFieldList)
{
	for (FieldList::const_iterator it = standardFieldList.begin(); it != standardFieldList.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			msg.addField(*field);
		}
	}

	if (logFields.size() > 0)
	{
		for (DataList<Field*>::const_iterator it = logFields.begin(); it != logFields.end(); ++it)
		{
			const Field* field = *it;

			if (field)
			{
				msg.addField(*field);
			}
		}
	}

	msg.addField(StringField(MESSAGE_TYPE_STRING, MSG_STRING));
	msg.addField(StringField(MESSAGE_SUBTYPE_STRING, "LOG"));

	// If the message already contains the EVENT-TIME field, then do NOT overwrite it.
	if (msg.getField(EVENT_TIME_STRING) == NULL)
	{
		char eventTime[GMSEC_TIME_BUFSIZE] = {0};
		GMSEC_TimeSpec theTime = TimeUtil::getCurrentTime();
		TimeUtil::formatTime(theTime, eventTime);

		msg.addField(StringField(EVENT_TIME_STRING, const_cast<const char*>(eventTime)));
	}

	if (m_specVersion == GMSEC_ISD_2014_00)
	{
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32) 2010.0);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 1.0);
	}
	else if(m_specVersion == GMSEC_ISD_2016_00)
	{
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32) 2010.0);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2016);
	}
	else if (m_specVersion == GMSEC_ISD_2018_00)
	{
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32) 2018);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32)2018);
	}
	else
	{
		GMSEC_WARNING << "Specification version unknown: " << m_specVersion << ", unable to autopopulate CONTENT-VERSION"
			<< " in Log message";
	}
}
#endif


//DEPRECATED with API 4.5
#if 0
void MessagePopulator::populateHeartbeatMessage(Message &msg, const DataList<Field*>& hbFields, FieldList &standardFieldList)
{
	for (FieldList::const_iterator it = standardFieldList.begin(); it != standardFieldList.end(); ++it)
	{
		const Field* field = *it;

		if (field)
		{
			msg.addField(*field);
		}
	}

	if (hbFields.size() > 0)
	{
		for (DataList<Field*>::const_iterator it = hbFields.begin(); it != hbFields.end(); ++it)
		{
			const Field* field = *it;

			if (field)
			{
				msg.addField(*field);
			}
		}
	}

	//TODO (MEH): This should be modified to validate schema version down the road in case identifiers for HB message change
	msg.addField(StringField(MESSAGE_TYPE_STRING, MSG_STRING));
	msg.addField(StringField(MESSAGE_SUBTYPE_STRING, C2CX_STRING));
	msg.addField(StringField(C2CX_SUBTYPE_STRING, "HB"));

	if(m_specVersion == GMSEC_ISD_2014_00){
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32) 2010.0);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2013.0);
	}else if(m_specVersion == GMSEC_ISD_2016_00){
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32) 2010.0);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2016);
	}else if (m_specVersion == GMSEC_ISD_2018_00) {
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32) 2018);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32)2018);
	}else{
		GMSEC_WARNING << "Specification version unknown: " << m_specVersion << ", unable to autopopulate CONTENT-VERSION"
			<< " in Heartbeat message";
	}
}
#endif


void MessagePopulator::populateSimpleServiceAck(Message &msg, ResponseStatus::Response ssResponse, const DataList<Field*>& fields)
{
	msg.addField(MESSAGE_TYPE_STRING, RESP_STRING);
	msg.addField(MESSAGE_SUBTYPE_STRING, SERV_STRING);

	if(m_specVersion == GMSEC_ISD_2014_00){
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2010);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2012.0);
		msg.addField(MSG_ID_STRING, generateUniqueID().c_str());
	}else if(m_specVersion == GMSEC_ISD_2016_00){
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2010);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2016);
	}else if (m_specVersion == GMSEC_ISD_2018_00) {
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2018);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32)2018);
	}else{
		GMSEC_WARNING << "Specification version unknown: " << m_specVersion << ", unable to autopopulate CONTENT-VERSION"
			<< " in Simple Service response message";
	}

	msg.addField(RESPONSE_STATUS_STRING, (GMSEC_I16) ssResponse);

	for (DataList<Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
	{
		const Field* valueField = *it;

		if (valueField)
		{
			msg.addField(*valueField);
		}
	}
}


void MessagePopulator::populateDirectiveAck(Message &msg, ResponseStatus::Response ssResponse, const DataList<Field*>& fields)
{
	msg.addField(MESSAGE_TYPE_STRING, RESP_STRING);
	msg.addField(MESSAGE_SUBTYPE_STRING, DIR_STRING);

	if(m_specVersion == GMSEC_ISD_2014_00){
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2010);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 1.0);
		msg.addField(MSG_ID_STRING, generateUniqueID().c_str());
	}else if(m_specVersion == GMSEC_ISD_2016_00){
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2010);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2016);
	}else if (m_specVersion == GMSEC_ISD_2018_00) {
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2018);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32)2018);
	}else{
		GMSEC_WARNING << "Specification version unknown: " << m_specVersion << ", unable to autopopulate CONTENT-VERSION"
			<< " in Directive response message";
	}

	msg.addField(RESPONSE_STATUS_STRING, (GMSEC_I16) ssResponse);

	for (DataList<Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
	{
		const Field* valueField = *it;

		if (valueField)
		{
			msg.addField(*valueField);
		}
	}
}


//DEPRECATED with API 4.5
#if 0
void MessagePopulator::populateResourceStaticMembers(Message &msg, size_t counter)
{
	msg.addField(MESSAGE_TYPE_STRING, MSG_STRING);
	msg.addField(MESSAGE_SUBTYPE_STRING, C2CX_STRING);
	msg.addField(C2CX_SUBTYPE_STRING, RSRC_STRING);

	if (m_specVersion <= GMSEC_ISD_2014_00)
	{
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2010);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2010.0);
		msg.addField(COUNTER_STRING, (GMSEC_I16) counter);
	}
	else if (m_specVersion <= GMSEC_ISD_2016_00)
	{
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2010);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2016);
		msg.addField(COUNTER_STRING, (GMSEC_U16) counter);
	}
	else if (m_specVersion <= GMSEC_ISD_2018_00)
	{
		msg.addField(HEADER_VERSION_STRING, (GMSEC_F32)2018);
		msg.addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2018);
		msg.addField(COUNTER_STRING, (GMSEC_I16) counter);
	}
	else
	{
		GMSEC_WARNING << "Specification version unknown: " << m_specVersion << ", unable to autopopulate CONTENT-VERSION"
		              << " in Resource message";
	}
}
#endif


std::string MessagePopulator::generateUniqueID()
{
	std::string host;
	SystemUtil::getHostName(host);

	std::ostringstream uniqueID;
	uniqueID << host << "."
	         << SystemUtil::getProcessID() << "."
	         << TimeUtil::getCurrentTime().nanoseconds;

	return uniqueID.str();
}
