#!/usr/bin/env python3


# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


""" 
 @file publish.py
 
 This file contains an example describing how to create and publish a
 message to the middleware bus using the lowest level function calls
 available for such operations.

 This example program is intended to be run after starting the 'subscribe'
 or 'subscribe_async' example program.
"""

import libgmsec_python3
import sys

EXAMPLE_MESSAGE_SUBJECT = "GMSEC.TEST.PUBLISH"


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

    # Print the GMSEC API version number using the GMSEC Logging
    # interface
    # This is useful for determining which version of the API is
    # configured within the environment
    libgmsec_python3.log_info(libgmsec_python3.ConnectionManager.get_API_version())

    try:
       
        # Create a ConnectionManager object
        # This is the linchpin for all communications between the
        # GMSEC API and the middleware server
        connMgr = libgmsec_python3.ConnectionManager(config)

        # Open the connection to the middleware
        libgmsec_python3.log_info("Opening the connection to the middleware server")
        connMgr.initialize()

        # Output middleware client library version
        libgmsec_python3.log_info(connMgr.get_library_version())

        # Create a message
        # Disclaimer: This message is not based off of a Message
        # Definition outlined by the GMSEC Interface
        # Specification Document (ISD).  It is meant to be an example
        # to demonstrate the various capabilities of the GMSEC Message
        # and Field classes. The GMSEC Team recommends that you map
        # your data into the Messages defined in the GMSEC ISD, as
        # doing so will make your software "GMSEC Compliant" resulting
        # in plug-and-play type functionality with other GMSEC
        # compliant software.
        message = libgmsec_python3.Message(EXAMPLE_MESSAGE_SUBJECT, libgmsec_python3.Message.PUBLISH)
        populateMessage(message)

        # Publish the message to the middleware bus
        connMgr.publish(message)

        # Display the XML string representation of the Message for
        # the sake of review
        libgmsec_python3.log_info("Published message:\n" + message.to_XML())

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
        

def populateMessage(message):

    # Add Fields of varying data types to the Message
    value = 1

    message.add_field(libgmsec_python3.CharField("CHAR-FIELD", 'c'))
    message.add_field(libgmsec_python3.BooleanField("BOOL-FIELD-TRUE", True))
    message.add_field(libgmsec_python3.BooleanField("BOOL-FIELD-FALSE", False))
    message.add_field(libgmsec_python3.I8Field("I8-FIELD", value))
    message.add_field(libgmsec_python3.I16Field("I16-FIELD", value))
    message.add_field(libgmsec_python3.I32Field("I32-FIELD", value))
    message.add_field(libgmsec_python3.I64Field("I64-FIELD", value))
    message.add_field(libgmsec_python3.U8Field("U8-FIELD", value))
    message.add_field(libgmsec_python3.U16Field("U16-FIELD", value))
    message.add_field(libgmsec_python3.U32Field("U32-FIELD", value))
    message.add_field(libgmsec_python3.U64Field("U64-FIELD", value))
    message.add_field(libgmsec_python3.StringField("STRING-FIELD", "This is a test"))
    message.add_field(libgmsec_python3.F32Field("F32-FIELD", (1 + 1. / value)))
    message.add_field(libgmsec_python3.F64Field("F64-FIELD", (1 + 1. / value)))
    message.add_field(libgmsec_python3.BinaryField("BIN-FIELD", "JLMNOPQ", 7))


#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())

