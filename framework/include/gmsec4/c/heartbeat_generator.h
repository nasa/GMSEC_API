/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file heartbeat_generator.h
 *
 *  @brief This file contains a class for starting a heartbeat message generator.
 */


#ifndef GMSEC_API_C_HEARTBEAT_GENERATOR_H
#define GMSEC_API_C_HEARTBEAT_GENERATOR_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>

#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @fn GMSEC_HeartbeatGenerator heartbeatGeneratorCreate(const GMSEC_Config config,
	 *                                                       const char* hbMsgSubject,
	 *                                                       GMSEC_U16 hbPubRate,
	 *                                                       GMSEC_Status status)
	 *
	 * @brief Creates an instance of a Heartbeat Generator.
	 *
	 * @param[in] config       - the configuration object to configure the ConnectionManager that is used to publish message
	 * @param[in] hbMsgSubject - the subject to include with published heartbeat messages
	 * @param[in] hbPubRate    - the message publish rate (in seconds)
	 * @param[out] status      - the result of the operation.
	 *
	 * @return A handle to a Heartbeat Generator, or NULL if an error occurs. If NULL is returned, refer to the status object.
	 *
	 * @note If the publish rate is set to 0 (zero), the heartbeat generator will only publish one message.
	 *
	 * @sa configCreate
	 * @sa mistMessageSetStandardFields
	 * @sa heartbeatGeneratorDestroy
	 */
	GMSEC_API GMSEC_HeartbeatGenerator heartbeatGeneratorCreate(const GMSEC_Config config,
	                                                            const char* hbMsgSubject,
	                                                            GMSEC_U16 hbPubRate,
	                                                            GMSEC_Status status);


	/**
	 * @fn GMSEC_HeartbeatGenerator heartbeatGeneratorCreateWithFields(const GMSEC_Config config,
	 *                                                                 const char* hbMsgSubject,
	 *                                                                 GMSEC_U16 hbPubRate,
	 *                                                                 GMSEC_Field fields[],
	 *                                                                 size_t numFields,
	 *                                                                 GMSEC_Status status)
	 *
	 * @brief Creates an instance of a Heartbeat Generator.
	 *
	 * @param[in] config       - the configuration object to configure the ConnectionManager that is used to publish message
	 * @param[in] hbMsgSubject - the subject to include with published heartbeat messages
	 * @param[in] hbPubRate    - the message publish rate (in seconds)
	 * @param[in] fields       - the fields to be added to the heartbeat message
	 * @param[in] numFields    - the number of fields
	 * @param[out] status      - the result of the operation.
	 *
	 * @return A handle to a Heartbeat Generator, or NULL if an error occurs. If NULL is returned, refer to the status object.
	 *
	 * @note If the publish rate is set to 0 (zero), the heartbeat generator will only publish one message.
	 *
	 * @sa configCreate
	 * @sa mistMessageSetStandardFields
	 * @sa heartbeatGeneratorDestroy
	 */
	GMSEC_API GMSEC_HeartbeatGenerator heartbeatGeneratorCreateWithFields(const GMSEC_Config config,
	                                                                      const char* hbMsgSubject,
	                                                                      GMSEC_U16 hbPubRate,
	                                                                      GMSEC_Field fields[],
	                                                                      size_t numFields,
	                                                                      GMSEC_Status status);


	/**
	 * @fn void heartbeatGeneratorDestroy(GMSEC_HeartbeatGenerator* hbgen)
	 *
	 * @brief Destroys the Heartbeat Generator object.
	 *
	 * @param[in,out] hbgen - the handle to the Heartbeat Generator to destroy.
	 */
	GMSEC_API void heartbeatGeneratorDestroy(GMSEC_HeartbeatGenerator* hbgen);


	/**
	 * @fn GMSEC_BOOL heartbeatGeneratorStart(GMSEC_HeartbeatGenerator hbgen, GMSEC_Status status)
	 *
	 * @brief Starts the heartbeat generator thread.
	 *
	 * @param[in]  hbgen  - the handle to the Heartbeat Generator object.
	 * @param[out] status - the result of the operation.
	 *
	 * @return GMSEC_TRUE if the heartbeat generator thread has started; GMSEC_FALSE otherwise.
	 *
	 * @note If the publish rate is set to 0 (zero), only one heartbeat message will be published,
	 * however the heartbeat generator thread will continue to run.
	 *
	 * @sa heartbeatGeneratorStop()
	 */
	GMSEC_API GMSEC_BOOL heartbeatGeneratorStart(GMSEC_HeartbeatGenerator hbgen, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL heartbeatGeneratorStop(GMSEC_HeartbeatGenerator hbgen, GMSEC_Status status)
	 *
	 * @brief Stops the heartbeat generator thread.
	 *
	 * @param[in]  hbgen  - the handle to the Heartbeat Generator object.
	 * @param[out] status - the result of the operation.
	 *
	 * @return GMSEC_TRUE if the heartbeat generator thread has stopped; GMSEC_FALSE otherwise.
	 *
	 * @sa heartbeatGeneratorStart()
	 */
	GMSEC_API GMSEC_BOOL heartbeatGeneratorStop(GMSEC_HeartbeatGenerator hbgen, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL heartbeatGeneratorIsRunning(GMSEC_HeartbeatGenerator hbgen, GMSEC_Status status)
	 *
	 * @brief Accessor that can be used to query whether the heartbeat generator thread is running.
	 *
	 * @param[in]  hbgen  - the handle to the Heartbeat Generator object.
	 * @param[out] status - the result of the operation.
	 *
	 * @return GMSEC_TRUE if the heartbeat generator thread is running; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL heartbeatGeneratorIsRunning(GMSEC_HeartbeatGenerator hbgen, GMSEC_Status status);


	/**
	 * @fn void heartbeatGeneratorChangePublishRate(GMSEC_HeartbeatGenerator hbgen, GMSEC_U16 pubRate, GMSEC_Status status)
	 *
	 * @brief Accessor for changing the publish rate for heartbeat message generator.
	 *
	 * @param[in]  hbgen   - the handle to the Heartbeat Generator object.
	 * @param[in]  pubRate - the new publish rate (in seconds)
	 * @param[out] status  - the result of the operation.
	 */
	GMSEC_API void heartbeatGeneratorChangePublishRate(GMSEC_HeartbeatGenerator hbgen, GMSEC_U16 pubRate, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL heartbeatGeneratorSetField(GMSEC_HeartbeatGenerator hbgen, GMSEC_Field field, GMSEC_Status status)
	 *
	 * @brief Accessor that will add the given field to heartbeat message.
	 *
	 * @param[in]  hbgen  - the handle to the Heartbeat Generator object.
	 * @param[in]  field  - the field to add to the heartbeat message.
	 * @param[out] status - the result of the operation.
	 *
	 * @return GMSEC_TRUE if an existing field was overwritten; GMSEC_FALSE otherwise.
	 *
	 * @note If the PUB-RATE field is provided, the publish rate for the heartbeat generator will
	 * be updated with the (presumably) new publish rate.
	 */
	GMSEC_API GMSEC_BOOL heartbeatGeneratorSetField(GMSEC_HeartbeatGenerator hbgen, GMSEC_Field field, GMSEC_Status status);

#ifdef __cplusplus
}	// extern "C"
#endif

#endif
