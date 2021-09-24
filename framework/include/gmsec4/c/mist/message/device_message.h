/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file device_message.h
 *
 * @brief This file contains the public interface to Device messages.
 */


#ifndef GMSEC_API_C_MIST_DEVICE_MESSAGE_H
#define GMSEC_API_C_MIST_DEVICE_MESSAGE_H

#include <gmsec4/c/mist/mist_defs.h>

#include <gmsec4/util/Deprecated.h>
#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>


#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Message deviceMessageCreate(const char* subject, unsigned int version, GMSEC_Status status)
	 *
 	 * @note This function has been deprecated.  Use the functions in gmsec4/c/message/device_message.h instead.
 	 *
	 * @brief Creates a DeviceMessage object, and returns a handle to such.
	 *
	 * @param[in]  subject - the message subject
	 * @param[in]  version - the version of the GMSEC Interface Specification Document (ISD) to associate with the message
	 * @param[out] status  - the result of the operation
	 *
	 * @return A handle to a DeviceMessage object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa messageDestroy
	 * @sa C_GMSEC_ISD_2014_00
	 * @sa C_GMSEC_ISD_CURRENT
	 */
	GMSEC_API GMSEC_Message deviceMessageCreate(const char* subject, unsigned int version, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message deviceMessageCreateWithConfig(const char* subject, const GMSEC_Config config, unsigned int version, GMSEC_Status status)
	 *
 	 * @note This function has been deprecated.  Use the functions in gmsec4/c/message/device_message.h instead.
 	 *
	 * @brief Creates a DeviceMessage object, and returns a handle to such.
	 *
	 * @param[in]  subject - the message subject
	 * @param[in]  config  - a handle to a Config object to associate with the message
	 * @param[in]  version - the version of the GMSEC Interface Specification Document (ISD) to associate with the message
	 * @param[out] status  - the result of the operation
	 *
	 * @return A handle to a DeviceMessage object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa messageDestroy
	 * @sa C_GMSEC_ISD_2014_00
	 * @sa C_GMSEC_ISD_CURRENT
	 */
	GMSEC_API GMSEC_Message deviceMessageCreateWithConfig(const char* subject, const GMSEC_Config config, unsigned int version, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message deviceMessageCreateWithSpec(const char* subject,
	 *                                               GMSEC_Specification spec,
	 *                                               GMSEC_Status status)
	 *
	 * @brief Creates a DeviceMessage object, and returns a handle to such.
	 *
	 * @param[in]  subject - the message subject
	 * @param[in]  spec    - the specification this message's schema will adhere to
	 * @param[out] status  - the result of the operation
	 *
	 * @return A handle to a DeviceMessage object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa specificationCreate
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message deviceMessageCreateWithSpec(const char* subject,
	                                                    GMSEC_Specification spec,
	                                                    GMSEC_Status status);


	/**
	 * @fn GMSEC_Message deviceMessageCreateWithConfigAndSpec(const char* subject,
	 *                                                        GMSEC_Config config,
	 *                                                        GMSEC_Specification spec,
	 *                                                        GMSEC_Status status)
	 *
	 * @brief Creates a DeviceMessage object, and returns a handle to such.
	 *
	 * @param[in]  subject - the message subject
	 * @param[in]  config  - the configuration to associate with the message
	 * @param[in]  spec    - the specification this message's schema will adhere to
	 * @param[out] status  - the result of the operation
	 *
	 * @return A handle to a DeviceMessage object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa configCreate
	 * @sa specificationCreate
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message deviceMessageCreateWithConfigAndSpec(const char* subject,
	                                                             GMSEC_Config config,
	                                                             GMSEC_Specification spec,
	                                                             GMSEC_Status status);


	/**
	 * @fn GMSEC_Message deviceMessageCreateUsingData(const char* data, GMSEC_Status status)
	 *
	 * @brief Creates a DeviceMessage object, and returns a handle to such.
	 *
	 * @param[in]  data   - Initialize the DeviceMessage from either XML or JSON data string
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to a DeviceMessage object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa messageDestroy
	 */
	GMSEC_DEPRECATED GMSEC_API GMSEC_Message deviceMessageCreateUsingData(const char* data, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message deviceMessageCreateUsingSpecAndData(const GMSEC_Specification spec, const char* data, GMSEC_Status status)
	 *
	 * @brief Creates a Device Message instance using the given XML or JSON data representation of the message, and ensures
	 * the message adheres to the provided specification.
	 *
	 * @param[in]  spec   - A reference to the specification this message's schema will adhere to.
	 * @param[in]  data   - the XML or JSON string representation of a message.
	 * @param[out] status - the result of the operation.
	 *
	 * @return A handle to a Message object, or NULL if an error occurs. If the latter occurs, check the status.
	 *
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message deviceMessageCreateUsingSpecAndData(const GMSEC_Specification spec, const char* data, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message deviceMessageCreateCopy(const GMSEC_Message other, GMSEC_Status status)
	 *
	 * @brief Copies the given DeviceMessage object, and returns a handle to such.
	 *
	 * @param[in]  other  - the handle to the DeviceMessage object to copy
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to a DeviceMessage object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message deviceMessageCreateCopy(const GMSEC_Message other, GMSEC_Status status);


	/**
	 * @fn void deviceMessageAddDevice(GMSEC_Message msg, const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Adds the given Device object to the DeviceMessage.
	 *
	 * @param[in]  msg    - the handle to the DeviceMessage object
	 * @param[in]  device - the handle to the Device object to add to the DeviceMessage
	 * @param[out] status - the result of the operation
	 */
	GMSEC_API void deviceMessageAddDevice(GMSEC_Message msg, const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn size_t deviceMessageGetNumDevices(const GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Returns the number of Device object associated with the DeviceMessage.
	 *
	 * @param[in]  msg    - the handle to the DeviceMessage object
	 * @param[out] status - the result of the operation
	 *
	 * @return The number of Device objects associated with the DeviceMessage.
	 */
	GMSEC_API size_t deviceMessageGetNumDevices(const GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Device deviceMessageGetDevice(const GMSEC_Message msg, size_t index, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the Device object associated with the DeviceMessage that is referenced by the particular index.
	 *
	 * @param[in]  msg    - the handle to the DeviceMessage object
	 * @param[in]  index  - the index of the Device
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to the Device object, or NULL if an error occurs.  For the latter, check the status.
	 *
	 * @sa deviceMessageGetNumDevices
	 */
	GMSEC_DEPRECATED GMSEC_API const GMSEC_Device deviceMessageGetDevice(const GMSEC_Message msg, size_t index, GMSEC_Status status);


	/**
	 * @fn GMSEC_DeviceIterator deviceMessageGetIterator(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @desc Function that allows the callee to get the Device Iterator associated with the %DeviceMessage.
	 * This iterator will allow for applications to iterate over the Devices stored within the %DeviceMessage.
	 * The iterator is reset each time deviceMessageGetIterator() is called.  The iterator itself is destroyed when the
	 * %DeviceMessage object is destroyed.
	 *
	 * @note Only one Device Iterator is associated with a %DeviceMessage; multiple calls to
	 * deviceMessageGetIterator() will return a handle to the same Device Iterator.  Each call will reset the iterator.
	 *
	 * @param[in]  msg    - the handle to the Device Message
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A handle to a Device Iterator.
	 *
	 * @sa deviceMessageGetIterator()
	 * @sa deviceIteratorHasNext()
	 * @sa deviceIteratorNext()
	 */
	GMSEC_API GMSEC_DeviceIterator deviceMessageGetIterator(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message deviceMessageConvert(const GMSEC_Message msg)
	 *
	 * @brief Converts the given message (presumably a non-DeviceMessage, into a DeviceMessage.
	 *
	 * @param[in]  msg    - the handle to the Message to convert
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to a DeviceMessage, or NULL if an error occurs.  For the latter, check the status.
	 *
	 * @sa messageDestroy()
	 */
	GMSEC_DEPRECATED GMSEC_API GMSEC_Message deviceMessageConvert(const GMSEC_Message msg, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
