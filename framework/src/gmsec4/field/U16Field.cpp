/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/field/U16Field.h>
#include <gmsec4/internal/field/InternalU16Field.h>


using namespace gmsec::api::internal;


namespace gmsec
{
namespace api
{

U16Field::U16Field(const char* name, GMSEC_U16 value)
	: m_internal(new InternalU16Field(name, value))
{
	registerChild(m_internal);
}


U16Field::U16Field(const U16Field& other)
	: m_internal(new InternalU16Field(other.getName(), other.getValue()))
{
	registerChild(m_internal);
}


U16Field::~U16Field()
{
	delete m_internal;
}


GMSEC_U16 U16Field::getValue() const
{
	return m_internal->getValue();
}


const char* U16Field::toXML() const
{
	return m_internal->toXML();
}


const char* U16Field::toJSON() const
{
	return m_internal->toJSON();
}

} //namespace api
} //namespace gmsec
