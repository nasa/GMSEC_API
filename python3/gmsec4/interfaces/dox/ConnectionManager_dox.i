
// File: classlibgmsec__python_1_1ConnectionManager.xml


%feature("docstring") gmsec::api::mist::ConnectionManager "

    This class wraps the GMSEC Connection, supplying both normal
    middleware interaction methods and some additional quality of
    service.
    This is accomplished through standard services for producing Log
    and Heartbeat messages, as well as functionality for validating
    messages.

    If ConnectionManager is configured to validate messages, all messages sent
    from this ConnectionManager via either Publish or Request will be
    compared to an available schema definition. If a template
    definition for the message in question is available, it will be
    validated. The message will not be sent over the middleware if
    validation fails. Messages without a template definition can be
    sent as well.

    See Also
    --------
    Config


    CONSTRUCTORS:

    ConnectionManager(cfg: Config, validate=False: bool, version=libgmsec_python3.GMSEC_ISD_CURRENT: int)

    Initializes the ConnectionManager instance without attempting a
    connection to the middleware.

    Note:
    Message validation will be disabled, unless otherwise indicated. If
    enabled, or if the configuration option GMSEC-MSG-CONTENT-VALIDATE
    is set to true, then messages will be validated against the default
    version of the GMSEC Interface Specification Document (ISD). The
    version of the ISD can be changed by specifying the configuration
    option GMSEC-SPECIFICATION-VERSION to a valid value (e.g. 201400).

    Parameters
    ----------
    cfg      : The Config used to construct the underlying Connection
    validate : Boolean flag to indicate whether the ConnectionManager
               should validate messages (default is False)
    version  : The version of the GMSEC message specification
               to be used in validating messages (default is GMSEC_ISD_CURRENT)

    Exceptions
    ----------
    A GmsecError is thrown if the configuration information cannot be
    used to deduce a Connection type, or if an anomaly occurs while
    loading schemas for the specified ISD.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::requestDirective "

    request_directive(self, subject: str, directiveString: StringField, fields: FieldList)

    This method creates a Directive message, per the description in
    the GMSEC interface specification document. The message is
    populated with the standard fields for this connection, as well as
    required versioning and message description information as
    described in the GMSEC interface specification document.

    This method then invokes publish(Message msg) to place that
    message on the GMSEC bus. This implicitly sets the message's
    response flag to false, so that receivers of the message will
    know a response is not necessary

    Parameters
    ----------
    subject         : Subject on which to publish the message
    directiveString : StringField containing the string directive that this
                      message is intended to convey
    fields          : A FieldList of and supplemental fields that the user wishes to
                      include with the directive message.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the message cannot be validated, or if
        other severe error occurs while attempting to publish the message.



    request_directive(self, subject: str, directiveString: StringField, fields: FieldList,
                            timeout: int, cb: ConnectionManagerReplyCallback, republish_ms=0: int)

    This method creates a Directive message, per the description in
    the GMSEC interface specification document. The message is
    populated with the standard fields for this connection, as well as
    required versioning and message description information as
    described in the GMSEC interface specification document.

    This method then invokes request() to place that message on the
    GMSEC bus. This implicitly sets the message's response flag to
    true, so that receivers of the message will know a response is
    necessary.

    Parameters
    ----------
    subject         : Subject on which to publish the message
    directiveString : StringField containing the string directive that this message is intended to convey
    fields          : A FieldList of and supplemental fields that the user wishes to
                      include with the directive message.
    timeout         : The time to wait before a response to the message will no
                      longer be routed to the supplied callback
    cb              : User-defined ConnectionManagerReplyCallback to be invoked upon reception of a
                      response message
    republish_ms    : Request message resubmission interval (in milliseconds). If set to a negative
                      value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.
                      If set to 0, the period will default to 60000ms, unless the user has provided an
                      alternate time period via the Config used to create the Connection. The minimum
                      republish period allowed is 100ms.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the message cannot be validated, or if
        other severe error occurs while attempting to publish the message.



    request_directive(self, subject: str, directiveString: StringField, fields: FieldList,
                            timeout: int, republish_ms=0: int) -> Message

    This method creates a Directive message, per the description in
    the GMSEC interface specification document. The message is
    populated with the standard fields for this connection, as well as
    required versioning and message description information as
    described in the GMSEC interface specification document.

    This method then invokes request() to place that message on the
    GMSEC bus. This implicitly sets the message's response flag to
    true, so that receivers of the message will know a response is
    necessary.

    Parameters
    ----------
    subject         : Subject on which to publish the message
    directiveString : StringField containing the string directive that this message is intended to convey
    fields          : A FieldList of and supplemental fields that the user wishes to
                      include with the directive message.
    timeout         : The time to wait before a response to the message will no
                      longer be routed to the supplied callback
    republish_ms    : Request message resubmission interval (in milliseconds). If set to a negative
                      value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.
                      If set to 0, the period will default to 60000ms, unless the user has provided an
                      alternate time period via the Config used to create the Connection. The minimum
                      republish period allowed is 100ms.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the message cannot be validated, or if
        other severe error occurs while attempting to publish the message.

    Returns
    -------
    A message received in response to the directive request
    message. This message must be released with release(msg).
    This method will return NULL of a message is not received
    in the proscribed period.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::createHeartbeatMessage "

    create_heartbeat_message(self, subject: str, heartbeatFields: FieldList) -> Message

    This method creates a message and passes ownership to the user.
    This message is populated with the standard set of required and
    optional heartbeat fields, as well as the required common fields
    defined in set_standard_fields().

    MESSAGE-TYPE, MESSAGE-SUBTYPE, and if applicable, C2CX-SUBTYPE fields will all be
    generated and added to the message automatically, according to the
    C2MS message standard.

    Parameters
    ----------
    subject         : the topic (subject) that will be applied to the returned message.
    heartbeatFields : FieldList of fields to be added to the heartbeat message.

    Returns
    -------
    A C2CX Heartbeat Message

    Exceptions
    ----------
    A GmsecError is thrown if the subject is NULL.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::addStandardFields "

    add_standard_fields(self, msg: Message)

    Adds the standard fields (if any) to the given Message.

    Parameters
    ----------
    msg : A Message

    See Also
    --------
    set_standard_fields()
    MistMessage.set_standard_fields()
