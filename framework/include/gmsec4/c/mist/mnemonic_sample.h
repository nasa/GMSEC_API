/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file mnemonic_sample.h
 *
 *  @brief This file contains functions for the management of a MIST DeviceParam object.
 */


#ifndef GMSEC_API_C_MNEMONIC_SAMPLE_H
#define GMSEC_API_C_MNEMONIC_SAMPLE_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


/**
 * @typedef GMSEC_LimitFlag
 *
 * @desc Enumeration for indicating state of MnenomicSample
 */
typedef enum
{
	GMSEC_UNSPECIFIED,          /**< Unspecified */
	GMSEC_NO_LIMIT_VIOLATION,   /**< No limit violation */
	GMSEC_RED_LOW,              /**< Red low */
	GMSEC_YELLOW_LOW,           /**< Yellow low */
	GMSEC_YELLOW_HIGH,          /**< Yellow high */
	GMSEC_RED_HIGH              /**< Red high */
} GMSEC_LimitFlag;

#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_MnemonicSample mnemonicSampleCreate(const char* timestamp, const GMSEC_Field rawValue, GMSEC_Status status)
	 *
	 * @brief Constructs a MnemonicSample object, and returns a handle to such.
	 *
	 * @param[in]  timestamp - the time at which the sample value was taken
	 * @param[in]  rawValue  - handle to a Field object that contains the raw sample value
	 * @param[out] status    - result of the operation
	 *
	 * @return A handle to a MnemonicSample, or NULL in case of an error.  In case of the latter, examine the status.
	 *
	 * @sa mnemonicSampleDestroy
	 */
	GMSEC_API GMSEC_MnemonicSample mnemonicSampleCreate(const char* timestamp, const GMSEC_Field rawValue, GMSEC_Status status);


	/**
	 * @fn GMSEC_MnemonicSample mnemonicSampleCreateCopy(const GMSEC_MnemonicSample other, GMSEC_Status status)
	 *
	 * @brief Constructs a copy of the given MnemonicSample object, and returns a handle to such.
	 *
	 * @param[in]  other  - the MnemonicSample object to copy
	 * @param[out] status - result of the operation
	 *
	 * @return A handle to a MnemonicSample, or NULL in case of an error.  In case of the latter, examine the status.
	 *
	 * @sa mnemonicSampleDestroy
	 */
	GMSEC_API GMSEC_MnemonicSample mnemonicSampleCreateCopy(const GMSEC_MnemonicSample other, GMSEC_Status status);


	/**
	 * @fn void mnemonicSampleDestroy(GMSEC_MnemonicSample* sample);
	 *
	 * @brief Destroys the given MnemonicSample object.
	 *
	 * @param[in,out] sample - the handle to the MnemonicSample object to destroy
	 */
	GMSEC_API void mnemonicSampleDestroy(GMSEC_MnemonicSample* sample);


	/**
	 * @fn const char* mnemonicSampleGetTimestamp(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns the timestamp assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return A timestamp string, or NULL if an error occurs.  In case of the latter, examine the status.
	 */
	GMSEC_API const char* mnemonicSampleGetTimestamp(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field mnemonicSampleGetRawValue(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns the raw-value field assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return A handle to a GMSEC_Field, or NULL if an error occurs.  In case of the latter, examine the status.
	 */
	GMSEC_API const GMSEC_Field mnemonicSampleGetRawValue(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL mnemonicSampleHasEUValue(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns whether an EU value (field) has been assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns GMSEC_TRUE if an EU value has been assigned; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL mnemonicSampleHasEUValue(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field mnemonicSampleGetEUValue(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns the EU field that has been assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns a handle to the EU field, or NULL if none assigned.  Always check the status to verify the data returned is valid.
	 *
	 * @sa mnemonicSampleHasEUValue
	 */
	GMSEC_API const GMSEC_Field mnemonicSampleGetEUValue(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn void mnemonicSampleSetEUValue(GMSEC_MnemonicSample sample, const GMSEC_Field value, GMSEC_Status status)
	 *
	 * @brief Allows for the setting of the EU field.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object
	 * @param[in]  value  - handle to the EU field
	 * @param[out] status - the result of the operation
	 */
	GMSEC_API void mnemonicSampleSetEUValue(GMSEC_MnemonicSample sample, const GMSEC_Field value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL mnemonicSampleHasTextValue(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns whether a text value has been assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns GMSEC_TRUE if a text value has been assigned; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL mnemonicSampleHasTextValue(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn const char* mnemonicSampleGetTextValue(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns the text value that has been assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns the text value, or NULL if none assigned.  Always check the status to verify the data returned is valid.
	 *
	 * @sa mnemonicSampleHasTextValue
	 */
	GMSEC_API const char* mnemonicSampleGetTextValue(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn void mnemonicSampleSetTextValue(GMSEC_MnemonicSample sample, const char* text, GMSEC_Status status)
	 *
	 * @brief Allows for the setting of the text value.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object
	 * @param[in]  text   - the text value
	 * @param[out] status - the result of the operation
	 */
	GMSEC_API void mnemonicSampleSetTextValue(GMSEC_MnemonicSample sample, const char* text, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL mnemonicSampleHasFlags(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns whether a Flags value has been assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns GMSEC_TRUE if a Flags value has been assigned; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL mnemonicSampleHasFlags(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn GMSEC_I32 mnemonicSampleGetFlags(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns the flags value that has been assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns the flags value.  Always check the status to verify the data returned is valid.
	 *
	 * @sa mnemonicSampleHasFlags
	 */
	GMSEC_API GMSEC_I32 mnemonicSampleGetFlags(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn void mnemonicSampleSetFlags(GMSEC_MnemonicSample sample, GMSEC_I32 flags, GMSEC_Status status)
	 *
	 * @brief Allows for the setting of the flags value.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object
	 * @param[in]  flags  - the limit enable/disable value
	 * @param[out] status - the result of the operation
	 */
	GMSEC_API void mnemonicSampleSetFlags(GMSEC_MnemonicSample sample, GMSEC_I32 flags, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL mnemonicSampleHasLimitEnableDisable(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns whether a Limit Enable/Disable value has been assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns GMSEC_TRUE if a Limit Enable/Disable value has been assigned; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL mnemonicSampleHasLimitEnableDisable(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL mnemonicSampleGetLimitEnableDisable(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns the Limit Enable/Disable value that has been assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns the Limit Enable/Disable value.  Always check the status to verify the data returned is valid.
	 *
	 * @sa mnemonicSampleHasLimitEnableDisable
	 */
	GMSEC_API GMSEC_BOOL mnemonicSampleGetLimitEnableDisable(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn void mnemonicSampleSetLimitEnableDisable(GMSEC_MnemonicSample sample, GMSEC_BOOL value, GMSEC_Status status)
	 *
	 * @brief Allows for the setting of the limit enable/disable value.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object
	 * @param[in]  value  - the limit enable/disable value
	 * @param[out] status - the result of the operation
	 */
	GMSEC_API void mnemonicSampleSetLimitEnableDisable(GMSEC_MnemonicSample sample, GMSEC_BOOL value, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL mnemonicSampleHasLimit(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns whether a Limit Flag value has been assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns GMSEC_TRUE if a Limit Flag value has been assigned; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL mnemonicSampleHasLimit(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn GMSEC_LimitFlag mnemonicSampleGetLimit(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns the Limit Flag value that has been assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns the Limit Flag value.  Always check the status to verify the data returned is valid.
	 *
	 * @sa mnemonicSampleHasLimit
	 */
	GMSEC_API GMSEC_LimitFlag mnemonicSampleGetLimit(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn void mnemonicSampleSetLimit(GMSEC_MnemonicSample sample, GMSEC_LimitFlag limit, GMSEC_Status status)
	 *
	 * @brief Allows for the setting of the limit value.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object
	 * @param[in]  limit  - the limit value
	 * @param[out] status - the result of the operation
	 */
	GMSEC_API void mnemonicSampleSetLimit(GMSEC_MnemonicSample sample, GMSEC_LimitFlag limit, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL mnemonicSampleHasStalenessStatus(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns whether a staleness status value has been assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns GMSEC_TRUE if a staleness status value has been assigned; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL mnemonicSampleHasStalenessStatus(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL mnemonicSampleGetStalenessStatus(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns the staleness status value that has been assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns the staleness status value.  Always check the status to verify the data returned is valid.
	 *
	 * @sa mnemonicSampleHasStalenessStatus
	 */
	GMSEC_API GMSEC_BOOL mnemonicSampleGetStalenessStatus(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn void mnemonicSampleSetStalenessStatus(GMSEC_MnemonicSample sample, GMSEC_BOOL staleness, GMSEC_Status status);
	 *
	 * @brief Allows for the setting of the staleness status value.
	 *
	 * @param[in]  sample    - the handle to the MnemonicSample object
	 * @param[in]  staleness - the staleness status value
	 * @param[out] status    - the result of the operation
	 */
	GMSEC_API void mnemonicSampleSetStalenessStatus(GMSEC_MnemonicSample sample, GMSEC_BOOL staleness, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL mnemonicSampleHasQuality(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns whether a quality value has been assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns GMSEC_TRUE if a quality value has been assigned; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL mnemonicSampleHasQuality(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL mnemonicSampleGetQuality(const GMSEC_MnemonicSample sample, GMSEC_Status status)
	 *
	 * @brief Returns the quality value that has been assigned to the MnemonicSample.
	 *
	 * @param[in]  sample - the handle to the MnemonicSample object.
	 * @param[out] status - the result of the operation
	 *
	 * @return Returns the quality value.  Always check the status to verify the data returned is valid.
	 *
	 * @sa mnemonicSampleHasQuality
	 */
	GMSEC_API GMSEC_BOOL mnemonicSampleGetQuality(const GMSEC_MnemonicSample sample, GMSEC_Status status);


	/**
	 * @fn void mnemonicSampleSetQuality(GMSEC_MnemonicSample sample, GMSEC_BOOL quality, GMSEC_Status status)
	 *
	 * @brief Allows for the setting of the quality value.
	 *
	 * @param[in]  sample  - the handle to the MnemonicSample object
	 * @param[in]  quality - the quality value
	 * @param[out] status  - the result of the operation
	 */
	GMSEC_API void mnemonicSampleSetQuality(GMSEC_MnemonicSample sample, GMSEC_BOOL quality, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
