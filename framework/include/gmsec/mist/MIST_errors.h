
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file MIST_errors.h
 *
 *  @brief This file contains the MIST error codes. These will be checked by
 *  the user in Status objects returned from MIST functions in
 *  ConnectionManager and should be treated as a supplement to the
 *  regular GMSEC error codes.
 *
**/

#ifndef MIST_errors_h
#define MIST_errors_h

/** @name MIST Error Class Codes
 *
 * @{
 */

/**
 * Error Class@n
 * No MIST error has occurred.
 */
#define GMSEC_NO_MIST_ERROR 0

/**
 * Error Class@n
 * A MIST error has occurred.
 */
#define GMSEC_GENERAL_MIST_ERROR 1001

/*  @} */

/** @name Error Instance Codes
 *
 * @{
 */

/**
 * Error Code@n
 * A general error having to do with the heartbeat service.
 */
#define GMSEC_HEARTBEAT_SERVICE_FAILURE 1009

/**
 * Error Code@n
 * A MIST error having to do with the heartbeat service not running,
 * when it is expected to be, has occurred.
 */
#define GMSEC_HEARTBEAT_SERVICE_NOT_RUNNING 1010

/**
 * Error Code@n
 * A MIST error having to do with the heartbeat service running,
 * when it is not expected to be, has occurred.
 */
#define GMSEC_HEARTBEAT_SERVICE_IS_RUNNING 1011

/**
 * Error Code@n
 * A MIST error having to do with fields not being set in a MessageTemplate,
 * or a name or description not being set in a field descriptor, has occurred.
 */
#define GMSEC_UNSET_FIELD 1012

/**
 * Error Code@n
 * A MIST error, typically having to do with an invalid template string
 * such as a NULL pointer being specifed for a C string, has occurred.
 */
#define GMSEC_INVALID_VALUE 1013

/**
 * Error Code@n
 * A MIST template id has been used or looked up which has not been registered.
 */
#define GMSEC_TEMPLATE_ID_DOES_NOT_EXIST 1014

/**
 * Error Code@n
 * No default MIST template directory path could be deduced, for the 
 * current hardware/OS system.
 */
#define GMSEC_SYSTEM_TEMPLATE_DIRECTORY_NOT_FOUND 1015

/**
 * Error Code@n
 * No default MIST template directory could be found, though a path
 * was identified for the current hardware/OS system.
 */
#define GMSEC_COULD_NOT_FIND_TEMPLATE_DIRECTORY 1016

/**
 * Error Code@n
 * Unable to list a templates directory, either default or user-specified.
 */
#define GMSEC_COULD_NOT_LIST_TEMPLATE_DIRECTORY 1017

/**
 * Error Code@n
 * A MIST template schema .xml file failed to parse correctly, most likely
 * due to incorrect format or missing required XML attributes.
 */
#define GMSEC_SCHEMA_FAILED_TO_PARSE 1018

/**
 * Error Code@n
 * A MIST message validation attempt showed that a required field,
 * of a certain template ID, was not present in a message.
 */
#define GMSEC_MISSING_REQUIRED_FIELD 1019

/**
 * Error Code@n
 * A MIST message validation attempt showed that a non-allowed field,
 * for a certain template ID, was present in a message.
 */
#define GMSEC_NON_ALLOWED_FIELD 1020

/**
 * Error Code@n
 * A MIST message field expected to be a type or subtype of the message
 * was not a string.
 */
#define GMSEC_INCORRECT_FIELD_TYPE 1021

/**
 * Error Code@n
 * An empty message was expected to be provided for MIST to instantiate
 * to a template.
 */
#define GMSEC_REQUIRED_BLANK_MESSAGE 1022

/**
 * Error Code@n
 * An attempt was made to access a status at a list index which exceeds
 * the existing indices of MIST context's present statuses list.
 * 
 */
#define GMSEC_OUTSIDE_STATUS_VALUE 1023

/**
 * Error Code@n
 * The connection in a MIST ConnectionManager was not initialized before 
 * there was an attempt to use it.
 * 
 */
#define GMSEC_CONNECTION_NOT_INITIALIZED 1024

/**
 * Error Code@n
 * There was an attempt to lookup a GMSEC message to see if a template ID
 * matched it, but the message was NULL.
 * 
 */
#define GMSEC_MESSAGE_NULL 1025

/*  @} */

#endif
