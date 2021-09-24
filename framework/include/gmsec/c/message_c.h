
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */








/** @file message_c.h
 *
 *  @brief This file contains function for managing messages.
 *
 * Example creation & use:
 * @code
 * GMSEC_MESSAGE_HANDLE msg = NULL;
 * GMSEC_FIELD_HANDLE fd = NULL;
 *
 * // create message with subject & kind
 * CreateMessage(conn,"gmsec.test.publish",GMSEC_MSG_PUBLISH,&msg,result);
 * if( isStatusError(result) )
 *	//handle error
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
 *
 * // do something (send message)
 *
 * DestroyMessage(conn, msg, result);
 * if( isStatusError(result) )
 *	//handle error
 * @endcode
 *
 *
**/

#ifndef gmsec_c_message_c_h
#define gmsec_c_message_c_h

#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec_defs.h>

#ifdef __cplusplus
extern "C"
{
#endif

	/** @fn isMsgValid(GMSEC_MESSAGE_HANDLE msg)
	 * @brief This will return 1 if the message is valid, 0 if not.
	 *
	 * @param[in] msg - message to test for validity
	 * @return GMSEC_BOOL - 1 or 0 for true or false
	 */
	GMSEC_DEPRECATED GMSEC_API GMSEC_BOOL CALL_TYPE isMsgValid(GMSEC_MESSAGE_HANDLE msg);

	/** @fn SetMsgKind(GMSEC_MESSAGE_HANDLE msg, GMSEC_MSG_KIND kind, GMSEC_STATUS_HANDLE status)
	 * @brief this will set the 'kind' for this message.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in,out] msg - on input message to set kind upon.  On output the new message
	 * @param[in]     kind - valid message kind
	 * @param[out]    status - operation result status
	 *
	 * @sa GetMsgKind()
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetMsgKind(GMSEC_MESSAGE_HANDLE msg, GMSEC_MSG_KIND kind, GMSEC_STATUS_HANDLE status);

	/** @fn GetMsgKind(GMSEC_MESSAGE_HANDLE msg, GMSEC_MSG_KIND *kind, GMSEC_STATUS_HANDLE status)
	 * @brief this will get the current 'kind' of this message
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  msg - message to get kind from
	 * @param[out]  kind - current message kind
	 * @param[out] status - operation result status
	 *
	 * @sa SetMsgKind()
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetMsgKind(GMSEC_MESSAGE_HANDLE msg, GMSEC_MSG_KIND *kind, GMSEC_STATUS_HANDLE status);

	/** @fn SetMsgSubject(GMSEC_MESSAGE_HANDLE msg, char* subject, GMSEC_STATUS_HANDLE status)
	 * @brief this will set the current message subject
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in,out] msg - on input message to set subject upon.  On out message with the new subject
	 * @param[in]     subject - message subject to set
	 * @param[out]    status -  operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE SetMsgSubject(GMSEC_MESSAGE_HANDLE msg, char* subject, GMSEC_STATUS_HANDLE status);

	/** @fn GetMsgSubject(GMSEC_MESSAGE_HANDLE msg, char* *subject, GMSEC_STATUS_HANDLE status)
	 * @brief this will get the current message subject
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  msg - message to get subject from
	 * @param[out] subject - current message subject
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE GetMsgSubject(GMSEC_MESSAGE_HANDLE msg, char* *subject, GMSEC_STATUS_HANDLE status);

	/** @fn MsgSetConfig(GMSEC_MESSAGE_HANDLE msg, GMSEC_CONFIG_HANDLE config, GMSEC_STATUS_HANDLE status)
	 * @brief This function sets the Configuration of this message.  This can be used to
	 * pass middleware specific items to the Message object.
	 *
	 * All messages implmentations understand the following config values:
	 *
	 * "subject"	-	sets the message subject
	 *
	 * "kind"		-	sets the message kind
	 *
	 * Configuration values from the passed in config will be copied or set within the message. Therefore
	 * the pased in config WILL NOT BE STORED and is the responsibility of the client program to clean up.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in,out] msg - on input message to set config upon.  On output, message with the new config
	 * @param[in]     config - configuration to set
	 * @param[out]    status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE MsgSetConfig(GMSEC_MESSAGE_HANDLE msg, GMSEC_CONFIG_HANDLE config, GMSEC_STATUS_HANDLE status);

	/** @fn MsgClearFields(GMSEC_MESSAGE_HANDLE msg,GMSEC_STATUS_HANDLE status)
	 * @brief This function will clear all fields from this message.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @param[in,out]  msg - on input message to operate upon.  On output a new empty message
	 * @param[out]     status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE MsgClearFields(GMSEC_MESSAGE_HANDLE msg,GMSEC_STATUS_HANDLE status);

	/** @fn MsgAddField(GMSEC_MESSAGE_HANDLE msg, GMSEC_FIELD_HANDLE field, GMSEC_STATUS_HANDLE status)
	 * @brief This function will add a new field to this message. A new field will be added to the message
	 * that matches the field passed in. Therefore the field is owned by the client program and can be reused.
	 * The client program must also clean up that field when no longer needed.
	 *
	 * Example:
	 * @code
	 * GMSEC_FIELD_HANDLE fd = NULL;
	 *
	 * // create field
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
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in,out]  msg - on input a handle to a message.  On output a message with a new field.
	 * @param[in]      field - field to add
	 * @param[out]     status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE MsgAddField(GMSEC_MESSAGE_HANDLE msg, GMSEC_FIELD_HANDLE field, GMSEC_STATUS_HANDLE status);

	/** @fn MsgClearField(GMSEC_MESSAGE_HANDLE msg, char* name, GMSEC_STATUS_HANDLE status)
	 * @brief This function will remove a field from this message by name.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @param[in,out] msg - on input a handle to a message.  On output a message with the specified field removed
	 * @param[in]     name - name of field to remove
	 * @param[out]    status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE MsgClearField(GMSEC_MESSAGE_HANDLE msg, char *name, GMSEC_STATUS_HANDLE status);

	/** @fn MsgGetField(GMSEC_MESSAGE_HANDLE msg, char* name, GMSEC_FIELD_HANDLE field, GMSEC_STATUS_HANDLE status)
	 * @brief This function will copy a field within a message into a field created by the client program, by name.
	 *
	 * Example:
	 * @code
	 * GMSEC_STRING value = NULL;
	 * GMSEC_FIELD_HANDLE fd = NULL;
	 *
	 * // create field
	 * CreateField(&fd,result);
	 * if( isStatusError(result) )
	 *	//handle error
	 *
	 * MsgGetField(msg,"char_field",fd,result);
	 * if( isStatusError(result) )
	 *	//handle error
	 *
	 * GetFieldValueSTRING(fd, &value, result)
	 * if( isStatusError(result) )
	 *	//handle error
	 *
	 * printf("value = %s\n",value);
	 * @endcode
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  msg - message handle
	 * @param[in]  name - name of field to get
	 * @param[out] field - field to copy
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE MsgGetField(GMSEC_MESSAGE_HANDLE msg, char* name, GMSEC_FIELD_HANDLE field, GMSEC_STATUS_HANDLE status);

	/** @fn MsgGetFieldCount(GMSEC_MESSAGE_HANDLE msg, GMSEC_I32 *count, GMSEC_STATUS_HANDLE status)
	 * @brief this will return the number of fields in the message.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  msg - message handle
	 * @param[out] count - number of fields
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE MsgGetFieldCount(GMSEC_MESSAGE_HANDLE msg, GMSEC_I32 *count, GMSEC_STATUS_HANDLE status);

	/** @fn MsgGetFirstField(GMSEC_MESSAGE_HANDLE msg, GMSEC_FIELD_HANDLE field, GMSEC_STATUS_HANDLE status)
	 * @brief This function and MsgGetNextField() are used to iterate through the fields of a message.
	 *
	 * Example:
	 * @code
	 * GMSEC_FIELD_HANDLE fd = NULL;
	 *
	 * // create field
	 * CreateField(&fd,result);
	 * if( isStatusError(result) )
	 *	//handle error
	 *
	 * // get first field
	 * MsgGetFirstField(msg,fd,result);
	 * while( !isStatusError(result) )
	 * {
	 *	// do something with the field here
	 *
	 *	// get next field
	 *	MsgGetNextField(msg,fd,result);
	 * }
	 * @endcode
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  msg - message handle
	 * @param[out] field - field to copy the current field into
	 * @param[out] status - operation result status
	 *
	 * @sa MsgGetNextField
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE MsgGetFirstField(GMSEC_MESSAGE_HANDLE msg, GMSEC_FIELD_HANDLE field, GMSEC_STATUS_HANDLE status);

	/** @fn MsgGetNextField(GMSEC_MESSAGE_HANDLE msg, GMSEC_FIELD_HANDLE field, GMSEC_STATUS_HANDLE status)
	 * @brief This function and MsgGetFirstField() are used to iterate through the fields of a message.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  msg - message handle
	 * @param[out] field - field to copy the current field into
	 * @param[out] status - operation result status
	 *
	 * @sa MsgGetFirstField
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE MsgGetNextField(GMSEC_MESSAGE_HANDLE msg, GMSEC_FIELD_HANDLE field, GMSEC_STATUS_HANDLE status);

	/** @fn MsgToXML( GMSEC_MESSAGE_HANDLE msg, char **outst, GMSEC_STATUS_HANDLE status )
	 * @brief This function will dump a message to an xml string. This format is compatible with
	 * the format used by MsgFromXML() and GetConfigFileMessage().
	 *
	 * XML Format example:
	 * @code
	 * <MESSAGE SUBJECT="GMSEC.MISSION.CONST.SAT.EVT.MSG" KIND="PUBLISH">
	 * 	<FIELD TYPE="CHAR" NAME="char_field">c</FIELD>
	 * 	<FIELD TYPE="BOOL" NAME="bool_field">TRUE</FIELD>
	 * 	<FIELD TYPE="SHORT" NAME="short_field">123</FIELD>
	 * 	<FIELD TYPE="USHORT" NAME="ushort_field">123</FIELD>
	 * 	<FIELD TYPE="LONG" NAME="long_field">123</FIELD>
	 * 	<FIELD TYPE="ULONG" NAME="ulong_field">123</FIELD>
	 * 	<FIELD TYPE="STRING" NAME="string_field">This is a test</FIELD>
	 * 	<FIELD TYPE="FLOAT" NAME="float_field">123</FIELD>
	 * 	<FIELD TYPE="DOUBLE" NAME="double_field">123</FIELD>
	 * 	<FIELD TYPE="BIN" NAME="bin_field">4a4c4d4e4f5051</FIELD>
	 * </MESSAGE>
	 * @endcode
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  msg - message handle
	 * @param[out] outst - xml string
	 * @param[out] status - operation result status
	 *
	 * @sa GetConfigFileMessage()
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE MsgToXML(GMSEC_MESSAGE_HANDLE msg, char **outst, GMSEC_STATUS_HANDLE status);

	/** @fn MsgFromXML( GMSEC_MESSAGE_HANDLE msg, char *xml, GMSEC_STATUS_HANDLE status )
	 * @brief This function will set subject, kind, and fields into a message as specified by the passed in
	 * xml string. The format is compatible with what is used by MsgToXML() and GetConfigFileMessage();
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] msg - message handle
	 * @param[in]  xml - xml string to parse
	 * @param[out] status - operation result status
	 *
	 * @sa GetConfigFileMessage()
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE MsgFromXML(GMSEC_MESSAGE_HANDLE msg, char *xml, GMSEC_STATUS_HANDLE status);

	/** @fn MsgGetSize(  GMSEC_MESSAGE_HANDLE msg, GMSEC_U32 *size, GMSEC_STATUS_HANDLE status )
	 * @brief This function will return the physical storage size of the message in bytes.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in] msg - message handle
	 * @param[out] size - the size of the message in bytes
	 * @param[out] status - operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE MsgGetSize(GMSEC_MESSAGE_HANDLE msg, GMSEC_U32 *size, GMSEC_STATUS_HANDLE status);

	/** @fn MsgGetTime()
	 * @brief This function will return the current time in the GMSEC standard date/time format.
	 *
	 * Example format:
	 * @code
	 * 2004-314-18:56:57.350
	 * @endcode
	 *
	 * @return string
	 */
	GMSEC_DEPRECATED GMSEC_API const char * CALL_TYPE MsgGetTime();


#ifdef __cplusplus
}	// extern "C"
#endif

#endif	// gmsec_c_message_c_h
