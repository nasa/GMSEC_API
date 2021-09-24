/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Errors.h
 *
 * @brief Types and constants for the gmsec API related to error codes.
 */

#ifndef GMSEC_API_ERRORS_H
#define GMSEC_API_ERRORS_H


namespace gmsec
{
namespace api
{


/**
 * @enum StatusClass
 * @brief The general GMSEC error status classes.
 */
enum StatusClass
{
	NO_ERROR_CLASS = 0,          ///< No error
	FACTORY_ERROR = 1,           ///< Error creating Connection object
	CONNECTION_ERROR = 2,        ///< Error within Connection class
	CONFIG_ERROR = 3,            ///< Error within Config class
	MIDDLEWARE_ERROR = 4,        ///< Middleware specific error
	MSG_ERROR = 5,               ///< Error within Message class
	FIELD_ERROR = 6,             ///< Error within Field class
	CALLBACK_ERROR = 7,          ///< Error using a callback
	CALLBACK_LOOKUP_ERROR = 8,   ///< Error referencing a callback object
	CONFIGFILE_ERROR = 9,        ///< Error within ConfigFile class
	ITERATOR_ERROR = 10,         ///< Error using iterrator
	POLICY_ERROR = 11,           ///< Error with policy
	DISPATCHER_ERROR = 12,       ///< Error with AutoDispatcher
	MIST_ERROR = 13,             ///< MIST error
	HEARTBEAT_GENERATOR_ERROR = 14, ///< HeartbeatGenerator error
	CUSTOM_ERROR = 49,           ///< Custom error
	OTHER_ERROR = 50             ///< Other error
};


/**
 * @enum StatusCode
 * @brief The general GMSEC error status codes.
 */
enum StatusCode
{
	// General Status Codes
	//
	NO_ERROR_CODE = 0,				///< No error
	INVALID_CONNECTION_TYPE = 1,	///< Invalid Connection Type specified in configuration
	AUTODISPATCH_FAILURE = 2,		///< Autodispatcher encounter an error
	INVALID_CONNECTION = 3,			///< Operation attempted with an invalid connection object
	FEATURE_NOT_SUPPORTED = 4,		///< Feature is not presently implemented
	INVALID_CONFIG_VALUE = 5,		///< Improper value has been supplied in configuration
	CONFIG_END_REACHED = 6,			///< No configuration elements remain
	INVALID_MSG = 7,				///< Supplied message object is invalid
	UNKNOWN_MSG_TYPE = 8,			///< Message created with invalid type
	FIELDS_END_REACHED = 9,			///< No more fields available
	FIELD_TYPE_MISMATCH = 10,		///< Conflict in field type
	UNKNOWN_FIELD_TYPE = 11,		///< Unknown Field Type
	INVALID_CALLBACK = 12,			///< Invalid Callback object
	REQUEST_DISPATCH_FAILURE = 13,	///< Failure encountered while dispatching request
	MSG_CONVERT_FAILURE = 14,		///< Error encountered while converting message
	INVALID_FIELD_NAME = 15,		///< Invalid field name supplied
	INVALID_FIELD_VALUE = 16,		///< Invalid field value supplied
	INVALID_CONFIG_NAME = 17,		///< Invalid configuration name supplied
	INVALID_SUBJECT_NAME = 18,		///< Invalid subject name supplied
	NO_MSG_AVAILABLE = 19,			///< No message available
	TIMEOUT_OCCURRED = 20,			///< Timeout occurred
	TRACKING_FAILURE = 21,			///< Tracking failure
	UNUSED_CONFIG_ITEM = 22,		///< Configuration element not used in operation
	INVALID_FIELD = 23,				///< Invalid field
	XML_PARSE_ERROR = 24,			///< Error parsing XML
	INVALID_CONFIG = 25,			///< Invalid configuration supplied
	ENCODING_ERROR = 26,			///< Error encoding GMSEC message
	OUT_OF_MEMORY = 27,				///< No more memory can be allocated
	ITER_INVALID_NEXT = 28,			///< Next element for iteration is invalid
	INITIALIZATION_ERROR = 29,		///< Error initializing
	USER_ACCESS_INVALID = 30,		///< User does not have access
	PUBLISH_NOT_AUTHORIZED = 31,	///< Publish operation was not authorized
	SUBSCRIBE_NOT_AUTHORIZED = 32,	///< Subscribe operation was not authorized
	BAD_MSG_FORMAT = 33,			///< Message supplied in improper format
	INVALID_SIGNATURE = 34,			///< Message supplied with invalid signature
	UNINITIALIZED_OBJECT = 35,		///< Object not initialized
	CUSTOM_ERROR_CODE = 36,			///< Custom error code supplied
	AUTODISPATCH_EXCLUSIVE = 37,	///< Autodispatch has locked requested function
	LIBRARY_LOAD_FAILURE = 38,		///< Requested library cannot be loaded
	CONNECTION_LOST = 39,			///< Connection lost
	CONNECTION_CONNECTED = 40,		///< Connection is connected
	CONNECTION_RECONNECT = 41,		///< Connection reconnect 
	JSON_PARSE_ERROR = 42,			///< Error parsing JSON
	PARSE_ERROR = 43,				///< General error parsing
	INVALID_STRING_PARAM = 44,		///< Invalid string parameter


