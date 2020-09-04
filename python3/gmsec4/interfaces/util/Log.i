%module Log
%{
#include <map>
#include <gmsec4/util/Log.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::util::Log::registerHandler(GMSEC_LogHandler*);
%ignore gmsec::api::util::Log::registerHandler(GMSEC_LogLevel, GMSEC_LogHandler*);

%rename("set_reporting_level") setReportingLevel;
%rename("get_reporting_level") getReportingLevel;
%rename("to_string") toString;
%rename("from_string") fromString;
%rename("log_error") logError;
%rename("log_secure") logSecure;
%rename("log_warning") logWarning;
%rename("log_info") logInfo;
%rename("log_verbose") logVerbose;
%rename("log_debug") logDebug;

%include "dox/Log_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/Log.h>

%extend gmsec::api::util::Log {

%pythoncode %{

import libgmsec_python3
import inspect
import os

class DefaultLogHandler(LogHandler):
    def __init__(self):
        LogHandler.__init__(self)

    def on_message(self, entry):
        filename = os.path.basename(entry.file)

        msg = TimeUtil.format_time(entry.time) \
            + " [" + Log.to_string(entry.level) + "]" \
            + " [" + filename + ":" + str(entry.line) + "] " \
            + entry.message + "\n"

        msg = msg.replace("\n", "\n\t")
        print(msg)


defaultLogHandler = DefaultLogHandler()

PyLogHandlers = {
    logERROR   : defaultLogHandler,
    logSECURE  : defaultLogHandler,
    logWARNING : defaultLogHandler,
    logINFO    : defaultLogHandler,
    logVERBOSE : defaultLogHandler,
    logDEBUG   : defaultLogHandler
}


# Python does not allow for the overloading of methods; thus we conjure up a
# method that user can use to register a custom LogHandler, and optionally
# provide a LogLevel.
#
@staticmethod
def register_handler(logHandler, level = None):

    """
    register_handler(handler: LogHandler, level=None: int)

    This function registers the given LogHandler for all logging levels if the
    level is not provided (or is None), otherwise the function registers the
    LogHandler for the specific logging level.

    Parameters
    ----------
    handler : User-defined instance of a class whose base class is LogHandler; or None
              to rely on the default LogHandler provided by the GMSEC API.
    level   : Optional LogLevel with possible values logERROR, logSECURE, logWARNING, or logINFO.

    Note
    ----------
    Due to constraints with GMSEC middlware wrappers, any attempt to register a LogHandler for
    LogLevels of logVERBOSE and logDEBUG will be ignored.
    """

    if logHandler != None and not isinstance(logHandler, LogHandler):
        raise TypeError("register_handler arg1 must be a LogHandler")
    if level != None and not isinstance(level, int):
        raise TypeError("register_handler arg2 must be an int representing a log level (e.g. logERROR, logSECURE, etc.)")

    handler = Log.defaultLogHandler if logHandler == None else logHandler

    if level == None:
        Log.PyLogHandlers[logERROR]   = handler
        Log.PyLogHandlers[logSECURE]  = handler
        Log.PyLogHandlers[logWARNING] = handler
        Log.PyLogHandlers[logINFO]    = handler
        Log.PyLogHandlers[logVERBOSE] = handler
        Log.PyLogHandlers[logDEBUG]   = handler

        # Do NOT register handler with the Core API for logVERBOSE or logDEBUG levels;
        # log messages can originate from a middleware wrapper 'long' after the LogHandler
        # if garbage collected.
        for l in range (logERROR, logVERBOSE):
            Log.registerHandler(l, handler)

    else:
        if Log.to_string(level) == "INVALID":
            raise TypeError("register_handler arg1 has illegal LogLevel value")

        Log.PyLogHandlers[level] = handler

        # Do NOT register handler with the Core API for logVERBOSE or logDEBUG levels;
        # log messages can originate from a middleware wrapper 'long' after the LogHandler
        # if garbage collected.
        if level != logVERBOSE and level != logDEBUG:
            Log.registerHandler(level, handler)


@staticmethod
def __make_log_entry__(frame, level, message):
    entry         = LogEntry()
    entry.time    = TimeUtil.get_current_time()
    entry.file    = frame.f_back.f_code.co_filename
    entry.line    = frame.f_back.f_lineno
    entry.level   = level
    entry.message = message
    return entry
%}

};


%pythoncode %{

import inspect
import os

def log_error(msg):
    if logERROR > Log.get_reporting_level():
        return
    entry = Log.__make_log_entry__(inspect.currentframe(), logERROR, msg)
    Log.PyLogHandlers[logERROR].on_message(entry)


def log_secure(msg):
    if logSECURE > Log.get_reporting_level():
        return
    entry = Log.__make_log_entry__(inspect.currentframe(), logSECURE, msg)
    Log.PyLogHandlers[logSECURE].on_message(entry)


def log_warning(msg):
    if logWARNING > Log.get_reporting_level():
        return
    entry = Log.__make_log_entry__(inspect.currentframe(), logWARNING, msg)
    Log.PyLogHandlers[logWARNING].on_message(entry)


def log_info(msg):
    if logINFO > Log.get_reporting_level():
        return
    entry = Log.__make_log_entry__(inspect.currentframe(), logINFO, msg)
    Log.PyLogHandlers[logINFO].on_message(entry)
        

def log_verbose(msg):
    if logVERBOSE > Log.get_reporting_level():
        return
    entry = Log.__make_log_entry__(inspect.currentframe(), logVERBOSE, msg)
    Log.PyLogHandlers[logVERBOSE].on_message(entry)


def log_debug(msg):
    if logDEBUG > Log.get_reporting_level():
        return
    entry = Log.__make_log_entry__(inspect.currentframe(), logDEBUG, msg)
    Log.PyLogHandlers[logDEBUG].on_message(entry)

%}