";

%feature("docstring") gmsec::api::mist::ConnectionManager::registerEventCallback "

    register_event_callback(self, event: Connection.ConnectionEvent, cb: ConnectionManagerEventCallback)

    This is a pass-through method to the underlying connection.

    Parameters
    ----------
    event : Type of event to register
    cb    : User-defined class derived from ConnectionManagerEventCallback to register for this error event

    Exceptions
    ----------
    A GmsecError is thrown if the callback is None.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::publish "

    publish(self, msg: Message)

    If this connection manager has been created with \"validate\"
    option disabled, this is a pass-through method to the underlying
    connection. Otherwise the message will be validated before it is
    published.

    Parameters
    ----------
    msg : The Message to be published.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the message fails validation, or if
        any other severe error occurs with sending the message.



    publish(self, msg: Message, mwConfig: Config)

    Publishes the given message to the middleware
    using the given configuration to enable or disable certain middleware-level
    publish functionalities (e.g. ActiveMQ - Durable Producer). Message will still
    be validated if message validation is enabled.

    Note: The actual Message published to the middleware will contain
    tracking fields; to disable this feature, create a ConnectionManager
    with the tracking=off configuration option.

    Parameters
    ----------
    msg      : The Message to be published
    mwConfig : Config object for providing middleware configuration options

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the message fails validation, or if any other severe error occurs with sending the message.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::stopAutoDispatch "

    stop_auto_dispatch(self, waitForCompletion=True: bool) -> bool

    This method will stop the auto dispatch thread.

    Parameters
    ----------
    waitForCompletion : If set to False, this method will not block;
                        otherwise the method will block until the
                        auto-dispatch thread has completed running.

    Returns
    -------
    True is returned if the auto-dispatcher was running and
    has been stopped; False otherwise. If the waitForCompletion
    flag is set to False, then users can expect a return value of False.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.

    See Also
    --------
    ConnectionManager::start_auto_dispatch()
";

