
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */








/** @file field_c.h
 *
 *  @brief This file contains function for managing message fields.
 *
 * Example creation and use:
 * @code
 * GMSEC_FIELD_HANDLE fd = NULL;
 *
 * //create field
 * CreateField(&fd,result);
 * if( isStatusError(result) )
 *	//handle error
 *
 * //set field name & value
 * SetFieldName(fd,"char_field",result);
 * if( isStatusError(result) )
 *	//handle error
 * SetFieldValueCHAR(fd,(GMSEC_CHAR)'c',result);
 * if( isStatusError(result) )
 *	//handle error
 *
 * //add to message (will copy)
 * MsgAddField(msg,fd,result);
 * if( isStatusError(result) )
 *	//handle error
 * @endcode
 *
 *
**/

#ifndef gmsec_c_field_c_h
#define gmsec_c_field_c_h

#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec_defs.h>

#ifdef __cplusplus
extern "C"
{
#endif

	/** @fn CreateField(GMSEC_FIELD_HANDLE *fld, GMSEC_STATUS_HANDLE status)
	 * @brief Create a field to be added to a message
	 *
	 * @note When a field is created with CreateField(), it is owned by the client program and
	 * therefore must be cleaned up. Also, when the field is added to a message, it is copied,
	 * and therefore it can be re-used.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - return created field
	 * @param[out] status - operation result status
	 *
	 * @sa MsgAddField @n
	 *	   MsgGetField @n
	 *	   DestroyField
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE CreateField(GMSEC_FIELD_HANDLE *fld, GMSEC_STATUS_HANDLE status);

	/** @fn DestroyField(GMSEC_FIELD_HANDLE *fld, GMSEC_STATUS_HANDLE status)
	 * @brief Destroy a field.
	 *
	 * @note When a field is created with CreateField(), it is owned by the client program and
	 * therefore must be cleaned up. Also, when the field is added to a message, it is copied,
	 * and therefore it can be re-used.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field to destroy
	 * @param[out] status - operation result status
	 *
	 * @sa CreateField @n
	 *     MsgAddField @n
	 *	   MsgGetField
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE DestroyField(GMSEC_FIELD_HANDLE *fld, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldName(GMSEC_FIELD_HANDLE fld, char **name, GMSEC_STATUS_HANDLE status)
	 * @brief Get the name of a field.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] name - name of field
	 * @param[out] status - operation result status
	 *
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldName(GMSEC_FIELD_HANDLE fld, char **name, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldName(GMSEC_FIELD_HANDLE fld, char *name, GMSEC_STATUS_HANDLE status )
	 * @brief Set the name of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in,out] fld - on input handle to the Field.  On output Field with the specified name
	 * @param[in] name - name to set
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldName(GMSEC_FIELD_HANDLE fld, char *name, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldType(GMSEC_FIELD_HANDLE fld, GMSEC_TYPE ftype, GMSEC_STATUS_HANDLE status)
	 * @brief Set the type of this field.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in,out]  fld - on input handle to the Field.  On output Field with the specified type
	 * @param[in]  ftype - identifier for type to set
	 * @param[out] status - operation result status
	 *
	 * @sa GMSEC_TYPE
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldType(GMSEC_FIELD_HANDLE fld, GMSEC_TYPE ftype, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldType(GMSEC_FIELD_HANDLE fld, GMSEC_TYPE *ftype, GMSEC_STATUS_HANDLE status)
	 * @brief Get the type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  fld - field handle
	 * @param[out] ftype - identifier for field type
	 * @param[out] status - operation result status
	 *
	 * @sa GMSEC_TYPE
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldType(GMSEC_FIELD_HANDLE fld, GMSEC_TYPE *ftype, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueCHAR(GMSEC_FIELD_HANDLE fld, GMSEC_CHAR *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_CHAR type. If the value can't be coerced into a GMSEC_CHAR
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  fld - field handle
	 * @param[out] value - value as GMSEC_CHAR
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueCHAR(GMSEC_FIELD_HANDLE fld, GMSEC_CHAR *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueBOOL(GMSEC_FIELD_HANDLE fld, GMSEC_BOOL *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_BOOL type. If the value can't be coerced into a GMSEC_BOOL
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_BOOL
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueBOOL(GMSEC_FIELD_HANDLE fld, GMSEC_BOOL *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueSHORT(GMSEC_FIELD_HANDLE fld, GMSEC_SHORT *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_SHORT type. If the value can't be coerced into a GMSEC_SHORT
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  fld - field handle
	 * @param[out] value - value as GMSEC_SHORT
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueSHORT(GMSEC_FIELD_HANDLE fld, GMSEC_SHORT *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueUSHORT(GMSEC_FIELD_HANDLE fld, GMSEC_USHORT *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_USHORT type. If the value can't be coerced into a GMSEC_USHORT
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 * THIS METHOD HAS BEEN DEPRECATED; use GetFieldValueU16() instead.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_USHORT
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueUSHORT(GMSEC_FIELD_HANDLE fld, GMSEC_USHORT *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueLONG(GMSEC_FIELD_HANDLE fld, GMSEC_LONG *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_LONG type. If the value can't be coerced into a GMSEC_LONG
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_LONG
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueLONG(GMSEC_FIELD_HANDLE fld, GMSEC_LONG *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueULONG(GMSEC_FIELD_HANDLE fld, GMSEC_ULONG *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_ULONG type. If the value can't be coerced into a GMSEC_ULONG
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_ULONG
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueULONG(GMSEC_FIELD_HANDLE fld, GMSEC_ULONG *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueFLOAT(GMSEC_FIELD_HANDLE fld, GMSEC_FLOAT *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_FLOAT type. If the value can't be coerced into a GMSEC_FLOAT
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_FLOAT
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueFLOAT(GMSEC_FIELD_HANDLE fld, GMSEC_FLOAT *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueDOUBLE(GMSEC_FIELD_HANDLE fld, GMSEC_DOUBLE *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_DOUBLE type. If the value can't be coerced into a GMSEC_DOUBLE
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_DOUBLE
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueDOUBLE(GMSEC_FIELD_HANDLE fld, GMSEC_DOUBLE *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueSTRING(GMSEC_FIELD_HANDLE fld, GMSEC_STRING *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_STRING type. If the value can't be coerced into a GMSEC_STRING
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_STRING
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueSTRING(GMSEC_FIELD_HANDLE fld, GMSEC_STRING *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueSTR(GMSEC_FIELD_HANDLE fld, GMSEC_STR *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_STR type. If the value can't be coerced into a GMSEC_STR
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_STR
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueSTR(GMSEC_FIELD_HANDLE fld, GMSEC_STR *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueBIN(GMSEC_FIELD_HANDLE fld, GMSEC_BIN *value, GMSEC_U32 *size, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_BIN type. If the value can't be coerced into a GMSEC_BIN
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_BIN
	 * @param[out] size - length of array in "value"
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueBIN(GMSEC_FIELD_HANDLE fld, GMSEC_BIN *value, GMSEC_U32 *size, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueI16(GMSEC_FIELD_HANDLE fld, GMSEC_I16 *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_I16 type. If the value can't be coerced into a GMSEC_16
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  fld - field handle
	 * @param[out] value - value as GMSEC_I16
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueI16(GMSEC_FIELD_HANDLE fld, GMSEC_I16 *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueU16(GMSEC_FIELD_HANDLE fld, GMSEC_U16 *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_U16 type. If the value can't be coerced into a GMSEC_USHORT
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_U16
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueU16(GMSEC_FIELD_HANDLE fld, GMSEC_U16 *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueI32(GMSEC_FIELD_HANDLE fld, GMSEC_I32 *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_I32 type. If the value can't be coerced into a GMSEC_I32
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_I32
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueI32(GMSEC_FIELD_HANDLE fld, GMSEC_I32 *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueU32(GMSEC_FIELD_HANDLE fld, GMSEC_U32 *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_U32 type. If the value can't be coerced into a GMSEC_U32
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_U32
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueU32(GMSEC_FIELD_HANDLE fld, GMSEC_U32 *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueF32(GMSEC_FIELD_HANDLE fld, GMSEC_F32 *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_F32 type. If the value can't be coerced into a GMSEC_F32
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_F32
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueF32(GMSEC_FIELD_HANDLE fld, GMSEC_F32 *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueF64(GMSEC_FIELD_HANDLE fld, GMSEC_F64 *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_F64 type. If the value can't be coerced into a GMSEC_F64
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_F64
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueF64(GMSEC_FIELD_HANDLE fld, GMSEC_F64 *value, GMSEC_STATUS_HANDLE status);

	/** @fn GetFieldValueI64(GMSEC_FIELD_HANDLE fld, GMSEC_I64 *value, GMSEC_STATUS_HANDLE status)
	 * @brief Get the value of the field as a GMSEC_I64 type. If the value can't be coerced into a GMSEC_I64
	 * type, an error will be returned.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] fld - field handle
	 * @param[out] value - value as GMSEC_I64
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetFieldValueI64(GMSEC_FIELD_HANDLE fld, GMSEC_I64 *value, GMSEC_STATUS_HANDLE status);


	/** @fn SetFieldValueCHAR(GMSEC_FIELD_HANDLE fld, GMSEC_CHAR value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_CHAR
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueCHAR(GMSEC_FIELD_HANDLE fld, GMSEC_CHAR value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueBOOL(GMSEC_FIELD_HANDLE fld, GMSEC_BOOL value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_BOOL
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueBOOL(GMSEC_FIELD_HANDLE fld, GMSEC_BOOL value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueSHORT(GMSEC_FIELD_HANDLE fld, GMSEC_SHORT value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_SHORT
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueSHORT(GMSEC_FIELD_HANDLE fld, GMSEC_SHORT value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueUSHORT(GMSEC_FIELD_HANDLE fld, GMSEC_USHORT value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 * THIS METHOD HAS BEEN DEPRECATED; use SetFieldValueU16() instead.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_USHORT
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueUSHORT(GMSEC_FIELD_HANDLE fld, GMSEC_USHORT value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueLONG(GMSEC_FIELD_HANDLE fld, GMSEC_LONG value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_LONG
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueLONG(GMSEC_FIELD_HANDLE fld, GMSEC_LONG value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueULONG(GMSEC_FIELD_HANDLE fld, GMSEC_ULONG value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_ULONG
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueULONG(GMSEC_FIELD_HANDLE fld, GMSEC_ULONG value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueFLOAT(GMSEC_FIELD_HANDLE fld, GMSEC_FLOAT value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_FLOAT
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueFLOAT(GMSEC_FIELD_HANDLE fld, GMSEC_FLOAT value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueDOUBLE(GMSEC_FIELD_HANDLE fld, GMSEC_DOUBLE value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_DOUBLE
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueDOUBLE(GMSEC_FIELD_HANDLE fld, GMSEC_DOUBLE value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueSTRING(GMSEC_FIELD_HANDLE fld, GMSEC_STRING value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_STRING
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueSTRING(GMSEC_FIELD_HANDLE fld, GMSEC_STR value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueSTR(GMSEC_FIELD_HANDLE fld, GMSEC_STR value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_STR
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueSTR(GMSEC_FIELD_HANDLE fld, GMSEC_STR value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueBIN(GMSEC_FIELD_HANDLE fld, GMSEC_BIN value, GMSEC_U32 size, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_BIN
	 * @param[in] size - size of array in "value"
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueBIN(GMSEC_FIELD_HANDLE fld, GMSEC_BIN value, GMSEC_U32 size, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueI16(GMSEC_FIELD_HANDLE fld, GMSEC_I16 value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_I16
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueI16(GMSEC_FIELD_HANDLE fld, GMSEC_I16 value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueU16(GMSEC_FIELD_HANDLE fld, GMSEC_U16 value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_U16
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueU16(GMSEC_FIELD_HANDLE fld, GMSEC_U16 value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueI32(GMSEC_FIELD_HANDLE fld, GMSEC_I32 value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_I32
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueI32(GMSEC_FIELD_HANDLE fld, GMSEC_I32 value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueU32(GMSEC_FIELD_HANDLE fld, GMSEC_U32 value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_U32
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueU32(GMSEC_FIELD_HANDLE fld, GMSEC_U32 value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueF32(GMSEC_FIELD_HANDLE fld, GMSEC_F32 value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_F32
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueF32(GMSEC_FIELD_HANDLE fld, GMSEC_F32 value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueF64(GMSEC_FIELD_HANDLE fld, GMSEC_F64 value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_F64
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueF64(GMSEC_FIELD_HANDLE fld, GMSEC_F64 value, GMSEC_STATUS_HANDLE status);

	/** @fn SetFieldValueI64(GMSEC_FIELD_HANDLE fld, GMSEC_I64 value, GMSEC_STATUS_HANDLE status)
	 * @brief Set the value and type of this field
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] fld - field handle
	 * @param[in] value - value as GMSEC_I64
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetFieldValueI64(GMSEC_FIELD_HANDLE fld, GMSEC_I64 value, GMSEC_STATUS_HANDLE status);

	/** @fn UnSetField( GMSEC_FIELD_HANDLE fld, GMSEC_STATUS_HANDLE status )
	 * @brief Reset the value and type of this field to UNSET
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in,out] fld - on input a handle to Field.  On out put a handle to Field with type equals to UNSET
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE UnSetField(GMSEC_FIELD_HANDLE fld, GMSEC_STATUS_HANDLE status);

#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* gmsec_c_field_c_h */


