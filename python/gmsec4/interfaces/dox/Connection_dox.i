
// File: classlibgmsec__python_1_1Connection.xml


%feature("docstring") gmsec::api::Connection "

    This is the public interface for middleware connections. Each middleware connection
    implements a class derived from Connection to abstract middleware specific connection details.
    The application will never access the middleware connection directly, but only through the
    Connection \"interface\".

    The connection class provides services available on an implemented GMSEC connection. An
    application can create multiple connection objects with different parameters and different
    middleware.
    Creating multiple connections to the same middleware is not prevented but may not be supported
    by all middleware implementations.

    Connection objects make use of a gmsec::util::AutoMutex by default and are therefore considered
    thread-safe.

    Example creation and use:

    #Create config from command line arguments
    cfg = libgmsec_python.Config()

    try:
        # Create the Connection
        conn = Connection.create(cfg)

        # Establish the connection
        conn.connect()

        ...

        # Disconnect from middleware server
        conn.disconnect()

        # Destroy the Connection object
        Connection.destroy(conn)
    except Exception as e:
        #handle error

    See Also
    --------
    Config
";

%feature("docstring") gmsec::api::Connection::getLibraryRootName "

    getLibraryRootName(self) -> char const *

    This function identifies the root library name and therefore the
    connection type that this connection is associated with.

    Returns
    -------
    root library name
";

%feature("docstring") gmsec::api::Connection::shutdownAllMiddlewares "

    shutdownAllMiddlewares()

    Calls shutdown routines for each middleware that has a shutdown routine registered.
";

%feature("docstring") gmsec::api::Connection::excludeSubject "

    excludeSubject(self, subject)

    Exclude any incoming messages with the specified subject.

    Parameters
    ----------
    subject: The subject pattern to look for in incoming messages.
";

%feature("docstring") gmsec::api::Connection::getName "

    getName(self) -> char const *

    Returns the name of the connection, automatically generated or user specified.

    Returns
    -------
    A string
";

%feature("docstring") gmsec::api::Connection::connect "

    connect(self)

    This function establishes this connection to the middleware

    Exceptions
    ----------
    Exception if an anomaly occurs while connecting
";

%feature("docstring") gmsec::api::Connection::destroy "

    destroy(conn)

    This static method is used to destroy the Connection object.

    Parameters
    ----------
    conn: the Connection object to destroy

    Exceptions
    ----------
    Exception if connection object is NULL.

    See Also
    --------
    create()
";

%feature("docstring") gmsec::api::Connection::request "

    request(self, request, timeout, cb, republish_ms=0)

    This function will send a request asyncronously. The callback will be
    called for the reply if it is received within the specified timeout. This
    function will not block. The timeout value is expressed in milliseconds.

    Note: The actual Message that is sent to the middleware will contain tracking fields;
    to disable this feature, create a Connection object with the tracking=off
    configuration option.

    Parameters
    ----------
    request: request message to be sent
    timeout: maximum time to wait for reply (in milliseconds)
    cb: Callback to call when reply is received
    republish_ms - request message resubmission interval (in milliseconds). If set to a negative
        value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
        the period will default to 60000ms, unless the user has provided an alternate time period via the
        Config object used to create the Connection object.  The minimum republish period allowed is 100ms.

    Exceptions
    ----------
    Exception on error with generating async request, or if ReplyCallback is NULL.

    See Also
    --------
    cancelRequest()

    request(self, request, timeout, republish_ms=0) -> Message

    This function will send a request, wait for the specified timeout, and return
    the received reply.
    This function will block until the reply is received or the timeout is reached.
    The timeout value is expressed in milliseconds.

    Note: The actual Message that is sent to the middleware will contain tracking fields;
        to disable this feature, create a Connection object with the tracking=off
        configuration option.

    Parameters
    ----------
    request: request message to be sent
    timeout:  maximum time to wait for reply (in milliseconds)
    republish_ms - request message resubmission interval (in milliseconds). If set to a negative
        value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
        the period will default to 60000ms, unless the user has provided an alternate time period via the
        Config object used to create the Connection object.  The minimum republish period allowed is 100ms.

    Returns
    -------
    Reply Message, or null if no reply received in time

    Exceptions
    ----------
    Exception on error transmitting request message

    See Also
    --------
    release()

";

%feature("docstring") gmsec::api::Connection::getLibraryVersion "

    getLibraryVersion(self) -> char const *

    This function returns a string containing the version information for
    this connection's associated middleware.

    Returns
    -------
    library version
";

%feature("docstring") gmsec::api::Connection::release "

    release(self, message)

    This function will destroy a Message object that has been returned by receive().  This
    method should NOT be used to destroy any other Message object.

    Parameters
    ----------
    message: message to be released/destroyed

    Exceptions
    ----------
    Exception on error

    See Also
    --------
    receive()
    request()
";

%feature("docstring") gmsec::api::Connection::getPublishQueueMessageCount "

    getPublishQueueMessageCount(self) -> GMSEC_U64

    Retrieves the number of messages queued for asynchronous publish operations

    Returns
    -------
    The number of messages in the publish queue
