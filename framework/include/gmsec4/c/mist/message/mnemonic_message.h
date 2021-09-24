/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file mnemonic_message.h
 *
 * @brief This file contains the public interface to Mnemonic messages.
 */


#ifndef GMSEC_API_C_MIST_MNEMONIC_MESSAGE_H
#define GMSEC_API_C_MIST_MNEMONIC_MESSAGE_H

#include <gmsec4/c/mist/mist_defs.h>

#include <gmsec4/util/Deprecated.h>
#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>

#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Message mnemonicMessageCreate(const char* subject, unsigned int version, GMSEC_Status status)
	 *
	 * @note This function has been deprecated.  Use the function in gmsec4/c/message/mnemonic_message.h instead.
	 *
	 * @brief Creates a MnemonicMessage object, and returns a handle to such.
	 *
	 * @param[in]  subject - the message subject
	 * @param[in]  version - the version of the GMSEC Interface Specification Document (ISD) to associate with the message
	 * @param[out] status  - the result of the operation
	 *
	 * @return A handle to a MnemonicMessage object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa messageDestroy
	 * @sa C_GMSEC_ISD_2014_00
	 * @sa C_GMSEC_ISD_CURRENT
	 */
	GMSEC_API GMSEC_Message mnemonicMessageCreate(const char* subject, unsigned int version, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message mnemonicMessageCreateWithConfig(const char* subject, const GMSEC_Config config, unsigned int version, GMSEC_Status status)
	 *
	 * @note This function has been deprecated.  Use the function in gmsec4/c/message/mnemonic_message.h instead.
	 *
	 * @brief Creates a MnemonicMessage object, and returns a handle to such.
	 *
	 * @param[in]  subject - the message subject
	 * @param[in]  config  - a handle to a Config object to associate with the message
	 * @param[in]  version - the version of the GMSEC Interface Specification Document (ISD) to associate with the message
	 * @param[out] status  - the result of the operation
	 *
	 * @return A handle to a MnemonicMessage object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa messageDestroy
	 * @sa C_GMSEC_ISD_2014_00
	 * @sa C_GMSEC_ISD_CURRENT
	 */
	GMSEC_API GMSEC_Message mnemonicMessageCreateWithConfig(const char* subject, const GMSEC_Config config, unsigned int version, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message mnemonicMessageCreateWithSpec(const char* subject,
	 *                                                 const char* schemaID,
	 *                                                 GMSEC_Specification spec,
	 *                                                 GMSEC_Status status)
	 *
	 * @brief Creates a MnemonicMessage object, and returns a handle to such.
	 *
	 * @param[in]  subject  - the message subject
	 * @param[in]  schemaID - the string used to identify the message schema in C2MS or other message specification. The schema ID has the
	 * format of: major.minor.schemaLevel.messagekind.messagetype (e.g. 2019.00.C2MS.MSG.MVAL) You may also use the 
	 * shorthand notation of messageKind.messageType (e.g. MSG.MVAL)  
	 * @param[in]  spec     - the specification this message's schema will adhere to
	 * @param[out] status   - the result of the operation
	 *
	 * @return A handle to a MnemonicMessage object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa specificationCreate
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message mnemonicMessageCreateWithSpec(const char* subject,
	                                                      const char* schemaID,
	                                                      GMSEC_Specification spec,
	                                                      GMSEC_Status status);


	/**
	 * @fn GMSEC_Message mnemonicMessageCreateWithConfigAndSpec(const char* subject,
	 *                                                          const char* schemaID,
	 *                                                          GMSEC_Config config,
	 *                                                          GMSEC_Specification spec,
	 *                                                          GMSEC_Status status)
	 *
	 * @brief Creates a MnemonicMessage object, and returns a handle to such.
	 *
	 * @param[in]  subject  - the message subject
	 * @param[in]  schemaID - the string used to identify the message schema in C2MS or other message specification. The schema ID has the
	 * format of: major.minor.schemaLevel.messagekind.messagetype (e.g. 2019.00.C2MS.MSG.MVAL) You may also use the 
	 * shorthand notation of messageKind.messageType (e.g. MSG.MVAL)  
	 * @param[in]  config   - the configuration to associate with the message
	 * @param[in]  spec     - the specification this message's schema will adhere to
	 * @param[out] status   - the result of the operation
	 *
	 * @return A handle to a MnemonicMessage object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa configCreate
	 * @sa specificationCreate
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message mnemonicMessageCreateWithConfigAndSpec(const char* subject,
	                                                               const char* schemaID,
	                                                               GMSEC_Config config,
	                                                               GMSEC_Specification spec,
	                                                               GMSEC_Status status);


	/**
	 * @fn GMSEC_Message mnemonicMessageCreateUsingData(const char* data, GMSEC_Status status)
	 *
	 * @brief Creates a MnemonicMessage object, and returns a handle to such.
	 *
	 * @param[in]  data   - Initialize the MnemonicMessage from either XML or JSON data string
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to a MnemonicMessage object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa messageDestroy
	 */
	GMSEC_DEPRECATED GMSEC_API GMSEC_Message mnemonicMessageCreateUsingData(const char* data, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message mnemonicMessageCreateUsingSpecAndData(GMSEC_Specification spec, const char* data, GMSEC_Status status)
	 *
	 * @brief Creates a Mnemonic Message instance using the given XML or JSON data representation of the message, and ensures
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
	GMSEC_API GMSEC_Message mnemonicMessageCreateUsingSpecAndData(const GMSEC_Specification spec, const char* data, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message mnemonicMessageCreateCopy(const GMSEC_Message other, GMSEC_Status status)
	 *
	 * @brief Copies the given MnemonicMessage object, and returns a handle to such.
	 *
	 * @param[in]  other  - the handle to the MnemonicMessage object to copy
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to a MnemonicMessage object, or NULL if an error occurs.  If the latter occurs, check the status.
	 *
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message mnemonicMessageCreateCopy(const GMSEC_Message other, GMSEC_Status status);


	/**
	 * @fn void mnemonicMessageAddMnemonic(GMSEC_Message msg, const GMSEC_Mnemonic mnemonic, GMSEC_Status status)
	 *
	 * @brief Adds the given Mnemonic object to the MnemonicMessage.
	 *
	 * @param[in]  msg      - the handle to the MnemonicMessage object
	 * @param[in]  mnemonic - the handle to the Mnemonic object to add to the MnemonicMessage
	 * @param[out] status   - the result of the operation
	 */
	GMSEC_API void mnemonicMessageAddMnemonic(GMSEC_Message msg, const GMSEC_Mnemonic mnemonic, GMSEC_Status status);


	/**
	 * @fn size_t mnemonicMessageGetNumMnemonics(const GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Returns the number of Mnemonic object associated with the MnemonicMessage.
	 *
	 * @param[in]  msg    - the handle to the MnemonicMessage object
	 * @param[out] status - the result of the operation
	 *
	 * @return The number of Mnemonic objects associated with the MnemonicMessage.
	 */
	GMSEC_API size_t mnemonicMessageGetNumMnemonics(const GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Mnemonic mnemonicMessageGetMnemonic(const GMSEC_Message msg, size_t index, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the Mnemonic object associated with the MnemonicMessage that is referenced by the particular index.
	 *
	 * @param[in]  msg    - the handle to the MnemonicMessage object
	 * @param[in]  index  - the index of the Mnemonic
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to the Mnemonic object, or NULL if an error occurs.  For the latter, check the status.
	 *
	 * @sa mnemonicMessageGetNumMnemonics
	 */
	GMSEC_DEPRECATED GMSEC_API const GMSEC_Mnemonic mnemonicMessageGetMnemonic(const GMSEC_Message msg, size_t index, GMSEC_Status status);


	/**
	 * @fn GMSEC_MnemonicIterator mnemonicMessageGetIterator(GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @desc Function that allows the callee to get the Mnemonic Iterator associated with the %MnemonicMessage.
	 * This iterator will allow for applications to iterate over the Mnemonics stored within the
	 * %MnemonicMessage.
	 * The iterator is reset each time mnemonicMessageGetIterator() is called.  The iterator itself is
	 * destroyed when the %MnemonicMessage object is destroyed.
	 *
	 * @note Only one Mnemonic Iterator is associated with a %MnemonicMessage; multiple calls to
	 * mnemonicMessageGetIterator() will return a handle to the same Mnemonic Iterator.  Each call will reset
	 * the iterator.
	 *
	 * @param[in]  msg    - the handle to the MnemonicMessage
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A handle to a MnemonicIterator.
	 *
	 * @sa mnemonicMessageGetIterator()
	 * @sa mnemonicIteratorHasNext()
	 * @sa mnemonicIteratorNext()
	 */
	GMSEC_API GMSEC_MnemonicIterator mnemonicMessageGetIterator(GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message mnemonicMessageConvert(const GMSEC_Message msg)
	 *
	 * @brief Converts the given message (presumably a non-MnemonicMessage, into a MnemonicMessage.
	 *
	 * @param[in]  msg    - the handle to the Message to convert
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to a MnemonicMessage, or NULL if an error occurs.  For the latter, check the status.
	 *
	 * @sa messageDestroy()
	 */
	GMSEC_DEPRECATED GMSEC_API GMSEC_Message mnemonicMessageConvert(const GMSEC_Message msg, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
