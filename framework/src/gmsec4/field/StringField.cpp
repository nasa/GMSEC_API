/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/field/StringField.h>
#include <gmsec4/internal/field/InternalStringField.h>


using namespace gmsec::api::internal;


namespace gmsec
{
namespace api
{

StringField::StringField(const char* name, const char* value)
	: m_internal(new InternalStringField(name, value))
{
	registerChild(m_internal);
}


StringField::StringField(const StringField& other)
	: m_internal(new InternalStringField(other.getName(), other.getValue()))
{
	registerChild(m_internal);
}


StringField::~StringField()
{
	delete m_internal;
}


const char* StringField::getValue() const
{
	return m_internal->getValue();
}


const char* StringField::toXML() const
{
	return m_internal->toXML();
}


const char* StringField::toJSON() const
{
	return m_internal->toJSON();
}

} //namespace api
} //namespace gmsec
