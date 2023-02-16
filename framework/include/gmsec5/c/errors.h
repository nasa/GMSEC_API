/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file errors.h
 *
 * @brief Types and constants for the gmsec API related to error codes.
 */

#ifndef GMSEC5_C_ERRORS_H
#define GMSEC5_C_ERRORS_H


/**
 * @enum GMSEC_StatusClass
 * @brief The general GMSEC error status classes.
 */
typedef enum
{
	GMSEC_NO_ERROR = 0,                   /**< No error */
	GMSEC_FACTORY_ERROR = 1,              /**< Error creating Connection object */
	GMSEC_CONNECTION_ERROR = 2,           /**< Error within Connection class */
	GMSEC_CONFIG_ERROR = 3,               /**< Error within Config class */
	GMSEC_MIDDLEWARE_ERROR = 4,           /**< Middleware specific error */
	GMSEC_MSG_ERROR = 5,                  /**< Error within Message class */
	GMSEC_FIELD_ERROR = 6,                /**< Error within Field class */
	GMSEC_CALLBACK_ERROR = 7,             /**< Error using a callback */
	GMSEC_CALLBACK_LOOKUP_ERROR = 8,      /**< Error referencing a callback object */
	GMSEC_CONFIGFILE_ERROR = 9,           /**< Error within ConfigFile class */
	GMSEC_ITERATOR_ERROR = 10,            /**< Error using iterrator */
	GMSEC_POLICY_ERROR = 11,              /**< Error with policy */
	GMSEC_DISPATCHER_ERROR = 12,          /**< Error with AutoDispatcher */
	GMSEC_RESOURCE_GENERATOR_ERROR = 13,  /**< ResourceGenerator error */
	GMSEC_HEARTBEAT_GENERATOR_ERROR = 14, /**< HeartbeatGenerator error */
	GMSEC_MSG_FACTORY_ERROR = 15,         /**< Message Template error */
	GMSEC_SPECIFICATION_ERROR = 16,       /**< Specification error */
	GMSEC_MSG_TEMPLATE_ERROR = 17,        /**< Message Template error */
	GMSEC_FIELD_TEMPLATE_ERROR = 18,      /**< Field Template error */
	GMSEC_CUSTOM_ERROR = 49,              /**< Custom error */
	GMSEC_OTHER_ERROR = 50                /**< Other error */
} GMSEC_StatusClass;


/**
 * @enum GMSEC_StatusCode
 * @brief The general GMSEC error status codes.
 */
