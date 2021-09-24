#!/usr/bin/env python3


# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


""" 
 @file device_message.cpp
 
 This file contains an example outlining how to create and publish a
 GMSEC-compliant Mnemonic Data Value message to the middleware bus using the
 Messaging Interface Standardization Toolkit (MIST) namespace
 ConnectionManager class function calls available for such operations.

 It is also recommended that you run a subscriber application
 (i.e. GMSEC_API/bin/gmsub) to see the LOG Messages which are published to the
 bus.
"""

import libgmsec_python3
import sys

GMSEC_SPEC_VERSION   = "2016.00"
DEV_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.DEVICE_MESSAGE.DEV"


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
               
        # Note: Since these Fields contain variable values which are
        # based on the context in which they are used, they cannot be
        # automatically populated using MistMessage.
        definedFields = libgmsec_python3.FieldList() 

        missionField = libgmsec_python3.StringField("MISSION-ID", "MISSION")
        # Note: SAT-ID-PHYSICAL is an optional header Field, according
        # to the GMSEC ISD.
        satIdField = libgmsec_python3.StringField("SAT-ID-PHYSICAL", "SPACECRAFT")
        facilityField = libgmsec_python3.StringField("FACILITY", "GMSEC Lab")
        componentField = libgmsec_python3.StringField("COMPONENT", "device_message")

        definedFields.append(missionField)
        definedFields.append(satIdField)
        definedFields.append(facilityField)
        definedFields.append(componentField)

        # Use set_standard_fields to define a set of header fields for
        # all messages which are created or published on the
        # ConnectionManager using the following functions:
        # create_log_message, publish_log, create_heartbeat_message,
        # start_heartbeat_service, create_resource_message, 
        # publishResourceMessage, or start_resource_message_service
        connManager.set_standard_fields(definedFields)

        paramVal = libgmsec_python3.I32Field("DEVICE.1.PARAM.1.VALUE", 79)
        param = libgmsec_python3.DeviceParam("DEV parameter 1", "parameter 1 timestamp", paramVal)

        device1 = libgmsec_python3.Device("device 1", libgmsec_python3.Device.RED)
        device1.set_group("group")
        device1.set_role("role")
        device1.set_model("model")
        device1.set_serial("1138")
        device1.set_version("1.4.5.2.3.4.5")
        devInfo = libgmsec_python3.I16Field("info", 5)
        device1.set_info(devInfo)
        devNum = libgmsec_python3.I16Field("num", 5)
        device1.set_number(devNum)
        device1.add_param(param)

        # Construct an DEV Message and add the Device values to it
        devMessage = libgmsec_python3.DeviceMessage(DEV_MESSAGE_SUBJECT, connManager.get_specification())
        devMessage.add_device(device1)

        connManager.add_standard_fields(devMessage)

        libgmsec_python3.log_info("Publishing DEV message:\n" + devMessage.to_XML())
        connManager.publish(devMessage)
        
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


