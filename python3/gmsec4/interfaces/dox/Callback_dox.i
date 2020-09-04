
%feature("docstring") gmsec::api::Callback "

    This class is the abstract base class for received message
    callbacks.

    A user created class, derived from this class, can be passed into
    Subscribe() and Request() to have user code executed asynchronously
    when a message is received.

    Please note that because users are able to create their own Callback
    class, reentrancy is not guaranteed unless if reentrancy rules are
    specified.

    Also note that because a Callback can be registered to multiple
    connections, it can be run concurrently amongst those connections.
    Because of this, the use of an AutoMutex is suggested to enforce
    thread safety.

    Example Callback class:
        class PublishCallback(libgmsec_python3.Callback):
            def onMessage(self, conn, msg):
                print(msg.toXML())

    Example Callback registration:
        cb = PublishCallback();
        conn.subscribe(\"GMSEC.*.PUBLISH\", cb);
";

%feature("docstring") gmsec::api::Callback::onMessage "

    onMessage(self, conn, msg)

    This method is called by the API in response to a message, from
    either the dispatchMsg() call or inside the Auto-dispatcher after a
    startAutoDispatch() call. A class derrived from Callback needs to
    be registered with a connection, using subscribe(), in order to be
    called for a particular subject registration pattern.

    Please note that if a Callback is registered to multiple
    connections, onMessage() can be invoked concurrently from different
    connection threads.

    The prototype for this funtion is:
        onMessage(self, conn, msg)

    Note: DO NOT DESTROY the Connection, or Message that is passed into
    this function by the API. They are owned by the API and do not need
    to be managed by the client program. Also, they can not be stored
    by the client program beyond the scope of this callback function.
    In order to store the Message, a copy must be made of such.

    Parameters
    ----------
    conn: connection on which the message was received
    msg: the received message
";

