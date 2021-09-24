/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file product_file_message.h
 *
 *  @brief This file contains functions for the management of a MIST Product File Message.
 */


#ifndef GMSEC_API_C_MIST_PRODUCT_FILE_MESSAGE_H
#define GMSEC_API_C_MIST_PRODUCT_FILE_MESSAGE_H

#include <gmsec4/c/mist/mist_defs.h>

#include <gmsec4/c/message.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/Deprecated.h>
#include <gmsec4/util/wdllexp.h>

#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Message productFileMessageCreate(const char* subject, GMSEC_ResponseStatus response, const char* type, const char* subtype, unsigned int version, GMSEC_Status status)
	 *
	 * @note This function has been deprecated.  Use the functions in gmsec4/c/message/product_file_message.h instead.
	 *
	 * @brief Creates a ProductFileMessage object, and returns a handle to such.
	 *
	 * @param[in]  subject  - the subject of the message
	 * @param[in]  response - the value of the RESPONSE-STATUS field
	 * @param[in]  type     - the product file type
	 * @param[in]  subtype  - the product file subtype
	 * @param[in]  version  - the version of the GMSEC Interface Specification (ISD) to associate with the message
	 * @param[out] status   - the result of the operation
	 *
	 * @return A handle to a ProductFileMessage object, or NULL if an error occurs.  In case of the latter, check the status.
	 *
	 * @sa productFileMessageCreateWithSpec(const char*, GMSEC_ResponseStatus, GMSEC_MessageKind, const char*, const char*, const GMSEC_Specification, GMSEC_Status);
	 * @sa messageDestroy
	 * @sa C_GMSEC_ISD_2014_00
	 * @sa C_GMSEC_ISD_CURRENT
	 */
	GMSEC_DEPRECATED
	GMSEC_API GMSEC_Message productFileMessageCreate(const char* subject, GMSEC_ResponseStatus response, const char* type, const char* subtype, unsigned int version, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message productFileMessageCreateWithConfig(const char* subject, const GMSEC_Config config, GMSEC_ResponseStatus response, const char* type, const char* subtype, unsigned int version, GMSEC_Status status)
	 *
	 * @note This function has been deprecated.  Use the functions in gmsec4/c/message/product_file_message.h instead.
	 *
	 * @brief Creates a ProductFileMessage object, and returns a handle to such.
	 *
	 * @param[in]  subject  - the subject of the message
	 * @param[in]  config   - the handle to the configuration object to apply when constructing the ProductFileMessage
	 * @param[in]  response - the value of the RESPONSE-STATUS field
	 * @param[in]  type     - the product file type
	 * @param[in]  subtype  - the product file subtype
	 * @param[in]  version  - the version of the GMSEC Interface Specification (ISD) to associate with the message
	 * @param[out] status   - the result of the operation
	 *
	 * @return A handle to a ProductFileMessage object, or NULL if an error occurs.  In case of the latter, check the status.
	 *
	 * @sa productFileMessageCreateWithConfigAndSpec(const char*, GMSEC_ResponseStatus, GMSEC_MessageKind, const char*, const char*, const GMSEC_Config, const GMSEC_Specification, GMSEC_Status);
	 * @sa messageDestroy
	 * @sa C_GMSEC_ISD_2014_00
	 * @sa C_GMSEC_ISD_CURRENT
	 */
	GMSEC_DEPRECATED
	GMSEC_API GMSEC_Message productFileMessageCreateWithConfig(const char* subject, const GMSEC_Config config, GMSEC_ResponseStatus response, const char* type, const char* subtype, unsigned int version, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message productFileMessageCreateWithSpec(const char* subject,
	 *                                                    GMSEC_ResponseStatus response,
	 *                                                    GMSEC_MessageKind msgKind,
	 *                                                    const char* type,
	 *                                                    const char* subtype,
	 *                                                    const GMSEC_Specification spec,
	 *                                                    GMSEC_Status status)
	 *
	 * @brief Creates a ProductFileMessage object, and returns a handle to such.
	 *
	 * @param[in]  subject  - the subject of the message
	 * @param[in]  response - the value of the RESPONSE-STATUS field
	 * @param[in]  msgKind  - the kind of message to instantiate
	 * @param[in]  type     - the product file type
	 * @param[in]  subtype  - the product file subtype
	 * @param[in]  spec     - the specification this message's schema will adhere to
	 * @param[out] status   - the result of the operation
	 *
	 * @return A handle to a ProductFileMessage object, or NULL if an error occurs.  In case of the latter, check the status.
	 *
	 * @sa specificationCreate
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message productFileMessageCreateWithSpec(const char* subject,
	                                                         GMSEC_ResponseStatus response,
	                                                         GMSEC_MessageKind msgKind,
	                                                         const char* type,
	                                                         const char* subtype,
	                                                         const GMSEC_Specification spec,
	                                                         GMSEC_Status status);



	/**
	 * @fn GMSEC_Message productFileMessageCreateWithConfigAndSpec(const char* subject,
	 *                                                             GMSEC_ResponseStatus response,
	 *                                                             GMSEC_MessageKind msgKind,
	 *                                                             const char* type,
	 *                                                             const char* subtype,
	 *                                                             const GMSEC_Config config,
	 *                                                             const GMSEC_Specification spec,
	 *                                                             GMSEC_Status status)
	 *
	 * @brief Creates a ProductFileMessage object, and returns a handle to such.
	 *
	 * @param[in]  subject  - the subject of the message
	 * @param[in]  response - the value of the RESPONSE-STATUS field
	 * @param[in]  msgKind  - the kind of message to instantiate
	 * @param[in]  type     - the product file type
	 * @param[in]  subtype  - the product file subtype
	 * @param[in]  config   - the configuration to associate with the message
	 * @param[in]  spec     - the specification this message's schema will adhere to
	 * @param[out] status   - the result of the operation
	 *
	 * @return A handle to a ProductFileMessage object, or NULL if an error occurs.  In case of the latter, check the status.
	 *
	 * @sa specificationCreate
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message productFileMessageCreateWithConfigAndSpec(const char* subject,
	                                                                  GMSEC_ResponseStatus response,
	                                                                  GMSEC_MessageKind msgKind,
	                                                                  const char* type,
	                                                                  const char* subtype,
	                                                                  const GMSEC_Config config,
	                                                                  const GMSEC_Specification spec,
	                                                                  GMSEC_Status status);


	/**
	 * @fn GMSEC_Message productFileMessageCreateUsingData(const char* data, GMSEC_Status status)
	 *
	 * @brief Initializes a ProductFileMessage using an XML or JSON string.
	 *
	 * @param[in]  data   - XML or JSON data
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to a ProductFileMessage object, or NULL if an error occurs.  In case of the latter, check the status.
	 */
	GMSEC_DEPRECATED GMSEC_API GMSEC_Message productFileMessageCreateUsingData(const char* data, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message productFileMessageCreateUsingSpecAndData(GMSEC_Specification spec, const char* data, GMSEC_Status status)
	 *
	 * @brief Creates a Product File Message instance using the given XML or JSON data representation of the message, and ensures
	 * the message adheres to the provided specification.
	 *
	 * @param[in]  spec   - A reference to the specification this message's schema will adhere to.
	 * @param[in]  data   - the XML or JSON string representation of a message.
	 * @param[out] status - the result of the operation.
	 *
	 * @return A handle to a Message object, or NULL if an error occurs. If the latter occurs, check the status.
	 *
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message productFileMessageCreateUsingSpecAndData(GMSEC_Specification spec, const char* data, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message productFileMessageCreateCopy(const GMSEC_Message other, GMSEC_Status status)
	 *
	 * @brief Createa deep-copy of the given ProductFileMessage object, and returns a handle to such.
	 *
	 * @param[in]  other  - the handle to the ProductFileMessage to copy
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to a ProductFileMessage object, or NULL if an error occurs.  In case of the latter, check the status.
	 *
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message productFileMessageCreateCopy(const GMSEC_Message other, GMSEC_Status status);


	/**
	 * @fn GMSEC_ResponseStatus productFileMessageGetResponseStatus(const GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Returns the Response Status associated with the ProductFileMessage.
	 *
	 * @param[in]  msg    - the handle to the ProductFileMessage
	 * @param[out] status - the result of the operation
	 *
	 * @return The Response Status of the ProductFileMessage.
	 */
	GMSEC_API GMSEC_ResponseStatus productFileMessageGetResponseStatus(const GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn const char* productFileMessageGetProductType(const GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Returns the product type associated with the ProductFileMessage.
	 *
	 * @param[in]  msg    - the handle to the ProductFileMessage
	 * @param[out] status - the result of the operation
	 *
	 * @return The product type, or NULL if an error occurs.
	 */
	GMSEC_API const char* productFileMessageGetProductType(const GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn const char* productFileMessageGetProductSubtype(const GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Returns the product subtype associated with the ProductFileMessage.
	 *
	 * @param[in]  msg    - the handle to the ProductFileMessage
	 * @param[out] status - the result of the operation
	 *
	 * @return The product subtype, or NULL if an error occurs.
	 */
	GMSEC_API const char* productFileMessageGetProductSubtype(const GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn void productFileMessageAddProductFile(GMSEC_Message msg, const ProductFile productFile, GMSEC_Status status)
	 *
	 * @brief Adds a copy of the given ProductFile to the ProductFileMessage.
	 *
	 * @param[in]  msg         - the handle to the ProductFileMessage
	 * @param[in]  productFile - the handle to the ProductFile object to add
	 * @param[out] status      - the result of the operation
	 */
	GMSEC_API void productFileMessageAddProductFile(GMSEC_Message msg, const GMSEC_ProductFile productFile, GMSEC_Status status);


	/**
	 * @fn size_t productFileMessageGetNumProductFiles(const GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Returns the number of ProductFile objects associated with the ProductFileMessage.
	 *
	 * @param[in]  msg    - the handle to the ProductFileMessage
	 * @param[out] status - the result of the operation
	 *
	 * @return The number of ProductFile objects associated with the ProductFileMessage.
	 */
	GMSEC_API size_t productFileMessageGetNumProductFiles(const GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn const GMSEC_ProductFile productFileMessageGetProductFile(const GMSEC_Message msg, size_t index, GMSEC_Status status)
	 *
	 * @brief Returns a handle to a ProductFile object associated with the ProductFileMessage.
	 *
	 * @param[in]  msg    - the handle to the ProductFileMessage
	 * @param[in]  index  - the index into the list of available ProductFile objects
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to a ProductFile object, or NULL if an error occurs.
	 *
	 * @sa productFileMessageGetNumProductFiles
	 */
	GMSEC_DEPRECATED GMSEC_API const GMSEC_ProductFile productFileMessageGetProductFile(const GMSEC_Message msg, size_t index, GMSEC_Status status);


	/**
	 * @fn GMSEC_ProductFileIterator productFileMessageGetIterator(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @desc Function that allows the callee to get the Product File Iterator associated with the
	 * %ProductFileMessage.
	 * This iterator will allow for applications to iterate over the Product Files stored within the
	 * %ProductFileMessage.
	 * The iterator is reset each time productFileMessageGetIterator() is called.  The iterator itself is
	 * destroyed when the %ProductFileMessage object is destroyed.
	 *
	 * @note Only one Product File Iterator is associated with a %ProductFileMessage; multiple calls to
	 * productFileMessageGetIterator() will return a handle to the same Product File Iterator.  Each call will
	 * reset the iterator.
	 *
	 * @param[in]  msg    - the handle to the ProductFileMessage
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A handle to a ProductFileIterator.
	 *
	 * @sa productFileMessageGetIterator()
	 * @sa productFileIteratorHasNext()
	 * @sa productFileIteratorNext()
	 */
	GMSEC_API GMSEC_ProductFileIterator productFileMessageGetIterator(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message productFileMessageConvert(const GMSEC_Message msg)
	 *
	 * @brief Converts the given message (presumably a non-ProductFileMessage, into a ProductFileMessage.
	 *
	 * @param[in]  msg    - the handle to the Message to convert
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to a ProductFileMessage, or NULL if an error occurs.  For the latter, check the status.
	 *
	 * @sa messageDestroy()
	 */
	GMSEC_DEPRECATED GMSEC_API GMSEC_Message productFileMessageConvert(const GMSEC_Message msg, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
