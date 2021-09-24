/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file ServiceParam.cpp
 *
 *  @brief This file contains definitions for Simple Service containers
**/


#include <gmsec4/mist/ServiceParam.h>
#include <gmsec4/internal/mist/InternalServiceParam.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


ServiceParam::ServiceParam(const char* dName, const Field& dValue)
            : m_iServiceParam(new InternalServiceParam(dName, dValue))
{
}


ServiceParam::ServiceParam(const ServiceParam& other)
            : m_iServiceParam(new InternalServiceParam(*(other.m_iServiceParam)))
{
}


ServiceParam::~ServiceParam()
{
	delete m_iServiceParam;
}


const char* ServiceParam::getName() const
         
{
	return m_iServiceParam->getName();
}


const Field& ServiceParam::getValue() const
{
	return m_iServiceParam->getValue();
}


bool ServiceParam::operator==(const ServiceParam& serviceParam) const
{
	return m_iServiceParam->operator==(*(serviceParam.m_iServiceParam));

}// ServiceParam::operator==()


bool ServiceParam::operator!=(const ServiceParam& serviceParam) const
{
	return m_iServiceParam->operator!=(*(serviceParam.m_iServiceParam));

}// ServiceParam::operator!=()
