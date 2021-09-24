/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file service_param.h
 *
 *  @brief This file contains functions for the management of a MIST Simple Service Parameter object.
 */


#ifndef GMSEC_API_C_SERVICE_PARAM_H
#define GMSEC_API_C_SERVICE_PARAM_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_ServiceParam serviceParamCreate(const char* name, const GMSEC_Field value, GMSEC_Status status)
	 *
	 * @brief Creates a Service Parameter object, and returns a handle to such.
	 *
	 * @param[in]  name   - the name of the Service Parameter
	 * @param[in]  value  - a handle to a Field object that contains the value of the Service Parameter
	 * @param[out] status - result of the operation
	 *
	 * @return A handle to a ServiceParam object, or NULL if an error occurs.  In case of the latter, check the status.
	 *
	 * @sa serviceParamDestroy
	 */
	GMSEC_API GMSEC_ServiceParam serviceParamCreate(const char* name, const GMSEC_Field value, GMSEC_Status status);


	/**
	 * @fn GMSEC_ServiceParam serviceParamCreateCopy(const GMSEC_ServiceParam other, GMSEC_Status status)
	 *
	 * @brief Creates a deep-copy of the given Service Parameter object, and returns a handle to such.
	 *
	 * @param[in]  other  - the handle to the ServiceParam object to copy
	 * @param[out] status - result of the operation
	 *
	 * @return A handle to a ServiceParam object, or NULL if an error occurs.  In case of the latter, check the status.
	 *
	 * @sa serviceParamDestroy
	 */
	GMSEC_API GMSEC_ServiceParam serviceParamCreateCopy(const GMSEC_ServiceParam other, GMSEC_Status status);


	/**
	 * @fn void serviceParamDestroy(GMSEC_ServiceParam* param)
	 *
	 * @brief Destroys the given ServiceParam object.
	 *
	 * @param[in,out] param - the handle to the ServiceParam object to destroy
	 */
	GMSEC_API void serviceParamDestroy(GMSEC_ServiceParam* param);


	/**
	 * @fn const char* serviceParamGetName(GMSEC_ServiceParam param, GMSEC_Status status)
	 *
	 * @brief Returns the name assigned to the ServiceParam object.
	 *
	 * @param[in]  param  - the handle to the ServiceParam object
	 * @param[out] status - result of the operation
	 *
	 * @return The name assigned to the ServiceParam object, or NULL if an error occurs.
	 */
	GMSEC_API const char* serviceParamGetName(GMSEC_ServiceParam param, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field serviceParamGetValue(GMSEC_ServiceParam param, GMSEC_Status status)
	 *
	 * @brief Returns the field value assigned to the ServiceParam object.
	 *
	 * @param[in]  param  - the handle to the ServiceParam object
	 * @param[out] status - result of the operation
	 *
	 * @return The field value assigned to the ServiceParam object, or NULL if an error occurs.
	 */
	GMSEC_API const GMSEC_Field serviceParamGetValue(GMSEC_ServiceParam param, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
