/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec5/FieldSpecification.h>

#include <gmsec5/internal/InternalFieldSpecification.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


FieldSpecification::FieldSpecification(const char* name, const char* type, const char* mode, const char* clazz, const char* value, const char* desc)
	: m_internal(new InternalFieldSpecification(name, type, mode, clazz, value, desc))
{
}


FieldSpecification::~FieldSpecification()
{
	delete m_internal;
}


const char* FieldSpecification::getName() const
{
	return m_internal->getName();
}


const char* FieldSpecification::getType() const
{
	return m_internal->getType();
}


const char* FieldSpecification::getMode() const
{
	return m_internal->getMode();
}


const char* FieldSpecification::getClassification() const
{
	return m_internal->getClassification();
}


const char* FieldSpecification::getValue() const
{
	return m_internal->getValue();
}


const char* FieldSpecification::getDescription() const
{
	return m_internal->getDescription();
}
