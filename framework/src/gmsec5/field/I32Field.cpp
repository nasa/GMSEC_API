/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/field/I32Field.h>
#include <gmsec5/internal/field/InternalI32Field.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


I32Field::I32Field(const char* name, GMSEC_I32 value, bool isHeader)
	: m_internal(new InternalI32Field(name, value, isHeader))
{
	registerChild(m_internal);
}


I32Field::I32Field(const I32Field& other)
	: m_internal(new InternalI32Field(other.getName(), other.getValue(), other.isHeader()))
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
