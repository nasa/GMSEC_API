#!/usr/bin/env python3


# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


""" 
 @file validation_custom.py
 
 This file contains an example demonstrating how to implement additional
 Message validation logic in addition to that which the GMSEC API provides.
"""

import libgmsec_python3 as lp
import sys 


HB_MSG_SUBJECT     = "GMSEC.MISSION.SATELLITE.MSG.HB.VALIDATION-CUSTOM"
ALT_HB_MSG_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.VALIDATION-CUSTOM.HB"


class CustomMessageValidator(lp.MessageValidator):

    def __init__(self):
        super().__init__()

    def validate_message(self, msg):
        status = lp.Status()

        # Get message type and subtype
        msgType = ""
        subtype = ""

        if msg.has_field("MESSAGE-TYPE"):
            msgType = msg.get_string_value("MESSAGE-TYPE")
        if msg.has_field("C2CX-SUBTYPE"):
            subtype = msg.get_string_value("C2CX-SUBTYPE")
        elif msg.has_field("MESSAGE-SUBTYPE"):
            subtype = msg.get_string_value("MESSAGE-SUBTYPE")

        # Ensure we have a Heartbeat message and it contains the PUB-RATE field
        if msgType == "MSG" and subtype == "HB" and msg.has_field("PUB-RATE"):
            pubRate = msg.get_integer_value("PUB-RATE")
            if pubRate < 10 or pubRate > 60:
                status.set(lp.MSG_ERROR, lp.VALUE_OUT_OF_RANGE, "PUB-RATE field does not have a valid value")

        else:
            status.set(lp.MSG_ERROR, lp.INVALID_MSG, "Non-Heartbeat message received")

        return status


def main():

    if(len(sys.argv) <= 1):
        usageMessage = "usage: " + sys.argv[0] + " mw-id=<middleware ID>"
        print(usageMessage)
        return -1


    # Load the command-line input into a GMSEC Config object
    config = lp.Config(sys.argv)

    # If it was not specified in the command-line arguments, set LOGLEVEL
    # to 'INFO' and LOGFILE to 'stdout' to allow the program report output
    # on the terminal/command line
    initializeLogging(config);

    # Enable Message validation.  This parameter is "false" by default.
    config.add_value("gmsec-msg-content-validate-send", "true")
    config.add_value("gmsec-validation-level", "3")

    lp.log_info("API version: " + lp.ConnectionManager.get_API_version())

    try:
        connMgr = lp.ConnectionManager(config)

        connMgr.initialize()

        lp.log_info("Middleware version: " + connMgr.get_library_version())

        # Register custom message validator
        validator = CustomMessageValidator()
        connMgr.register_message_validator(validator)

        # Set up standard/common fields used with all messages
        specVersion = connMgr.get_specification().get_version()
        setupStandardFields(specVersion)

        # Create Heartbeat Message
        # Note: Message subject and schema ID vary depending on the specification in use
        if specVersion > lp.GMSEC_ISD_2018_00:
            subject  = HB_MSG_SUBJECT
            schemaID = "MSG.HB"
        else:
            subject  = ALT_HB_MSG_SUBJECT
            schemaID = "MSG.C2CX.HB"

        msg = lp.MistMessage(subject, schemaID, connMgr.get_specification())

        # Add PUB-RATE field with illegal value
        msg.set_value("PUB-RATE", "5")

        # For very old specifications, we need to add a MSG-ID field
        if specVersion <= lp.GMSEC_ISD_2014_00:
            msg.add_field("MSG-ID", "12345")

        # Attempt to publish malformed message
        try:
            lp.log_info("Attempting to publish malformed message...")
            connMgr.publish(msg)

            lp.log_warning("Was expecting an error")

        except lp.GmsecError as e:
            # We expect to encounter error with the PUB-RATE field
            lp.log_info("This is an expected error:\n" + str(e))

        # Fix PUB-RATE field with legal value
        msg.set_value("PUB-RATE", "15")

        # Publish a good message
        try:
            lp.log_info("Attempting to publish good message...")
            connMgr.publish(msg)

            lp.log_info("Message published!")

        except lp.GmsecError as e:
            lp.log_warning("Unexpected error:\n" + str(e))

        # Disconnect from the middleware and clean up the Connection
        connMgr.cleanup()

        clearStandardFields()
        
    except lp.GmsecError as e:
        lp.log_error(str(e))
        return -1

    return 0


def initializeLogging(config):

    logLevel = config.get_value("LOGLEVEL")
    logFile  = config.get_value("LOGFILE")

    if (not logLevel):
        config.add_value("LOGLEVEL", "INFO")

    if (not logFile):
        config.add_value("LOGFILE", "STDERR")


def setupStandardFields(specVersion):

    definedFields = lp.FieldList()

    mission       = lp.StringField("MISSION-ID", "MISSION")
    constellation = lp.StringField("CONSTELLATION-ID", "CONSTELLATION")
    satIdPhys     = lp.StringField("SAT-ID-PHYSICAL", "SATELLITE")
    satIdLog      = lp.StringField("SAT-ID-LOGICAL", "SATELLITE")
    facility      = lp.StringField("FACILITY", "GMSEC-LAB")
    component     = lp.StringField("COMPONENT", "VALIDATION-CUSTOM")
    domain1       = lp.StringField("DOMAIN1", "DOMAIN1")
    domain2       = lp.StringField("DOMAIN2", "DOMAIN2")

    definedFields.append(mission)
    definedFields.append(constellation)
    definedFields.append(satIdPhys)
    definedFields.append(satIdLog)
    definedFields.append(facility)
    definedFields.append(component)

    if specVersion >= lp.GMSEC_ISD_2018_00:
        definedFields.append(domain1)
        definedFields.append(domain2)

    lp.MistMessage.set_standard_fields(definedFields)


def clearStandardFields():
    lp.MistMessage.clear_standard_fields()


#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())

