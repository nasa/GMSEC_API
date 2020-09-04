
%feature("docstring") gmsec::api::ReplyCallback "

    This class is the abstract base class for received replies from
    asynchronous request messages.
    A user created class, derived from this class, can be passed into
    request() to have user code executed asynchronously when a reply is
    received or when an error occurs.
    Please note that because users are able to create their own
    ReplyCallback class, reentrancy is not guarunteed unless if they
    implement their own reentrancy rules.
    Also note that because a ReplyCallback can be registered to
    multiple connections, it can be run concurrently amongst those
    connections. Because of this, the use of an AutoMutex is
    suggested to enforce thread safety.

    Example ReplyCallback class:

    class ReqReplyCallback(libgmsec_python3.ReplyCallback):

        onReply(self, conn, request, reply):
            print(\"Request:\\n\" + request.toXML())
            print(\"Reply:\\n\" + reply.toXML())

        onEvent(self, conn, status, event):
            print(\"Status: \" + status.get())
            print(\"\\nEvent: \" + str(event))

    Example ReplyCallback registration:

    request = Message(\"GMSEC.MY.REQUEST\", Message.REQUEST)

    # Add message fields

    conn.request(request, timeout, cb)

    See Also
    --------
    Connection::request()
";

%feature("docstring") gmsec::api::ReplyCallback::onReply "

    onReply(self, conn, request, reply)

    This function is called by the API in response to a reply received
    from a request, from within the request() call. A class derived
    from ReplyCallback needs to be passed into the request() call.

    Please note that if a ReplyCallback is registered to multiple
    connections, onReply() can be invoked concurrently from the
    different connection threads.

    Note: DO NOT DESTROY the Connection or Messages that are passed
        into this function by the API. They are owned by the API and do
        not need to be managed by the client program. Also, they can
        not be stored by the client program beyond the scope of this
        callback function. In order to store the Messages, they must
        be copied.

    Parameters
    ----------
    conn: connection on which the message was received
    request: the pending request message
    reply: the received reply message

    See Also
    --------
    Connection::request()
";
