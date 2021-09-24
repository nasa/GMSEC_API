/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%{
#include <gmsec4/Errors.h>
using namespace gmsec::api;
%}

%include <gmsec4/Errors.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl Errors

=head1 DESCRIPTION

Types and constants for the GMSEC API related to error codes used in Status and Exception objects.

=head2 Public Types and Constants

=head3 Error Class

=over

=item libgmsec_perl::NO_ERROR_CLASS

No error.

=item libgmsec_perl::FACTORY_ERROR

Error creating Connection object.

=item libgmsec_perl::CONNECTION_ERROR

Error within Connection class.

=item libgmsec_perl::CONFIG_ERROR 

Error within Config class.

=item libgmsec_perl::MIDDLEWARE_ERROR 

Middleware specific error.

=item libgmsec_perl::MSG_ERROR 

Error within Message class.

=item libgmsec_perl::FIELD_ERROR 

Error within Field class.

=item libgmsec_perl::CALLBACK_ERROR 

Error using a callback.

=item libgmsec_perl::CALLBACK_LOOKUP_ERROR 

Error referencing a callback object.

=item libgmsec_perl::CONFIGFILE_ERROR 

Error within ConfigFile class

=item libgmsec_perl::ITERATOR_ERROR 

Error using iterator.

=item libgmsec_perl::POLICY_ERROR 

Error with policy.

=item libgmsec_perl::DISPATCHER_ERROR 

Error with AutoDispatcher.

=item libgmsec_perl::MIST_ERROR 

MIST error.

=item libgmsec_perl::CUSTOM_ERROR 

Custom error.

=item libgmsec_perl::OTHER_ERROR 

Other error.

=back

=head3 Error Code

=over

=item libgmsec_perl::NO_ERROR_CODE

No error.

=item libgmsec_perl::INVALID_CONNECTION_TYPE

Invalid Connection type specified in configuration.

=item libgmsec_perl::AUTODISPATCH_FAILURE

AutoDispatcher encountered an error.

=item libgmsec_perl::INVALID_CONNECTION

Operation attempted with an invalid connection object.

=item libgmsec_perl::FEATURE_NOT_SUPPORTED

Feature is not presently implemented.

=item libgmsec_perl::INVALID_CONFIG_VALUE

Improper value has been supplied in configuration.

=item libgmsec_perl::CONFIG_END_REACHED

No configuration elements remain.

=item libgmsec_perl::INVALID_MSG

Supplied message object is invalid.

=item libgmsec_perl::UNKNOWN_MSG_TYPE

Message created with invalid type.

=item libgmsec_perl::FIELDS_END_REACHED

No more fields available.

=item libgmsec_perl::FIELD_TYPE_MISMATCH

Conflict in field type.

=item libgmsec_perl::UNKNOWN_FIELD_TYPE

Unknown field type.

=item libgmsec_perl::INVALID_CALLBACK

Invalid Callback object.

=item libgmsec_perl::REQUEST_DISPATCH_FAILURE

Failure encountered while dispatching request.

=item libgmsec_perl::MSG_CONVERT_FAILURE

Error encountered while converting message.

=item libgmsec_perl::INVALID_FIELD_NAME

Invalid field name supplied.

=item libgmsec_perl::INVALID_FIELD_VALUE

Invalid field value supplied.

=item libgmsec_perl::INVALID_CONFIG_NAME

Invalid configuration name supplied.

=item libgmsec_perl::INVALID_SUBJECT_NAME

Invalid subject name supplied.

=item libgmsec_perl::NO_MSG_AVAILABLE

No message available.

=item libgmsec_perl::TIMEOUT_OCCURRED

Timeout occurred.

=item libgmsec_perl::TRACKING_FAILURE

Tracking failure.

=item libgmsec_perl::UNUSED_CONFIG_ITEM

Configuration element not used in operation.

=item libgmsec_perl::INVALID_FIELD

Invalid field.

=item libgmsec_perl::XML_PARSE_ERROR

Error parsing XML.

=item libgmsec_perl::INVALID_CONFIG

Invalid configuration supplied.

=item libgmsec_perl::ENCODING_ERROR

Error encoding GMSEC message.

=item libgmsec_perl::OUT_OF_MEMORY

No more memory can be allocated.

=item libgmsec_perl::ITER_INVALID_NEXT

Next element for iteration is invalid.

=item libgmsec_perl::INITIALIZATION_ERROR

Error initializing.

=item libgmsec_perl::USER_ACCESS_INVALID

User does not have access.

