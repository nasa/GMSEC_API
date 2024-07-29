#!/usr/bin/env python3

# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file custom_validation.py

 @brief This example program demonstrates how to implement additional
 message validation logic in addition to that which the GMSEC API
 provides.
"""


import libgmsec_python3 as lp
import sys


# Define a custom message validator
#
class CustomMessageValidator(lp.MessageValidator):
    def __init__(self):
        super().__init__()

    def validate_message(self, msg):
        status = lp.Status()

        # Check if we have a Heartbeat message, and if so, then check if
        # the PUB-RATE has an acceptable value.
        try:
            if msg.get_string_value("MESSAGE-SUBTYPE") == "HB":
                if msg.has_field("PUB-RATE"):
                    pubRate = msg.get_integer_value("PUB-RATE")

                    if pubRate < 10 or pubRate > 60:
                        status.set(lp.MSG_ERROR, lp.VALUE_OUT_OF_RANGE, "PUB-RATE field does not have a valid value")
                else:
                    status.set(lp.MSG_ERROR, lp.MISSING_REQUIRED_FIELD, "PUB-RATE field not found")

        except lp.GmsecError as e:
            status = lp.Status(e)

        return status


class custom_validation:
    def __init__(self):
        # Set up connection configuration options using values from the command line.
        self.config = lp.Config(sys.argv)

        # Initialize log level for output
        self.initialize_log_level()

        # Validate messages before being sent to the bus
        self.config.add_value("gmsec-msg-content-validate-send", "true")

    def initialize_log_level(self):
        level = lp.Log.from_string( self.config.get_value("loglevel", "info") )
        lp.Log.set_reporting_level(level)

    def run(self):
        try:
            # Create connection instance.
            conn = lp.Connection( self.config )

            # Set up standard fields within the MessageFactory associated
            # with the connection object.
            self.set_standard_fields(conn.get_message_factory(), "CUSTOM-VALIDATION")

            # Establish connection to the GMSEC Bus.
            conn.connect()

            # Register custom message validator
            cmv = CustomMessageValidator()
            conn.get_message_factory().register_message_validator(cmv)

            # Output information regarding the API version and connection
            lp.log_info(lp.Connection.get_api_version())
            lp.log_info("Middleware version = " + conn.get_library_version())

            # Create a Heartbeat message using the MessageFactory.
            # Set both required and optional fields within the message.
            hbMsg = conn.get_message_factory().create_message("HB")

            # Publish the message without PUB-RATE (expecting custom validation error)
            try:
                lp.log_info("Publishing Message:\n" + hbMsg.to_xml())
                conn.publish(hbMsg)

                lp.log_warning("An error was expected!")
            except lp.GmsecError as e:
                lp.log_info("This error is expected: " + str(e))

            # Publish the message with invalid PUB-RATE value (expecting custom validation error)
            try:
                hbMsg.set_field_value("PUB-RATE", 5)

                lp.log_info("Publishing Message:\n" + hbMsg.to_xml())
                conn.publish(hbMsg)

                lp.log_warning("An error was expected!")
            except lp.GmsecError as e:
                lp.log_info("This error is expected: " + str(e))

            # Publish the message with valid PUB-RATE value (expecting no validation error)
            try:
                hbMsg.set_field_value("PUB-RATE", 10)

                lp.log_info("Publishing Message:\n" + hbMsg.to_xml())
                conn.publish(hbMsg)

                lp.log_info("Message is valid!")
            except lp.GmsecError as e:
                lp.log_error("Unexpected error: " + str(e))

            # Disconnect from the GMSEC Bus, and terminate subscriptions.
            conn.disconnect()

            # Destroy the Connection
            del conn

        except lp.GmsecError as e:
            # If here, the most likely case is the middleware could not be
            # deduced, or a failure occurred when attempting to connect to
            # the GMSEC Bus.
            lp.log_error("Exception: " + str(e))



    # Convenience function for associating standard (i.e. common) fields
    # with the MessageFactory which will be included automatically within
    # all messages created using the factory.
    #
    def set_standard_fields(self, factory, component):
        field1 = lp.StringField("MISSION-ID", "MY-MISSION", True)
        field2 = lp.StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", True)
        field3 = lp.StringField("SAT-ID-LOGICAL", "MY-SAT-ID", True)
        field4 = lp.StringField("CONSTELLATION-ID", "MY-CNST", True)
        field5 = lp.StringField("FACILITY", "MY-FACILITY", True)
        field6 = lp.StringField("DOMAIN1", "MY-DOMAIN-1", True)
        field7 = lp.StringField("DOMAIN2", "MY-DOMAIN-2", True)
        field8 = lp.StringField("COMPONENT", component, True)

        standardFields = lp.FieldList()

        standardFields.push_back( field1 ) 
        standardFields.push_back( field2 ) 
        standardFields.push_back( field3 ) 
        standardFields.push_back( field4 ) 
        standardFields.push_back( field5 ) 
        standardFields.push_back( field6 ) 
        standardFields.push_back( field7 ) 
        standardFields.push_back( field8 ) 

        factory.set_standard_fields(standardFields)


# Main entry point
ex = custom_validation()
ex.run()
