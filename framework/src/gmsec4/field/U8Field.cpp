/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/field/U8Field.h>
#include <gmsec4/internal/field/InternalU8Field.h>


using namespace gmsec::api::internal;


namespace gmsec
{
namespace api
{

U8Field::U8Field(const char* name, GMSEC_U8 value)
	: m_internal(new InternalU8Field(name, value))
{
	registerChild(m_internal);
}


U8Field::U8Field(const U8Field& other)
	: m_internal(new InternalU8Field(other.getName(), other.getValue()))
{
	registerChild(m_internal);
}


U8Field::~U8Field()
{
	delete m_internal;
}


GMSEC_U8 U8Field::getValue() const
{
	return m_internal->getValue();
}


const char* U8Field::toXML() const
{
	return m_internal->toXML();
}


const char* U8Field::toJSON() const
{
	return m_internal->toJSON();
}

} //namespace api
} //namespace gmsec
