/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_FIELD_H
#define GMSEC_API5_INTERNAL_FIELD_H

#include <gmsec5/field/Field.h>

#include <gmsec5/internal/tinyxml2.h>
#include <gmsec5/internal/json-forwards.h>

#include <gmsec5/util/wdllexp.h>

#include <string>
#include <sstream>


namespace gmsec
{
namespace api5
{
namespace internal
{


class GMSEC_API InternalField
{
public:
	virtual ~InternalField();

	const char* CALL_TYPE getName() const;

	Field::Type CALL_TYPE getType() const;

	bool CALL_TYPE isHeader() const;

	bool CALL_TYPE isTracking() const;

	void CALL_TYPE isTracking(bool tracking);

	virtual const char* CALL_TYPE toXML() const = 0;
	virtual const char* CALL_TYPE toJSON() const = 0;

	const char* CALL_TYPE getStringValue() const;
	bool CALL_TYPE getBooleanValue() const;
	GMSEC_I16 CALL_TYPE getI16Value() const;
	GMSEC_I32 CALL_TYPE getI32Value() const;
	GMSEC_I64 CALL_TYPE getI64Value() const;
	GMSEC_U16 CALL_TYPE getU16Value() const;
	GMSEC_U32 CALL_TYPE getU32Value() const;
	GMSEC_U64 CALL_TYPE getU64Value() const;
	GMSEC_F64 CALL_TYPE getF64Value() const;

	static void CALL_TYPE identifyHeaderField(bool flag);

	// For internal use only!
	//
	static bool CALL_TYPE identifyHeaderField();

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
	static Field::Type CALL_TYPE lookupType(const char* typeStr);

	// For internal use only!
	//
	static std::string CALL_TYPE lookupTypeStr(Field::Type ftype);

	// For internal use only!
	//
	static bool CALL_TYPE isFieldNameCompliant(const char* name);

protected:
	InternalField(const char* name, Field::Type type, bool isHeader);

	mutable std::string m_xml;

	mutable std::string m_json;

private:
	static Field* createField(const char* name, const char* type, const char* value, const char* bits, const char* head); 

	std::string         m_name;
	Field::Type         m_type;
	bool                m_header;
	bool                m_tracking;

	static bool         s_identifyHeaderField;

	mutable std::string m_string;
};


class FieldBuddy
{
public:
	static InternalField& getInternal(const Field& field)
	{
		return *(field.m_internal);
	}
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
