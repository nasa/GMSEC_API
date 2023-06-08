#!/usr/bin/env python3

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file heartbeat_generator.py

 @brief This example program provides a basic demonstration of using
 the HeartbeatGenerator to publish heartbeat messages onto the GMSEC
 Bus.
"""

import libgmsec_python3 as lp
import sys


class heartbeat_generator:
    def __init__(self):
        # Set up connection configuration options using values from the command line.
        self.config = lp.Config(sys.argv)

        # Initialize log level for output
        self.initialize_log_level()

        # Validate messages before being sent to the bus
        self.config.add_value("gmsec-msg-content-validate-send", "true")

        # Define the heartbeat publish rate
        self.publishRate = 2  # in seconds


    def initialize_log_level(self):
        level = lp.Log.from_string( self.config.get_value("loglevel", "info") )
        lp.Log.set_reporting_level(level)


    def run(self):
        try:
            # Create HeartbeatGenerator instance.
            hbgen = lp.HeartbeatGenerator(self.config, self.publishRate, self.get_standard_fields())

            # Output information regarding the API version and connection
            lp.log_info(lp.Connection.get_api_version())
            lp.log_info("HeartbeatGenerator publish rate is: " + str(self.publishRate) + " seconds")

            # Start the heartbeat generator
            hbgen.start()

            lp.log_info("HeartbeatGenerator is running use gmsub or other utility to monitor messages")

            lp.log_info("Press <enter> to stop the HeartbeatGenerator")
            input("")

            # Stop the HeartbeatGenerator
            hbgen.stop()

            lp.log_info("HeartbeatGenerator has been stopped")

        except lp.GmsecError as e:
            # If here, the most likely case is the middleware could not be
            # deduced, or a failure occurred when attempting to connect to
            # the GMSEC Bus.
            lp.log_error("Exception: " + str(e))



    def get_standard_fields(self):
        # Define standard fields for the heartbeat message
        self.field1 = lp.StringField("MISSION-ID", "MY-MISSION", True)
        self.field2 = lp.StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", True)
        self.field3 = lp.StringField("SAT-ID-LOGICAL", "MY-SAT-ID", True)
        self.field4 = lp.StringField("CONSTELLATION-ID", "MY-CNST", True)
        self.field5 = lp.StringField("FACILITY", "MY-FACILITY", True)
        self.field6 = lp.StringField("DOMAIN1", "MY-DOMAIN-1", True)
        self.field7 = lp.StringField("DOMAIN2", "MY-DOMAIN-2", True)
        self.field8 = lp.StringField("COMPONENT", "HB-GEN", True)

        standardFields = lp.FieldList()

        standardFields.push_back( self.field1 )
        standardFields.push_back( self.field2 )
        standardFields.push_back( self.field3 )
        standardFields.push_back( self.field4 )
        standardFields.push_back( self.field5 )
        standardFields.push_back( self.field6 )
        standardFields.push_back( self.field7 )
        standardFields.push_back( self.field8 )

        return standardFields


# Main entry point
ex = heartbeat_generator()
ex.run()
