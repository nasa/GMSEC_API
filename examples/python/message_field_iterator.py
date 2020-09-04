#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""

"""
 @file message_field_iterator.cpp
 
 This file contains a simple example demonstrating the use of the
 MessageFieldIterator class to iterate over the Fields of a Message.

 This example program is intended to be run prior to executing any example
 program which publishes a GMSEC message (e.g. The publish example).
"""

import libgmsec_python
import sys

DEFAULT_SUBSCRIPTION_SUBJECT = "GMSEC.>"

class FieldIterationCallback(libgmsec_python.ConnectionManagerCallback):

    def __init__(self):
        libgmsec_python.ConnectionManagerCallback.__init__(self)

    def onMessage(self, connection, message):
        try:
            libgmsec_python.logInfo("Received a message with subject: " + message.getSubject())

            libgmsec_python.logInfo("Field Name (Field Type): Field Value")
            iter = message.getFieldIterator()

            while (iter.hasNext()):
                field = iter.next()

                # Extract the Field Name, Type, and Value (As
                # a string, to print)
                #
                # Note: 'getter' functions are also defined for
                # Integer, Unsigned Integer, and Double values.
                libgmsec_python.logInfo(field.getName() + " (" + typeToString(field.getType()) + "): " + field.getStringValue())

                # Field objects can also be converted to
                # specific Field types prior to retrieval of
                # the value contained in the Field.  This is
                # useful for ensuring that data types do not
                # lose any level of precision, but requires
                # a more intricate implementation.
                #
                # See the getFieldValue() function (commented
                # out at the bottom of this example program)
                # for an example of how field can be done.

        except libgmsec_python.Exception as e:
            libgmsec_python.logError(e.what())
     

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


    initializeLogging(config)

    #Print the GMSEC API version number using the GMSEC Logging
    # interface
    # This is useful for determining which version of the API is
    # configured within the environment
    libgmsec_python.logInfo(libgmsec_python.ConnectionManager.getAPIVersion())

    try:
        # Create a ConnectionManager object
        # This is the linchpin for all communications between the
        # GMSEC API and the middleware server
        connMgr = libgmsec_python.ConnectionManager(config)

        # Open the connection to the middleware
        libgmsec_python.logInfo("Opening the connection to the middleware server")
        connMgr.initialize()

        # Output middleware client library version
        libgmsec_python.logInfo(connMgr.getLibraryVersion())

        # Set up a subscription to listen for Messages with the topic
        # "GMSEC.TEST.PUBLISH" which are published to the middleware
        # bus and register the callback object to the subscription
        # Subscription subject wildcard syntax:
        # * - Matches any one token separated by periods in a subject
        # > - Reading left to right, matches everything up to and ONE
        #     or more tokens in a subject
        # + - Reading left to right, matches everything up to and ZERO 
        #     or more tokens in a subject
        # For more information on wildcards, please see the GMSEC API
        # User's Guide
        libgmsec_python.logInfo("Subscribing to the topic: " + DEFAULT_SUBSCRIPTION_SUBJECT)
        cb = FieldIterationCallback()
        connMgr.subscribe(DEFAULT_SUBSCRIPTION_SUBJECT, cb)

        # Start the AutoDispatcher to begin asynchronously processing
        # messages
        connMgr.startAutoDispatch()

        # Wait for user input to end the program
        libgmsec_python.logInfo("Listening for Messages indefinitely, press <enter> to exit the program")

        raw_input("")
                
        # Clean up
        connMgr.stopAutoDispatch()
        connMgr.cleanup()
        
    except libgmsec_python.Exception as e:
        libgmsec_python.logError(e.what())
        return -1
        
    return 0


def initializeLogging(config):

    # If it was not specified in the command-line arguments, set
    # LOGLEVEL to 'INFO' and LOGFILE to 'stdout' to allow the 
    # program report output on the terminal/command line
    logLevel  = config.getValue("LOGLEVEL")
    logFile   = config.getValue("LOGFILE")

    if (not logLevel):
        config.addValue("LOGLEVEL", "INFO")
        
    if (not logFile):
        config.addValue("LOGFILE", "STDERR")


def typeToString(type):
    if (libgmsec_python.Field.BOOL_TYPE):
        return "BOOL"

    elif (libgmsec_python.Field.BIN_TYPE):
        return "BIN"

    elif (libgmsec_python.Field.CHAR_TYPE):
        return "CHAR"

    elif (libgmsec_python.Field.I8_TYPE):
        return "I8"

    elif (libgmsec_python.Field.I16_TYPE):
        return "I16"

    elif (libgmsec_python.Field.I32_TYPE):
        return "I32"

    elif (libgmsec_python.Field.I64_TYPE):
        return "I64"

    elif (libgmsec_python.Field.F32_TYPE):
        return "F32"

    elif (libgmsec_python.Field.F64_TYPE):
        return "F64"

    elif (libgmsec_python.Field.STRING_TYPE):
        return "STRING"

    elif (libgmsec_python.Field.U8_TYPE):
        return "U8"

    elif (libgmsec_python.Field.U16_TYPE):
        return "U16"

    elif (libgmsec_python.Field.U32_TYPE):
        return "U32"

    elif (libgmsec_python.Field.U64_TYPE):
        return "U64"

    else:
        raise libgmsec_python.Exception(libgmsec_python.FIELD_ERROR, libgmsec_python.UNKNOWN_FIELD_TYPE, "Unsupported Field TYPE): " + type)


#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())
 