%feature("docstring") gmsec::api::mist::ConnectionManager::acknowledgeSimpleService "

    acknowledge_simple_service(self, subject: str, request: message,
                                     ssResponse: ResponseStatus::Response, fields: FieldList)

    This method acknowledges a Simple Service message, per the
    description in the GMSEC interface specification document. The
    message is populated with the standard fields for this connection,
    as well as required versioning and message description information
    as described in the GMSEC interface specification document.

    This method then invokes reply() to place that message on the
    GMSEC bus and route it to the original requestor

    Parameters
    ----------
    subject    : Subject on which to publish the message
    request    : The original directive Message
    ssResponse : An enumeration specifying the response code to the simple service request.
    fields     : FieldList of supplemental information fields which may be included with the message

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the message cannot be validated, or if other severe
        error occurs while attempting to publish the message.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::startResourceMessageService "

    start_resource_message_service(self, subject: str, intervalSeconds=60: int,
                                         sampleInterval=1: int, averageInterval=10: int)

    This method starts the resource message service, which publishes
    resource messages at a fixed interval as defined by this method.

    Parameters
    ----------
    subject         : Subject with which to create the message
    intervalSeconds : The time interval, in seconds, where messages are published to the bus.
    sampleInterval  : The time distance between samples (in seconds).
    averageInterval : The time window over which to make a moving average of samples (in seconds)
";

%feature("docstring") gmsec::api::mist::ConnectionManager::dispatch "

    dispatch(self, msg: Message)

    Attempts to dispatch the given message to callbacks that have been previously registered.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.

    Parameters
    ----------
    msg : Message to be dispatched
";

