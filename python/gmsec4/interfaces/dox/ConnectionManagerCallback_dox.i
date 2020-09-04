
// File: classlibgmsec__python_1_1ConnectionManagerCallback.xml


%feature("docstring") gmsec::api::mist::ConnectionManagerCallback "

    This class is the abstract base class for received message
    callbacks.

    A user created class, derived from this class, can be passed into
    subscribe() to have user code executed asynchronously when a
    message is received.

    Note that because users are able to create their own ConnectionManagerCallback
    class, reentrancy is not guaranteed unless if reentrancy rules are specified.

    In addition, if a ConnectionManagerCallback is registered to multiple ConnectionManagers,
    onMessage() can be invoked concurrently from different threads. Use of a Mutex is
    suggested to enforce thread safety.

    Example ConnectionManagerCallback class:
    class MyCallback(libgmsec_python.ConnectionManagerCallback):
        def __init__(self):
            libgmsec_python.ConnectionManagerCallback.__init__(self)

        def onMessage(self, connMgr, msg):
            print msg.toXML()

    Example callback registration:
    cb = MyCallback()

    try:
        config = libgmsec_python.Config()
        connMgr = libgmsec_python.ConnectionManager(config)

        connMgr.subscribe(\"GMSEC.MISSION.SAT.>\", cb)

        ...
    except Exception as e:
        print >> sys.stderr, e.what()
";

%feature("docstring") gmsec::api::mist::ConnectionManagerCallback::onMessage "

    onMessage(self, connMgr, msg)

    This method is called by the API in response to a message, from
    either the dispatch() call or inside the Auto-dispatcher after a
    startAutoDispatch() call. A class derived from ConnectionManagerCallback
    needs to be registered with a connection, using subscribe() in order to be
    called for a particular subject registration pattern.

    If a ConnectionManagerCallback is registered to multiple connections,
    onMessage() can be invoked concurrently from different connection threads.

    Note: DO NOT STORE or CHANGE STATE of the ConnectionManager object; it
    should only be used within the scope of the callback method.

    Note: DO NOT STORE the Message object for use beyond the scope of the
    callback. Otherwise, make a copy of the Message object.

    Parameters
    ----------
    connMgr: connection manager on which the message was received
    msg: the received message

    See Also
    --------
    ConnectionManager::subscribe(subject, cb)
    ConnectionManager::dispatch(msg)
    ConnectionManager::startAutoDispatch()

";