";

%feature("docstring") gmsec::api::Connection::create "

    create(cfg) -> Connection

    This static method can be used to create a Connection object.

    Parameters
    ----------
    cfg: the Config object that specifies the type of Connection object to create.

    Returns
    -------
    Pointer to Connection object.

    Exceptions
    ----------
    Exception if configuration information cannot be used to deduce Connection type.

    See Also
    --------
    destroy()
";

%feature("docstring") gmsec::api::Connection::getAPIVersion "

    getAPIVersion() -> char const *

    This function identifies the version of the API.

    Returns
    -------
    String containing API version information.
";

%feature("docstring") gmsec::api::Connection::registerEventCallback "

    registerEventCallback(self, event, cb)

    This function allows the registration of a callback for a particular
    error event.

    Parameters
    ----------
    event: type of event to register
    cb: object derived from EventCallback to register for this error event

    Exceptions
    ----------
    Exception if the EventCallback is NULL.

    See Also
    --------
    EventCallback

";

%feature("docstring") gmsec::api::Connection::unsubscribe "

    unsubscribe(self, info)

    This function unsubscribes to a particular subject pattern, and will stop
    the reception of messages that match this pattern. It will also remove the
    registration of any callbacks with this subject pattern.

    Parameters
    ----------
    info: SubscriptionInfo handle from subscription.

    See Also
    --------
    subscribe()
    receive()
    dispatch()
    startAutoDispatch()
    stopAutoDispatch()

";

%feature("docstring") gmsec::api::Connection::publish "

    publish(self, msg)

    This function will publish a message to the middleware.

    Note: The actual Message published to the middleware will contain tracking fields;
        to disable this feature, create a Connection object with the tracking=off
        configuration option.

    Parameters
    ----------
    msg: message to be published

    Exceptions
    ----------
    Exception if error occurs while attempting to publish the message

    See Also
    --------
    subscribe()
    receive()
    startAutoDispatch()
    stopAutoDispatch()

    publish(self, msg, config)

    This function will publish a message to the middleware
    using the config object provided to toggle between special middleware-level
    publish functionalities. (eg. ActiveMQ - Durable Producer)

    Note: The actual Message published to the middleware will contain tracking fields;
        to disable this feature, create a Connection object with the tracking=off
        configuration option.

    Parameters
    ----------
    msg: message to be published
    config: config object to be used by the publish operation

    Exceptions
    ----------
    Exception if error occurs while attempting to publish the message

    See Also
    --------
    subscribe()
    receive()
    startAutoDispatch()
    stopAutoDispatch()
";

%feature("docstring") gmsec::api::Connection::shutdownMiddleware "

    shutdownMiddleware(name)

    Calls the shutdown routine for the middleware with the given name.

    Parameters
    ----------
    name: A string representing the library name of the gmsec wrapper for
        the middleware to shutdown; e.g., \"gmsec_mb\".
";

%feature("docstring") gmsec::api::Connection::cancelRequest "

    cancelRequest(self, cb)

    This function can be used to cancel a pending request that may be associated with
    the given ReplyCallback.

    Parameters
    ----------
    cb: The ReplyCallback to disassociate from any pending requests.

    See Also
    --------
    request(request, timeout, cb, republish_ms = 0)

";

%feature("docstring") gmsec::api::Connection::startAutoDispatch "

    startAutoDispatch(self) -> bool

    This function will start a thread that will dispatch messages asynchronously when they are received.
    If this is used, all subscriptions must be made with callbacks or the messages with be dropped. If
    receive() is called while the auto-dispatcher is used, the behavior will be undesireable and undefined.

    Returns
    -------
    A value of true is returned if the auto-dispatcher has been started; false otherwise.

";

%feature("docstring") gmsec::api::Connection::stopAutoDispatch "

    stopAutoDispatch(self, waitForCompletion=True) -> bool

    This function will stop the auto dispatch thread.

    Parameters
    ----------
    [optional] waitForCompletion: if set to false, this method will not block; otherwise the method will
        block until the auto-dispatch thread has completed running.

    Returns
    -------
    A value of true is returned if the auto-dispatcher was running and has been stopped; false otherwise.
    If the waitForCompletion flag is set to false, then users can expect a return value of false.

    See Also
    --------
    Connection::startAutoDispatch()

";

%feature("docstring") gmsec::api::Connection::dispatch "

    dispatch(self, msg)

    This function will cause the any callbacks that are registered with matching
    message subject patterns to be called.

    Parameters
    ----------
    msg: message to be dispatched

    Exceptions
    ----------
    Exception on error

    See Also
    --------
    subscribe()
    receive()
";

%feature("docstring") gmsec::api::Connection::setName "

    setName(self, name)

    Set the logical name of this connection. This can be used for
    Identifying connections withing a client program. If a name is not given,
    one will be automatically generated.

    Parameters
    ----------
    name: name of this connection
";

%feature("docstring") gmsec::api::Connection::getMWInfo "

    getMWInfo(self) -> char const *

    Returns a string containing middleware information.
