/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file StatusCode.java
 */

package gov.nasa.gsfc.gmsec.api;


/**
 * Error/Status codes (used with Status and Exception classes)
 */
public enum StatusCode
{
	/** No error */
	NO_ERROR_CODE(0),

	/** Invalid Connection Type specified in configuration */
	INVALID_CONNECTION_TYPE(1),

	/** Autodispatcher encounter an error */
	AUTODISPATCH_FAILURE(2),

	/** Operation attempted with an invalid connection object */
	INVALID_CONNECTION(3),

	/** Feature is not presently implemented */
	FEATURE_NOT_SUPPORTED(4),

	/** Improper value has been supplied in configuration */
	INVALID_CONFIG_VALUE(5),

	/** No configuration elements remain */
	CONFIG_END_REACHED(6),

	/** Supplied message object is invalid */
	INVALID_MSG(7),

	/** Message created with invalid type */
	UNKNOWN_MSG_TYPE(8),

	/** No more fields available */
	FIELDS_END_REACHED(9),

	/** Conflict in field type */
	FIELD_TYPE_MISMATCH(10),

	/** Unknown Field Type */
	UNKNOWN_FIELD_TYPE(11),

	/** Invalid Callback object */
	INVALID_CALLBACK(12),

	/** Failure encountered while dispatching request */
	REQUEST_DISPATCH_FAILURE(13),

	/** Error encountered while converting message */
	MSG_CONVERT_FAILURE(14),

	/** Invalid field name supplied */
	INVALID_FIELD_NAME(15),

	/** Invalid field value supplied */
	INVALID_FIELD_VALUE(16),

	/** Invalid configuration name supplied */
	INVALID_CONFIG_NAME(17),

	/** Invalid subject name supplied */
	INVALID_SUBJECT_NAME(18),

	/** No message available */
	NO_MSG_AVAILABLE(19),

	/** Timeout occurred */
	TIMEOUT_OCCURRED(20),

	/** Tracking failure */
	TRACKING_FAILURE(21),

	/** Configuration element not used in operation */
	UNUSED_CONFIG_ITEM(22),

	/** Invalid field */
	INVALID_FIELD(23),

	/** No message available */
	XML_PARSE_ERROR(24),

	/** Invalid configuration supplied */
	INVALID_CONFIG(25),

	/** Error encoding GMSEC message */
	ENCODING_ERROR(26),

	/** No more memory can be allocated */
	OUT_OF_MEMORY(27),

	/** Next element for iteration is invalid */
	ITER_INVALID_NEXT(28),

	/** Error initializing */
	INITIALIZATION_ERROR(29),

	/** User does not have access */
	USER_ACCESS_INVALID(30),

	/** Publish operation was not authorized */
	PUBLISH_NOT_AUTHORIZED(31),

	/** Subscribe operation was not authorized */
	SUBSCRIBE_NOT_AUTHORIZED(32),

	/** Message supplied in improper format */
	BAD_MSG_FORMAT(33),

	/** Message supplied with invalid signature */
	INVALID_SIGNATURE(34),

	/** Object not initialized */
	UNINITIALIZED_OBJECT(35),

	/** Custom error code supplied */
	CUSTOM_ERROR_CODE(36),

	/** Autodispatch has locked requested function */
	AUTODISPATCH_EXCLUSIVE(37),

	/** Requested library cannot be loaded */
	LIBRARY_LOAD_FAILURE(38),

	/** Connection lost */
	CONNECTION_LOST(39),

	/** Connection is connected */
	CONNECTION_CONNECTED(40),

	/** Connection reconnect */
	CONNECTION_RECONNECT(41),

	/** Error parsing JSON */
	JSON_PARSE_ERROR(42),

	/** General error parsing */
	PARSE_ERROR(43),

	/** Invalid string parameter */
	INVALID_STRING_PARAM(44),

	// MIST-related Status Codes (used with Status and Exception classes)
	//
	/** Connection not initialized */
	CONNECTION_NOT_INITIALIZED(100),

	/** Operation cannot be completed due to running heartbeat service */
	HEARTBEAT_SERVICE_IS_RUNNING(101),

	/** Operation cannot be completed due to stopped heartbeat service */
	HEARTBEAT_SERVICE_NOT_RUNNING(102),

	/** Template directory not found */
	TEMPLATE_DIR_NOT_FOUND(103),

	/** Error in accessing template directory */
	TEMPLATE_DIR_ERROR(104),

	/** Template ID not found */
	TEMPLATE_ID_DOES_NOT_EXIST(105),

	/** A message must be empty to be instantiated via template */
	REQUIRED_EMPTY_MESSAGE(106),

	/** Unable to parse message schema */
	SCHEMA_FAILED_TO_PARSE(107),

	/** MIST-related incorrect field type */
	INCORRECT_FIELD_TYPE(108),

	/** Required field not included in message */
	MISSING_REQUIRED_FIELD(109),

	/** Non-allowed field used */
	NON_ALLOWED_FIELD(110),

	/** Error with looking up message */
	MSG_LOOKUP_FAILURE(111),

	/** Supplied index out of range */
	INDEX_OUT_OF_RANGE(112),

	/** Field not set */
	FIELD_NOT_SET(113),

	/** Operation cannot be completed due to running resource service */
   	RESOURCE_SERVICE_IS_RUNNING(114),

	/** Operation cannot be completed due to stopped resource service */
   	RESOURCE_SERVICE_NOT_RUNNING(115),

	/** Error accessing memory information by resource information system */
   	RESOURCE_INFO_MEMORY_ERROR(116),

	/** Error accessing disk information by resource information system */
   	RESOURCE_INFO_DISK_ERROR(117),

	/** Error accessing O.S. information by resource information system */
   	RESOURCE_INFO_OS_VERSION_ERROR(118),

	/** Error accessing CPU information by resource information system */
   	RESOURCE_INFO_CPU_ERROR(119),

	/** Error accessing network information by resource information system */
   	RESOURCE_INFO_NET_ERROR(120),

	/** Error sampling system resources by resource information system */
   	RESOURCE_INFO_SAMPLING_ERROR(121),

	/** Error prevented field from being added to message */
   	FIELD_NOT_ADDED(122),

	/** Could not find a field template with the name provided */
	FIELD_TEMPLATE_NOT_FOUND(123),

	/** Error converting data type: incompatible data types */
	INVALID_TYPE_CONVERSION(124),

	/** Error converting data type: value overflows container */
	VALUE_OUT_OF_RANGE(125),

	/** The message failed to pass validation due to one or more errors */
	MESSAGE_FAILED_VALIDATION(126),

	/** The field failed to pass validation due to one or more errors */
	FIELD_FAILED_VALIDATION(127),

	/** Other error */
	OTHER_ERROR_CODE(500);


	private int value;
	private StatusCode(int value) { this.value = value; }

	public int getValue() { return value; };

	public static StatusCode getUsingValue(int value) 
	{
		StatusCode sc = OTHER_ERROR_CODE;

		for (StatusCode n : StatusCode.values()) 
		{
			if (n.value == value)
			{
				sc = n;
				break;
			}
		}

		return sc;
	}
}
