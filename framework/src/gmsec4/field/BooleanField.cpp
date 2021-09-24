/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/field/BooleanField.h>
#include <gmsec4/internal/field/InternalBooleanField.h>


using namespace gmsec::api::internal;


namespace gmsec
{
namespace api
{

BooleanField::BooleanField(const char* name, bool value)
	: m_internal(new InternalBooleanField(name, value))
{
	registerChild(m_internal);
}


BooleanField::BooleanField(const BooleanField& other)
	: m_internal(new InternalBooleanField(other.getName(), other.getValue()))
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

} //namespace api
} //namespace gmsec
