/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/field/I16Field.h>
#include <gmsec5/internal/field/InternalI16Field.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


I16Field::I16Field(const char* name, GMSEC_I16 value, bool isHeader)
	: m_internal(new InternalI16Field(name, value, isHeader))
{
	registerChild(m_internal);
}


I16Field::I16Field(const I16Field& other)
	: m_internal(new InternalI16Field(other.getName(), other.getValue(), other.isHeader()))
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
