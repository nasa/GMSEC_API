/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_MESSAGE_H
#define GMSEC_API_INTERNAL_MESSAGE_H

#include <gmsec4/internal/MsgFieldMap.h>
#include <gmsec4/internal/json-forwards.h>
#include <gmsec4/internal/tinyxml2.h>
#include <gmsec4/internal/TrackingDetails.h>
#include <gmsec4/internal/Value.h>

#include <gmsec4/Config.h>
#include <gmsec4/Fields.h>
#include <gmsec4/Message.h>
#include <gmsec4/MessageFieldIterator.h>

#include <gmsec4/util/DataList.h>
#include <gmsec4/util/StdUniquePtr.h>
#include <gmsec4/util/wdllexp.h>

#include <string>


namespace gmsec
{
// Forward declaration(s)
class Message;

namespace api
{
// Forward declaration(s)
class Field;

namespace internal
{

class GMSEC_API InternalMessage
{
public:
	InternalMessage(const char* subject, Message::MessageKind kind);
	InternalMessage(const char* subject, Message::MessageKind kind, const Config& config);
	InternalMessage(const char* xml);
	InternalMessage(const InternalMessage& other);

	virtual ~InternalMessage();

	void CALL_TYPE addConfig(const Config& config);

	const Config& CALL_TYPE getConfig() const;

	const char* CALL_TYPE getSubject() const;

	void CALL_TYPE setSubject(const char* subject);

	Message::MessageKind CALL_TYPE getKind() const;

	// Only for use by middleware wrappers!
	void CALL_TYPE setKind(Message::MessageKind kind);

	bool CALL_TYPE addField(const Field& field, bool makeCopy = true);
	bool CALL_TYPE addField(const char* name, GMSEC_BIN bin, size_t len);
	bool CALL_TYPE addField(const char* name, bool value);
	bool CALL_TYPE addField(const char* name, GMSEC_CHAR value);
	bool CALL_TYPE addField(const char* name, GMSEC_I8 value);
	bool CALL_TYPE addField(const char* name, GMSEC_I16 value);
	bool CALL_TYPE addField(const char* name, GMSEC_I32 value);
	bool CALL_TYPE addField(const char* name, GMSEC_I64 value);
	bool CALL_TYPE addField(const char* name, GMSEC_F32 value);
	bool CALL_TYPE addField(const char* name, GMSEC_F64 value);
	bool CALL_TYPE addField(const char* name, const char* value);
	bool CALL_TYPE addField(const char* name, GMSEC_U8 value);
	bool CALL_TYPE addField(const char* name, GMSEC_U16 value);
	bool CALL_TYPE addField(const char* name, GMSEC_U32 value);
	bool CALL_TYPE addField(const char* name, GMSEC_U64 value);

	bool CALL_TYPE addFields(const gmsec::api::util::DataList<Field*>& fields);

	void CALL_TYPE clearFields();

	bool CALL_TYPE clearField(const char* name);

    GMSEC_I64 CALL_TYPE getIntegerValue(const char* fieldName) const;
    GMSEC_U64 CALL_TYPE getUnsignedIntegerValue(const char* fieldName) const;
    GMSEC_F64 CALL_TYPE getDoubleValue(const char* fieldName) const;
    const char* CALL_TYPE getStringValue(const char* fieldName) const;

	bool hasField(const char* name) const;

	const Field* CALL_TYPE getField(const char* name) const;

	Field::FieldType CALL_TYPE getFieldType(const char* name) const;

	const BinaryField& CALL_TYPE getBinaryField(const char* name) const;

	const BooleanField& CALL_TYPE getBooleanField(const char* name) const;

	const CharField& CALL_TYPE getCharField(const char* name) const;

	const F32Field& CALL_TYPE getF32Field(const char* name) const;

	const F64Field& CALL_TYPE getF64Field(const char* name) const;

	const I8Field& CALL_TYPE getI8Field(const char* name) const;

	const I16Field& CALL_TYPE getI16Field(const char* name) const;

	const I32Field& CALL_TYPE getI32Field(const char* name) const;

	const I64Field& CALL_TYPE getI64Field(const char* name) const;

	const U8Field& CALL_TYPE getU8Field(const char* name) const;

	const U16Field& CALL_TYPE getU16Field(const char* name) const;

	const U32Field& CALL_TYPE getU32Field(const char* name) const;

	const U64Field& CALL_TYPE getU64Field(const char* name) const;

	const StringField& CALL_TYPE getStringField(const char* name) const;

	size_t CALL_TYPE getFieldCount() const;

	void CALL_TYPE copyFields(InternalMessage& toMsg) const;

	const char* CALL_TYPE toXML() const;

	const char* CALL_TYPE toJSON() const;

	size_t CALL_TYPE getSize(const Message& msg) const;

	void CALL_TYPE resetFieldIterator();

	bool CALL_TYPE hasNextField() const;

	const Field& CALL_TYPE nextField();

	MessageFieldIterator& CALL_TYPE getFieldIterator(MessageFieldIterator::Selector selector = MessageFieldIterator::ALL_FIELDS);

	ValueMap& getDetails();

	void setTracking(const TrackingDetails& tracking);

	TrackingDetails& getTracking();

	void CALL_TYPE processConfig(const Config& config);

	bool CALL_TYPE processConfigValue(const char* name, const char* value);

private:
	// defined, but not implemented
	InternalMessage();

	void CALL_TYPE validateFieldName(const char* name) const;

	void CALL_TYPE fromXML(tinyxml2::XMLElement* element);

	void CALL_TYPE fromJSON(const Json::Value& root);

	typedef gmsec::api::util::StdUniquePtr<ValueMap> Details;

	std::string          m_subject;
	Message::MessageKind m_kind;
	Config               m_config;
	MsgFieldMap          m_fields;
	MessageFieldIterator m_fieldIterator;
	mutable std::string  m_xml;
	mutable std::string  m_json;
	Details              m_details;
	TrackingDetails      m_tracking;
};

}  // namespace internal
}  // namespace api
}  // namespace gmsec

#endif
