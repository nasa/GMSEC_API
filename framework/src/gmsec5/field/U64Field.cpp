/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/field/U64Field.h>
#include <gmsec5/internal/field/InternalU64Field.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


U64Field::U64Field(const char* name, GMSEC_U64 value, bool isHeader)
	: m_internal(new InternalU64Field(name, value, isHeader))
{
	registerChild(m_internal);
}


U64Field::U64Field(const U64Field& other)
	: m_internal(new InternalU64Field(other.getName(), other.getValue(), other.isHeader()))
{
	registerChild(m_internal);
}


U64Field::~U64Field()
{
	delete m_internal;
}


GMSEC_U64 U64Field::getValue() const
{
	return m_internal->getValue();
}


const char* U64Field::toXML() const
{
	return m_internal->toXML();
}


const char* U64Field::toJSON() const
{
	return m_internal->toJSON();
}
