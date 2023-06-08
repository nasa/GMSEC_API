/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/field/Field.h>
#include <gmsec5/internal/field/InternalField.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


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


Field::Type Field::getType() const
{
	return m_internal->getType();
}


bool Field::isHeader() const
{
	return m_internal->isHeader();
}


const char* Field::getStringValue() const
{
	return m_internal->getStringValue();
}


bool Field::getBooleanValue() const
{
	return m_internal->getBooleanValue();
}


GMSEC_I16 Field::getI16Value() const
{
	return m_internal->getI16Value();
}


GMSEC_I32 Field::getI32Value() const
{
	return m_internal->getI32Value();
}


GMSEC_I64 Field::getI64Value() const
{
	return m_internal->getI64Value();
}


GMSEC_U16 Field::getU16Value() const
{
	return m_internal->getU16Value();
}


GMSEC_U32 Field::getU32Value() const
{
	return m_internal->getU32Value();
}


GMSEC_U64 Field::getU64Value() const
{
	return m_internal->getU64Value();
}


GMSEC_F64 Field::getF64Value() const
{
	return m_internal->getF64Value();
}
