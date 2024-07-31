/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file specification.h
 *
 *  @brief This file contains functions for the management of a Specification object.
 */


#ifndef GMSEC_API5_C_SPECIFICATION_H
#define GMSEC_API5_C_SPECIFICATION_H

#include <gmsec5/c/message_specification.h>

#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>

#include <stddef.h>


/**
 * @enum GMSEC_SchemaLevel
 *
 * @sa specificationGetSchemaLevel
 */
typedef enum
{
	GMSEC_SCHEMA_LEVEL_0,    /**< C2MS */
	GMSEC_SCHEMA_LEVEL_1,    /**< C2MS Extensions, or optionally user-defined */
	GMSEC_SCHEMA_LEVEL_2,    /**< NASA/GMSEC Addendum, or optionally user-defined */
	GMSEC_SCHEMA_LEVEL_3,    /**< User-defined */
	GMSEC_SCHEMA_LEVEL_4,    /**< User-defined */
	GMSEC_SCHEMA_LEVEL_5,    /**< User-defined */
	GMSEC_SCHEMA_LEVEL_6     /**< User-defined */
} GMSEC_SchemaLevel;


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Specification specificationCreate(GMSEC_Status status)
	 *
	 * @brief Creates a Specification object instance using the default templates.
	 *
	 * @param[out] status - out parameter operation result status.
	 *
	 * @return A handle to a Specification, or NULL if an error occurs. If NULL is returned, refer to the status object.
	 *
	 * @sa specificationDestroy
	 */
	GMSEC_API GMSEC_Specification specificationCreate(GMSEC_Status status);


	/**
	 * @fn GMSEC_Specification specificationCreateUsingConfig(GMSEC_Config config, GMSEC_Status status)
	 *
	 * @brief Creates a Specification object instance using the given configuration.
	 *
	 * @param[in]  config - the handle to the Config object.
	 * @param[out] status - out parameter operation result status.
	 *
	 * @return A handle to a Specification, or NULL if an error occurs. If NULL is returned, refer to the status object.
	 *
	 * @sa specificationDestroy
	 */
	GMSEC_API GMSEC_Specification specificationCreateUsingConfig(GMSEC_Config config, GMSEC_Status status);


	/**
	 * @fn GMSEC_Specification specificationCreateCopy(GMSEC_Specification otherSpec, GMSEC_Status status)
	 *
	 * @brief Creates a copy (clone) of the given Specification object.
	 *
	 * @param[in]  otherSpec - the handle to the Specification object to copy.
	 * @param[out] status    - out parameter operation result status.
	 *
	 * @return A handle to a Specification, or NULL if an error occurs.  If NULL is returned, refer to the status object.
	 *
	 * @sa specificationDestroy
	 */
	GMSEC_API GMSEC_Specification specificationCreateCopy(GMSEC_Specification otherSpec, GMSEC_Status status);


	/**
	 * @fn void specificationDestroy(GMSEC_Specification* spec)
	 *
	 * @brief Destroys the Specification object.
	 *
	 * @param[in,out] spec - handle to the Specification to destroy.
	 */
	GMSEC_API void specificationDestroy(GMSEC_Specification* spec);


	/**
	 * @fn GMSEC_SchemaIDIterator specificationGetSchemaIDIterator(GMSEC_Specification spec, GMSEC_Status status)
	 *
	 * @brief Returns a SchemaIDIterator which can be used to iterate over the IDs of the loaded templates.
	 *
	 * @param[in]  spec   - the handle to the Specification object.
	 * @param[out] status - out parameter operation result status.
	 *
	 * @return A handle to a SchemaIDIterator, or NULL if an error occurs.
	 */
	GMSEC_API GMSEC_SchemaIDIterator specificationGetSchemaIDIterator(GMSEC_Specification spec, GMSEC_Status status);


	/**
	 * @fn unsigned int specificationGetVersion(GMSEC_Specification spec, GMSEC_Status status)
	 *
	 * @brief Returns the version of the GMSEC Interface Specification Document (ISD) that is referenced
	 * by the %Specification object.
	 *
	 * @param[in]  spec   - the handle to the Specification object.
	 * @param[out] status - out parameter operation result status.
	 *
	 * @return The ISD version (e.g. 201600).
	 */
	GMSEC_API unsigned int specificationGetVersion(GMSEC_Specification spec, GMSEC_Status status);


	/**
	 * @fn GMSEC_SchemaLevel specificationGetSchemaLevel(GMSEC_Specification spec, GMSEC_Status status)
	 *
	 * @brief Returns the operating schema level for the given Specification.
	 *
	 * @param[in]  spec   - the handle to the Specification object.
	 * @param[out] status - out parameter operation result status.
	 *
	 * @return Enumerated value from GMSEC_SchemaLevel.
	 */
	GMSEC_API GMSEC_SchemaLevel specificationGetSchemaLevel(GMSEC_Specification spec, GMSEC_Status status);


	/**
	 * @fn void specificationGetMessageSpecifications(GMSEC_Specification spec, GMSEC_MessageSpecification* msgSpecs, size_t* numMsgSpecs, GMSEC_Status status)
	 *
	 * @brief Returns the list of Message Specifications, each of which includes a schema ID and a list of Field Specifications. The user is responsible for
	 * destroying the return list using specificationDestroyMessageSpecifications().
	 *
	 * @param[in]     spec        - the handle to the Specification object.
	 * @param[in/out] msgSpecs    - the handle from which to return the Message Specification(s)
	 * @param[in/out] numMsgSpecs - the handle from which to return the number of Message Specification(s)
	 * @param[out]    status      - out parameter operation result status.
	 *
	 * @sa specificationDestroyMessageSpecifications
	 */
	GMSEC_API void specificationGetMessageSpecifications(GMSEC_Specification spec, GMSEC_MessageSpecification** msgSpecs, size_t* numMsgSpecs, GMSEC_Status status);


	/**
	 * @fn void specificationDestroyMessageSpecifications(GMSEC_MessageSpecification* msgSpecs, size_t numMsgSpecs, GMSEC_Status status)
	 *
	 * @brief Convenience function for destroying (deallocating) memory associated with a list of Message Specifications.
	 *
	 * @param[in]  msgSpecs    - the handle from which to return the Message Specification(s)
	 * @param[in]  numMsgSpecs - the handle from which to return the number of Message Specification(s)
	 * @param[out] status      - out parameter operation result status.
	 */
	GMSEC_API void specificationDestroyMessageSpecifications(GMSEC_MessageSpecification* msgSpecs, size_t numMsgSpecs, GMSEC_Status status);


	/**
	 * @fn void specificationGetHeaderFieldNames(GMSEC_Specification spec, const char* schemaName, const char*** headerFields, size_t* numHeaderFields, GMSEC_Status status)
	 * 
	 * @brief Returns a list of the field names for the specified header 
	 * 
	 * @param[in]  spec            - the handle to the Specification object
	 * @param[in]  schemaName      - The name of the specified schema level (e.g. C2MS, C2MS-PIPE, GMSEC, DEFAULT).  An empty or NULL string refers to DEFAULT.
	 * @param[out] headerFields    - the handle from which to return a list of header field names.
	 * @param[out] numHeaderFields - the handle from which to return the number of names in headerFields.
	 * @param[out] status          - out parameter operation result status.
	 * 
	 */
	GMSEC_API void specificationGetHeaderFieldNames(GMSEC_Specification spec, const char* schemaName, const char*** headerFields, size_t* numHeaderFields, GMSEC_Status status);


	/**
	 * @fn void specificationDestroyHeaderList(const char*** headerFields)
	 * 
	 * @brief Convenience function for destroying (deallocating) memory with a list of header field names.
	 * 
	 * @param[in] headerFields - handle for the list of field names to be destroyed
	 */
	GMSEC_API void specificationDestroyHeaderList(const char*** headerFields);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
