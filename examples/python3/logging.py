#!/usr/bin/env python3

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


""" 
 @file logging.py
 
 This file contains an example outlining the use of the GMSEC API logging
 framework.
"""

import libgmsec_python3
import sys

# Define a baseline LogHandler
# This will be used by the Log macros; The implementation of on_message
# determines how messages will be logged to output

class BaseHandler(libgmsec_python3.LogHandler):

    def __init__(self, wai):
        # Use a mutex so messages from different threads don't get mixed up
        libgmsec_python3.LogHandler.__init__(self)
        self.mutex = libgmsec_python3.Mutex()
        self.whoami = wai

    def on_message(self, entry):
        self.mutex.enter_mutex()

        tempBuffer = libgmsec_python3.TimeUtil.format_time(entry.time)

        msg = "[BaseHandler::on_message] for: " + self.whoami \
            + " : " + tempBuffer + " [" + libgmsec_python3.Log.to_string(entry.level) + "]" \
            + " [" + entry.file + ":" + str(entry.line) + "] " + entry.message + "\n" 

        print(msg.replace("\n", "\n\t"))

        self.mutex.leave_mutex()


# Different Handlers can be implemented for each logging level, if so desired
class ErrorHandler(BaseHandler):
    def __init__(self):
        BaseHandler.__init__(self,"GMSEC_ERROR")

class WarningHandler(BaseHandler):
    def __init__(self):
        BaseHandler.__init__(self,"GMSEC_WARNING")

class InfoHandler(BaseHandler):
    def __init__(self):
        BaseHandler.__init__(self,"GMSEC_INFO")

class AnyHandler(BaseHandler):
    def __init__(self):
        BaseHandler.__init__(self,"ANY_HANDLER") 


