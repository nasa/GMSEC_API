/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/field/I16Field.h>
#include <gmsec4/internal/field/InternalI16Field.h>


using namespace gmsec::api::internal;


namespace gmsec
{
namespace api
{

I16Field::I16Field(const char* name, GMSEC_I16 value)
	: m_internal(new InternalI16Field(name, value))
{
	registerChild(m_internal);
}


I16Field::I16Field(const I16Field& other)
	: m_internal(new InternalI16Field(other.getName(), other.getValue()))
{
	registerChild(m_internal);
}


I16Field::~I16Field()
{
	delete m_internal;
}


GMSEC_I16 I16Field::getValue() const
{
	return m_internal->getValue();
}


const char* I16Field::toXML() const
{
	return m_internal->toXML();
}


const char* I16Field::toJSON() const
{
	return m_internal->toJSON();
}

} //namespace api
} //namespace gmsec
