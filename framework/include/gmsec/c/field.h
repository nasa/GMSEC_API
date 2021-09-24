
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */








/** @file field.h
 *
 *  @brief This file contains function for managing message fields.
 *
 * Example creation and use:
 * @code
 * GMSEC_FIELD_OBJECT fd = NULL;
 *
 * //create field
 * gmsec_CreateField(&fd,result);
 * if( GMSEC_STATUS_NO_ERROR != gmsec_GetStatusClass(result) )
 *	//handle error
 *
 * //set field name & value
 * gmsec_SetFieldName(fd,"char_field",result);
 * if( GMSEC_STATUS_NO_ERROR != gmsec_GetStatusClass(result) )
 *	//handle error
 * gmsec_SetFieldValueCHAR(fd,(GMSEC_CHAR)'c',result);
 * if( GMSEC_STATUS_NO_ERROR != gmsec_GetStatusClass(result) )
 *	//handle error
 *
 * //add to message (will copy)
 * gmsec_MsgAddField(msg,fd,result);
 * if( GMSEC_STATUS_NO_ERROR != gmsec_GetStatusClass(result) )
 *	//handle error
 * @endcode
 *
 *
**/

#ifndef gmsec_c_field_h
#define gmsec_c_field_h

#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec_defs.h>

