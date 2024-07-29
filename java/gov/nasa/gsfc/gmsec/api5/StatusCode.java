/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file StatusCode.java
 */

package gov.nasa.gsfc.gmsec.api5;


/**
 * Error/Status codes (used with Status and GmsecException classes)
 */
public class StatusCode
{
	/** No error */
	public final static int NO_ERROR_CODE = 0;

	/** Invalid Connection Type specified in configuration */
	public final static int INVALID_CONNECTION_TYPE = 1;

	/** Autodispatcher encounter an error */
	public final static int AUTODISPATCH_FAILURE = 2;

	/** Operation attempted with an invalid connection object */
	public final static int INVALID_CONNECTION = 3;

	/** Feature is not presently implemented */
	public final static int FEATURE_NOT_SUPPORTED = 4;

	/** Improper value has been supplied in configuration */
	public final static int INVALID_CONFIG_VALUE = 5;

	/** No configuration elements remain */
	public final static int CONFIG_END_REACHED = 6;

	/** Supplied message object is invalid */
	public final static int INVALID_MSG = 7;

	/** Message created with invalid type */
	public final static int UNKNOWN_MSG_TYPE = 8;

	/** No more fields available */
	public final static int FIELDS_END_REACHED = 9;

	/** Conflict in field type */
	public final static int FIELD_TYPE_MISMATCH = 10;

	/** Unknown Field Type */
	public final static int UNKNOWN_FIELD_TYPE = 11;

	/** Invalid Callback object */
	public final static int INVALID_CALLBACK = 12;

	/** Failure encountered while dispatching request */
	public final static int REQUEST_DISPATCH_FAILURE = 13;

	/** Error encountered while converting message */
	public final static int MSG_CONVERT_FAILURE = 14;

	/** Invalid field name supplied */
	public final static int INVALID_FIELD_NAME = 15;

	/** Invalid field value supplied */
	public final static int INVALID_FIELD_VALUE = 16;

	/** Invalid configuration name supplied */
	public final static int INVALID_CONFIG_NAME = 17;

	/** Invalid subject name supplied */
	public final static int INVALID_SUBJECT_NAME = 18;

	/** No message available */
	public final static int NO_MSG_AVAILABLE = 19;

	/** Timeout occurred */
	public final static int TIMEOUT_OCCURRED = 20;

	/** Tracking failure */
	public final static int TRACKING_FAILURE = 21;

	/** Configuration element not used in operation */
	public final static int UNUSED_CONFIG_ITEM = 22;

	/** Invalid field */
	public final static int INVALID_FIELD = 23;

	/** No message available */
	public final static int XML_PARSE_ERROR = 24;

	/** Invalid configuration supplied */
	public final static int INVALID_CONFIG = 25;

	/** Error encoding GMSEC message */
	public final static int ENCODING_ERROR = 26;

	/** No more memory can be allocated */
	public final static int OUT_OF_MEMORY = 27;

	/** Next element for iteration is invalid */
	public final static int ITER_INVALID_NEXT = 28;

	/** Error initializing */
	public final static int INITIALIZATION_ERROR = 29;

	/** User does not have access */
	public final static int USER_ACCESS_INVALID = 30;

	/** Publish operation was not authorized */
	public final static int PUBLISH_NOT_AUTHORIZED = 31;

	/** Subscribe operation was not authorized */
	public final static int SUBSCRIBE_NOT_AUTHORIZED = 32;

	/** Message supplied in improper format */
	public final static int BAD_MSG_FORMAT = 33;

	/** Message supplied with invalid signature */
	public final static int INVALID_SIGNATURE = 34;

	/** Object not initialized */
	public final static int UNINITIALIZED_OBJECT = 35;

	/** Custom error code supplied */
	public final static int CUSTOM_ERROR_CODE = 36;

	/** Autodispatch has locked requested function */
	public final static int AUTODISPATCH_EXCLUSIVE = 37;

	/** Requested library cannot be loaded */
	public final static int LIBRARY_LOAD_FAILURE = 38;

	/** Connection lost */
	public final static int CONNECTION_LOST = 39;

	/** Connection is connected */
	public final static int CONNECTION_CONNECTED = 40;

	/** Connection reconnect */
	public final static int CONNECTION_RECONNECT = 41;

	/** Error parsing JSON */
	public final static int JSON_PARSE_ERROR = 42;

	/** General error parsing */
	public final static int PARSE_ERROR = 43;

	/** Invalid string parameter */
	public final static int INVALID_STRING_PARAM = 44;

	/** Connection not initialized */
	public final static int CONNECTION_NOT_INITIALIZED = 100;

	/** Template directory not found */
	public final static int TEMPLATE_DIR_NOT_FOUND = 103;

	/** Error in accessing template directory */
	public final static int TEMPLATE_DIR_ERROR = 104;

	/** Template ID not found */
	public final static int TEMPLATE_ID_DOES_NOT_EXIST = 105;

	/** A message must be empty to be instantiated via template */
	public final static int REQUIRED_EMPTY_MESSAGE = 106;

	/** Unable to parse message schema */
	public final static int SCHEMA_FAILED_TO_PARSE = 107;

	/** Incorrect field type */
	public final static int INCORRECT_FIELD_TYPE = 108;

	/** Required field not included in message */
	public final static int MISSING_REQUIRED_FIELD = 109;

	/** Non-allowed field used */
	public final static int NON_ALLOWED_FIELD = 110;

	/** Error with looking up message */
	public final static int MSG_LOOKUP_FAILURE = 111;

	/** Supplied index out of range */
	public final static int INDEX_OUT_OF_RANGE = 112;

	/** Field not set */
	public final static int FIELD_NOT_SET = 113;

	/** Operation cannot be completed due to running resource service */
   	public final static int RESOURCE_SERVICE_IS_RUNNING = 114;

	/** Operation cannot be completed due to stopped resource service */
   	public final static int RESOURCE_SERVICE_NOT_RUNNING = 115;

	/** Error accessing memory information by resource information system */
   	public final static int RESOURCE_INFO_MEMORY_ERROR = 116;

	/** Error accessing disk information by resource information system */
   	public final static int RESOURCE_INFO_DISK_ERROR = 117;

	/** Error accessing O.S. information by resource information system */
   	public final static int RESOURCE_INFO_OS_VERSION_ERROR = 118;

	/** Error accessing CPU information by resource information system */
   	public final static int RESOURCE_INFO_CPU_ERROR = 119;

	/** Error accessing network information by resource information system */
   	public final static int RESOURCE_INFO_NET_ERROR = 120;

	/** Error sampling system resources by resource information system */
   	public final static int RESOURCE_INFO_SAMPLING_ERROR = 121;

	/** Error prevented field from being added to message */
   	public final static int FIELD_NOT_ADDED = 122;

	/** Could not find a field template with the name provided */
	public final static int FIELD_TEMPLATE_NOT_FOUND = 123;

	/** Error converting data type: incompatible data types */
	public final static int INVALID_TYPE_CONVERSION = 124;

	/** Error converting data type: value overflows container */
	public final static int VALUE_OUT_OF_RANGE = 125;

	/** The message failed to pass validation due to one or more errors */
	public final static int MESSAGE_FAILED_VALIDATION = 126;

	/** The field failed to pass validation due to one or more errors */
	public final static int FIELD_FAILED_VALIDATION = 127;

	/** Other error */
	public final static int OTHER_ERROR_CODE = 500;
}
