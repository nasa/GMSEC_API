/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file DeviceParam.cpp
 *
 *  @brief This file contains the implementation for a device parameter class.
**/

#include <gmsec4/mist/DeviceParam.h>
#include <gmsec4/internal/mist/InternalDeviceParam.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


DeviceParam::DeviceParam(const char*  name,
                         const char*  timestamp,
                         const Field& value)
           : m_iDeviceParam(new InternalDeviceParam(name, timestamp, value))
{
}// DeviceParam constructor


DeviceParam::DeviceParam(const DeviceParam& deviceParam)
           : m_iDeviceParam(new InternalDeviceParam(*(deviceParam.m_iDeviceParam)))
{
}// DeviceParam copy constructor


DeviceParam::~DeviceParam()
{
	delete m_iDeviceParam;

}// DeviceParam copy constructor


const char* DeviceParam::getName() const
{
	return m_iDeviceParam->getName();

}// getName()


const char* DeviceParam::getTimestamp() const
{
	return m_iDeviceParam->getTimestamp();

}// getTimestamp()


const Field& DeviceParam::getValue() const
{
	return m_iDeviceParam->getValue();

}// getValue()


bool DeviceParam::operator==(const DeviceParam& deviceParam) const
{
	return m_iDeviceParam->operator==(*(deviceParam.m_iDeviceParam));

}// DeviceParam::operator==()


bool DeviceParam::operator!=(const DeviceParam& deviceParam) const
{
	return m_iDeviceParam->operator!=(*(deviceParam.m_iDeviceParam));

}// DeviceParam::operator!=()
