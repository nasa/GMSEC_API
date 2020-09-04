
// File: classlibgmsec__python_1_1ReplyCallback.xml


%feature("docstring") gmsec::api::ReplyCallback "

    This class is the abstract base class for received replies from asynchronous request messages.
    A user created class, derived from this class, can be passed into request() to have user code
    executed asynchronously when a reply is received or when an error occurs.
    Please note that because users are able to create their own ReplyCallback class, reentrancy is not
    guarunteed unless if they implement their own reentrancy rules.
    Also note that because a ReplyCallback can be registered to multiple connections, it can be run
    concurrently amongst those connections.  Because of this, the use of a gmsec::api::util::AutoMutex is
    suggested to enforce thread safety.

    Example ReplyCallback class:
    class ReqReplyCallback(libgmsec_python.ReplyCallback):
        onReply(self, conn, request, reply):
            print \"Request:\\n\" + request.toXML() + \"\\n\" + \"Reply:\\n\" + reply.toXML()
        onEvent(self, conn, status, event):
            print \"Status: \" + status.get() + \"\\nEvent: \" + event

    Example ReplyCallback registration:
    request = libgmsec_python.Message(\"GMSEC.MY.REQUEST\", libgmsec_python.Message.REQUEST)
    # Add message fields

    conn.request(request, timeout, cb)

    See Also
    --------
    Connection::request()
";

%feature("docstring") gmsec::api::ReplyCallback::onReply "

    onReply(self, conn, request, reply)

    This function is called by the API in response to a reply recieved from a request,
    from within the request() call. A class derived from gmsec::api::ReplyCallback needs to be
    passed into the request() call.

    Please note that if a ReplyCallback is registered to multiple connections, onReply() can be
    invoked concurrently from the different connection threads.

    Note: DO NOT DESTROY the Connection or Messages that are passed into this function by the API.
        They are owned by the API and do not need to be managed by the client program. Also, they can
        not be stored by the client program beyond the scope of this callback function. In order to store
        the Messages, they must be copied.

    Parameters
    ----------
    conn: connection on which the message was received
    request: the pending request message
    reply: the received reply message

    See Also
    --------
    Connection::request()
";
