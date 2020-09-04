
%feature("docstring") gmsec::api::mist::ConnectionManagerCallback "

    This class is the abstract base class for received message
    callbacks.

    A user created class, derived from this class, can be passed into
    Subscribe() to have user code executed asynchronously when a
    message is received.

    Note that because users are able to create their own ConnectionManagerCallback
    class, reentrancy is not guaranteed unless if reentrancy rules are specified.

    In addition, if a ConnectionManagerCallback is registered to multiple ConnectionManagers,
    on_message() can be invoked concurrently from different threads. Use of a Mutex is
    suggested to enforce thread safety.

    Example ConnectionManagerCallback class:
    class MyCallback(libgmsec_python3.ConnectionManagerCallback):
        def __init__(self):
            libgmsec_python3.ConnectionManagerCallback.__init__(self)
            self.mutex = libgmsec_python3.Mutex()

        def onMessage(self, connMgr, msg):
            self.mutex.enter_mutex()
            print(msg.toXML())
            self.mutex.leave_mutex()

    Example callback registration:
    cb = MyCallback()

    try:
        config = libgmsec_python3.Config()
        connMgr = libgmsec_python3.ConnectionManager(config)

        connMgr.subscribe(\"GMSEC.MISSION.SAT.>\", cb)

        ...
    except GmsecError as e:
        print(e.what())
";

%feature("docstring") gmsec::api::mist::ConnectionManagerCallback::onMessage "

    on_message(self, connMgr: ConnectionManager, msg: Message)

    This method is called by the API in response to a message, from
    either the dispatch() call or inside the Auto-dispatcher after a
    start_auto_dispatch() call. A class derived from ConnectionManagerCallback
    needs to be registered with a connection, using subscribe() in order to be
    called for a particular subject registration pattern.

    If a ConnectionManagerCallback is registered to multiple connections,
    on_message() can be invoked concurrently from different connection threads.

    Note: DO NOT STORE or CHANGE STATE of the ConnectionManager object; it
    should only be used within the scope of the callback method.

    Note: DO NOT STORE the Message object for use beyond the scope of the
    callback. Otherwise, make a copy of the Message object.

    Parameters
    ----------
    connMgr : ConnectionManager on which the message was received
    msg     : The received Message

    See Also
    --------
    ConnectionManager.subscribe(subject, cb)
    ConnectionManager.dispatch(msg)
    ConnectionManager.start_auto_dispatch()
";

