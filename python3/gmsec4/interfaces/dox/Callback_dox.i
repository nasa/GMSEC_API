
%feature("docstring") gmsec::api::Callback "

    This class is the abstract base class for received message
    callbacks.

    A user created class, derived from this class, can be passed into
    Subscribe() to have user code executed asynchronously
    when a message is received.

    Please note that because users are able to create their own Callback
    class, reentrancy is not guaranteed unless if reentrancy rules are
    specified.

    In addition, if a Callback is registered to multiple connections,
    on_message() can be invoked concurrently from different connection
    threads. Use of a Mutex is suggested to enforce thread safety.

    Example Callback class:
        class PublishCallback(libgmsec_python3.Callback):
            def __init__(self):
                libgmsec_python3.Callback.__init__(self)
                self.mutex = libgmsec_python3.Mutex()

            def on_message(self, conn, msg):
                self.mutex.enter_mutex()
                print(msg.toXML())
                self.mutex.leave_mutex()

    Example Callback registration:
        cb = PublishCallback();
        conn.subscribe(\"GMSEC.*.PUBLISH\", cb);
";

%feature("docstring") gmsec::api::Callback::onMessage "

    on_message(self, conn: Connection, msg: Message)

    This method is called by the API in response to a message, from
    either the dispatch() call or inside the Auto-dispatcher after a
    start_auto_dispatch() call. A class derived from Callback needs to
    be registered with a connection, using subscribe(), in order to be
    called for a particular subject registration pattern.

    Note that if a Callback is registered to multiple connections, on_message()
    can be invoked concurrently from different connection threads.

    Note: DO NOT DESTROY or CHANGE STATE of the Connection object that is
    passed to the callback method, nor store it for use beyond the scope
    of the callback method.

    Note: DO NOT STORE the Message object for use beyond the scope of the
    callback. Otherwise, make a copy of the Message object.

    Parameters
    ----------
    conn : The Connection on which the message was received
    msg  : The received Message
";

