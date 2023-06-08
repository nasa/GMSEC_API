/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file message.h
 *
 * @brief This file contains functions for managing messages.
 *
 * Example creation & use:
 * @code
 * // create message using Message Factory
 * //
 * GMSEC_Status         status  = createStatus();
 * GMSEC_MessageFactory factory = messageFactoryCreate(status);
 * GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "MSG.HB", status);
 *
 * if (statusHasError(status))
 *     // handle error
 *
 * // create field
 * //
 * GMSEC_U16Field field = u16FieldCreate("PUB-RATE", (GMSEC_U16) 30, status);
 *
 * if (statusHasError(status))
 *     // handle error
 *
 * // add field to message (field will be copied)
 * //
 * messageAddField(msg, field, status);
 *
 * if (statusHasError(status))
 *     // handle error
 *
 * // do something with message (i.e. publish using connection object)
 * 
 * fieldDestroy(&field)
 * messageDestroy(&msg);
 * messageFactoryDestroy(&factory);
 * statusDestroy(&status);
 * @endcode
 */

#ifndef GMSEC_API5_C_MESSAGE_H
#define GMSEC_API5_C_MESSAGE_H

#include <gmsec5_defs.h>

#include <gmsec5/c/specification.h>

#include <gmsec5/c/field/field.h>

#include <gmsec5/util/wdllexp.h>

#include <stddef.h>


/**
 * @enum GMSEC_MessageKind
 *
 * @brief The various kinds of GMSEC Messages that are supported.
 *
 * @note The message kind is auto-deduced using the MESSAGE-TYPE field;
 * if the field is not present in the message, or does not contain a
 * value of MSG, REQ, or RESP, then the message kind will default to
 * GMSEC_PUBLISH.
 */
typedef enum
{
	GMSEC_PUBLISH = 1,   /**< For messages that are to be published */
	GMSEC_REQUEST = 2,	 /**< For messages that are to be sent as requests */
	GMSEC_REPLY   = 3    /**< For messages that are to be sent as replies */
} GMSEC_MessageKind;


/**
 * @enum GMSEC_ResponseStatus
 * @brief Possible values for the a GMSEC Message's Response to a request.
 */
typedef enum
{
	GMSEC_ACKNOWLEDGEMENT = 1,        ///< Basic acknowledgement
	GMSEC_WORKING_KEEP_ALIVE = 2,     ///< Still working on operation
	GMSEC_SUCCESSFUL_COMPLETION = 3,  ///< Operation successfully completed
	GMSEC_FAILED_COMPLETION = 4,      ///< Operation failed to complete
	GMSEC_INVALID_REQUEST = 5,        ///< The request for an operation was invalid
	GMSEC_FINAL_MESSAGE = 6           ///< This message is the last in a set of responses
} GMSEC_ResponseStatus;


/**
 * @typedef GMSEC_MessageValidator
 *
 * @brief Typedef for GMSEC_MessageValidator function.
 *
 * @code
 * void CustomMessageValidator(const GMSEC_Message msg, GMSEC_Status status)
 * {
 *    // Perform validation check on message
 *
 *    if (error == GMSEC_TRUE)
 *        statusSet(status, clazz, code, reason, customCode);
 * }
 * @endcode
 *
 * @param[in]  msg    - the message to validate
 * @param[out] status - the status of the operation, should one need to be returned
 *
 * @sa messageRegisterMessageValidator
 * @sa messageFactoryRegisterMessageValidator
 */
