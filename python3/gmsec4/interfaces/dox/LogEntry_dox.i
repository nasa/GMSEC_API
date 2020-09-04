
%feature("docstring") gmsec::api::LogEntry "

    The class that is passed to a LogHandler object when a message is
    logged.

    The structure is defined as containing:

    time    : The time the log entry was produced
    level   : The severity level of the log level
    file    : The file name from where the log message originated from
    line    : The line number from within the file
    message : The log message

    See Also
    --------
    Log
    LogHandler
";