def main(agrv=None):

    if(len(sys.argv) <= 1):
        usageMessage =  "usage: " + sys.argv[0] + " mw-id=<middleware ID>"
        print(usageMessage)
        return -1

    config = libgmsec_python3.Config(sys.argv)

    # Create and register log handlers
    errorHandler   = ErrorHandler()
    warningHandler = WarningHandler()
    infoHandler    = InfoHandler()
    anyHandler     = AnyHandler()

    libgmsec_python3.Log.register_handler(errorHandler,   libgmsec_python3.logERROR)
    libgmsec_python3.Log.register_handler(warningHandler, libgmsec_python3.logWARNING)
    libgmsec_python3.Log.register_handler(infoHandler,    libgmsec_python3.logINFO)
    # Note: At this time, for the Python3 binding, setting log handlers for logVERBOSE and logDEBUG are not permitted,
    # This is because certain middleware wrappers output log messages after the custom Python log handler
    # has been garbage collected, and this can lead to fatal errors.
    libgmsec_python3.Log.register_handler(anyHandler, libgmsec_python3.logVERBOSE)   # has no effect, but shown for demo purposes
    libgmsec_python3.Log.register_handler(anyHandler,   libgmsec_python3.logDEBUG)     # has no effect, but shown for demo purposes

    # Set logging reporting level
    libgmsec_python3.Log.set_reporting_level(libgmsec_python3.logVERBOSE)
    libgmsec_python3.log_verbose("The log reporting level is now set to: " + str(libgmsec_python3.Log.get_reporting_level()))

    # Print the GMSEC API version number using the GMSEC Logging
    # interface
    # This is useful for determining which version of the API is
    # configured within the environment
    libgmsec_python3.log_info(libgmsec_python3.ConnectionManager.get_API_version())

    try:
        # Create the ConnectionManager
        connMgr = libgmsec_python3.ConnectionManager(config)

        # Connect
        connMgr.initialize()

        # Output middleware client library version
        libgmsec_python3.log_info(connMgr.get_library_version())

        # Publish a message
        publishTestMessage(connMgr, "GMSEC.TEST.PUBLISH")

        # Disconnect from the middleware and clean up the Connection
        connMgr.cleanup()
        
    except libgmsec_python3.GmsecError as e:
        libgmsec_python3.log_error(str(e))
        return -1
        

    # Unregister log handlers
    libgmsec_python3.Log.register_handler(None)

    # Set log stream to stderr
    config.add_value("LOGFILE", "STDERR")
    libgmsec_python3.log_info("This message should go to stderr, not stdout.  " + "For example, in bash test by running as:\n" + "./logging mw-id=bolt 2> testfile.txt\n" + "... and then check the contents of testfile.txt")

    # Reset log stream to stdout
    config.add_value("LOGFILE", "STDOUT")

    libgmsec_python3.log_error("This is an example error message.")
    libgmsec_python3.log_warning("This is an example warning message.")
    libgmsec_python3.log_verbose("This is an example \"verbose\" message.")
    libgmsec_python3.log_debug("This is an example debug message which should not show.")

    # This last message cannot be shown right now because
    # Log::set_reporting_level(logVERBOSE), used above, does not
    # allow DEBUG messages to come out.
    libgmsec_python3.log_verbose("This is another example \"verbose\" message.")

    # Set logging reporting level to now allow DEBUG messages to be shown
    libgmsec_python3.Log.set_reporting_level(libgmsec_python3.logDEBUG)
    if (libgmsec_python3.Log.get_reporting_level() == libgmsec_python3.logDEBUG):
        libgmsec_python3.log_info("Changed reporting level to logDEBUG")
        
    else:
        libgmsec_python3.log_error("Failed to change reporting level to logDEBUG")
        
    # The DEBUG message below will be shown successfully, unlike the last
    # debug message.
    libgmsec_python3.log_debug("This is an example debug message which should show.")

    libgmsec_python3.log_debug("NONE reporting level, numerically, is " + str(libgmsec_python3.Log.from_string("NONE")))
    libgmsec_python3.log_debug("ERROR reporting level, numerically, is " + str(libgmsec_python3.Log.from_string("ERROR")))
    libgmsec_python3.log_debug("SECURE reporting level, numerically, is "  + str(libgmsec_python3.Log.from_string("SECURE")))
    libgmsec_python3.log_debug("WARNING reporting level, numerically, is " + str(libgmsec_python3.Log.from_string("WARNING")))
    libgmsec_python3.log_debug("INFO reporting level, numerically, is " + str(libgmsec_python3.Log.from_string("INFO")))
    libgmsec_python3.log_debug("VERBOSE reporting level, numerically, is " + str(libgmsec_python3.Log.from_string("VERBOSE")))
    libgmsec_python3.log_debug("DEBUG reporting level, numerically, is " + str(libgmsec_python3.Log.from_string("DEBUG")))

    # Register general-purpose handler and test
    try:
        libgmsec_python3.Log.register_handler(anyHandler)

    except libgmsec_python3.GmsecError as e:
        print(str(e))

    libgmsec_python3.log_error("NONE reporting level, numerically, is " + str(libgmsec_python3.Log.from_string("NONE")))
    libgmsec_python3.log_error("ERROR reporting level, numerically, is " + str(libgmsec_python3.Log.from_string("ERROR")))
    libgmsec_python3.log_warning("WARNING reporting level, numerically, is " + str(libgmsec_python3.Log.from_string("WARNING")))
    libgmsec_python3.log_info("INFO reporting level, numerically, is " + str(libgmsec_python3.Log.from_string("INFO")))
    libgmsec_python3.log_verbose("VERBOSE reporting level, numerically, is " + str(libgmsec_python3.Log.from_string("VERBOSE")))
    libgmsec_python3.log_debug("DEBUG reporting level, numerically, is " + str(libgmsec_python3.Log.from_string("DEBUG")))

    # Unregister log handlers
    libgmsec_python3.Log.register_handler(None)

    return 0


def publishTestMessage(connMgr, subject):

    i = 123

    # Create a Message object
    message = libgmsec_python3.Message(subject, libgmsec_python3.Message.PUBLISH)

    # Add fields to the Message
    message.add_field("F", False)
    message.add_field(libgmsec_python3.I32Field("I", i))
    message.add_field(libgmsec_python3.U16Field("K", i))
    message.add_field("S", "This is a test")
    message.add_field(libgmsec_python3.F32Field("D", 1 + 1./i))
    message.add_field(libgmsec_python3.BinaryField("X", "JLMNOPQ", 7))

    # Publish Message 
    connMgr.publish(message)

    # Output the Message's XML string representation by invoking Log macro
    libgmsec_python3.log_info("Sent:\n" + message.to_XML())


#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())

