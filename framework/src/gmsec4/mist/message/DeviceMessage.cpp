/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file DeviceMessage.cpp
 *
 * @brief This file contains the implementation of Device messages.
 */


#include <gmsec4/mist/message/DeviceMessage.h>

#include <gmsec4/internal/mist/message/InternalDeviceMessage.h>

#include <gmsec4/mist/Device.h>
#include <gmsec4/mist/DeviceIterator.h>
#include <gmsec4/mist/Specification.h>

#include <gmsec4/Config.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::mist::message::internal;


DeviceMessage::DeviceMessage(const char* subject, const Specification& spec)
	: MistMessage(new InternalDeviceMessage(subject, InternalDeviceMessage::buildSchemaID(spec).c_str(), spec))
{
}


DeviceMessage::DeviceMessage(const char* subject, const gmsec::api::Config& config, const Specification& spec)
	: MistMessage(new InternalDeviceMessage(subject, InternalDeviceMessage::buildSchemaID(spec).c_str(), config, spec))
{
}


DeviceMessage::DeviceMessage(const DeviceMessage& other)
	: MistMessage(new InternalDeviceMessage(dynamic_cast<const InternalDeviceMessage&>(other.getInternal())))
{
}


DeviceMessage::DeviceMessage(const char* data)
	: MistMessage(new InternalDeviceMessage(data))
{
}


DeviceMessage::DeviceMessage(const Specification& spec, const char* data)
	: MistMessage(new InternalDeviceMessage(spec, data))
{
}


DeviceMessage& DeviceMessage::operator=(const DeviceMessage& other)
{
	if (this != &other)
	{
		this->registerInternal(new InternalDeviceMessage(dynamic_cast<const InternalDeviceMessage&>(other.getInternal())));
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
	GMSEC_DISABLE_DEPRECATED_WARNINGS
	return DeviceMessage(message.toXML());
	GMSEC_ENABLE_DEPRECATED_WARNINGS
}