	// MIST-related Status Codes
	//
	CONNECTION_NOT_INITIALIZED = 100,		///< Connection not initialized
	HEARTBEAT_SERVICE_IS_RUNNING = 101,		///< Operation cannot be completed due to running heartbeat service
	HEARTBEAT_SERVICE_NOT_RUNNING = 102,	///< Operation cannot be completed due to stopped heartbeat service
	TEMPLATE_DIR_NOT_FOUND = 103,			///< Template directory not found
	TEMPLATE_DIR_ERROR = 104,				///< Error in accessing template directory
	TEMPLATE_ID_DOES_NOT_EXIST = 105,		///< Template ID not found
	REQUIRED_EMPTY_MESSAGE = 106,			///< A message must be empty to be instantiated via template
	SCHEMA_FAILED_TO_PARSE = 107,			///< Unable to parse message schema
	INCORRECT_FIELD_TYPE = 108,				///< MIST-related incorrect field type
	MISSING_REQUIRED_FIELD = 109,			///< Required field not included in message
	NON_ALLOWED_FIELD = 110,				///< Non-allowed field used
	MSG_LOOKUP_FAILURE = 111,				///< Error in looking up message
	INDEX_OUT_OF_RANGE = 112,				///< Supplied index out of range
	FIELD_NOT_SET = 113,					///< Field not set
	RESOURCE_SERVICE_IS_RUNNING = 114,		///< Operation cannot be completed due to running resource service
	RESOURCE_SERVICE_NOT_RUNNING = 115,		///< Operation cannot be completed due to stopped resource service
	RESOURCE_INFO_MEMORY_ERROR = 116,		///< Error accessing memory information by resource information system
	RESOURCE_INFO_DISK_ERROR = 117,			///< Error accessing disk information by resource information system
	RESOURCE_INFO_OS_VERSION_ERROR = 118,	///< Error accessing O.S. information by resource information system
	RESOURCE_INFO_CPU_ERROR = 119,			///< Error accessing CPU information by resource information system
	RESOURCE_INFO_NET_ERROR = 120,			///< Error accessing network information by resource information system
	RESOURCE_INFO_SAMPLING_ERROR = 121,		///< Error sampling system resources by resource information system
	FIELD_NOT_ADDED = 122,					///< Error prevented field from being added to message
	FIELD_TEMPLATE_NOT_FOUND = 123,			///< Could not find a field template with the name provided
	INVALID_TYPE_CONVERSION = 124,			///< Error converting data type: incompatible data types
	VALUE_OUT_OF_RANGE = 125,				///< Error converting data type: value overflows container
	MESSAGE_FAILED_VALIDATION = 126,		///< The message failed to pass validation due to one or more errors
	FIELD_FAILED_VALIDATION = 127,			///< The field failed to pass validation due to one or more errors

	OTHER_ERROR_CODE = 500					///< Other error
};


}  // namespace api
}  // namespace gmsec

#endif
