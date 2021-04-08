
%feature("docstring") gmsec::api::Connection "

    This is the public interface for middleware connections. Each
    middleware connection implements a class derived from Connection to
    abstract middleware specific connection details. The application
    will never access the middleware connection directly, but only
    through the Connection \"interface\".

    The connection class provides services available on an implemented
    GMSEC connection. An application can create multiple Connections
    with different parameters and different middleware.
    Creating multiple connections to the same middleware is not
    prevented but may not be supported by all middleware
    implementations.

    Connections make use of a Mutex by default and are therefore
    considered thread-safe.

    Example creation and use:

    #Create config from command line arguments
    cfg = libgmsec_python3.Config()

    try:
        # Create the Connection
        conn = Connection.create(cfg)

        # Establish the connection
        conn.connect()

        ...

        # Disconnect from middleware server
        conn.disconnect()

        # Destroy the Connection
        Connection.destroy(conn)

    except GmsecError as e:
        #handle error

    See Also
    --------
    Config
";

%feature("docstring") gmsec::api::Connection::getLibraryRootName "

    get_library_root_name(self) -> str

    This method identifies the root library name and therefore the
    connection type that this connection is associated with.

    Returns
    -------
    root library name
";

%feature("docstring") gmsec::api::Connection::shutdownAllMiddlewares "

    shutdown_all_middlewares()

    Calls shutdown routines for each middleware that has a shutdown routine registered.
	This method should only be called once, typically just before an application terminates. 
	Note the calling of this method is optional.
";

%feature("docstring") gmsec::api::Connection::excludeSubject "

    exclude_subject(self, subject: str)

    Exclude any incoming messages with the specified subject.

    Parameters
    ----------
    subject: The subject pattern to look for in incoming messages.
";

%feature("docstring") gmsec::api::Connection::getName "

    get_name(self) -> str

    Returns the name of the connection, automatically generated or user specified.

    Returns
    -------
    The connection name.
";

%feature("docstring") gmsec::api::Connection::connect "

    connect(self)

    This method establishes this connection to the middleware

    Exceptions
    ----------
    A GmsecError is thrown if an anomaly occurs while connecting
";

%feature("docstring") gmsec::api::Connection::destroy "

    destroy(conn)

    This static method is used to destroy the Connection.

    Parameters
    ----------
    conn: the Connection to destroy

    Exceptions
    ----------
    A GmsecError is thrown if Connection is NULL.

    See Also
    --------
    create()
";

%feature("docstring") gmsec::api::Connection::request "

    request(self, request: Message, timeout: int, cb: ReplyCallback, republish_ms=0: int)

    This method will send a request asyncronously. The callback will
    be called for the reply if it is received within the specified
    timeout. This method will not block. The timeout value is
    expressed in milliseconds.

    Note: The actual Message that is sent to the middleware will
    contain tracking fields; to disable this feature, create a
    Connection with the tracking=off configuration option.

    Parameters
    ----------
    request      : Request message to be sent
    timeout      : Maximum time to wait for reply (in milliseconds)
    cb           : User-defined ReplyCallback to call when reply is received
    republish_ms : Request message resubmission interval (in milliseconds).
                   If set to a negative value (eg. GMSEC_REQUEST_REPUBLISH_NEVER)
                   it will never republish a request message. If set to 0, the period
                   will default to 60000ms, unless the user has provided an alternate
                   time period via the Config used to create the Connection. The
                   minimum republish period allowed is 100ms.

    Exceptions
    ----------
    A GmsecError is thrown on error with generating async request, or if ReplyCallback is NULL.

    See Also
    --------
    ReplyCallback
    cancel_request()



    request(self, request: Message, timeout: int, republish_ms=0: int) -> Message

    This method will send a request, wait for the specified timeout,
    and return the received reply.
    This method will block until the reply is received or the timeout
    is reached.
    The timeout value is expressed in milliseconds.

    Note: The actual Message that is sent to the middleware will
          contain tracking fields; to disable this feature, create a
          Connection with the tracking=off configuration option.

    Parameters
    ----------
    request      : Request message to be sent
    timeout      : Maximum time to wait for reply (in milliseconds)
    republish_ms : Request message resubmission interval (in milliseconds).
                   If set to a negative value (eg. GMSEC_REQUEST_REPUBLISH_NEVER)
                   it will never republish a request message. If set to 0, the period
                   will default to 60000ms, unless the user has provided an alternate
                   time period via the Config used to create the Connection. The
                   minimum republish period allowed is 100ms.

    Returns
    -------
    Reply Message, or None if no reply received in time.

    Exceptions
    ----------
    A GmsecError is thrown on error transmitting request message.

    See Also
    --------
    release()
