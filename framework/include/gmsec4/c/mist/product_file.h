/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file product_file.h
 *
 *  @brief This file contains functions for the management of a MIST ProductFile object.
 */


#ifndef GMSEC_API_C_PRODUCT_FILE_H
#define GMSEC_API_C_PRODUCT_FILE_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>

#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_ProductFile productFileCreate(const char* name, const char* description, const char* version, const char* format, const char* uri, GMSEC_Status status)
	 *
	 * @brief Creates a ProductFile object, and returns a handle to such.
	 *
	 * @param[in]  name         - the name of the file
	 * @param[in]  destcription - a description of the file
	 * @param[in]  version      - the version of the file
	 * @param[in]  format       - the descriptor for the file format
	 * @param[in]  uri          - the URI for the file
	 * @param[out] status       - result of the operation
	 *
	 * @return A handle to a ProductFile object, or NULL if an error occurs.  In case of the latter, examine the status.
	 *
	 * @sa productFileDestroy
	 */
	GMSEC_API GMSEC_ProductFile productFileCreate(const char* name, const char* description, const char* version, const char* format, const char* uri, GMSEC_Status status);


	/**
	 * @fn GMSEC_ProductFile productFileCreateUsingData(const char* name, const char* description, const char* version, const char* format, const GMSEC_BIN data, size_t dataSize, GMSEC_Status status)
	 *
	 * @brief Creates a ProductFile object, and returns a handle to such.
	 *
	 * @param[in]  name         - the name of the file
	 * @param[in]  destcription - a description of the file
	 * @param[in]  version      - the version of the file
	 * @param[in]  format       - the descriptor for the file format
	 * @param[in]  data         - the contents of the file
	 * @param[in]  dataSize     - the size (in bytes) of the data
	 * @param[out] status       - result of the operation
	 *
	 * @return A handle to a ProductFile object, or NULL if an error occurs.  In case of the latter, examine the status.
	 *
	 * @sa productFileDestroy
	 */
	GMSEC_API GMSEC_ProductFile productFileCreateUsingData(const char* name, const char* description, const char* version, const char* format, const GMSEC_BIN data, size_t dataSize, GMSEC_Status status);


	/**
	 * @fn GMSEC_ProductFile productFileCreateCopy(const GMSEC_ProductFile other, GMSEC_Status status)
	 *
	 * @brief Creates a deep-copy of the given ProductFile object, and returns a handle to such.
	 *
	 * @param[in]  other  - the handle to the ProductFile object to copy
	 * @param[out] status - result of the operation
	 *
	 * @return A handle to a ProductFile object, or NULL if an error occurs.  In case of the latter, examine the status.
	 *
	 * @sa productFileDestroy
	 */
	GMSEC_API GMSEC_ProductFile productFileCreateCopy(const GMSEC_ProductFile other, GMSEC_Status status);


	/**
	 * @fn void productFileDestroy(GMSEC_ProductFile* productFile);
	 *
	 * @brief Destroys the given ProductFile object.
	 *
	 * @param[in,out] productFile - the handle to the ProductFile object to destroy
	 */
	GMSEC_API void productFileDestroy(GMSEC_ProductFile* productFile);


	/**
	 * @fn const char* productFileGetName(const GMSEC_ProductFile productFile, GMSEC_Status status)
	 *
	 * @brief Returns the name associated with the ProductFile.
	 *
	 * @param[in]  productFile - the handle to the ProductFile object
	 * @param[out] status      - result of the operation
	 *
	 * @return The name associated with the ProductFile object, or NULL if an error occurs.
	 */
	GMSEC_API const char* productFileGetName(const GMSEC_ProductFile productFile, GMSEC_Status status);


	/**
	 * @fn const char* productFileGetDescription(const GMSEC_ProductFile productFile, GMSEC_Status status)
	 *
	 * @brief Returns the description associated with the ProductFile.
	 *
	 * @param[in]  productFile - the handle to the ProductFile object
	 * @param[out] status      - result of the operation
	 *
	 * @return The description associated with the ProductFile, or NULL if an error occurs.
	 */
	GMSEC_API const char* productFileGetDescription(const GMSEC_ProductFile productFile, GMSEC_Status status);


	/**
	 * @fn const char* productFileGetVersion(const GMSEC_ProductFile productFile, GMSEC_Status status)
	 *
	 * @brief Returns the version of the file associated with the ProductFile.
	 *
	 * @param[in]  productFile - the handle to the ProductFile object
	 * @param[out] status      - result of the operation
	 *
	 * @return The version of the file associated with the ProductFile, or NULL if an error occurs.
	 */
	GMSEC_API const char* productFileGetVersion(const GMSEC_ProductFile productFile, GMSEC_Status status);


	/**
	 * @fn const char* productFileGetFormat(const GMSEC_ProductFile productFile, GMSEC_Status status)
	 *
	 * @brief Returns the format of the file associated with the ProductFile.
	 *
	 * @param[in]  productFile - the handle to the ProductFile object
	 * @param[out] status      - result of the operation
	 *
	 * @return The format of the file associated with the ProductFile, or NULL if an error occurs.
	 */
	GMSEC_API const char* productFileGetFormat(const GMSEC_ProductFile productFile, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL productFileURIAvailable(const GMSEC_ProductFile productFile, GMSEC_Status status)
	 *
	 * @brief Returns whether a file URI is associated with the ProductFile.
	 *
	 * @param[in]  productFile - the handle to the ProductFile object
	 * @param[out] status      - result of the operation
	 *
	 * @return A value of GMSEC_TRUE is returned if a file URI is available, otherwise GMSEC_FALSE.
	 */
	GMSEC_API GMSEC_BOOL productFileURIAvailable(const GMSEC_ProductFile productFile, GMSEC_Status status);


	/**
	 * @fn const char* productFileGetURI(const GMSEC_ProductFile productFile, GMSEC_Status status)
	 *
	 * @brief Returns the file URI (if available) associated with the ProductFile.
	 *
	 * @param[in]  productFile - the handle to the ProductFile object
	 * @param[out] status      - result of the operation
	 *
	 * @return The file URI (if available), otherwise NULL.
	 */
	GMSEC_API const char* productFileGetURI(const GMSEC_ProductFile productFile, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL productFileContentsAvailable(const GMSEC_ProductFile productFile, GMSEC_Status status)
	 *
	 * @brief Returns whether file data is associated with the ProductFile.
	 *
	 * @param[in]  productFile - the handle to the ProductFile object
	 * @param[out] status      - result of the operation
	 *
	 * @return A value of GMSEC_TRUE is return if file data is available, otherwise GMSEC_FALSE.
	 */
	GMSEC_API GMSEC_BOOL productFileContentsAvailable(const GMSEC_ProductFile productFile, GMSEC_Status status);


	/**
	 * @fn size_t productFileGetContents(const GMSEC_ProductFile productFile, GMSEC_BIN* data, GMSEC_Status status)
	 *
	 * @brief Returns the file data and size of such that is associated with the ProductFile.
	 *
	 * @param[in]  productFile - the handle to the ProductFile object
	 * @param[out] data        - the file data contents
	 * @param[out] status      - result of the operation
	 *
	 * @return The size of the file data contents (in bytes).
	 */
	GMSEC_API size_t productFileGetContents(const GMSEC_ProductFile productFile, GMSEC_BIN* data, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
