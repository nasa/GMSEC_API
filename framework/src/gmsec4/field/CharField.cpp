/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/field/CharField.h>
#include <gmsec4/internal/field/InternalCharField.h>


using namespace gmsec::api::internal;


namespace gmsec
{
namespace api
{

CharField::CharField(const char* name, GMSEC_CHAR value)
	: m_internal(new InternalCharField(name, value))
{
	registerChild(m_internal);
}


CharField::CharField(const CharField& other)
	: m_internal(new InternalCharField(other.getName(), other.getValue()))
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

} //namespace api
} //namespace gmsec
