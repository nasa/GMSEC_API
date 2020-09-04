
// File: classlibgmsec__python_1_1Callback.xml


%feature("docstring") gmsec::api::Callback "

  This class is the abstract base class for received message
  callbacks.

  A user created class, derived from this class, can be passed into
  subscribe() to have user code executed asynchronously
  when a message is received.

  Please note that because users are able to create their own Callback
  class, reentrancy is not guaranteed unless if reentrancy rules are
  specified.

  In addition, if a Callback is registered to multiple connections,
  onMessage() can be invoked concurrently from different connection
  threads. Use of a Mutex is suggested to enforce thread safety.

  Example Callback class:
  class PublishCallback(libgmsec_python.Callback):
    def __init__(self):
        libgmsec_python.Callback.__init__(self)

    def onMessage(self, conn, msg):
    	print msg.toXML() + \"\\n\";

  Example Callback registration:
  cb = PublishCallback();
  connsubscribe(\"GMSEC.*.PUBLISH\", cb);

  Connection::subscribe(const char* subject, Callback* cb)
";

%feature("docstring") gmsec::api::Callback::onMessage "

    onMessage(self, conn, msg)

    This method is called by the API in response to a message, from
    either the dispatch() call or inside the Auto-dispatcher after a
    startAutoDispatch() call. A class derived from Callback needs to
    be registered with a connection, using subscribe(), in order to be
    called for a particular subject registration pattern.

    Note that if a Callback is registered to multiple connections, onMessage()
    can be invoked concurrently from different connection threads.

    Note: DO NOT DESTROY or CHANGE STATE of the Connection object that is
    passed to the callback method, nor store it for use beyond the scope
    of the callback method.

    Note: DO NOT STORE the Message object for use beyond the scope of the
    callback. Otherwise, make a copy of the Message object.

    Parameters
    ----------
    conn: connection on which the message was received
    msg: the received message
";

