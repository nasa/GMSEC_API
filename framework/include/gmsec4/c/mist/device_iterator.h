/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file device_iterator.h
 *
 *  @brief Defines the functions which support the iteration over the Device objects stored in a DeviceMessage.
 *
 */


#ifndef GMSEC_API_C_DEVICE_ITERATOR_H
#define GMSEC_API_C_DEVICE_ITERATOR_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_BOOL deviceIteratorHasNext(GMSEC_DeviceIterator iter, GMSEC_Status status)
	 *
	 * @brief Provides information as to whether there are additional Device objects that can be referenced using deviceIteratorNext().
	 *
	 * @param[in]  iter   - the handle to the Device Iterator
	 * @param[out] status - out parameter operation result status
	 *
	 * @return GMSEC_TRUE if additional Devices are available, GMSEC_FALSE otherwise.
	 *
	 * @sa deviceMessageGetIterator()
	 */
	GMSEC_API GMSEC_BOOL deviceIteratorHasNext(GMSEC_DeviceIterator iter, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Device deviceIteratorNext(GMSEC_DeviceIterator iter, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the next available Device.
	 *
	 * @param[in]  iter   - the handle to the Device Iterator
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A handle to a Device, or NULL if one is not available.  In case of the latter, check the status.
	 *
	 * @sa deviceMessageGetIterator()
	 * @sa deviceIteratorHasNext()
	 */
	GMSEC_API const GMSEC_Device deviceIteratorNext(GMSEC_DeviceIterator iter, GMSEC_Status status);


	/**
	 * @fn void deviceIteratorReset(GMSEC_DeviceIterator iter, GMSEC_Status status)
	 *
	 * @brief Resets the iterator to the beginning of the Device list that is maintained by the DeviceMessage.
	 *
	 * @param[in]  iter   - the handle to the Device Iterator
	 * @param[out] status - out parameter operation result status
	 *
	 * @sa deviceMessageGetIterator()
	 * @sa deviceIteratorHasNext()
	 * @sa deviceIteratorNext()
	 */
	GMSEC_API void deviceIteratorReset(GMSEC_DeviceIterator iter, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
