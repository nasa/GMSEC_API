#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""



""" 
 @file log_message.py
 
 This file contains an example outlining how to create and publish a
 GMSEC-compliant LOG message to the middleware bus using the Messaging
 Interface Standardization Toolkit (MIST) namespace ConnectionManager class
 function calls available for such operations.

 It is also recommended that you run a subscriber application
 (i.e. GMSEC_API/bin/gmsub) to see the LOG Messages which are published to the
 bus.
"""

import libgmsec_python
import sys

LOG_MESSAGE_SUBJECT = "GMSEC.MISSION.SPACECRAFT.MSG.LOG"


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
        connManager = libgmsec_python.ConnectionManager(config)

        # Open the connection to the middleware
        libgmsec_python.logInfo("Opening the connection to the middleware server")
        connManager.initialize()

        # Output middleware client library version
        libgmsec_python.logInfo(connManager.getLibraryVersion())

        # Begin the steps necessary to create a GMSEC-compliant LOG
        # message using the ConnectionManager

        # Create all of the GMSEC Message header Fields which will
        # be used by all GMSEC Messages
        
        # Note: Since these Fields contain variable values which are
        # based on the context in which they are used, they cannot be
        # automatically populated using MistMessage.
        definedFields = libgmsec_python.FieldList() 

        missionField = libgmsec_python.StringField("MISSION-ID", "MISSION")
        # Note: SAT-ID-PHYSICAL is an optional header Field, according
        # to the GMSEC ISD.
        satIdField = libgmsec_python.StringField("SAT-ID-PHYSICAL", "SPACECRAFT")
        facilityField = libgmsec_python.StringField("FACILITY", "GMSEC Lab")
        componentField = libgmsec_python.StringField("COMPONENT", "log_message")

        definedFields.append(missionField)
        definedFields.append(satIdField)
        definedFields.append(facilityField)
        definedFields.append(componentField)

        # Use setStandardFields to define a set of header fields for
        # all messages which are created or published on the
        # ConnectionManager using the following functions:
        # createLogMessage, publishLog, createHeartbeatMessage,
        # startHeartbeatService, createResourceMessage, 
        # publishResourceMessage, or startResourceMessageService
        connManager.setStandardFields(definedFields)

        # Use MistMessage to construct a GMSEC LOG message based off
        # of the latest XML templates provided with the GMSEC API.
        # This will automatically populate the Message with all of the
        # Fields which have specific values defined for them in the XML
        # template files.  For more information on which Fields have
        # values defined for them, please review the XML template files
        # located in GMSEC_API/templates.
        
        # Note: The second parameter is an identifier for the type of
        # message to construct.
        logMsg = libgmsec_python.MistMessage(LOG_MESSAGE_SUBJECT, "MSG.LOG", connManager.getSpecification())

        # Add the LOG-specific fields to the LOG message
        
        # Note: Since these Fields contain variable values which are
        # based on the context in which they are used, they cannot be
        # automatically populated using MistMessage.
        eventTime = "YYYY-DDD-HH:MM:SS.sss"
        libgmsec_python.TimeUtil.formatTime(libgmsec_python.TimeUtil.getCurrentTime(), eventTime);

        logMsg.addField(libgmsec_python.I16Field("SEVERITY",1))
        logMsg.setValue("MSG-TEXT", "Creating an example GMSEC LOG Message")
        logMsg.setValue("OCCURRENCE-TYPE", "SYS")
        logMsg.setValue("SUBCLASS", "AST")
        logMsg.setValue("EVENT-TIME", eventTime)

        # Add the standard fields to the LOG message
        connManager.addStandardFields(logMsg)

        libgmsec_python.logInfo("Publishing LOG message:\n" + logMsg.toXML())
        connManager.publish(logMsg)
        
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

