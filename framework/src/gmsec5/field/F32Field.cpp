/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/field/F32Field.h>
#include <gmsec5/internal/field/InternalF32Field.h>



using namespace gmsec::api5;
using namespace gmsec::api5::internal;


F32Field::F32Field(const char* name, GMSEC_F32 value, bool isHeader)
	: m_internal(new InternalF32Field(name, value, isHeader))
{
	registerChild(m_internal);
}


F32Field::F32Field(const F32Field& other)
	: m_internal(new InternalF32Field(other.getName(), other.getValue(), other.isHeader()))
{
	registerChild(m_internal);
}


F32Field::~F32Field()
{
	delete m_internal;
}


GMSEC_F32 F32Field::getValue() const
{
	return m_internal->getValue();
}


const char* F32Field::toXML() const
{
	return m_internal->toXML();
}


const char* F32Field::toJSON() const
{
	return m_internal->toJSON();
}
