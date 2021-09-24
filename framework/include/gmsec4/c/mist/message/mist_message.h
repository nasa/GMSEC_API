/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file mist_message.h
 *
 *  @brief This file contains functions for the management of generic MIST Messages.
 */


#ifndef GMSEC_API_C_MIST_MESSAGE_H
#define GMSEC_API_C_MIST_MESSAGE_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>
#include <gmsec4/util/Deprecated.h>

#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Message mistMessageCreate(const char* subject,
	 *                                     const char* schemaID,
	 *                                     const GMSEC_Specification spec,
	 *                                     GMSEC_Status status)
	 *
	 * @brief Creates a MIST %Message instance with a template determined by ID and spec.
	 *
	 * @param[in]  subject  - the subject string for the message.
	 * @param[in]  schemaID - the string used to identify the message schema in C2MS or other message specification
	 * (e.g. 2019.00.C2MS.MSG.HB or MSG.HB).
	 * @param[in]  spec     - A reference to the specification this message's schema will adhere to.
	 * @param[out] status   - the result of the operation.
	 *
	 * @return A handle to a Message object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa specificationCreate
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message mistMessageCreate(const char* subject,
	                                          const char* schemaID,
	                                          const GMSEC_Specification spec,
	                                          GMSEC_Status status);


	/**
	 * @fn GMSEC_Message mistMessageCreateWithConfig(const char* subject,
	 *                                               const char* schemaID,
	 *                                               const GMSEC_Config config,
	 *                                               const GMSEC_Specification spec,
	 *                                               GMSEC_Status status)
	 *
	 * @brief Creates a MIST %Message instance with a template determined by ID and spec, and associates it with the given configuration.
	 *
	 * @param[in]  subject  - the subject string for the message.
	 * @param[in]  schemaID - the string used to identify the message schema in C2MS or other message specification
	 * (e.g. 2019.00.C2MS.MSG.HB or MSG.HB).
	 * @param[in]  spec     - A reference to the specification this message's schema will adhere to.
	 * @param[in]  config   - A configuration to associate with the message.
	 * @param[out] status   - the result of the operation.
	 *
	 * @return A handle to a Message object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa configCreate
	 * @sa specificationCreate
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message mistMessageCreateWithConfig(const char* subject,
	                                                    const char* schemaID,
	                                                    const GMSEC_Config config,
	                                                    const GMSEC_Specification spec,
	                                                    GMSEC_Status status);


	/**
	 * @fn GMSEC_Message mistMessageCreateUsingData(const char* data, GMSEC_Status status)
	 *
	 * @brief Creates a MIST %Message instance using the given XML or JSON data representation of the message.
	 *
	 * @param[in]  data   - the XML or JSON string representation of a message.
	 * @param[out] status - the result of the operation.
	 *
	 * @return A handle to a Message object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa messageDestroy
	 *
	 * @note This method has been deprecated. Use mistMessageCreateUsingSpec() instead.
	 */
	GMSEC_DEPRECATED GMSEC_API GMSEC_Message mistMessageCreateUsingData(const char* data, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message mistMessageCreateUsingSpecAndData(const GMSEC_Specification spec, const char* data, GMSEC_Status status)
	 *
	 * @brief Creates a MIST %Message instance using the given XML or JSON data representation of the message, and ensures
	 * the message adheres to the provided specification.
	 *
	 * @param[in]  spec   - A reference to the specification this message's schema will adhere to.
	 * @param[in]  data   - the XML or JSON string representation of a message.
	 * @param[out] status - the result of the operation.
	 *
	 * @return A handle to a Message object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message mistMessageCreateUsingSpecAndData(const GMSEC_Specification spec, const char* data, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message mistMessageCreateCopy(const GMSEC_Message other, GMSEC_Status status)
	 *
	 * @brief Creates a copy of the given MIST %Message instance.
	 *
	 * @param[in]  other  - the MIST %Message to copy.
	 * @param[out] status - the result of the operation.
	 *
	 * @return A handle to a Message object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message mistMessageCreateCopy(const GMSEC_Message other, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message mistMessageCreateFromMessage(const GMSEC_Message msg, GMSEC_Config specConfig, GMSEC_Status status)
	 *
	 * @brief Special constructor that builds a %MistMessage using a simple %Message.
	 *
	 * @param[in]  msg        - the simple %Message to reference while building the %MistMessage.
	 * @param[in]  specConfig - optional (i.e. can be NULL) %Specification configuration to apply when constructing the %MistMessage.
	 * @param[out] status     - the result of the operation.
	 *
	 * @return A handle to a Message object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa messageCreate
	 * @sa messageDestroy
	 * @sa createConfig
	 */
	GMSEC_API GMSEC_Message mistMessageCreateFromMessage(const GMSEC_Message msg, const GMSEC_Config specConfig, GMSEC_Status status);


	/**
	 * @fn void mistMessageSetStandardFields(const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
	 *
	 * @brief Sets the internal list of fields which are to be automatically placed in all
	 * MistMessage objects that are created.  Internal copies of the Fields are made, and
	 * thus ownership of the fields that are provided are not retained by MistMessage.
	 *
	 * @param[in]  fields    - the array of fields to be copied to the internal set of fields
	 * @param[in]  numFields - the number of fields in the array
	 * @param[out] status    - the result of the operation
	 */
	GMSEC_API void mistMessageSetStandardFields(const GMSEC_Field fields[], size_t numFields, GMSEC_Status status);


	/**
	 * @fn void mistMessageClearStandardFields()
	 *
	 * @brief Destroys the lists of standard fields that are included with MistMessage objects.
	 */
	GMSEC_API void mistMessageClearStandardFields();


	/**
	 * @fn mistMessageGetSchemaID(const GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Returns a string that identifies the schema that the Message is based off of.
	 *
	 * @param[in]  msg    - the reference to the MIST %Message.
	 * @param[out] status - the result of the operation.
	 *
	 * @return The schema ID associated with the messsage, or NULL if an error occurs.  If the latter occurs, check the status.
	 */
	GMSEC_API const char* mistMessageGetSchemaID(const GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn mistMessageSetValue(const GMSEC_Message msg, const char* fieldName, const char* value, GMSEC_Status status)
	 *
	 * @brief Sets the value for a field indicated by user.  The value's type is automatically
	 * determined by the function depending on the message schema being used.  If the field 
	 * does not have a required type, a new StringField will be created with the specified name 
	 * and value.
	 *
	 * @param[in]  msg       - the reference to the MIST %Message.
	 * @param[in]  fieldName - the name of the field.
	 * @param[in]  value     - the value of the field.
	 * @param[out] status    - the result of the operation.
	 */
	GMSEC_API void mistMessageSetValue(const GMSEC_Message msg, const char* fieldName, const char* value, GMSEC_Status status);


	/**
	 * @fn mistMessageSetValueI64(const GMSEC_Message msg, const char* fieldName, GMSEC_I64 value, GMSEC_Status status)
	 *
	 * @brief Sets the value for a field indicated by user.  The value's type is automatically
	 * determined by the function depending on the message schema being used.  If the field 
	 * does not have a required type, a new I64Field will be created with the specified name 
	 * and value.
	 *
	 * @param[in]  msg       - the reference to the MIST %Message.
	 * @param[in]  fieldName - the name of the field.
	 * @param[in]  value     - the value of the field.
	 * @param[out] status    - the result of the operation.
	 */
	GMSEC_API void mistMessageSetValueI64(const GMSEC_Message msg, const char* fieldName, GMSEC_I64 value, GMSEC_Status status);


	/**
	 * @fn mistMessageSetValueF64(const GMSEC_Message msg, const char* fieldName, GMSEC_F64 value, GMSEC_Status status)
	 *
	 * @brief Sets the value for a field indicated by user.  The value's type is automatically
	 * determined by the function depending on the message schema being used.  If the field 
	 * does not have a required type, a new F64Field will be created with the specified name 
	 * and value.
	 *
	 * @param[in]  msg       - the reference to the MIST %Message.
	 * @param[in]  fieldName - the name of the field.
	 * @param[in]  value     - the value of the field.
	 * @param[out] status    - the result of the operation.
	 */
	GMSEC_API void mistMessageSetValueF64(const GMSEC_Message msg, const char* fieldName, GMSEC_F64 value, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
