/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalMessage.h
 *
 * @brief This file contains the internal interface for a Message.
 */

#ifndef GMSEC_API5_INTERNAL_MESSAGE_H
#define GMSEC_API5_INTERNAL_MESSAGE_H

#include <gmsec5_defs.h>

#include <gmsec5/internal/json-forwards.h>
#include <gmsec5/internal/tinyxml2.h>

#include <gmsec5/internal/FieldTemplate.h>
#include <gmsec5/internal/MessageTemplate.h>
#include <gmsec5/internal/MiddlewareInfo.h>
#include <gmsec5/internal/MsgFieldMap.h>
#include <gmsec5/internal/TrackingDetails.h>
#include <gmsec5/internal/Value.h>

#include <gmsec5/Config.h>
#include <gmsec5/Fields.h>
#include <gmsec5/Message.h>
#include <gmsec5/MessageFieldIterator.h>
#include <gmsec5/Specification.h>
#include <gmsec5/Status.h>

#include <gmsec5/util/List.h>
#include <gmsec5/util/StdUniquePtr.h>
#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/wdllexp.h>

#include <string>
#include <stddef.h>  // for size_t


namespace gmsec
{
namespace api5
{
namespace internal
{
	// Forward declaration(s)
	class ComplianceValidator;


class GMSEC_API InternalMessage
{
public:
	//SubjectElement
	//first string: name of the element
	//second string: value of the element (name of the field that contains the value)
	typedef std::pair<std::string, std::string> SubjectElement;
	typedef std::vector<SubjectElement>         SubjectElementList;

	typedef gmsec::api5::util::StdSharedPtr<MiddlewareInfo> MWInfo;


	InternalMessage(Message& parent);

	InternalMessage(Message& parent, const InternalMessage& other);

	virtual ~InternalMessage();

	void acknowledge();

	static void destroy(Message*& msg);

	const char* CALL_TYPE getSchemaID() const;

	Status CALL_TYPE isCompliant() const;

    void CALL_TYPE registerMessageValidator(MessageValidator* validator);
    void CALL_TYPE registerMessageValidator(GMSEC_MessageValidator* validator);

	bool CALL_TYPE setFieldValue(const char* fieldName, const char* value, bool convert = true);
	bool CALL_TYPE setFieldValue(const char* fieldName, GMSEC_I16 value);
	bool CALL_TYPE setFieldValue(const char* fieldName, GMSEC_U16 value);
	bool CALL_TYPE setFieldValue(const char* fieldName, GMSEC_I32 value);
	bool CALL_TYPE setFieldValue(const char* fieldName, GMSEC_U32 value);
	bool CALL_TYPE setFieldValue(const char* fieldName, GMSEC_I64 value, Field::Type desiredFieldType = Field::Type::I64);
	bool CALL_TYPE setFieldValue(const char* fieldName, GMSEC_U64 value);
	bool CALL_TYPE setFieldValue(const char* fieldName, GMSEC_F64 value, Field::Type desiredFieldType = Field::Type::F64);

	void CALL_TYPE setConfig(const Config& config);
	Config& CALL_TYPE getConfig();
	const Config& CALL_TYPE getConfig() const;

	void CALL_TYPE setSubject(const char* subject);
	const char* CALL_TYPE getSubject() const;

	const char* CALL_TYPE getResponseTopic(bool useWildcards = false) const;

	void CALL_TYPE setKind(Message::Kind kind);
	Message::Kind CALL_TYPE getKind() const;

	bool CALL_TYPE addField(const Field& field, bool makeCopy = true);
	bool CALL_TYPE addField(const char* name, const GMSEC_U8* blob, size_t len);
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

	bool CALL_TYPE addFields(const gmsec::api5::util::List<Field*>& fields);

	void CALL_TYPE clearFields();

	bool CALL_TYPE clearField(const char* name);

	bool CALL_TYPE hasField(const char* name) const;

