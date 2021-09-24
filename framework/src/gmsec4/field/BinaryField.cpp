/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/field/BinaryField.h>
#include <gmsec4/internal/field/InternalBinaryField.h>


using namespace gmsec::api::internal;


namespace gmsec
{
namespace api
{

BinaryField::BinaryField(const char* name, const GMSEC_BIN blob, size_t length)
	: m_internal(new InternalBinaryField(name, blob, length))
{
	registerChild(m_internal);
}


BinaryField::BinaryField(const BinaryField& other)
	: m_internal(new InternalBinaryField(other.getName(), other.getValue(), other.getLength()))
{
	registerChild(m_internal);
}


BinaryField::~BinaryField()
{
	delete m_internal;
}


GMSEC_BIN BinaryField::getValue() const
{
	return m_internal->getValue();
}


size_t BinaryField::getLength() const
{
	return m_internal->getLength();
}


const char* BinaryField::toXML() const
{
	return m_internal->toXML();
}


const char* BinaryField::toJSON() const
{
	return m_internal->toJSON();
}

} //namespace api
} //namespace gmsec
