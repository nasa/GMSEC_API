#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""



"""
 @file heartbeat_service.py

 This file contains an example outlining how to use the Messaging Interface
 Standardization Toolkit (MIST) namespace ConnectionManager's HeartbeatService
 to publish GMSEC-compliant Heartbeat (HB) messages to the middleware bus.

 It is also recommended that you run a subscriber application
 (i.e. GMSEC_API/bin/gmsub) to see the HB Messages which are published to the
 bus.
"""

import libgmsec_python
import sys

HB_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.HEARTBEAT-SERVICE.HB"
HB_PUBLISH_RATE    = 5 # in seconds


def main(argv=None):

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

        # Create all of the GMSEC Message header Fields which will
        # be used by all GMSEC Messages
        headerFields = libgmsec_python.FieldList()

        version = connManager.getSpecification().getVersion()

        missionField = libgmsec_python.StringField("MISSION-ID", "MY-MISSION")
        facilityField = libgmsec_python.StringField("FACILITY", "MY-FACILITY")
        componentField = libgmsec_python.StringField("COMPONENT", "HEARTBEAT-SERVICE")
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

        # Note: Fields are immutable, so plan appropriately if you wish
        # to re-use variable names!

        # Create all of the GMSEC Message header Fields which
        # will be used by all GMSEC HB Messages
        hbStandardFields = libgmsec_python.FieldList()

        pubRateField = libgmsec_python.U16Field("PUB-RATE", HB_PUBLISH_RATE)
        counterField = libgmsec_python.U16Field("COUNTER", 1)
        # Note: COMPONENT-STATUS is an optional field used to
        # denote the operating status of the component, the
        # values are as follows:
        # 0 - Debug
        # 1 - Normal / Green
        # 2 - Warning / Yellow
        # 3 - Orange
        # 4 - Error / Red
        componentStatusField = libgmsec_python.I16Field("COMPONENT-STATUS", 0)

        hbStandardFields.append(pubRateField)
        hbStandardFields.append(counterField)
        hbStandardFields.append(componentStatusField)


        # Create and publish a Heartbeat message using
        # createLogMessage() and publish()

        # Note: This is useful for applications which may need
        # to create proxy heartbeats on behalf of a subsystem,
        # as creating multiple ConnectionManagers can consume
        # more memory than necessary.  In this case, extra
        # logic would need to be added to handle the timing of
        # the publications.
        hbMsg = connManager.createHeartbeatMessage(HB_MESSAGE_SUBJECT, hbStandardFields)
        libgmsec_python.logInfo("Publishing the GMSEC C2CX HB message which was just created using createHeartbeatMessage():\n" + hbMsg.toXML())
        connManager.publish(hbMsg)

        # Kick off the Heartbeat Service -- This will publish
        # heartbeat messages automatically every X seconds,
        # where Xis the value which was provided for PUB-RATE
        # Note: If PUB-RATE was not provided, it will default
        # to 30 seconds per automatic Heartbeat publication
        libgmsec_python.logInfo("Starting the Heartbeat service, a message will be published every " + pubRateField.getStringValue() + " seconds")

        connManager.startHeartbeatService(HB_MESSAGE_SUBJECT, hbStandardFields)

        # Use setHeartbeatServiceField to change the state of the
        # COMPONENT-STATUS Field to indicate that the component has
        # transitioned from a startup/debug state to a running/green
        # state.
        componentStatusField = libgmsec_python.I16Field("COMPONENT-STATUS", 1)
        connManager.setHeartbeatServiceField(componentStatusField)

        libgmsec_python.logInfo("Publishing C2CX Heartbeat Messages indefinitely, press <enter> to exit the program")

        raw_input("")

        # Stop the Heartbeat Service
        connManager.stopHeartbeatService()

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
