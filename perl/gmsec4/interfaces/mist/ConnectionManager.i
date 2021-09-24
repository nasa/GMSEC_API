/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ConnectionManager
%{
#include <gmsec4/mist/ConnectionManager.h>
#include <list>
using namespace gmsec::api::mist;
%}

//Functions containing lists that will be redefined
%ignore gmsec::api::mist::ConnectionManager::setStandardFields(const gmsec::api::util::DataList<gmsec::api::Field*>&);
%ignore gmsec::api::mist::ConnectionManager::createHeartbeatMessage(const char*, const gmsec::api::util::DataList<gmsec::api::Field*>&);
%ignore gmsec::api::mist::ConnectionManager::startHeartbeatService(const char*, const gmsec::api::util::DataList<gmsec::api::Field*>&);
%ignore gmsec::api::mist::ConnectionManager::createLogMessage(const char*, const gmsec::api::util::DataList<gmsec::api::Field*>&);
%ignore gmsec::api::mist::ConnectionManager::setLoggingDefaults(const char*, const gmsec::api::util::DataList<gmsec::api::Field*>&);
%ignore gmsec::api::mist::ConnectionManager::requestDirective(const char*, const gmsec::api::Field&, const gmsec::api::util::DataList<gmsec::api::Field*>&);
%ignore gmsec::api::mist::ConnectionManager::requestDirective(const char*, const gmsec::api::Field&, const gmsec::api::util::DataList<gmsec::api::Field*>&, int, gmsec::api::mist::ConnectionManagerReplyCallback*, int);
%ignore gmsec::api::mist::ConnectionManager::requestDirective(const char*, const gmsec::api::Field&, const gmsec::api::util::DataList<gmsec::api::Field*>&, int, int);
%ignore gmsec::api::mist::Connectionmanager::acknowledgeDirectiveRequest(const char*, const gmsec::api::Message&, gmsec::api::mist::ResponseStatus::Response, const gmsec::api::util::DataList<gmsec::api::Field*>&);
%ignore gmsec::api::mist::ConnectionManager::acknowledgeSimpleService(const char*, const gmsec::api::Mesage&, gmsec::api::mist::ResponseStatus::Response, const gmsec::api::util::DataList<Field*>& fields);
%ignore gmsec::api::mist::ConnectionManager::requestSimpleService(const char*, const char*, const gmsec::api::Field&, const gmsec::api::util::DataList<gmsec::api::Field*>&, const gmsec::api::util::DataList<gmsec::api::mist::ServiceParam*>&);
%ignore gmsec::api::mist::ConnectionManager::requestSimpleService(const char*, const char*, const gmsec::api::Field&, const gmsec::api::util::DataList<gmsec::api::Field*>&, const gmsec::api::util::DataList<gmsec::api::mist::ServiceParam*>&, int, gmsec::api::mist::ConnectionManagerReplyCallback*, int);
%ignore gmsec::api::mist::ConnectionManager::requestSimpleService(const char*, const char*, const gmsec::api::Field&, const gmsec::api::util::DataList<gmsec::api::Field*>&, const gmsec::api::util::DataList<gmsec::api::mist::ServiceParam*>&, int, int);

//callback related functions to be ignored
%ignore gmsec::api::mist::ConnectionManager::registerEventCallback(gmsec::api::Connection::ConnectionEvent, gmsec::api::mist::ConnectionManagerEventCallback);
%ignore gmsec::api::mist::ConnectionManager::subscribe(const char*, gmsec::api::mist::ConnectionManagerCallback*);
%ignore gmsec::api::mist::ConnectionManager::subscribe(const char*, const gmsec::api::Config&, gmsec::api::mist::ConnectionManagerCallback*);

%ignore gmsec::api::mist::ConnectionManager::request(const gmsec::api::Message&, GMSEC_I32, gmsec::api::mist::ConnectionManagerReplyCallback*, GMSEC_I32);
%ignore gmsec::api::mist::ConnectionManager::cancelRequest(gmsec::api::mist::COnnectionManagerReplyCallback*);
%ignore gmsec::api::mist::ConnectionManager::startAutoDispatch();
%ignore gmsec::api::mist::ConnectionManager::stopAutoDispatch(bool);

//perl does not handle references to pointers very well, so these functions will be redefined
%ignore gmsec::api::mist::ConnectionManager::unsubscribe(SubscriptionInfo*&);
%ignore gmsec::api::mist::ConnectionManager::release(Message*&);


