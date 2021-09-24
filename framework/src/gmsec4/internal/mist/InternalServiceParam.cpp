/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalServiceParam.cpp
 *
 *  @brief This file contains definitions for Simple Service containers
**/


#include <gmsec4/internal/mist/InternalServiceParam.h>
#include <gmsec4/internal/field/InternalField.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Errors.h>


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


InternalServiceParam::InternalServiceParam(const char* dName,
                                           const Field& dValue)
	            : m_value(InternalField::makeFieldCopy(dValue))
{
	if (dName && !std::string(dName).empty())
	{
		m_name = dName;
	}
	else
	{
		throw Exception(MIST_ERROR,
		                OTHER_ERROR_CODE,
		                "InternalServiceParam constructor:  NULL or empty name provided.");
	}
}


InternalServiceParam::InternalServiceParam(const InternalServiceParam& other)
                    : m_name(other.m_name),
                      m_value(InternalField::makeFieldCopy(*(other.m_value)))
{
}


InternalServiceParam::~InternalServiceParam()
{
	delete m_value;
	m_value = 0;
}


const char* InternalServiceParam::getName() const
{
	return m_name.c_str();
}


const Field& InternalServiceParam::getValue() const
{
	return *m_value;
}


bool InternalServiceParam::operator==(const InternalServiceParam& serviceParam) const
{
	bool ret_val = false;

	if (m_name == serviceParam.m_name &&
	    InternalField::testEquals(*m_value, *(serviceParam.m_value)))
	{
		ret_val = true;
	}

	return ret_val;

}// InternalServiceParam::operator==()


bool InternalServiceParam::operator!=(const InternalServiceParam& serviceParam) const
{
	return !(operator==(serviceParam));

}// InternalServiceParam::operator!=()
