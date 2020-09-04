
%feature("docstring") gmsec::api::mist::ConnectionManagerEventCallback "

    This class is the abstract base class for event callbacks.
    A user created class, derived from this class, can be passed into
    register_event_callback() to have user code executed asynchronously
    when an event (typically an error) occurs within the Connection.

    Note that because users are able to create their own
    ConnectionManagerEventCallback class, reentrancy is not guaranteed
    unless if they implement their own reentrancy rules.

    In addition, if a ConnectionManagerEventCallback is registered to
    multiple Connection Manager objects, on_event() can be invoked
    concurrently from different threads. Use of a Mutex is suggested
    to enforce thread safety.

    Example callback class:
    class MyEventCallback(libgmsec_python3.ConnectionManagerEventCallback):
        def __init__(self):
            libgmsec_python3.ConnectionManagerEventCallback.__init__(self)
            self.mutex = libgmsec_python3.Mutex()

        def on_event(self, connMgr, status, event):
            self.mutex().enter_mutex()
            print(status.get())
            self.mutex().leave_mutex()

    Example callback registration:
    cb = MyEventCallback()
    connMgr.register_event_callback(libgmsec_python3.Connection.REQUEST_TIMEOUT, cb)

    See Also
    --------
    ConnectionManager::register_event_callback(self, event, cb)

";

%feature("docstring") gmsec::api::mist::ConnectionManagerEventCallback::onEvent "

    on_event(self, connMgr: ConnectionManager, status: Status, event: Connection.ConnectionEvent)

    This method is called in response to a event after a call to register_event_callback().

    If a callback is registered to multiple ConnectionManager objects, on_event() can be
    invoked concurrently from the different connection threads.

    Note: DO NOT STORE or CHANGE STATE of the ConnectionManager object that is passed to
    the callback method.

    Note: DO NOT STORE the Status object for use beyond the scope of the callback. Otherwise,
    make a copy of the Status object.

    Parameters
    ----------
    connMgr : A reference to the ConnectionManager that is reporting the event
    status  : A reference to a Status that contains information regarding the event
    event   : The event that led the callback to be summoned

    See Also
    --------
    ConnectionManager::register_event_callback(self, event, cb)
";