//C binding functions that have nothing to do with perl binding
%ignore gmsec::api::mist::ConnectionManager::registerMessageValidator(GMSEC_MessageValidator*);
%ignore gmsec::api::mist::ConnectionManager::registerEventCallback(Connection::ConnectionEvent, GMSEC_ConnectionMgrEventCallback*);
%ignore gmsec::api::mist::ConnectionManager::subscribe(const char*, const Config&, GMSEC_ConnectionMgrCallback*);
%ignore gmsec::api::mist::ConnectionManager::request(const Message&, GMSEC_I32, GMSEC_ConnectionMgrReplyCallback*, GMSEC_ConnectionMgrEventCallback*, GMSEC_I32);
%ignore gmsec::api::mist::ConnectionManager::cancelRequest(GMSEC_ConnectionMgrReplyCallback*);
%ignore gmsec::api::mist::ConnectionManager::requestDirective(const char*, const Field&, const gmsec::api::util::DataList<Field*>&, GMSEC_I32, GMSEC_ConnectionMgrReplyCallback*, GMSEC_ConnectionMgrEventCallback*, GMSEC_I32);
%ignore gmsec::api::mist::ConnectionManager::requestSimpleService(const char*, const char*, const Field&, const gmsec::api::util::DataList<Field*>&, const gmsec::api::util::DataList<ServiceParam*>&, GMSEC_I32, GMSEC_ConnectionMgrReplyCallback*, GMSEC_ConnectionMgrEventCallback*, GMSEC_I32);

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ConnectionManager.h>

%include <std_list.i>
namespace std
{
    %template(FieldList) list<gmsec::api::Field*>;
    %template(ServiceParamList) list<gmsec::api::mist::ServiceParam*>;
}

