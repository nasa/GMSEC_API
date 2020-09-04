
// File: classlibgmsec__python_1_1EventCallback.xml


%feature("docstring") gmsec::api::EventCallback "

    This class is the abstract base class for event callbacks.
    A user created class, derived from this class, can be passed into
    registerEventCallback() to have user code executed asynchronously
    when an event occurs in the Connection.

    Note that because users are able to create their own EventCallback
    class, reentrancy is not guaranteed unless if they implement their
    own reentrancy rules.

    In addition, if an EventCallback is registered to multiple Connection
    objects, onEvent() can be invoked concurrently from different threads.
    Use of a Mutex is suggested to enforce thread safety.

    Example callback class:
    class MyEventCallback(libgmsec_python.EventCallback):
        def __init__(self):
            libgmsec_python.EventCallback.__init__(self)

        def onEvent(self, conn, status, event):
            print status.get()

    Example callback registration:
    cb = MyEventCallback()
    conn.registerEventCallback(libgmsec_python.Connection.REQUEST_TIMEOUT, cb)

    See Also
    --------
    Connection::registerEventCallback()
";

%feature("docstring") gmsec::api::EventCallback::onEvent "

    onEvent(self, conn, status, event)

    This method is called in response to a error after a call to
    registerEventCallback().

    If an EventCallback is registered to multiple connections, onEvent()
    can be invoked concurrently from the different connection threads.

    Note: DO NOT DESTROY or CHANGE STATE of the Connection object that
    is passed to the callback method, nor store it for use beyond the
    scope of the callback.

    Note: DO NOT STORE the Status object for use beyond the scope of
    the callback. Otherwise, make a copy of the Status object.

    Parameters
    ----------
    conn: A reference to the Connection object that is reporting the event
    status: A reference to a status object that contains information regarding the event
    event: the event that led the callback to be summoned

    See Also
    --------
    Connection::registerEventCallback()
";
