/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_FIELD_H
#define GMSEC_API_INTERNAL_FIELD_H

#include <gmsec4/field/Field.h>

#include <gmsec4/internal/tinyxml2.h>
#include <gmsec4/internal/json-forwards.h>

#include <gmsec4/util/wdllexp.h>

#include <string>
#include <sstream>


namespace gmsec
{
namespace api
{
namespace internal
{


class GMSEC_API InternalField
{
public:
	virtual ~InternalField();

	const char* CALL_TYPE getName() const;
	void CALL_TYPE setName(const char* name);

	Field::FieldType CALL_TYPE getType() const;
	void CALL_TYPE setType(Field::FieldType type);

	bool CALL_TYPE isHeader() const;
	void CALL_TYPE isHeader(bool isHeader);

	virtual const char* CALL_TYPE toXML() const = 0;
	virtual const char* CALL_TYPE toJSON() const = 0;

	GMSEC_I64 getIntegerValue() const;
	GMSEC_U64 getUnsignedIntegerValue() const;
	GMSEC_F64 getDoubleValue() const;
	const char* getStringValue() const;

	// For internal use only!
	//
	static Field* CALL_TYPE fromXML(tinyxml2::XMLElement* element);

	// For internal use only!
	//
	static Field* CALL_TYPE fromJSON(const Json::Value& root);

	// For internal use only!
	//
	static Field* CALL_TYPE makeFieldCopy(const Field& other);

	// For internal use only!
	//
	static bool CALL_TYPE testEquals(const Field& first, const Field& second);

	static Field::FieldType CALL_TYPE lookupType(const char* typeStr);

	static std::string CALL_TYPE lookupTypeStr(Field::FieldType ftype);

protected:
	InternalField(const char* name, Field::FieldType type);

	mutable std::string m_xml;

	mutable std::string m_json;

private:
	static Field* createField(const char* name, const char* type, const char* value, const char* bits, const char* head); 

	std::string         m_name;
	Field::FieldType    m_type;
	bool                m_header;

	mutable std::string m_string;
};

} //namespace internal
} //namespace api
} //namespace gmsec

#endif
