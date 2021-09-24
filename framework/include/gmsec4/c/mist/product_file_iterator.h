/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file product_file_iterator.h
 *
 *  @brief Defines the functions which support the iteration over the Product File objects stored in a ProductFileMessage.
 */


#ifndef GMSEC_API_C_PRODUCT_FILE_ITERATOR_H
#define GMSEC_API_C_PRODUCT_FILE_ITERATOR_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_BOOL productFileIteratorHasNext(GMSEC_ProductFileIterator iter, GMSEC_Status status)
	 *
	 * @brief Provides information as to whether there are additional ProductFile objects that can be referenced using
	 * productFileIteratorNext().
	 *
	 * @param[in]  iter   - the handle to the ProductFile Iterator
	 * @param[out] status - out parameter operation result status
	 *
	 * @return GMSEC_TRUE if additional ProductFiles are available, GMSEC_FALSE otherwise.
	 *
	 * @sa productFileMessageGetIterator()
	 */
	GMSEC_API GMSEC_BOOL productFileIteratorHasNext(GMSEC_ProductFileIterator iter, GMSEC_Status status);


	/**
	 * @fn const GMSEC_ProductFile productFileIteratorNext(GMSEC_ProductFileIterator iter, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the next available Product File.
	 *
	 * @param[in]  iter   - the handle to the Product File Iterator
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A handle to a ProductFile, or NULL if one is not available.  In case of the latter, check the status.
	 *
	 * @sa productFileMessageGetIterator()
	 * @sa productFileIteratorHasNext()
	 */
	GMSEC_API const GMSEC_ProductFile productFileIteratorNext(GMSEC_ProductFileIterator iter, GMSEC_Status status);


	/**
	 * @fn void productFileIteratorReset(GMSEC_ProductFileIterator iter, GMSEC_Status status)
	 *
	 * @brief Resets the iterator to the beginning of the ProductFile list that is maintained by the ProductFileMessage.
	 *
	 * @param[in]  iter   - the handle to the Product File Iterator
	 * @param[out] status - out parameter operation result status
	 *
	 * @sa productFileMessageGetIterator()
	 * @sa productFileIteratorHasNext()
	 * @sa productFileIteratorNext()
	 */
	GMSEC_API void productFileIteratorReset(GMSEC_ProductFileIterator iter, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