%feature("docstring") gmsec::api::mist::ConnectionManager::requestSimpleService "

    requestSimpleService(self, subject: str, opName: str, opNumber: Field,
                               fields: FieldList, sParams: ServiceParamList)

    This method creates a Simple Service message, per the description
    in the GMSEC interface specification document. The message is
    populated with the standard fields for this connection, as well as
    required versioning and message description information as
    described in the GMSEC interface specification document.

    This method then invokes request() to place that message on the
    GMSEC bus. This implicitly sets the message's response flag to
    false, so that receivers of the message will know a response is
    not necessary.

    Parameters
    ----------
    subject  : Subject on which to publish the message
    opName   : The name of the operation of the service
    opNumber : The operation number Field for the service message is intended to convey
    fields   : FieldList of supplemental fields that the user wishes to include with the directive message.
    sParams  : ServiceParamList of the ServiceParam providing meta data for this service invocation

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the message cannot be validated, or if other severe error occurs while attempting to send the message.



    requestSimpleService(self, subject, opName: str, opNumber: Field,
                               fields: FieldList, sParams: ServiceParamList, timeout: int,
                               cb: ConnectionManagerReplyCallback, republish_ms=0: int)

    This method creates a Simple Service message, per the description
    in the GMSEC interface specification document. The message is
    populated with the standard fields for this connection, as well as
    required versioning and message description information as
    described in the GMSEC interface specification document.

    This method then invokes request() to place that message on the
    GMSEC bus. This implicitly sets the message's response flag to
    true, so that receivers of the message will know a response is
    necessary.

    Parameters
    ----------
    subject      : Subject on which to publish the message
    opName       : The name of the operation of the service
    opNumber     : The operation number Field for the service message is intended to convey
    fields       : FieldList of supplemental fields that the user wishes to include with the directive message
    sParams      : ServiceParamList of the ServiceParams providing meta data for this service invocation
    timeout      : The time to wait before a response to the message will no longer be routed to the supplied
                   callback
    cb           : User-defined ConnectionManagerReplyCallback to be invoked upon reception of a response
                   message
    republish_ms : Request message resubmission interval (in milliseconds). If set to a negative value
                   (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message. If set to 0,
                   the period will default to 60000ms, unless the user has provided an alternate time period
                   via the Config used to create the ConnectionManager. The minimum republish period allowed
                   is 100ms.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the message cannot be validated, or if other severe
        error occurs while attempting to publish the message.



    requestSimpleService(self, subject: str, opName: str, opNumber: Field,
                               fields: FieldList, sParams: ServiceParamList, timeout: int,
                               republish_ms=0: int) -> Message

    This method creates a Simple Service message, per the description
    in the GMSEC interface specification document. The message is
    populated with the standard fields for this connection, as well as
    required versioning and message description information as
    described in the GMSEC interface specification document.

    This method then invokes request() to place that message on the
    GMSEC bus. This implicitly sets the message's response flag to
    true, so that receivers of the message will know a response is
    necessary.

    Parameters
    ----------
    subject      : Subject on which to publish the message
    opName       : The name of the operation of the service
    opNumber     : The operation number Field for the service message is intended to convey
    fields       : FieldList of supplemental fields that the user wishes to include with the directive message
    sParams      : ServiceParamList of the ServiceParams providing meta data for this service invocation
    timeout      : The time to wait before a response to the message will no longer be routed to the supplied
                   callback
    republish_ms : Request message resubmission interval (in milliseconds). If set to a negative value
                   (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message. If set to 0,
                   the period will default to 60000ms, unless the user has provided an alternate time period
                   via the Config used to create the ConnectionManager. The minimum republish period allowed
                   is 100ms.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the message cannot be validated, or if other severe error occurs while
        attempting to publish the message.

    Returns
    -------
    A message received in response to the directive request message. This message must be released with
    release(msg). This method will return NULL of a message is not received in the proscribed period.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::createResourceMessage "

    create_resource_message(self, subject: src, sampleInterval: int, averageInterval: int)-> Message

    This method creates a valid resource message, as described in
    the GMSEC specification, and returns that message to the user.

    Parameters
    ----------
    subject         : Subject with which to create the message
    sampleInterval  : Distance between samples (in seconds)
    averageInterval : Time interval over which to make a moving average of samples (in seconds)

    Returns
    ----------
    A resource Message.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::getLibraryVersion "

    get_library_version(self) -> str

    This method returns a string containing the version information
    for this connection's associated middleware.

    Returns
    -------
    library version
";

%feature("docstring") gmsec::api::mist::ConnectionManager::subscribe "

    subscribe(self, subject: str, cb=None: ConnectionManagerCallback) -> SubscriptionInfo

    This method subscribes to a particular subject or pattern and if
    provided. If a callback is given, then it will be associated with
    the subscription. If all subscriptions are setup to use a callback,
    then the auto-dispatcher should be used to asynchronously receive
    messages. An alternative is to use receive() to receive messages,
    and then to forward them to callbacks using dispatch().

    Parameters
    ----------
    subject : Subject pattern to match received messages
    cb      : User-defined ConnectionManagerCallback to be called when message is received (optional)

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the subject string is None, or is an empty string.



    subscribe(self, subject: str, config: Config, cb=None: ConnectionManagerCallback) -> SubscriptionInfo

    This method subscribes to a particular subject or pattern and uses
    the provided Config to enable or disable middleware-level
    subscription functionalities (e.g. ActiveMQ Durable Consumer).
    If a callback is given, then it will be associated with the
    subscription.
    If all subscriptions are setup to use a callback, then the
    auto-dispatcher should be used to asynchronously receive messages.
    An alternative is to use receive() to receive messages, and then to
    forward them to callbacks using dispatch().

    Parameters
    ----------
    subject : Subject pattern to match received messages
    config  : Config to be used for subscription operation
    cb      : User-defined ConnectionManagerCallback to be called when message is received (optional)

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the subject string is None, or is an empty string.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::startAutoDispatch "

    start_auto_dispatch(self) -> bool

    This method will start a thread that will dispatch messages
    asynchronously when they are received. If this is used, all
    subscriptions must be made with callbacks or the messages will
    be dropped. If receive() is called while the auto-dispatcher
    is used, the behavior will be undesireable and undefined.

    Returns
    -------
    True of the dispatcher started successfully; false otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.

    See Also
    --------
    ConnectionManager::subscribe()
    ConnectionManager::stop_auto_dispatch()
";

%feature("docstring") gmsec::api::mist::ConnectionManager::request "

    request(self, request: Message, timeout: int, cb: ConnectionManagerReplyCallback, republish_ms=0: int)

    If this connection manager has been created with \"validate\"
    option disabled, this is a pass-through method to the underlying
    connection. Otherwise the request message will be validated before
    it is issued.

    Parameters
    ----------
    request      : Request Message to send
    timeout      : Maximum time to wait for reply (in milliseconds)
    cb           : User-define ConnectionManagerReplyCallback to call when reply is received
    republish_ms : Request message resubmission interval (in milliseconds). If set to a negative value
                   (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message. If set to 0,
                   the period will default to 60000ms, unless the user has provided an alternate time period
                   via the Config used to create the ConnectionManager. The minimum republish period allowed
                   is 100ms.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the request message fails validation,
        or if any other severe error occurs with issuing the request.

    See Also
    --------
    cancel_request()



    request(self, request: Message, timeout: int, republish_ms=0: int) -> Message

    If this connection manager has been created with \"validate\"
    option disabled, this is a pass-through method to the underlying
    connection.
    Otherwise the request message will be validated before it is issued.

    Parameters
    ----------
    request      : Request Message to send
    timeout      : Maximum time to wait for reply (in milliseconds)
    republish_ms : Request message resubmission interval (in milliseconds). If set to a negative value
                   (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message. If set to 0,
                   the period will default to 60000ms, unless the user has provided an alternate time period
                   via the Config used to create the ConnectionManager. The minimum republish period allowed
                   is 100ms.

    Returns
    -------
    reply - Reply Message (if received), or None if timeout occurs.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.

    See Also
    --------
    ConnectionManager::release
";

%feature("docstring") gmsec::api::mist::ConnectionManager::acknowledgeDirectiveRequest "

    acknowledgeDirectiveRequest(self, subject: str, request: Message,
                                      ssResponse: ResponseStatus:Response, fields: FieldList)

    This method acknowledges a Directive message, per the description
    in the GMSEC interface specification document. The message is
    populated with the standard fields for this connection, as well as
    required versioning and message description information as
    described in the GMSEC interface specification document.

    This method then invokes reply() to place that message on the
    GMSEC bus and route it to the original requestor.

    Parameters
    ----------
    subject    : Subject on which to publish the message
    request    : The request directive Message
    ssResponse : An enumeration specifying the response code to the directive request
    fields     : FieldList of supplemental information fields which may be included with the message

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the message cannot be validated, or if other severe error occurs while
        attempting to send the message.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::cancelRequest "

    cancel_request(self, cb: ConnectionManagerReplyCallback)

    This method can be used to cancel a pending request that is
    associated with the given ConnectionManagerReplyCallback.

    Parameters
    ----------
    cb : User-defined ConnectionManagerReplyCallback to disassociate from a pending request.

    See Also
    --------
    request(request, timeout, cb, republish_ms=0)

    Note
    --------
    This method is currently not functional.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::reply "

    reply(self, request: Message, reply: Message)

    If this connection manager has been created with \"validate\"
    option disabled, this is a pass-through method to the underlying
    connection. Otherwise the request message will be validated before
    it is issued.

    Parameters
    ----------
    request : The pending request Message
    reply   : The reply Message to be sent

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.

    See Also
    --------
    ConnectionManager::request
";

%feature("docstring") gmsec::api::mist::ConnectionManager::stopResourceMessageService "

    stop_resource_message_service(self) -> bool

    This method stops the resource message service

    Returns
    -------
    Returns true if the resource message service was running before
    invocation of this method, false otherwise.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::getSpecification "

    get_specification(self) -> Specification

    Returns the Specification associated with the Connection Manager.

    Returns
    -------
    A Specification.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::setSpecification "

    set_specification(self, spec: Specification) -> void

    This method will allow for a user to register their custom subclass of the
    Specification class with the Connection Manager. This custom Specification can
    implement its own validateMessage() method which can be used to perform validation
    of messages currently not performed by the GMSEC API.

    Note: The API does not assume ownership of the provided Specification object, nor
    does it make a copy of such. The user is responsible to ensure that the provided
    Specification object is not destroyed while the ConnectionManager is in possession
    of such.

    Parameters
    ----------
    spec: A specialized subclass of the Specification class.

    Exceptions
    ----------
    A GmsecError is thrown if the given Specification is null.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::setStandardFields "

    set_standard_fields(self, standardFields: FieldList)

    Sets the internal list of fields that are added to all messages
    created using the ConnectionManager. Internal
    copies of the Fields are made, thus ownership is not retained by the
    ConnectionManager. The supplied set of fields will not be validated
    here; validation occurs at the time a message is to be published.

    Parameters
    ----------
    standardFields : FieldList of fields to be set as standard fields

";

%feature("docstring") gmsec::api::mist::ConnectionManager::publishResourceMessage "

    publish_resource_message(self, subject: str, sampleInterval=1: int, averageInterval=10: int)

    This method creates a valid resource message, as described in the
    GMSEC specification. This method then publishes the message to
    the GMSEC bus on the supplied subject.

    Parameters
    ----------
    subject         : Subject on which to publish the message
    sampleInterval  : Distance between samples (in seconds)
    averageInterval : Time interval over which to make a moving average of samples (in seconds)

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if a severe error occurs while attempting to publish the message.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::excludeSubject "

    exclude_subject(self, subject: str)

    Exclude any incoming messages with the specified subject.

    Parameters
    ----------
    subject : The subject pattern to match when excluding incoming messages.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the subject is None, or contains an empty string.

    See Also
    --------
    ConnectionManager::subscribe()
    ConnectionManager::remove_excluded_subject()
";

%feature("docstring") gmsec::api::mist::ConnectionManager::stopHeartbeatService "

    stop_heartbeat_service(self)

    This method terminates opertaion of the heartbeat service and
    cleans up related resources. It will be called by the global
    connection manager \"cleanup\" if not done manually by the user.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::publishLog "

    publish_log(self, logMessage: Message, severity: I16Field)

    This method creates a log message comprised of the global fields
    from the entire ConnectionManager, the fields set in
    set_logging_defaults(), and the log text and severity fields as
    supplied as arguments to this method. If validation is enabled,
    the message will be published. If validation is enabled, the
    message will be validated.  If validation of the message fails,
    then a GmsecError is thrown.

    This method automatically populates the message with the correct
    MESSAGE-TYPE and MESSAGE-SUBTYPE fields. EVENT-TIME is set
    automatically to the time of method invocation.

    Parameters
    ----------
    logMessage : The Message to log
    severity   : The I16Field defining the severity of the log message.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the Log Message cannot be validated, or if other
        severe error occurs while attempting to send the message.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::createLogMessage "

    create_log_message(self, subject: str, logFields: FieldList) -> Message

    This method creates a Log Message and passes ownership to the user.
    This message is populated with the standard set of required and
    optional log fields, as well as the required common fields defined
    in set_standard_fields(). The message is not validated at this time,
    as MSG-TEXT and SEVERITY fields must be set by the user at the time
    the message is to be sent.

    This message automatically is generated with MESSAGE-TYPE and
    MESSAGE-SUBTYPE correctly set to the GMSEC Message Standard
    requirements, regardless of user input. EVENT-TIME defaults to the
    time which this method was invoked; the user may override this
    time once the message has been returned.

    Parameters
    ----------
    subject   : The topic (subject) to apply to the returned message.
    logFields : FieldList of fields to be added to the log message.

    Returns
    -------
    A Log Message.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::setHeartbeatServiceField "

    set_heartbeat_service_field(self, field: Field) -> Status

    This method can be used to update or set a field within the
    Heartbeat Message being published by the Heartbeat Service. If
    validation is enabled, the field will be validated before it is
    added to the Heartbeat Message.  Should validation pass, the field
    will be persisted to the heartbeat service; otherwise an error is
    returned.

    Parameters
    ----------
    field : The Field containing new or updated information for the Heartbeat Message

    Returns
    -------
    The status of the operation.

    Note
    -------
    If a (valid) PUB-RATE field is passed to this method, and
    the Heartbeat Service is running, then the Heartbeat Service
    publish rate will be changed to the provided rate. Note that a
    publish rate of 0 seconds or less will be treated as an error.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::setLoggingDefaults "

    set_logging_defaults(self, subject: str, logFields: FieldList)

    This method sets the standard default fields which will be added to
    a new log message when publish_log() is called.

    Parameters
    ----------
    subject   : Subject to use for log messages
    logFields : FieldList of fields to be added to log messages.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::removeExcludedSubject "

    remove_excluded_subject(self, subject: str)

    Remove an excluded subject, allowing incoming messages with the matching subject to once again be received.

    Parameters
    ----------
    subject : The subject pattern to remove.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
    A GmsecError is thrown if the subject is None, or is an empty string.

    See Also
    --------
    ConnectionManager::exclude_subject()
";

%feature("docstring") gmsec::api::mist::ConnectionManager::receive "

    receive(self, timeout: int) -> Message

    This method is a pass-through method to the underlying
    connection. This method returns the next message received within
    the specified timeout. The received messages are determined by the
    subscribe() method(s), but dispatch() needs to be called
    messages received from this method to ensure all registered
    callbacks are executed.

    This method MUST NOT BE USED if the auto-dispatcher is being used.

    Parameters
    ----------
    timeout : The maximum time to block waiting for a message, in milliseconds.
              Use -1 to block indefinitely.

    Returns
    -------
    The next received message, if any. If a timeout occurs, None is returned.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.

    See Also
    --------
    ConnectionManager::subscribe
    ConnectionManager::release
";

%feature("docstring") gmsec::api::mist::ConnectionManager::release "

    release(self, msg: Message)

    This method destroys a message and cleans up any associated memory.

    Parameters
    ----------
    msg : The Message to be destroyed

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.

    See Also
    --------
    ConnectionManager::receive()
";

%feature("docstring") gmsec::api::mist::ConnectionManager::unsubscribe "

    unsubscribe(self, info: MIST_SubscriptionInfo)

    This method unsubscribes to a particular subject or pattern, and
    will stop the reception or messages that match this pattern. It
    will also remove the registration of any callbacks with this
    subject pattern.

    Parameters
    ----------
    info : MIST_SubscriptionInfo to deregister

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been initialized.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::cleanup "

    cleanup(self)

    This method disconnects and destroys the underlying Connection,
    returning an error status if this operation is not successful.

    Exceptions
    ----------
    A GmsecError is thrown if an anomaly occurs while disconnecting

    See Also
    --------
    ConnectionManager.initialize()
";

%feature("docstring") gmsec::api::mist::ConnectionManager::initialize "

    initialize(self)

    Establishes a connection with the GMSEC middleware server. Once
    this call successfully returns, the ConnectionManager is ready
    for message operations.

    Exceptions
    ----------
    A GmsecError is thrown if an anomaly occurs while connecting to
    the middleware server.

    See Also
    --------
    ConnectionManager.cleanup()
";

%feature("docstring") gmsec::api::mist::ConnectionManager::startHeartbeatService "

    start_heartbeat_service(self, subject: subject, heartbeatFields: FieldList)

    This method creates an archetypal heartbeat message from the
    concatenation of the standard fields applied globally to this
    ConnectionManager and to this method. This message will then be
    validated, if the \"validate\" flag is set. If there is an error,
    a GmsecError will be thrown. If it has not been set, the message
    will be published at an interval supplied by the \"PUB-RATE\" field
    regardless of validation results. If no \"PUB-RATE\" has been
    defined, the service will default to the GMSEC standard 30 second
    heartbeat interval.

    MESSAGE-TYPE, MESSAGE-SUBTYPE, and if applicable, C2CX-SUBTYPE fields will all be
    generated and added to the message automatically, according to the
    C2MS Message Standard

    Parameters
    ----------
    subject         : Subject to set as the default for heartbeat messages
    heartbeatFields : FieldList of fields to be added to the published heartbeat messages

    Exceptions
    ----------
    A GmsecError is thrown if the subject is NULL, or is an empty string.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::getMWInfo "

    get_MW_info(self) -> str

    Returns a string containing the middleware information for the underlying Connection.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::getConnectionEndpoint "

    get_connection_endpoint(self) -> str

    Returns middleware broker connection information.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::getID "

    get_id(self) -> str

    Returns the string ID for the underlying Connection.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::getLibraryRootName "

    get_library_root_name(self) -> str

    Returns the root library name and therefore the connection type that the
    underlying Connection is associated with.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::getName "

    get_name(self) -> str

    Returns the name of the underlying Connection, which was automatically
    generated or user supplied.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::setName "

    set_name(self, name: str)

    Sets the logical name of the underlying Connection. This can be used for
    identifying connections within a client program.

    Parameters
    ----------
    name : The name of the connection

    Exceptions
    ----------
    A GmsecError is thrown if the given name is None.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::getPublishQueueMessageCount "

    get_publish_queue_message_count(self) -> int

    Returns the number of messages queue for asynchronous publish operations.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::getStandardFields "

    get_standard_fields(self) -> FieldList

    Returns a FieldList containing the standard fields, if any, that have been
    associated with the ConnectionManager.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::getState "

    get_state(self) -> Connection::ConnectionState

    Returns the current state of the connection to the middleware.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::getAPIVersion "

    get_API_version(self) -> str

    Returns a string containing the API version information.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::shutdownAllMiddlewares "

    shutdown_all_middlewares(self)

    Calls shutdown routines for each middleware that has a shutdown routine registered.
	This method should only be called once, typically just before an application terminates. 
	Note the calling of this method is optional.
";
