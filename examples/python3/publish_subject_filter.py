#!/usr/bin/env python3

# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file publish_subject_filter.py
 
 This file contains an example publishing a few messages with different
 subjects in order to demonstrate the subject filtering functionality of the
 API. 

 This example program is intended to be run after starting the
 'subscribe_subject_filter' example program.
"""

import libgmsec_python3 as lp
import sys


class publish_subject_filter:
    def __init__(self):
        # Set up connection configuration options using values from the command line.
        self.config = lp.Config(sys.argv)

        # Initialize log level for output
        self.initialize_log_level()


    def initialize_log_level(self):
        level = lp.Log.from_string( self.config.get_value("loglevel", "info") )
        lp.Log.set_reporting_level(level)


    def run(self):
        try:
            # Create connection instance.
            conn = lp.Connection(self.config)

            # Set up standard fields within the MessageFactory associated
            # with the connection object.
            self.set_standard_fields(conn.get_message_factory(), "PUBLISH-SUBJECT-FILTER")

            # Establish connection to the GMSEC Bus.
            conn.connect()
		
            # Output information regarding the API version and connection
            lp.log_info(lp.Connection.get_api_version())
            lp.log_info("Middleware version = " + conn.get_library_version())

            # Create a valid Log message using the MessageFactory.
            logMsg = conn.get_message_factory().create_message("LOG")

            # Add required fields
            logMsg.add_field( lp.StringField("SUBCLASS", "INFO") )
            logMsg.add_field( lp.StringField("OCCURRENCE-TYPE", "TEST") )
            logMsg.add_field( lp.I16Field("SEVERITY", 1) )
            logMsg.add_field( lp.StringField("MSG-TEXT", "Hello subscriber!") )

            # And the current (event) time
            logMsg.add_field( lp.StringField("EVENT-TIME", lp.TimeUtil.format_time( lp.TimeUtil.get_current_time() )) )

            # Create a valid Heartbeat message using the MessageFactory.
            hbMsg = conn.get_message_factory().create_message("HB")

            lp.log_info("Publishing two messages -- Log message will be received by the subscriber,"
                        + " the Heartbeat message will be filtered out")

            # Publish the Log message
            conn.publish(logMsg)
            lp.log_info("Published LOG message:\n" + logMsg.to_xml())

            # Delay a bit
            lp.TimeUtil.millisleep(2000)

            # Publish the Heartbeat message
            conn.publish(hbMsg)
            lp.log_info("Published HB message:\n" + hbMsg.to_xml())

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


# Main entry point
ex = publish_subject_filter()
ex.run()
