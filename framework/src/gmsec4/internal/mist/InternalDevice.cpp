/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalDevice.cpp
 *
 *  @brief This file contains definitions for Device containers
**/

#include <gmsec4/internal/mist/InternalDevice.h>
#include <gmsec4/mist/Device.h>
#include <gmsec4/internal/field/InternalField.h>
#include <gmsec4/Exception.h>


using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


InternalDevice::InternalDevice(const char* name, Device::DeviceStatus status)
	: m_number(NULL),
	  m_hasNumber(false),
	  m_hasModel(false),
	  m_hasSerial(false),
	  m_hasVersion(false),
	  m_hasGroup(false),
	  m_hasRole(false),
	  m_status(status),
	  m_info(NULL),
	  m_hasInfo(false),
	  m_paramList()
{
	if (!name || std::string(name).empty())
	{
		throw Exception(MIST_ERROR, OTHER_ERROR_CODE, "InternalDevice constructor:  NULL or empty name provided.");
	}

	m_name = name;
}


InternalDevice::InternalDevice(const char* name, Device::DeviceStatus status, const gmsec::api::util::DataList<DeviceParam*>& params)
	: m_number(NULL),
	  m_hasNumber(false),
	  m_hasModel(false),
	  m_hasSerial(false),
	  m_hasVersion(false),
	  m_hasGroup(false),
	  m_hasRole(false),
	  m_status(status),
	  m_info(NULL),
	  m_hasInfo(false),
	  m_paramList()
{
	if (!name || std::string(name).empty())
	{
		throw Exception(MIST_ERROR, OTHER_ERROR_CODE, "InternalDevice constructor:  NULL or empty name provided.");
	}

	m_name = name;

	for (DataList<DeviceParam*>::const_iterator it = params.begin(); it != params.end(); ++it)
	{
		const DeviceParam* param = *it;
		m_paramList.push_back(new DeviceParam(*param));
	}
}


InternalDevice::InternalDevice(const InternalDevice& other)
	: m_name(other.m_name),
	  m_number(NULL),
	  m_hasNumber(other.m_hasNumber),
	  m_model(other.m_model),
	  m_hasModel(other.m_hasModel),
	  m_serial(other.m_serial),
	  m_hasSerial(other.m_hasSerial),
	  m_version(other.m_version),
	  m_hasVersion(other.m_hasVersion),
	  m_group(other.m_group),
	  m_hasGroup(other.m_hasGroup),
	  m_role(other.m_role),
	  m_hasRole(other.m_hasRole),
	  m_status(other.m_status),
	  m_info(NULL),
	  m_hasInfo(other.m_hasInfo),
	  m_paramList()
{
	for (DataList<DeviceParam*>::const_iterator it = other.m_paramList.begin(); it != other.m_paramList.end(); ++it)
	{
		const DeviceParam* param = *it;
		m_paramList.push_back(new DeviceParam(*param));
	}

	if(m_hasNumber){
		setNumber(*other.m_number);
	}else{
		m_number = NULL;
	}
	if(m_hasInfo){
		setInfo(*other.m_info);
	}else{
		m_info = NULL;
	}
}


InternalDevice::~InternalDevice()
{
	
	if (m_number)
	{
		delete m_number;
	}

	if (m_info)
	{
		delete m_info;
	}

	for (DataList<DeviceParam*>::iterator it = m_paramList.begin(); it != m_paramList.end(); ++it)
	{
		delete *it;
	}

	m_paramList.clear();
}

InternalDevice& InternalDevice::operator=(const InternalDevice& other)
{
    if (this != &other)
    {
        m_name = other.m_name;
 
        if (other.m_number)
        {
            m_number    = gmsec::api::internal::InternalField::makeFieldCopy(*other.m_number);
        }
		m_hasNumber = other.m_hasNumber;

		m_model = other.m_model;
		m_hasModel = other.m_hasModel;

		m_serial = other.m_serial;
		m_hasSerial = other.m_hasSerial;

		m_version = other.m_version;
		m_hasVersion = other.m_hasVersion;

		m_group = other.m_group;
		m_hasGroup = other.m_hasGroup;

		m_role = other.m_role;
		m_hasRole = other.m_hasRole;

		m_status = other.m_status;

		if (other.m_info)
        {
            m_info    = gmsec::api::internal::InternalField::makeFieldCopy(*other.m_info);
        }
		m_hasInfo = other.m_hasInfo;
	
		for (DataList<DeviceParam*>::const_iterator it = other.m_paramList.begin(); it != other.m_paramList.end(); ++it)
		{
			const DeviceParam* param = *it;
			m_paramList.push_back(new DeviceParam(*param));
		}
    }
 
    return *this;
}

