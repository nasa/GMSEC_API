
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file gmsec_errors.h
 *
 *	@brief Types and constants for the gmsec API related to error codes.
 *	These are implemented as defines to support c & c++ interfaces.
 */

#ifndef gmsec_errors_h
#define gmsec_errors_h

/* GMSEC_SCODE
 * these are general error codes
 *
 * The error string needs to include general error description, detail
 * description, and any middlware specific info.
 */
#define GMSEC_STATUS_CLASS unsigned short

/** @name Error Class Codes
 *
 * @{
 */

/**
* Error Class@n
* An Error has not occurred.
*/
#define GMSEC_STATUS_NO_ERROR			0

/**
* Error Class@n
* An Error occurred in the ConnectionFactory Object.
*/
#define GMSEC_STATUS_FACTORY_ERROR		1

/**
* Error Class@n
* An Error occurred in the Connection Object.
*/
#define GMSEC_STATUS_CONNECTION_ERROR	2

/**
* Error Class@n
* An Error occurred in the Config Object.
*/
#define GMSEC_STATUS_CONFIG_ERROR		3

/**
* Error Class@n
* A vendor specific error occurred in a (middleware?) library.
*/
#define GMSEC_STATUS_LIBRARY_ERROR		4
#define GMSEC_STATUS_MIDDLEWARE_ERROR	GMSEC_STATUS_LIBRARY_ERROR

/**
* Error Class@n
* An Error occurred in the Message Object.
*/
#define GMSEC_STATUS_MESSAGE_ERROR		5

/**
* Error Class@n
* An Error occurred in the Field Object.
*/
#define GMSEC_STATUS_FIELD_ERROR		6

/**
* Error Class@n
* An Error occurred in the Callback Object.
*/
#define GMSEC_STATUS_CALLBACK_ERROR		7

/**
* Error Class@n
* An Error occurred in the CallbackLookup Object.
*/
#define GMSEC_STATUS_CALLBACKLKP_ERROR	8

/**
* Error Class@n
* An Error occurred in the ConfigFile Object
*/
#define GMSEC_STATUS_CONFIGFILE_ERROR	9

/**
* Error Class@n
* An Error occurred an Iterator Object
*/
#define GMSEC_STATUS_ITERATOR_ERROR		10

/**
* Error Class@n
* A Policy error occurred.
*/
#define GMSEC_STATUS_POLICY_ERROR		11

/**
* Error Class@n
* A custom error.
*/
#define GMSEC_STATUS_CUSTOM_ERROR		12

/**
* Error Class@n
* An Unknown Error occurred in the GMSEC API.
*/
#define GMSEC_STATUS_OTHER_ERROR		20
/*  @} */

/** @name Error Instance Codes
 *
 * @{
 */