";

%feature("docstring") gmsec::api::Connection::getLibraryVersion "

    get_library_version(self) -> str

    This method returns a string containing the version information
    for this connection's associated middleware.

    Returns
    -------
    library version
";

%feature("docstring") gmsec::api::Connection::release "

    release(self, message: Message)

    This method will destroy a Message that has been returned
    by receive(). This method should NOT be used to destroy any other
    Message.

    Parameters
    ----------
    message : Message to be released/destroyed

    Exceptions
    ----------
    A GmsecError is thrown on error.

    See Also
    --------
    receive()
    request()
";

%feature("docstring") gmsec::api::Connection::getPublishQueueMessageCount "

    get_publish_queue_message_count(self) -> GMSEC_U64

    Retrieves the number of messages queued for asynchronous publish operations

    Returns
    -------
    The number of messages in the publish queue.
";

%feature("docstring") gmsec::api::Connection::create "

    create(config: Config) -> Connection

    This static method can be used to create a Connection.

    Parameters
    ----------
    config : The Config that specifies the type of Connection to create.

    Returns
    -------
    A handle to a Connection.

    Exceptions
    ----------
    A GmsecError is thrown if configuration information cannot be used
    to deduce Connection type.

    See Also
    --------
    destroy()
";

%feature("docstring") gmsec::api::Connection::getAPIVersion "

    get_API_version() -> str

    This method identifies the version of the API.

    Returns
    -------
    String containing API version information.
";

%feature("docstring") gmsec::api::Connection::registerEventCallback "

    register_event_callback(self, event: Connection.ConnectionEvent, cb: EventCallback)

    This method allows the registration of a callback for a particular error event.

    Parameters
    ----------
    event : Type of event to register
    cb    : User-defined EventCallback to register for this error event.

    Exceptions
    ----------
    A GmsecError is thrown if the EventCallback is NULL.

    See Also
    --------
    EventCallback
";

%feature("docstring") gmsec::api::Connection::unsubscribe "

    unsubscribe(self, info: SubscriptionInfo)

    This method unsubscribes to a particular subject pattern, and
    will stop the reception of messages that match this pattern. It
    will also remove the registration of any callbacks with this
    subject pattern.

    Parameters
    ----------
    info : SubscriptionInfo handle from subscription.

    See Also
    --------
    subscribe()
    receive()
    dispatch()
    start_auto_dispatch()
    stop_auto_dispatch()
";

%feature("docstring") gmsec::api::Connection::publish "

    publish(self, msg: Message)

    This method will publish a message to the middleware.

    Note: The actual Message published to the middleware will contain
    tracking fields; to disable this feature, create a Connection
    with the tracking=off configuration option.

    Parameters
    ----------
    msg : Message to be published

    Exceptions
    ----------
    A GmsecError is thrown if error occurs while attempting to publish the message

    See Also
    --------
    subscribe()
    receive()
    start_auto_dispatch()
    stop_auto_dispatch()



    publish(self, msg: Message, mwConfig: Config)

    Publishes the given message to the middleware
    using the given configuration to enable or disable certain middleware-level
    publish functionalities (e.g. ActiveMQ - Durable Producer).

    Note: The actual Message published to the middleware will contain
    tracking fields; to disable this feature, create a Connection
    with the tracking=off configuration option.

    Parameters
    ----------
    msg      : Message to be published
    mwConfig : Config object for providing middleware configuration options

    Exceptions
    ----------
    A GmsecError is thrown if error occurs while attempting to publish the message

    See Also
    --------
    subscribe()
    receive()
    start_auto_dispatch()
    stop_auto_dispatch()
";

