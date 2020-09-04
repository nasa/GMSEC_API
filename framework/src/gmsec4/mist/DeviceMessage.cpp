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

#include <gmsec4/internal/mist/InternalDeviceMessage.h>

#include <gmsec4/mist/Device.h>
#include <gmsec4/mist/DeviceIterator.h>

#include <gmsec4/Config.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


DeviceMessage::DeviceMessage(const char* subject, unsigned int version)
	: Message(subject, Message::PUBLISH),
	  m_internal(new InternalDeviceMessage(subject, version))
{
	registerChild(m_internal);
}


DeviceMessage::DeviceMessage(const char* subject, const Config& config, unsigned int version)
	: Message(subject, Message::PUBLISH, config),
	  m_internal(new InternalDeviceMessage(subject, config, version))
{
	registerChild(m_internal);
}


DeviceMessage::DeviceMessage(const DeviceMessage& other)
	: Message(other),
	  m_internal(new InternalDeviceMessage(*(other.m_internal)))
{
	registerChild(m_internal);
}


DeviceMessage::DeviceMessage(const char* data)
	: Message(data),
	  m_internal(new InternalDeviceMessage(data))
{
	registerChild(m_internal);
}


DeviceMessage& DeviceMessage::operator=(const DeviceMessage& other)
{
	if (this != &other)
	{
		delete m_internal;

		m_internal = new InternalDeviceMessage(*(other.m_internal));

		registerChild(m_internal);
	}

	return *this;
}


DeviceMessage::~DeviceMessage()
{
	// Message parent class deletes m_internal (if it's been registered).
}


void DeviceMessage::addDevice(const Device& device)
{
	m_internal->addDevice(device);
}


size_t DeviceMessage::getNumDevices() const
{
	return m_internal->getNumDevices();
}


const Device& DeviceMessage::getDevice(size_t index) const
{
	return m_internal->getDevice(index);
}


DeviceIterator& DeviceMessage::getDeviceIterator() const
{
	return m_internal->getDeviceIterator();
}


DeviceMessage DeviceMessage::convertMessage(const Message& message)
{
	DeviceMessage device_message(message.toXML());

	return device_message;
}
