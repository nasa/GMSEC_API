#!/usr/bin/env python3


# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file reply.py
 
 This file contains an example demonstrating how to issue a reply to a
 request message.

 This example program is intended to be run before starting up the
 'request' or 'request_async' example program.
"""

import libgmsec_python3
import sys

DEFAULT_REQUEST_SUBJECT = "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL"

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

        # Open the connection to the middleware
        libgmsec_python3.log_info("Opening the connection to the middleware server")
        connMgr.initialize()

        # Output middleware client/wrapper version
        libgmsec_python3.log_info(connMgr.get_library_version())

        connMgr.subscribe(DEFAULT_REQUEST_SUBJECT)
        libgmsec_python3.log_info("Subscribed to: " + DEFAULT_REQUEST_SUBJECT);

        # Call receive() to synchronously retrieve a message that has
        # been received by the middleware client libraries
        # Timeout periods:
        # -1 - Wait forever
        #  0 - Return immediately
        # >0 - Time in milliseconds before timing out
        requestMsg = connMgr.receive(-1)

        # Example error handling for calling receive() with a timeout
        if (requestMsg):
                
            # Display the XML representation of the received message
            libgmsec_python3.log_info("Received a message\n" + requestMsg.to_XML())

            # Double-check the Message type to ensure that it is a request
            if (requestMsg.get_kind() == libgmsec_python3.Message.REQUEST):
                            
                # Get the name of the component who issued the request
                component = 0

                # Construct a Reply message
                try:
                    compField = requestMsg.get_string_field("COMPONENT")
                    component = compField.get_value()
                                
                except GmsecError as e:
                     libgmsec_python3.log_error(str(e))
                                

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

                # Set the reply subject.
                # See API Interface Specificaiton Document for
                # more information on Reply Message subjects.
                # Generally speaking, they are composed
                # accordingly:
                # [Spec].[Mission].[Satellite ID].
                # [Message Type].[Component Name].[Status Code]
                               
                reply_subject = "GMSEC.MISSION.SAT_ID.RESP.REPLY." + status_code

                # Create reply message
                replyMsg = libgmsec_python3.Message(reply_subject, libgmsec_python3.Message.REPLY)

                # Add fields to the reply message
                replyMsg.add_field("ANSWER", "Yup, I'm here!")
                replyMsg.add_field("COMPONENT", component)

                # Display XML representation of the reply message
                libgmsec_python3.log_info("Prepared Reply:\n" + replyMsg.to_XML())

                # Send Reply
                connMgr.reply(requestMsg, replyMsg)

            # Destroy request message to release its memory
            connMgr.release(requestMsg)

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
       
