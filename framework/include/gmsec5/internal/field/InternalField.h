/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_FIELD_H
#define GMSEC_API5_INTERNAL_FIELD_H

#include <gmsec5/field/Field.h>

#include <gmsec5/internal/tinyxml2.h>
#include <gmsec5/internal/json-forwards.h>

#include <string>
#include <sstream>


namespace gmsec
{
namespace api5
{
namespace internal
{


class InternalField
{
public:
	virtual ~InternalField();

	const char* getName() const;

	Field::Type getType() const;

	bool isHeader() const;

	virtual const char* toXML() const = 0;
	virtual const char* toJSON() const = 0;

	const char* getStringValue() const;
	bool getBooleanValue() const;
	GMSEC_I16 getI16Value() const;
	GMSEC_I32 getI32Value() const;
	GMSEC_I64 getI64Value() const;
	GMSEC_U16 getU16Value() const;
	GMSEC_U32 getU32Value() const;
	GMSEC_U64 getU64Value() const;
	GMSEC_F64 getF64Value() const;

	static void identifyHeaderField(bool flag);

	// For internal use only!
	//
	static bool identifyHeaderField();


	// For internal use only!
	//
	static Field* fromXML(tinyxml2::XMLElement* element);

	// For internal use only!
	//
	static Field* fromJSON(const Json::Value& root);

	// For internal use only!
	//
	static Field* makeFieldCopy(const Field& other);

	// For internal use only!
	//
	static Field::Type lookupType(const char* typeStr);

	static std::string lookupTypeStr(Field::Type ftype);

protected:
	InternalField(const char* name, Field::Type type, bool isHeader);

	mutable std::string m_xml;

	mutable std::string m_json;

private:
	static Field* createField(const char* name, const char* type, const char* value, const char* bits, const char* head); 

	std::string         m_name;
	Field::Type         m_type;
	bool                m_header;

	static bool         s_identifyHeaderField;

	mutable std::string m_string;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