    const char* CALL_TYPE getStringValue(const char* fieldName) const;
	bool CALL_TYPE getBooleanValue(const char* fieldName) const;
    GMSEC_I16 CALL_TYPE getI16Value(const char* fieldName) const;
    GMSEC_I32 CALL_TYPE getI32Value(const char* fieldName) const;
    GMSEC_I64 CALL_TYPE getI64Value(const char* fieldName) const;
    GMSEC_U16 CALL_TYPE getU16Value(const char* fieldName) const;
    GMSEC_U32 CALL_TYPE getU32Value(const char* fieldName) const;
    GMSEC_U64 CALL_TYPE getU64Value(const char* fieldName) const;
    GMSEC_F64 CALL_TYPE getF64Value(const char* fieldName) const;

	const Field* CALL_TYPE getField(const char* name) const;

	Field::Type CALL_TYPE getFieldType(const char* name) const;

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

	MessageFieldIterator& CALL_TYPE getFieldIterator(MessageFieldIterator::Selector selector = MessageFieldIterator::Selector::ALL_FIELDS);

	ValueMap& CALL_TYPE getDetails() const;

	void CALL_TYPE setTracking(const TrackingDetails& tracking);

	TrackingDetails& CALL_TYPE getTracking();

	void CALL_TYPE processConfig(const Config& config);

	bool CALL_TYPE processConfigValue(const char* name, const char* value);

	void CALL_TYPE resetFieldIterator();

	bool CALL_TYPE hasNextField() const;

	const Field& CALL_TYPE nextField();

	void CALL_TYPE setMiddlewareInfo(MWInfo& info);

	MWInfo& CALL_TYPE getMiddlewareInfo();

	void CALL_TYPE setVersion(unsigned int version);
	unsigned int CALL_TYPE getVersion() const;

	void CALL_TYPE setSchemaLevel(Specification::SchemaLevel level);
	Specification::SchemaLevel CALL_TYPE getSchemaLevel() const;

	void CALL_TYPE setTemplate(gmsec::api5::util::StdSharedPtr<MessageTemplate> msgTemplate);
	const gmsec::api5::internal::MessageTemplate* CALL_TYPE getTemplate() const;

	void CALL_TYPE init();

	std::string CALL_TYPE buildSubject(bool forTopic = false, bool useWildcards = false) const;

	void CALL_TYPE processData(const char* data, DataType type);

private:
	// defined, but not implemented
	InternalMessage& operator=(const InternalMessage&);

	void fromXML(tinyxml2::XMLElement* element);

	void fromJSON(const Json::Value& root);

	void validateFieldName(const char* name) const;

	const gmsec::api5::internal::FieldTemplate& findFieldTemplate(const char* fieldName);


	typedef gmsec::api5::util::StdUniquePtr<ValueMap>            SafeDetails;
	typedef gmsec::api5::util::StdSharedPtr<MessageTemplate>     SafeMessageTemplate;
	typedef gmsec::api5::util::StdUniquePtr<ComplianceValidator> SafeComplianceValidator;

	Message&                     m_parent;
	Config                       m_config;
	MsgFieldMap                  m_fields;
	MessageFieldIterator         m_fieldIterator;
	SafeDetails                  m_details;
	TrackingDetails              m_tracking;
	bool                         m_valid;
	std::string	                 m_valueBuffer;

	SafeMessageTemplate          m_template;
	unsigned int                 m_msgVersion;
	Specification::SchemaLevel   m_msgSchemaLevel;

	mutable Message::Kind        m_kind;
	mutable std::string          m_subject;
	mutable std::string          m_responseTopic;
	mutable std::string          m_xml;
	mutable std::string          m_json;
	bool                         m_subjectSet;
	bool                         m_kindSet;
	bool                         m_showMsgConfig;

	MWInfo                       m_mwInfo;

	SafeComplianceValidator      m_complianceValidator;
};

}  // end namespace internal
}  // end namespace api5
}  // end namespace gmsec

#endif
