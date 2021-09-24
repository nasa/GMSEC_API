#!/usr/bin/env python3


# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


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

import libgmsec_python3
import sys

DEFAULT_REQUEST_SUBJECT = "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL"
# Globals
replySent = False


class ExampleCallback(libgmsec_python3.ConnectionManagerCallback):

    def __init__(self):
        libgmsec_python3.ConnectionManagerCallback.__init__(self)
        self.replySent = False
    
    def on_message(self, connMgr, message):
        
        try:
            # Display the XML representation of the received message
            libgmsec_python3.log_info("[ExampleCallback:on_message] Received:\n" + message.to_XML())

            # Construct Reply subject.
            component = ""

            try:
                component = message.get_string_field("COMPONENT").get_value()
                    
            except GmsecError as e:
                libgmsec_python3.log_warning("COMPONENT field is not available: " + str(e))
                        

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

            reply = libgmsec_python3.Message(reply_subject, libgmsec_python3.Message.REPLY)

            # Add fields to the reply message
            reply.add_field("COMPONENT", component)
            reply.add_field("ANSWER", "Sure looks like it!")

            # Display XML representation of reply message
            libgmsec_python3.log_info("Prepared Reply:\n" + reply.to_XML())

            # Send Reply
            connMgr.reply(message,reply)

            self.replySent = True
                
        except libgmsec_python3.GmsecError as e:
            libgmsec_python3.log_error("[ExampleCallback::on_message] " + str(e))


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
    libgmsec_python3.log_info(libgmsec_python3.ConnectionManager.get_API_version())

    try:
        # Create the Connection
        connMgr = libgmsec_python3.ConnectionManager(config)

        # Connect
        connMgr.initialize()
        
        # output connection middleware version
        libgmsec_python3.log_info(connMgr.get_library_version())

        # Subscribe
        cb = ExampleCallback()

        connMgr.subscribe(DEFAULT_REQUEST_SUBJECT, cb)
        libgmsec_python3.log_info("Subscribed to: " + DEFAULT_REQUEST_SUBJECT)

        # Start the AutoDispatcher to begin async message receipt
        connMgr.start_auto_dispatch()

        # Loop while waiting for the asynchronous response until done
        while (cb.replySent == False):
            libgmsec_python3.TimeUtil.millisleep(100)
                
        # Clean up
        connMgr.stop_auto_dispatch()

        connMgr.cleanup()

    except libgmsec_python3.GmsecError as e:
        libgmsec_python3.log_error(str(e))

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

