/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API {

/// <summary>Error/Status codes (used with Status and GmsecException classes)</summary>

public enum StatusCode {

  /// <summary>No error</summary>
  NO_ERROR_CODE = 0,

  /// <summary>Invalid Connection Type specified in configuration</summary>
  INVALID_CONNECTION_TYPE = 1,

  /// <summary>Autodispatcher encounter an error</summary>
  AUTODISPATCH_FAILURE = 2,

  /// <summary>Operation attempted with an invalid connection object</summary>
  INVALID_CONNECTION = 3,

  /// <summary>Feature is not presently implemented</summary>
  FEATURE_NOT_SUPPORTED = 4,

  /// <summary>Improper value has been supplied in configuration</summary>
  INVALID_CONFIG_VALUE = 5,

  /// <summary>No configuration elements remain</summary>
  CONFIG_END_REACHED = 6,

  /// <summary>Supplied message object is invalid</summary>
  INVALID_MSG = 7,

  /// <summary>Message created with invalid type</summary>
  UNKNOWN_MSG_TYPE = 8,

  /// <summary>No more fields available</summary>
  FIELDS_END_REACHED = 9,

  /// <summary>Conflict in field type</summary>
  FIELD_TYPE_MISMATCH = 10,

  /// <summary>Unknown Field Type</summary>
  UNKNOWN_FIELD_TYPE = 11,

  /// <summary>Invalid callback object</summary>
  INVALID_CALLBACK = 12,

  /// <summary>Failure encountered while dispatching request</summary>
  REQUEST_DISPATCH_FAILURE = 13,

  /// <summary>Error encountered while converting message</summary>
  MSG_CONVERT_FAILURE = 14,

  /// <summary>Invalid field name supplied</summary>
  INVALID_FIELD_NAME = 15,

  /// <summary>Invalid field value supplied</summary>
  INVALID_FIELD_VALUE = 16,

  /// <summary>Invalid configuration name supplied</summary>
  INVALID_CONFIG_NAME = 17,

  /// <summary>Invalid subject name supplied</summary>
  INVALID_SUBJECT_NAME = 18,

  /// <summary>No message available</summary>
  NO_MSG_AVAILABLE = 19,

  /// <summary>Timeout occurred</summary>
  TIMEOUT_OCCURRED = 20,

  /// <summary>Tracking failure</summary>
  TRACKING_FAILURE = 21,

  /// <summary>Configuration element not used in operation</summary>
  UNUSED_CONFIG_ITEM = 22,

  /// <summary>Invalid field</summary>
  INVALID_FIELD = 23,

  /// <summary>No message available</summary>
  XML_PARSE_ERROR = 24,

  /// <summary>Invalid configuration supplied</summary>
  INVALID_CONFIG = 25,

  /// <summary>Error encoding GMSEC message</summary>
  ENCODING_ERROR = 26,

  /// <summary>No more memory can be allocated</summary>
  OUT_OF_MEMORY = 27,

  /// <summary>Next element for iteration is invalid</summary>
  ITER_INVALID_NEXT = 28,

  /// <summary>Error initializing</summary>
  INITIALIZATION_ERROR = 29,

  /// <summary>User does not have access</summary>
  USER_ACCESS_INVALID = 30,

  /// <summary>Publish operation was not authorized</summary>
  PUBLISH_NOT_AUTHORIZED = 31,

  /// <summary>Subscribe operation was not authorized</summary>
  SUBSCRIBE_NOT_AUTHORIZED = 32,

  /// <summary>Message supplied in improper format</summary>
  BAD_MSG_FORMAT = 33,

  /// <summary>Message supplied with invalid signature</summary>
  INVALID_SIGNATURE = 34,

  /// <summary>Object not initialized</summary>
  UNINITIALIZED_OBJECT = 35,

  /// <summary>Custom error code supplied</summary>
  CUSTOM_ERROR_CODE = 36,

  /// <summary>Autodispatch has locked requested function</summary>
  AUTODISPATCH_EXCLUSIVE = 37,

  /// <summary>Requested library cannot be loaded</summary>
  LIBRARY_LOAD_FAILURE = 38,

  /// <summary>Connection lost</summary>
  CONNECTION_LOST = 39,

  /// <summary>Connection is connected</summary>
  CONNECTION_CONNECTED = 40,

  /// <summary>Connection reconnect</summary>
  CONNECTION_RECONNECT = 41,

  /// <summary>Error parsing JSON</summary>
  JSON_PARSE_ERROR = 42,

  /// <summary>General error parsing</summary>
  PARSE_ERROR = 43,

  /// <summary>Invalid string parameter</summary>
  INVALID_STRING_PARAM = 44,

  /// <summary>ConnectionManager not initialized</summary>
  CONNECTION_NOT_INITIALIZED = 100,

  /// <summary>Operation cannot be completed due to running heartbeat service</summary>
  HEARTBEAT_SERVICE_IS_RUNNING = 101,

  /// <summary>Operation cannot be completed due to stopped heartbeat service</summary>
  HEARTBEAT_SERVICE_NOT_RUNNING = 102,

  /// <summary>Template directory not found</summary>
  TEMPLATE_DIR_NOT_FOUND = 103,

  /// <summary>Error in accessing template directory</summary>
  TEMPLATE_DIR_ERROR = 104,

  /// <summary>Template ID not found</summary>
  TEMPLATE_ID_DOES_NOT_EXIST = 105,

  /// <summary>A message must be empty to be instantiated via template</summary>
  REQUIRED_EMPTY_MESSAGE = 106,

  /// <summary>Unable to parse message schema</summary>
  SCHEMA_FAILED_TO_PARSE = 107,

  /// <summary>MIST-related incorrect field type</summary>
  INCORRECT_FIELD_TYPE = 108,

  /// <summary>Required field not included in message</summary>
  MISSING_REQUIRED_FIELD = 109,

  /// <summary>Non-allowed field used</summary>
  NON_ALLOWED_FIELD = 110,

  /// <summary>Error with looking up message</summary>
  MSG_LOOKUP_FAILURE = 111,

  /// <summary>Supplied index out of range</summary>
  INDEX_OUT_OF_RANGE = 112,

  /// <summary>Field not set</summary>
  FIELD_NOT_SET = 113,

  /// <summary>Operation cannot be completed due to running resource service</summary>
  RESOURCE_SERVICE_IS_RUNNING = 114,

  /// <summary>Operation cannot be completed due to stopped resource service</summary>
  RESOURCE_SERVICE_NOT_RUNNING = 115,

  /// <summary>Error accessing memory information by resource information system</summary>
  RESOURCE_INFO_MEMORY_ERROR = 116,

  /// <summary>Error accessing disk information by resource information system</summary>
  RESOURCE_INFO_DISK_ERROR = 117,

  /// <summary>Error accessing O.S. information by resource information system</summary>
  RESOURCE_INFO_OS_VERSION_ERROR = 118,

  /// <summary>Error accessing CPU information by resource information system</summary>
  RESOURCE_INFO_CPU_ERROR = 119,

  /// <summary>Error accessing network information by resource information system</summary>
  RESOURCE_INFO_NET_ERROR = 120,

  /// <summary>Error sampling system resources by resource information system</summary>
  RESOURCE_INFO_SAMPLING_ERROR = 121,

  /// <summary>Error prevented field from being added to message</summary>
  FIELD_NOT_ADDED = 122,

  /// <summary>Could not find a field template with the name provided</summary>
  FIELD_TEMPLATE_NOT_FOUND = 123,

  /// <summary>Error converting data type: incompatible data types</summary>
  INVALID_TYPE_CONVERSION = 124,

  /// <summary>Error converting data type: value overflows container</summary>
  VALUE_OUT_OF_RANGE = 125,

  /// <summary>The message failed to pass validation due to one or more errors</summary>
  MESSAGE_FAILED_VALIDATION = 126,

  /// <summary>The field failed to pass validation due to one or more errors</summary>
  FIELD_FAILED_VALIDATION = 127,

  /// <summary>Other error</summary>
  OTHER_ERROR_CODE = 500
}

}
