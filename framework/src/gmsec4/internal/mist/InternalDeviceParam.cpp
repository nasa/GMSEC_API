/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalDeviceParam.cpp
 *
 *  @brief This file contains the implementation for a device parameter class.
**/

#include <gmsec4/internal/mist/InternalDeviceParam.h>
#include <gmsec4/mist/DeviceParam.h>
#include <gmsec4/internal/field/InternalField.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Errors.h>
#include <string>

using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


InternalDeviceParam::InternalDeviceParam(const char*  name,
                                         const char*  timestamp,
                                         const Field& value)
                   : m_value(InternalField::makeFieldCopy(value))
{
	if (name && !std::string(name).empty())
	{
		m_name = name;
	}
	else
	{
		throw Exception(MIST_ERROR,
		                OTHER_ERROR_CODE,
		                "InternalDeviceParam constructor:  NULL or empty name "
		                "provided.");
	}

	if (timestamp && !std::string(timestamp).empty())
	{
		m_timestamp = timestamp;
	}
	else
	{
		throw Exception(MIST_ERROR,
		                OTHER_ERROR_CODE,
		                "InternalDeviceParam constructor:  NULL or empty "
		                "timestamp provided.");
	}

}// InternalDeviceParam constructor


InternalDeviceParam::InternalDeviceParam(const InternalDeviceParam& deviceParam)
           : m_name(deviceParam.m_name),
             m_timestamp(deviceParam.m_timestamp),
             m_value(InternalField::makeFieldCopy(*(deviceParam.m_value)))
{
}// InternalDeviceParam constructor


InternalDeviceParam::~InternalDeviceParam()
{
	delete m_value;

}// InternalDeviceParam destructor


const char* InternalDeviceParam::getName() const
{
	return m_name.c_str();

}// getName()


const char* InternalDeviceParam::getTimestamp() const
{
	return m_timestamp.c_str();

}// getTimestamp()


const Field& InternalDeviceParam::getValue() const
{
	return *m_value;

}// getValue()


bool InternalDeviceParam::operator==(const InternalDeviceParam& deviceParam) const
{
	bool ret_val = false;

	if (m_name              == deviceParam.m_name &&
	    m_timestamp         == deviceParam.m_timestamp &&
	    InternalField::testEquals(*m_value, *(deviceParam.m_value)))
	{
		ret_val = true;
	}

	return ret_val;

}// DeviceParam::operator==()


bool InternalDeviceParam::operator!=(const InternalDeviceParam& deviceParam) const
{
	bool ret_val = false;

	if (m_name              != deviceParam.m_name ||
	    m_timestamp         != deviceParam.m_timestamp ||
	    !InternalField::testEquals(*m_value, *(deviceParam.m_value)))
	{
		ret_val = true;
	}

	return ret_val;

}// DeviceParam::operator!=()
