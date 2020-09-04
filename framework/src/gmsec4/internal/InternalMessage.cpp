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

#include <gmsec4/internal/InternalMessage.h>

#include <gmsec4/internal/Encoder.h>
#include <gmsec4/internal/InternalConfig.h>
#include <gmsec4/internal/InternalMessageFieldIterator.h>
#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>

#include <gmsec4/util/Buffer.h>
#include <gmsec4/util/Log.h>


#include <tinyxml2.h>
#include <json.h>

#include <algorithm>
#include <cctype>
#include <sstream>


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;


static const char*  MSG_FLD_STORAGE_TYPE_OPT = "gmsec-msgfld-store-type";
static const char*  MSG_FLD_STORAGE_SIZE_OPT = "gmsec-msgfld-store-size";
static const char*  TREE_TYPE                = "tree";
static const char*  HASH_TYPE                = "hash";
static const size_t MSG_FLD_STORAGE_ROLLOVER = 50;


InternalMessage::InternalMessage(const char* subject, Message::MessageKind kind)
	: m_subject(),
	  m_kind(kind),
	  m_config(),
	  m_fields(MsgFieldMap::BINARY_TREE_MAP, MSG_FLD_STORAGE_ROLLOVER),
	  m_fieldIterator(*this),
	  m_xml()
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MSG_ERROR, INVALID_SUBJECT_NAME,
				"Subject cannot be NULL, nor an empty string");
	}

	m_subject = subject;
}


InternalMessage::InternalMessage(const char* subject, Message::MessageKind kind, const Config& config)
	: m_subject(),
	  m_kind(kind),
	  m_config(config),
	  m_fields(MsgFieldMap::BINARY_TREE_MAP, MSG_FLD_STORAGE_ROLLOVER),
	  m_fieldIterator(*this),
	  m_xml()
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MSG_ERROR, INVALID_SUBJECT_NAME,
				"Subject cannot be NULL, nor an empty string");
	}

	m_subject = subject;

	processConfig(config);
}


InternalMessage::InternalMessage(const char* data)
	: m_subject(),
	  m_config(),
	  m_fields(MsgFieldMap::BINARY_TREE_MAP, MSG_FLD_STORAGE_ROLLOVER),
	  m_fieldIterator(*this),
	  m_xml(),
	  m_json()
{
	if (!data || std::string(data).empty())
	{
		throw Exception(MSG_ERROR, PARSE_ERROR,
			"data string cannot be NULL, nor empty.");
	}

	// Use this to determine if any of the parsing functionalities worked
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError code = doc.Parse(data);

	if (code == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement* element = doc.RootElement();

		if (!element)
		{
			throw Exception(MSG_ERROR, XML_PARSE_ERROR,
				"Invalid XML format -- no root element");
		}

		fromXML(element);

		// Assuming it reached here without throwing an exception: Success!
		return;
	}

	Json::Reader reader;
	Json::Value root;
	bool success = reader.parse(data, root);

	if (success)
	{
		if (!root)
		{
			throw Exception(MSG_ERROR, JSON_PARSE_ERROR,
				"Invalid JSON message format -- invalid JSON string data");
		}

		fromJSON(root);

		// Assuming it reached here without throwing an exception: Success!
		return;
	}

	// If none of the return statements were hit above, that means the data was unable to be parsed.
	throw Exception(MSG_ERROR, PARSE_ERROR,
		"Unable to parse data string, string must be a valid XML or JSON data structure.");
}


InternalMessage::InternalMessage(const InternalMessage& other)
	: m_subject(other.m_subject),
	  m_kind(other.m_kind),
	  m_config(other.m_config),
	  m_fields(other.m_fields.getStorageType(), other.m_fields.getRolloverLimit()),
	  m_fieldIterator(*this),
	  m_xml()
{
	processConfig(m_config);

	// copy standard fields (if any)
	other.copyFields(*this);

	// copy meta-data (if any)
	this->getDetails().cloneValues(const_cast<InternalMessage&>(other).getDetails());
}


InternalMessage::~InternalMessage()
{
	clearFields();
}


