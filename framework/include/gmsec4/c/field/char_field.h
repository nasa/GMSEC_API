/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file char_field.h
 *
 *  @brief This file contains functions for the management of a CharField object.
 *
 * Example creation and use:
 * @code
 * GMSEC_Status status = statusCreate();
 * GMSEC_Field  field  = charFieldCreate("CHAR-FIELD", 'c', status);
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


#ifndef GMSEC_API_C_CHAR_FIELD_H
#define GMSEC_API_C_CHAR_FIELD_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Field charFieldCreate(const char* fieldName, GMSEC_CHAR value, GMSEC_Status status)
	 *
	 * @brief This function will create a CharField object.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  fieldName - the name to assign to the field.
	 * @param[in]  value     - the value to assign to the field.
	 * @param[out] status    - the handle to a status object in which status will be returned.
	 *
	 * @return A handle to a CharField, or NULL if an error occurs.  If NULL is returned, refer to status object.
	 *
	 * @sa fieldDestroy()
	 */
	GMSEC_API GMSEC_Field charFieldCreate(const char* fieldName, GMSEC_CHAR value, GMSEC_Status status);


	/**
	 * @fn GMSEC_CHAR charFieldGetValue(const GMSEC_Field field, GMSEC_Status status)
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  field  - handle to field
	 * @param[out] status - the handle to a status object in which status will be returned.
	 *
	 * @return The data contained within the field.
	 */
	GMSEC_API GMSEC_CHAR charFieldGetValue(const GMSEC_Field field, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
