/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/field/U32Field.h>
#include <gmsec4/internal/field/InternalU32Field.h>


using namespace gmsec::api::internal;


namespace gmsec
{
namespace api
{

U32Field::U32Field(const char* name, GMSEC_U32 value)
	: m_internal(new InternalU32Field(name, value))
{
	registerChild(m_internal);
}


U32Field::U32Field(const U32Field& other)
	: m_internal(new InternalU32Field(other.getName(), other.getValue()))
{
	registerChild(m_internal);
}


U32Field::~U32Field()
{
	delete m_internal;
}


GMSEC_U32 U32Field::getValue() const
{
	return m_internal->getValue();
}


const char* U32Field::toXML() const
{
	return m_internal->toXML();
}


const char* U32Field::toJSON() const
{
	return m_internal->toJSON();
}

} //namespace api
} //namespace gmsec
