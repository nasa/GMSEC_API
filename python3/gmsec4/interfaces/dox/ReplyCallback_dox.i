
%feature("docstring") gmsec::api::ReplyCallback "

    This class is the abstract base class for received replies from
    asynchronous request messages.

    A user created class, derived from this class, can be passed into
    request() to have user code executed asynchronously when a reply is
    received or when an error occurs.

    Note that because users are able to create their own ReplyCallback
    class, reentrancy is not guaranteed unless if they implement their
    own reentrancy rules.

    In addition, if a ReplyCallback is registered to multiple Connection
    objects, on_reply() can be invoked concurrently from different threads.
    Use of a Mutex is suggested to enforce thread safety.

    Example ReplyCallback class:

    class ReqReplyCallback(libgmsec_python3.ReplyCallback):
        def __init__(self):
            libgmsec_python3.ReplyCallback.__init__(self)
            self.mutex = libgmsec_python3.Mutex()

        def on_reply(self, conn, request, reply):
            self.mutex.enter_mutex()
            print(request.to_XML())
            print(reply.to_XML())
            self.mutex.leave_mutex()

        def on_event(self, conn, status, event):
            self.mutex.enter_mutex()
            print(status.get())
            print(str(event))
            self.mutex.leave_mutex()

    Example ReplyCallback registration:

    cb = ReqReplyCallback()

    request = Message(\"GMSEC.MY.REQUEST\", Message.REQUEST)

    ...

    conn.request(request, timeout, cb)

    See Also
    --------
    Connection::request()
";

%feature("docstring") gmsec::api::ReplyCallback::onReply "

    on_reply(self, conn: Connection, request: Message, reply: Message)

    This method is called by the API in response to a reply received
    from a request, from within the request() call. A class derived
    from ReplyCallback needs to be passed into the request() call.

    Note that if a ReplyCallback is registered to multiple connections, on_reply()
    can be invoked concurrently from the different connection threads.

    Note: DO NOT DESTROY or CHANGE STATE of the Connection object that is passed to
    the callback method, nor store it for use beyond the scope of the callback method.

    Note: DO NOT STORE the Message objects for use beyond the scope of the callback.
    Otherwise, make a copy of the Message object(s).

    Parameters
    ----------
    conn    : The Connection on which the message was received
    request : The pending request Message
    reply   : The received reply Message

    See Also
    --------
    Connection.request()
";
