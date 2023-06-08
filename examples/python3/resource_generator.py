#!/usr/bin/env python3

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file resource_generator.py

 @brief This example program provides a basic demonstration of using
 the ResourceGenerator to publish resource messages onto the GMSEC
 Bus.
"""

import libgmsec_python3 as lp
import sys


class resource_generator:
    def __init__(self):
        # Set up connection configuration options using values from the command line.
        self.config = lp.Config(sys.argv)

        # Initialize log level for output
        self.initialize_log_level()

        # Validate messages before they are sent to the bus
        self.config.add_value("gmsec-msg-content-validate-send", "true")


    def initialize_log_level(self):
        level = lp.Log.from_string( self.config.get_value("loglevel", "info") )
        lp.Log.set_reporting_level(level)


    def run(self):
        # Define the resource publish rate and how often resource are collected
        publishRate     = 5  # in seconds
        sampleInterval  = 1  # in seconds
        averageInterval = 10 # in seconds

        try:
            # Acquire standard fields to be included with the heartbeat messages
            standardFields = self.get_standard_fields()

            # Create ResourceGenerator instance.
            rsrcgen = lp.ResourceGenerator(self.config, publishRate, sampleInterval, averageInterval, standardFields)

            # Output information regarding the API version and connection
            lp.log_info(lp.Connection.get_api_version())
            lp.log_info("ResourceGenerator publish rate is: " + str(publishRate) + " seconds")

            # Start the resource generator
            rsrcgen.start()

            lp.log_info("ResourceGenerator is running use gmsub or other utility to monitor messages")

            lp.log_info("Press <enter> to stop the ResourceGenerator")
            input("")

            # Stop the ResourceGenerator
            rsrcgen.stop()

            lp.log_info("ResourceGenerator has been stopped")

        except lp.GmsecError as e:
            # If here, the most likely case is the middleware could not be
            # deduced, or a failure occurred when attempting to connect to
            # the GMSEC Bus.
            lp.log_error("Exception: " + str(e))


    def get_standard_fields(self):
        self.field1 = lp.StringField("MISSION-ID", "MY-MISSION", True)
        self.field2 = lp.StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", True)
        self.field3 = lp.StringField("SAT-ID-LOGICAL", "MY-SAT-ID", True)
        self.field4 = lp.StringField("CONSTELLATION-ID", "MY-CNST", True)
        self.field5 = lp.StringField("FACILITY", "MY-FACILITY", True)
        self.field6 = lp.StringField("DOMAIN1", "MY-DOMAIN-1", True)
        self.field7 = lp.StringField("DOMAIN2", "MY-DOMAIN-2", True)
        self.field8 = lp.StringField("COMPONENT", "RSRC-GEN", True)

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
ex = resource_generator()
ex.run()
