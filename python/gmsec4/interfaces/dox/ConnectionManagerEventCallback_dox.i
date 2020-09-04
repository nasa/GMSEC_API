
// File: classlibgmsec__python_1_1ConnectionManagerEventCallback.xml


%feature("docstring") gmsec::api::mist::ConnectionManagerEventCallback "

    This class is the abstract base class for MIST event callbacks.
    A user created class, derrived from this class, can be passed into registerEventCallback() to have
    user code executed asynchronously when an event (typically an error) occurs in the connection object.
    Please note that because users are able to create their own ConnectionManagerEventCallback class, reentrancy
    is not guaranteed unless if they implement their own reentrancy rules.
    Also note that because a ConnectionManagerEventCallback can be registered to multiple connections,
    it can be run concurrently amongst those connections.  Because of this, the use of a gmsec::util::AutoMutex is
    suggested to enforce thread safety.

    Example callback class:
    class MyEventCallback(libgmsec_python.ConnectionManagerEventCallback):
        def onEvent(self, connMgr, status, event):
            print status.get()

    Example callback registration:
    cb = MyEventCallback()
    connMgr.registerEventCallback(libgmsec_python.Connection.REQUEST_TIMEOUT, cb)

    See Also
    --------
    ConnectionManager::registerEventCallback(self, event, cb)

";

%feature("docstring") gmsec::api::mist::ConnectionManagerEventCallback::onEvent "

    onEvent(self, connMgr, status, event)

    This function is called in response to a event after a call to registerEventCallback().

    Please note that if a callback is registered to multiple connections, onEvent() can be invoked concurrently
    from the different connection threads.

    Note: DO NOT DESTROY the ConnectionManager that is passed into this function
        by the API.  It is owned by the API and does not need to be managed by the client program.  Also,
        the ConnectionManager object should not be stored by the client program beyond the scope of this
        callback function.

    Parameters
    ----------
    connMgr: A reference to the ConnectionManager object that is reporting the event
    status: A reference to a status object that contains information regarding the event
    event: the event that led the callback to be summoned

    See Also
    --------
    ConnectionManager::registerEventCallback(self, event, cb)

";