%extend gmsec::api::mist::ConnectionManager {

    void CALL_TYPE setStandardFields(const std::list<gmsec::api::Field*>& standardFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for (std::list<gmsec::api::Field*>::const_iterator it = standardFields.begin(); it != standardFields.end(); ++it)
            fields.push_back(*it);


        self->setStandardFields(fields);
    }
        
    gmsec::api::Message CALL_TYPE createHeartbeatMessage(const char* subject, const std::list<gmsec::api::Field*>& heartbeatFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;
        
        for(std::list<gmsec::api::Field*>::const_iterator it = heartbeatFields.begin(); it != heartbeatFields.end(); ++it)
            fields.push_back(*it);

        return self->createHeartbeatMessage(subject, fields);
    }

    void CALL_TYPE startHeartbeatService(const char* subject, const std::list<gmsec::api::Field*>& heartbeatFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for(std::list<gmsec::api::Field*>::const_iterator it = heartbeatFields.begin(); it != heartbeatFields.end(); ++it)
            fields.push_back(*it);

        self->startHeartbeatService(subject, fields);
    }

    gmsec::api::Message CALL_TYPE createLogMessage(const char* subject, const std::list<gmsec::api::Field*>& logFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for(std::list<gmsec::api::Field*>::const_iterator it = logFields.begin(); it != logFields.end(); ++it)
            fields.push_back(*it);

        return self->createLogMessage(subject, fields);
    }

    void CALL_TYPE setLoggingDefaults(const char* subject, const std::list<gmsec::api::Field*> logFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for(std::list<gmsec::api::Field*>::const_iterator it = logFields.begin(); it != logFields.end(); ++it)
            fields.push_back(*it);

        self->setLoggingDefaults(subject, fields);
    }

    void CALL_TYPE requestDirective(const char* subject, const gmsec::api::Field& directiveString, const std::list<gmsec::api::Field*> dFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for(std::list<gmsec::api::Field*>::const_iterator it = dFields.begin(); it != dFields.end(); ++it)
            fields.push_back(*it);

        self->requestDirective(subject, directiveString, fields);
    }
    
    gmsec::api::Message* CALL_TYPE requestDirective(const char* subject, const gmsec::api::Field& directiveString, const std::list<gmsec::api::Field*> dFields, int timeout, int republish_ms)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for(std::list<gmsec::api::Field*>::const_iterator it = dFields.begin(); it != dFields.end(); ++it)
            fields.push_back(*it);

        return self->requestDirective(subject, directiveString, fields, timeout, republish_ms);
    }

    void CALL_TYPE acknowledgeDirectiveRequest(const char* subject, const gmsec::api::Message& request, gmsec::api::mist::ResponseStatus::Response ssResponse, const std::list<gmsec::api::Field*>& dFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for(std::list<gmsec::api::Field*>::const_iterator it = dFields.begin(); it != dFields.end(); ++it)
            fields.push_back(*it);

        self->acknowledgeDirectiveRequest(subject, request, ssResponse, fields);
    }

    void acknowledgeSimpleService(const char* subject, const gmsec::api::Message& request, gmsec::api::mist::ResponseStatus::Response ssResponse, const std::list<Field*>& sFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for(std::list<gmsec::api::Field*>::const_iterator it = sFields.begin(); it != sFields.end(); ++it)
            fields.push_back(*it);

        self->acknowledgeSimpleService(subject, request, ssResponse, fields);
    }

    void CALL_TYPE requestSimpleService(const char* subject, const char* opName, const gmsec::api::Field& opNumber, const std::list<gmsec::api::Field*>& sFields, const std::list<gmsec::api::mist::ServiceParam*>& sParams)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;
        gmsec::api::util::DataList<gmsec::api::mist::ServiceParam*> params;

        for(std::list<gmsec::api::Field*>::const_iterator it = sFields.begin(); it != sFields.end(); ++it)
            fields.push_back(*it);

        for(std::list<gmsec::api::mist::ServiceParam*>::const_iterator it = sParams.begin(); it != sParams.end(); ++it)
            params.push_back(*it);

        self->requestSimpleService(subject, opName, opNumber, fields, params);
    }
    
    void CALL_TYPE requestSimpleService(const char* subject, const char* opName, const gmsec::api::Field& opNumber, const std::list<gmsec::api::Field*>& sFields, const std::list<gmsec::api::mist::ServiceParam*>& sParams, int timeout, int republish_ms)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;
        gmsec::api::util::DataList<gmsec::api::mist::ServiceParam*> params;

        for(std::list<gmsec::api::Field*>::const_iterator it = sFields.begin(); it != sFields.end(); ++it)
            fields.push_back(*it);

        for(std::list<gmsec::api::mist::ServiceParam*>::const_iterator it = sParams.begin(); it != sParams.end(); ++it)
            params.push_back(*it);

        self->requestSimpleService(subject, opName, opNumber, fields, params, timeout, republish_ms);
    }

    void CALL_TYPE unsubscribe(SubscriptionInfo* info)
    {
        self->unsubscribe(info);
    }

    void CALL_TYPE release(Message* message)
    {
        self->release(message);
    }
}

