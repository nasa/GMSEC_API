/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file specification.h
 *
 *  @brief This file contains functions for the management of a Specification object.
 *
 * Example creation and use:
 * @code
 * GMSEC_Status        status = statusCreate();
 * GMSEC_Config        config = configCreateWithArgs(argc, argv);
 * GMSEC_Specification spec   = specificationCreate(config, status);
 * GMSEC_Message       msg    = messageCreate("GMSEC.SAT.MISSION.C2CX.HB.FOO", GMSEC_PUBLISH, status);
 *
 * ... add fields to message
 *
 * specificationValidateMessage(spec, msg, status);
 *
 * if (statusIsError(status))
 * {
 *    ... message is invalid
 *    ... handle error
 * }
 * else
 * {
 *    ... message is valid
 * }
 * @endcode
 */


#ifndef GMSEC_API_C_SPECIFICATION_H
#define GMSEC_API_C_SPECIFICATION_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Specification specificationCreate(const GMSEC_Config config, GMSEC_Status status)
	 *
	 * @brief Creates a Specification object instance.
	 *
	 * @param[in]  config - the handle to the Config object.
	 * @param[out] status - out parameter operation result status.
	 *
	 * @return A handle to a Specification, or NULL if an error occurs.  If NULL is returned, refer to the status object.
	 *
	 * @sa specificationDestroy
	 */
	GMSEC_API GMSEC_Specification specificationCreate(const GMSEC_Config config, GMSEC_Status status);


	/**
	 * @fn GMSEC_Specification specificationCreateCopy(const GMSEC_Specification otherSpec, GMSEC_Status status)
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
	GMSEC_API GMSEC_Specification specificationCreateCopy(const GMSEC_Specification otherSpec, GMSEC_Status status);


	/**
	 * @fn void specificationDestroy(GMSEC_Specification* spec)
	 *
	 * @brief Destroys the Specification object.
	 *
	 * @param[in,out] spec - handle to the Specification to destroy.
	 */
	GMSEC_API void specificationDestroy(GMSEC_Specification* spec);


	/**
	 * @fn void specificationValidateMessage(GMSEC_Specification spec, const GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @@brief Using the given message's subject, the appropriate template from the message registry
	 * is referenced.  The contents of the message are then compared to the template to ensure the
	 * message complies with the GMSEC Interface Specification Document (ISD).  If a template cannot
	 * be found for the message, then a new template based on the message will be added to the registry.
	 *
	 * @param[in]  spec   - the handle to the Specification object.
	 * @param[in]  msg    - the handle to the Message to validate.
	 * @param[out] status - out parameter operation result status.
	 */
	GMSEC_API void specificationValidateMessage(GMSEC_Specification spec, const GMSEC_Message msg, GMSEC_Status status);


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
	 * @fn const char* specificationGetTemplateXML(GMSEC_Specification spec, const char* subject, const char* schemaID, GMSEC_Status status)
	 *
	 * @brief Returns a message schema, as XML, from the template with the matching schema ID.  This
	 * XML data can be used to construct a Message object.
	 *
	 * @param[in]  spec     - the handle to the Specification object.
	 * @param[in]  subject  - the desired subject of the message.
	 * @param[in]  schemaID - the schema ID for the desired template.
	 * @param[out] status   - out parameter operation result status.
	 *
	 * @return XML string representation of the message schema, or NULL if a schema cannot be referenced.
	 */
	GMSEC_API const char* specificationGetTemplateXML(GMSEC_Specification spec, const char* subject, const char* schemaID, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
