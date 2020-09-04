#!/usr/bin/env python


"""
 Copyright 2007-2017 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""



""" 
 @file connection_state.py
 
 This file contains an example which demonstrates how to utilize
 EventCallbacks to track the state of the GMSEC Connection and its connection
 to the middleware.
"""

import libgmsec_python
import sys

EXAMPLE_MESSAGE_SUBJECT = "GMSEC.TEST.PUBLISH"

# Globals
eventFired = False

# Define an EventCallback to report the status of WebSphere async publications
# Note: The WebSphere MQ client libraries only report the first warning or
# failure code encountered during the series of message publications made
# since the last time that the client libraries retrieved the status of
# asynchronous publish operations.

class ConnectionStateCallback(libgmsec_python.ConnectionManagerEventCallback):

    def __init__(self):
        libgmsec_python.ConnectionManagerEventCallback.__init__(self)

    def onEvent(self, connMgr, status, event):
        
        # Perform some action based on the type of event that occurred
        if (event == libgmsec_python.Connection.CONNECTION_SUCCESSFUL_EVENT):
        
            libgmsec_python.logInfo("Connected to the middleware server")
                
        elif (event == libgmsec_python.Connection.CONNECTION_BROKEN_EVENT):
                
            libgmsec_python.logInfo("Connection to the middleware lost or terminated")
    
        elif (event == libgmsec_python.Connection.CONNECTION_RECONNECT_EVENT):

            libgmsec_python.logInfo("Attempting to reestablish the connection to the middleware")


        eventFired = True


def main():

    if(len(sys.argv) <= 1):        
        usageMessage = "usage: " + sys.argv[0] + " mw-id=<middleware ID>"
        print usageMessage

    cb = ConnectionStateCallback()

    # Load the command-line input into a GMSEC Config object
    config = libgmsec_python.Config()

    for arg in sys.argv[1:]:
        value = arg.split('=')
        config.addValue(value[0], value[1])


    # If it was not specified in the command-line arguments, set LOGLEVEL
    # to 'INFO' and LOGFILE to 'stdout' to allow the program report output
    # on the terminal/command line
    initializeLogging(config)

    # Toggle the WebSphere MQ client libraries-level asynchronous publish
    # functionality on
    config.addValue("MW-ASYNC-PUBLISH", "true")

    # Toggle the periodic WebSphere MQ asynchronous publish status report
    # functionality on.  The GMSEC API Connection will periodically fire
    # off an Event which will trigger the onEvent function of an
    # EventCallback registered to the event labeled,
    # Connection::WSMQ_ASYNC_STATUS_CHECK_EVENT
    config.addValue("MW-ASYNC-STATUS-CHECK", "true")

    # Additionally, the "MW-ASYNC-STATUS-CHECK-MESSAGE-INTERVAL"
    # configuration option can be used to instruct the GMSEC Connection
    # on how frequently (in number of publish operations) it should
    # periodically fire the WSMQ_ASYNC_STATUS_CHECK_EVENT. By default, it
    # will fire once every 100 messages.
    # For the sake of this example, we will use 500 messages. 
    config.addValue("MW-ASYNC-STATUS-CHECK-MESSAGE-INTERVAL", "500")

    # Print the GMSEC API version number using the GMSEC Logging
    # interface
    # TODO: Once available, replace this statement with usage of
    # ConnectionManager::getAPIVersion (See RTC 4798)
    libgmsec_python.logInfo(libgmsec_python.Connection.getAPIVersion())

    try:
                
        # Create the Connection
        connMgr = libgmsec_python.ConnectionManager(config)
        # Connect
        connMgr.initialize()


        # Register the event callback with the connection to catch
        # connection state changes, including:
        # 1. Connection successfully established to middleware
        # 2. Connection broken from middleware
        # 3. Reconnecting to the middleware

        connMgr.registerEventCallback(libgmsec_python.Connection.CONNECTION_SUCCESSFUL_EVENT, cb)
        connMgr.registerEventCallback(libgmsec_python.Connection.CONNECTION_BROKEN_EVENT, cb)
        connMgr.registerEventCallback(libgmsec_python.Connection.CONNECTION_RECONNECT_EVENT, cb)

        # Output middleware version
        libgmsec_python.logInfo("Middleware version = " + connMgr.getLibraryVersion())

        # Assuming that the user provided proper reconnection syntax
        # for the corresponding middleware they are using, one could
        # kill the middleware server at this point, then start it back
        # up to demonstrate the EventCallback.onEvent() function
        # triggering
            
        # Wait for user input to end the program
        libgmsec_python.logInfo("Waiting for Connection events to occur, press <enter> to exit the program")

        raw_input("")

        # Clean up the ConnectionManager before exiting the program
        connMgr.cleanup()
        
    except libgmsec_python.Exception as e:
            
        libgmsec_python.logError(e.what())
        return -1

    return 0


def initializeLogging(config):
    
    logLevel  = config.getValue("LOGLEVEL")
    logFile   = config.getValue("LOGFILE")

    if (not logLevel):
        
        config.addValue("LOGLEVEL", "INFO")
      
    if (not logFile):
        
        config.addValue("LOGFILE", "STDERR")


def populateMessage(message, value):
    
    # Note: Since C++ is a strongly-typed language, it is possible to use
    # type casting to add Field objects to a Message without having to
    # first create a Field object, then add it to the message.
    message.addField("CHAR-FIELD", 'c')
    message.addField("BOOL-FIELD-TRUE", True)
    message.addField("BOOL-FIELD-FALSE", False)
    message.addField(libgmsec_python.I8Field("I8-FIELD", value))
    message.addField(libgmsec_python.I16Field("I16-FIELD", value))
    message.addField(libgmsec_python.I32Field("I32-FIELD", value))
    message.addField(libgmsec_python.I64Field("I64-FIELD", value))
    message.addField(libgmsec_python.U8Field("U8-FIELD", value))
    message.addField(libgmsec_python.U16Field("U16-FIELD", value))
    message.addField(libgmsec_python.U32Field("U32-FIELD", value))
    message.addField(libgmsec_python.U64Field("U64-FIELD", value))
    message.addField("STRING-FIELD", "This is a test")
    message.addField(libgmsec_python.F32Field("F32-FIELD", (1 + 1. / value)))
    message.addField(libgmsec_python.F64Field("F64-FIELD", (1 + 1. / value)))
    message.addField(libgmsec_python.BinaryField("BIN-FIELD", "JLMNOPQ", 7))


#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())