void InternalMessage::addConfig(const Config& config)
{
	const char* name  = 0;
	const char* value = 0;

	bool hasConfig = config.getFirst(name, value);

	while (hasConfig)
	{
		m_config.addValue(name, value);

		hasConfig = config.getNext(name, value);
	}

	processConfig(config);
}


const Config& InternalMessage::getConfig() const
{
	return m_config;
}


const char* InternalMessage::getSubject() const
{
	return m_subject.c_str();
}


void InternalMessage::setSubject(const char* subject)
{
	if (!subject || std::string(subject).empty())
	{
		throw Exception(MSG_ERROR, INVALID_SUBJECT_NAME,
				"Subject cannot be NULL, nor an empty string");
	}

	m_subject = subject;
}


Message::MessageKind InternalMessage::getKind() const
{
	return m_kind;
}


void InternalMessage::setKind(Message::MessageKind kind)
{
	m_kind = kind;
}


bool InternalMessage::addField(const Field& field)
{
	return m_fields.addField(field);
}


bool InternalMessage::addField(const char* name, GMSEC_BIN bin, size_t len)
{
	return addField(BinaryField(name, bin, len));
}


bool InternalMessage::addField(const char* name, bool value)
{
	return addField(BooleanField(name, value));
}


bool InternalMessage::addField(const char* name, GMSEC_CHAR value)
{
	return addField(CharField(name, value));
}


bool InternalMessage::addField(const char* name, GMSEC_I8 value)
{
	return addField(I8Field(name, value));
}


bool InternalMessage::addField(const char* name, GMSEC_I16 value)
{
	return addField(I16Field(name, value));
}


bool InternalMessage::addField(const char* name, GMSEC_I32 value)
{
	return addField(I32Field(name, value));
}


bool InternalMessage::addField(const char* name, GMSEC_I64 value)
{
	return addField(I64Field(name, value));
}


bool InternalMessage::addField(const char* name, GMSEC_F32 value)
{
	return addField(F32Field(name, value));
}


bool InternalMessage::addField(const char* name, GMSEC_F64 value)
{
	return addField(F64Field(name, value));
}


bool InternalMessage::addField(const char* name, const char* value)
{
	return addField(StringField(name, value));
}


bool InternalMessage::addField(const char* name, GMSEC_U8 value)
{
	return addField(U8Field(name, value));
}


bool InternalMessage::addField(const char* name, GMSEC_U16 value)
{
	return addField(U16Field(name, value));
}


bool InternalMessage::addField(const char* name, GMSEC_U32 value)
{
	return addField(U32Field(name, value));
}


