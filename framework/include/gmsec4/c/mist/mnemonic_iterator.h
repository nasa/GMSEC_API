/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file mnemonic_iterator.h
 *
 *  @brief Defines the functions which support the iteration over the Mnemonic objects stored in a MnemonicMessage.
 */


#ifndef GMSEC_API_C_MNEMONIC_ITERATOR_H
#define GMSEC_API_C_MNEMONIC_ITERATOR_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_BOOL mnemonicIteratorHasNext(GMSEC_MnemonicIterator iter, GMSEC_Status status)
	 *
	 * @brief Provides information as to whether there are additional Mnemonic objects that can be referenced using mnemonicIteratorNext().
	 *
	 * @param[in]  iter   - the handle to the Mnemonic Iterator
	 * @param[out] status - out parameter operation result status
	 *
	 * @return GMSEC_TRUE if additional Devices are available, GMSEC_FALSE otherwise.
	 *
	 * @sa mnemonicMessageGetIterator()
	 */
	GMSEC_API GMSEC_BOOL mnemonicIteratorHasNext(GMSEC_MnemonicIterator iter, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Mnemonic mnemonicIteratorNext(GMSEC_MnemonicIterator iter, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the next available Mnemonic.
	 *
	 * @param[in]  iter   - the handle to the Mnemonic Iterator
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A handle to a Mnemonic, or NULL if one is not available.  In case of the latter, check the status.
	 *
	 * @sa mnemonicMessageGetIterator()
	 * @sa mnemonicIteratorHasNext()
	 */
	GMSEC_API const GMSEC_Mnemonic mnemonicIteratorNext(GMSEC_MnemonicIterator iter, GMSEC_Status status);


	/**
	 * @fn void mnemonicIteratorReset(GMSEC_MnemonicIterator iter, GMSEC_Status status)
	 *
	 * @brief Resets the iterator to the beginning of the Mnemonic list that is maintained by the MnemonicMessage.
	 *
	 * @param[in]  iter   - the handle to the Mnemonic Iterator
	 * @param[out] status - out parameter operation result status
	 *
	 * @sa mnemonicMessageGetIterator()
	 * @sa mnemonicIteratorHasNext()
	 * @sa mnemonicIteratorNext()
	 */
	GMSEC_API void mnemonicIteratorReset(GMSEC_MnemonicIterator iter, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
