/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file binary_field.h
 *
 * @brief This file contains functions for the management of a BinaryField object.
 *
 * Example creation and use:
 * @code
 * GMSEC_U8 data[128];
 *
 * for (size_t i = 0; i < 128; ++i)
 * {
 *     data[i] = (unsigned char) i % 256;
 * }
 *
 * GMSEC_Status status = statusCreate();
 * GMSEC_Field  field  = binaryFieldCreate("BIN-FIELD", data, sizeof(data), GMSEC_FALSE, status);
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

#ifndef GMSEC_API5_C_BINARY_FIELD_H
#define GMSEC_API5_C_BINARY_FIELD_H

#include <gmsec5_defs.h>

#include <gmsec5/util/wdllexp.h>

#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Field binaryFieldCreate(const char* fieldName, const GMSEC_U8* data, size_t dataLength, GMSEC_BOOL isHeader, GMSEC_Status status);
	 *
	 * @brief This function will create a BinaryField object.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  fieldName  - the name to assign to the field.
	 * @param[in]  data       - the data to assign to the field.
	 * @param[in]  dataLength - the length of the data.
	 * @param[in]  isHeader   - used to indicate if Field is a header field.
	 * @param[out] status     - the handle to a status object in which status will be returned.
	 *
	 * @return A handle to a BinaryField, or NULL if an error occurs.  If NULL is returned, refer to status object.
	 *
	 * @sa fieldDestroy()
	 */
	GMSEC_API GMSEC_Field binaryFieldCreate(const char* fieldName, const GMSEC_U8* data, size_t dataLength, GMSEC_BOOL isHeader, GMSEC_Status status);


	/**
	 * @fn GMSEC_U8 const* binaryFieldGetValue(GMSEC_Field field, GMSEC_Status status);
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  field  - handle to field.
	 * @param[out] status - the handle to a status object in which status will be returned.
	 *
	 * @return The data contained within the BinaryField.
	 */
	GMSEC_API GMSEC_U8 const* binaryFieldGetValue(GMSEC_Field field, GMSEC_Status status);


	/**
	 * @fn size_t binaryFieldGetLength(GMSEC_Field field, GMSEC_Status status);
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  field  - handle to field.
	 * @param[out] status - the handle to a status object in which status will be returned.
	 *
	 * @return The length of the data contained within the BinaryField.
	 */
	GMSEC_API size_t binaryFieldGetLength(GMSEC_Field field, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
