
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
    multiple Connection Manager objects, on_reply() can be invoked
    concurrently from different threads. Use of a Mutex is suggested
    to enforce thread safety.

    Example Callback class:
    class MyReplyCallback(libgmsec_python3.ConnectionManagerReplyCallback):
        def __init__(self):
            libgmsec_python3.ConnectionManagerReplyCallback.__init__(self)
            self.mutex = libgmsec_python3.Mutex()

        def on_reply(self, connMgr, status, event):
            self.mutex.enter_mutex()
            print(request.toXML())
            print(reply.toXML())
            self.mutex.leave_mutex()

        def on_event(self, connMgr, status, event):
            self.mutex.enter_mutex()
            print(status.get())
            self.mutex.leave_mutex()

    Example ConnectionManagerReplyCallback registration:
    cb = MyReplyCallback()
    reqMsg = libgmsec_python3.Message(\"GMSEC.MISSION.SAT.REQ.DIR\",
                              libgmsec_python3.python.Message.REQUEST)
    timeout = 1000

    try:
        connMgr.request(reqMsg, timeout, cb)
    except GmsecError as e:
        print(str(e))
";

%feature("docstring") gmsec::api::mist::ConnectionManagerReplyCallback::onReply "

    on_reply(self, connMgr: ConnectionManager, request: Message, reply: Message)

    This method is called by the API in response to a reply received
    from a request, from within the Request call. A class derived from
    ConnectionManagerReplyCallback needs to be passed into the
    request() call.

    If a callback is registered to multiple connections, on_reply() can
    be invoked concurrently from the different connection threads.

    Note: DO NOT STORE or CHANGE STATE of the ConnectionManager object
    that is passed to the callback method.

    Note: DO NOT STORE the Message objects for use beyond the scope of
    the callback. Otherwise, make a copy of the Message object(s).

    Parameters
    ----------
    connMgr : ConnectionManager on which the message was received
    request : The request Message
    reply   : The reply Message

    See Also
    --------
    ConnectionManager::request(self, request, timeout, cb, republish_ms)
";
