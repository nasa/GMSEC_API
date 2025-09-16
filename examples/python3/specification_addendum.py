#!/usr/bin/env python3

# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file specification_addendum.py
 
 This file contains an example demonstrating the extensibility of the GMSEC
 API Message validation engine through use of additional XML template files
 represending an additional layer of Message definitions to be validated.
"""

import libgmsec_python3 as lp
import sys


class specification_addendum:
    def __init__(self):
        # Set up connection configuration options using values from the command line.
        self.config = lp.Config(sys.argv)

        # Initialize log level for output
        self.initialize_log_level()

        # Validate messages before they are sent to the bus
        self.config.add_value("GMSEC-MSG-CONTENT-VALIDATE-SEND", "true")

        # Tell the API that there is an additional layer of message schema to
        # validate (The 'EXAMPLE' message definitions). By default, this value
        # is set to 0 (only 'C2MS' specification validation).
        # 
        # Note: These levels for validation are determined by the "LEVEL-X"
        # attributes defined in the DIRECTORY.xml file contained in the XML
        # templates directory. In the case of this example, Level-0 implies
        # C2MS, and Level-1 is our example extension.
        # 
        # Note: The GMSEC team envisions using message specifications in a
        # layered hierarchical fashion. For example, the "C2MS" message
        # specification would be 'layer 0', followed by an organization-level
        # message specification at 'layer 1' which builds upon the message
        # specification outlined in C2MS.  This would be followed by a mission
        # level message specification at 'layer 2' (e.g. GMSEC) and so on.
        self.config.add_value("GMSEC-SCHEMA-LEVEL", "1")

        # Tell the API where to find all of the schema files.
        # 
        # Note: This example only demonstrates a simple usage case of this
        # functionality. When using this functionality, if the intent is
        # to use any of the GMSEC message definitions, then ALL of the XML
        # template files must be contained in the same directory.
        # e.g. GMSEC_API/templates/2019.00 (or the location defined in
        # GMSEC-SCHEMA-PATH)
        self.config.add_value("GMSEC-SCHEMA-PATH", "templates")

        # Since this example relies on the 2019.00 version of the templates,
        # we indicate such within the configuration object.
        self.config.add_value("GMSEC-SPECIFICATION-VERSION", "201900")


    def initialize_log_level(self):
        level = lp.Log.from_string( self.config.get_value("loglevel", "info") )
        lp.Log.set_reporting_level(level)


    def run(self):
        try:
            # Create connection instance using custom configuration.
            conn = lp.Connection(self.config)

            # Set up standard fields within the MessageFactory associated
            # with the connection object.
            self.set_standard_fields(conn.get_message_factory(), "SPECIFICATION-ADDENDUM")

            # Establish connection to the GMSEC Bus.
            conn.connect()

            # Output information regarding the API version and connection.
            lp.log_info(lp.Connection.get_api_version())
            lp.log_info("Middleware version = " + conn.get_library_version())

            # Create a LOG Message using a subject defined in the XML template
            # outlining our example addendum message definitions.
            logMsg = conn.get_message_factory().create_message("LOG")

            # Add all of the required fields to the LOG message.
            logMsg.add_field( lp.U16Field("NUM-OF-EVENTS", 2) )
            logMsg.add_field( lp.StringField("EVENT.1.DESC", self.add_time_to_string("AOS occurred at: ")) )
            logMsg.add_field( lp.StringField("EVENT.2.DESC", self.add_time_to_string("Telemetry downlink began at: ")) )

            # Publish the LOG message to the GMSEC Bus.
            conn.publish(logMsg)

            # Display the XML string representation of the LOG message just published.
            lp.log_info("Published custom LOG Message:\n" + logMsg.to_xml())

            # Setup a new LOG message without the NUM-OF-EVENTS required field and
            # attempt to publish it (i.e. trigger a validation failure)
            badLogMsg = conn.get_message_factory().create_message("LOG")

            try:
                conn.publish(badLogMsg)
                lp.log_warning("Surprisingly was able to publish an invalid message:\n" + badLogMsg.to_xml())
            except lp.GmsecError as e:
                lp.log_info("This validation error is expected for the incomplete LOG message:\n" + str(e))

            # Disconnect from the GMSEC Bus.
            conn.disconnect()

            # Destroy the Connection
            del conn

        except lp.GmsecError as e:
            # If here, the most likely case is the middleware could not be
            # deduced, or a failure occurred when attempting to connect to
            # the GMSEC Bus.
            lp.log_error("Exception: " + str(e))


    # Convenience method for associating standard (i.e. common) fields
    # with the MessageFactory which will be included automatically within
    # all messages created using the factory.
    #
    def set_standard_fields(self, factory, component):
        self.field1 = lp.StringField("MISSION-ID", "MY-MISSION", True)
        self.field2 = lp.StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", True)
        self.field3 = lp.StringField("SAT-ID-LOGICAL", "MY-SAT-ID", True)
        self.field4 = lp.StringField("CONSTELLATION-ID", "MY-CNST", True)
        self.field5 = lp.StringField("FACILITY", "MY-FACILITY", True)
        self.field6 = lp.StringField("DOMAIN1", "MY-DOMAIN-1", True)
        self.field7 = lp.StringField("DOMAIN2", "MY-DOMAIN-2", True)
        self.field8 = lp.StringField("COMPONENT", component, True)

        standardFields = lp.FieldList()

        standardFields.push_back( self.field1 )
        standardFields.push_back( self.field2 )
        standardFields.push_back( self.field3 )
        standardFields.push_back( self.field4 )
        standardFields.push_back( self.field5 )
        standardFields.push_back( self.field6 )
        standardFields.push_back( self.field7 )
        standardFields.push_back( self.field8 )

        factory.set_standard_fields(standardFields)


    def add_time_to_string(self, preface):
        return preface + lp.TimeUtil.format_time( lp.TimeUtil.get_current_time() )


# Main entry point
ex = specification_addendum()
ex.run()
