/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file u32_field.h
 *
 *  @brief This file contains functions for the management of a U32Field object.
 *
 * Example creation and use:
 * @code
 * GMSEC_Status status = statusCreate();
 * GMSEC_Field  field  = u32FieldCreate("U32-FIELD", 12345, GMSEC_FALSE, status);
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


#ifndef GMSEC_API5_C_U32_FIELD_H
#define GMSEC_API5_C_U32_FIELD_H

#include <gmsec5_defs.h>

#include <gmsec5/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Field u32FieldCreate(const char* fieldName, GMSEC_U32 value, GMSEC_BOOL isHeader, GMSEC_Status status)
	 *
	 * @brief This function will create a U32Field object.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  fieldName - the name to assign to the field.
	 * @param[in]  value     - the value to assign to the field.
	 * @param[in]  isHeader  - used to indicate if Field is a header field.
	 * @param[out] status    - the handle to a status object in which status will be returned.
	 *
	 * @return A handle to a U32Field, or NULL if an error occurs.  If NULL is returned, refer to status object.
	 *
	 * @sa fieldDestroy()
	 */
	GMSEC_API GMSEC_Field u32FieldCreate(const char* fieldName, GMSEC_U32 value, GMSEC_BOOL isHeader, GMSEC_Status status);


	/**
	 * @fn GMSEC_U32 u32FieldGetValue(GMSEC_Field field, GMSEC_Status status)
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  field  - handle to field
	 * @param[out] status - the handle to a status object in which status will be returned.
	 *
	 * @return The data contained within the field.
	 */
	GMSEC_API GMSEC_U32 u32FieldGetValue(GMSEC_Field field, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
