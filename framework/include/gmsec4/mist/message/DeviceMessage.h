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


#ifndef GMSEC_API_MESSAGE_DEVICE_MESSAGE_H
#define GMSEC_API_MESSAGE_DEVICE_MESSAGE_H

#include <gmsec4/mist/message/MistMessage.h>

#include <gmsec4/util/Deprecated.h>


namespace gmsec
{
namespace api
{
namespace mist
{
	// Forward declaration(s)
	class Device;
	class DeviceIterator;
	class Specification;

	namespace internal
	{
		class InternalDeviceIterator;
	}

namespace message
{
	namespace internal
	{
		// Forward declaration(s)
		class InternalDeviceMessage;
	}

/**
 * @class DeviceMessage
 *
 * @brief A MistMessage object that is capable of storing Device objects
 * The following message schema IDs and their templates are supported: @n
 * MSG.DEV
 *
 * @sa Message @n
 *	   MistMessage @n
 *     Config @n
 *	   Specification @n
 *     Field @n
 *     MsgFieldIterator
 */
class GMSEC_API DeviceMessage : public MistMessage
{
public:
	/**
	 * @fn DeviceMessage(const char* subject, const Specification& spec)
	 *
	 * @brief Default constructor - Initializes the message instance and automatically builds the appropriate 
	 * schemaID based on the version of Specification used
	 *
	 * @param subject - The subject string for the message.
	 * @param spec - A reference to the specification this message's schema will adhere to.
	 * 
	 * @throw An exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.
	 */
	  DeviceMessage(const char* subject, const Specification& spec);


	/**
	 * @fn DeviceMessage(const char* subject, const Config& config, const Specification& spec)
	 *
	 * @brief constructor - Initializes the message instance and associates a Configuration object and 
	 * automatically builds the appropriate schemaID based on the version of Specification used
	 *
	 * @param subject - The subject string for the message.
	 * @param config - A configuration to associate with the message.
	 * @param spec - A reference to the specification this message's schema will adhere to.
	 * 
	 * @throw An exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.
	 */
	  DeviceMessage(const char* subject, const gmsec::api::Config& config, const Specification& spec);


	/**
	 * @fn DeviceMessage(const DeviceMessage& other)
	 *
	 * @brief Copy constructor - initializes the message instance using the other given DeviceMessage
	 *
	 * @param other - the other DeviceMessage object to copy.
	 */
	DeviceMessage(const DeviceMessage& other);


	/**
	 * @fn DeviceMessage(const char* data)
	 *
	 * @brief Initialize DeviceMessage from XML or JSON string.
	 *
	 * @param data - XML or JSON string used to initialize DeviceMessage.
	 */
	GMSEC_DEPRECATED DeviceMessage(const char* data);


	/**
	 * @fn DeviceMessage(const Specification& spec, const char* data)
	 *
	 * @brief Initializes a Device Message using the given XML or JSON data representation of the message,
	 * and ensures the message adheres to the provided specification.
	 *
	 * @param spec - A reference to the specification this message's schema will adhere to.
	 * @param data - XML or JSON string used to initialize the message
	 *
	 * @throw An Exception is thrown if the given data does not represent a valid XML or JSON statement.
	 * @throw An Exception is thrown if the given data does not represent a Device Message.
	 */
	DeviceMessage(const Specification& spec, const char* data);


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
	 * @brief Get device numbered at index. Note that the index which will be
	 * retrieved does not correspond to the GMSEC ISD and starts from 0 instead of 1.
	 * For example, getDevice(0) would return the Device corresponding to
	 * DEVICE.1.
	 *
	 *
	 * @return Device at specified index inside of DeviceMessage.
	 *
	 * @throw Throws an exception if the index specified is not in the
	 *        range of Devices in the message.
	 *
	 * @note This function has been deprecated, use DeviceIterator instead.
	 */
	GMSEC_DEPRECATED const Device& CALL_TYPE getDevice(size_t index) const;


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
	DeviceIterator& CALL_TYPE getDeviceIterator() const;


	/**
	 * @fn DeviceMessage convertMessage(const Message& message)
	 *
	 * @brief Constructs and returns a DeviceMessage from an an ordinary message.
	 */
	static DeviceMessage CALL_TYPE convertMessage(const Message& message);


private:
	friend class gmsec::api::mist::internal::InternalDeviceIterator;
};

} // namespace message
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
