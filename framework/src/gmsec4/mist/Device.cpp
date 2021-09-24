/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file Device.cpp
 *
 *  @brief This file contains the pass-through implementation for MIST Device
 *
**/

#include <gmsec4/mist/Device.h>
#include <gmsec4/internal/mist/InternalDevice.h>
#include <gmsec4/Exception.h>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


Device::Device(const char* name, Device::DeviceStatus status)
	: m_iDevice(new internal::InternalDevice(name, status))
{
}


Device::Device(const char* name, DeviceStatus status, const gmsec::api::util::DataList<DeviceParam*>& params)
	: m_iDevice(new internal::InternalDevice(name, status, params))
{
}


Device::Device(const Device& other)
	: m_iDevice(new InternalDevice(*(other.m_iDevice)))
{
}


Device::~Device()
{
	delete m_iDevice;
	m_iDevice = 0;
}

Device& Device::operator=(const Device& other)
{
	if (this != &other)
    {
        delete m_iDevice;
 
        m_iDevice = new InternalDevice(*other.m_iDevice);
    }
 
    return *this;
}


const char* Device::getName() const
{
	return m_iDevice->getName();
}


bool Device::numberAvailable() const
{
	return m_iDevice->numberAvailable();
}


const Field& Device::getNumber() const
{
	return m_iDevice->getNumber();
}


void Device::setNumber(const Field& number)
{
	m_iDevice->setNumber(number);
}


bool Device::modelAvailable() const
{
	return m_iDevice->modelAvailable();
}


const char* Device::getModel() const
{
	return m_iDevice->getModel();
}


void Device::setModel(const char* model)
{
	m_iDevice->setModel(model);
}


bool Device::serialAvailable() const
{
	return m_iDevice->serialAvailable();
}


const char* Device::getSerial() const
{
	return m_iDevice->getSerial();
}


void Device::setSerial(const char* serial)
{
	m_iDevice->setSerial(serial);
}


bool Device::versionAvailable() const
{
	return m_iDevice->versionAvailable();
}


const char* Device::getVersion() const
{
	return m_iDevice->getVersion();
}


void Device::setVersion(const char* version)
{
	m_iDevice->setVersion(version);
}


bool Device::groupAvailable() const
{
	return m_iDevice->groupAvailable();
}


const char* Device::getGroup() const
{
	return m_iDevice->getGroup();
}


void Device::setGroup(const char* group)
{
	m_iDevice->setGroup(group);
}


bool Device::roleAvailable() const
{
	return m_iDevice->roleAvailable();
}


const char* Device::getRole() const
{
	return m_iDevice->getRole();
}


void Device::setRole(const char* role)
{
	m_iDevice->setRole(role);
}


Device::DeviceStatus Device::getStatus() const
{
	return m_iDevice->getStatus();
}


void Device::setStatus(Device::DeviceStatus status)
{
	m_iDevice->setStatus(status);
}


bool Device::infoAvailable() const
{
	return m_iDevice->infoAvailable();
}


const Field& Device::getInfo() const
{
	return m_iDevice->getInfo();
}


void Device::setInfo(const Field& info)
{
	m_iDevice->setInfo(info);
}


size_t Device::getParamCount() const
{
	return m_iDevice->getParamCount();
}


const DeviceParam& Device::getParam(size_t idx) const
{
	return m_iDevice->getParam(idx);
}


void Device::addParam(const DeviceParam& param)
{
	m_iDevice->addParam(param);
}
