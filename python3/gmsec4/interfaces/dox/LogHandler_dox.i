
%feature("docstring") gmsec::api::util::LogHandler "

    This class is the abstract base class for the logger handler
    callbacks.

    Example LogHandler class:

    class MyLogHandler(libgmsec_python3.LogHandler):
        def __init__(self):
            libgmsec_python3.LogHandler.__init__(self)
            self.mutex = libgmsec_python3.Mutex()

        def on_message(self, entry):
            self.mutex.enter_mutex()

            print(libgmsec_python3.TimeUtil.format_time(entry.time))
            print(libgmsec_python3.Log.to_string(entry.level))
            print(entry.file)
            print(entry.line)
            print(entry.message)

            self.mutex.leave_mutex()

    See Also
    --------
    Log.register_handler()
";

%feature("docstring") gmsec::api::util::LogHandler::onMessage "

    on_message(self, entry: LogEntry)

    This function is called by the API in response to a log message.
    A class derived from LogHandler needs to be registered with
    Log.register_handler() in order to be called for a particular
    logging level.

    Because this class is user defined, it is not considered
    thread-safe unless made so by the user. The use of a Mutex is
    suggested to encapsulate any sensitive code blocks.

    Parameters
    ----------
    entry : The received LogEntry

    Notes
    ----------
    The LogEntry structure is defined as containing:

        time    : The time the log entry was produced
        level   : The severity level of the log level
        file    : The file name from where the log message originated from
        line    : The line number from within the file
        message : The log message
";
