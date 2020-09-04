
// File: classlibgmsec__python_1_1ConnectionManager.xml


%feature("docstring") gmsec::api::mist::ConnectionManager "

    This class wraps the GMSEC Connection object, supplying both normal
    middleware interaction functions and some additional quality of
    service.
    This is accomplished through standard services for producing Log
    and Heartbeat messages, as well as functionality for validating
    messages.

    If the object is configured to validate messages, all messages sent
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

    ConnectionManager(self, cfg, validate, version)

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
    cfg: The config object used to construct the underlying GMSEC
    connection object
    validate (optional): boolean flag to indicate whether the
    ConnectionManager should validate messages (default is false)
    version (optional): the version of the GMSEC message specification
    to be used in validating messages (default is GMSEC_ISD_CURRENT)

    Exceptions
    ----------
    A GmsecError is thrown if the configuration information cannot be
    used to deduce a Connection type, or if an anomaly occurs while
    loading schemas for the specified ISD.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::requestDirective "

    requestDirective(self, subject, directiveString, fields)

    This function creates a Directive message, per the description in
    the GMSEC interface specification document. The message is
    populated with the standard fields for this connection, as well as
    required versioning and message description information as
    described in the GMSEC interface specification document.

    This function then invokes publish(Message msg) to place that
    message on the GMSEC bus. This implicitly sets the message's
    response flag to false, so that receivers of the message will
    know a response is not necessary

    Parameters
    ----------
    subject: subject on which to publish the message
    directiveString: a field containing the string directive that this
        message is intended to convey
    fields: a list of and supplemental fields that the user wishes to
        include with the directive message.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
    initialized.
    A GmsecError is thrown if the message cannot be validated, or if
    other severe error occurs while attempting to publish the message.

    requestDirective(self, subject, directiveString, fields,
                           timeout, cb, republish_ms=0)

    This function creates a Directive message, per the description in
    the GMSEC interface specification document. The message is
    populated with the standard fields for this connection, as well as
    required versioning and message description information as
    described in the GMSEC interface specification document.

    This function then invokes request() to place that message on the
    GMSEC bus. This implicitly sets the message's response flag to
    true, so that receivers of the message will know a response is
    necessary.

    Parameters
    ----------
    subject: subject on which to publish the message
    directiveString: a field containing the string directive that this
        message is intended to convey
    fields: a list of and supplemental fields that the user wishes to
        include with the directive message.
    timeout: The time to wait before a response to the message will no
        longer be routed to the supplied callback
    cb: the callback to be invoked upon reception of a response message
    republish_ms - request message resubmission interval (in
        milliseconds). If set to a negative value (eg.
        GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a
        request message.  If set to 0, the period will default to
        60000ms, unless the user has provided an alternate time
        period via the Config object used to create the Connection
        object.  The minimum republish period allowed is 100ms.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.
    A GmsecError is thrown if the message cannot be validated, or if
        other severe error occurs while attempting to publish the
        message.

    requestDirective(self, subject, directiveString, fields, timeout, republish_ms=0) -> Message

    This function creates a Directive message, per the description in
    the GMSEC interface specification document. The message is
    populated with the standard fields for this connection, as well as
    required versioning and message description information as
    described in the GMSEC interface specification document.

    This function then invokes request() to place that message on the
    GMSEC bus. This implicitly sets the message's response flag to
    true, so that receivers of the message will know a response is
    necessary.

    Parameters
    ----------
    subject: subject on which to publish the message
    directiveString: a field containing the string directive that this
        message is intended to convey
    fields: a list of and supplemental fields that the user wishes to
        include with the directive message.
    timeout: The time to wait before a response to the message will
        no longer be routed to the supplied callback
    republish_ms - request message resubmission interval (in
        milliseconds). If set to a negative value (eg.
        GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a
        request message.  If set to 0, the period will default to
        60000ms, unless the user has provided an alternate time period
        via the Config object used to create the Connection object.
        The minimum republish period allowed is 100ms.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.
    A GmsecError is thrown if the message cannot be validated, or if
        other severe error occurs while attempting to publish the
        message.

    Returns
    -------
    A message received in response to the directive request
    message. This message must be released with release(msg).
    This function will return NULL of a message is not received
    in the proscribed period.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::createHeartbeatMessage "

    createHeartbeatMessage(self, subject, heartbeatFields) -> Message

    This method creates a message and passes ownership to the user.
    This message is populated with the standard set of required and
    optional heartbeat fields, as well as the required common fields
    defined in setStandardFields(). If validation is enabled for this
    ConnectionManager and neither the common fields from
    setStandardFields(), nor the fields supplied in the first argument
    of this function are sufficient to complete a set of fields
    required by validation, an error will be returned.

    MESSAGE-TYPE, MESSAGE-SUBTYPE, and C2CX-SUBTYPE fields will all be
    generated and added to the message automatically, according to the
    GMSEC Message Standard.

    Note: When the user done with the message, they should destroy it
    using release.

    Parameters
    ----------
    subject: the topic (subject) that will be applied to the returned
        messages.
    heartbeatFields: list of fields to be added to the heartbeat

    Returns
    -------
    A C2CX Heartbeat Message

    Exceptions
    ----------
    A GmsecError is thrown if the subject is NULL.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::addStandardFields "

    addStandardFields(self, msg)

    Adds the standard fields (if any) to the given Message object.

    Parameters
    ----------
    msg: A Message object

    See Also
    --------
    setStandardFields()
";

%feature("docstring") gmsec::api::mist::ConnectionManager::registerEventCallback "

    registerEventCallback(self, event, cb)

    This is a pass-through function to the underlying connection.

    Parameters
    ----------
    event: type of event to register
    cb: object derived from EventCallback to register for this error
        event

    See Also
    --------
    Connection.registerEventCallback(event, cb)

    Exceptions
    ----------
    A GmsecError is thrown if the callback is NULL.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::publish "

    publish(self, msg)

    If this connection manager has been created with \"validate\"
    option disabled, this is a pass-through function to the underlying
    connection. Otherwise the message will be validated before it is
    published.

    Parameters
    ----------
    msg: The GMSEC message to be published.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
    initialized.
    A GmsecError is thrown if the message fails validation, or if
        any other severe error occurs with sending the message.

    publish(self, msg, config)

    If this connection manager has been created with \"validate\"
    option disabled, this is a pass-through function to the underlying
    connection. Otherwise the message will be validated before it is
    published. The given configuration object is applied to the message.

    Parameters
    ----------
    msg: The GMSEC message to be published
    config: config object to be used by the publish operation

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.
    A GmsecError is thrown if the message fails validation, or if any
        other severe error occurs with sending the message.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::changeCPUMemory "

    changeCPUMemory(self, cpuMemory) -> Status

    This method updates the field which is included within the
    Heartbeat Message being published by the Heartbeat Service.  If
    validatiion is enabled, the message will then be re-validated with
    the modified field set. Should the validation pass, the change will
    be persisted to the heartbeat service, otherwise a GmsecError is
    thrown.

    Parameters
    ----------
    cpuMemory: Field containing updated information for the Heartbeat
        Message

    Returns
    -------
    The status of the operation.

    Note: This method has been deprecated.  Use
        setHeartbeatServiceField() instead.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::stopAutoDispatch "

    stopAutoDispatch(self, waitForCompletion=True) -> bool

    This function will stop the auto dispatch thread.

    Parameters
    ----------
    [optional] waitForCompletion: if set to false, this method will not
        block; otherwise the method will block until the auto-dispatch
        thread has completed running.

    Returns
    -------
    A value of true is returned if the auto-dispatcher was running and
    has been stopped; false otherwise. If the waitForCompletion flag is
    set to false, then users can expect a return value of false.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
    initialized.

    See Also
    --------
    ConnectionManager.startAutoDispatch()
";

%feature("docstring") gmsec::api::mist::ConnectionManager::acknowledgeSimpleService "

    acknowledgeSimpleService(self, subject, request, ssResponse, fields)

    This function acknowledges a Simple Service message, per the
    description in the GMSEC interface specification document. The
    message is populated with the standard fields for this connection,
    as well as required versioning and message description information
    as described in the GMSEC interface specification document.

    This function then invokes reply() to place that message on the
    GMSEC bus and route it to the original requestor

    Parameters
    ----------
    subject: subject on which to publish the message
    request: the original directive message
    ssResponse: an enumeration specifying the response code to the
        simple service request.
    fields: supplemental information fields which may be included with
        the message

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.
    A GmsecError is thrown if the message cannot be validated, or if
        other severe error occurs while attempting to publish the
        message.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::startResourceMessageService "

    startResourceMessageService(self, subject, intervalSeconds=60, sampleInterval=1, averageInterval=10)

    This function starts the resource message service, which publishes
    resource messages at a fixed interval as defined by this function.

    Parameters
    ----------
    subject: subject with which to create the message
    intervalSeconds: This the interval, in seconds, where messages are
        published to the bus.
    sampleInterval: This is the the time distance between samples (in
        seconds).
    averageInterval: time window over which to make a moving average of
        samples (in seconds)

";

%feature("docstring") gmsec::api::mist::ConnectionManager::dispatch "

    dispatch(self, msg)

    Attempts to dispatch the given message to callbacks that have been
        previously registered.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.

    Parameters
    ----------
    msg: message to be dispatched

";

%feature("docstring") gmsec::api::mist::ConnectionManager::changeComponentStatus "

    changeComponentStatus(self, componentStatus) -> Status

    This method updates the field which is included within the
    Heartbeat Message being published by the Heartbeat Service. If
    validatiion is enabled, the message will then be re-validated with
    the modified field set. Should the validation pass, the change will
    be persisted to the heartbeat service, otherwise a GmsecError is
    thrown.

    Parameters
    ----------
    componentStatus: Field containing updated information for the
        Heartbeat Message

    Returns
    -------
    The status of the operation.

    Note: This method has been deprecated.  Use
    setHeartbeatServiceField() instead.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::requestSimpleService "

    requestSimpleService(self, subject, opName, opNumber,
                               fields, sParams)

    This function creates a Simple Service message, per the description
    in the GMSEC interface specification document. The message is
    populated with the standard fields for this connection, as well as
    required versioning and message description information as
    described in the GMSEC interface specification document.

    This function then invokes request() to place that message on the
    GMSEC bus. This implicitly sets the message's response flag to
    false, so that receivers of the message will know a response is
    not necessary.

    Parameters
    ----------
    subject: subject on which to publish the message
    opName: the name of the operation of the service
    opNumber: the operation number for the service
        message is intended to convey
    fields: a list of and supplemental fields that the user wishes to
        include with the directive message.
    sParams: a list of the ServiceParam objects providing meta data
        for this service invocation

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.
    A GmsecError is thrown if the message cannot be validated, or if
        other severe error occurs while attempting to send the
        message.

    requestSimpleService(self, subject, opName, opNumber,
                               fields, sParams, timeout, cb,
                               republish_ms=0)

    This function creates a Simple Service message, per the description
    in the GMSEC interface specification document. The message is
    populated with the standard fields for this connection, as well as
    required versioning and message description information as
    described in the GMSEC interface specification document.

    This function then invokes request() to place that message on the
    GMSEC bus. This implicitly sets the message's response flag to
    true, so that receivers of the message will know a response is
    necessary.

    Parameters
    ----------
    subject: subject on which to publish the message
    opName: the name of the operation of the service
    opNumber: the operation number for the service message is
        intended to convey
    fields: a list of and supplemental fields that the user wishes
        to include with the directive message.
    sParams: a list of the ServiceParam objects providing meta data
        for this service invocation
    timeout: The time to wait before a response to the message will
        no longer be routed to the supplied callback
    cb: the callback to be invoked upon reception of a response message
    republish_ms: request message resubmission interval (in
        milliseconds). If set to a negative value (eg.
        GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a
        request message. If set to 0, the period will default to
        60000ms, unless the user has provided an alternate time period
        via the Config object used to create the Connection object. The
        minimum republish period allowed is 100ms.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.
    A GmsecError is thrown if the message cannot be validated, or if
        other severe error occurs while attempting to publish the
        message.

    requestSimpleService(self, subject, opName, opNumber,
                               fields, sParams, timeout,
                               republish_ms=0) -> Message

    This function creates a Simple Service message, per the description
    in the GMSEC interface specification document. The message is
    populated with the standard fields for this connection, as well as
    required versioning and message description information as
    described in the GMSEC interface specification document.

    This function then invokes request() to place that message on the
    GMSEC bus. This implicitly sets the message's response flag to
    true, so that receivers of the message will know a response is
    necessary.

    Parameters
    ----------
    subject: subject on which to publish the message
    opName: the name of the operation of the service
    opNumber: the operation number for the service message is intended
        to convey
    fields: a list of and supplemental fields that the user wishes to
        include with the directive message.
    sParams: a list of the ServiceParam objects providing meta data
        for this service invocation
    timeout: The time to wait before a response to the message will
        no longer be routed to the supplied callback
    republish_ms: request message resubmission interval (in
        milliseconds). If set to a negative value (eg.
        GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a
        request message. If set to 0, the period will default to
        60000ms, unless the user has provided an alternate time period
        via the Config object used to create the Connection object.
        The minimum republish period allowed is 100ms.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.
    A GmsecError is thrown if the message cannot be validated,
        or if other severe error occurs while attempting to publish
        the message.

    Returns
    -------
    A message received in response to the directive request
    message. This message must be released with release(msg). This
    function will return NULL of a message is not received in the
    proscribed period.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::createResourceMessage "

    createResourceMessage(self, subject, sampleInterval,
                                averageInterval)-> Message

    This function creates a valid resource message, as described in
    the GMSEC specification, and returns that message to the user.

    Parameters
    ----------
    subject: subject with which to create the message
    sampleInterval: distance between samples (in seconds)
    averageInterval: time interval over which to make a moving average
        of samples (in seconds)

";

%feature("docstring") gmsec::api::mist::ConnectionManager::getLibraryVersion "

    getLibraryVersion(self) -> char const *

    This function returns a string containing the version information
    for this connection's associated middleware.

    Returns
    -------
    library version

";

%feature("docstring") gmsec::api::mist::ConnectionManager::subscribe "

    subscribe(self, subject, cb=None) -> SubscriptionInfo

    This method subscribes to a particular subject or pattern and if
    provided. If a callback is given, then it will be associated with
    the subscription. If all subscriptions are setup to use a callback,
    then the auto-dispatcher should be used to asynchronously receive
    messages. An alternative is to use receive() to receive messages,
    and then to forward them to callbacks using dispatch().

    Parameters
    ----------
    subject: subject pattern to match received messages
    cb: callback to be called when message is received (optional)

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.
    A GmsecError is thrown if the subject string is NULL.

    subscribe(self, subject, config, cb=None) -> SubscriptionInfo

    This method subscribes to a particular subject or pattern and uses
    the provided config object to enable or disable middleware-level
    subscription functionalities (e.g. ActiveMQ Durable Consumer).
    If a callback is given, then it will be associated with the
    subscription.
    If all subscriptions are setup to use a callback, then the
    auto-dispatcher should be used to asynchronously receive messages.
    An alternative is to use receive() to receive messages, and then to
    forward them to callbacks using dispatch().

    Parameters
    ----------
    subject: subject pattern to match received messages
    config: config object to be used for subscription operation
    cb: callback to be called when message is received (optional)

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.
    A GmsecError is thrown if the subject string is NULL.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::startAutoDispatch "

    startAutoDispatch(self) -> bool

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
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.

    See Also
    --------
    ConnectionManager::subscribe()
    ConnectionManager::stopAutoDispatch()

";

%feature("docstring") gmsec::api::mist::ConnectionManager::request "

    request(self, request, timeout, cb, republish_ms=0)

    If this connection manager has been created with \"validate\"
    option disabled, this is a pass-through function to the underlying
    connection. Otherwise the request message will be validated before
    it is issued.

    Parameters
    ----------
    request: message to be sent
    timeout: maximum time to wait for reply (in milliseconds)
    cb: callback to call when reply is received
    republish_ms - request message resubmission interval (in
        milliseconds). If set to a negative value (eg.
        GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a
        request message.  If set to 0, the period will default to
        60000ms, unless the user has provided an alternate time period
        via the Config object used to create the Connection object. The
        minimum republish period allowed is 100ms.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.
    A GmsecError is thrown if the request message fails validation,
        or if any other severe error occurs with issuing the request.

    See Also
    --------
    cancelRequest()

    request(self, request, timeout, republish_ms=0) -> Message

    If this connection manager has been created with \"validate\"
    option disabled, this is a pass-through function to the underlying
    connection.
    Otherwise the request message will be validated before it is issued.

    Parameters
    ----------
    request: message to be sent
    timeout: maximum time to wait for reply (in milliseconds)
    republish_ms - request message resubmission interval (in
        milliseconds). If set to a negative value (eg.
        GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a
        request message.  If set to 0, the period will default to
        60000ms, unless the user has provided an alternate time period
        via the Config object used to create the Connection object. The
        minimum republish period allowed is 100ms.

    Returns
    -------
    reply - reply message (if received)

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.

    See Also
    --------
    ConnectionManager::release

";

%feature("docstring") gmsec::api::mist::ConnectionManager::acknowledgeDirectiveRequest "

    acknowledgeDirectiveRequest(self, subject, request,
                                ssResponse, fields)

    This function acknowledges a Directive message, per the description
    in the GMSEC interface specification document. The message is
    populated with the standard fields for this connection, as well as
    required versioning and message description information as
    described in the GMSEC interface specification document.

    This function then invokes reply() to place that message on the
    GMSEC bus and route it to the original requestor.

    Parameters
    ----------
    subject: subject on which to publish the message
    request: the original directive message
    ssResponse: an enumeration specifying the response code to the
        directive request.
    fields: supplemental information fields which may be included
        with the message

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.
    A GmsecError is thrown if the message cannot be validated, or
        if other severe error occurs while attempting to send
        the message.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::cancelRequest "

    cancelRequest(self, cb)

    This function can be used to cancel a pending request that is
    associated with the given ConnectionManagerReplyCallback object.

    Parameters
    ----------
    cb: The ConnectionManagerReplyCallback to disassociate from a
        pending request.

    See Also
    --------
    void request(request, timeout, cb, republish_ms = 0)

";

%feature("docstring") gmsec::api::mist::ConnectionManager::reply "

    reply(self, request, reply)

    If this connection manager has been created with \"validate\"
    option disabled, this is a pass-through function to the underlying
    connection. Otherwise the request message will be validated before
    it is issued.

    Parameters
    ----------
    request: the pending request message
    reply: the reply message to be sent

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.

    See Also
    --------
    ConnectionManager::request
";

%feature("docstring") gmsec::api::mist::ConnectionManager::stopResourceMessageService "

    stopResourceMessageService(self) -> bool

    This function stops the resource message service

    Returns
    -------
    Returns true if the resource message service was running before
    invocation of this function, false otherwise.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::getSpecification "

    getSpecification(self) -> Specification

    Returns the Specification object associated with the Connection
    Manager.

    Returns
    -------
    A Specification object.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::setStandardFields "

    setStandardFields(self, standardFields)

    Sets the internal list of fields which are to be automatically
    placed in all messages sent by this ConnectionManager. Internal
    copies of the Fields are made, ownership is not retained by the
    ConnectionManager. The supplied set of fields will not be validated
    here; validation occurs at the time a message is to be published.

    Parameters
    ----------
    standardFields: A list of fields to be copied to the internal set
        of fields, which will in turn be appended to all messages.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::publishResourceMessage "

    publishResourceMessage(self, subject, sampleInterval=1, averageInterval=10)

    This function creates a valid resource message, as described in the
    GMSEC specification. This function then publishes the message to
    the GMSEC bus on the supplied subject.

    Parameters
    ----------
    subject: subject on which to publish the message
    sampleInterval: distance between samples (in seconds)
    averageInterval: time interval over which to make a moving average
        of samples (in seconds)

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.
    A GmsecError is thrown if a severe error occurs while attempting
        to publish the message.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::excludeSubject "

    excludeSubject(self, subject)

    Exclude any incoming messages with the specified subject.

    Parameters
    ----------
    subject: The subject pattern to look for in incoming messages.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
    initialized.

    See Also
    --------
    ConnectionManager::ExcludeSubject()

";

%feature("docstring") gmsec::api::mist::ConnectionManager::stopHeartbeatService "

    stopHeartbeatService(self)

    This method terminates opertaion of the heartbeat service and
    cleans up related resources. It will be called by the global
    connection manager \"cleanup\" if not done manually by the user.
";

%feature("docstring") gmsec::api::mist::ConnectionManager::publishLog "

    publishLog(self, logMessage, severity)

    This method creates a log message comprised of the global fields
    from the entire ConnectionManager, the fields set in
    setLoggingDefaults, and the log text and severity fields as
    supplied as arguments to this function. If validation is enabled,
    the message will be published. If validation is enabled, the
    message will be validated.  If validation of the message fails,
    then a GmsecError is thrown.

    This function automatically populates the message with the correct
    MESSAGE-TYPE and MESSAGE-SUBTYPE fields. EVENT-TIME is set
    automatically to the time of function invocation.

    Parameters
    ----------
    logMessage: the message to log
    severity: the Field object defining the severity of the log message.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.
    A GmsecError is thrown if the Log Message cannot be validated,
        or if other severe error occurs while attempting to send
        the message.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::changeCPUUtil "

    changeCPUUtil(self, cpuUtil) -> Status

    This method updates the field which is included within the
    Heartbeat Message being published by the Heartbeat Service. If
    validation is enabled, the message will then be re-validated with
    the modified field set. Should the validation pass, the change will
    be persisted to the heartbeat service, otherwise a GmsecError is
    thrown.

    Parameters
    ----------
    cpuUtil: Field containing updated information for the Heartbeat
        Message

    Returns
    -------
    The status of the operation.

    Note: This method has been deprecated. Use
    setHeartbeatServiceField() instead.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::createLogMessage "

    createLogMessage(self, subject, logFields) -> Message

    This method creates a Log Message and passes ownership to the user.
    This message is populated with the standard set of required and
    optional log fields, as well as the required common fields defined
    in setStandardFields. The message is not validated at this time,
    as MSG-TEXT and SEVERITY fields must be set by the user at the time
    the message is to be sent.

    This message automatically is generated with MESSAGE-TYPE and
    MESSAGE-SUBTYPE correctly set to the GMSEC Message Standard
    requirements, regardless of user input. EVENT-TIME defaults to the
    time which this function was invoked; the user may override this
    time once the message has been returned.

    Note: When the user done with the message, they should destroy it
    using release.

    Parameters
    ----------
    subject: the topic (subject) to apply to the returned message.
    logFields: list of fields to be added to the log message.

    Returns
    -------
    A Log Message.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::setHeartbeatServiceField "

    setHeartbeatServiceField(self, field) -> Status

    This method can be used to update or set a field within the
    Heartbeat Message being published by the Heartbeat Service. If
    validation is enabled, the field will be validated before it is
    added to the Heartbeat Message.  Should validation pass, the field
    will be persisted to the heartbeat service; otherwise an error is
    returned.

    Parameters
    ----------
    field: the field containing new or updated information for the
    Heartbeat Message

    Returns
    -------
    The status of the operation.

    Note: If a (valid) PUB-RATE field is passed to this method, and
    the Heartbeat Service is running, then the Heartbeat Service
    publish rate will be changed to the provided rate.  Note that a
    publish rate of 0 seconds or less will be treated as an error.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::changeComponentInfo "

    changeComponentInfo(self, componentInfo) -> Status

    This method updates the field which is included within the
    Heartbeat Message being published by the Heartbeat Service. If
    validation is enabled, the message will then be re-validated with
    the modified field set. Should the validation pass, the change will
    be persisted to the heartbeat service, otherwise a GmsecError is
    thrown.

    Parameters
    ----------
    componentInfo: Field containing updated information for the
        Heartbeat Message

    Returns
    -------
    The status of the operation.

    Note: This method has been deprecated. Use
        setHeartbeatServiceField() instead.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::setLoggingDefaults "

    setLoggingDefaults(self, subject, logFields)

    This method sets the standard default fields which will be added to
    a new log message when publishLog is called.

    Parameters
    ----------
    subject: subject to use for log messages
    logFields: list of fields to be added to log messages.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::removeExcludedSubject "

    removeExcludedSubject(self, subject)

    Remove an excluded subject, allowing incoming messages with the
    matching subject to once again be received.

    Parameters
    ----------
    subject: The subject pattern to remove.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
    initialized.

    See Also
    --------
    ConnectionManager::RemoveExcludedSubject()

";

%feature("docstring") gmsec::api::mist::ConnectionManager::receive "

    receive(self, timeout) -> Message

    This function is a pass-through function to the underlying
    connection. This function returns the next message received within
    the specified timeout. The received messages are determined by the
    subscribe() function(s), but dispatchMsg() needs to be called
    messages received from this function to ensure all registered
    callbacks are executed.

    This function MUST NOT BE USED if the auto-dispatcher is being used.

    Parameters
    ----------
    timeout: the maximum time to block waiting for a message, in
        milliseconds

    Returns
    -------
    The next received message, if any. If a timeout occurs, NULL is
        returned.

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.

    See Also
    --------
    ConnectionManager::subscribe
    ConnectionManager::release

";

%feature("docstring") gmsec::api::mist::ConnectionManager::release "

    release(self, msg)

    This function destroys a message and cleans up any associated
    memory.

    Parameters
    ----------
    msg: message to be destroyed

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
    initialized.

    See Also
    --------
    ConnectionManager::receive()

";

%feature("docstring") gmsec::api::mist::ConnectionManager::unsubscribe "

    unsubscribe(self, info)

    This method unsubscribes to a particular subject or pattern, and
    will stop the reception or messages that match this pattern. It
    will also remove the registration of any callbacks with this
    subject pattern.

    Parameters
    ----------
    info: subscription pattern to deregister

    Exceptions
    ----------
    A GmsecError is thrown if the ConnectionManager has not been
        initialized.
    A GmsecError is thrown if the subject string is NULL, or if
        the callback that is given is not registered.

";

%feature("docstring") gmsec::api::mist::ConnectionManager::cleanup "

    cleanup(self)

    This function disconnects and destroys the underlying Connection
    object, returning an error status if this operation is not
    successful.

    Exceptions
    ----------
    A GmsecError is thrown if an anomaly occurs while disconnecting

    See Also
    --------
    Connection:disconnect()

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

";

%feature("docstring") gmsec::api::mist::ConnectionManager::startHeartbeatService "

    startHeartbeatService(self, subject, heartbeatFields)

    This method creates an archetypal heartbeat message from the
    concatenation of the standard fields applied globally to this
    ConnectionManager and to this function. This message will then be
    validated, if the \"validate\" flag is set. If there is an error,
    a GmsecError will be thrown. If it has not been set, the message
    will be published at an interval supplied by the \"PUB-RATE\" field
    regardless of validation results. If no \"PUB-RATE\" has been
    defined, the service will default to the GMSEC standard 30 second
    heartbeat interval.

    MESSAGE-TYPE, MESSAGE-SUBTYPE, and C2CX-SUBTYPE fields will all be
    generated and added to the message automatically, according to the
    GMSEC Message Standard

    Parameters
    ----------
    subject: subject to set as the default for heartbeat messages
    heartbeatFields: list of fields to be added to the published
        heartbeat messages

    Exceptions
    ----------
    A GmsecError is thrown if the subject is NULL.

";