%perlcode%{

=pod

=head1 NAME

libgmsec_perl::ConnectionManager

=head1 DESCRIPTION

This class wraps the GMSEC Connection object, supplying both normal middleware interaction functions and some additional quality of service.  This is accomplished through standard services for producing Log and Heartbeat messages, as well as functionality for validating messages.

If the object is configured to validate messages, all messages sent from this ConnectionManager via either Publish or Request will be compared to an available schema definition.  If a template definition for the message in question is available, it will be validated.  The message will not be sent over the middleware if validation fails.  Messages without a template definition can be sent as well.

=head2 Public Member Subroutines

=head3 getAPIVersion()

C<libgmsec_perl::ConnectionManager-E<gt>getAPIVersion()>

    This function identifies the version of the API.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    API version information

=head3 new

C<libgmsec_perl::ConnectionManager-E<gt>new($cfg, $validate, $version)>

    Constructor - Initializes the ConnectionManager instance without attempting a connection to the middleware. Note that message validation will be disabled, unless otherwise indicated.  If enabled, or if the configuration option GMSEC-MSG-CONTENT-VALIDATE is set to true, then messages will be validated against the default version of the GMSEC Interface Specification Document (ISD).  The version of the ISD can be changed by specifying the configuration option GMSEC-SPECIFICATION-VERSION to a valid value (e.g. 201400).

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters</b><br>

    $cfg - the config object used to construct the underlying connection object
    $validate - boolean flag to indicate whether the ConnectionManager should validate messages (default is false)
    $version - the version of the GMSEC message specification to be used in validating messages (default is GMSEC_ISD_CURRENT)

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A ConnectionManager object configured using the specified config object and additional parameters

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    An Exception is thrown if the configuration information cannot be used to deduce a Connection type, or if an anomaly occurs while loading schemas for the specified ISD.

=head3 DESTROY

C<libgmsec_perl::ConnectionManager-E<gt>DESTROY()>

    destructor

=head3 initialize

C<libgmsec_perl::ConnectionManager-E<gt>initialize()>

    Establishes a connection with the GMSEC middleware server.  Once this call successfully returns, the ConnectionManager is ready for message operations.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    An Exception is thrown if an anomaly occurs while connecting to the middleware server

=head3 cleanup

C<libgmsec_perl::ConnectionManager-E<gt>cleanup()>

    this function disconnects and destroys the underlying Connection object, throwing an error status if this operation is not successful

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    An Exception is thrown if an anomaly occurs while disconnecting

=head3 getLibraryRootName()

C<libgmsec_perl::ConnectionManager-E<gt>getLibraryRootName()>

    This function identifies the root library name and therefore the connection type that this connection is associated with.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    root library name

=head3 getLibraryVersion()

C<libgmsec_perl::ConnectionManager-E<gt>getLibraryVersion()>

    This function returns a string containing the version information for the associated middleware of the connection.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    library version

=head3 getSpecification

C<libgmsec_perl::ConnectionManager-E<gt>getSpecification()>

    Returns the specification object associated with ConnectionManager

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns</b><br>

    A Specification object

=head3 setSpecification

C<libgmsec_perl::ConnectionManager-E<gt>setSpecification()>

    This method will allow for a user to register their custom subclass of the Specification class with the Connection Manager. This custom Specification can implement its own validateMessage() method which can be used to perform validation of messages currently not performed by the GMSEC API.

    Note the API does not assume ownership of the provided Specification object, nor does it make a copy of such. The user is responsible to ensure that the provided Specification object is not destroyed while the ConnectionManager is in possession of such.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $spec - A specialized subsclass of the Specification class

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:>/b><br>

    Exception if the given Specification object is null

=head3 setStandardFields

C<libgmsec_perl::ConnectionManager-E<gt>setStandardFields($standardFields)>

     Sets the internal list of fields that are added to all messages that are created using the ConnectionManager.  Internal copies of the provided Fields are made, thus ownership is not retained by the ConnectionManager. The supplied set of fields will not be validated here; validation occurs at the time a message is to be published.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>
    
    $standardFields - A FieldList object containing fields to be copied to the internal set of fields, which will in turn be appended to all messages.

head3 addStandardFields

C<libgmsec_perl::ConnectionManager-E<gt>addStandardFields($msg)>

    Adds the standard fields (if any) to the given Message object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $msg - a message object

=head3 unsubscribe

C<libgmsec_perl::ConnectionManager-E<gt>unsubscribe($info)>

    This method unsubscribes to a particular subject or pattern, and will stop the reception of messages that match this pattern.  It will also remove the registration of any callbacks with this subject pattern.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $info - a SubscriptionInfo object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized
    Exception if SubscriptionInfo object originated from a different ConnectionManager
    Exception if error occurs at the middleware level

=head3 publish

C<libgmsec_perl::ConnectionManager-E<gt>publish($msg)>

    If ConnectionManager has been created with "validate" option disabled, this is a pass-through function to the underlying connection.  Otherwise, the message will be validated before it is published.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $msg - the GMSEC message to be published

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been intitialized
    Exception if message fails validation, or if any other severe error occurs with sending the message.

=head3 publish

C<libgmsec_perl::ConnectionManager-E<gt>publish($msg, $mwConfig)>

    Publishes the given message to the middleware using the given configuration to enable or disable certain middleware-level publish functionalities (e.g. ActiveMQ - Durable Producer). Message will still be validated if message validation is enabled.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

    The actual Message published to the middleware will contain tracking fields; to disable this feature, create a ConnectionManager object with the tracking=off configuration option.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $msg - the GMSEC message to be published
    $mwConfig - config object for providing middleware configuration options

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized
    Exception if the message fails validation, or if any other severe error occurs with sending the message

=head3 request

C<libgmsec_perl::ConnectionManager-E<gt>request($request, $timeout, $republish_ms)>

    If this connection manager has been created with "validate" option disabled, this is a pass-through function to the underlying connection.  Otherwise, the request message will be validated before it is issued.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $request - request message to be sent
    $timeout - maximum timeout to wait for reply (in milliseconds).
    $republish_ms - request message resubmission interval (in milliseconds). If set to a negative value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0, the period will default to 60000ms, unless the user has provided an alternate time period via the Config object used to create the Connection object.  The minimum republish period allowed is 100ms.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    Reply message, or NULL if no reply is received in time

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:>/b><br>

    Exception if ConnectionManager has not been initialized
    Exception if Request message fails validation, or on error transmitting request message

=head3 reply

C<libgmsec_perl::ConnectionManager-E<gt>reply($request, $reply)>

    If ConnectionManager has been created with "validate" option disabled, this is a pass-through function to the underlying connection.  

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $request - the pending request message
    $reply - the reply message to be sent

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized
    Exception if reply message fails validation, or on error transmitting reply message

=head3 dispatch

C<libgmsec_perl::ConnectionManager-E<gt>dispatch($message)>

    Attempts to dispatch the given message to callbacks that have been previously registered.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $message - message to be dispatched

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized

=for html &nbsp;&nbsp;&nbsp;&nbsp;See Also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ConnectionManager.html#subscribe">subscribe()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ConnectionManager.html#receive">receive()</a><br>

=head3 receive

C<libgmsec_perl::ConnectionManager-E<gt>receive($timeout)>

    This function is a pass-through function to the underlying connection.  This function returns the next message received within the specified timeout.  The received messages are determined by the subscribe() functions(s), but dispatchMsg needs to be called on messages received from this function to ensure all registered callbacks are executed.

    This function <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $timeout - the maximum time to block waiting for a message, in millliseconds

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    the next received message, if any. If a timeout occurs, NULL is returned.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized

=head3 release

C<libgmsec_perl::ConnectionManager-E<gt>release($message)>

    Destroys a message and cleans up any associated memory.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $message - GMSEC message to be destroyed

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions</b><br>

    Exception if ConnectionManager has not been initialized

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ConnectionManager.html#receive">receive()</a><br>

=head3 startAutoDispatch

=head3 excludeSubject

C<libgmsec_perl::ConnectionManager-E<gt>excludeSubject($subject)>

    Remove an excluded subject, allowing incoming messages with the matching subject to once again be received.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - the subject pattern to look for in incoming messages.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized

=head3 removeExcludedSubject

C<libgmsec_perl::ConnectionManager-E<gt>removeExcludedSubject($subject)>

    Remove an excluded subject, allowing incoming messages with the matching subject to once again be received

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - the subject pattern to remove

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized

=head3 getName

C<libgmsec_perl::ConnectionManager-E<gt>getName()>

    Returns the name of the connection, automatically generated or user specified.

=head3 setName

C<libgmsec_perl::ConnectionManager-E<gt>setName($name)>

    Set the logical name of this connection. This can be used for Identifying connections withing a client program. If a name is not given, one will be automatically generated.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $name - the name of this connection

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized, or if the name string is NULL.

=head3 getID

C<libgmsec_perl::ConnectionManager-E<gt>getID()>

    Returns the string ID for this connection.

=head3 getMWInfo

C<libgmsec_perl::ConnectionManager-E<gt>getMWInfo()>

    Returns a string containing middleware information.

=head3 getConnectionEndpoint

C<libgmsec_perl::ConnectionManager-E<gt>getConnectionEndpoint()>

    Returns middleware broker connection information.

=head3 getPublishQueueMessageCount

C<libgmsec_perl::ConnectionManager-E<gt>getPublishQueueMessageCount()>

    Retrieves the number of messages queued for asynchronous publish operations.

=head3 createHeartbeatmessage

C<libgmsec_perl::ConnectionManager-E<gt>createHeartbeatmessage($subject, $heartbeatFields)>

    Creates a message and passes ownership to the user.  This message is populated with the standard set of required and optional heartbeat fields, as well as the required common fields defined in setStandardFields().  

    MESSAGE-TYPE, MESSAGE-SUBTYPE, and if applicable, C2CX-SUBTYPE fields will all be generated and added to the message automatically, according to the C2MS Message Standard.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - the topic (subject) that will be applied to the return messages.
    $heartbeatfields - a Fieldlist object containing fields to be added to the heartbeat

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
    
    A C2CX heartbeat message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized
    Exception if subject is NULL

=head3 startHeartbeatMessage

C<libgmsec_perl::ConnectionManager-E<gt>starthearbeatService($subject, $heartbeatFields)>

    Creates a archetypal heartbeat message from the concatenation of the standard fields applied globally to this ConnectionManager and to this function.  This message will then be validated, if the "validate" flag is set.  If there is an error, an Exception will be thrown.  If it has not been set, the message will be published at an interval supplied by the "PUB-RATE" field regardless of validation results.  If no "PUB-RATE" has been defined, the service will default to the GMSEC standard of 30 second heartbeat interval.

    If users would like to have a COUNTER field added to the published heartbeat message, then the Heartbeat
    Service should be provided with this field within the list of field provided to this method.

    MESSAGE-TYPE, MESSAGE-SUBTYPE, and if applicable, C2CX-SUBTYPE fields will all be generated and added to the message automatically, according to the C2MS Message Standard

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - subject to set as the default for heartbeat messages
    $heartbeatFields - a FieldList object containing fields to be added to the published heartbeat messages

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized

=head3 stopheartbeatService

C<libgmsec_perl::ConnectionManager-E<gt>stopHeartbeatService()>

    Terminates operation of the heartbeat service and cleans up related resources.  It will be called by the cleanup() if not done manually by the user.

=head3 changeComponentStatus

C<libgmsec_perl::ConnectionManager-E<gt>changeComponentStatus($componentStatus)>

    Updates the field which is included within the Heartbeat Message being published by the Heartbeat Service.  If validation is enabled, the message will then be re-validated with the modified field set.  Should the validation pass, the change will be persisted to the heartbeat service, otherwise an Exception is thrown.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $componentStatus - Field containing updated information for the Heartbeat message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Return:</b><br>

    The status of the operation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

    This method has been deprecated; use setHeartbeatServiceField() instead.

=head3 changeComponentInfo

C<libgmsec_perl::ConnectionManager-E<gt>changeComponentInfo($componentInfo)>

    Updates the field which is included within the Heartbeat Message being published by the Heartbeat Service.  If validation is enabled, the message will then be re-validated with the modified field set.  Should the validation pass, the change will be persisted to the heartbeat service, otherwise an Exception is thrown.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $componentInfo - Field containing updated information for the Heartbeat message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Return:</b><br>

    The status of the operation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

    This method has been deprecated; use setHeartbeatServiceField() instead.

=head3 changeCPUMemory

C<libgmsec_perl::ConnectionManager-E<gt>changeCPUMemory($componentCPUMemory)>

    Updates the field which is included within the Heartbeat Message being published by the Heartbeat Service.  If validation is enabled, the message will then be re-validated with the modified field set.  Should the validation pass, the change will be persisted to the heartbeat service, otherwise an Exception is thrown.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $componentCPUMemory - Field containing updated information for the Heartbeat message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Return:</b><br>

    The status of the operation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

    This method has been deprecated; use setHeartbeatServiceField() instead.

=head3 changeCPUUtil

C<libgmsec_perl::ConnectionManager-E<gt>changeCPUUtil($componentCPUUtil)>

    Updates the field which is included within the Heartbeat Message being published by the Heartbeat Service.  If validation is enabled, the message will then be re-validated with the modified field set.  Should the validation pass, the change will be persisted to the heartbeat service, otherwise an Exception is thrown.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $componentCPUUtil - Field containing updated information for the Heartbeat message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Return:</b><br>

    The status of the operation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

    This method has been deprecated; use setHeartbeatServiceField() instead.

=head3 setHeartbeatServiceField

C<libgmsec_perl::ConnectionManager-E<gt>setHeartbeatServiceField($field)>

    This method can be used to update or set a field within the Heartbeat Message being published by the Heartbeat Service.  If validation is enabled, the field will be validated before it is added to the Heartbeat Message.  Should validation pass, the field will be persisted to the heartbeat service; otherwise an error is returned.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $field - the field containing new or updated information for the Heartbeat Message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Return:</b><br>

    The status of the operation.

=head3 createLogMessage

C<libgmsec_perl::ConnectionManager-E<gt>createLogMessage($subject, $logFields)>

    Creates a Log Message and passes ownership to the User.  This message is populated with the standard set of required and optional log fields, as well as the required common fields defined in setStandardFields.  The message is not validated at this time, as MSG-TEXT and SEVERITY fields mist be set by the user at the time the message is to be sent.

    This message automatically is generated with MESSAGE-TYPE and MESSAGE-SYBTYPE correctly set to the GMSEC Message Standard requirements, regardless of user input.  EVENT-TIME defaults to the time which this function was invoked; the user may override this time once the message has been returned.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - the topic (subject) to apply to the returned message
    $logFields - a FieldList object containing fields to be added to the log message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A Log Message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized

=head3 setLoggingDefaults 

C<libgmsec_perl::ConnectionManager-E<gt>setLoggingDefaults($subject, $logFields)>

    Sets the standard default fields which will be added to new log messages when publishLog is called.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - subject to use for log messages
    $logFields - a FieldList object containing fields to be added to log messages

=head3 publishLog 

C<libgmsec_perl::ConnectionManager-E<gt>publishLog($logMessage, $severity)>

    Creates a log message comprised of the global fields from the entire ConnectionManager, the fields set in setLoggingDefaults, and the log text and severity fields as supplied as arguments to this function.  If validation is enabled, the message will be published.  If validation is enabled, the message will be published.  If validation is enabled, the message will be validated.  If validation of the message fails, then an Exception will be thrown.

    This function automatically populates the message with teh correct MESSAGE-TYPE and MESSAGE-SUBTYPE fields.  EVENT-TIME is set automatically to the time of the publish invovation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $logMessage - the message string to log
    $severity - the Field object defining the severity of the log message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized
    Exception if the Log Message fails validation, or if other severe error occurs while attempting to publish the message

=head3 requestDirective

C<libgmsec_perl::ConnectionManager-E<gt>requestDirective($subject, $directiveString, $fields)>

    Creates a Directive message, per the description in the ISD.  The message is populated with the standard fields for this connection, as well as required versioning and message description information as described in the GMSEC Interface Specification Document.

    This function then invokes a publish call to place that message on the GMSEC bus.  This implicitly sets the message response flag to false, so that receivers of the message will know a response is not necessary.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - subject on which to publish the message
    $directiveString - a field containing the string directive that this message is intended to convey
    $fields - a FieldList object containing supplemental fields that the user wishes to include with the diractive message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized
    Exception if the Log Message fails validation, or if other severe error occurs while attempting to publish the message

=head3 requestDirective

C<libgmsec_perl::ConnectionManager-E<gt>requestDirective($subject, $directiveString, $fields, $timeout, republish_ms)>

    Creates a Directive message, per the description in the ISD.  The message is populated with the standard fields for this connection, as well as required versioning and message description information as described in the GMSEC Interface Specification Document.

    This function then invokes a request call to place that message on the GMSEC bus.  This implicitly sets the message response flag to true, so that receivers of the message will know a response is necessary.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - subject on which to publish the message
    $directiveString - a field containing the string directive that this message is intended to convey
    $fields - a FieldList object containing supplemental fields that the user wishes to include with the directive message.
    $timeout - the time to wait before a response to the message will no longer be routed to the supplied callback
    $republish_ms - request message resubmission interval (in milliseconds). If set to a negative value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0, the period will default to 60000ms, unless the user has provided an alternate time period via the Config object used to create the Connection object.  The minimum republish period allowed is 100ms.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A pointer to a message received in response to the directive request message.  This message must be released with release($message).  This function will return NULL if a message is not received in the designated period.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized
    Exception if the Log Message fails validation, or if other severe error occurs while attempting to publish the message

=head3 acknowledgeDirectiveRequest

C<libgmsec_perl::ConnectionManager-E<gt>::acknowledgeDirectiveRequest($subject, $request, $responseStatus, $fields)>

    Acknowledges a Directive message, per the description in the ISD.  The message is populated with the standard fields for this connection, as well as required versioning and message description information as desribed in the GMSEC Interface Specification Document. This function then invokes reply($request, $reply) to place that message on the GMSEC bus and route it to the original requestor.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - the subject on which to publish the message
    $request - the original directive message
    $responseStatus - an enumeration specifying the response code to the directive request
    $fields - a FieldList objet containing supplemental information fields which may be included in the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized
    Exception if the message cannot be validated, or if other severe error occurs while attempting to publish message.

=head3 publishResourceMessage

C<libgmsec_perl::ConnectionManager-E<gt>publishResourceMessage($subject, $sampleInterval, $averageInterval)>

    Creates a valid resource message, as described in the ISD and publishes the message to the GMSEC bus on the supplied subject.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - subject on which to publish the message
    $sampleInterval - distance between samples (in seconds) (default is 1)
    $averageInterval - time interval over which to make a moving average of samples (in seconds) (default is 10)

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized.
    Exception if a severe error occurs while attempting to publish the message.

=head3 createresourceMessage

C<libgmsec_perl::ConnectionManager-E<gt>createResourceMessage($subject, $sampleInterval, $averageInterval)>

    Creates a valid resource message, as described in the GMSEC specification, and returns that message to the user.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - subject with which to create the message
    $sampleInterval - distance between samples (in seconds) (default is 1)
    $averageInterval - time interval over which to make a moving average of samples (in seconds) (default is 10)

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A C2CX resource message

=head3 startResourceMessageService

C<libgmsec_perl::ConnectionManager-E<gt>startResourceMessageService($subject, $intervalSeconds, $sampleInterval, $averageInterval)>

    Starts the resource message service, which publishes resource messages at a fixed interval as defined by this function.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - subject with which to create the message
    $intervalSeconds - the interval where messages are published to the bus (in seconds) (default is 60)
    $sampleInterval - the time distance between samples (in seconds) (default is 1)
    $averageInterval - time window over which to make a moving average of samples (in seconds) (default is 10)

=head3 stopResourceService

C<libgmsec_perl::ConnectionManager-E<gt>stopResourceMessageService()>

    Stops the resource message service

=for html $nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    Returns 1 if the resource message service was running before invocation of this function, 0 otherwise.

=head3 acknowledgeSimpleService

C<libgmsec_perl::ConnectionManager-E<gt>acknowledgeSimpleService($subject, $request, $responseStatus, $fields)>
    
    Acknowledges a Simple Service message, per the description in the ISD.  The message is populated with the sandard fields for this connection, as well as required versioning and message description information as described by the ISD.  This function then invokes reply($request, $reply) to place that message on the GMSEC bus and route it to the original requestor.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - subject on which to publish the message
    $request - the original directive message
    $responseStatus - an enumeration specifying the response code to the simple service request
    $fields - a FieldList object containing supplemental information fields which may be included with the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized
    Exception if the message cannot be validated, or if other severe error occurs while attempting to publish the message

=head3 requestSimpleService

C<libgmsec_perl::ConnectionManager-E<gt>requestSimpleService($subject, $opName, $opNumber, $fields, $params)>

    Creates a Simple Service message, per the description in the ISD.  The message is populated with the standard fields for this connection, as well as required versioning and message description information as described in the ISD.  This function then invokes publish($msg) to place that message on the GMSEC bus.  This implicitly sets the message response flag to false, so that receivers of the message will know a respnse is not necessary.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - subject on which to publish the message
    $opName - the name of the operation of the service
    $opNumber - a Field object containing the operation number for the service
    $fields - a FieldList object containing supplemental fields that the user wishes to include with the directive message
    $param - a ServiceParamList object containing ServiceParam objects providing meta data for this service invocation

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if ConnectionManager has not been initialized
    Exception if the message cannot be validated, or if other severe error occurs while attempting to publish the message

=head3 requestSimpleService

C<libgmsec_perl::ConnectionManager-E<gt>requestSimpleService($subject, $opName, $opNumber, $fields, $params, $timeout, $republish_ms)>

    Creates a Simple Service message, per the description in the ISD.  The message is populated with the standard fields for this connection, as well as required versioning and message description information as described in the ISD.  This function then invokes request($request, $timeout, $republish_ms) to place that message on the GMSEC bus.  This implicitly sets the message response flag to true, so that receivers of the message will know a response is necessary.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $subject - the subject on which to publish the message
    $opName - the name of the operation of the service
    $opNumber - a Field object containing the operation number for the service
    $fields - a FieldList object containing supplemental fields that the user wishes to include with the directive message
    $param - a ServiceParamList object containing the ServiceParam objects providing meta data for this service invocation
    $republish_ms - request message resubmission interval (in milliseconds). If set to a negative value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0, the period will default to 60000ms, unless the user has provided an alternate time period via the Config object used to create the Connection object.  The minimum republish period allowed is 100ms.

=cut
%}
