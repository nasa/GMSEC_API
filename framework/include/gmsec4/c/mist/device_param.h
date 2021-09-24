/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file device_param.h
 *
 *  @brief This file contains functions for the management of a MIST DeviceParam object.
 *
 * Example creation and use:
 * @code
 * GMSEC_Status      status = statusCreate();
 * GMSEC_Field       value  = i16FieldCreate("SC-TEMP", (GMSEC_I16) 432, status);
 * GMSEC_DeviceParam param  = deviceParamCreate("name", "2015-10-21 12:34:56", value, status);
 * GMSEC_Device      device = deviceCreate("MyDevice", GMSEC_NOMINAL_GREEN, status);
 *
 * if (param && device)
 * {
 *     deviceAddParam(device, param, status);
 *
 *     if (statusIsError(status) == GMSEC_TRUE)
 *     {
 *         printf("Error adding Device Parameter -- %s\n", statusGet(status));
 *     }
 * }
 * else
 * {
 *     printf("Error creating Field -- %s\n", statusGet(status));
 * }
 * @endcode
 */


#ifndef GMSEC_API_C_DEVICE_PARAM_H
#define GMSEC_API_C_DEVICE_PARAM_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_DeviceParam deviceParamCreate(const char* name, const char* timestamp, GMSEC_Field value, GMSEC_Status status)
	 *
	 * @brief Constructs a DeviceParam, and returns a handle to such.
	 *
	 * @param[in]  name      - the name of the device parameter
	 * @param[in]  timestamp - the time at which the sampled value was taken
	 * @param[in]  value     - the value of the parameter
	 * @param[out] status    - the result of the operation
	 *
	 * @return On succcess, a handle to a DeviceParam object is returned; otherwise NULL.  In case of the latter, refer to the returned status.
	 *
	 * @sa deviceParamDestroy
	 */
	GMSEC_API GMSEC_DeviceParam deviceParamCreate(const char* name, const char* timestamp, GMSEC_Field value, GMSEC_Status status);


	/**
	 * @fn GMSEC_DeviceParam deviceParamCreateCopy(const GMSEC_DeviceParam other, GMSEC_Status status)
	 *
	 * @brief Constructs a DeviceParam, using the information from another DeviceParam.
	 *
	 * @param[out] other  - the handle to the DeviceParam to copy
	 * @param[out] status - the result of the operation
	 *
	 * @return On succcess, a handle to a DeviceParam object is returned; otherwise NULL.  In case of the latter, refer to the returned status.
	 *
	 * @sa deviceParamDestroy
	 */
	GMSEC_API GMSEC_DeviceParam deviceParamCreateCopy(const GMSEC_DeviceParam other, GMSEC_Status status);


	/**
	 * @fn void deviceParamDestroy(GMSEC_DeviceParam* param)
	 *
	 * @brief Destroys the DeviceParam.
	 *
	 * @param[in,out] param - the handle to the DeviceParam to destroy
	 */
	GMSEC_API void deviceParamDestroy(GMSEC_DeviceParam* param);


	/**
	 * @fn const char* deviceParamGetName(const GMSEC_DeviceParam param, GMSEC_Status status)
	 *
	 * @brief Function that is used to return the name of the given DeviceParam.
	 *
	 * @param[in]  param  - the handle to the DeviceParam
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns the name associated with the DeviceParam, or NULL if error occurs.
	 */
	GMSEC_API const char* deviceParamGetName(const GMSEC_DeviceParam param, GMSEC_Status status);


	/**
	 * @fn const char* deviceParamGetTimestamp(const GMSEC_DeviceParam param, GMSEC_Status status)
	 *
	 * @brief Function that is used to return the timestamp of the value associated with the given DeviceParam.
	 *
	 * @param[in]  param  - the handle to the DeviceParam
	 * @param[out] status - the result of the operation
	 *
	 * @return The timestamp associated with the DeviceParam, or NULL if an error occurs.
	 */
	GMSEC_API const char* deviceParamGetTimestamp(const GMSEC_DeviceParam param, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field deviceParamGetValue(const GMSEC_DeviceParam param, GMSEC_Status status)
	 *
	 * @brief Function that is used to return the value associated with the given DeviceParam.
	 *
	 * @param[in]  param  - the handle to the DeviceParam
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to the value Field associated with the DeviceParam, or NULL if an error occurs.
	 */
	GMSEC_API const GMSEC_Field deviceParamGetValue(const GMSEC_DeviceParam param, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
