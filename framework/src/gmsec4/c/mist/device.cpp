/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file device.cpp
 *
 *  @brief This file contains functions for the management of a MIST Device object.
 */


#include <gmsec4/c/mist/device.h>

#include <gmsec4/mist/Device.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>

using namespace gmsec::api;
using namespace gmsec::api::mist;


GMSEC_Device CALL_TYPE deviceCreate(const char* subject, GMSEC_DeviceStatus deviceStatus, GMSEC_Status status)
{
	GMSEC_Device device = NULL;
	Status       result;

	try
	{
		int tmp = deviceStatus;
		Device::DeviceStatus devStatus = static_cast<Device::DeviceStatus>(tmp);

		device = reinterpret_cast<GMSEC_Device>(new Device(subject, devStatus));
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return device;
}


GMSEC_Device CALL_TYPE deviceCreateCopy(const GMSEC_Device other, GMSEC_Status status)
{
	GMSEC_Device device = NULL;
	Status       result;

	const Device* otherDevice = reinterpret_cast<const Device*>(other);

	if (otherDevice)
	{
		device = reinterpret_cast<GMSEC_Device>(new Device(*otherDevice));
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return device;
}


void CALL_TYPE deviceDestroy(GMSEC_Device* device)
{
	if (device && *device)
	{
		Device* tmp = reinterpret_cast<Device*>(*device);

		delete tmp;

		*device = NULL;
	}
}


const char* CALL_TYPE deviceGetName(const GMSEC_Device device, GMSEC_Status status)
{
	const char* name = NULL;
	Status      result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		name = dev->getName();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return name;
}


GMSEC_DeviceStatus CALL_TYPE deviceGetStatus(const GMSEC_Device device, GMSEC_Status status)
{
	GMSEC_DeviceStatus devStatus = GMSEC_RED;
	Status             result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		int tmp = dev->getStatus();
		devStatus = static_cast<GMSEC_DeviceStatus>(tmp);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return devStatus;
}


void CALL_TYPE deviceSetStatus(GMSEC_Device device, GMSEC_DeviceStatus deviceStatus, GMSEC_Status status)
{
	Status result;

	Device* dev = reinterpret_cast<Device*>(device);

	if (dev)
	{
		int tmp = deviceStatus;

		dev->setStatus(static_cast<Device::DeviceStatus>(tmp));
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE deviceNumberAvailable(const GMSEC_Device device, GMSEC_Status status)
{
	GMSEC_BOOL available = GMSEC_FALSE;
	Status     result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		available = (dev->numberAvailable() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return available;
}


const GMSEC_Field CALL_TYPE deviceGetNumber(const GMSEC_Device device, GMSEC_Status status)
{
	GMSEC_Field number = NULL;
	Status      result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		try
		{
			Field& tmp = const_cast<Field&>(dev->getNumber());

			number = reinterpret_cast<GMSEC_Field>(&tmp);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return number;
}


void CALL_TYPE deviceSetNumber(GMSEC_Device device, const GMSEC_Field number, GMSEC_Status status)
{
	Status result;

	Device*      dev   = reinterpret_cast<Device*>(device);
	const Field* field = reinterpret_cast<const Field*>(number);

	if (!dev)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}
	else if (!field)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Field handle is NULL");
	}
	else
	{
		dev->setNumber(*field);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE deviceModelAvailable(const GMSEC_Device device, GMSEC_Status status)
{
	GMSEC_BOOL available = GMSEC_FALSE;
	Status     result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		available = (dev->modelAvailable() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return available;
}


const char* CALL_TYPE deviceGetModel(const GMSEC_Device device, GMSEC_Status status)
{
	const char* model = NULL;
	Status      result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		try
		{
			model = dev->getModel();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return model;
}


void CALL_TYPE deviceSetModel(GMSEC_Device device, const char* model, GMSEC_Status status)
{
	Status result;

	Device* dev = reinterpret_cast<Device*>(device);

	if (dev)
	{
		try
		{
			dev->setModel(model);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE deviceSerialAvailable(const GMSEC_Device device, GMSEC_Status status)
{
	GMSEC_BOOL available = GMSEC_FALSE;
	Status     result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		available = (dev->serialAvailable() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return available;
}


const char* CALL_TYPE deviceGetSerial(const GMSEC_Device device, GMSEC_Status status)
{
	const char* serial = NULL;
	Status      result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		try
		{
			serial = dev->getSerial();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return serial;
}


void CALL_TYPE deviceSetSerial(GMSEC_Device device, const char* serial, GMSEC_Status status)
{
	Status result;

	Device* dev = reinterpret_cast<Device*>(device);

	if (dev)
	{
		try
		{
			dev->setSerial(serial);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE deviceVersionAvailable(const GMSEC_Device device, GMSEC_Status status)
{
	GMSEC_BOOL available = GMSEC_FALSE;
	Status     result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		available = (dev->versionAvailable() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return available;
}


const char* CALL_TYPE deviceGetVersion(const GMSEC_Device device, GMSEC_Status status)
{
	const char* version = NULL;
	Status      result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		try
		{
			version = dev->getVersion();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return version;
}


void CALL_TYPE deviceSetVersion(GMSEC_Device device, const char* version, GMSEC_Status status)
{
	Status result;

	Device* dev = reinterpret_cast<Device*>(device);

	if (dev)
	{
		try
		{
			dev->setVersion(version);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE deviceGroupAvailable(const GMSEC_Device device, GMSEC_Status status)
{
	GMSEC_BOOL available = GMSEC_FALSE;
	Status     result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		available = (dev->groupAvailable() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return available;
}


const char* CALL_TYPE deviceGetGroup(const GMSEC_Device device, GMSEC_Status status)
{
	const char* group = NULL;
	Status      result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		try
		{
			group = dev->getGroup();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return group;
}


void CALL_TYPE deviceSetGroup(GMSEC_Device device, const char* group, GMSEC_Status status)
{
	Status result;

	Device* dev = reinterpret_cast<Device*>(device);

	if (dev)
	{
		try
		{
			dev->setGroup(group);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE deviceRoleAvailable(const GMSEC_Device device, GMSEC_Status status)
{
	GMSEC_BOOL available = GMSEC_FALSE;
	Status     result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		available = (dev->roleAvailable() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return available;
}


const char* CALL_TYPE deviceGetRole(const GMSEC_Device device, GMSEC_Status status)
{
	const char* role = NULL;
	Status      result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		try
		{
			role = dev->getRole();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return role;
}


void CALL_TYPE deviceSetRole(GMSEC_Device device, const char* role, GMSEC_Status status)
{
	Status result;

	Device* dev = reinterpret_cast<Device*>(device);

	if (dev)
	{
		try
		{
			dev->setRole(role);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE deviceInfoAvailable(const GMSEC_Device device, GMSEC_Status status)
{
	GMSEC_BOOL available = GMSEC_FALSE;
	Status     result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		available = (dev->infoAvailable() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return available;
}


const GMSEC_Field CALL_TYPE deviceGetInfo(const GMSEC_Device device, GMSEC_Status status)
{
	GMSEC_Field info = NULL;
	Status      result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		try
		{
			Field& tmp = const_cast<Field&>(dev->getNumber());

			info = reinterpret_cast<GMSEC_Field>(&tmp);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return info;
}


void CALL_TYPE deviceSetInfo(GMSEC_Device device, const GMSEC_Field info, GMSEC_Status status)
{
	Status result;

	Device*      dev   = reinterpret_cast<Device*>(device);
	const Field* field = reinterpret_cast<const Field*>(info);

	if (!dev)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}
	else if (!field)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Field handle is NULL");
	}
	else
	{
		dev->setInfo(*field);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE deviceAddParam(const GMSEC_Device device, const GMSEC_DeviceParam param, GMSEC_Status status)
{
	Status result;

	Device*            dev      = reinterpret_cast<Device*>(device);
	const DeviceParam* devParam = reinterpret_cast<const DeviceParam*>(param);

	if (!dev)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}
	else if (!devParam)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "DeviceParam handle is NULL");
	}
	else
	{
		dev->addParam(*devParam);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


size_t CALL_TYPE deviceGetParamCount(const GMSEC_Device device, GMSEC_Status status)
{
	size_t count = 0;
	Status result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		count = dev->getParamCount();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return count;
}


const GMSEC_DeviceParam CALL_TYPE deviceGetParam(const GMSEC_Device device, size_t index, GMSEC_Status status)
{
	GMSEC_DeviceParam param = NULL;
	Status            result;

	const Device* dev = reinterpret_cast<const Device*>(device);

	if (dev)
	{
		try
		{
			DeviceParam& tmp = const_cast<DeviceParam&>(dev->getParam(index));

			param = reinterpret_cast<GMSEC_DeviceParam>(&tmp);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return param;
}
