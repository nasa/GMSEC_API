#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""



"""
 @file request_async.py
 
 This file contains an example demonstrating how to asynchronously handle the
 receipt of reply message(s) after issuing a request.
 Asynchronous request operations essentially mean that a Callback subclass is
 implemented, then a corresponding Callback object is supplied when invoking
 request. Reply messages that are then received asynchronously and handled by
 the Callback object.

 This example program is intended to be run after either starting up the
 'reply' or 'reply_async' example program.
"""

import libgmsec_python
import sys

DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST"

# Globals
receivedReply = False


class ExampleReplyCallback(libgmsec_python.ConnectionManagerReplyCallback):

    def __init__(self):
        libgmsec_python.ConnectionManagerReplyCallback.__init__(self)
        self.receivedReply = False
                                                                  
    def onReply(self, connection, request, reply):
        # Display XML representation of reply message
        libgmsec_python.logInfo("[ExampleReplyCallback::onReply]\n" + reply.toXML())
        self.receivedReply = True
        
    def onEvent(self, connection, status, event):
        if (status.isError()):
            libgmsec_python.logError("[ExampleReplyCallback::onError] " + status.get() + ", event=" + event)


def main():

    if(len(sys.argv) <= 1):
        usageMessage = "usage: " +  sys.argv[0] + " mw-id=<middleware ID>"
        print usageMessage
        return -1


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

    # Output GMSEC API version
    libgmsec_python.logInfo(libgmsec_python.Connection.getAPIVersion())

    try:
        # Create the Connection
        connMgr = libgmsec_python.ConnectionManager(config)

        # Connect
        connMgr.initialize()

        # output connection middleware version
        libgmsec_python.logInfo(connMgr.getLibraryVersion())

        # Create request message
        requestMsg = libgmsec_python.Message(DEFAULT_REQUEST_SUBJECT, libgmsec_python.Message.REQUEST)

        # Add fields to request message
        requestMsg.addField("QUESTION", "Does the request/reply functionality still work?")
        requestMsg.addField("COMPONENT", "request_async")

        # Display XML representation of request message
        libgmsec_python.logInfo("Requesting:\n" + requestMsg.toXML())

        cb = ExampleReplyCallback()
        connMgr.request(requestMsg, -1, cb)

        libgmsec_python.logInfo("Waiting for response...")

        # Loop while waiting for the asynchronous response until done
        while (cb.receivedReply == 0):
            libgmsec_python.TimeUtil.millisleep(100)
                
        if (cb.receivedReply):
            libgmsec_python.logInfo("Response Received!")
        else:
            libgmsec_python.logWarning("No response received")

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

#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())
        
