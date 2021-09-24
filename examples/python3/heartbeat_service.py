#!/usr/bin/env python3


# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file heartbeat_service.py

 This file contains an example outlining how to use the Messaging Interface
 Standardization Toolkit (MIST) namespace ConnectionManager's HeartbeatService
 to publish GMSEC-compliant Heartbeat (HB) messages to the middleware bus.

 It is also recommended that you run a subscriber application
 (i.e. GMSEC_API/bin/gmsub) to see the HB Messages which are published to the
 bus.
"""

import libgmsec_python3
import sys

HB_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.HEARTBEAT-SERVICE.HB"
HB_PUBLISH_RATE    = 5 # in seconds


def main(argv=None):

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
        connManager = libgmsec_python3.ConnectionManager(config)

        # Open the connection to the middleware
        libgmsec_python3.log_info("Opening the connection to the middleware server")
        connManager.initialize()

        # Output middleware client library version
        libgmsec_python3.log_info(connManager.get_library_version())

        # Create all of the GMSEC Message header Fields which will
        # be used by all GMSEC Messages
        headerFields = libgmsec_python3.FieldList()

        version = connManager.get_specification().get_version()

        missionField = libgmsec_python3.StringField("MISSION-ID", "MY-MISSION")
        facilityField = libgmsec_python3.StringField("FACILITY", "MY-FACILITY")
        componentField = libgmsec_python3.StringField("COMPONENT", "HEARTBEAT-SERVICE")
        domain1Field = libgmsec_python3.StringField("DOMAIN1", "MY-DOMAIN-1")
        domain2Field =libgmsec_python3.StringField("DOMAIN2", "MY-DOMAIN-2")
        msgID = libgmsec_python3.StringField("MSG-ID", "MY-MSG-ID")

        headerFields.append(missionField)
        headerFields.append(facilityField)
        headerFields.append(componentField)

        if (version == 201400):
            headerFields.append(msgID)

        elif (version >= 201800):
            headerFields.append(domain1Field)
            headerFields.append(domain2Field)


        # Use set_standard_fields to define a set of header fields for
        # all messages which are created or published on the
        # ConnectionManager using the following functions:
        # create_log_message, publish_log, create_heartbeat_message,
        # start_heartbeat_service, create_resource_message,
        # publishResourceMessage, or start_resource_message_service
        connManager.set_standard_fields(headerFields)

        # Note: Fields are immutable, so plan appropriately if you wish
        # to re-use variable names!

        # Create all of the GMSEC Message header Fields which
        # will be used by all GMSEC HB Messages
        hbStandardFields = libgmsec_python3.FieldList()

        pubRateField = libgmsec_python3.U16Field("PUB-RATE", HB_PUBLISH_RATE)
        counterField = libgmsec_python3.U16Field("COUNTER", 1)
        # Note: COMPONENT-STATUS is an optional field used to
        # denote the operating status of the component, the
        # values are as follows:
        # 0 - Debug
        # 1 - Normal / Green
        # 2 - Warning / Yellow
        # 3 - Orange
        # 4 - Error / Red
        componentStatusField = libgmsec_python3.I16Field("COMPONENT-STATUS", 0)

        hbStandardFields.append(pubRateField)
        hbStandardFields.append(counterField)
        hbStandardFields.append(componentStatusField)


        # Create and publish a Heartbeat message using
        # create_log_message() and publish()

        # Note: This is useful for applications which may need
        # to create proxy heartbeats on behalf of a subsystem,
        # as creating multiple ConnectionManagers can consume
        # more memory than necessary.  In this case, extra
        # logic would need to be added to handle the timing of
        # the publications.
        hbMsg = connManager.create_heartbeat_message(HB_MESSAGE_SUBJECT, hbStandardFields)
        libgmsec_python3.log_info("Publishing the GMSEC C2CX HB message which was just created using create_heartbeat_message():\n" + hbMsg.to_XML())
        connManager.publish(hbMsg)

        # Kick off the Heartbeat Service -- This will publish
        # heartbeat messages automatically every X seconds,
        # where Xis the value which was provided for PUB-RATE
        # Note: If PUB-RATE was not provided, it will default
        # to 30 seconds per automatic Heartbeat publication
        libgmsec_python3.log_info("Starting the Heartbeat service, a message will be published every " + pubRateField.get_string_value() + " seconds")

        connManager.start_heartbeat_service(HB_MESSAGE_SUBJECT, hbStandardFields)

        # Use set_heartbeat_service_field to change the state of the
        # COMPONENT-STATUS Field to indicate that the component has
        # transitioned from a startup/debug state to a running/green
        # state.
        componentStatusField = libgmsec_python3.I16Field("COMPONENT-STATUS", 1)
        connManager.set_heartbeat_service_field(componentStatusField)

        libgmsec_python3.log_info("Publishing C2CX Heartbeat Messages indefinitely, press <enter> to exit the program")

        input("")

        # Stop the Heartbeat Service
        connManager.stop_heartbeat_service()

        # Cleanup
        connManager.cleanup()

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
