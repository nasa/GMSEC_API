/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/field/I8Field.h>
#include <gmsec5/internal/field/InternalI8Field.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


I8Field::I8Field(const char* name, GMSEC_I8 value, bool isHeader)
	: m_internal(new InternalI8Field(name, value, isHeader))
{
	registerChild(m_internal);
}


I8Field::I8Field(const I8Field& other)
	: m_internal(new InternalI8Field(other.getName(), other.getValue(), other.isHeader()))
{
	registerChild(m_internal);
}


I8Field::~I8Field()
{
	delete m_internal;
}


GMSEC_I8 I8Field::getValue() const
{
	return m_internal->getValue();
}


const char* I8Field::toXML() const
{
	return m_internal->toXML();
}


const char* I8Field::toJSON() const
{
	return m_internal->toJSON();
}
