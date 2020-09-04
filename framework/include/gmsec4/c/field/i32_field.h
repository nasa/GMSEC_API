/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file i32_field.h
 *
 *  @brief This file contains functions for the management of a I32Field object.
 *
 * Example creation and use:
 * @code
 * GMSEC_Status status = statusCreate();
 * GMSEC_Field  field  = i32FieldCreate("I32-FIELD", 1234, status);
 *
 * if (!field)
 * {
 *     // error; examine status for details
 * }
 *
 * ...
 *
 * fieldDestroy(field);
 * @endcode
 */


#ifndef GMSEC_API_C_I32_FIELD_H
#define GMSEC_API_C_I32_FIELD_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Field i32FieldCreate(const char* fieldName, GMSEC_I32 value, GMSEC_Status status)
	 *
	 * @brief This function will create a I32Field object.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  fieldName - the name to assign to the field.
	 * @param[in]  value     - the value to assign to the field.
	 * @param[out] status    - the handle to a status object in which status will be returned.
	 *
	 * @return A handle to a I32Field, or NULL if an error occurs.  If NULL is returned, refer to status object.
	 *
	 * @sa fieldDestroy()
	 */
	GMSEC_API GMSEC_Field i32FieldCreate(const char* fieldName, GMSEC_I32 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_I32 i32FieldGetValue(const GMSEC_Field field, GMSEC_Status status)
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  field  - handle to field
	 * @param[out] status - the handle to a status object in which status will be returned.
	 *
	 * @return The data contained within the field.
	 */
	GMSEC_API GMSEC_I32 i32FieldGetValue(const GMSEC_Field field, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
