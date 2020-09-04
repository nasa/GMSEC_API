
// File: classlibgmsec__python_1_1EventCallback.xml


%feature("docstring") gmsec::api::EventCallback "

    This class is the abstract base class for event callbacks.
    A user created class, derrived from this class, can be passed into registerEventCallback() to have
    user code executed asynchronously when an event occurs in the connection object.
    Please note that because users are able to create their own EventCallback class, reentrancy is not
    guaranteed unless if they implement their own reentrancy rules.
    Also note that because an EventCallback can be registered to multiple connections, it can be run
    concurrently amongst those connections.  Because of this, the use of a gmsec::api::util::AutoMutex is
    suggested to enforce thread safety.

    Example callback class:
    class MyEventCallback(libgmsec_python.EventCallback):
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

    This function is called in response to a error after a call to registerEventCallback().

    Please note that if an EventCallback is registered to multiple connections, onEvent() can be invoked
    concurrently from the different connection threads.

    Note: DO NOT DESTROY the Connection that is passed into this function by the API.
        It is owned by the API and does not need to be managed by the client program. References to
        the Connection object, the Status object and the event string should not be stored by the
        client program beyond the scope of this callback function.

    Parameters
    ----------
    conn: A reference to the Connection object that is reporting the event
    status: A reference to a status object that contains information regarding the event
    event: the event that led the callback to be summoned

    See Also
    --------
    Connection::registerEventCallback()
";