=item libgmsec_perl::PUBLISH_NOT_AUTHORIZED

Publish operation was not authorized.

=item libgmsec_perl::SUBSCRIBE_NOT_AUTHORIZED

Subscribe operation was not authorized.

=item libgmsec_perl::BAD_MSG_FORMAT

Message supplied in improper format.

=item libgmsec_perl::INVALID_SIGNATURE

Message supplied with invalid signature.

=item libgmsec_perl::UNINITIALIZED_OBJECT

Object not initialized.

=item libgmsec_perl::CUSTOM_ERROR_CODE

Custom error code supplied.

=item libgmsec_perl::AUTODISPATCH_EXCLUSIVE

AutoDispatch has locked request function.

=item libgmsec_perl::LIBRARY_LOAD_FAILURE

Requested library cannot be loaded.

=item libgmsec_perl::CONNECTION_LOST

Connection to middleware lost.

=item libgmsec_perl::CONNECTION_CONNECTED

Connection is connected to the middleware.

=item libgmsec_perl::CONNECTION_RECONNECT

Connection is trying to reconnect to the middleware.

=item libgmsec_perl::JSON_PARSE_ERROR

Error parsing JSON.

=item libgmsec_perl::PARSE_ERROR

General error parsing.

=item libgmsec_perl::INVALID_STRING_PARAM

Invalid string parameter.

=item libgmsec_perl::CONNECTION_NOT_INITIALIZED

Connection not initialized.

=item libgmsec_perl::HEARTBEAT_SERVICE_IS_RUNNING

Operation cannot be completed due to running heartbeat service.

=item libgmsec_perl::HEARTBEAT_SERVICE_NOT_RUNNING

Operation cannot be completed due to stopped heartbeat service.

=item libgmsec_perl::TEMPLATE_DIR_NOT_FOUND

Template directory not found.

=item libgmsec_perl::TEMPLATE_DIR_ERROR

Error accessing template directory.

=item libgmsec_perl::TEMPLATE_ID_DOES_NOT_EXIST

Template ID not found.

=item libgmsec_perl::REQUIRED_EMPTY_MESSAGE

A message must be empty to be initialized via template.

=item libgmsec_perl::SCHEMA_FAILED_TO_PARSE

Unable to parse message schema.

=item libgmsec_perl::INCORRECT_FIELD_TYPE

MIST-related incorrect field type.

=item libgmsec_perl::MISSING_REQUIRED_FIELD

Required field not included in message.

=item libgmsec_perl::NON_ALLOWED_FIELD

Non-allowed field used.

=item libgmsec_perl::MSG_LOOKUP_FAILURE

Error looking up message.

=item libgmsec_perl::INDEX_OUT_OF_RANGE

Supplied index out of range.

=item libgmsec_perl::FIELD_NOT_SET

Field not set.

=item libgmsec_perl::RESOURCE_SERVICE_IS_RUNNING

Operation cannot be completed due to running resource service.

=item libgmsec_perl::RESOURCE_SERVICE_NOT_RUNNING

Operation cannot be completed due to stopped resource service.

=item libgmsec_perl::RESOURCE_INFO_MEMORY_ERROR

Error accessing memory information by resource information system.

=item libgmsec_perl::RESOURCE_INFO_DISK_ERROR

Error accessing disk information by resource information system.

=item libgmsec_perl::RESOURCE_INFO_OS_VERSION_ERROR

Error accessing OS information by resource information system.

=item libgmsec_perl::RESOURCE_INFO_CPU_ERROR

Error accessing CPU information by resource information system.

=item libgmsec_perl::RESOURCE_INFO_NET_ERROR

Error accessing network information by resource information system.

=item libgmsec_perl::RESOURCE_INFO_SAMPLING_ERROR

Error sampling system resources by resource information system.

=item libgmsec_perl::FIELD_NOT_ADDED

Error prevented field from being added to the message.

=item libgmsec_perl::FIELD_TEMPLATE_NOT_FOUND

Could not find a field template with the name provided.

=item libgmsec_perl::INVALID_TYPE_CONVERSION

Error converting data type: incompatible data types.

=item libgmsec_perl::VALUE_OUT_OF_RANGE

Error converting data type: value overflows container.

=item libgmsec_perl::MESSAGE_FAILED_VALIDATION

The message failed to pass validation due to one or more errors.

=item libgmsec_perl::FIELD_FAILED_VALIDATION

The field failed to pass validation due to one or more errors.

=item libgmsec_perl::OTHER_ERROR_CODE

Other error.

=back

=cut
%}
