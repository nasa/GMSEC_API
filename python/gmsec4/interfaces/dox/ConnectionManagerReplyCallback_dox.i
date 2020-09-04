
// File: classlibgmsec__python_1_1ConnectionManagerReplyCallback.xml


%feature("docstring") gmsec::api::mist::ConnectionManagerReplyCallback "

    This class is the abstract base class for received replies from
    asynchronous request messages.
    A user created class, derived from this class, can be passed into
    request() to have user code executed asynchronously when a reply is
    received or when an error occurs.

    Note that because users are able to create their own
    ConnectionManagerReplyCallback class, reentrancy is not guaranteed
    unless they implement their own reentrancy rules.

    In addition, if a ConnectionManagerReplyCallback is registered to
    multiple Connection Manager objects, onReply() can be invoked
    concurrently from different threads. Use of a Mutex is suggested
    to enforce thread safety.

    Example Callback class:
    class MyReplyCallback(libgmsec_python.ConnectionManagerReplyCallback):
        def __init__(self):
            libgmsec_python.ConnectionManagerReplyCallback.__init__(self)

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

    This method is called by the API in response to a reply received
    from a request, from within the Request call. A class derived from
    ConnectionManagerReplyCallback needs to be passed into the
    request() call.

    If a callback is registered to multiple connections, onReply() can
    be invoked concurrently from the different connection threads.

    Note: DO NOT STORE or CHANGE STATE of the ConnectionManager object
    that is passed to the callback method.

    Note: DO NOT STORE the Message objects for use beyond the scope of
    the callback. Otherwise, make a copy of the Message object(s).

    Parameters
    ----------
    connMgr: connection manager on which the message was received
    request: the pending request
    reply: the received reply

    See Also
    --------
    ConnectionManager::request(self, request, timeout, cb, republish_ms = 0)

";
