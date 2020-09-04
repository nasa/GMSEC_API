#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""

""" 
 @file message_bins.py
 
 This file contains an example describing how to aggregate multiple messages
 into a singular message sent to the middleware server.  This functionality
 is useful for cutting down on the number of messages sent across the
 middleware bus, if there is an application in the system which is publishing
 an exorbitantly large number of messages.

 It is recommended that you execute the application GMSEC_API/bin/gmsub
 prior to running this program in order to see that the aggregated messages
 still appear on the receiving end as if they were individual messages.

 Please note that if an aggregated message is sent by the GMSEC API, any
 receivers will need to be running at least version 4.2 of the API in order
 to de-aggregate the messages. Older versions can receive the messages
 without crashing, but they will be unusable.
"""
import libgmsec_python
import sys

EXAMPLE_MESSAGE_SUBJECT     = "GMSEC.AGGREGATE.PUBLISH"
EXAMPLE_BIN_EXCLUDE_SUBJECT = "GMSEC.BIN-EXCLUDE.PUBLISH"


def main():

    if(len(sys.argv) <= 1):
        usageMessage = "usage: " + sys.argv[0] + " mw-id=<middleware ID>" 
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

    # Enable Message Binning
    config.addValue("GMSEC-USE-MSG-BINS", "true")

    # Specify the number of messages to be aggregated prior to publishing
    # the aggregate message to the middleware server (This applies to all
    # of the messages which match the subject(s) provided in the
    # GMSEC-MSG-BIN-SUBJECT-N configuration parameters
    # Note: The aggregate message will be sent to the middleware server
    # immediately upon this many messages being published, regardless of
    # the value supplied for GMSEC-MSG-BIN-TIMEOUT.
    config.addValue("GMSEC-MSG-BIN-SIZE", "10")

    # Specify a timeout (in milliseconds) for the aggregate message to be
    # sent to the middleware server
    # Note: The aggregate message will be sent to the middleware server
    # after this period of time if the message bin does not fill up (per
    # the value provided for GMSEC-MSG-BIN-SIZE) prior to this timeout
    config.addValue("GMSEC-MSG-BIN-TIMEOUT", "5000")

    # Specify the subjects to aggreate messages for.
    # Note: Subscription wildcard syntax can be used here, and has been
    # supported since GMSEC API version 4.3.
    config.addValue("GMSEC-MSG-BIN-SUBJECT-1", "GMSEC.*.PUBLISH")

    # Specify any subjects that should be excluded from being aggregated
    # This is useful if a wildcard subscription is provided in one of the
    # GMSEC-MSG-BIN-SUBJECT-N parameters.
    config.addValue("GMSEC-MSG-BIN-EXCLUDE-SUBJECT-1", EXAMPLE_BIN_EXCLUDE_SUBJECT)

    # Since this example program uses an invalid message, we ensure the
    # validation check is disabled.
    config.addValue("gmsec-msg-content-validate-all", "false")

    # If it was not specified in the command-line arguments, set LOGLEVEL
    # to 'INFO' and LOGFILE to 'stdout' to allow the program report output
    # on the terminal/command line
    initializeLogging(config)
    # Print the GMSEC API version number using the GMSEC Logging
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

                
        # Create a message 
        message = libgmsec_python.Message(EXAMPLE_MESSAGE_SUBJECT, libgmsec_python.Message.PUBLISH)

        for i in range(0,5):
            populateMessage(message, i+1)

            # Publish the message to the middleware bus
            connMgr.publish(message)

            # Display the XML string representation of the Message for
            # the sake of review
            libgmsec_python.logInfo("Published message: " + message.toXML())

        # Create another message
        message = libgmsec_python.Message(EXAMPLE_BIN_EXCLUDE_SUBJECT, libgmsec_python.Message.PUBLISH)

        populateMessage(message, 1)

        # Publish the message to the middleware bus
        # Note: When calling publish(), if a message does NOT match
        # one of the subjects to be aggregated, it will be immediately
        # published
        connMgr.publish(message)

        # Display the XML string representation of the Message for
        # the sake of review
        libgmsec_python.logInfo("Published message: " + message.toXML())
                

        # Disconnect from the middleware and clean up the Connection
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
        

def populateMessage(message, count):
           
    # Note: If a field of the same name is added to an existing message,
    # the value passed when calling addField will overwrite the existing
    # value.
    message.addField("CHAR-FIELD", 'c')
    message.addField("BOOL-FIELD-TRUE", True)
    message.addField("BOOL-FIELD-FALSE", False)
    message.addField(libgmsec_python.I8Field("I8-FIELD", count))
    message.addField(libgmsec_python.I16Field("I16-FIELD", count))
    message.addField(libgmsec_python.I32Field("I32-FIELD", count))
    message.addField(libgmsec_python.I64Field("I64-FIELD", count))
    message.addField(libgmsec_python.U8Field("U8-FIELD", count))
    message.addField(libgmsec_python.U16Field("U16-FIELD", count))
    message.addField(libgmsec_python.U32Field("U32-FIELD", count))
    message.addField(libgmsec_python.U64Field("U64-FIELD", count))
    message.addField(libgmsec_python.StringField("STRING-FIELD", "This is a test"))
    message.addField(libgmsec_python.F32Field("F32-FIELD", (1 + 1. / count)))
    message.addField(libgmsec_python.F64Field("F64-FIELD", (1 + 1. / count)))
    message.addField(libgmsec_python.BinaryField("BIN-FIELD", "JLMNOPQ", 7))


#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())

