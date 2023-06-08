/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file resource_generator.h
 *
 *  @brief This file contains a class for starting a resource message generator.
 */


#ifndef GMSEC_API5_C_RESOURCE_GENERATOR_H
#define GMSEC_API5_C_RESOURCE_GENERATOR_H

#include <gmsec5_defs.h>

#include <gmsec5/util/wdllexp.h>

#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @fn GMSEC_ResourceGenerator resourceGeneratorCreate(GMSEC_Config config,
	 *                                                     GMSEC_U16    pubRate,
	 *                                                     GMSEC_U16    sampleInterval,
	 *                                                     GMSEC_U16    averageInterval,
	 *                                                     GMSEC_Status status)
	 *
	 * @brief Creates an instance of a Resource Generator.
	 *
	 * @param[in]  config          - the configuration object to configure the Connection that is used to publish message
	 * @param[in]  pubRate         - the message publish rate (in seconds)
	 * @param[in]  sampleInterval  - the time distance between sample collection (in seconds)
	 * @param[in]  averageInterval - time window over which to make a moving average of samples (in seconds)
	 * @param[out] status          - the result of the operation.
	 *
	 * @return A handle to a Resource Generator, or NULL if an error occurs. If NULL is returned, refer to the status object.
	 *
	 * @note If the publish rate is set to 0 (zero), the resource generator will only publish one message.
	 *
	 * @sa configCreate
	 * @sa resourceGeneratorSetField
	 * @sa resourceGeneratorDestroy
	 */
	GMSEC_API GMSEC_ResourceGenerator resourceGeneratorCreate(GMSEC_Config config,
	                                                          GMSEC_U16    pubRate,
	                                                          GMSEC_U16    sampleInterval,
	                                                          GMSEC_U16    averageInterval,
	                                                          GMSEC_Status status);


	/**
	 * @fn GMSEC_ResourceGenerator resourceGeneratorCreateWithFields(GMSEC_Config config,
	 *                                                               GMSEC_U16    pubRate,
	 *                                                               GMSEC_U16    sampleInterval,
	 *                                                               GMSEC_U16    averageInterval,
	 *                                                               GMSEC_Field  fields[],
	 *                                                               size_t       numFields,
	 *                                                               GMSEC_Status status)
	 *
	 * @brief Creates an instance of a Resource Generator.
	 *
	 * @param[in]  config          - the configuration object to configure the Connection that is used to publish message
	 * @param[in]  pubRate         - the message publish rate (in seconds)
	 * @param[in]  sampleInterval  - the time distance between sample collection (in seconds)
	 * @param[in]  averageInterval - time window over which to make a moving average of samples (in seconds)
	 * @param[in]  fields          - the fields to be added to the resource message
	 * @param[in]  numFields       - the number of fields
	 * @param[out] status          - the result of the operation.
	 *
	 * @return A handle to a Resource Generator, or NULL if an error occurs. If NULL is returned, refer to the status object.
	 *
	 * @note If the publish rate is set to 0 (zero), the resource generator will only publish one message.
	 *
	 * @sa configCreate
	 * @sa resourceGeneratorDestroy
	 */
	 GMSEC_API GMSEC_ResourceGenerator resourceGeneratorCreateWithFields(GMSEC_Config config,
	                                                                     GMSEC_U16    pubRate,
	                                                                     GMSEC_U16    sampleInterval,
	                                                                     GMSEC_U16    averageInterval,
	                                                                     GMSEC_Field  fields[],
	                                                                     size_t       numFields,
	                                                                     GMSEC_Status status);


	/**
	 * @fn void resourceGeneratorDestroy(GMSEC_ResourceGenerator* rsrcgen)
	 *
	 * @brief Destroys the Resource Generator object.
	 *
	 * @param[in,out] rsrcgen - the handle to the Resource Generator to destroy.
	 */
	GMSEC_API void resourceGeneratorDestroy(GMSEC_ResourceGenerator* rsrcgen);


	/**
	 * @fn GMSEC_BOOL resourceGeneratorStart(GMSEC_ResourceGenerator rsrcgen, GMSEC_Status status)
	 *
	 * @brief Starts the resource generator thread.
	 *
	 * @param[in]  rsrcgen - the handle to the Resource Generator object.
	 * @param[out] status  - the result of the operation.
	 *
	 * @return GMSEC_TRUE if the resource generator thread has started; GMSEC_FALSE otherwise.
	 *
	 * @note If the publish rate is set to 0 (zero), only one resource message will be published,
	 * however the resource generator thread will continue to run.
	 *
	 * @sa resourceGeneratorStop()
	 */
	GMSEC_API GMSEC_BOOL resourceGeneratorStart(GMSEC_ResourceGenerator rsrcgen, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL resourceGeneratorStop(GMSEC_ResourceGenerator rsrcgen, GMSEC_Status status)
	 *
	 * @brief Stops the resource generator thread.
	 *
	 * @param[in]  rsrcgen - the handle to the Resource Generator object.
	 * @param[out] status  - the result of the operation.
	 *
	 * @return GMSEC_TRUE if the resource generator thread has stopped; GMSEC_FALSE otherwise.
	 *
	 * @sa resourceGeneratorStart()
	 */
	GMSEC_API GMSEC_BOOL resourceGeneratorStop(GMSEC_ResourceGenerator rsrcgen, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL resourceGeneratorIsRunning(GMSEC_ResourceGenerator rsrcgen, GMSEC_Status status)
	 *
	 * @brief Accessor that can be used to query whether the resource generator thread is running.
	 *
	 * @param[in]  rsrcgen - the handle to the Resource Generator object.
	 * @param[out] status  - the result of the operation.
	 *
	 * @return GMSEC_TRUE if the resource generator thread is running; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL resourceGeneratorIsRunning(GMSEC_ResourceGenerator rsrcgen, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL resourceGeneratorSetField(GMSEC_ResourceGenerator rsrcgen, GMSEC_Field field, GMSEC_Status status)
	 *
	 * @brief Accessor that will add the given field to resource message.
	 *
	 * @param[in]  rsrcgen - the handle to the Resource Generator object.
	 * @param[in]  field   - the field to add to the resource message.
	 * @param[out] status  - the result of the operation.
	 *
	 * @return GMSEC_TRUE if an existing field was overwritten; GMSEC_FALSE otherwise.
	 *
	 * @note If the PUB-RATE field is provided, the publish rate for the resource generator will
	 * be updated with the (presumably) new publish rate.
	 */
	GMSEC_API GMSEC_BOOL resourceGeneratorSetField(GMSEC_ResourceGenerator rsrcgen, GMSEC_Field field, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message createResourceMessage(GMSEC_MessageFactory factory, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval, GMSEC_Status status)
	 *
	 * @brief Creates/returns a Resource Message using the given MessageFactory.
	 *
	 * @param[in]  factory         - the MessageFactory to reference when creating the resource message.
	 * @param[in]  sampleInterval  - the time distance between sample collection (in seconds).
	 * @param[in]  averageInterval - time window over which to make a moving average of samples (in seconds).
	 * @param[out] status          - the result of the operation.
	 *
	 * @returns A Message object representing a Resource Message.
	 *
	 * @sa connectionGetMessageFactory()
	 * @sa messageDestroy()
	 */
	GMSEC_API GMSEC_Message createResourceMessage(GMSEC_MessageFactory factory, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval, GMSEC_Status status);

#ifdef __cplusplus
}	// extern "C"
#endif

#endif
