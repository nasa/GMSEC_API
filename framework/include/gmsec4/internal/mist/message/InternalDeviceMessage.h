/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalDeviceMessage.h
 *
 * @brief This file contains the interface to InternalDeviceMessage,
 *        which is a subclass of InternalMessage containing MSG C2CX DEV
 *        message contents specifically.
 */


#ifndef GMSEC_API_MESSAGE_INTERNAL_DEVICE_MESSAGE_H
#define GMSEC_API_MESSAGE_INTERNAL_DEVICE_MESSAGE_H

#include <gmsec4/internal/mist/message/InternalMistMessage.h>

#include <gmsec4/mist/Device.h>
#include <gmsec4/mist/DeviceIterator.h>

#include <string>
#include <vector>


namespace gmsec
{
namespace api
{
namespace mist
{
	class Specification;

namespace message
{
namespace internal
{

/**
 * @class InternalDeviceMessage
 *
 * @brief The Message object contains a GMSEC MSG C2CX DEV message.
 *
 * @sa Message @n
 *     Config @n
 *     Connection @n
 *     Field @n
 *     MsgFieldIterator
 */
class GMSEC_API InternalDeviceMessage : public InternalMistMessage
{
  public:

	/**
	 * @fn InternalDeviceMessage(const char* subject, unsigned int version)
	 *
	 * @brief Default constructor - initializes the message instance
	 *
	 * @param subject - the subject string for the message.
	 * @param version - The version of the GMSEC message specification to
	 *                  be used.
	 *
	 */
	InternalDeviceMessage(const char* subject, const char* id, const Specification& spec);


	/**
	 * @fn InternalDeviceMessage(const char* subject, const Config& config, unsigned int version)
	 *
	 * @brief constructor - initializes the message instance and associates a Configuration object
	 *
	 * @param subject - the subject string for the message.
	 * @param config - a configuration to associate with the message.
	 * @param version - The version of the GMSEC message specification to
	 *                  be used.
	 *
	 */
	InternalDeviceMessage(const char* subject, const char* id, const gmsec::api::Config& config, const Specification& spec);


	/**
	 * @fn InternalDeviceMessage(const InternalDeviceMessage& other)
	 *
	 * @brief Copy constructor - initializes the message instance using the other given InternalDeviceMessage
	 *
	 * @param other - the other InternalDeviceMessage object to copy.
	 */
	 InternalDeviceMessage(const InternalDeviceMessage& other);


	/**
	 * @fn InternalDeviceMessage(const char* data)
	 *
	 * @brief Initializes InternalDeviceMessage from an XML or JSON string.
	 *
	 * @param data - XML or JSON string used to initialize InternalDeviceMessage.
	 */
	InternalDeviceMessage(const char* data);


	InternalDeviceMessage(const Specification& spec, const char* data);


	/**
	 * @fn ~InternalDeviceMessage()
	 *
	 * @brief Destructor - cleans up the message instance
	 */
	virtual ~InternalDeviceMessage();


	/**
	 * @fn InternalDeviceMessage& operator=(const InternalDeviceMessage& other)
	 *
	 * @brief Overloaded assignment operator method for making a copy of other InternalDeviceMessage object
	 *
	 * @param other - the other InternalDeviceMessage object to copy.
	 *
	 * @return A copy of the other InternalDeviceMessage object that was given.
	 */
	InternalDeviceMessage& CALL_TYPE operator=(const InternalDeviceMessage& other);


	/**
	 * @fn void addDevice(const Device& device)
	 *
	 * @brief Add a device to the message.
	 *
	 */
	void CALL_TYPE addDevice(const Device& device);


	/**
	 * @fn const Device& getDevice(size_t index) const
	 *
	 * @brief Get device numbered at index.
	 *
	 * @return Device at specified index inside of DeviceMessage.
	 *
	 * @throw Throws an exception if the index specified is not in the
	 *        range of Devices in this message.
	 *
	 */
	const Device& CALL_TYPE getDevice(size_t index) const;


	/**
	 * @fn size_t getNumDevices() const
	 *
	 * @brief Get the number of devices in this message.
	 *
	 * @return Return the number of devices in the message.
	 */
	size_t CALL_TYPE getNumDevices() const;


	DeviceIterator& getDeviceIterator();


	void resetDeviceIterator();


	bool hasNextDevice() const;


	const Device& nextDevice();


	//helper function, determines message kind and builds appropriate SchemaID
	static std::string buildSchemaID(const Specification& spec);


private:
	Device extractMessageDevice(size_t index) const;


	void init();


	std::vector<Device>              m_list;
	std::vector<Device>::iterator    m_listIter;
	gmsec::api::mist::DeviceIterator m_deviceIterator;
};

} // namespace internal
} // namespace message
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
