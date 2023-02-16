/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/field/U8Field.h>
#include <gmsec5/internal/field/InternalU8Field.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


U8Field::U8Field(const char* name, GMSEC_U8 value, bool isHeader)
	: m_internal(new InternalU8Field(name, value, isHeader))
{
	registerChild(m_internal);
}


U8Field::U8Field(const U8Field& other)
	: m_internal(new InternalU8Field(other.getName(), other.getValue(), other.isHeader()))
{
	registerChild(m_internal);
}


U8Field::~U8Field()
{
	delete m_internal;
}


GMSEC_U8 U8Field::getValue() const
{
	return m_internal->getValue();
}


const char* U8Field::toXML() const
{
	return m_internal->toXML();
}


const char* U8Field::toJSON() const
{
	return m_internal->toJSON();
}
