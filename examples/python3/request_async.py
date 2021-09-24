#!/usr/bin/env python3


# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


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

import libgmsec_python3
import sys

DEFAULT_REQUEST_SUBJECT = "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL"

# Globals
receivedReply = False


class ExampleReplyCallback(libgmsec_python3.ConnectionManagerReplyCallback):

    def __init__(self):
        libgmsec_python3.ConnectionManagerReplyCallback.__init__(self)
        self.receivedReply = False
                                                                  
    def on_reply(self, connection, request, reply):
        # Display XML representation of reply message
        libgmsec_python3.log_info("[ExampleReplyCallback::on_reply]\n" + reply.to_XML())
        self.receivedReply = True
        
    def on_event(self, connection, status, event):
        if (status.is_error()):
            libgmsec_python3.log_error("[ExampleReplyCallback::onError] " + status.get() + ", event=" + event)


def main():

    if(len(sys.argv) <= 1):
        usageMessage = "usage: " +  sys.argv[0] + " mw-id=<middleware ID>"
        print(usageMessage)
        return -1


    # Load the command-line input into a GMSEC Config object
    # A Config object is basically a key-value pair map which is used to
    # pass configuration options into objects such as Connections,
    # ConnectionManagers, Subscribe and Publish function calls, Messages,
    # etc.
    config = libgmsec_python3.Config(sys.argv)

    # Since this example program uses an invalid message, we ensure the
    # validation check is disabled.
    config.add_value("gmsec-msg-content-validate-all", "false")

    # If it was not specified in the command-line arguments, set LOGLEVEL
    # to 'INFO' and LOGFILE to 'stdout' to allow the program report output
    # on the terminal/command line
    initializeLogging(config)

    # Output GMSEC API version
    libgmsec_python3.log_info(libgmsec_python3.Connection.get_API_version())

    try:
        # Create the Connection
        connMgr = libgmsec_python3.ConnectionManager(config)

        # Connect
        connMgr.initialize()

        # output connection middleware version
        libgmsec_python3.log_info(connMgr.get_library_version())

        # Create request message
        requestMsg = libgmsec_python3.Message(DEFAULT_REQUEST_SUBJECT, libgmsec_python3.Message.REQUEST)

        # Add fields to request message
        requestMsg.add_field("QUESTION", "Does the request/reply functionality still work?")
        requestMsg.add_field("COMPONENT", "request_async")

        # Display XML representation of request message
        libgmsec_python3.log_info("Requesting:\n" + requestMsg.to_XML())

        cb = ExampleReplyCallback()
        connMgr.request(requestMsg, libgmsec_python3.GMSEC_WAIT_FOREVER, cb)

        libgmsec_python3.log_info("Waiting for response...")

        # Loop while waiting for the asynchronous response until done
        while (cb.receivedReply == 0):
            libgmsec_python3.TimeUtil.millisleep(100)
                
        if (cb.receivedReply):
            libgmsec_python3.log_info("Response Received!")
        else:
            libgmsec_python3.log_warning("No response received")

        connMgr.cleanup()
            
    except libgmsec_python3.GmsecError as e:
        libgmsec_python3.log_error(str(e))
        return -1
        
    return 0


def initializeLogging(config):

    logLevel  = config.get_value("LOGLEVEL")
    logFile   = config.get_value("LOGFILE")

    if (not logLevel):
        config.add_value("LOGLEVEL", "INFO")
        
    if (not logFile):
        config.add_value("LOGFILE", "STDERR")


#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())
        
