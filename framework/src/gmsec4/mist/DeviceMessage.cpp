/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file DeviceMessage.cpp
 *
 * @brief This file contains the implementation of Device messages.
 */


#include <gmsec4/mist/DeviceMessage.h>

#include <gmsec4/internal/mist/message/InternalDeviceMessage.h>

#include <gmsec4/mist/Device.h>
#include <gmsec4/mist/DeviceIterator.h>

#include <gmsec4/mist/Specification.h>

#include <gmsec4/Config.h>

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::mist::message::internal;


DeviceMessage::DeviceMessage(const char* subject, unsigned int version)
	: Message(new InternalDeviceMessage(subject,
	                                    InternalMistMessage::buildSchemaID(PUBLISH, "C2CX", "DEV", version).c_str(),
	                                    InternalMistMessage::buildSpecification(version)))
{
	GMSEC_WARNING << "This class has been deprecated; use gmsec::api::mist::message::DeviceMessage instead.";
}


DeviceMessage::DeviceMessage(const char* subject, const Config& config, unsigned int version)
	: Message(new InternalDeviceMessage(subject,
	                                    InternalMistMessage::buildSchemaID(PUBLISH, "C2CX", "DEV", version).c_str(),
	                                    config,
	                                    InternalMistMessage::buildSpecification(version)))
{
	GMSEC_WARNING << "This class has been deprecated; use gmsec::api::mist::message::DeviceMessage instead.";
}


DeviceMessage::DeviceMessage(const DeviceMessage& other)
	: Message(new InternalDeviceMessage(dynamic_cast<InternalDeviceMessage&>(other.getInternal())))
{
	GMSEC_WARNING << "This class has been deprecated; use gmsec::api::mist::message::DeviceMessage instead.";
}


DeviceMessage::DeviceMessage(const char* data)
	: Message(new InternalDeviceMessage(data))
{
	GMSEC_WARNING << "This class has been deprecated; use gmsec::api::mist::message::DeviceMessage instead.";
}


DeviceMessage& DeviceMessage::operator=(const DeviceMessage& other)
{
	GMSEC_WARNING << "This class has been deprecated; use gmsec::api::mist::message::DeviceMessage instead.";

	if (this != &other)
	{
		this->registerInternal(new InternalDeviceMessage(dynamic_cast<InternalDeviceMessage&>(other.getInternal())));
	}

	return *this;
}


DeviceMessage::~DeviceMessage()
{
}


void DeviceMessage::addDevice(const Device& device)
{
	dynamic_cast<InternalDeviceMessage&>(getInternal()).addDevice(device);
}


size_t DeviceMessage::getNumDevices() const
{
	return dynamic_cast<InternalDeviceMessage&>(getInternal()).getNumDevices();
}


const Device& DeviceMessage::getDevice(size_t index) const
{
	return dynamic_cast<InternalDeviceMessage&>(getInternal()).getDevice(index);
}


DeviceIterator& DeviceMessage::getDeviceIterator() const
{
	return dynamic_cast<InternalDeviceMessage&>(getInternal()).getDeviceIterator();
}


DeviceMessage DeviceMessage::convertMessage(const Message& message)
{
	return DeviceMessage(message.toXML());
}
