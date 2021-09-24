#!/usr/bin/env python3


# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file request_open_response.py
 
 This file contains an example demonstrating how to issue a request message
 and handle a coinciding reply message when using the open-response
 configuration option. This example program is intended to be run after
 starting up the 'reply_open_resp' example program.

 It is also recommended that you run a subscriber application
 (i.e. GMSEC_API/bin/gmsub) with the configuration option 'mw-expose-resp'
 set to true in order to see how any subscriber can receive reply messages
 while using the open-response functionality. Note that by setting the
 configuration option 'GMSEC-REQ-RESP' to 'open-resp' in the requester
 automatically turns on the 'mw-expose-resp' option.
"""

import libgmsec_python3
import sys


OPEN_RESP_REQUEST_SUBJECT = "GMSEC.MISSION.SAT_ID.RESP.REQUEST_OPENRESP";
OPEN_RESP_REPLY_SUBJECT   = "GMSEC.MISSION.SAT_ID.RESP.REPLY_OPENRESP";


def main():

    if(len(sys.argv) <= 1):
        usageMessage = "usage: " + sys.argv[0] + " mw-id=<middleware ID>"
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

    # Ensure that the open-response is enabled
    # Note: Other subscribing applications should set the configuration
    # option 'mw-expose-resp' to 'true' in order to receive exposed replies
    # By setting the configuration option 'GMSEC-REQ-RESP' to 'open-resp'
    # here, it automatically enables the 'mw-expose-resp' option.
    config.add_value("GMSEC-REQ-RESP", "OPEN-RESP")

    # If it was not specified in the command-line arguments, set LOGLEVEL
    # to 'INFO' and LOGFILE to 'stdout' to allow the program report output
    # on the terminal/command line
    initializeLogging(config)

    # Print the GMSEC API version number using the GMSEC Logging
    # interface
    # This is useful for determining which version of the API is
    # configured within the environment
    libgmsec_python3.log_info(libgmsec_python3.ConnectionManager.get_API_version())

    try:
        # Create the ConnectionManager
        connMgr = libgmsec_python3.ConnectionManager(config)

        # Open the connection to the middleware
        connMgr.initialize()

        # Output middleware client library version
        libgmsec_python3.log_info(connMgr.get_library_version())

        # Subscribe to the bus in preparation to receive the
        # open-response message (Because it will not be routed
        # to the reqeust() call)
                
        reply_subject = OPEN_RESP_REPLY_SUBJECT + ".*"
        connMgr.subscribe(reply_subject)

        # Output information
        libgmsec_python3.log_info("Issuing a request using the subject '" + OPEN_RESP_REQUEST_SUBJECT + "'")

        # Create message
        requestMsg = libgmsec_python3.Message(OPEN_RESP_REQUEST_SUBJECT, libgmsec_python3.Message.REQUEST)

        # Add fields to message
        requestMsg.add_field("QUESTION", "Is there anyone out there?")
        requestMsg.add_field("COMPONENT", "request")

        # Display XML representation of request message
        libgmsec_python3.log_info("Sending request message:\n" + requestMsg.to_XML())

        # Send Request Message
        # Timeout periods:
        # -1 - Wait forever
        #  0 - Return immediately
        # >0 - Time in milliseconds before timing out
        replyMsg = connMgr.request(requestMsg, 1000, libgmsec_python3.GMSEC_REQUEST_REPUBLISH_NEVER)

        # Example error handling for calling request() with a timeout
        if (replyMsg):
            # Display the XML string representation of the reply
            libgmsec_python3.log_info("Received replyMsg:\n" + replyMsg.to_XML())

            # Destroy the replyMsg message
            connMgr.release(replyMsg)

        # Disconnect from the middleware and clean up the Connection
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

