/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file message_factory.h
 *
 *  @brief This file contains functions for the management of a MessageFactory object.
 */


#ifndef GMSEC_API5_C_MESSAGE_FACTORY_H
#define GMSEC_API5_C_MESSAGE_FACTORY_H

#include <gmsec5/c/message.h>

#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>

#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_MessageFactory messageFactoryCreate(GMSEC_Status status)
	 *
	 * @brief Creates MessageFactory that produces messages based on the latest available
	 * message specification.
	 *
	 * @param[out] status - the result of the operation.
	 *
	 * @returns A handle to a MessageFactory object.
	 *
	 * @sa messageFactoryDestroy
	 */
	GMSEC_API GMSEC_MessageFactory messageFactoryCreate(GMSEC_Status status);


	/**
	 * @fn GMSEC_MessageFactory messageFactoryCreateUsingConfig(GMSEC_Config config, GMSEC_Status status)
	 *
	 * @brief Creates MessageFactory that produces messages based on the information contained
	 * within the supplied configuration object (e.g. gmsec-message-specification=201900).
	 *
	 * @param[in]  config - handle to a Config object.
	 * @param[out] status - the result of the operation.
	 *
	 * @returns A handle to a MessageFactory object.
	 *
	 * @sa messageFactoryDestroy
	 */
	GMSEC_API GMSEC_MessageFactory messageFactoryCreateUsingConfig(GMSEC_Config config, GMSEC_Status status);


	/**
	 * @fn void messageFactoryDestroy(GMSEC_MessageFactory* factory)
	 *
	 * @brief Destroys the given MessageFactory object.
	 *
	 * @param[in,out] factory - handle to the MessageFactory object to destroy.
	 */
	GMSEC_API void messageFactoryDestroy(GMSEC_MessageFactory* factory);


	/**
	 * @fn void messageFactorySetStandardFields(GMSEC_MessageFactory factory, GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
	 *
	 * @brief Allows for the setting of common (standard) fields that should be applied to all messages
	 * created by the MessageFactory.
	 *
	 * @param[in]  factory   - the handle to the MessageFactory
	 * @param[in]  fields    - the array of fields to be copied to the internal set of fields
	 * @param[in]  numFields - the number of fields in the array
	 * @param[out] status    - the result of the operation
	 */
	GMSEC_API void messageFactorySetStandardFields(GMSEC_MessageFactory factory, GMSEC_Field fields[], size_t numFields, GMSEC_Status status);


	/**
	 * @fn void messageFactoryClearStandardFields(GMSEC_MessageFactory factory, GMSEC_Status status)
	 *
	 * @brief Removes any common/standard fields that have previously been associated with the 
	 * MessageFactory.
	 *
	 * @param[in]  factory - the handle to the MessageFactory
	 * @param[out] status  - the result of the operation
	 */
	GMSEC_API void messageFactoryClearStandardFields(GMSEC_MessageFactory factory, GMSEC_Status status);


	/**
	 * @fn void messageFactorySetMessageConfig(GMSEC_MessageFactory factory, GMSEC_Config msgConfig, GMSEC_Status status)
	 *
	 * @brief Allows for the setting of a common message configuration that will be applied to all messages
	 * created by the MessageFactory.
	 *
	 * @param[in]  factory   - the handle to the MessageFactory
	 * @param[in]  msgConfig - the handle to the message Config object
	 * @param[out] status    - the result of the operation
	 */
	GMSEC_API void messageFactorySetMessageConfig(GMSEC_MessageFactory factory, GMSEC_Config msgConfig, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message messageFactoryCreateSimpleMessage(GMSEC_MessageFactory factory, GMSEC_Status status)
	 *
	 * @brief Creates and returns a Message object. The Message will include any user-supplied message attributes
	 * (such as a message configuration, standard fields, and custom message validator).
	 *
	 * @param[in]  factory - the handle to the MessageFactory
	 * @param[out] status  - the result of the operation
	 *
	 * @returns A handle to a Message object.
	 *
	 * @sa messageFactorySetMessageConfig
	 * @sa messageFactorySetStandardFields
	 * @sa messageFactoryRegisterMessageValidator
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message messageFactoryCreateSimpleMessage(GMSEC_MessageFactory factory, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message messageFactoryCreateMessage(GMSEC_MessageFactory factory, const char* schemaID, GMSEC_Status status)
	 *
	 * @brief Creates and returns a Message object that is populated with fields derived from the working message specification
	 * and schema ID. In addition, the Message object will include any user-supplied message attributes (such as a message
	 * configuration, standard fields, and a custom message validator).
	 *
	 * @param[in]  factory  - the handle to the MessageFactory
	 * @param[in]  schemaID - the string identifying which message to create (e.g. "HB")
	 * @param[out] status   - the result of the operation
	 *
	 * @returns A handle to a Message object.
	 *
	 * @sa messageFactorySetMessageConfig
	 * @sa messageFactorySetStandardFields
	 * @sa messageFactoryRegisterMessageValidator
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message messageFactoryCreateMessage(GMSEC_MessageFactory factory, const char* schemaID, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message messageFactoryFromData(GMSEC_MessageFactory factory, const char* data, DataType type, GMSEC_Status status)
	 *
	 * @brief Creates and returns a Message object based on the given XML or JSON data.
	 *
	 * @param[in]  factory - the handle to the MessageFactory
	 * @param[in]  data    - an XML or JSON data string that represents a GMSEC-style message
	 * @param[in]  type    - the type of data being provided
	 * @param[out] status  - the result of the operation
	 *
	 * @returns A handle to a Message object.
	 *
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message messageFactoryFromData(GMSEC_MessageFactory factory, const char* data, DataType type, GMSEC_Status status);


	/**
	 * @fn GMSEC_Specification messageFactoryGetSpecification(GMSEC_MessageFactory factory, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the Specification object associated with the MessageFactory.
	 *
	 * @param[in]  factory - the handle to the MessageFactory
	 * @param[out] status  - the result of the operation
	 *
	 * @returns A handle to a Specification object.
	 *
	 * @note Do *NOT* destroy the returned Specification object.
	 */
	GMSEC_API GMSEC_Specification messageFactoryGetSpecification(GMSEC_MessageFactory factory, GMSEC_Status status);


	/**
	 * @fn void messageFactoryRegisterMessageValidator(GMSEC_MessageFactory factory, GMSEC_MessageValidator* validator, GMSEC_Status status)
	 *
	 * @brief Registers the given message validator function with each message created using the MessageFactory.
	 *
	 * @param[in]  factory   - the handle to the MessageFactory
	 * @param[in]  validator - handle to custom message validation function
	 * @param[out] status    - the result of the operation
	 */
	GMSEC_API void messageFactoryRegisterMessageValidator(GMSEC_MessageFactory factory, GMSEC_MessageValidator* validator, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
