
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*
* Classification
*  
* UNCLASSIFIED
*
*/    
#ifndef GMSECErrors_Net
#define GMSECErrors_Net

using namespace System;

/// <summary>
/// Types and constants for the gmsec API related to error codes.
/// These are implemented as literals
/// </summary>
namespace Nasa
{
namespace Gmsec
{
namespace Net
{

/// <summary>These are general error codes
/// The error string needs to include general error description, detail
/// description, and any middleware specific info.</summary>
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public enum class GMSECErrorClasses : UInt16
{
	/// <summary>An Error has not occurred.</summary>
	STATUS_NO_ERROR = 0,
	
	/// <summary>An Error occurred in the ConnectionFactory Object.</summary>
	STATUS_FACTORY_ERROR = 1,
	
	/// <summary>An Error occurred in the Connection Object.</summary>
	STATUS_CONNECTION_ERROR = 2,
	
	/// <summary>An Error occurred in the Config Object.</summary>
	STATUS_CONFIG_ERROR = 3,
	
	/// <summary>A vendor specific error occurred in a (middleware?) library.</summary>
	STATUS_MIDDLEWARE_ERROR = 4,
	
	/// <summary>An Error occurred in the Message Object.</summary>
	STATUS_MESSAGE_ERROR = 5,
	
	/// <summary>An Error occurred in the Field Object.</summary>
	STATUS_FIELD_ERROR = 6,
	
	/// <summary>An Error occurred in the Callback Object.</summary>
	STATUS_CALLBACK_ERROR = 7,
	
	/// <summary>An Error occurred in the CallbackLookup Object.</summary>
	STATUS_CALLBACKLKP_ERROR = 8,
	
	/// <summary>An Error occurred in the ConfigFile Object</summary>
	STATUS_CONFIGFILE_ERROR = 9,

	/// <summary>An Error occurred an Iterator Object</summary>
	STATUS_ITERATOR_ERROR	= 10,

	/// <summary>A Policy error occurred.</summary>
	STATUS_POLICY_ERROR = 11,

	/// <summary>A custom error.</summary>
	STATUS_CUSTOM_ERROR = 12,
	
	/// <summary>An Unknown Error occurred in the GMSEC API</summary>
	STATUS_OTHER_ERROR = 20
};

/// <summary>GMSEC Error codes
/// These codes correspond to the specific error being generated.</summary>
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public enum class GMSECErrorCodes : UInt32
{
	/// <summary>The ConnectionFactory object was unable to load one of the needed libraries for the specified middleware</summary>
	LIBRARY_LOAD_FAILURE = 0,

	/// <summary>The connectionType object specified in the ConnectionFactory is invalid.</summary>
	INVALID_CONNECTION_TYPE = 1,
	
	/// <summary>The Connection object could not start/stop the auto dispatcher.</summary>
	AUTODISPATCH_FAILURE = 2,
	
	/// <summary>The ConnectionFactory object was unable to create a valid connection.</summary>
	INVALID_CONNECTION = 3,
	
	/// <summary>The feature is not supported with the specified middleware.</summary>
	FEATURE_NOT_SUPPORTED = 4,
	
	/// <summary>The Config object received invalid configuration parameters</summary>
	INVALID_CONFIG_VALUE = 5,
	
	/// <summary>The Config object has reached the end of the list while performing a
	/// GetNext().  This does not indicate that an actual Error has occurred, only
	/// that the end of the list has been reached.</summary>
	CONFIG_END_REACHED = 6,
	
	/// <summary>The Connection object has received an invalid Message.</summary>
	INVALID_MESSAGE = 7,
	
	/// <summary>The Message object has an unknown message type.</summary>
	UNKNOWN_MSG_TYPE = 8,
	
	/// <summary>The Message object has reached the end of the list while performing a
	/// GetNextField().  This does not indicate that an actual Error has occurred, only
	/// that the end of the list has been reached.</summary>
	FIELDS_END_REACHED = 9,
	
	/// <summary>The Field object has encountered a type mismatch while doing a GetValue().</summary>
	FIELD_TYPE_MISMATCH = 10,
	
	/// <summary>The Field object has encountered a type that is not defined.  This indicates a Field type
	/// that is not one of the GMSEC defined types</summary>
	UNKNOWN_FIELD_TYPE = 11,
	
	/// <summary>The CallBackLookup object could not find the specified callback function in AddCallback.</summary>
	INVALID_CALLBACK = 12,
	
	/// <summary>The Connection object request auto dispatcher failed.</summary>
	REQDISPATCH_FAILURE = 13,
	
	/// <summary>The Connection object ConvertMessage() received invalid input parameter.</summary>
	MSGCONVERT_FAILURE = 14,
	
	/// <summary>The Field object SetName received an invalid Field name for input.</summary>
	INVALID_FIELD_NAME = 15,
	
	/// <summary>The Field object SetName received an invalid Field value for input.</summary>
	INVALID_FIELD_VALUE = 16,
	
	/// <summary>The Config object received an invalid name value for input.</summary>
	INVALID_CONFIG_NAME = 17,
	
	/// <summary>The Connection object received an invalid subject name as input.</summary>
	INVALID_SUBJECT_NAME = 18,
	
	/// <summary>The Connection object returned with no message.</summary>
	NO_MESSAGE_AVAILABLE = 19,
	
	/// <summary>The Connection object GetNextMessage() or ConnectionRequest(),
	/// if the timeout was reached and no message was received.</summary>
	TIMEOUT_OCCURRED = 20,
	
	/// <summary>The Connection object has failed to automatically insert tracking fields.</summary>
	TRACKING_FAILURE = 21,
	
	/// <summary>Config items were set that were not used.  This can happen on a
	/// Message or Connection when trying to set values.  It is possible to
	/// send Config items, which aren't used intentionally, if that is the
	/// case this item can be ignored.</summary>
	UNUSED_CONFIG_ITEM = 22,
	
	/// <summary>A valid field object was not allocated and passed in
	/// as needed by certain calls within the C API.</summary>
	INVALID_FIELD = 23,
	
	/// <summary>This error code is reported when there are problems
	/// parsing xml to create a message.</summary>
	XML_PARSE_ERROR = 24,
	
	/// <summary>This error code is reported when a config object pointer is invalid</summary>
	INVALID_CONFIG = 25,
	
	/// <summary>This error code is reported when a encoding problem occurs</summary>
	ENCODING_ERROR = 26,
	
	/// <summary>This error code indicates GMSEC encountered a problem allocating memory</summary>
	OUT_OF_MEMORY = 27,
	
	/// <summary>This error code indicates the user called Next on an iterator that
	/// reached the end of the field set.</summary>
	INVALID_NEXT = 28,
	
	/// <summary>This error code indicates an operation was attempted on an uninitialized object.</summary>
	INITIALIZATION_ERROR = 29,
	
	/// <summary>This error code indicates an incorrect signature.</summary>
	USER_ACCESS_INVALID = 30,
	
	/// <summary>This error code indicates an unauthorized attempt to publish.</summary>
	PUBLISH_NOT_AUTHORIZED = 31,
	
	/// <summary>This error code indicates an unauthorized attempt to subscribe.</summary>
	SUBSCRIBE_NOT_AUTHORIZED = 32,
	
	/// <summary>This error code indicates message format is incorrect.</summary>
	BAD_MESSAGE_FORMAT = 33,
	
	/// <summary>This error code indicates message format is incorrect.</summary>
	INVALID_SIGNATURE = 34,
	
	/// <summary>This error code indicates an operation was attempted on an uninitialized object.</summary>
	UNINITIALIZED_OBJECT = 35,
	
	/// <summary>This error code indicates a custom code is available via GetCustomCode().</summary>
	CUSTOM_ERROR = 36,

	/// <summary> A non specific Error occurred in the GMSEC API.</summary>
	OTHER_ERROR = 50
};

}
}
}

#endif // GMSECErrors_Net
