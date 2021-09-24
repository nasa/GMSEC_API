/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file schema_id_iterator.h
 *
 *  @brief This file contains functions for the management of a SchemaIDIterator object.
 *
 * Example creation and use:
 * @code
 * GMSEC_Status           status = statusCreate();
 * GMSEC_Config           config = configCreateWithArgs(argc, argv);
 * GMSEC_Specification    spec   = specificationCreate(config, status);
 * GMSEC_SchemaIDIterator iter   = specificationGetSchemaIDIterator(spec, status);
 *
 * while (schemaIDIteratorHasNext(iter, status) == GMSEC_TRUE)
 * {
 *     const char* schemaID = schemaIDIteratorNext(iter, status);
 *
 *     // do something with schema ID
 * }
 * @endcode
 */


#ifndef GMSEC_API_C_SCHEMA_ID_ITERATOR_H
#define GMSEC_API_C_SCHEMA_ID_ITERATOR_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_BOOL schemaIDIteratorHasNext(const GMSEC_SchemaIDIterator iter, GMSEC_Status status)
	 *
	 * @brief Indicates whether there are additional Schema IDs that can be referenced using schemaIDIteratorNext().
	 *
	 * @param[in]  iter   - the handle to the SchemaIDIterator object.
	 * @param[out] status - out parameter operation result status
	 *
	 * @return GMSEC_TRUE if additional schema IDs are available, GMSEC_FALSE otherwise.
	 *
	 * @sa specificationGetSchemaIDIterator()
	 */
	GMSEC_API GMSEC_BOOL schemaIDIteratorHasNext(const GMSEC_SchemaIDIterator iter, GMSEC_Status status);


	/**
	 * @fn const const char* schemaIDIteratorNext(GMSEC_SchemaIDIterator iter, GMSEC_Status status)
	 *
	 * @brief Returns the next schema ID.
	 *
	 * @param[in]  iter   - the handle to the SchemaIDIterator object.
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A schema ID string, or NULL if one can not be referenced.  For the latter case, examine the status.
	 *
	 * @sa specificationGetSchemaIDIterator()
	 * @sa schemaIDIteratorHasNext()
	 */
	GMSEC_API const char* schemaIDIteratorNext(GMSEC_SchemaIDIterator iter, GMSEC_Status status);


	/**
	 * @fn void schemaIDIteratorReset(GMSEC_SchemaIDIterator iter, GMSEC_Status status)
	 *
	 * @brief Resets the interator to the beginning of the schema ID list that is maintained by the Specification.
	 *
	 * @param[in]  iter   - the handle to the SchemaIDIterator object.
	 * @param[out] status - out parameter operation result status
	 *
	 * @sa specificationGetSchemaIDIterator()
	 * @sa schemaIDIteratorHasNext()
	 * @sa schemaIDIteratorNext()
	 */
	GMSEC_API void schemaIDIteratorReset(GMSEC_SchemaIDIterator iter, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
