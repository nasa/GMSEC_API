#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""



"""
 @file reply_async.py
 
 This file contains an example demonstrating how to asynchronously reply to a
 request message.

 Asynchronous receipt operations essentially mean that a Callback subclass is
 implemented, then the corresponding Callback object is registered to a
 subscription, and that messages are received asynchronously after starting
 the AutoDispatcher. In this case, replies are created and sent asynchronously
 when receiving request messages.

 This example program is intended to be run before starting up either the
 'request' or 'request_async' example program.
"""

import libgmsec_python
import sys

DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST"
# Globals
replySent = False


class ExampleCallback(libgmsec_python.ConnectionManagerCallback):

    def __init__(self):
        libgmsec_python.ConnectionManagerCallback.__init__(self)
        self.replySent = False
    
    def onMessage(self, connMgr, message):
        
        try:
            # Display the XML representation of the received message
            libgmsec_python.logInfo("[ExampleCallback:onMessage] Received:\n" + message.toXML())

            # Construct Reply subject.
            component = ""

            try:
                component = message.getStringField("COMPONENT").getValue()
                    
            except Exception as e:
                libgmsec_python.logWarning("COMPONENT field is not available: " + e.what())
                        

            # Set Status Code to indicate Successful Completion.
            # The GMSEC Interface Specification Document defines 6
            # unique STATUS-CODE values:
            # 1 - Acknowledgement
            # 2 - Working/keep alive
            # 3 - Successful completion
            # 4 - Failed completion
            # 5 - Invalid request
            # 6 - Final message
            # If an asynchronous requestor is awaiting a reply, the
            # ReplyCallback in use will remain active for multiple
            # messages, so long as the messages it receives contain
            # a STATUS-CODE of either 1 or 2.
            status_code = "3"

            # Create the reply subject.
            # See API Interface Specificaiton Document for
            # more information on Reply Message subjects.
            # Generally speaking, they are composed
            # accordingly:
            # [Spec].[Mission].[Satellite ID].
            # [Message Type].[Component Name].[Status Code]
               
            reply_subject = "GMSEC.MISSION.SAT_ID.RESP.REPLY_ASYNC." + status_code

            reply = libgmsec_python.Message(reply_subject, libgmsec_python.Message.REPLY)

            # Add fields to the reply message
            reply.addField("COMPONENT", component)
            reply.addField("ANSWER", "Sure looks like it!")

            # Display XML representation of reply message
            libgmsec_python.logInfo("Prepared Reply:\n" + reply.toXML())

            # Send Reply
            connMgr.reply(message,reply)

            self.replySent = True
                
        except libgmsec_python.Exception as e:
            libgmsec_python.logError("[ExampleCallback::onMessage] " + e.what())


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
    libgmsec_python.logInfo(libgmsec_python.ConnectionManager.getAPIVersion())

    try:
        # Create the Connection
        connMgr = libgmsec_python.ConnectionManager(config)

        # Connect
        connMgr.initialize()
        
        # output connection middleware version
        libgmsec_python.logInfo(connMgr.getLibraryVersion())

        # Subscribe
        cb = ExampleCallback()

        connMgr.subscribe(DEFAULT_REQUEST_SUBJECT, cb)

        # Start the AutoDispatcher to begin async message receipt
        connMgr.startAutoDispatch()

        # Loop while waiting for the asynchronous response until done
        while (cb.replySent == False):
            libgmsec_python.TimeUtil.millisleep(100)
                
        # Clean up
        connMgr.stopAutoDispatch()

        connMgr.cleanup()

    except libgmsec_python.Exception as e:
        libgmsec_python.logError(e.what())

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