const char* InternalDevice::getName() const
{
	return m_name.c_str();
}


bool InternalDevice::numberAvailable() const
{
	return m_hasNumber;
}


const Field& InternalDevice::getNumber() const
{
	if (m_hasNumber)
	{
		return *m_number;
	}
	throw Exception(MIST_ERROR,
	                INVALID_FIELD,
	                "InternalDevice::getNumber():  number field does not exist in this Device right now.");
}


void InternalDevice::setNumber(const Field& number)
{
	if(m_number)
	{
		delete m_number;
	}
	m_number    = gmsec::api::internal::InternalField::makeFieldCopy(number);
	m_hasNumber = true;
}


bool InternalDevice::modelAvailable() const
{
	return m_hasModel;
}


const char* InternalDevice::getModel() const
{
	if (m_hasModel)
	{
		return m_model.c_str();
	}
	throw Exception(MIST_ERROR,
	                INVALID_FIELD,
	                "InternalDevice::getModel():  model field does not exist in this Device right now.");
}


void InternalDevice::setModel(const char* model)
{
	m_model = model;
	m_hasModel = true;
}


bool InternalDevice::serialAvailable() const
{
	return m_hasSerial;
}


const char* InternalDevice::getSerial() const
{
	if (m_hasSerial)
	{
		return m_serial.c_str();
	}

	throw Exception(MIST_ERROR,
	                INVALID_FIELD,
	                "InternalDevice::getSerial():  serial field does not exist in this Device right now.");
}


void InternalDevice::setSerial(const char* serial)
{
	m_serial = serial;
	m_hasSerial = true;
}


bool InternalDevice::versionAvailable() const
{
	return m_hasVersion;
}


const char* InternalDevice::getVersion() const
{
	if (m_hasVersion)
	{
		return m_version.c_str();
	}
	else
	{
		throw Exception(MIST_ERROR,
		                INVALID_FIELD,
		                "InternalDevice::getSerial():  version field does not exist in this Device right now.");
	}
}


void InternalDevice::setVersion(const char* version)
{
	m_version = version;
	m_hasVersion = true;
}


bool InternalDevice::groupAvailable() const
{
	return m_hasGroup;
}


const char* InternalDevice::getGroup() const
{
	if (m_hasGroup)
	{
		return m_group.c_str();
	}
	throw Exception(MIST_ERROR,
	                INVALID_FIELD,
	                "InternalDevice::getGroup():  group field does not exist in this Device right now.");
}


void InternalDevice::setGroup(const char* group)
{
	m_group = group;
	m_hasGroup = true;
}


bool InternalDevice::roleAvailable() const
{
	return m_hasRole;
}


const char* InternalDevice::getRole() const
{
	if (m_hasRole)
	{
		return m_role.c_str();
	}

	throw Exception(MIST_ERROR,
	                INVALID_FIELD,
	                "InternalDevice::getGroup():  role field does not exist in this Device right now.");
}


void InternalDevice::setRole(const char* role)
{
	m_role = role;
	m_hasRole = true;
}


Device::DeviceStatus InternalDevice::getStatus() const
{
	return m_status;
}


void InternalDevice::setStatus(Device::DeviceStatus status)
{
	m_status = status;
}


bool InternalDevice::infoAvailable() const
{
	return m_hasInfo;
}


const Field& InternalDevice::getInfo() const
{
	if (m_hasInfo)
	{
		return *m_info;
	}

	throw Exception(MIST_ERROR,
	                INVALID_FIELD,
	                "InternalDevice::getInfo():  info field does not exist in this Device right now.");
}


void InternalDevice::setInfo(const Field& info)
{
	if(m_info)
	{
		delete m_info;
	}
	m_info    = gmsec::api::internal::InternalField::makeFieldCopy(info);
	m_hasInfo = true;
}


size_t InternalDevice::getParamCount() const
{
	return m_paramList.size();

}


const DeviceParam& InternalDevice::getParam(size_t idx) const
{
	if (idx < m_paramList.size())
	{
		return *(m_paramList.get(idx));
	}

	throw Exception(MIST_ERROR, INDEX_OUT_OF_RANGE,
		"InternalDevice::getParam():  Device parameter index was specified out-of-range.");
}


void InternalDevice::addParam(const DeviceParam& param)
{
	m_paramList.add(new DeviceParam(param));
}
