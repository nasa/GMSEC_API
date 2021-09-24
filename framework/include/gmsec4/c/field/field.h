/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file field.h
 *
 *  @brief This file contains common functions for the management of Field objects.
 *
 * Example creation and use:
 * @code
 * GMSEC_Status status = statusCreate();
 * GMSEC_Field  field  = booleanFieldCreate("BOOL-FIELD", GMSEC_TRUE, status);
 *
 * if (field)
 * {
 *     // For brevity, the status is not checked.  Note, an error status is reported only if
 *     // field handle is NULL.  Otherwise the operation is guaranteed to succeed.
 *
 *     printf("Field Name: %s\n", fieldGetName(field, status));
 *     printf("Field Type: %d\n", fieldGetType(field, status));
 *     printf("Field XML:\n%s\n", fieldToXML(field, status));
 *
 *     fieldDestroy(&field);
 * }
 * else
 * {
 *     //check status for error
 * }
 * @endcode
 */


#ifndef GMSEC_API_C_FIELD_H
#define GMSEC_API_C_FIELD_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


/**
 * @enum GMSEC_FieldType
 * @brief The various types of GMSEC Fields that are supported.
 */
typedef enum
{
	GMSEC_CHAR_TYPE   = 1,		/**< Character field type */
	GMSEC_BOOL_TYPE   = 2,		/**< Boolean field type */
	GMSEC_I16_TYPE    = 3,		/**< Signed 16-bit field type */
	GMSEC_U16_TYPE    = 4,		/**< Unsigned 16-bit field type */
	GMSEC_I32_TYPE    = 5,		/**< Signed 32-bit field type */
	GMSEC_U32_TYPE    = 6,		/**< Unsigned 32-bit field type */
	GMSEC_F32_TYPE    = 7,		/**< 32-bit floating point field type */
	GMSEC_F64_TYPE    = 8,		/**< 64-bit floating point field type */
	GMSEC_STRING_TYPE = 9,		/**< String field type */
	GMSEC_BIN_TYPE    = 10,		/**< Binary field type */
	GMSEC_I8_TYPE     = 20,		/**< Signed 8-bit field type */
	GMSEC_I64_TYPE    = 22,		/**< Signed 64-bit field type */
	GMSEC_U8_TYPE     = 21,		/**< Unsigned 8-bit field type */
	GMSEC_U64_TYPE    = 23,		/**< Unsigned 64-bit field type */
	GMSEC_BAD_TYPE
} GMSEC_FieldType;


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn const char* fieldGetName(const GMSEC_Field field, GMSEC_Status status)
	 *
	 * @brief This function will return the name of the given field object.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  field  - handle to the field.
	 * @param[out] status - operation result status
	 *
	 * @return The name of the given field object, or NULL if the field object is invalid.
	 */
	GMSEC_API const char* fieldGetName(const GMSEC_Field field, GMSEC_Status status);


	/**
	 * @fn void fieldSetName(GMSEC_Field field, const char* name, GMSEC_Status status)
	 *
	 * @brief Sets the field name to the given string.
	 *
	 * @param[in]  field  - handle to the field.
	 * @param[in]  name   - the new field name.
	 * @param[out] status - operation result status
	 */
	GMSEC_API void fieldSetName(GMSEC_Field field, const char* name, GMSEC_Status status);


	/**
	 * @fn GMSEC_FieldType fieldGetType(const GMSEC_Field field, GMSEC_Status status)
	 *
	 * @brief This function will return the type of the given field object.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  field  - handle to the field.
	 * @param[out] status - operation result status
	 *
	 * @return The type of the given field object, or -1 if the field object is invalid.
	 */
	GMSEC_API GMSEC_FieldType fieldGetType(const GMSEC_Field field, GMSEC_Status status);


	/**
	 * @fn const char* fieldToXML(const GMSEC_Field field, GMSEC_Status status)
	 *
	 * @brief This function will return the XML string representation of the given field object.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  field  - handle to the field.
	 * @param[out] status - operation result status
	 *
	 * @return An XML string, or NULL if the field object is invalid.
	 */
	GMSEC_API const char* fieldToXML(const GMSEC_Field field, GMSEC_Status status);


	/**
	 * @fn const char* fieldToJSON(const GMSEC_Field field, GMSEC_Status status)
	 *
	 * @brief This function will return the JSON string representation of the given field object.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  field  - handle to the field.
	 * @param[out] status - operation result status
	 *
	 * @return An JSON string, or NULL if the field object is invalid.
	 */
	GMSEC_API const char* fieldToJSON(const GMSEC_Field field, GMSEC_Status status);


	/**
	 * @fn GMSEC_I64 fieldGetIntegerValue(const GMSEC_Field field, GMSEC_Status status)
	 *
	 * @brief Attempts to convert the field value into a signed 64-bit integer number representation.
	 *
	 * @param[in]  field  - handle to the field.
	 * @param[out] status - operation result status
	 *
	 * @return Returns the field value as an integer.
	 */
    GMSEC_API GMSEC_I64 fieldGetIntegerValue(const GMSEC_Field field, GMSEC_Status status);


	/**
	 * @fn GMSEC_U64 fieldGetUnsignedIntegerValue(const GMSEC_Field field, GMSEC_Status status)
	 *
	 * @brief Attempts to convert the field value into an unsigned 64-bit integer number representation.
	 *
	 * @param[in]  field  - handle to the field.
	 * @param[out] status - operation result status
	 *
	 * @return Returns the field value as an unsigned integer.
	 */
    GMSEC_API GMSEC_U64 fieldGetUnsignedIntegerValue(const GMSEC_Field field, GMSEC_Status status);


	/**
	 * @fn GMSEC_F64 fieldGetDoubleValue(const GMSEC_Field field, GMSEC_Status status)
	 *
	 * @brief Attempts to convert the field value into a 64-bit floating point number representation.
	 *
	 * @param[in]  field  - handle to the field.
	 * @param[out] status - operation result status
	 *
	 * @return Returns the field value as a floating point number.
	 */
	GMSEC_API GMSEC_F64 fieldGetDoubleValue(const GMSEC_Field field, GMSEC_Status status);


	/**
	 * @fn const char* fieldGetStringValue(const GMSEC_Field field, GMSEC_Status status)
	 *
	 * @brief Attempts to convert the field value into string representation.
	 *
	 * @param[in]  field  - handle to the field.
	 * @param[out] status - operation result status
	 *
	 * @return The string value of the field, or NULL if the field object is invalid.
	 */
	GMSEC_API const char* fieldGetStringValue(const GMSEC_Field field, GMSEC_Status status);


	/**
	 * @fn GMSEC_Field fieldClone(const GMSEC_Field field, GMSEC_Status status)
	 *
	 * @brief Creates/returns a clone of the given field object.
	 *
	 * @param[in]  field  - handle to the field to clone.
	 * @param[out] status - operation result status
	 *
	 * @return A handle to the cloned field object.
	 *
	 * @sa fieldDestroy
	 */
	GMSEC_API GMSEC_Field fieldClone(const GMSEC_Field field, GMSEC_Status status);


	/**
	 * @fn void fieldDestroy(GMSEC_Field* field)
	 *
	 * @brief This function will destroy the field object.
	 *
	 * @param[in,out] field - handle of field to clean up
	 */
	GMSEC_API void fieldDestroy(GMSEC_Field* field);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
