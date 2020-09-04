
// File: classlibgmsec__python_1_1LogHandler.xml


%feature("docstring") gmsec::api::util::LogHandler "

    This class is the abstract base class for the logger handler callbacks.

    Example LogHandlerclass:

    class MyLogHandler(libgmsec_python.LogHandler):
        def __init__(self):
            libgmsec_python.LogHandler.__init__(self)

        def onMessage(self, entry):
            print(entry.message)

    See Also
    --------
    Log.registerHandler()
";

%feature("docstring") gmsec::api::util::LogHandler::onMessage "

    onMessage(self, entry)

    This function is called by the API in response to a log message. A class derived from
    gmsec::api::util::LogHandler needs to be registered with Log::registerHandler() in order
    to be called for a particular logging level.

    Because this class is user defined, it is not considered thread-safe unless made so by the
    user.  The use of a Mutex is suggested to encapsulate any sensitive code blocks.

    Parameters
    ----------
    entry: the received log entry

    Notes
    ----------
    The LogEntry structure is defined as containing:

        time    : The time the log entry was produced
        level   : The severity level of the log level
        file    : The file name from where the log message originated from
        line    : The line number from within the file
        message : The log message


";
