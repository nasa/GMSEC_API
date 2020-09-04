
// File: classlibgmsec__python_1_1ConnectionManagerEventCallback.xml


%feature("docstring") gmsec::api::mist::ConnectionManagerEventCallback "

    This class is the abstract base class for event callbacks.
    A user created class, derived from this class, can be passed into
    registerEventCallback() to have user code executed asynchronously
    when an event (typically an error) occurs within the Connection.

    Note that because users are able to create their own
    ConnectionManagerEventCallback class, reentrancy is not guaranteed
    unless if they implement their own reentrancy rules.

    In addition, if a ConnectionManagerEventCallback is registered to
    multiple Connection Manager objects, onEvent() can be invoked
    concurrently from different threads. Use of a Mutex is suggested
    to enforce thread safety.

    Example callback class:
    class MyEventCallback(libgmsec_python.ConnectionManagerEventCallback):
        def __init(self):
            libgmsec_python.ConnectionManagerCallback.__init__(self)

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

    This method is called in response to a event after a call to registerEventCallback().

    If a callback is registered to multiple ConnectionManager objects, onEvent() can be
    invoked concurrently from the different connection threads.

    Note: DO NOT STORE or CHANGE STATE of the ConnectionManager object that is passed to
    the callback method.

    Note: DO NOT STORE the Status object for use beyond the scope of the callback. Otherwise,
    make a copy of the Status object.

    Parameters
    ----------
    connMgr: A reference to the ConnectionManager object that is reporting the event
    status: A reference to a status object that contains information regarding the event
    event: the event that led the callback to be summoned

    See Also
    --------
    ConnectionManager::registerEventCallback(self, event, cb)

";

