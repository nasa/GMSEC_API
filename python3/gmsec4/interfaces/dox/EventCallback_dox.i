
%feature("docstring") gmsec::api::EventCallback "

    This class is the abstract base class for event callbacks.
    A user created class, derived from this class, can be passed into
    register_event_callback() to have user code executed asynchronously
    when an event occurs in the Connection.

    Note that because users are able to create their own EventCallback
    class, reentrancy is not guaranteed unless if they implement their
    own reentrancy rules.

    In addition, if an EventCallback is registered to multiple Connection
    objects, on_event() can be invoked concurrently from different threads.
    Use of a Mutex is suggested to enforce thread safety.

    Example callback class:
    class MyEventCallback(libgmsec_python3.EventCallback):
        def __init__(self):
            libgmsec_python3.EventCallback.__init__(self)
            self.mutex = libgmsec_python3.Mutex()

        def on_event(self, conn, status, event):
            self.mutex.enter_mutex()
            print(status.get())
            self.mutex.leave_mutex()

    Example callback registration:
    cb = MyEventCallback()
    conn.register_event_callback(Connection.REQUEST_TIMEOUT, cb)

    See Also
    --------
    Connection::register_event_callback()
";

%feature("docstring") gmsec::api::EventCallback::onEvent "

    on_event(self, conn: Connection, status: Status, event: Connection::ConnectionEvent)

    This method is called in response to a error after a call to
    register_event_callback().

    If an EventCallback is registered to multiple connections, on_event()
    can be invoked concurrently from the different connection threads.

    Note: DO NOT DESTROY or CHANGE STATE of the Connection object that
    is passed to the callback method, nor store it for use beyond the
    scope of the callback.

    Note: DO NOT STORE the Status object for use beyond the scope of
    the callback. Otherwise, make a copy of the Status object.

    Parameters
    ----------
    conn   : A reference to the Connection that is reporting the event
    status : A reference to a Status that contains information regarding the event
    event  : The event that led the callback to be summoned

    See Also
    --------
    Connection::register_event_callback()
";