typedef enum
{
	/*
	 * General Status Codes
	 */
	GMSEC_NO_ERROR_CODE = 0,				/**< No error */
	GMSEC_INVALID_CONNECTION_TYPE = 1,		/**< Invalid Connection Type specified in configuration */
	GMSEC_AUTODISPATCH_FAILURE = 2,			/**< Autodispatcher encounter an error */
	GMSEC_INVALID_CONNECTION = 3,			/**< Operation attempted with an invalid connection object */
	GMSEC_FEATURE_NOT_SUPPORTED = 4,		/**< Feature is not presently implemented */
	GMSEC_INVALID_CONFIG_VALUE = 5,			/**< Improper value has been supplied in configuration */
	GMSEC_CONFIG_END_REACHED = 6,			/**< No configuration elements remain */
	GMSEC_INVALID_MSG = 7,					/**< Supplied message object is invalid */
	GMSEC_UNKNOWN_MSG_TYPE = 8,				/**< Message created with invalid type */
	GMSEC_FIELDS_END_REACHED = 9,			/**< No more fields available */
	GMSEC_FIELD_TYPE_MISMATCH = 10,			/**< Conflict in field type */
	GMSEC_UNKNOWN_FIELD_TYPE = 11,			/**< Unknown Field Type */
	GMSEC_INVALID_CALLBACK = 12,			/**< Invalid Callback object */
	GMSEC_REQUEST_DISPATCH_FAILURE = 13,	/**< Failure encountered while dispatching request */
	GMSEC_MSG_CONVERT_FAILURE = 14,			/**< Error encountered while converting message */
	GMSEC_INVALID_FIELD_NAME = 15,			/**< Invalid field name supplied */
	GMSEC_INVALID_FIELD_VALUE = 16,			/**< Invalid field value supplied */
	GMSEC_INVALID_CONFIG_NAME = 17,			/**< Invalid configuration name supplied */
	GMSEC_INVALID_SUBJECT_NAME = 18,		/**< Invalid subject name supplied */
	GMSEC_NO_MSG_AVAILABLE = 19,			/**< No message available */
	GMSEC_TIMEOUT_OCCURRED = 20,			/**< Timeout occurred */
	GMSEC_TRACKING_FAILURE = 21,			/**< Tracking failure */
	GMSEC_UNUSED_CONFIG_ITEM = 22,			/**< Configuration element not used in operation */
	GMSEC_INVALID_FIELD = 23,				/**< Invalid field */
	GMSEC_XML_PARSE_ERROR = 24,				/**< No message available */
	GMSEC_INVALID_CONFIG = 25,				/**< Invalid configuration supplied */
	GMSEC_ENCODING_ERROR = 26,				/**< Error encoding GMSEC message */
	GMSEC_OUT_OF_MEMORY = 27,				/**< No more memory can be allocated */
	GMSEC_ITER_INVALID_NEXT = 28,			/**< Next element for iteration is invalid */
	GMSEC_INITIALIZATION_ERROR = 29,		/**< Error initializing */
	GMSEC_USER_ACCESS_INVALID = 30,			/**< User does not have access */
	GMSEC_PUBLISH_NOT_AUTHORIZED = 31,		/**< Publish operation was not authorized */
	GMSEC_SUBSCRIBE_NOT_AUTHORIZED = 32,	/**< Subscribe operation was not authorized */
	GMSEC_BAD_MSG_FORMAT = 33,				/**< Message supplied in improper fromat */
	GMSEC_INVALID_SIGNATURE = 34,			/**< Message supplied with invalid signature */
	GMSEC_UNINITIALIZED_OBJECT = 35,		/**< Object not initialized */
	GMSEC_CUSTOM_ERROR_CODE = 36,			/**< Custom error code supplied */
	GMSEC_AUTODISPATCH_EXCLUSIVE = 37,		/**< Autodispatch has locked requested function */
	GMSEC_LIBRARY_LOAD_FAILURE = 38,		/**< Requested library cannot be loaded */
	GMSEC_CONNECTION_LOST = 39,				/**< Connection lost */
	GMSEC_CONNECTION_CONNECTED = 40,		/**< Connection is connected */
	GMSEC_CONNECTION_RECONNECT = 41,		/**< Connection reconnect  */
	GMSEC_JSON_PARSE_ERROR = 42,			/**< Error parsing JSON */
	GMSEC_PARSE_ERROR = 43,					/**< General error parsing */
	GMSEC_INVALID_STRING_PARAM = 44,            /**< Invalid string parameter */

	GMSEC_CONNECTION_NOT_INITIALIZED = 100,			/**< Connection not initialized */
	GMSEC_HEARTBEAT_SERVICE_IS_RUNNING = 101,		/**< Operation cannot be completed due to running heartbeat service */
	GMSEC_HEARTBEAT_SERVICE_NOT_RUNNING = 102,		/**< Operation cannot be completed due to stopped heartbeat service */
	GMSEC_TEMPLATE_DIR_NOT_FOUND = 103,				/**< Template directory not found */
	GMSEC_TEMPLATE_DIR_ERROR = 104,					/**< Error in accessing template directory */
	GMSEC_TEMPLATE_ID_DOES_NOT_EXIST = 105,			/**< Template ID not found */
	GMSEC_REQUIRED_EMPTY_MESSAGE = 106,				/**< A message must be empty to be instantiated via template */
	GMSEC_SCHEMA_FAILED_TO_PARSE = 107,				/**< Unable to parse message schema */
	GMSEC_INCORRECT_FIELD_TYPE = 108,				/**< Incorrect field type */
	GMSEC_MISSING_REQUIRED_FIELD = 109,				/**< Required field not included in message */
	GMSEC_NON_ALLOWED_FIELD = 110,					/**< Non-allowed field used */
	GMSEC_MSG_LOOKUP_FAILURE = 111,					/**< Error in looking up message */
	GMSEC_INDEX_OUT_OF_RANGE = 112,					/**< Supplied index out of range */
	GMSEC_FIELD_NOT_SET = 113,						/**< Field not set */
	GMSEC_RESOURCE_SERVICE_IS_RUNNING = 114,		/**< Operation cannot be completed due to running resource service */
	GMSEC_RESOURCE_SERVICE_NOT_RUNNING = 115,		/**< Operation cannot be completed due to stopped resource service */
	GMSEC_RESOURCE_INFO_MEMORY_ERROR = 116,			/**< Error accessing memory information by resource information system */
	GMSEC_RESOURCE_INFO_DISK_ERROR = 117,			/**< Error accessing disk information by resource information system */
	GMSEC_RESOURCE_INFO_OS_VERSION_ERROR = 118,		/**< Error accessing O.S. information by resource information system */
	GMSEC_RESOURCE_INFO_CPU_ERROR = 119,			/**< Error accessing CPU information by resource information system */
	GMSEC_RESOURCE_INFO_NET_ERROR = 120,			/**< Error accessing network information by resource information system */
	GMSEC_RESOURCE_INFO_SAMPLING_ERROR = 121,		/**< Error sampling system resources by resource information system */
	GMSEC_FIELD_NOT_ADDED = 122,					/**< Error prevented field from being added to message */
	GMSEC_FIELD_TEMPLATE_NOT_FOUND = 123,			/**< Could not fin a field template with the name provided */
	GMSEC_INVALID_TYPE_CONVERSION = 124,			/**< Error converting data type: incompatible data types */
	GMSEC_VALUE_OUT_OF_RANGE = 125,					/**< Error converting data type: value overflows container */
	GMSEC_MESSAGE_FAILED_VALIDATION = 126,			/**< The message failed to pass validation dur to one or more errors */
	GMSEC_FIELD_FAILED_VALIDATION = 127,			/**< The field failed to pass validation dur to one or more errors */

	GMSEC_OTHER_ERROR_CODE = 500					/**< Other error */
} GMSEC_StatusCode;


#endif
