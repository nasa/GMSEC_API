#!/usr/bin/env python3


"""
 Copyright 2007-2018 United States Government as represented by the
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

import libgmsec_python3
import sys

RSRC_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.RESOURCE_SERVICE.RSRC"

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
    config = libgmsec_python3.Config()

    for arg in sys.argv[1:]:
        value = arg.split('=')
        config.add_value(value[0], value[1])

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

        versionField = libgmsec_python3.F32Field("HEADER-VERSION", 2010.0)
        missionField = libgmsec_python3.StringField("MISSION-ID", "GMSEC")
        facilityField = libgmsec_python3.StringField("FACILITY", "GMSEC Lab")
        componentField = libgmsec_python3.StringField("COMPONENT", "heartbeat_service")

        headerFields.append(versionField)
        headerFields.append(missionField)
        headerFields.append(facilityField)
        headerFields.append(componentField)

        # Use set_standard_fields to define a set of header fields for
        # all messages which are created or published on the
        # ConnectionManager using the following functions:
        # create_log_message, publish_log, create_heartbeat_message,
        # start_heartbeat_service, create_resource_message, 
        # publishResourceMessage, or start_resource_message_service
        connManager.set_standard_fields(headerFields)

        # Create and publish a Resource message using
        # create_resource_message() and publish()
                
        # Note: This is useful for applications which may need to add
        # additional Fields to the Resource Messages which are not
        # currently added by the GMSEC API
        rsrcMsg = connManager.create_resource_message(RSRC_MESSAGE_SUBJECT)
        libgmsec_python3.log_info("Publishing the GMSEC C2CX RSRC message which was created using create_resource_message():\n" + rsrcMsg.to_XML())
        connManager.publish(rsrcMsg)

        # Kick off the Resource Service -- This will publish resource
        # messages automatically every X seconds, where X is the second
        # parameter provided to the start_resource_message_service() function.
        # If an interval is not provided, the service will default to
        # publishing a message every 60 seconds.
        interval_s = 30
        libgmsec_python3.log_info("Starting the Resource Message service, a message will be published every " + str(interval_s) + " seconds")

        connManager.start_resource_message_service(RSRC_MESSAGE_SUBJECT, interval_s)

        # Wait for user input to end the program
        libgmsec_python3.log_info("Publishing C2CX Resource Messages indefinitely, press <enter> to exit the program")
        input("")

        # Stop the Heartbeat Service
        connManager.stop_resource_message_service()

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