bool InternalMessage::addField(const char* name, GMSEC_U64 value)
{
	return addField(U64Field(name, value));
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


GMSEC_I64 InternalMessage::getIntegerValue(const char* fieldName) const
{
	const Field* field = getField(fieldName);

	if (field)
	{
		return field->getIntegerValue();
	}

	throw Exception(MSG_ERROR, INVALID_FIELD, "Field not found");
}


GMSEC_U64 InternalMessage::getUnsignedIntegerValue(const char* fieldName) const
{
	const Field* field = getField(fieldName);

	if (field)
	{
		return field->getUnsignedIntegerValue();
	}

	throw Exception(MSG_ERROR, INVALID_FIELD, "Field not found");
}


GMSEC_F64 InternalMessage::getDoubleValue(const char* fieldName) const
{
	const Field* field = getField(fieldName);

	if (field)
	{
		return field->getDoubleValue();
	}

	throw Exception(MSG_ERROR, INVALID_FIELD, "Field not found");
}


const char* InternalMessage::getStringValue(const char* fieldName) const
{
	const Field* field = getField(fieldName);

	if (field)
	{
		return field->getStringValue();
	}

	throw Exception(MSG_ERROR, INVALID_FIELD, "Field not found");
}


const Field* InternalMessage::getField(const char* name) const
{
	validateFieldName(name);

	return m_fields.getField(name);
}


Field::FieldType InternalMessage::getFieldType(const char* name) const
{
	const Field* field = getField(name);

	if (!field)
	{
		std::string errmsg = "Message does not contain field with name: ";
		errmsg += name;
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME, errmsg.c_str());
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

	oss << "<MESSAGE SUBJECT=\"" << m_subject << "\" ";

	switch (m_kind)
	{
		case Message::PUBLISH:
			oss << "KIND=\"PUBLISH\"";
			break;

		case Message::REQUEST:
			oss << "KIND=\"REQUEST\"";
			break;

		case Message::REPLY:
			oss << "KIND=\"REPLY\"";
			break;
	}

	const char* cfgXML = m_config.toXML();
	const char* fldXML = m_fields.toXML();

	bool haveCfg = false;
	bool haveFld = false;

	if (StringUtil::stringEquals(cfgXML, "<CONFIG>\n</CONFIG>") == false)
	{
		haveCfg = true;
	}
	if (m_fields.getFieldCount() > 0)
	{
		haveFld = true;
	}
	if (haveCfg)
	{
		oss << ">" << "\n";

		oss << cfgXML << "\n";
	}
	if (haveFld)
	{
		if (!haveCfg)
		{
			oss << ">" << "\n";
		}

		oss << fldXML;
	}
	if (haveCfg || haveFld)
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

	jsonStream << "{\"MESSAGE\":{";

	jsonStream << "\"SUBJECT\":\"" << m_subject << "\",";

	switch (m_kind)
	{
		case Message::PUBLISH:
			jsonStream << "\"KIND\":\"PUBLISH\",";
			break;

		case Message::REQUEST:
			jsonStream << "\"KIND\":\"REQUEST\",";
			break;

		case Message::REPLY:
			jsonStream << "\"KIND\":\"REPLY\",";
			break;
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


void InternalMessage::processConfig(const Config& config)
{
	const char* name  = 0;
	const char* value = 0;

	bool hasNext = config.getFirst(name, value);
	while (hasNext)
	{
		if (!processConfigValue(name, value))
		{
			GMSEC_WARNING << "Message configuration item [" << name << "," << value << "] was not used.";
		}

		hasNext = config.getNext(name, value);
	}
}


bool InternalMessage::processConfigValue(const char* name, const char* value)
{
	if (StringUtil::stringEqualsIgnoreCase(name, MSG_FLD_STORAGE_TYPE_OPT))
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
			ss << MSG_FLD_STORAGE_TYPE_OPT << " must be either '" << TREE_TYPE
			   << "' or '" << HASH_TYPE << "'; got '" << value << "'.";
			throw Exception(MSG_ERROR, UNUSED_CONFIG_ITEM, ss.str().c_str());
		}
	}
	else if (StringUtil::stringEqualsIgnoreCase(name, MSG_FLD_STORAGE_SIZE_OPT))
	{
		std::stringstream ss(value);
		int limit = 0;
		ss >> limit;
		if (!ss.fail() && ss.eof())
		{
			m_fields.setRolloverLimit(limit);
			return true;
		}
		else
		{
			std::stringstream ss;
			ss << MSG_FLD_STORAGE_SIZE_OPT << " value must be a number; got '" << value << "'.";
			throw Exception(MSG_ERROR, UNUSED_CONFIG_ITEM, ss.str().c_str());
		}
	}
	else if (StringUtil::stringEqualsIgnoreCase(name, "subject"))
	{
		setSubject(value);
		return true;
	}
	else if (StringUtil::stringEqualsIgnoreCase(name, "kind"))
	{
		Message::MessageKind kind = Message::PUBLISH;

		if (StringUtil::stringEqualsIgnoreCase(value, "publish") || StringUtil::stringEqualsIgnoreCase(value, "GMSEC_MSG_PUBLISH"))
		{
			kind = Message::PUBLISH;
		}
		else if (StringUtil::stringEqualsIgnoreCase(value, "request") || StringUtil::stringEqualsIgnoreCase(value, "GMSEC_MSG_REQUEST"))
		{
			kind = Message::REQUEST;
		}
		else if (StringUtil::stringEqualsIgnoreCase(value, "reply") || StringUtil::stringEqualsIgnoreCase(value, "GMSEC_MSG_REPLY"))
		{
			kind = Message::REPLY;
		}

		setKind(kind);
		return true;
	}

	TrackingDetails& tracking = this->getTracking();

	static const int TRACK_OFF = MESSAGE_TRACKINGFIELDS_OFF;
	static const int TRACK_ON  = MESSAGE_TRACKINGFIELDS_ON;

	// Set the tracking field state
	if (StringUtil::stringEqualsIgnoreCase(name, "TRACKING"))
	{
		tracking.set(StringUtil::stringEqualsIgnoreCase(value, "FALSE") ? TRACK_OFF : TRACK_ON);
		return true;
	}

	else if (StringUtil::stringEqualsIgnoreCase(name, "TRACKING-NODE"))
	{
		tracking.setNode(StringUtil::stringEqualsIgnoreCase(value, "FALSE") ? TRACK_OFF : TRACK_ON);
		return true;
	}

	else if (StringUtil::stringEqualsIgnoreCase(name, "TRACKING-PROCESS-ID"))
	{
		tracking.setProcessId(StringUtil::stringEqualsIgnoreCase(value, "FALSE") ? TRACK_OFF : TRACK_ON);
		return true;
	}

	else if (StringUtil::stringEqualsIgnoreCase(name, "TRACKING-USER-NAME"))
	{
		tracking.setUserName(StringUtil::stringEqualsIgnoreCase(value, "FALSE") ? TRACK_OFF : TRACK_ON);
		return true;
	}

	else if (StringUtil::stringEqualsIgnoreCase(name, "TRACKING-CONNECTION-ID"))
	{
		tracking.setConnectionId(StringUtil::stringEqualsIgnoreCase(value, "FALSE") ? TRACK_OFF : TRACK_ON);
		return true;
	}

	else if (StringUtil::stringEqualsIgnoreCase(name, "TRACKING-PUBLISH-TIME"))
	{
		tracking.setPublishTime(StringUtil::stringEqualsIgnoreCase(value, "FALSE") ? TRACK_OFF : TRACK_ON);
		return true;
	}

	else if (StringUtil::stringEqualsIgnoreCase(name, "TRACKING-UNIQUE-ID"))
	{
		tracking.setUniqueId(StringUtil::stringEqualsIgnoreCase(value, "FALSE") ? TRACK_OFF : TRACK_ON);
		return true;
	}

	else if (StringUtil::stringEqualsIgnoreCase(name, "TRACKING-MW-INFO"))
	{
		tracking.setMwInfo(StringUtil::stringEqualsIgnoreCase(value, "FALSE") ? TRACK_OFF : TRACK_ON);
		return true;
	}

	return false;
}


void InternalMessage::validateFieldName(const char* name) const
{
	if (!name || std::string(name).empty())
	{
		throw Exception(MSG_ERROR, INVALID_FIELD_NAME,
				"Field name cannot be NULL, nor an empty string");
	}
}


void InternalMessage::fromXML(tinyxml2::XMLElement* element)
{
	if (!element)
	{
		throw Exception(MSG_ERROR, XML_PARSE_ERROR, "Invalid XML -- element is NULL");
	}

	/* verify that our root node is "MESSAGE" */
	const char* mename = element->Name();
	if (!mename || std::string(mename) != "MESSAGE")
	{
		throw Exception(MSG_ERROR, XML_PARSE_ERROR, "Invalid XML -- MESSAGE element not found");
	}

	const char* subject = 0;
	const char* mtype   = 0;

	for (const tinyxml2::XMLAttribute *attr = element->FirstAttribute(); attr; attr = attr->Next())
	{
		const char* caname = attr->Name();

		if (caname && std::string(caname) == "SUBJECT")
		{
			subject = attr->Value();
		}
		else if (caname && (std::string(caname) == "KIND" || std::string(caname) == "TYPE"))
		{
			mtype = attr->Value();
		}
	}

	if (subject)
	{
		m_subject = subject;
	}
	else
	{
		throw Exception(MSG_ERROR, XML_PARSE_ERROR,
				"Invalid XML -- MESSAGE element does not contain SUBJECT definition");
	}

	// set kind
	if (mtype)
	{
		std::string upperMsgType = mtype;
		std::transform(upperMsgType.begin(), upperMsgType.end(), upperMsgType.begin(), ::toupper);

		if (upperMsgType == "PUBLISH")
		{
			m_kind = Message::PUBLISH;
		}
		else if (upperMsgType == "REQUEST")
		{
			m_kind = Message::REQUEST;
		}
		else if (upperMsgType == "REPLY")
		{
			m_kind = Message::REPLY;
		}
		else
		{
			throw Exception(MSG_ERROR, XML_PARSE_ERROR,
					"Invalid XML -- MESSAGE element contains invalid KIND/TYPE definition");
		}
	}
	else
	{
		throw Exception(MSG_ERROR, XML_PARSE_ERROR,
				"Invalid XML -- MESSAGE element does not contain KIND/TYPE definition");
	}

	// fields (child nodes)
	for (tinyxml2::XMLElement* node = element->FirstChildElement(); node; node = node->NextSiblingElement())
	{
		//field nodes only
		const char* cnname = node->Name();
		if (cnname && std::string(cnname) == "FIELD")
		{
			Field* field = InternalField::fromXML(node);

			if (field)
			{
				addField(*field);
				delete field;
			}
		}
		// configuration node(s)
		else if (cnname && std::string(cnname) == "CONFIG")
		{
			try
			{
				// only the InternalConfig has the means to ingest an XML document node
				InternalConfig config;
				config.fromXML(node);

				// cannot construct Config using InternalConfig, thus we need to iterate
				// manually over each name/value pair in the InternalConfig, and then
				// add pair to our InternalMessage's config object.
				const char* name  = 0;
				const char* value = 0;

				bool result = config.getFirst(name, value);

				while (result)
				{
					m_config.addValue(name, value);

					result = config.getNext(name, value);
				}

				// apply the config to our message
				processConfig(m_config);
			}
			catch (Exception& e)
			{
				throw Exception(MSG_ERROR, XML_PARSE_ERROR, e.what());
			}
		}
	}
}


void InternalMessage::fromJSON(const Json::Value& origRoot)
{
	Json::Value root;
	if (origRoot.isMember(std::string("MESSAGE")))
	{
		root = origRoot["MESSAGE"];
	}
	else
	{
		throw Exception(MSG_ERROR, JSON_PARSE_ERROR,
			"Invalid JSON Message format -- no root element");
	}

	const char* subject = NULL;
	if (root.isMember(std::string("SUBJECT")))
	{
		subject = root["SUBJECT"].asCString();
	}
	else
	{
		throw Exception(MSG_ERROR, JSON_PARSE_ERROR,
			"Invalid JSON Message format - no SUBJECT provided");
	}

	const char* kind = NULL;
	if (root.isMember(std::string("KIND")))
	{
		kind = root["KIND"].asCString();
	}
	else
	{
		throw Exception(MSG_ERROR, JSON_PARSE_ERROR,
			"Invalid JSON Message format - no KIND provided");
	}

	// Set subject
	m_subject = subject;

	// Set kind
	if (kind != NULL)
	{
		std::string upperMsgType = kind;
		std::transform(upperMsgType.begin(), upperMsgType.end(), upperMsgType.begin(), ::toupper);

		if (upperMsgType == "PUBLISH")
		{
			m_kind = Message::PUBLISH;
		}
		else if (upperMsgType == "REQUEST")
		{
			m_kind = Message::REQUEST;
		}
		else if (upperMsgType == "REPLY")
		{
			m_kind = Message::REPLY;
		}
		else
		{
			throw Exception(MSG_ERROR, XML_PARSE_ERROR,
				"Invalid JSON Message format -- invalid KIND provided");
		}
	}

	if (root.isMember(std::string("FIELD")))
	{
		kind = root["KIND"].asCString();
		const Json::Value fields = root["FIELD"];

		// Fields
		for (unsigned int i = 0; i < fields.size(); i++)
		{
			Field* field = InternalField::fromJSON(fields[i]); 
			if (field)
			{
				addField(*field);
				delete field;
			}
		}
	}
}


MessageFieldIterator& InternalMessage::getFieldIterator(MessageFieldIterator::Selector selector)
{
	m_fieldIterator.m_iter->setSelector(selector);
	m_fieldIterator.m_iter->reset();
	return m_fieldIterator;
}


ValueMap& InternalMessage::getDetails()
{
	if (!m_details.get())
	{
		m_details.reset(new ValueMap());
	}

	return *m_details;
}


void InternalMessage::setTracking(const TrackingDetails& tracking)
{
	m_tracking.set(tracking);
}


TrackingDetails& InternalMessage::getTracking()
{
	return m_tracking;
}
