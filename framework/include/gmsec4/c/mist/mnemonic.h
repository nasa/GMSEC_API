/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file mnemonic.h
 *
 *  @brief This file contains functions for the management of a MIST Mnemonic object.
 */


#ifndef GMSEC_API_C_MNEMONIC_H
#define GMSEC_API_C_MNEMONIC_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>

#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Mnemonic mnemonicCreate(const char* name, const GMSEC_MnemonicSample samples[], size_t numSamples, GMSEC_Status status)
	 *
	 * @brief Constructs a MnemonicSample object, and returns a handle to such.
	 *
	 * @param[in]  name       - the name of the Mnemonic
	 * @param[in]  samples    - the array of MnemonicSamples to add to the Mnemonic object
	 * @param[in]  numSamples - the number of MnemonicSamples in the array
	 * @param[out] status     - result of the operation
	 *
	 * @return A handle to a Mnemonic object, or NULL in case of an error.  In case of the latter, examine the status.
	 *
	 * @sa mnemonicDestroy
	 */
	GMSEC_API GMSEC_Mnemonic mnemonicCreate(const char* name, const GMSEC_MnemonicSample samples[], size_t numSamples, GMSEC_Status status);


    /**
     * @fn GMSEC_Mnemonic mnemonicCreateWithNoSamples(const char* name, GMSEC_Status status)
     *
     * @brief Constructs a MnemonicSample object with no samples.
     *
	 * @param[in]  name       - the name of the Mnemonic
	 * @param[out] status     - result of the operation
	 *
	 * @return A handle to a Mnemonic object, or NULL in case of an error.  In case of the latter, examine the status.
	 *
	 * @sa mnemonicDestroy
     */
	GMSEC_API GMSEC_Mnemonic mnemonicCreateWithNoSamples(const char* name, GMSEC_Status status);



	/**
	 * @fn GMSEC_Mnemonic mnemonicCreateCopy(const GMSEC_Mnemonic other, GMSEC_Status status)
	 *
	 * @brief Create a deep copy of the given Mnemonic object, and returns a handle to such.
	 *
	 * @param[in]  other  - the Mnemonic object to copy
	 * @param[out] status - result of the operation
	 *
	 * @return A handle to a Mnemonic object, or NULL in case of an error.  In case of the latter, examine the status.
	 *
	 * @sa mnemonicDestroy
	 */
	GMSEC_API GMSEC_Mnemonic mnemonicCreateCopy(const GMSEC_Mnemonic other, GMSEC_Status status);


	/**
	 * @fn void mnemonicDestroy(GMSEC_Mnemonic* mnemonic)
	 *
	 * @brief Destroys the given Mnemonic object.
	 *
	 * @param[in,out] mnemonic - the Mnemonic object to destroy
	 */
	GMSEC_API void mnemonicDestroy(GMSEC_Mnemonic* mnemonic);


	/**
	 * @fn const char* mnemonicGetName(const GMSEC_Mnemonic mnemonic, GMSEC_Status status)
	 *
	 * @brief Returns the name associated with the Mnemonic object.
	 *
	 * @param[in]  mnemonic - the handle to the Mnemonic object
	 * @param[out] status   - result of the operation
	 *
	 * @return The name of the Mnemonic, or NULL if an error occurs.  In case of the latter, refer to the status.
	 */
	GMSEC_API const char* mnemonicGetName(const GMSEC_Mnemonic mnemonic, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL mnemonicStatusAvailable(const GMSEC_Mnemonic mnemonic, GMSEC_Status status)
	 *
	 * @brief Returns whether a status field is associated with the Mnemonic object.
	 *
	 * @param[in]  mnemonic - the handle to the Mnemonic object
	 * @param[out] status   - result of the operation
	 *
	 * @return A value of GMSEC_TRUE is returned if the status field is available; GMSEC_FALSE otherwise (or if an error occurs).
	 */
	GMSEC_API GMSEC_BOOL mnemonicStatusAvailable(const GMSEC_Mnemonic mnemonic, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field mnemonicGetStatus(const GMSEC_Mnemonic mnemonic, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the status field (if it is available).
	 *
	 * @param[in]  mnemonic - the handle to the Mnemonic object
	 * @param[out] status   - result of the operation
	 *
	 * @return A handle to the status field, or NULL if it is not available.
	 *
	 * @sa mnemonicStatusAvailable
	 */
	GMSEC_API const GMSEC_Field mnemonicGetStatus(const GMSEC_Mnemonic mnemonic, GMSEC_Status status);


	/**
	 * @fn void mnemonicSetStatus(GMSEC_Mnemonic mnemonic, const GMSEC_Field statusField, GMSEC_Status status)
	 *
	 * @brief Assigns a status field to the Mnemonic object.
	 *
	 * @param[in]  mnemonic    - the handle to the Mnemonic object
	 * @param[in]  statusField - the handle to the Field containing the status value
	 * @param[out] status      - result of the operation
	 */
	GMSEC_API void mnemonicSetStatus(GMSEC_Mnemonic mnemonic, const GMSEC_Field statusField, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL mnemonicUnitsAvailable(const GMSEC_Mnemonic mnemonic, GMSEC_Status status)
	 *
	 * @brief Returns whether a unit string is associated with the Mnemonic object.
	 *
	 * @param[in]  mnemonic - the handle to the Mnemonic object
	 * @param[out] status   - result of the operation
	 *
	 * @return The unit string value, or NULL if none is assigned (or if an error occurs).
	 */
	GMSEC_API GMSEC_BOOL mnemonicUnitsAvailable(const GMSEC_Mnemonic mnemonic, GMSEC_Status status);


	/**
	 * @fn const char* mnemonicGetUnits(const GMSEC_Mnemonic mnemonic, GMSEC_Status status)
	 *
	 * @brief Returns the units value, if any, assigned to the Mnemonic object.
	 *
	 * @param[in]  mnemonic - the handle to the Mnemonic object
	 * @param[out] status   - result of the operation
	 *
	 * @return The units value, or NULL if the value is not available.
	 *
	 * @sa mnemonicUnitsAvailable
	 */
	GMSEC_API const char* mnemonicGetUnits(const GMSEC_Mnemonic mnemonic, GMSEC_Status status);


	/**
	 * @fn void mnemonicSetUnits(GMSEC_Mnemonic mnemonic, const char* units, GMSEC_Status status)
	 *
	 * @brief Assigns the unit string to the Mnemonic object.
	 *
	 * @param[in]  mnemonic - the handle to the Mnemonic object
	 * @param[in]  units    - the string containing the units
	 * @param[out] status   - result of the operation
	 */
	GMSEC_API void mnemonicSetUnits(GMSEC_Mnemonic mnemonic, const char* units, GMSEC_Status status);


	/**
	 * @fn void mnemonicAddSample(GMSEC_Mnemonic mnemonic, const MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Adds a copy of the given MnemonicSample to the Mnemonic object.
	 *
	 * @param[in]  mnemonic - the handle to the Mnemonic object
	 * @param[in]  sample   - the handle to a MnemonicSample object
	 * @param[out] status   - result of the operation
	 *
	 * @sa mnemonicSampleCreate
	 */
	GMSEC_API void mnemonicAddSample(GMSEC_Mnemonic mnemonic, const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn size_t mnemonicGetSampleCount(const GMSEC_Mnemonic mnemonic, GMSEC_Status status)
	 *
	 * @brief Returns the number of MnemonicSamples assigned to the Mnemonic object.
	 *
	 * @param[in]  mnemonic - the handle to the Mnemonic object
	 * @param[out] status   - result of the operation
	 *
	 * @return The number of MnemonicSamples assigned to the Mnemonic object.
	 */
	GMSEC_API size_t mnemonicGetSampleCount(const GMSEC_Mnemonic mnemonic, GMSEC_Status status);


	/**
	 * @fn const GMSEC_MnemonicSample mnemonicGetSample(const GMSEC_Mnemonic mnemonic, size_t index, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the MnemonicSample object held in the Mnemonic object.
	 *
	 * @param[in]  mnemonic - the handle to the Mnemonic object
	 * @param[in]  index    - the index into the list of MnemonicSamples
	 * @param[out] status   - result of the operation
	 *
	 * @return A handle to a MnemonicSample object, or NULL if not available at the given index.  For the latter, examine the status.
	 *
	 * @sa mnemonicGetSampleCount
	 */
	GMSEC_API const GMSEC_MnemonicSample mnemonicGetSample(const GMSEC_Mnemonic mnemonic, size_t index, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