#ifdef __cplusplus
extern "C"
{
#endif

	/** @fn gmsec_CreateField(GMSEC_FIELD_OBJECT *fld, GMSEC_STATUS_OBJECT status)
	 * @brief Create a field to be added to a message
	 *
	 * @note When a field is created with CreateField(), it is owned by the client program and
	 * therefore must be cleaned up. Also, when the field is added to a message, it is copied,
	 * and therefore it can be re-used.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - return created field
	 * @param[out] status - operation result status
	 *
	 * @sa gmsec_MsgAddField @n
	 *	   gmsec_MsgGetField @n
	 *	   gmsec_DestroyField
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_CreateField(GMSEC_FIELD_OBJECT *fld, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_DestroyField(GMSEC_FIELD_OBJECT *fld, GMSEC_STATUS_OBJECT status)
	 * @brief Destroy a field.
	 *
	 * @note When a field is created with CreateField(), it is owned by the client program and
	 * therefore must be cleaned up. Also, when the field is added to a message, it is copied,
	 * and therefore it can be re-used.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field to destroy
	 * @param[out] status - operation result status
	 *
	 * @sa gmsec_CreateField @n
	 *     gmsec_MsgAddField @n
	 *	   gmsec_MsgGetField
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_DestroyField(GMSEC_FIELD_OBJECT *fld, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldName(GMSEC_FIELD_OBJECT fld, const char **name, GMSEC_STATUS_OBJECT status)
	 * @brief Get the name of a field.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] name - name of field
	 * @param[out] status - operation result status
	 *
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldName(GMSEC_FIELD_OBJECT fld, const char **name, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldName(GMSEC_FIELD_OBJECT fld, const char *name, GMSEC_STATUS_OBJECT status)
	 * @brief Set the name of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in,out] fld - on input handle to the Field.  On output Field with the specified name
	 * @param[in] name - name to set
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldName(GMSEC_FIELD_OBJECT fld, const char *name, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldType(GMSEC_FIELD_OBJECT fld, GMSEC_TYPE ftype, GMSEC_STATUS_OBJECT status)
	 * @brief Set the type of this field.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in,out]  fld - on input handle to the Field.  On output Field with the specified type
	 * @param[in]  ftype - identifier for type to set
	 * @param[out] status - operation result status
	 *
	 * @sa GMSEC_TYPE
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldType(GMSEC_FIELD_OBJECT fld, GMSEC_TYPE ftype, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldType(GMSEC_FIELD_OBJECT fld, GMSEC_TYPE *ftype, GMSEC_STATUS_OBJECT status)
	 * @brief Get the type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  fld - field handle
	 * @param[out] ftype - identifier for field type
	 * @param[out] status - operation result status
	 *
	 * @sa GMSEC_TYPE
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldType(GMSEC_FIELD_OBJECT fld, GMSEC_TYPE *ftype, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueCHAR(GMSEC_FIELD_OBJECT fld, GMSEC_CHAR *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_CHAR type. If the value can't be coerced into a GMSEC_CHAR
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  fld - field handle
	 * @param[out] value - value as GMSEC_CHAR
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueCHAR(GMSEC_FIELD_OBJECT fld, GMSEC_CHAR *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueBOOL(GMSEC_FIELD_OBJECT fld, GMSEC_BOOL *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_BOOL type. If the value can't be coerced into a GMSEC_BOOL
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_BOOL
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueBOOL(GMSEC_FIELD_OBJECT fld, GMSEC_BOOL *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueSHORT(GMSEC_FIELD_OBJECT fld, GMSEC_SHORT *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_SHORT type. If the value can't be coerced into a GMSEC_SHORT
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  fld - field handle
	 * @param[out] value - value as GMSEC_SHORT
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueSHORT(GMSEC_FIELD_OBJECT fld, GMSEC_SHORT *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueUSHORT(GMSEC_FIELD_OBJECT fld, GMSEC_USHORT *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_USHORT type. If the value can't be coerced into a GMSEC_USHORT
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 * THIS METHOD HAS BEEN DEPRECATED; use gmsec_GetFieldValueU16() instead.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_USHORT
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueUSHORT(GMSEC_FIELD_OBJECT fld, GMSEC_USHORT *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueLONG(GMSEC_FIELD_OBJECT fld, GMSEC_LONG *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_LONG type. If the value can't be coerced into a GMSEC_LONG
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_LONG
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueLONG(GMSEC_FIELD_OBJECT fld, GMSEC_LONG *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueULONG(GMSEC_FIELD_OBJECT fld, GMSEC_ULONG *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_ULONG type. If the value can't be coerced into a GMSEC_ULONG
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_ULONG
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueULONG(GMSEC_FIELD_OBJECT fld, GMSEC_ULONG *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueFLOAT(GMSEC_FIELD_OBJECT fld, GMSEC_FLOAT *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_FLOAT type. If the value can't be coerced into a GMSEC_FLOAT
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_FLOAT
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueFLOAT(GMSEC_FIELD_OBJECT fld, GMSEC_FLOAT *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueDOUBLE(GMSEC_FIELD_OBJECT fld, GMSEC_DOUBLE *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_DOUBLE type. If the value can't be coerced into a GMSEC_DOUBLE
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_DOUBLE
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueDOUBLE(GMSEC_FIELD_OBJECT fld, GMSEC_DOUBLE *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueSTRING(GMSEC_FIELD_OBJECT fld, GMSEC_STRING *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_STRING type. If the value can't be coerced into a GMSEC_STRING
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_STRING
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueSTRING(GMSEC_FIELD_OBJECT fld, GMSEC_STRING *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueSTR(GMSEC_FIELD_OBJECT fld, GMSEC_STR *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_STR type. If the value can't be coerced into a GMSEC_STR
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_STR
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueSTR(GMSEC_FIELD_OBJECT fld, GMSEC_STR *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueBIN(GMSEC_FIELD_OBJECT fld, GMSEC_BIN *value, GMSEC_U32 *size, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_BIN type. If the value can't be coerced into a GMSEC_BIN
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_BIN
	 * @param[out] size - length of array in "value"
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueBIN(GMSEC_FIELD_OBJECT fld, GMSEC_BIN *value, GMSEC_U32 *size, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueI16(GMSEC_FIELD_OBJECT fld, GMSEC_I16 *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_I16 type. If the value can't be coerced into a GMSEC_16
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  fld - field handle
	 * @param[out] value - value as GMSEC_I16
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueI16(GMSEC_FIELD_OBJECT fld, GMSEC_I16 *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueU16(GMSEC_FIELD_OBJECT fld, GMSEC_U16 *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_U16 type. If the value can't be coerced into a GMSEC_USHORT
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_U16
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueU16(GMSEC_FIELD_OBJECT fld, GMSEC_U16 *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueI32(GMSEC_FIELD_OBJECT fld, GMSEC_I32 *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_I32 type. If the value can't be coerced into a GMSEC_I32
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_I32
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueI32(GMSEC_FIELD_OBJECT fld, GMSEC_I32 *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueU32(GMSEC_FIELD_OBJECT fld, GMSEC_U32 *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_U32 type. If the value can't be coerced into a GMSEC_U32
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_U32
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueU32(GMSEC_FIELD_OBJECT fld, GMSEC_U32 *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueF32(GMSEC_FIELD_OBJECT fld, GMSEC_F32 *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_F32 type. If the value can't be coerced into a GMSEC_F32
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_F32
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueF32(GMSEC_FIELD_OBJECT fld, GMSEC_F32 *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueF64(GMSEC_FIELD_OBJECT fld, GMSEC_F64 *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_F64 type. If the value can't be coerced into a GMSEC_F64
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_F64
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueF64(GMSEC_FIELD_OBJECT fld, GMSEC_F64 *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueSTRING(GMSEC_FIELD_OBJECT fld, GMSEC_STRING *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_STRING type. If the value can't be coerced into a GMSEC_STRING
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_STRING
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueSTRING(GMSEC_FIELD_OBJECT fld, GMSEC_STRING *value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueBIN(GMSEC_FIELD_OBJECT fld, GMSEC_BIN *value, GMSEC_U32 *size, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_BIN type. If the value can't be coerced into a GMSEC_BIN
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_BIN
	 * @param[out] size - length of array in "value"
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueBIN(GMSEC_FIELD_OBJECT fld, GMSEC_BIN *value, GMSEC_U32 *size, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetFieldValueI64(GMSEC_FIELD_OBJECT fld, GMSEC_I64 *value, GMSEC_STATUS_OBJECT status)
	 * @brief Get the value of the field as a GMSEC_I64 type. If the value can't be coerced into a GMSEC_I64
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_I64
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_GetFieldValueI64(GMSEC_FIELD_OBJECT fld, GMSEC_I64 *value, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_SetFieldValueCHAR(GMSEC_FIELD_OBJECT fld, GMSEC_CHAR value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_CHAR
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueCHAR(GMSEC_FIELD_OBJECT fld, GMSEC_CHAR value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueBOOL(GMSEC_FIELD_OBJECT fld, GMSEC_BOOL value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_BOOL
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueBOOL(GMSEC_FIELD_OBJECT fld, GMSEC_BOOL value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueSHORT(GMSEC_FIELD_OBJECT fld, GMSEC_SHORT value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_SHORT
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueSHORT(GMSEC_FIELD_OBJECT fld, GMSEC_SHORT value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueUSHORT(GMSEC_FIELD_OBJECT fld, GMSEC_USHORT value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 * THIS METHOD HAS BEEN DEPRECATED; use gmsec_SetFieldValueU16() instead.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_USHORT
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueUSHORT(GMSEC_FIELD_OBJECT fld, GMSEC_USHORT value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueLONG(GMSEC_FIELD_OBJECT fld, GMSEC_LONG value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_LONG
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueLONG(GMSEC_FIELD_OBJECT fld, GMSEC_LONG value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueULONG(GMSEC_FIELD_OBJECT fld, GMSEC_ULONG value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_ULONG
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueULONG(GMSEC_FIELD_OBJECT fld, GMSEC_ULONG value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueFLOAT(GMSEC_FIELD_OBJECT fld, GMSEC_FLOAT value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_FLOAT
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueFLOAT(GMSEC_FIELD_OBJECT fld, GMSEC_FLOAT value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueDOUBLE(GMSEC_FIELD_OBJECT fld, GMSEC_DOUBLE value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_DOUBLE
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueDOUBLE(GMSEC_FIELD_OBJECT fld, GMSEC_DOUBLE value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueSTRING(GMSEC_FIELD_OBJECT fld, GMSEC_STR value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_STR
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueSTRING(GMSEC_FIELD_OBJECT fld, GMSEC_STR value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueSTR(GMSEC_FIELD_OBJECT fld, GMSEC_STR value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_STR
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueSTR(GMSEC_FIELD_OBJECT fld, GMSEC_STR value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueBIN(GMSEC_FIELD_OBJECT fld, GMSEC_BIN value, GMSEC_U32 size, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_BIN
	 * @param[in] size - size of array in "value"
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueBIN(GMSEC_FIELD_OBJECT fld, GMSEC_BIN value, GMSEC_U32 size, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueI16(GMSEC_FIELD_OBJECT fld, GMSEC_I16 value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_I16
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueI16(GMSEC_FIELD_OBJECT fld, GMSEC_I16 value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueU16(GMSEC_FIELD_OBJECT fld, GMSEC_U16 value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_U16
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueU16(GMSEC_FIELD_OBJECT fld, GMSEC_U16 value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueI32(GMSEC_FIELD_OBJECT fld, GMSEC_I32 value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_I32
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueI32(GMSEC_FIELD_OBJECT fld, GMSEC_I32 value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueU32(GMSEC_FIELD_OBJECT fld, GMSEC_U32 value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_U32
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueU32(GMSEC_FIELD_OBJECT fld, GMSEC_U32 value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueF32(GMSEC_FIELD_OBJECT fld, GMSEC_F32 value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_F32
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueF32(GMSEC_FIELD_OBJECT fld, GMSEC_F32 value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueF64(GMSEC_FIELD_OBJECT fld, GMSEC_F64 value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_F64
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueF64(GMSEC_FIELD_OBJECT fld, GMSEC_F64 value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_SetFieldValueI64(GMSEC_FIELD_OBJECT fld, GMSEC_I64 value, GMSEC_STATUS_OBJECT status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_I64
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_SetFieldValueI64(GMSEC_FIELD_OBJECT fld, GMSEC_I64 value, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_UnSetField(GMSEC_FIELD_OBJECT fld, GMSEC_STATUS_OBJECT status)
	 * @brief Reset the value and type of this field to UNSET
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in,out] fld - on input a handle to Field.  On out put a handle to Field with type equals to UNSET
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_UnSetField(GMSEC_FIELD_OBJECT fld, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_LookupFieldType(const char *ftype)
	 * @brief Lookup a standard field type identifier by string name. The valid strings
	 * correspond to the last part of the identifier names. ex. "BOOL" => GMSEC_TYPE_BOOL
	 *
	 * @param ftype - string that matches a known GMSEC field type
	 * @return GMSEC field type identifier, or GMSEC_UNSET
	 *
	 * @sa GMSEC_TYPE
	 */
	GMSEC_DEPRECATED GMSEC_API GMSEC_TYPE CALL_TYPE gmsec_LookupFieldType(const char * ftype);

#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* gmsec_c_field_h */


