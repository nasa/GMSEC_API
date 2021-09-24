/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file DeviceMessage.h
 *
 * @brief This file contains the public interface to Device messages.
 */


#ifndef GMSEC_API_DEVICE_MESSAGE_H
#define GMSEC_API_DEVICE_MESSAGE_H

#include <gmsec4/Message.h>

#include <gmsec4/util/Deprecated.h>


namespace gmsec
{

namespace api
{

namespace mist
{
	class Device;
	class DeviceIterator;

	namespace internal
	{
		class InternalDeviceIterator;
	}

	namespace message
	{
		namespace internal
		{
			class InternalDeviceMessage;
		}
	}


/**
 * @class DeviceMessage
 *
 * @brief The Message object contains a GMSEC MSG C2CX DEV message.
 * The following message schema IDs and their templates are supported: @n
 * 2014.00.GMSEC.MSG.C2CX.DEV
 * 2016.00.GMSEC.MSG.C2CX.DEV
 *
 * @note gmsec::api::mist::DeviceMessage has been deprecated.  It is recommended to use gmsec::api::mist::message::DeviceMessage
 *
 * @sa Message @n
 *     Config @n
 *     Connection @n
 *     Field @n
 *     MsgFieldIterator
 */
class GMSEC_API DeviceMessage : public Message
{
  public:

	/**
	 * @fn DeviceMessage(const char* subject, unsigned int version)
	 *
	 * @brief Default constructor - Initializes the message instance
	 *
	 * @param subject - The subject string for the message.
	 * @param version - The version of the GMSEC message specification to be used.
	 */
	GMSEC_DEPRECATED DeviceMessage(const char* subject, unsigned int version);


	/**
	 * @fn DeviceMessage(const char* subject, const Config& config, unsigned int version)
	 *
	 * @brief constructor - Initializes the message instance and associates a Configuration object
	 *
	 * @param subject - The subject string for the message.
	 * @param config - A configuration to associate with the message.
	 * @param version - The version of the GMSEC message specification to be used.
	 */
	GMSEC_DEPRECATED DeviceMessage(const char* subject, const Config& config, unsigned int version);


	/**
	 * @fn DeviceMessage(const DeviceMessage& other)
	 *
	 * @brief Copy constructor - initializes the message instance using the other given DeviceMessage
	 *
	 * @param other - the other DeviceMessage object to copy.
	 */
	GMSEC_DEPRECATED DeviceMessage(const DeviceMessage& other);


	/**
	 * @fn DeviceMessage(const char* data)
	 *
	 * @brief Initialize DeviceMessage from XML or JSON string.
	 *
	 * @param data - XML or JSON string used to initialize DeviceMessage.
	 */
	GMSEC_DEPRECATED DeviceMessage(const char* data);


	/**
	 * @fn ~DeviceMessage()
	 *
	 * @brief Destructor - cleans up the message instance
	 */
	virtual ~DeviceMessage();


	/**
	 * @fn DeviceMessage& operator=(const DeviceMessage& other)
	 *
	 * @brief Overloaded assignment operator method for making a copy of other DeviceMessage object
	 *
	 * @param other - the other DeviceMessage object to copy.
	 *
	 * @return A copy of the other DeviceMessage object that was given.
	 */
	DeviceMessage& operator=(const DeviceMessage& other);


	/**
	 * @fn void addDevice(const Device& device)
	 *
	 * @brief Add a device to the message.
	 */
	void CALL_TYPE addDevice(const Device& device);


	/**
	 * @fn size_t getNumDevices() const
	 *
	 * @brief Get the number of devices in this message.
	 *
	 * @return Return the number of devices in the message.
	 */
	size_t CALL_TYPE getNumDevices() const;


	/**
	 * @fn const Device& getDevice(size_t index) const
	 *
	 * @brief Get device numbered at index.
	 *
	 * @return Device at specified index inside of DeviceMessage.
	 *
	 * @throw Throws an exception if the index specified is not in the
	 *        range of Devices in the message.
	 */
	const Device& CALL_TYPE getDevice(size_t index) const;


	/**
	 * @fn DeviceIterator& getDeviceIterator() const
	 *
	 * @desc Method that allows the callee to get the DeviceIterator associated with the %DeviceMessage.
	 * This iterator will allow for applications to iterate over the Device objects stored within the %DeviceMessage.
	 * The iterator is reset each time getDeviceIterator() is called.  The iterator itself is destroyed when the
	 * %DeviceMessage object is destroyed.
	 *
	 * @note Only one DeviceIterator object is associated with a %DeviceMessage object; multiple calls to
	 * getDeviceIterator() will return a reference to the same DeviceIterator object.  Each call will reset the iterator.
	 *
	 * @return A reference to a DeviceIterator object.
	 */
	gmsec::api::mist::DeviceIterator& CALL_TYPE getDeviceIterator() const;


	/**
	 * @fn DeviceMessage convertMessage(const Message& message)
	 *
	 * @brief Constructs and returns a DeviceMessage from an an ordinary message.
	 */
	GMSEC_DEPRECATED static DeviceMessage CALL_TYPE convertMessage(const Message& message);


private:
	friend class gmsec::api::mist::internal::InternalDeviceIterator;

	// Defined so as to preserve binary compatibility with API 4.1
	void* filler;
};

} // namespace mist
} // namespace api
} // namespace gmsec

#endif
