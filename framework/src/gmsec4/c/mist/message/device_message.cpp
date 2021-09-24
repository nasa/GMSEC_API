/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file device_message.cpp
 *
 *  @brief This file contains functions for the management of a MIST DeviceMessage object.
 */


#include <gmsec4/c/mist/message/device_message.h>

#include <gmsec4/mist/message/DeviceMessage.h>

#include <gmsec4/mist/Specification.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Config.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/Log.h>

#include <sstream>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;


static Specification buildSpecification(unsigned int version)
{
	std::ostringstream oss;
	oss << version;

	Config config;
	config.addValue("gmsec-specification-version", oss.str().c_str());

	return Specification(config);
}


GMSEC_Message CALL_TYPE deviceMessageCreateWithSpec(const char* subject,
	                                                GMSEC_Specification spec,
	                                                GMSEC_Status status)
{
	GMSEC_Message  msg = NULL;
	Specification* s   = reinterpret_cast<Specification*>(spec);
	Status         result;

	try
	{
		if (!s)
		{
			result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
		}
		else
		{
			msg = reinterpret_cast<GMSEC_Message>(new DeviceMessage(subject, *s));
		}
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message CALL_TYPE deviceMessageCreateWithConfigAndSpec(const char* subject,
	                                                         GMSEC_Config config,
	                                                         GMSEC_Specification spec,
	                                                         GMSEC_Status status)
{
	GMSEC_Message  msg = NULL;
	Config*        cfg = reinterpret_cast<Config*>(config);
	Specification* s   = reinterpret_cast<Specification*>(spec);
	Status         result;

	try
	{
		if (!cfg)
		{
			result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
		}
		else if (!s)
		{
			result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
		}
		else
		{
			msg = reinterpret_cast<GMSEC_Message>(new DeviceMessage(subject, *cfg, *s));
		}
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message CALL_TYPE deviceMessageCreateUsingData(const char* data, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	GMSEC_DISABLE_DEPRECATED_WARNINGS
	try
	{
		msg = reinterpret_cast<GMSEC_Message>(new DeviceMessage(data));
	}
	catch (Exception& e)
	{
		result = Status(e);
	}
	GMSEC_ENABLE_DEPRECATED_WARNINGS

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message CALL_TYPE deviceMessageCreateUsingSpecAndData(const GMSEC_Specification spec, const char* data, GMSEC_Status status)
{
	GMSEC_Message  msg = NULL;
	Specification* s   = reinterpret_cast<Specification*>(spec);
	Status         result;

	if (!s)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else
	{
		try
		{
			msg = reinterpret_cast<GMSEC_Message>(new DeviceMessage(*s, data));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message CALL_TYPE deviceMessageCreateCopy(const GMSEC_Message other, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	const Message*       tmpMsg = reinterpret_cast<const Message*>(other);
	const DeviceMessage* devMsg = dynamic_cast<const DeviceMessage*>(tmpMsg);

	if (devMsg)
	{
		try
		{
			msg = reinterpret_cast<GMSEC_Message>(new DeviceMessage(*devMsg));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "DeviceMessage handle is NULL or is not a DeviceMessage");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


void CALL_TYPE deviceMessageAddDevice(GMSEC_Message msg, const GMSEC_Device device, GMSEC_Status status)
{
	Status result;

	Message*       tmpMsg = reinterpret_cast<Message*>(msg);
	DeviceMessage* devMsg = dynamic_cast<DeviceMessage*>(tmpMsg);
	const Device*  dev    = reinterpret_cast<const Device*>(device);

	if (!devMsg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "DeviceMessage handle is NULL or is not a DeviceMessage");
	}
	else if (!dev)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Device handle is NULL");
	}
	else
	{
		devMsg->addDevice(*dev);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


size_t CALL_TYPE deviceMessageGetNumDevices(const GMSEC_Message msg, GMSEC_Status status)
{
	size_t numDevices = 0;
	Status result;

	const Message*       tmpMsg = reinterpret_cast<const Message*>(msg);
	const DeviceMessage* devMsg = dynamic_cast<const DeviceMessage*>(tmpMsg);

	if (devMsg)
	{
		numDevices = devMsg->getNumDevices();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "DeviceMessage handle is NULL or is not a DeviceMessage");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return numDevices;
}


const GMSEC_Device CALL_TYPE deviceMessageGetDevice(const GMSEC_Message msg, size_t index, GMSEC_Status status)
{
	GMSEC_Device device = NULL;
	Status       result;

	const Message*       tmpMsg = reinterpret_cast<const Message*>(msg);
	const DeviceMessage* devMsg = dynamic_cast<const DeviceMessage*>(tmpMsg);

	if (devMsg)
	{
		try
		{
			GMSEC_DISABLE_DEPRECATED_WARNINGS

			Device& tmp = const_cast<Device&>(devMsg->getDevice(index));

			GMSEC_ENABLE_DEPRECATED_WARNINGS

			device = reinterpret_cast<GMSEC_Device>(&tmp);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "DeviceMessage handle is NULL or is not a DeviceMessage");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return device;
}


GMSEC_DeviceIterator CALL_TYPE deviceMessageGetIterator(GMSEC_Message msg, GMSEC_Status status)
{
	GMSEC_DeviceIterator iter = NULL;
	Status               result;

	const Message*       tmpMsg = reinterpret_cast<const Message*>(msg);
	const DeviceMessage* devMsg = dynamic_cast<const DeviceMessage*>(tmpMsg);

	if (devMsg)
	{
		DeviceIterator& devIter = devMsg->getDeviceIterator();

		iter = &devIter;
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "DeviceMessage handle is NULL or is not a DeviceMessage");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return iter;
}


GMSEC_Message CALL_TYPE deviceMessageConvert(const GMSEC_Message msg, GMSEC_Status status)
{
	GMSEC_Message deviceMsg = NULL;
	Status        result;

	const Message* tmpMsg = reinterpret_cast<const Message*>(msg);

	if (tmpMsg)
	{
		GMSEC_DISABLE_DEPRECATED_WARNINGS
		DeviceMessage tmpDeviceMsg = DeviceMessage::convertMessage(*tmpMsg);
		GMSEC_ENABLE_DEPRECATED_WARNINGS

		deviceMsg = reinterpret_cast<GMSEC_Message>(new DeviceMessage(tmpDeviceMsg));
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return deviceMsg;
}


//
// DEPRECATED!
//
GMSEC_Message CALL_TYPE deviceMessageCreate(const char* subject, unsigned int version, GMSEC_Status status)
{
	GMSEC_WARNING << "This function has been deprecated; use the following function instead:\n"
	                 "deviceMessageCreateWithSpec(const char* subject, GMSEC_Specification spec, GMSEC_Status status)";

	GMSEC_Message msg = NULL;
	Status        result;

	try
	{
		msg = reinterpret_cast<GMSEC_Message>(new DeviceMessage(subject, buildSpecification(version)));
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


//
// DEPRECATED!
//
GMSEC_Message CALL_TYPE deviceMessageCreateWithConfig(const char* subject, const GMSEC_Config config, unsigned int version, GMSEC_Status status)
{
	GMSEC_WARNING << "This function has been deprecated; use the following function instead:\n"
	                 "deviceMessageCreateWithConfigAndSpec(const char* subject, GMSEC_Config config, GMSEC_Specification spec, GMSEC_Status status)";


	GMSEC_Message msg = NULL;
	Status        result;

	const Config* cfg = reinterpret_cast<const Config*>(config);

	if (cfg)
	{
		try
		{
			msg = reinterpret_cast<GMSEC_Message>(new DeviceMessage(subject, *cfg, buildSpecification(version)));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}
