
// File: classlibgmsec__python_1_1ConnectionManagerCallback.xml


%feature("docstring") gmsec::api::mist::ConnectionManagerCallback "

    This class is the abstract base class for received message callbacks.
    A user created class, derrived from this class, can be passed into
    Subscribe() to have user code executed asynchronously when a message is received.
    Please note that because users are able to create their own Callback class, reentrancy is not
    guaranteed unless if reentrancy rules are specified.
    Also note that because a ConnectionManagerCallback can be registered to multiple Connection
    Managers, it can be run concurrently amongst those Connection Managers.  Because of this,
    the use of a gmsec::util::AutoMutex is suggested to enforce thread safety.

    Example ConnectionManagerCallback class:
    class MyCallback(libgmsec_python.ConnectionManagerCallback):
        def __init__(self):
            self.mutex = libgmsec_python.Mutex()
        def onMessage(self, connMgr, msg):
            lock = libgmsec_python.AutoMutex(self.mutex)
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

    This function is called by the API in response to a message, from either the dispatch()
    call or inside the Auto-dispatcher after a startAutoDispatch() call. A class derrived from
    ConnectionManagerCallback needs to be registered with a connection, using subscribe() in order
    to be called for a particular subject registration pattern.

    Please note that if a ConnectionManagerCallback is registered to multiple connections, onMessage
    can be invoked concurrently from different connection threads.

    Note : DO NOT DESTROY the ConnectionManager or Message that is passed into this function
        by the API.
        They are owned by the API and do not need to be managed by the client program. Also, they can
        not be stored by the client program beyond the scope of this callback function. In order to store
        the Message, a copy must be made.

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