typedef void GMSEC_MessageValidator(const GMSEC_Message msg, GMSEC_Status status);


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Message messageCreate()
	 *
	 * @brief Constructs an message object with no associated fields.
	 *
	 * @return A handle to a Message object.
	 *
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message messageCreate();


	/**
	 * @fn GMSEC_Message messageCreateCopy(GMSEC_Message otherMsg, GMSEC_Status status)
	 *
	 * @brief Creates a copy (clone) of a message object.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  otherMsg - handle to the Message object to copy
	 * @param[out] status   - the handle to a status object in which status will be returned.
	 *
	 * @return A handle to a Message, or NULL if an error occurs. If NULL is returned, refer to status object.
	 *
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message messageCreateCopy(GMSEC_Message otherMsg, GMSEC_Status status);


    /**
     * @fn void messageAcknowledge(GMSEC_Message msg, GMSEC_Status status)
     *
     * @brief Acknowledges the processing of the message that has been received from
	 * the GMSEC Bus.
     *
     * @note Some middleware can retain a delivered message in a queue until said time
     * the message has been acknowledged as being processed. Should the message not be
     * acknowledged, the middleware will deliver the message to another subscriber client.
     *
	 * @param[in]  msg    - the message being acknowledged as having been processed
	 * @param[out] status - out parameter operation result status
     */
    GMSEC_API void messageAcknowledge(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn void messageDestroy(GMSEC_Message* msg)
	 *
	 * @brief Destroys the Message object.
	 * This function should NOT be used to destroy a Message object received within a callback.
	 *
	 * @param[in,out] msg - handle to the Message to clean up.
	 */
	GMSEC_API void messageDestroy(GMSEC_Message* msg);


	/**
	 * @fn const char* messageGetSchemaID(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Returns a string that identifies the schema from which the Message is based.
	 *
	 * @param[in]  msg    - a handle to a Message object
	 * @param[out] status - the handle to a Status object in which status will be returned.
	 *
	 * @return The Message schema ID.
	 */
	GMSEC_API const char* messageGetSchemaID(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn unsigned int messageGetVersion(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Returns the specification version number (if known) that was used to create the message.
	 *
	 * @param[in]  msg    - a handle to a Message object
	 * @param[out] status - the handle to a Status object in which status will be returned.
	 *
	 * @return The Message specification version.
	 */
	GMSEC_API unsigned int messageGetVersion(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn GMSEC_SchemaLevel messageGetSchemaLevel(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Returns the specification schema level (if known) that was used to create the message.
	 *
	 * @param[in]  msg    - a handle to a Message object
	 * @param[out] status - the handle to a Status object in which status will be returned.
	 *
	 * @return The Message Schema Level.
	 */
	GMSEC_API GMSEC_SchemaLevel messageGetSchemaLevel(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn void messageIsCompliant(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief The contents of the message are compared to the template to ensure the
	 * message is in compliance with the message specification.
	 *
	 * @param[in]  msg    - a handle to a Message object
	 * @param[out] status - the handle to a Status object in which status will be returned.
	 */
	GMSEC_API void messageIsCompliant(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn void messageRegisterMessageValidator(GMSEC_Message msg, GMSEC_MessageValidator* validator, GMSEC_Status status)
	 *
	 * @brief Registers the given message validator that is used when the message is checked for compliance.
	 *
	 * @param[in]  msg       - a handle to a Message object
	 * @param[in]  validator - the address to a GMSEC_MessageValidator function.
	 * @param[out] status    - the handle to a Status object in which status will be returned.
	 */
	GMSEC_API void messageRegisterMessageValidator(GMSEC_Message msg, GMSEC_MessageValidator* validator, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageSetFieldValue(GMSEC_Message msg, const char* fieldName, const char* value, GMSEC_Status status);
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling messageAddField() or one of its variances instead.
	 *
	 * @param[in]  msg       - a handle to a Message object
	 * @param[in]  fieldName - name of the field to be modified/created
	 * @param[in]  value     - the value of the field
	 * @param[out] status    - the handle to a Status object in which status will be returned
	 *
	 * @returns Returns GMSEC_TRUE if the field was replaced; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageSetFieldValue(GMSEC_Message msg, const char* fieldName, const char* value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageSetFieldValueI16(GMSEC_Message msg, const char* fieldName, GMSEC_I16 value, GMSEC_Status status);
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling messageAddField() or one of its variances instead.
	 *
	 * @param[in]  msg       - a handle to a Message object
	 * @param[in]  fieldName - name of the field to be modified/created
	 * @param[in]  value     - the value of the field
	 * @param[out] status    - the handle to a Status object in which status will be returned
	 *
	 * @returns Returns GMSEC_TRUE if the field was replaced; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageSetFieldValueI16(GMSEC_Message msg, const char* fieldName, GMSEC_I16 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageSetFieldValueU16(GMSEC_Message msg, const char* fieldName, GMSEC_U16 value, GMSEC_Status status);
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling messageAddField() or one of its variances instead.
	 *
	 * @param[in]  msg       - a handle to a Message object
	 * @param[in]  fieldName - name of the field to be modified/created
	 * @param[in]  value     - the value of the field
	 * @param[out] status    - the handle to a Status object in which status will be returned
	 *
	 * @returns Returns GMSEC_TRUE if the field was replaced; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageSetFieldValueU16(GMSEC_Message msg, const char* fieldName, GMSEC_U16 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageSetFieldValueI32(GMSEC_Message msg, const char* fieldName, GMSEC_I32 value, GMSEC_Status status);
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling messageAddField() or one of its variances instead.
	 *
	 * @param[in]  msg       - a handle to a Message object
	 * @param[in]  fieldName - name of the field to be modified/created
	 * @param[in]  value     - the value of the field
	 * @param[out] status    - the handle to a Status object in which status will be returned
	 *
	 * @returns Returns GMSEC_TRUE if the field was replaced; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageSetFieldValueI32(GMSEC_Message msg, const char* fieldName, GMSEC_I32 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageSetFieldValueU32(GMSEC_Message msg, const char* fieldName, GMSEC_U32 value, GMSEC_Status status);
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling messageAddField() or one of its variances instead.
	 *
	 * @param[in]  msg       - a handle to a Message object
	 * @param[in]  fieldName - name of the field to be modified/created
	 * @param[in]  value     - the value of the field
	 * @param[out] status    - the handle to a Status object in which status will be returned
	 *
	 * @returns Returns GMSEC_TRUE if the field was replaced; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageSetFieldValueU32(GMSEC_Message msg, const char* fieldName, GMSEC_U32 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageSetFieldValueI64(GMSEC_Message msg, const char* fieldName, GMSEC_I64 value, GMSEC_Status status);
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling messageAddField() or one of its variances instead.
	 *
	 * @param[in]  msg       - a handle to a Message object
	 * @param[in]  fieldName - name of the field to be modified/created
	 * @param[in]  value     - the value of the field
	 * @param[out] status    - the handle to a Status object in which status will be returned
	 *
	 * @returns Returns GMSEC_TRUE if the field was replaced; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageSetFieldValueI64(GMSEC_Message msg, const char* fieldName, GMSEC_I64 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageSetFieldValueU64(GMSEC_Message msg, const char* fieldName, GMSEC_U64 value, GMSEC_Status status);
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling messageAddField() or one of its variances instead.
	 *
	 * @param[in]  msg       - a handle to a Message object
	 * @param[in]  fieldName - name of the field to be modified/created
	 * @param[in]  value     - the value of the field
	 * @param[out] status    - the handle to a Status object in which status will be returned
	 *
	 * @returns Returns GMSEC_TRUE if the field was replaced; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageSetFieldValueU64(GMSEC_Message msg, const char* fieldName, GMSEC_U64 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageSetFieldValueF64(GMSEC_Message msg, const char* fieldName, GMSEC_F64 value, GMSEC_Status status);
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling messageAddField() or one of its variances instead.
	 *
	 * @param[in]  msg       - a handle to a Message object
	 * @param[in]  fieldName - name of the field to be modified/created
	 * @param[in]  value     - the value of the field
	 * @param[out] status    - the handle to a Status object in which status will be returned
	 *
	 * @returns Returns GMSEC_TRUE if the field was replaced; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageSetFieldValueF64(GMSEC_Message msg, const char* fieldName, GMSEC_F64 value, GMSEC_Status status);


	/**
	 * @fn void messageSetConfig(GMSEC_Message msg, GMSEC_Config cfg, GMSEC_Status status)
	 *
	 * @brief This function sets configuration items to the message. This can be used to pass middleware specific items to the Message object.
	 *
	 * @note Only Message-related configuration options are held by the Message object.
	 * These configuration options are NOT included with the message when it is sent
	 * across the GMSEC Bus.
	 *
	 * @param[in]  msg    - a handle to a Message object
	 * @param[in]  cfg    - a handle to a Config object
	 * @param[out] status - the handle to a Status object in which status will be returned.
	 */
	GMSEC_API void messageSetConfig(GMSEC_Message msg, GMSEC_Config cfg, GMSEC_Status status);


	/**
	 * @fn GMSEC_Config messageGetConfig(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Returns the configuration associcated with the message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @note Do NOT destroy the handle to the Config object.
	 *
	 * @param[in]  msg    - a handle to a Message object
	 * @param[out] status - the handle to a Status object in which status will be returned.
	 *
	 * @return A handle to the Config object associated with the Message.
	 */
	GMSEC_API GMSEC_Config messageGetConfig(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn void messageSetSubject(GMSEC_Message msg, const char* subject, GMSEC_Status status)
	 *
	 * @brief Allows for the setting/changing of the message subject.
	 *
	 * @param[in]  msg     - handle to the Message
	 * @param[in]  subject - the subject/topic to associate with the message
	 * @param[out] status  - status of the operation
	 */
	GMSEC_API void messageSetSubject(GMSEC_Message msg, const char* subject, GMSEC_Status status);


	/**
	 * @fn const char* messageGetSubject(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Returns the subject/topic associated with the message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - handle to the Message
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return The message subject/topic.
	 */
	GMSEC_API const char* messageGetSubject(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn void messageSetKind(GMSEC_Message msg, GMSEC_MessageKind kind, GMSEC_Status status)
	 *
	 * @brief This function can be used to set the message kind.
	 *
	 * @note The API can auto-deduce the message kind, but there may be cases where
	 * this function may be useful. For most common Use Cases, this function should
	 * NOT need to be called.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - handle to the message
	 * @param[in]  kind   - the message kind
	 * @param[out] status - status of the operation, which will always be nominal if a valid message handle is provided.
	 */
	GMSEC_API void messageSetKind(GMSEC_Message msg, GMSEC_MessageKind kind, GMSEC_Status status);


	/**
	 * @fn GMSEC_MessageKind messageGetKind(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Returns the kind of message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - handle to the message
	 * @param[out] status - status of the operation, which will always be nominal if a valid message handle is provided.
	 *
	 * @return The message type (either GMSEC_MSG_PUBLISH, GMSEC_MSG_REQUEST, or GMSEC_MSG_REPLY)
	 */
	GMSEC_API GMSEC_MessageKind messageGetKind(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddField(GMSEC_Message msg, GMSEC_Field field, GMSEC_Status status)
	 *
	 * @brief This function will add a new field to the message. A new field will be added to the message that matches the
	 * field passed in. Therefore the field is owned by the client program and can be reused. The client program must also
	 * clean up that field when no longer needed.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @note Any field handle (e.g. GMSEC_CharField, GMSEC_I32Field, etc.) can be passed to this function.
	 *
	 * @code
	 * // note: error checking skipped for brevity
	 *
	 * GMSEC_Status   status = statusCreate();
	 * GMSEC_Message  msg    = messageCreate("GMSEC.FOO.BAR", GMSEC_MSG_PUBLISH, status);
	 * GMSEC_I32Field field  = i32FieldCreate("I32-FIELD", 1234, status);
	 *
	 * if (messageAddField(msg, field, status) == GMSEC_TRUE) {
	 *     // field was replaced
	 * }
	 * else {
	 *     // new field added
	 * }
	 * @endcode
	 *
	 * @param[in]  msg    - handle to the Message
	 * @param[in]  field  - the field to add to the message
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 */
	GMSEC_API GMSEC_BOOL messageAddField(GMSEC_Message msg, GMSEC_Field field, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddBinaryField(GMSEC_Message msg, const char* name, const GMSEC_U8* data, size_t dataLen, GMSEC_Status status)
	 *
	 * @brief Adds a BinaryField to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg     - the handle to the Message object.
	 * @param[in]  name    - the name of the field
	 * @param[in]  data    - the data blob to associate with the field
	 * @param[in]  dataLen - the length of the data blob
	 * @param[out] status  - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field is replacing one with the same name; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageAddBinaryField(GMSEC_Message msg, const char* name, const GMSEC_U8* data, size_t dataLen, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddBooleanField(GMSEC_Message msg, const char* name, GMSEC_BOOL value, GMSEC_Status status)
	 *
	 * @brief Adds a BooleanField to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field
	 * @param[in]  value  - the value to associate with the field
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field is replacing one with the same name; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageAddBooleanField(GMSEC_Message msg, const char* name, GMSEC_BOOL value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddCharField(GMSEC_Message msg, const char* name, GMSEC_CHAR value, GMSEC_Status status)
	 *
	 * @brief Adds a CharField to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field
	 * @param[in]  value  - the value to associate with the field
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field is replacing one with the same name; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageAddCharField(GMSEC_Message msg, const char* name, GMSEC_CHAR value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddF32Field(GMSEC_Message msg, const char* name, GMSEC_F32 value, GMSEC_Status status)
	 *
	 * @brief Adds a F32Field to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field
	 * @param[in]  value  - the value to associate with the field
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field is replacing one with the same name; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageAddF32Field(GMSEC_Message msg, const char* name, GMSEC_F32 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddF64Field(GMSEC_Message msg, const char* name, GMSEC_F64 value, GMSEC_Status status)
	 *
	 * @brief Adds a F64Field to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field
	 * @param[in]  value  - the value to associate with the field
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field is replacing one with the same name; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageAddF64Field(GMSEC_Message msg, const char* name, GMSEC_F64 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddI8Field(GMSEC_Message msg, const char* name, GMSEC_I8 value, GMSEC_Status status)
	 *
	 * @brief Adds a I8Field to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field
	 * @param[in]  value  - the value to associate with the field
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field is replacing one with the same name; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageAddI8Field(GMSEC_Message msg, const char* name, GMSEC_I8 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddI16Field(GMSEC_Message msg, const char* name, GMSEC_I16 value, GMSEC_Status status)
	 *
	 * @brief Adds a I16Field to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field
	 * @param[in]  value  - the value to associate with the field
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field is replacing one with the same name; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageAddI16Field(GMSEC_Message msg, const char* name, GMSEC_I16 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddI32Field(GMSEC_Message msg, const char* name, GMSEC_I32 value, GMSEC_Status status)
	 *
	 * @brief Adds a I32Field to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field
	 * @param[in]  value  - the value to associate with the field
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field is replacing one with the same name; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageAddI32Field(GMSEC_Message msg, const char* name, GMSEC_I32 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddI64Field(GMSEC_Message msg, const char* name, GMSEC_I64 value, GMSEC_Status status)
	 *
	 * @brief Adds a I64Field to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field
	 * @param[in]  value  - the value to associate with the field
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field is replacing one with the same name; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageAddI64Field(GMSEC_Message msg, const char* name, GMSEC_I64 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddStringField(GMSEC_Message msg, const char* name, const char* value, GMSEC_Status status)
	 *
	 * @brief Adds a StringField to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field
	 * @param[in]  value  - the value to associate with the field
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field is replacing one with the same name; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageAddStringField(GMSEC_Message msg, const char* name, const char* value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddU8Field(GMSEC_Message msg, const char* name, GMSEC_U8 value, GMSEC_Status status)
	 *
	 * @brief Adds a U8Field to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field
	 * @param[in]  value  - the value to associate with the field
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field is replacing one with the same name; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageAddU8Field(GMSEC_Message msg, const char* name, GMSEC_U8 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddU16Field(GMSEC_Message msg, const char* name, GMSEC_U16 value, GMSEC_Status status)
	 *
	 * @brief Adds a U16Field to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field
	 * @param[in]  value  - the value to associate with the field
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field is replacing one with the same name; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageAddU16Field(GMSEC_Message msg, const char* name, GMSEC_U16 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddU32Field(GMSEC_Message msg, const char* name, GMSEC_U32 value, GMSEC_Status status)
	 *
	 * @brief Adds a U32Field to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field
	 * @param[in]  value  - the value to associate with the field
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field is replacing one with the same name; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageAddU32Field(GMSEC_Message msg, const char* name, GMSEC_U32 value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageAddU64Field(GMSEC_Message msg, const char* name, GMSEC_U64 value, GMSEC_Status status)
	 *
	 * @brief Adds a U64Field to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field
	 * @param[in]  value  - the value to associate with the field
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field is replacing one with the same name; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageAddU64Field(GMSEC_Message msg, const char* name, GMSEC_U64 value, GMSEC_Status status);


    /**
     * @fn GMSEC_BOOL messageAddFields(GMSEC_Message msg, GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
     *
     * @brief Adds each Field in the given array to the Message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg       - the handle to the Message object
	 * @param[in]  fields    - the array of fields to add to the message
	 * @param[in]  numFields - the number of fields in the array
	 * @param[out] status    - the result of the operation
     *
     * @return Returns GMSEC_TRUE if any existing field in the Message has been replaced; GMSEC_FALSE otherwise.
     */
	GMSEC_API GMSEC_BOOL messageAddFields(GMSEC_Message msg, GMSEC_Field fields[], size_t numFields, GMSEC_Status status);


	/**
	 * @fn void messageClearFields(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Clears all fields associated with the message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 */
	GMSEC_API void messageClearFields(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageClearField(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief This function will remove a field that is identified by the given name from the message
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to remove.
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns GMSEC_TRUE if the field was removed; false otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageClearField(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageHasField(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Reports whether the field, identified by name, is present in the Message.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field from which to reference the value.
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return GMSEC_TRUE if the field is found; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL messageHasField(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetField(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief This function will return the named Field object contained within the Message object, if it exists. Otherwise a NULL pointer is returned. 
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided
	 * and the field is found.
	 *
	 * @return If found, a handle to the field; otherwise NULL.
	 */
	GMSEC_API const GMSEC_Field messageGetField(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn GMSEC_FieldType messageGetFieldType(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns the field type associated with the named field.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided
	 * and the field is found.
	 *
	 * @return A field type if the field is found in the message; check status to ensure results.
	 */
	GMSEC_API GMSEC_FieldType messageGetFieldType(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const char* messageGetStringValue(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
	 *
	 * @brief Attempts to convert the field value into string representation.
	 *
	 * @param[in]  msg       - the handle to the Message object.
	 * @param[in]  fieldName - the name of the field from which to reference the value.
	 * @param[out] status    - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns the field value as a string value.
	 */
	GMSEC_API const char* messageGetStringValue(GMSEC_Message msg, const char* fieldName, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL messageGetBooleanValue(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
	 *
	 * @brief Attempts to convert the field value into boolean representation.
	 *
	 * @note Any non-zero value will be interpreted as true, otherwise as false.
	 *
	 * @param[in]  msg       - the handle to the Message object
	 * @param[in]  fieldName - the name of the field from which to reference the value
	 * @param[out] status    - status of the operation
	 *
	 * @return Upon success, returns the field value as a boolean value.
	 */
	GMSEC_API GMSEC_BOOL messageGetBooleanValue(GMSEC_Message msg, const char* fieldName, GMSEC_Status status);


	/**
	 * @fn GMSEC_I16 messageGetI16Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
	 *
	 * @brief Attempts to convert the field value into a signed 16-bit integer number representation.
	 *
	 * @param[in]  msg       - the handle to the Message object.
	 * @param[in]  fieldName - the name of the field from which to reference the value.
	 * @param[out] status    - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns the field value as a GMSEC_I16.
	 */
	GMSEC_API GMSEC_I16 messageGetI16Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status);


	/**
	 * @fn GMSEC_I32 messageGetI32Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
	 *
	 * @brief Attempts to convert the field value into a signed 32-bit integer number representation.
	 *
	 * @param[in]  msg       - the handle to the Message object.
	 * @param[in]  fieldName - the name of the field from which to reference the value.
	 * @param[out] status    - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns the field value as a GMSEC_I32.
	 */
	GMSEC_API GMSEC_I32 messageGetI32Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status);


	/**
	 * @fn GMSEC_I64 messageGetI64Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
	 *
	 * @brief Attempts to convert the field value into a signed 64-bit integer number representation.
	 *
	 * @param[in]  msg       - the handle to the Message object.
	 * @param[in]  fieldName - the name of the field from which to reference the value.
	 * @param[out] status    - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns the field value as a GMSEC_I64.
	 */
	GMSEC_API GMSEC_I64 messageGetI64Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status);


	/**
	 * @fn GMSEC_U16 messageGetU16Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
	 *
	 * @brief Attempts to convert the field value into an unsigned 16-bit integer number representation.
	 *
	 * @param[in]  msg       - the handle to the Message object.
	 * @param[in]  fieldName - the name of the field from which to reference the value.
	 * @param[out] status    - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns the field value as a GMSEC_U16.
	 */
	GMSEC_API GMSEC_U16 messageGetU16Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status);


	/**
	 * @fn GMSEC_U32 messageGetU32Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
	 *
	 * @brief Attempts to convert the field value into an unsigned 32-bit integer number representation.
	 *
	 * @param[in]  msg       - the handle to the Message object.
	 * @param[in]  fieldName - the name of the field from which to reference the value.
	 * @param[out] status    - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns the field value as a GMSEC_U32.
	 */
	GMSEC_API GMSEC_U32 messageGetU32Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status);


	/**
	 * @fn GMSEC_U64 messageGetU64Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
	 *
	 * @brief Attempts to convert the field value into an unsigned 64-bit integer number representation.
	 *
	 * @param[in]  msg       - the handle to the Message object.
	 * @param[in]  fieldName - the name of the field from which to reference the value.
	 * @param[out] status    - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns the field value as a GMSEC_U64.
	 */
	GMSEC_API GMSEC_U64 messageGetU64Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status);


	/**
	 * @fn GMSEC_F64 messageGetF64Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status)
	 *
	 * @brief Attempts to convert the field value into an 64-bit floating-point number representation.
	 *
	 * @param[in]  msg       - the handle to the Message object.
	 * @param[in]  fieldName - the name of the field from which to reference the value.
	 * @param[out] status    - status of the operation, which will always be nominal if a valid Message handle is provided.
	 *
	 * @return Returns the field value as a floating-point value.
	 */
	GMSEC_API GMSEC_F64 messageGetF64Value(GMSEC_Message msg, const char* fieldName, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetBinaryField(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the BinaryField referenced by the given name.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation
	 *
	 * @return A handle to a BinaryField if the field can be referenced, NULL otherwise.  In case of the latter, refer to the status.
	 */
	GMSEC_API const GMSEC_Field messageGetBinaryField(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetBooleanField(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the BooleanField referenced by the given name.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation
	 *
	 * @return A handle to a BooleanField if the field can be referenced, NULL otherwise.  In case of the latter, refer to the status.
	 */
	GMSEC_API const GMSEC_Field messageGetBooleanField(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetCharField(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the CharField referenced by the given name.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation
	 *
	 * @return A handle to a CharField if the field can be referenced, NULL otherwise.  In case of the latter, refer to the status.
	 */
	GMSEC_API const GMSEC_Field messageGetCharField(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetF32Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the F32Field referenced by the given name.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation
	 *
	 * @return A handle to a F32Field if the field can be referenced, NULL otherwise.  In case of the latter, refer to the status.
	 */
	GMSEC_API const GMSEC_Field messageGetF32Field(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetF64Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the F64Field referenced by the given name.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation
	 *
	 * @return A handle to a F64Field if the field can be referenced, NULL otherwise.  In case of the latter, refer to the status.
	 */
	GMSEC_API const GMSEC_Field messageGetF64Field(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetI8Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the I8Field referenced by the given name.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation
	 *
	 * @return A handle to a I8Field if the field can be referenced, NULL otherwise.  In case of the latter, refer to the status.
	 */
	GMSEC_API const GMSEC_Field messageGetI8Field(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetI16Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the I16Field referenced by the given name.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation
	 *
	 * @return A handle to a I16Field if the field can be referenced, NULL otherwise.  In case of the latter, refer to the status.
	 */
	GMSEC_API const GMSEC_Field messageGetI16Field(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetI32Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the I32Field referenced by the given name.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation
	 *
	 * @return A handle to a I32Field if the field can be referenced, NULL otherwise.  In case of the latter, refer to the status.
	 */
	GMSEC_API const GMSEC_Field messageGetI32Field(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetI64Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the I64Field referenced by the given name.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation
	 *
	 * @return A handle to a I64Field if the field can be referenced, NULL otherwise.  In case of the latter, refer to the status.
	 */
	GMSEC_API const GMSEC_Field messageGetI64Field(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetU8Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the U8Field referenced by the given name.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation
	 *
	 * @return A handle to a U8Field if the field can be referenced, NULL otherwise.  In case of the latter, refer to the status.
	 */
	GMSEC_API const GMSEC_Field messageGetU8Field(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetU16Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the U16Field referenced by the given name.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation
	 *
	 * @return A handle to a U16Field if the field can be referenced, NULL otherwise.  In case of the latter, refer to the status.
	 */
	GMSEC_API const GMSEC_Field messageGetU16Field(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetU32Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the U32Field referenced by the given name.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation
	 *
	 * @return A handle to a U32Field if the field can be referenced, NULL otherwise.  In case of the latter, refer to the status.
	 */
	GMSEC_API const GMSEC_Field messageGetU32Field(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetU64Field(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the U64Field referenced by the given name.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation
	 *
	 * @return A handle to a U64Field if the field can be referenced, NULL otherwise.  In case of the latter, refer to the status.
	 */
	GMSEC_API const GMSEC_Field messageGetU64Field(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageGetStringField(GMSEC_Message msg, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the StringField referenced by the given name.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[in]  name   - the name of the field to reference.
	 * @param[out] status - status of the operation
	 *
	 * @return A handle to a StringField if the field can be referenced, NULL otherwise.  In case of the latter, refer to the status.
	 */
	GMSEC_API const GMSEC_Field messageGetStringField(GMSEC_Message msg, const char* name, GMSEC_Status status);


	/**
	 * @fn size_t messageGetFieldCount(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Returns the number of fields associated with the message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided
	 *
	 * @return The number of fields associated with the message.
	 */
	GMSEC_API size_t messageGetFieldCount(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn void messageCopyFields(GMSEC_Message msg, GMSEC_Message toMsg, GMSEC_Status status)
	 *
	 * @brief Copies the fields from one message to another message.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[out] toMsg  - the handle to the Message object to where the fields will be copied.
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided
	 */
	GMSEC_API void messageCopyFields(GMSEC_Message msg, GMSEC_Message toMsg, GMSEC_Status status);


	/**
	 * @fn const char* messageToXML(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief This function will dump a message to an XML formatted string.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided
	 *
	 * @return The XML represensation of the message.
	 *
	 * @sa messageFromData
	 */
	GMSEC_API const char* messageToXML(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn const char* messageToJSON(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief This function will dump a message to a JSON formatted string.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided
	 *
	 * @return The JSON represensation of the message.
	 *
	 * @sa messageFromData
	 */
	GMSEC_API const char* messageToJSON(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn size_t messageGetSize(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief This function will return the physical storage size of the message in bytes. Please note that additional
	 * tracking data fields are added to messages at the time of publication which will affect the size of the message. 
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided
	 *
	 * @return The size of the message (in bytes)
	 */
	GMSEC_API size_t messageGetSize(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn GMSEC_MessageFieldIterator messageGetFieldIterator(GMSEC_Message msg, GMSEC_Status status)
	 *
     * @brief Method that allows the callee to get the MessageFieldIterator associated with the %Message.
     * This iterator will allow for applications to iterate over the Field objects stored within the %Message.
     * The iterator is reset each time getFieldIterator() is called.  The iterator itself is destroyed when the
     * %Message object is destroyed.
     *
     * @note Only one MessageFieldIterator object is associated with a %Message object; multiple calls to
     * messageGetFieldIterator() will return a handle to the same MessageFieldIterator object.  Each call will reset the iterator.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  msg    - the handle to the Message object.
	 * @param[out] status - status of the operation, which will always be nominal if a valid Message handle is provided
     *
     * @return A handle to a MessageFieldIterator object.
	 *
	 * @sa messageFieldIteratorHasNext
	 * @sa messageFieldIteratorNext
	 * @sa messageFieldIteratorReset
	 */
	GMSEC_API GMSEC_MessageFieldIterator messageGetFieldIterator(GMSEC_Message msg, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
