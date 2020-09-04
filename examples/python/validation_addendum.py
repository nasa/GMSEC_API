#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""

""" 
 @file validation_addendum.py
 
 This file contains an example demonstrating the extensibility of the GMSEC
 API Message validation engine through use of additional XML template files
 represending an additional layer of Message definitions to be validated.
"""

import libgmsec_python
import sys

# Note: The name 'EXAMPLE' in this subject represents the name of the message
# specification which is being used.  If using message definitions outlined in
# the GMSEC Interface Specification Document, this would instead be, "GMSEC."
EXAMPLE_MESSAGE_SUBJECT = "EXAMPLE.MISSION.SPACECRAFT.MSG.LOG"


def main():

    if(len(sys.argv) <= 1):
        usageMessage = "usage: " + sys.argv[0] + " mw-id=<middleware ID>"
        print usageMessage
        return -1


    # Load the command-line input into a GMSEC Config object
    config = libgmsec_python.Config()

    for arg in sys.argv[1:]:
        value = arg.split('=')
        config.addValue(value[0], value[1])


    # If it was not specified in the command-line arguments, set LOGLEVEL
    # to 'INFO' and LOGFILE to 'stdout' to allow the program report output
    # on the terminal/command line

    initializeLogging(config);

    # Enable Message validation.  This parameter is "false" by default.
    config.addValue("GMSEC-MSG-CONTENT-VALIDATE", "true")

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
    config.addValue("GMSEC-SCHEMA-LEVEL", "1")

    # Tell the API where to find all of the schema files.

    # Note: This example only demonstrates a simple usage case of this
    # functionality.  When using this functionality, if the intent is
    # to use any of the GMSEC message definitions, then ALL of the XML
    # template files must be contained in the same directory.
    # e.g. GMSEC_API/templates/2016.00 (Or the location defined in
    # GMSEC-SCHEMA-PATH)
    config.addValue("GMSEC-SCHEMA-PATH", "templates")

    # Since this example relies on the 2016.00 version of the templates,
    # we indicate such within the configuration object.
    config.addValue("GMSEC-SPECIFICATION-VERSION", "201600");

    libgmsec_python.logInfo(libgmsec_python.ConnectionManager.getAPIVersion())

    try:
        connMgr = libgmsec_python.ConnectionManager(config)

        libgmsec_python.logInfo("Opening the connection to the middleware server")
        connMgr.initialize()

        libgmsec_python.logInfo(connMgr.getLibraryVersion())

        definedFields = libgmsec_python.FieldList()

        missionField = libgmsec_python.StringField("MISSION-ID", "MISSION")
        satIdField = libgmsec_python.StringField("SAT-ID-PHYSICAL", "SPACECRAFT")
        facilityField = libgmsec_python.StringField("FACILITY", "GMSEC Lab")
        componentField = libgmsec_python.StringField("COMPONENT", "log_message")

        definedFields.append(missionField)
        definedFields.append(satIdField)
        definedFields.append(facilityField)
        definedFields.append(componentField)

        connMgr.setStandardFields(definedFields)

        # Create a Message using a subject defined in the XML template
        # outlining our example addendum message definitions
        message = libgmsec_python.MistMessage(EXAMPLE_MESSAGE_SUBJECT, "MSG.LOG", connMgr.getSpecification())

        # Add all of the necessary Fields to our message
        message.addField(libgmsec_python.U16Field("NUM-OF-EVENTS", 2))
        message.setValue("EVENT.1", addTimeToString("AOS occurred at: "))
        message.setValue("EVENT.2", addTimeToString("Telemetry downlink began at: "))

        connMgr.addStandardFields(message)

        # Publish the message to the middleware bus
        connMgr.publish(message)

        # Display the XML string representation of the Message for
        # the sake of review
        libgmsec_python.logInfo("Published message:\n" + message.toXML())

        # Setup a new message without some of the Required Fields and
        # attempt to publish it (i.e. Trigger a validation failure)
        badMessage = libgmsec_python.MistMessage(EXAMPLE_MESSAGE_SUBJECT, "MSG.LOG", connMgr.getSpecification())

        try:
            connMgr.publish(badMessage)
        except libgmsec_python.Exception as e:
            libgmsec_python.logError("This is error is expected:\n" + e.what())
                
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


def addTimeToString(preface):

        theTime = "YYYY-DDD-HH:MM:SS.sss"   # Prime the time string with the proper length
        libgmsec_python.TimeUtil.formatTime(libgmsec_python.TimeUtil.getCurrentTime(), theTime)
        return preface + theTime


#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())


