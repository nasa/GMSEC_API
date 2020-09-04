
// File: classlibgmsec__python_1_1LogHandler.xml


%feature("docstring") gmsec::api::util::LogHandler "

    This class is the abstract base class for the logger handler callbacks.

    See Also
    --------
    Log
";

%feature("docstring") gmsec::api::util::LogHandler::onMessage "

    onMessage(self, entry)

    This function is called by the API in response to a log message. A class derrived from
    gmsec::api::util::LogHandler needs to be registered with Log::registerHandler() in order
    to be called for a particular logging level.

    Because this class is user defined, it is not considered thread-safe unless made so by the
    user.  The use of a gmsec::util::AutoMutex is suggested to encapsulate any sensitive code
    blocks.

    Parameters
    ----------
    entry: the received log entry

";
