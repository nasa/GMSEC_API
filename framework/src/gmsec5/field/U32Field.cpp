/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/field/U32Field.h>
#include <gmsec5/internal/field/InternalU32Field.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


U32Field::U32Field(const char* name, GMSEC_U32 value, bool isHeader)
	: m_internal(new InternalU32Field(name, value, isHeader))
{
	registerChild(m_internal);
}


U32Field::U32Field(const U32Field& other)
	: m_internal(new InternalU32Field(other.getName(), other.getValue(), other.isHeader()))
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
