/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::LogEntry "

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