";

%feature("docstring") gmsec::api::Connection::receive "

    receive(self, timeout=-1) -> Message

    This function returns the next message received within the specified timeout.
    The received messages are determined by the message subscriptions set up with the
    subscribe() function(s).

    Note: This function MUST NOT BE USED if the auto-dispatcher is being used.

    Parameters
    ----------
    timeout: the maximum time (in milliseconds) to block waiting for a message; if -1
        is specified for the timeout, then the call will block indefinitely.

    Returns
    -------
    A pointer to the next available Message, or NULL if a timeout occurs.  All received
    messages should be destroyed using release().

    Exceptions
    ----------
    Exception on error

    See Also
    --------
    subscribe()
    dispatch()
    release()

";

%feature("docstring") gmsec::api::Connection::subscribe "

    subscribe(self, subject, cb=None) -> SubscriptionInfo

    This function subscribes to a particular subject or pattern and
    associates a callback to be called when messages matching the subject
    or pattern are received. If all subscriptions are performed using this
    function then the auto-dispatcher can be used to asynchronously receive
    messages. If receive() is used to pull messages then dispatch()
    will need to be called to ensure registered Callbacks are called.

    Example subscription patterns:

    // this will match only messages with this exact subject
    info = conn.subscribe(\"GMSEC.MISSION.CONST.SAT.EVT.MSG\")

    // this will match messages with any mission
    info = conn.subscribe(\"GMSEC.*.CONST.SAT.EVT.MSG\")

    // this will match messages that have AT LEAST ONE MORE TAG
    //     (will not match \"GMSEC.MISSION.CONST.SAT\")
    info = conn.subscribe(\"GMSEC.MISSION.CONST.SAT.>\")

    // this will match messages that have ZERO OR MORE TAG
    //     (will match \"GMSEC.MISSION.CONST.SAT\")
    info = conn.subscribe(\"GMSEC.MISSION.CONST.SAT.+\")

    // this will match any event message
    info = conn.subscribe(\"GMSEC.*.*.*.EVT.>\")

    // this will match any GMSEC-compliant message, and forward these messages to a callback
    cb = MyCallback()
    info = conn.subscribe(\"GMSEC.>\", cb)

    Note: Although subscription behavior is outlined as above, the actual behavior for a
        particular middleware implementation MAY slightly deviate from this behavior.

    Parameters
    ----------
    subject: subject pattern to match received messages
    cb: callback to be called when message is received

    Returns
    -------
    handle used to cancel or modify subscription.
    Connection maintains ownership of SubscriptionInfo; user should not delete but instead call unsubscribe()
    to free resource.

    Exceptions
    ----------
    Exception on error establishing subscription

    See Also
    --------
    unsubscribe()
    receive()
    dispatch()
    startAutoDispatch()
    stopAutoDispatch()

    subscribe(self, subject, config, cb=None) -> SubscriptionInfo

    This function subscribes to a particular subject or pattern and uses the provided config
    object to enable or disable middleware-level subscription functionalities (e.g. ActiveMQ Durable Consumer).
    If a callback is specified, then the auto-dispatcher can be used to deliver
    messages to the callback. If receive() is used to pull messages, then dispatch()
    will need to be called to ensure registered Callbacks are called.

    See Connection::subscribe() for an explanation of subscription patterns

    Parameters
    ----------
    subject: subject pattern to match received messages
    config: config object to be used for subscription operation
    cb: callback to be called when message is received

    Returns
    -------
    handle used to cancel or modify subscription.
    Connection maintains ownership of SubscriptionInfo; user should not delete but instead call unsubscribe()
    to free resource.

    Exceptions
    ----------
    Exception on error establishing subscription

    See Also
    --------
    unsubscribe()
    receive()

";

%feature("docstring") gmsec::api::Connection::disconnect "

    disconnect(self)

    This function terminates this connection to the middleware.
    It is automatically called by the destructor if necessary

    Exceptions
    ----------
    Exception if an anomaly occurs while disconnecting
";

%feature("docstring") gmsec::api::Connection::reply "

    reply(self, request, reply)

    This function will send a reply in response to a given request.

    Note: The actual Message that is sent to the middleware will contain tracking fields;
        to disable this feature, create a Connection object with the tracking=off
        configuration option.

    Parameters
    ----------
    request: the request message that was received, and to which we are responding to
    reply: the reply message to be sent

    Exceptions
    ----------
    Exception on error transmitting message
";

%feature("docstring") gmsec::api::Connection::removeExcludedSubject "

    removeExcludedSubject(self, subject)

    Remove an excluded subject, allowing incoming messages with the matching
    subject to once again be received.

    Parameters
    ----------
    subject: The subject pattern to remove.
";

%feature("docstring") gmsec::api::Connection::getID "

    getID(self) -> char const *

    Get the string ID for this connection.

";

%feature("docstring") gmsec::api::Connection::getState "

    getState(self) -> gmsec::api::Connection::ConnectionState

    This function returns the current state of the connection to the middleware.

    Returns
    -------
    Enumerated ConnectionState value.
";