%feature("docstring") gmsec::api::Connection::shutdownMiddleware "

    shutdown_middleware(name: str)

    Calls the shutdown routine for the middleware with the given name.
	This method should only be called once, typically just before an application terminates. 
	Note the calling of this method is optional.

    Parameters
    ----------
    name : A string representing the library name of the GMSEC wrapper
           for the middleware to shutdown; e.g., \"gmsec_mb\".
";

%feature("docstring") gmsec::api::Connection::cancelRequest "

    cancel_request(self, cb: ReplyCallback)

    This method can be used to cancel a pending request that may be
    associated with the given ReplyCallback.

    Parameters
    ----------
    cb : ReplyCallback to disassociate from any pending requests.

    See Also
    --------
    request(request, timeout, cb, republish_ms)

    Note
    --------
    This method is NOT functional. 
";

%feature("docstring") gmsec::api::Connection::startAutoDispatch "

    start_auto_dispatch(self) -> bool

    This method will start a thread that will dispatch messages
    asynchronously when they are received. If this is used, all
    subscriptions must be made with callbacks or the messages with be
    dropped. If receive() is called while the auto-dispatcher is used,
    the behavior will be undesireable and undefined.

    Returns
    -------
    True if the auto-dispatcher has been started; False otherwise.
";

%feature("docstring") gmsec::api::Connection::stopAutoDispatch "

    stop_auto_dispatch(self, waitForCompletion=True: bool) -> bool

    This method will stop the auto dispatch thread.

    Parameters
    ----------
    waitForCompletion : If set to false, this method will not
                        block; otherwise the method will block until the auto-dispatch
                        thread has completed running.

    Returns
    -------
    True if the auto-dispatcher was running and has been stopped; False otherwise.
    If the waitForCompletion flag is set to False, then users can expect a return value of False.

    See Also
    --------
    Connection.start_auto_dispatch()
";

%feature("docstring") gmsec::api::Connection::dispatch "

    dispatch(self, msg: Message)

    This method will cause the any callbacks that are registered with
    matching message subject patterns to be called.

    Parameters
    ----------
    msg : Message to be dispatched

    Exceptions
    ----------
    A GmsecError is thrown on error

    See Also
    --------
    subscribe()
    receive()
";

%feature("docstring") gmsec::api::Connection::setName "

    set_name(self, name: str)

    Set the logical name of this connection. This can be used for
    identifying connections withing a client program. If a name is not
    given, one will be automatically generated.

    Parameters
    ----------
    name : Name of this connection
";

%feature("docstring") gmsec::api::Connection::getMWInfo "

    get_MW_info(self) -> str

    Returns a string containing middleware information.
";

%feature("docstring") gmsec::api::Connection::getConnectionEndpoint "

    get_connection_endpoint(self) -> str

    Returns middleware broker connection information.
";

%feature("docstring") gmsec::api::Connection::receive "

    receive(self, timeout=-1: int) -> Message

    This method returns the next message received within the
    specified timeout.
    The received messages are determined by the message subscriptions
    set up with the subscribe() method(s).

    Note: This method MUST NOT BE USED if the auto-dispatcher is
    being used.

    Parameters
    ----------
    timeout : The maximum time (in milliseconds) to block waiting for a
              message; if -1 is specified for the timeout, then the call will
              block indefinitely.

    Returns
    -------
    The next available Message, or None if a timeout occurs.

    Note
    -------
    All received messages should be destroyed using release().

    Exceptions
    ----------
    A GmsecError is thrown on error

    See Also
    --------
    subscribe()
    dispatch()
    release()
";

