/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file config_file_iterator.h
 *
 * @brief This file contains functions for accessing elements of a configuration file (ConfigFile) object.
 */


#ifndef GMSEC_API_C_CONFIG_FILE_ITERATOR_H
#define GMSEC_API_C_CONFIG_FILE_ITERATOR_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_BOOL configFileIteratorHasNextConfig(const GMSEC_ConfigFileIterator iter, GMSEC_Status status)
	 *
	 * @brief Determines if there is a next Config in the iterator.
	 *
	 * @param[in]  iter   - the handle to the ConfigFileIterator object
	 * @param[out] status - the result of the operation
	 *
	 * @return Whether there is a next config element.
	 */
	GMSEC_API GMSEC_BOOL configFileIteratorHasNextConfig(const GMSEC_ConfigFileIterator iter, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL configFileIteratorHasNextMessage(const GMSEC_ConfigFileIterator iter, GMSEC_Status status)
	 *
	 * @brief Determines if there is a next Message in the iterator.
	 *
	 * @param[in]  iter   - the handle to the ConfigFileIterator object
	 * @param[out] status - the result of the operation
	 *
	 * @return Whether there is a next message element.
	 */
	GMSEC_API GMSEC_BOOL configFileIteratorHasNextMessage(const GMSEC_ConfigFileIterator iter, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL configFileIteratorHasNextSubscription(const GMSEC_ConfigFileIterator iter, GMSEC_Status status)
	 *
	 * @brief Determines if there is a next subscription in the iterator.
	 *
	 * @param[in]  iter   - the handle to the ConfigFileIterator object
	 * @param[out] status - the result of the operation
	 *
	 * @return Whether there is a next subscription element.
	 */
	GMSEC_API GMSEC_BOOL configFileIteratorHasNextSubscription(const GMSEC_ConfigFileIterator iter, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL configFileIteratorHasNextCustomElement(const GMSEC_ConfigFileIterator iter, GMSEC_Status status)
	 *
	 * @brief Determines if there is another custom XML element in the iterator.
	 *
	 * @param[in]  iter   - the handle to the ConfigFileIterator object
	 * @param[out] status - the result of the operation
	 *
	 * @return Whether there is a next custom element.
	 */
	GMSEC_API GMSEC_BOOL configFileIteratorHasNextCustomElement(const GMSEC_ConfigFileIterator iter, GMSEC_Status status);


	/**
	 * @fn GMSEC_ConfigEntry configFileIteratorNextConfig(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
	 *
	 * @brief Returns a copy of the next Config object.
	 *
	 * @note The user is responsible for destroying the Config object.
	 *
	 * @param[in]  iter   - the handle to the ConfigFileIterator object
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to a Config object, or NULL if no more config elements are available.
	 *
	 * @sa configDestroy()
	 */
	GMSEC_API GMSEC_ConfigEntry configFileIteratorNextConfig(GMSEC_ConfigFileIterator iter, GMSEC_Status status);


	/**
	 * @fn GMSEC_MessageEntry configFileIteratorNextMessage(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
	 *
	 * @brief Returns a copy of the next Message object.
	 *
	 * @note The user is responsible for destroying the Message object.
	 *
	 * @param[in]  iter   - the handle to the ConfigFileIterator object
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to a Message object, or NULL if no more message elements are available.
	 *
	 * @sa messageDestroy()
	 */
	GMSEC_API GMSEC_MessageEntry configFileIteratorNextMessage(GMSEC_ConfigFileIterator iter, GMSEC_Status status);


	/**
	 * @fn GMSEC_SubscriptionEntry configFileIteratorNextSubscription(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
	 *
	 * @brief Returns the next subscription.
	 *
	 * @param[in]  iter   - the handle to the ConfigFileIterator object
	 * @param[out] status - the result of the operation
	 *
	 * @return The next subscription subject/topic, or NULL if no more subscription elements are available.
	 */
	GMSEC_API GMSEC_SubscriptionEntry configFileIteratorNextSubscription(GMSEC_ConfigFileIterator iter, GMSEC_Status status);


	/**
	 * @fn const char* configFileIteratorNextCustomElement(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
	 *
	 * @brief Returns the next custom XML element.
	 *
	 * @param[in]  iter   - the handle to the ConfigFileIterator object
	 * @param[out] status - the result of the operation
	 *
	 * @return An XML string, or NULL if no more custom XML elements are available.
	 */
	GMSEC_API const char* configFileIteratorNextCustomElement(GMSEC_ConfigFileIterator iter, GMSEC_Status status);


	/**
	 * @fn void configFileIteratorReset(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
	 *
	 * @brief Resets all iterators to the beginning of the respective lists that are maintained by the ConfigFileIterator.
	 *
	 * @param[in]  iter   - the handle to the ConfigFileIterator object
	 * @param[out] status - the result of the operation; always succeeds if valid ConfigFileIterator handle is given.
	 */
	GMSEC_API void configFileIteratorReset(GMSEC_ConfigFileIterator iter, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL configFileIteratorHasNextSubscriptionEntry(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
	 *
	 * @brief Determines if there is a next subscription entry in the iterator
	 *
	 * @param[in]  iter   - the handle to the ConfigFileIterator object
	 * @param[out] status - the result of the operation; always succeeds if valid ConfigFileIterator handle is given.
	 *
	 * @return Whether or not there is a next subscription entry
	 */
	GMSEC_API GMSEC_BOOL CALL_TYPE configFileIteratorHasNextSubscriptionEntry(GMSEC_ConfigFileIterator iter, GMSEC_Status status);


	/**
	 * @fn GMSEC_SubscriptionEntry_Handle configFileIteratorNextSubscriptionEntry(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
	 *
	 * @brief Returns the handle to the next Subscription Entry object.
	 *
	 * @param[in]  iter   - the handle to the ConfigFileIterator object
	 * @param[out] status - the result of the operation; always succeeds if valid ConfigFileIterator handle is given.
	 *
	 * @return The next subscription subject/topic, or NULL if no more subscription elements are available.
	 */ 
	GMSEC_API GMSEC_SubscriptionEntry_Handle configFileIteratorNextSubscriptionEntry(GMSEC_ConfigFileIterator iter, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
