/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/field/StringField.h>
#include <gmsec5/internal/field/InternalStringField.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


StringField::StringField(const char* name, const char* value, bool isHeader)
	: m_internal(new InternalStringField(name, value, isHeader))
{
	registerChild(m_internal);
}


StringField::StringField(const StringField& other)
	: m_internal(new InternalStringField(other.getName(), other.getValue(), other.isHeader()))
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