/**
* Error Code@n
* The ConnectionFactory object was unable to load one of the needed libraries for the specified middleware.
*/
#define GMSEC_LIBRARY_LOAD_FAILURE		0
/**
* Error Code@n
* The connectionType object specified in the ConnectionFactory is invalid.
*/
#define GMSEC_INVALID_CONNECTION_TYPE	1
/**
* Error Code@n
* The Connection object could not start/stop the auto dispatcher.
*/
#define GMSEC_AUTODISPATCH_FAILURE		2
/**
* Error Code@n
* The ConnectionFactory object was unable to create a valid connection.
*/
#define GMSEC_INVALID_CONNECTION		3
/**
* Error Code@n
* The feature is not supported with the specified middleware.
*/
#define GMSEC_FEATURE_NOT_SUPPORTED		4
/**
* Error Code@n
* The Config object received invalid configuration parameters.
*/
#define GMSEC_INVALID_CONFIG_VALUE		5
/**
* Error Code@n
* The Config object has reached the end of the list while performing a
* GetNext().  This does not indicate that an actual Error has occurred, only
* that the end of the list has been reached.
*/
#define GMSEC_CONFIG_END_REACHED		6
/**
* Error Code@n
* The Connection object has received an invalid Message.
*/
#define GMSEC_INVALID_MESSAGE			7
/**
* Error Code@n
* The Message object has an unknown message type.
*/
#define GMSEC_UNKNOWN_MSG_TYPE			8
/**
* Error Code@n
* The Message object has reached the end of the list while performing a
* GetNextField().  This does not indicate that an actual Error has occurred, only
* that the end of the list has been reached.
*/
#define GMSEC_FIELDS_END_REACHED		9
/**
* Error Code@n
* The Field object has encountered a type mismatch while doing a GetValue().
*/
#define GMSEC_FIELD_TYPE_MISMATCH		10
/**
* Error Code@n
* The Field object has encountered a type that is not defined.  This indicates a Field type
* that is not one of the GMSEC defined types
*/
#define GMSEC_UNKNOWN_FIELD_TYPE		11
/**
* Error Code@n
* The CallBackLookup object could not find the specified callback function in AddCallback.
*/
#define GMSEC_INVALID_CALLBACK			12
/**
* Error Code@n
* The Connection object request auto dispatcher failed.
*/
#define GMSEC_REQDISPATCH_FAILURE		13
/**
* Error Code@n
* The Connection object ConvertMessage() received invalid input parameter.
*/
#define GMSEC_MSGCONVERT_FAILURE		14
/**
* Error Code@n
* The Field object SetName received an invalid Field name for input.
*/
#define GMSEC_INVALID_FIELD_NAME		15
/**
* Error Code@n
* The Field object SetName received an invalid Field value for input.
*/
#define GMSEC_INVALID_FIELD_VALUE		16
/**
* Error Code@n
* The Config object received an invalid name value for input.
*/
#define GMSEC_INVALID_CONFIG_NAME		17
/**
* Error Code@n
* The Connection object received an invalid subject name as input.
*/
#define GMSEC_INVALID_SUBJECT_NAME		18
/**
* Error Code@n
* The Connection object returned with no message.
*/
#define GMSEC_NO_MESSAGE_AVAILABLE		19
/**
* Error Code@n
* The Connection object GetNextMessage() or ConnectionRequest(),
* if the timeout was reached and no message was received.
*/
#define GMSEC_TIMEOUT_OCCURRED			20
/**
* Error Code@n
* The Connection object has failed to automatically insert
* tracking fields.
*/
#define GMSEC_TRACKING_FAILURE			21
/**
* Error Code@n
* Config items were set that were not used.  This can happen on a
* Message or Connection when trying to set values.  It is possible to
* send Config items, which aren't used intentionally, if that is the
* case this item can be ignored.
*/
#define GMSEC_UNUSED_CONFIG_ITEM		22
/**
* Error Code@n
* A valid field object was not allocated and passed in
* as needed by certain calls within the C API.
*/
#define GMSEC_INVALID_FIELD				23
/**
* Error Code@n
* This error code is reported when there are problems
* parsing xml to create a message or config.
*/
#define GMSEC_XML_PARSE_ERROR			24
/**
* Error Code@n
* This error code is reported when a config object
* pointer is invalid
*/
#define GMSEC_INVALID_CONFIG			25

/**
* Error Code@n
* This error code is reported when a encoding problem occurs
*/
#define GMSEC_ENCODING_ERROR			26

/**
* Error Code@n
* This error code indicates GMSEC encountered a problem allocating memory
*/
#define GMSEC_OUT_OF_MEMORY				27

/**
* Error Code@n
* This error code indicates the user called Next on an iterator that
* reached the end of the field set.
*/
#define GMSEC_INVALID_NEXT				28

/**
* Error Code@n
* This error code indicates an operation was attempted on an uninitialized object.
*/
#define GMSEC_INITIALIZATION_ERROR		29

/**
* Error Code@n
* This error code indicates an incorrect signature.
*/
#define GMSEC_USER_ACCESS_INVALID		30

/**
* Error Code@n
* This error code indicates an unauthorized attempt to publish.
*/
#define GMSEC_PUBLISH_NOT_AUTHORIZED	31

/**
* Error Code@n
* This error code indicates an unauthorized attempt to subscribe.
*/
#define GMSEC_SUBSCRIBE_NOT_AUTHORIZED	32

/**
* Error Code@n
* This error code indicates message format is incorrect.
*/
#define GMSEC_BAD_MESSAGE_FORMAT		33

/**
* Error Code@n
* This error code indicates message format is incorrect.
*/
#define GMSEC_INVALID_SIGNATURE			34

/**
* Error Code@n
* This error code indicates an operation was attempted on an uninitialized object.
*/
#define GMSEC_UNINITIALIZED_OBJECT		35

/**
* Error Code@n
* This error code indicates a custom code is available via GetCustomCode().
*/
#define GMSEC_CUSTOM_ERROR			36

/**
* Error Code@n
* This error code indicates the AutoDispatcher is running and an exclusive method was called.
*/
#define GMSEC_AUTODISPATCH_EXCLUSIVE		37

/**
* Error Code@n
* This error code indicates the Connection to the middleware server/broker has been interrupted.
*/
#define GMSEC_CONNECTION_LOST        39

/**
* Error Code@n
* This error code indicates the Connection to the middleware server/broker has been established.
*/
#define GMSEC_CONNECTION_CONNECTED     40

/**
* Error Code@n
* This error code indicates that an attempt to reconnect to the middleware server/broker is being attempted.
*/
#define GMSEC_CONNECTION_RECONNECT     41

/**
* Error Code@n
* This error code is reported when there are problems
* parsing json to create a message.
*/
#define GMSEC_JSON_PARSE_ERROR			42

/**
* Error Code@n
* A non specific Error occurred in the GMSEC API.
*/
#define GMSEC_OTHER_ERROR				50


/*  @} */

#endif /* gmsec_errors_h */