%feature("docstring") gmsec::api::Connection::subscribe "

    subscribe(self, subject: str, cb=None: Callback) -> SubscriptionInfo

    This method subscribes to a particular subject or pattern and
    associates a callback to be called when messages matching the
    subject or pattern are received. If all subscriptions are performed
    using this method then the auto-dispatcher can be used to
    asynchronously receive messages. If receive() is used to pull
    messages then dispatch() will need to be called to ensure
    registered Callbacks are called.

    Example subscription patterns:

    # this will match only messages with this exact subject
    info = conn.subscribe(\"GMSEC.MISSION.CONST.SAT.EVT.MSG\")

    # this will match messages with any mission
    info = conn.subscribe(\"GMSEC.*.CONST.SAT.EVT.MSG\")

    # this will match messages that have AT LEAST ONE MORE TAG
    # (will not match \"GMSEC.MISSION.CONST.SAT\")
    info = conn.subscribe(\"GMSEC.MISSION.CONST.SAT.>\")

    # this will match messages that have ZERO OR MORE TAG
    # (will match \"GMSEC.MISSION.CONST.SAT\")
    info = conn.subscribe(\"GMSEC.MISSION.CONST.SAT.+\")

    # this will match any event message
    info = conn.subscribe(\"GMSEC.*.*.*.EVT.>\")

    # this will match any GMSEC-compliant message, and forward these
    # messages to a callback
    cb = MyCallback()
    info = conn.subscribe(\"GMSEC.>\", cb)

    Note: Although subscription behavior is outlined as above, the
    actual behavior for a particular middleware implementation MAY
    slightly deviate from this behavior.

    Parameters
    ----------
    subject : Subject pattern to match received messages
    cb      : User-defined Callback to be called when message is received

    Returns
    -------
    SubscriptionInfo used to cancel or modify subscription.

    Note
    -------
    Connection maintains ownership of SubscriptionInfo; user should
    not delete but instead call unsubscribe() to free resource. Only
    under rare circumstances should user retain handle to SubscriptionInfo.

    Exceptions
    ----------
    A GmsecError is thrown on error establishing subscription.

    See Also
    --------
    unsubscribe()
    receive()
    dispatch()
    start_auto_dispatch()
    stop_auto_dispatch()



    subscribe(self, subject:str, config: Config, cb=None: Callback) -> SubscriptionInfo

    This method subscribes to a particular subject or pattern and
    uses the provided Config to enable or disable middleware-
    level subscription functionalities (e.g. ActiveMQ Durable Consumer).
    If a callback is specified, then the auto-dispatcher can be used to
    deliver messages to the callback. If receive() is used to pull
    messages, then dispatch() will need to be called to ensure
    registered Callbacks are called.

    See Connection::subscribe() for an explanation of subscription
    patterns

    Parameters
    ----------
    subject : Subject pattern to match received messages
    config  : Config to be used for subscription operation
    cb      : User-defined Callback to be called when message is received

    Returns
    -------
    SubscriptionInfo used to cancel or modify subscription.

    Note
    -------
    Connection maintains ownership of SubscriptionInfo; user should
    not delete but instead call unsubscribe() to free resource. Only
    under rare circumstances should user retain handle to SubscriptionInfo.

    Exceptions
    ----------
    A GmsecError is thrown on failure to establish a subscription

    See Also
    --------
    unsubscribe()
    receive()
";

%feature("docstring") gmsec::api::Connection::disconnect "

    disconnect(self)

    This method terminates this connection to the middleware.
    It is automatically called by the destructor if necessary

    Exceptions
    ----------
    A GmsecError is thrown if an anomaly occurs while disconnecting
";

%feature("docstring") gmsec::api::Connection::reply "

    reply(self, request: Message, reply: Message)

    This method will send a reply in response to a given request.

    Note: The actual Message that is sent to the middleware will
    contain tracking fields; to disable this feature, create a
    Connection with the tracking=off configuration option.

    Parameters
    ----------
    request : The request Message that was received, and to which we are responding to
    reply   : The reply Message to be sent

    Exceptions
    ----------
    A GmsecError is thrown on error transmitting message
";

%feature("docstring") gmsec::api::Connection::removeExcludedSubject "

    remove_excluded_subject(self, subject: str)

    Remove an excluded subject, allowing incoming messages with the
    matching subject to once again be received.

    Parameters
    ----------
    subject : The subject pattern to remove.
";

%feature("docstring") gmsec::api::Connection::getID "

    get_id(self) -> str

    Get the string ID for this connection.
";

%feature("docstring") gmsec::api::Connection::getState "

    get_state(self) -> Connection::ConnectionState

    This method returns the current state of the connection to the middleware.

    Returns
    -------
    Enumerated ConnectionState value.
";
