#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""



""" 
 @file logging.py
 
 This file contains an example outlining the use of the GMSEC API logging
 framework.
"""

import libgmsec_python
import sys

# Define a baseline LogHandler
# This will be used by the Log macros; The implementation of onMessage
# determines how messages will be logged to output

class BaseHandler(libgmsec_python.LogHandler):
    def __init__(self, wai):
        # Use a mutex so messages from different threads don't get mixed up
        libgmsec_python.LogHandler.__init__(self)
        self.mutex = libgmsec_python.Mutex()
        self.whoami = wai

    def onMessage(self, entry):
        self.mutex.enterMutex()

        tempBuffer = "YYYY-DDD-HH:MM:SS.sss"   # Lame, but we need temp space for holding return time buffer
        libgmsec_python.TimeUtil.formatTime(entry.time, tempBuffer)

        msg = "[BaseHandler for " + self.whoami + "]" \
            + " : " + tempBuffer + " [" + libgmsec_python.Log.toString(entry.level) + "]" \
            + " [" + entry.file + ":" + str(entry.line) + "] " \
            + entry.message + "\n"

        print msg.replace("\n", "\n\t")

        self.mutex.leaveMutex()


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

class VerboseHandler(BaseHandler):
    def __init__(self):
        BaseHandler.__init__(self,"GMSEC_VERBOSE")

class DebugHandler(BaseHandler):
    def __init__(self):
        BaseHandler.__init__(self,"GMSEC_DEBUG")

class AnyHandler(BaseHandler):
    def __init__(self):
        BaseHandler.__init__(self,"ANY_HANDLER") 


