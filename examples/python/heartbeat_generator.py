#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""

""" 
 @file heartbeat_generator.py
 
 This file contains an example describing how to instantiate and
 start/stop a Heartbeat (Message) Generator.
"""

import libgmsec_python
import sys


HB_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.HEARTBEAT-GENERATOR.HB"
HB_PUBLISH_RATE    = 5


def main():

    if(len(sys.argv) <= 1):
        print("Usage: " + sys.argv[0] + " mw-id=<middleware ID>")
        return -1

    # Setup configuration with the supplied command line arguments
    config = libgmsec_python.Config()

    for arg in sys.argv[1:]:
        value = arg.split('=')
        config.addValue(value[0], value[1])

    # Unless otherwise configured, setup configuration that allows us to
    # log messages to stderr.
    initializeLogging(config)

    # Display the version number of the GMSEC API
    libgmsec_python.logInfo(libgmsec_python.ConnectionManager.getAPIVersion())

    # Obtain the specification version that we are running with; if not provided, default to GMSEC_ISD_CURRENT
    specVersion = config.getIntegerValue("gmsec-specification-version", libgmsec_python.GMSEC_ISD_CURRENT)

    # Define standard fields that will be included with the heartbeat messages
    standardFields = libgmsec_python.FieldList()

    component = libgmsec_python.StringField("COMPONENT", "HEARTBEAT-GENERATOR")
    mission   = libgmsec_python.StringField("MISSION-ID", "MY-MISSION")
    satellite = libgmsec_python.StringField("SAT-ID-PHYSICAL", "MY-SAT-ID")
    facility  = libgmsec_python.StringField("FACILITY", "MY-FACILITY")

    standardFields.push_back(component)
    standardFields.push_back(mission)
    standardFields.push_back(satellite)
    standardFields.push_back(facility)

    if specVersion == libgmsec_python.GMSEC_ISD_2014_00:
        msgID = libgmsec_python.StringField("MSG-ID", "MY-MSG-ID")
        standardFields.push_back(msgID)

    elif specVersion >= libgmsec_python.GMSEC_ISD_2018_00:
        domain1 = libgmsec_python.StringField("DOMAIN1", "MY-DOMAIN-1")
        domain2 = libgmsec_python.StringField("DOMAIN2", "MY-DOMAIN-2")
        standardFields.push_back(domain1)
        standardFields.push_back(domain2)

    try:
        # Instantiate the heartbeat generator
        hbgen = libgmsec_python.HeartbeatGenerator(config, HB_MESSAGE_SUBJECT, HB_PUBLISH_RATE, standardFields)

        # Start the heartbeat generator
        hbgen.start()
        libgmsec_python.logInfo("Heartbeat Generator is running; use gmsub or other utility to monitor messages.")

        # Wait for input from user to stop the heartbeat generator
        libgmsec_python.logInfo("Press <enter> to stop the heartbeat generator")
        raw_input("")

        # Stop the heartbeat generator
        hbgen.stop();
        libgmsec_python.logInfo("Heartbeat Generator has been stopped.")

    except libgmsec_python.Exception as e:
        libgmsec_python.logError(e.what())
        return -1

    return 0


def initializeLogging(config):
    logLevel = config.getValue("loglevel")
    logFile  = config.getValue("logfile")

    if (not logLevel):
        config.addValue("loglevel", "info")
    if (not logFile):
        config.addValue("logfile", "stderr")


#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())

