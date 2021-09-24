/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/field/F64Field.h>
#include <gmsec4/internal/field/InternalF64Field.h>


using namespace gmsec::api::internal;


namespace gmsec
{
namespace api
{

F64Field::F64Field(const char* name, GMSEC_F64 value)
	: m_internal(new InternalF64Field(name, value))
{
	registerChild(m_internal);
}


F64Field::F64Field(const F64Field& other)
	: m_internal(new InternalF64Field(other.getName(), other.getValue()))
{
	registerChild(m_internal);
}


F64Field::~F64Field()
{
	delete m_internal;
}


GMSEC_F64 F64Field::getValue() const
{
	return m_internal->getValue();
}


const char* F64Field::toXML() const
{
	return m_internal->toXML();
}


const char* F64Field::toJSON() const
{
	return m_internal->toJSON();
}

} //namespace api
} //namespace gmsec
