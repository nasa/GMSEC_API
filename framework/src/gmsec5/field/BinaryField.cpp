/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/field/BinaryField.h>
#include <gmsec5/internal/field/InternalBinaryField.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


BinaryField::BinaryField(const char* name, const GMSEC_U8* blob, size_t length, bool isHeader)
	: m_internal(new InternalBinaryField(name, blob, length, isHeader))
{
	registerChild(m_internal);
}


BinaryField::BinaryField(const BinaryField& other)
	: m_internal(new InternalBinaryField(other.getName(), other.getValue(), other.getLength(), other.isHeader()))
{
	registerChild(m_internal);
}


BinaryField::~BinaryField()
{
	delete m_internal;
}


GMSEC_U8 const * BinaryField::getValue() const
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
