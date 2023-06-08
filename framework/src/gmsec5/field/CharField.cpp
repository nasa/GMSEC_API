/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/field/CharField.h>
#include <gmsec5/internal/field/InternalCharField.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


CharField::CharField(const char* name, GMSEC_CHAR value, bool isHeader)
	: m_internal(new InternalCharField(name, value, isHeader))
{
	registerChild(m_internal);
}


CharField::CharField(const CharField& other)
	: m_internal(new InternalCharField(other.getName(), other.getValue(), other.isHeader()))
{
	registerChild(m_internal);
}



CharField::~CharField()
{
	delete m_internal;
}


GMSEC_CHAR CharField::getValue() const
{
	return m_internal->getValue();
}


const char* CharField::toXML() const
{
	return m_internal->toXML();
}


const char* CharField::toJSON() const
{
	return m_internal->toJSON();
}