def main(agrv=None):

    if(len(sys.argv) <= 1):
        usageMessage =  "usage: " + sys.argv[0] + " mw-id=<middleware ID>"
        print usageMessage
        return -1

    config = libgmsec_python.Config()

    for arg in sys.argv[1:]:
        value = arg.split('=')
        config.addValue(value[0], value[1])

    libgmsec_python.Log.registerHandler(None)
    libgmsec_python.Log.setReportingLevel(libgmsec_python.logINFO)

    # Create and register log handlers
    errorHandler   = ErrorHandler()
    warningHandler = WarningHandler()
    infoHandler    = InfoHandler()
    verboseHandler = VerboseHandler()
    debugHandler   = DebugHandler()
    anyHandler     = AnyHandler()

    libgmsec_python.Log.registerHandler(libgmsec_python.logERROR,   errorHandler)
    libgmsec_python.Log.registerHandler(libgmsec_python.logWARNING, warningHandler)
    libgmsec_python.Log.registerHandler(libgmsec_python.logINFO,    infoHandler)
    libgmsec_python.Log.registerHandler(libgmsec_python.logVERBOSE, verboseHandler)
    libgmsec_python.Log.registerHandler(libgmsec_python.logDEBUG,   debugHandler)

    # Set logging reporting level
    libgmsec_python.Log.setReportingLevel(libgmsec_python.logVERBOSE)
    libgmsec_python.logVerbose("The log reporting level is now set to: " + str(libgmsec_python.Log.getReportingLevel()))

    # Print the GMSEC API version number using the GMSEC Logging
    # interface
    # This is useful for determining which version of the API is
    # configured within the environment
    libgmsec_python.logInfo(libgmsec_python.ConnectionManager.getAPIVersion())

    try:
        # Create the ConnectionManager
        connMgr = libgmsec_python.ConnectionManager(config)

        # Connect
        connMgr.initialize()

        # Output middleware client library version
        libgmsec_python.logInfo(connMgr.getLibraryVersion())

        # Publish a message
        publishTestMessage(connMgr, "GMSEC.TEST.PUBLISH")

        # Disconnect from the middleware and clean up the Connection
        connMgr.cleanup()
        
    except libgmsec_python.Exception as e:
        libgmsec_python.logError(e.what())
        return -1
        

    # Unregister log handlers
    libgmsec_python.Log.registerHandler(None)

    # Set log stream to stderr
    config.addValue("LOGFILE", "STDERR")
    libgmsec_python.logInfo("This message should go to stderr, not stdout.  " + "For example, in bash test by running as:\n" + "./logging mw-id=bolt 2> testfile.txt\n" + "... and then check the contents of testfile.txt")

    # Reset log stream to stdout
    config.addValue("LOGFILE", "STDOUT")

    libgmsec_python.logError("This is an example error message.")
    libgmsec_python.logWarning("This is an example warning message.")
    libgmsec_python.logVerbose("This is an example \"verbose\" message.")
    libgmsec_python.logDebug("This is an example debug message which should not show.")

    # This last message cannot be shown right now because
    # Log::setReportingLevel(logVERBOSE), used above, does not
    # allow DEBUG messages to come out.
    libgmsec_python.logVerbose("This is another example \"verbose\" message.")

    # Set logging reporting level to now allow DEBUG messages to be shown
    libgmsec_python.Log.setReportingLevel(libgmsec_python.logDEBUG)

    if (libgmsec_python.Log.getReportingLevel() == libgmsec_python.logDEBUG):
        libgmsec_python.logInfo("Changed reporting level to logDEBUG")
    else:
        libgmsec_python.logError("Failed to change reporting level to logDEBUG")
        
    # The DEBUG message below will be shown successfully, unlike the last
    # debug message.
    libgmsec_python.logDebug("This is an example debug message which should show.")

    libgmsec_python.logDebug("NONE reporting level, numerically, is " + str(libgmsec_python.Log.fromString("NONE")))
    libgmsec_python.logDebug("ERROR reporting level, numerically, is " + str(libgmsec_python.Log.fromString("ERROR")))
    libgmsec_python.logDebug("SECURE reporting level, numerically, is "  + str(libgmsec_python.Log.fromString("SECURE")))
    libgmsec_python.logDebug("WARNING reporting level, numerically, is " + str(libgmsec_python.Log.fromString("WARNING")))
    libgmsec_python.logDebug("INFO reporting level, numerically, is " + str(libgmsec_python.Log.fromString("INFO")))
    libgmsec_python.logDebug("VERBOSE reporting level, numerically, is " + str(libgmsec_python.Log.fromString("VERBOSE")))
    libgmsec_python.logDebug("DEBUG reporting level, numerically, is " + str(libgmsec_python.Log.fromString("DEBUG")))

    # Register general-purpose handler and test
    try:
        libgmsec_python.Log.registerHandler(anyHandler)

    except libgmsec_python.Exception as e:
        print e.what()

    libgmsec_python.logError("NONE reporting level, numerically, is " + str(libgmsec_python.Log.fromString("NONE")))
    libgmsec_python.logError("ERROR reporting level, numerically, is " + str(libgmsec_python.Log.fromString("ERROR")))
    libgmsec_python.logWarning("WARNING reporting level, numerically, is " + str(libgmsec_python.Log.fromString("WARNING")))
    libgmsec_python.logInfo("INFO reporting level, numerically, is " + str(libgmsec_python.Log.fromString("INFO")))
    libgmsec_python.logVerbose("VERBOSE reporting level, numerically, is " + str(libgmsec_python.Log.fromString("VERBOSE")))
    libgmsec_python.logDebug("DEBUG reporting level, numerically, is " + str(libgmsec_python.Log.fromString("DEBUG")))

    # Unregister log handlers
    libgmsec_python.Log.registerHandler(None)

    return 0


def publishTestMessage(connMgr, subject):

    i = 123

    # Create a Message object
    message = libgmsec_python.Message(subject, libgmsec_python.Message.PUBLISH)

    # Add fields to the Message
    message.addField("F", False)
    message.addField(libgmsec_python.I32Field("I", i))
    message.addField(libgmsec_python.U16Field("K", i))
    message.addField("S", "This is a test")
    message.addField(libgmsec_python.F32Field("D", 1 + 1./i))
    message.addField(libgmsec_python.BinaryField("X", "JLMNOPQ", 7))

    # Publish Message 
    connMgr.publish(message)

    # Output the Message's XML string representation by invoking Log macro
    libgmsec_python.logInfo("Sent:\n" + message.toXML())

#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())

