#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""



"""
 @file resource_service.py

 This file contains an example outlining how to use the Messaging Interface
 Standardization Toolkit (MIST) namespace ConnectionManager's ResourceMessageService
 to publish GMSEC-compliant Resource (RSRC) messages to the middleware bus.

 It is also recommended that you run a subscriber application
 (i.e. GMSEC_API/bin/gmsub) to see the HB Messages which are published to the
 bus.
"""

import libgmsec_python
import sys

RSRC_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.RESOURCE-SERVICE.RSRC"
RSRC_PUBLISH_RATE    = 5 # in seconds


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

        # Create all of the GMSEC Message header Fields which will
        # be used by all GMSEC Messages
        headerFields = libgmsec_python.FieldList()

        version = connManager.getSpecification().getVersion()

        missionField = libgmsec_python.StringField("MISSION-ID", "MY-MISSION")
        facilityField = libgmsec_python.StringField("FACILITY", "MY-FACILITY")
        componentField = libgmsec_python.StringField("COMPONENT", "RESOURCE-SERVICE")
        domain1Field = libgmsec_python.StringField("DOMAIN1", "MY-DOMAIN-1")
        domain2Field =libgmsec_python.StringField("DOMAIN2", "MY-DOMAIN-2")
        msgID = libgmsec_python.StringField("MSG-ID", "MY-MSG-ID")

        headerFields.append(missionField)
        headerFields.append(facilityField)
        headerFields.append(componentField)

        if (version == 201400):
            headerFields.append(msgID)

        elif (version >= 201800):
            headerFields.append(domain1Field)
            headerFields.append(domain2Field)


        # Use setStandardFields to define a set of header fields for
        # all messages which are created or published on the
        # ConnectionManager using the following functions:
        # createLogMessage, publishLog, createHeartbeatMessage,
        # startHeartbeatService, createResourceMessage,
        # publishResourceMessage, or startResourceMessageService
        connManager.setStandardFields(headerFields)

        # Create and publish a Resource message using
        # createResourceMessage() and publish()

        # Note: This is useful for applications which may need to add
        # additional Fields to the Resource Messages which are not
        # currently added by the GMSEC API
        rsrcMsg = connManager.createResourceMessage(RSRC_MESSAGE_SUBJECT)
        libgmsec_python.logInfo("Publishing the GMSEC C2CX RSRC message which was created using createResourceMessage():\n" + rsrcMsg.toXML())
        connManager.publish(rsrcMsg)

        # Kick off the Resource Service -- This will publish resource
        # messages automatically every X seconds, where X is the second
        # parameter provided to the startResourceMessageService() function.
        # If an interval is not provided, the service will default to
        # publishing a message every 60 seconds.
        libgmsec_python.logInfo("Starting the Resource Message service, a message will be published every " + str(RSRC_PUBLISH_RATE) + " seconds")

        connManager.startResourceMessageService(RSRC_MESSAGE_SUBJECT, RSRC_PUBLISH_RATE)

        # Wait for user input to end the program
        libgmsec_python.logInfo("Publishing C2CX Resource Messages indefinitely, press <enter> to exit the program")
        raw_input("")

        # Stop the Heartbeat Service
        connManager.stopResourceMessageService()

        # Cleanup
        connManager.cleanup()

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
