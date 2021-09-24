/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file status.h
 *
 * @brief This file contains functions for the management of status.
 *
 * Example creation & use:
 * @code
 * GMSEC_Status status = statusCreate();
 *
 * ...
 *
 * statusDestroy(status);
 * @endcode
 */


#ifndef GMSEC_API_C_STATUS_H
#define GMSEC_API_C_STATUS_H

#include <gmsec4/c/errors.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Status statusCreate(void)
	 *
	 * @brief This function will create a status that can be used for C API calls.
	 * This status can be reused for multiple calls, but must be cleaned up when
	 * it is no longer needed.
	 *
	 * @return A handle to a status object.
	 *
	 * @sa destroyStatus()
	 */
	GMSEC_API GMSEC_Status statusCreate(void);


	/**
	 * @fn GMSEC_Status statusCreateWithValues(GMSEC_StatusClass clazz, GMSEC_StatusCode code, const char* reason, GMSEC_I32 custom)
	 *
	 * @brief This function will create a status that is initialized with the given parameter values.
	 *
	 * @param[in] clazz  - the status class
	 * @param[in] code   - the status code
	 * @param[in] reason - the reason to include with the status (or NULL if none)
	 * @param[in] custom - the custom status code (if none, then set to 0)
	 *
	 * @return A handle to a status object.
	 *
	 * @sa destroyStatus()
	 */
	GMSEC_API GMSEC_Status statusCreateWithValues(GMSEC_StatusClass clazz, GMSEC_StatusCode code, const char* reason, GMSEC_I32 custom);


	/**
	 * @fn GMSEC_Status statusCreateCopy(const GMSEC_Status other)
	 *
	 * @brief Creates a copy of the given Status object, and returns a handle to such.
	 *
	 * @param[in] other - the handle to the Status object to copy
	 *
	 * @return A handle to a status object.
	 *
	 * @sa destroyStatus()
	 */
	GMSEC_API GMSEC_Status statusCreateCopy(const GMSEC_Status other);


	/**
	 * @fn void statusDestroy(GMSEC_Status* status)
	 *
	 * @brief This function will clean up a status and any related memory.
	 *
	 * @param[in,out] status - status handle to clean up
	 *
	 * @sa createStatus()
	 */
	GMSEC_API void statusDestroy(GMSEC_Status* status);


	/**
	 * @fn GMSEC_BOOL statusIsError(const GMSEC_Status status)
	 *
	 * @brief Used to discern if an error has been returned.
	 *
	 * @param[in] status - status handle
	 *
	 * @return GMSEC_TRUE if there is an error, GMSEC_FALSE otherwise
	 */
	GMSEC_API GMSEC_BOOL statusIsError(const GMSEC_Status status);


	/**
	 * @fn const char* statusGet(const GMSEC_Status status)
	 *
	 * @brief Get the complete status string from this status.
	 * This string will be the form: "[class,code,custom code]: reason"
	 *
	 * @param[out] status - status to get string from
	 *
	 * @return error string
	 */
	GMSEC_API const char* statusGet(const GMSEC_Status status);


	/**
	 * @fn void statusSet(GMSEC_Status status, GMSEC_StatusClass clazz, GMSEC_StatusCode code, const char* reason, GMSEC_I32 custom)
	 *
	 * @brief This function will set the status values within the Status object.
	 *
	 * @param[in] status - the handle to the Status object
	 * @param[in] clazz  - the status class
	 * @param[in] code   - the status code
	 * @param[in] reason - the reason to include with the status (or NULL if none)
	 * @param[in] custom - the custom status code (if none, then set to 0)
	 *
	 * @return A handle to a status object.
	 *
	 * @sa destroyStatus()
	 */
	GMSEC_API void statusSet(GMSEC_Status status, GMSEC_StatusClass clazz, GMSEC_StatusCode code, const char* reason, GMSEC_I32 custom);


	/**
	 * @fn GMSEC_StatusClass statusGetClass(const GMSEC_Status status)
	 *
	 * @brief Get status class code of this Status object.
	 *
	 * @param[in] status - status handle
	 *
	 * @return The status class.
	 *
	 * @sa errors.h
	 */
	GMSEC_API GMSEC_StatusClass statusGetClass(const GMSEC_Status status);


	/**
	 * @fn void statusSetClass(GMSEC_Status status, GMSEC_StatusClass clazz)
	 *
	 * @brief Sets the status class code of this Status object.
	 *
	 * @param[in] status - status handle
	 * @param[in] clazz  - the status class to assign to the Status
	 *
	 * @return The status class.
	 *
	 * @sa errors.h
	 */
	GMSEC_API void statusSetClass(const GMSEC_Status status, GMSEC_StatusClass clazz);


	/**
	 * @fn GMSEC_StatusCode statusGetCode(const GMSEC_Status status)
	 *
	 * @brief Get the status code of this Status object.
	 *
	 * @param[in] status - status handle
	 *
	 * @return error code
	 *
	 * @sa errors.h
	 */
	GMSEC_API GMSEC_StatusCode statusGetCode(const GMSEC_Status status);


	/**
	 * @fn void statusSetCode(GMSEC_Status status, GMSEC_StatusCode code)
	 *
	 * @brief Sets the status code of this Status object.
	 *
	 * @param[in] status - status handle
	 * @param[in] code   - the status code to assign to the Status
	 *
	 * @sa errors.h
	 */
	GMSEC_API void statusSetCode(GMSEC_Status status, GMSEC_StatusCode code);


	/**
	 * @fn const char* statusGetReason(const GMSEC_Status status)
	 *
	 * @brief Returns the error string from the status.
	 *
	 * @param[in] status - status handle
	 *
	 * @return error string
	 */
	GMSEC_API const char* statusGetReason(const GMSEC_Status status);


	/**
	 * @fn void statusSetReason(GMSEC_Status status, const char* reason)
	 *
	 * @brief Sets the error string for the Status object.
	 *
	 * @param[in] status - status handle
	 * @param[in] reason - the error string
	 */
	GMSEC_API void statusSetReason(GMSEC_Status status, const char* reason);


	/**
	 * @fn GMSEC_I32 statusGetCustomCode(const GMSEC_Status status)
	 *
	 * @brief Returns the custom code associated with the Status object.
	 *
	 * @param[in] status - status handle
	 *
	 * @return The custom code associated with the Status object.
	 */
	GMSEC_API GMSEC_I32 statusGetCustomCode(const GMSEC_Status status);


	/**
	 * @fn void statusSetCustomCode(GMSEC_Status status, GMSEC_I32 custom)
	 *
	 * @brief Sets the custom code within the Status object.
	 *
	 * @param[in] status - status handle
	 * @param[in] custom - the custom code
	 *
	 * @return The custom code associated with the Status object.
	 */
	GMSEC_API void statusSetCustomCode(GMSEC_Status status, GMSEC_I32 custom);


	/**
	 * @fn void statusReset(GMSEC_Status status)
	 *
	 * @brief Resets the state of the Status object.
	 *
	 * @param[in] status - status handle
	 */
	GMSEC_API void statusReset(GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
