/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/field/I64Field.h>
#include <gmsec4/internal/field/InternalI64Field.h>


using namespace gmsec::api::internal;


namespace gmsec
{
namespace api
{

I64Field::I64Field(const char* name, GMSEC_I64 value)
	: m_internal(new InternalI64Field(name, value))
{
	registerChild(m_internal);
}


I64Field::I64Field(const I64Field& other)
	: m_internal(new InternalI64Field(other.getName(), other.getValue()))
{
	registerChild(m_internal);
}


I64Field::~I64Field()
{
	delete m_internal;
}


GMSEC_I64 I64Field::getValue() const
{
	return m_internal->getValue();
}


const char* I64Field::toXML() const
{
	return m_internal->toXML();
}


const char* I64Field::toJSON() const
{
	return m_internal->toJSON();
}

} //namespace api
} //namespace gmsec
