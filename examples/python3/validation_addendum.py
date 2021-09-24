#!/usr/bin/env python3


# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


""" 
 @file validation_addendum.py
 
 This file contains an example demonstrating the extensibility of the GMSEC
 API Message validation engine through use of additional XML template files
 represending an additional layer of Message definitions to be validated.
"""

import libgmsec_python3
import sys

# Note: The name 'EXAMPLE' in this subject represents the name of the message
# specification which is being used.  If using message definitions outlined in
# the GMSEC Interface Specification Document, this would instead be, "GMSEC."
EXAMPLE_MESSAGE_SUBJECT = "EXAMPLE.MISSION.SPACECRAFT.MSG.LOG"


def main():

    if(len(sys.argv) <= 1):
        usageMessage = "usage: " + sys.argv[0] + " mw-id=<middleware ID>"
        print(usageMessage)
        return -1


    # Load the command-line input into a GMSEC Config object
    config = libgmsec_python3.Config()

    for arg in sys.argv[1:]:
        value = arg.split('=')
        config.add_value(value[0], value[1])


    # If it was not specified in the command-line arguments, set LOGLEVEL
    # to 'INFO' and LOGFILE to 'stdout' to allow the program report output
    # on the terminal/command line

    initializeLogging(config);

    # Enable Message validation.  This parameter is "false" by default.
    config.add_value("GMSEC-MSG-CONTENT-VALIDATE", "true")

    # Tell the API that there is an additional layer of message schema to
    # validate (The 'EXAMPLE' message definitions).  This value is set to
    # 0 (Only 'GMSEC' specification validation) by default.
    
    # Note: These levels for validation are determined by the "LEVEL-X"
    # attributes defined in the .DIRECTORY.xml file contained in the XML
    # templates directory.  In thise case, Level-0 means GMSEC and Level-1
    # means EXAMPLE.
    
    # Note: The GMSEC team envisions using message specifications in a
    # layered hierarchical fashion.  For example, the "GMSEC" message
    # specification would be 'layer 0', followed by an organization-level
    # message specification at 'layer 1' which builds upon the message
    # specification outlined in the GMSEC ISD.  This would be followed by
    # a mission-level message specification at 'layer 2' and so on.
    config.add_value("GMSEC-SCHEMA-LEVEL", "1")

    # Tell the API where to find all of the schema files.

    # Note: This example only demonstrates a simple usage case of this
    # functionality.  When using this functionality, if the intent is
    # to use any of the GMSEC message definitions, then ALL of the XML
    # template files must be contained in the same directory.
    # e.g. GMSEC_API/templates/2016.00 (Or the location defined in
    # GMSEC-SCHEMA-PATH)
    config.add_value("GMSEC-SCHEMA-PATH", "templates")

    # Since this example relies on the 2016.00 version of the templates,
    # we indicate such within the configuration object.
    config.add_value("GMSEC-SPECIFICATION-VERSION", "201600");

    libgmsec_python3.log_info(libgmsec_python3.ConnectionManager.get_API_version())

    try:
        connMgr = libgmsec_python3.ConnectionManager(config)

        libgmsec_python3.log_info("Opening the connection to the middleware server")
        connMgr.initialize()

        libgmsec_python3.log_info(connMgr.get_library_version())

        definedFields = libgmsec_python3.FieldList()

        missionField = libgmsec_python3.StringField("MISSION-ID", "MISSION")
        satIdField = libgmsec_python3.StringField("SAT-ID-PHYSICAL", "SPACECRAFT")
        facilityField = libgmsec_python3.StringField("FACILITY", "GMSEC Lab")
        componentField = libgmsec_python3.StringField("COMPONENT", "log_message")

        definedFields.append(missionField)
        definedFields.append(satIdField)
        definedFields.append(facilityField)
        definedFields.append(componentField)

        connMgr.set_standard_fields(definedFields)

        # Create a Message using a subject defined in the XML template
        # outlining our example addendum message definitions
        message = libgmsec_python3.MistMessage(EXAMPLE_MESSAGE_SUBJECT, "MSG.LOG", connMgr.get_specification())

        # Add all of the necessary Fields to our message
        message.add_field(libgmsec_python3.U16Field("NUM-OF-EVENTS", 2))
        message.set_value("EVENT.1", addTimeToString("AOS occurred at: "))
        message.set_value("EVENT.2", addTimeToString("Telemetry downlink began at: "))

        connMgr.add_standard_fields(message)

        # Publish the message to the middleware bus
        connMgr.publish(message)

        # Display the XML string representation of the Message for
        # the sake of review
        libgmsec_python3.log_info("Published message:\n" + message.to_XML())

        # Setup a new message without some of the Required Fields and
        # attempt to publish it (i.e. Trigger a validation failure)
        badMessage = libgmsec_python3.MistMessage(EXAMPLE_MESSAGE_SUBJECT, "MSG.LOG", connMgr.get_specification())

        try:
            connMgr.publish(badMessage)
        except libgmsec_python3.GmsecError as e:
            libgmsec_python3.log_error("This error is expected:\n" + str(e))
                
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


def addTimeToString(preface):

        theTime = libgmsec_python3.TimeUtil.format_time(libgmsec_python3.TimeUtil.get_current_time())
        return preface + theTime


#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())


