/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/field/I32Field.h>
#include <gmsec4/internal/field/InternalI32Field.h>


using namespace gmsec::api::internal;


namespace gmsec
{
namespace api
{

I32Field::I32Field(const char* name, GMSEC_I32 value)
	: m_internal(new InternalI32Field(name, value))
{
	registerChild(m_internal);
}


I32Field::I32Field(const I32Field& other)
	: m_internal(new InternalI32Field(other.getName(), other.getValue()))
{
	registerChild(m_internal);
}


I32Field::~I32Field()
{
	delete m_internal;
}


GMSEC_I32 I32Field::getValue() const
{
	return m_internal->getValue();
}


const char* I32Field::toXML() const
{
	return m_internal->toXML();
}


const char* I32Field::toJSON() const
{
	return m_internal->toJSON();
}

} //namespace api
} //namespace gmsec
