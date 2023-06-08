/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::util::Log "

    This class defines the logging interface.
";

%feature("docstring") gmsec::api5::util::Log::toString "

    to_string(level: int) -> str

    This function converts the log level number to a string value

    Parameters
    ----------
    level: LogLevel with possible values logNONE, logERROR, logSECURE, logWARNING, logINFO, logVERBOSE, or logDEBUG

    Returns
    -------
    String with possibilities of \"NONE\", \"ERROR\", \"SECURE\", \"WARNING\", \"INFO\", \"VERBOSE\", \"DEBUG\", or \"INVALID\"
";

%feature("docstring") gmsec::api5::util::Log::fromString "

    from_string(level: str) -> int

    This function converts the string value to the log level number

    Parameters
    ----------
    level : Can be one of \"NONE\", \"ERROR\", \"SECURE\", \"WARNING\", \"INFO\", \"VERBOSE\", or \"DEBUG\"

    Returns
    -------
    LogLevel with possible values logNONE, logERROR, logSECURE, logWARNING, logINFO, logVERBOSE, or logDEBUG
";

%feature("docstring") gmsec::api5::util::Log::setReportingLevel "

    set_reporting_level(level: int)

    This function sets the maximum logging level

    Parameters
    ----------
    level : LogLevel with possible values logNONE, logERROR, logSECURE, logWARNING, logINFO, logVERBOSE, or logDEBUG

";

%feature("docstring") gmsec::api5::util::Log::getReportingLevel "

    get_reporting_level() -> int

    This function returns the maximum logging level set by the library
    or the maximum logging level set by the user

    Returns
    -------
    LogLevel with possible values logNONE, logERROR, logSECURE, logWARNING, logINFO, logVERBOSE, or logDEBUG
";

