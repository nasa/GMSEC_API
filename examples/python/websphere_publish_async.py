#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""



""" 
 @file websphere_publish_async.py
 
 This file contains an example which demonstrates how to utilize the
 WebSphere MQ asynchronous publish functionality, as well as how to check on
 the status of publish operations (Since the client does not wait to receive
 the status of each operation in order to achieve asynchronicity) by
 registering an EventCallback with the ConnectionManager.

 It is recommended that users execute the program GMSEC_API/bin/gmsub to
 receive the messages that this example publishes.
"""

import libgmsec_python
import sys

EXAMPLE_MESSAGE_SUBJECT = "GMSEC.TEST.PUBLISH"

# Define an EventCallback to report the status of WebSphere async publications
# Note: The WebSphere MQ client libraries only report the first warning or
# failure code encountered during the series of message publications made
# since the last time that the client libraries retrieved the status of
# asynchronous publish operations.

class AsyncStatusCheckCallback(libgmsec_python.ConnectionManagerEventCallback):

    def __init__(self):
        libgmsec_python.ConnectionManagerEventCallback.__init__(self)
        self.eventFired = False

    def onEvent(self, connMgr, status, event):
        # Print the status of publish operations.  This includes counts
        # for successes, warnings, and errors.
        libgmsec_python.logInfo(status.getReason())

        if (status.isError()):
            libgsmsec_python.logError("The first occurrence of a WebSphere MQ Asynchronous Put Response warning or failure returned the WebSphere Reason Code: " + status.getCustomCode())

        self.eventFired = True
        

def main():

    if(len(sys.argv) <= 1):
        usageMessage = "usage: " +  sys.argv[0] + " mw-id=<middleware ID>"
        print usageMessage
        return -1


    cb = AsyncStatusCheckCallback()

    # Load the command-line input into a GMSEC Config object
    # A Config object is basically a key-value pair map which is used to
    # pass configuration options into objects such as Connections,
    # ConnectionManagers, Subscribe and Publish function calls, Messages,
    # etc.
    config = libgmsec_python.Config()

    for arg in sys.argv[1:]:
        value = arg.split('=')
        config.addValue(value[0], value[1])

    # Since this example program uses an invalid message, we ensure the
    # validation check is disabled.
    config.addValue("gmsec-msg-content-validate-all", "false")

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
    libgmsec_python.logInfo(libgmsec_python.ConnectionManager.getAPIVersion())

    try:
        # Create the Connection
        connMgr = libgmsec_python.ConnectionManager(config)

        # Connect
        connMgr.initialize()

        # Register the event callback with the connection to catch
        # the WebSphere asynchronous publish status events which are
        # eminated from the API
        connMgr.registerEventCallback(libgmsec_python.Connection.WSMQ_ASYNC_STATUS_CHECK_EVENT, cb)
        # Output middleware version
        libgmsec_python.logInfo("Middleware version = " + connMgr.getLibraryVersion())

        libgmsec_python.logInfo("Publishing messages using the subject: " + EXAMPLE_MESSAGE_SUBJECT)

        # Create a GMSEC Message object
        message = libgmsec_python.Message(EXAMPLE_MESSAGE_SUBJECT, libgmsec_python.Message.PUBLISH)

        # Publish message as quickly as possible
        # (i.e. No sleep operation between each publish operation)
        count = 0
        while (not cb.eventFired):
            # Populate the Message with fields, increment a
            # counter so that a publisher can track how many
            # messages were published (if they are interested)
            count += 1
            populateMessage(message, count)

            # Publish the message to the middleware bus
            connMgr.publish(message)

            # Note: We do not recommend printing the output of a
            # message when publishing it while using the WebSphere
            # async publish functionality, as it is
            # counter-intuitive to take take up CPU resources
            # performing I/O operations when attempting to achieve
            # high message throughput rates. As such, if you want
            # to analyze the messages published by this program,
            # we recommend you execute GMSEC_API/bin/gmsub to
            # receive the messages.
                

        libgmsec_python.logInfo("Event detected, ending example demonstration")

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
    message.addField(libgmsec_python.CharField("CHAR-FIELD", 'c'))
    message.addField(libgmsec_python.BooleanField("BOOL-FIELD-TRUE", True))
    message.addField(libgmsec_python.BooleanField("BOOL-FIELD-FALSE", False))
    message.addField(libgmsec_python.I8Field("I8-FIELD", value % 128))
    message.addField(libgmsec_python.I16Field("I16-FIELD", value))
    message.addField(libgmsec_python.I32Field("I32-FIELD", value))
    message.addField(libgmsec_python.I64Field("I64-FIELD", value))
    message.addField(libgmsec_python.U8Field("U8-FIELD", value % 256))
    message.addField(libgmsec_python.U16Field("U16-FIELD", value))
    message.addField(libgmsec_python.U32Field("U32-FIELD", value))
    message.addField(libgmsec_python.U64Field("U64-FIELD", value))
    message.addField(libgmsec_python.StringField("STRING-FIELD", "This is a test"))
    message.addField(libgmsec_python.F32Field("F32-FIELD", (1 + 1. / value)))
    message.addField(libgmsec_python.F64Field("F64-FIELD", (1 + 1. / value)))
    message.addField(libgmsec_python.BinaryField("BIN-FIELD", "JLMNOPQ", 7))


#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())

