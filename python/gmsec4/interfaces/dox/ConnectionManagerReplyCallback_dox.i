
// File: classlibgmsec__python_1_1ConnectionManagerReplyCallback.xml


%feature("docstring") gmsec::api::mist::ConnectionManagerReplyCallback "

    This class is the abstract base class for received replies from asynchronous request messages.
    A user created class, derrived from this class, can be passed into
    request() to have user code executed asynchronously when a reply is received or when an error occurs.
    Please note that because users are able to create their own ConnectionManagerReplyCallback class,
    reentrancy is not guaranteed unless they implement their own reentrancy rules.
    Also note that because a ConnectionManagerReplyCallback can be registered to multiple connections,
    it can be run concurrently amongst those connections.  Because of this, the use of a gmsec::util::AutoMutex
    is * suggested to enforce thread safety.

    Example Callback class:
    class MyReplyCallback(libgmsec_python.ConnectionManagerReplyCallback):
        def onReply(self, connMgr, status, event):
            print request.toXML()
            print reply.toXML()
        def onEvent(self, connMgr, status, event):
            print status.get()

    Example ConnectionManagerReplyCallback registration:
    cb = MyReplyCallback()
    reqMsg = libgmsec_python.Message(\"GMSEC.MISSION.SAT.REQ.DIR\", libgmsec_python.python.Message.REQUEST)
    timeout = 1000

    try:
        connMgr.request(reqMsg, timeout, cb)
    except Exception as e:
        print >> sys.stderr, e.what()
";

%feature("docstring") gmsec::api::mist::ConnectionManagerReplyCallback::onReply "

    onReply(self, connMgr, request, reply)

    This function is called by the API in response to a reply recieved from a request,
    from within the Request call. A class derrived from gmsec::mist::ConnectionManagerReplyCallback needs
    to be passed into the request() call.

    Please note that if a callback is registered to multiple connections, onReply can be invoked concurrently
    from the different connection threads.

    Note: DO NOT DESTROY the ConnectionManager, or the Messages that are passed into this function by
        the API.  They are owned by the API and do not need to be managed by the client program. Also, they can
        not be stored by the client program beyond the scope of this callback function. In order to store
        a Message, the message must be copied.

    Parameters
    ----------
    connMgr: connection manager on which the message was received
    request: the pending request
    reply: the received reply

    See Also
    --------
    ConnectionManager::request(self, request, timeout, cb, republish_ms = 0)

";
