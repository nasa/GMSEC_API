/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/field/Field.h>
#include <gmsec4/internal/field/InternalField.h>


using namespace gmsec::api::internal;


namespace gmsec
{
namespace api
{

Field::Field()
	: m_internal(0)
{
}


Field::~Field()
{
	m_internal = 0;
}


void Field::registerChild(InternalField* child)
{
	m_internal = child;
}


const char* Field::getName() const
{
	return m_internal->getName();
}


void Field::setName(const char* name)
{
	m_internal->setName(name);
}


Field::FieldType Field::getType() const
{
	return m_internal->getType();
}


bool Field::isHeader() const
{
	return m_internal->isHeader();
}


void Field::isHeader(bool header)
{
	m_internal->isHeader(header);
}


GMSEC_I64 Field::getIntegerValue() const
{
	return m_internal->getIntegerValue();
}


GMSEC_U64 Field::getUnsignedIntegerValue() const
{
	return m_internal->getUnsignedIntegerValue();
}


GMSEC_F64 Field::getDoubleValue() const
{
	return m_internal->getDoubleValue();
}


const char* Field::getStringValue() const
{
	return m_internal->getStringValue();
}


Field* Field::clone() const
{
	return InternalField::makeFieldCopy(*this);
}


void Field::destroyClone(Field*& clone)
{
	delete clone;
	clone = NULL;
}


} //namespace api
} //namespace gmsec
