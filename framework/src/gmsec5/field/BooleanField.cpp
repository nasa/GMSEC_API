/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/field/BooleanField.h>
#include <gmsec5/internal/field/InternalBooleanField.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


BooleanField::BooleanField(const char* name, bool value, bool isHeader)
	: m_internal(new InternalBooleanField(name, value, isHeader))
{
	registerChild(m_internal);
}


BooleanField::BooleanField(const BooleanField& other)
	: m_internal(new InternalBooleanField(other.getName(), other.getValue(), other.isHeader()))
{
	registerChild(m_internal);
}



BooleanField::~BooleanField()
{
	delete m_internal;
}


bool BooleanField::getValue() const
{
	return m_internal->getValue();
}


const char* BooleanField::toXML() const
{
	return m_internal->toXML();
}


const char* BooleanField::toJSON() const
{
	return m_internal->toJSON();
}
