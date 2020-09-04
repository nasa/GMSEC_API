
// File: classlibgmsec__python_1_1Log.xml


%feature("docstring") gmsec::api::util::Log "

    This class defines the logging interface.
";

%feature("docstring") gmsec::api::util::Log::registerHandler "

    registerHandler(handler)

    This function registers a LogHandler that will be called for all logging levels.

    Parameters
    ----------
    handler: Instance of a class whose base class is LogHandler.

    registerHandler(level, handler)

    This function registers a LogHandler that will be called for a specific logging level.

    Parameters
    ----------
    level: LogLevel with possible values logNONE, logERROR,
        logSECURE, logWARNING, logINFO, logVERBOSE, or
        logDEBUG
    handler: Instance of a class whose base class is LogHandler.

";

%feature("docstring") gmsec::api::util::Log::toString "

    toString(level) -> char const *

    This function converts the log level number to a string value

    Parameters
    ----------
    level: LogLevel with possible values logNONE, logERROR,
        logSECURE, logWARNING, logINFO, logVERBOSE, or
        logDEBUG

    Returns
    -------
    static const char* pointing to possibilities \"NONE,\"
    \"ERROR,\" \"SECURE,\" \"WARNING,\" \"INFO,\" \"VERBOSE,\" \"DEBUG,\"
    or \"INVALID\"
";

%feature("docstring") gmsec::api::util::Log::setReportingLevel "

    setReportingLevel(level)

    This function sets the maximum logging level

    Parameters
    ----------
    level: LogLevel with possible values logNONE, logERROR,
        logSECURE, logWARNING, logINFO, logVERBOSE, or
        logDEBUG

";

%feature("docstring") gmsec::api::util::Log::fromString "

    fromString(level) -> LogLevel

    This function converts the string value to the log level number

    Parameters
    ----------
    level: function expects possibilities \"NONE,\"
        \"ERROR,\" \"SECURE,\" \"WARNING,\" \"INFO,\" \"VERBOSE,\" or \"DEBUG\"

    Returns
    -------
    LogLevel with possible values logNONE, logERROR,
    logSECURE, logWARNING, logINFO, logVERBOSE, or
    logDEBUG

";

%feature("docstring") gmsec::api::util::Log::getReportingLevel "

    getReportingLevel() -> LogLevel

    This function returns the maximum logging level set by the library or
    the maximum logging level set by the user

    Returns
    -------
    LogLevel with possible values logNONE, logERROR,
    logSECURE, logWARNING, logINFO, logVERBOSE, or logDEBUG

    getReportingLevel() -> LogLevel
";

