/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file device.h
 *
 *  @brief This file contains functions for the management of a MIST Device object.
 *
 * Example creation and use:
 * @code
 * GMSEC_Status  status = statusCreate();
 * GMSEC_Device  device = deviceCreate("MyDevice", GMSEC_NOMINAL_GREEN, status);
 *
 * if (device)
 * {
 *     deviceDestroy(&device);
 * }
 * else
 * {
 *     printf("Error -- %s\n", statusGet(status));
 * }
 * @endcode
 */


#ifndef GMSEC_API_C_DEVICE_H
#define GMSEC_API_C_DEVICE_H

#include <gmsec4/c/connection.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>

#include <stddef.h>


/**
 * @enum GMSEC_DeviceStatus
 *
 * @sa deviceCreate
 * @sa deviceGetStatus
 * @sa deviceSetStatus
 */
typedef enum
{
	GMSEC_DEBUG,          /**< debug */
	GMSEC_NOMINAL_GREEN,  /**< nominal green */
	GMSEC_YELLOW,         /**< yellow */
	GMSEC_ORANGE,         /**< orange */
	GMSEC_RED             /**< red */
} GMSEC_DeviceStatus;


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Device deviceCreate(const char* name, GMSEC_DeviceStatus deviceStatus, GMSEC_Status status)
	 *
	 * @brief Creates a Device, and returns a handle to such.
	 *
	 * @param[in]  name         - the name used to identify the device
	 * @param[in]  deviceStatus - the status of the device
	 * @param[out] status       - result of the operation
	 *
	 * @return A handle to a Device, or NULL if an error occurs.  For the latter, check the status.
	 *
	 * @sa deviceDestroy
	 */
	GMSEC_API GMSEC_Device deviceCreate(const char* subject, GMSEC_DeviceStatus deviceStatus, GMSEC_Status status);


	/**
	 * @fn GMSEC_Device deviceCreateCopy(const GMSEC_Device other, GMSEC_Status status)
	 *
	 * @brief Creates a copy of the given Device.
	 *
	 * @param[in]  other  - the Device to copy
	 * @param[out] status - result of the operation
	 *
	 * @return A handle to a Device, or NULL if an error occurs.  For the latter, check the status.
	 *
	 * @sa deviceDestroy
	 */
	GMSEC_API GMSEC_Device deviceCreateCopy(const GMSEC_Device other, GMSEC_Status status);


	/**
	 * @fn void deviceDestroy(GMSEC_Device* device)
	 *
	 * @brief Destroys the Device.
	 *
	 * @param[in,out] device - the handle to the device to destroy
	 */
	GMSEC_API void deviceDestroy(GMSEC_Device* device);


	/**
	 * @fn const char* deviceGetName(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns the name of the device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return Returns the name of the device, or NULL if an error occurs.  For the latter, check the status.
	 */
	GMSEC_API const char* deviceGetName(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn GMSEC_DeviceStatus deviceGetStatus(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns the status of the given device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return Enumerated value correlating with the device's status; check status to ensure value is valid.
	 */
	GMSEC_API GMSEC_DeviceStatus deviceGetStatus(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn void deviceSetStatus(GMSEC_Device device, GMSEC_DeviceStatus deviceStatus, GMSEC_Status status)
	 *
	 * @brief Allows for the setting of the device status, should the status have change since the creation of the device.
	 *
	 * @param[in]  device       - the handle to the device
	 * @param[in]  deviceStatus - the status of the device
	 * @param[out] status       - result of the operation
	 */
	GMSEC_API void deviceSetStatus(GMSEC_Device device, GMSEC_DeviceStatus deviceStatus, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL deviceNumberAvailable(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns whether there is a number field associated with the device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return Returns GMSEC_TRUE if a number field is available; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL deviceNumberAvailable(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field deviceGetNumber(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns the handle to the number field (if available).
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return A handle to the device's number field, or NULL if the field is not available.
	 */
	GMSEC_API const GMSEC_Field deviceGetNumber(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn void deviceSetNumber(GMSEC_Device device, const GMSEC_Field number, GMSEC_Status status)
	 *
	 * @brief Associates the given number field with the device.
	 *
	 * @note The Device will make a copy of the number field; the user is responsible for destroying the original.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[in]  number - the handle to the number field
	 * @param[out] status - result of the operation
	 */
	GMSEC_API void deviceSetNumber(GMSEC_Device device, const GMSEC_Field number, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL deviceModelAvailable(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns whether there is a model string associated with the device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return Returns GMSEC_TRUE if a model string is available; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL deviceModelAvailable(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn const char* deviceGetModel(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns the model string (if available).
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return The device's model string, or NULL if it is not available.
	 */
	GMSEC_API const char* deviceGetModel(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn void deviceSetModel(GMSEC_Device device, const char* model, GMSEC_Status status)
	 *
	 * @brief Associates the given model string with the device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[in]  model  - the model string
	 * @param[out] status - result of the operation
	 */
	GMSEC_API void deviceSetModel(GMSEC_Device device, const char* model, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL deviceSerialAvailable(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns whether there is a serial string associated with the device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return Returns GMSEC_TRUE if a serial string is available; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL deviceSerialAvailable(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn const char* deviceGetSerial(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns the serial string (if available).
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return The device's serial string, or NULL if it is not available.
	 */
	GMSEC_API const char* deviceGetSerial(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn void deviceSetSerial(GMSEC_Device device, const char* serial, GMSEC_Status status)
	 *
	 * @brief Associates the given serial string with the device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[in]  serial - the serial string
	 * @param[out] status - result of the operation
	 */
	GMSEC_API void deviceSetSerial(GMSEC_Device device, const char* serial, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL deviceVersionAvailable(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns whether there is a version string associated with the device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return Returns GMSEC_TRUE if a number version string is available; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL deviceVersionAvailable(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn const char* deviceGetVersion(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns the version string (if available).
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return The device's version string, or NULL if it is not available.
	 */
	GMSEC_API const char* deviceGetVersion(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn void deviceSetVersion(GMSEC_Device device, const char* version, GMSEC_Status status)
	 *
	 * @brief Associates the given version string with the device.
	 *
	 * @param[in]  device  - the handle to the device
	 * @param[in]  version - the version string
	 * @param[out] status  - result of the operation
	 */
	GMSEC_API void deviceSetVersion(GMSEC_Device device, const char* version, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL deviceGroupAvailable(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns whether there is a group string associated with the device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return Returns GMSEC_TRUE if a group string is available; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL deviceGroupAvailable(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn const char* deviceGetGroup(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns the group string (if available).
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return The device's group string, or NULL if it is not available.
	 */
	GMSEC_API const char* deviceGetGroup(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn void deviceSetGroup(GMSEC_Device device, const char* group, GMSEC_Status status)
	 *
	 * @brief Associates the given group string with the device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[in]  group  - the group string
	 * @param[out] status - result of the operation
	 */
	GMSEC_API void deviceSetGroup(GMSEC_Device device, const char* group, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL deviceRoleAvailable(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns whether there is a role string associated with the device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return Returns GMSEC_TRUE if a role string is available; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL deviceRoleAvailable(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn const char* deviceGetRole(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns the role string (if available).
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return The device's role string, or NULL if it is not available.
	 */
	GMSEC_API const char* deviceGetRole(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn void deviceSetRole(GMSEC_Device device, const char* role, GMSEC_Status status)
	 *
	 * @brief Associates the given role string with the device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[in]  role   - the role string
	 * @param[out] status - result of the operation
	 */
	GMSEC_API void deviceSetRole(GMSEC_Device device, const char* role, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL deviceInfoAvailable(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns whether there is an information field associated with the device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return Returns GMSEC_TRUE if an information field is available; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL deviceInfoAvailable(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn const char* deviceGetInfo(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the information field (if available).
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return A handle to the device's information field, or NULL if it is not available.
	 */
	GMSEC_API const GMSEC_Field deviceGetInfo(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn void deviceSetInfo(GMSEC_Device device, const GMSEC_Field info, GMSEC_Status status)
	 *
	 * @brief Associates the given information field with the device.
	 *
	 * @note The Device will make a copy of the information field; the user is responsible for destroying the original.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[in]  info   - the handle to the information field
	 * @param[out] status - result of the operation
	 */
	GMSEC_API void deviceSetInfo(GMSEC_Device device, const GMSEC_Field info, GMSEC_Status status);


	/**
	 * @fn void deviceAddParam(GMSEC_Device device, const GMSEC_DeviceParam param, GMSEC_Status status)
	 *
	 * @brief Adds a copy of the give DeviceParam to the Device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[in]  param  - the handle to the DeviceParam object to add
	 * @param[out] status - result of the operation
	 */
	GMSEC_API void deviceAddParam(GMSEC_Device device, const GMSEC_DeviceParam param, GMSEC_Status status);


	/**
	 * @fn size_t deviceGetParamCount(const GMSEC_Device device, GMSEC_Status status)
	 *
	 * @brief Returns the number of DeviceParam objects associated with the Device.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[out] status - result of the operation
	 *
	 * @return The number of DeviceParam objects associated with the Device.
	 */
	GMSEC_API size_t deviceGetParamCount(const GMSEC_Device device, GMSEC_Status status);


	/**
	 * @fn const GMSEC_DeviceParam deviceGetParam(const GMSEC_Device device, size_t index, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the DeviceParam object within the Device referenced at the given index.
	 *
	 * @param[in]  device - the handle to the device
	 * @param[in]  index  - the index of the DeviceParam to return
	 * @param[out] status - result of the operation
	 *
	 * @return A handle to a DeviceParam object, or NULL if an error occurs.  For the latter, check the status.
	 *
	 * @sa deviceGetParamCount
	 */
	GMSEC_API const GMSEC_DeviceParam deviceGetParam(const GMSEC_Device device, size_t index, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
