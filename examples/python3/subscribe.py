#!/usr/bin/env python3

# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file subscribe.py

 @brief This example program provides a basic demonstration of subscribing
 to receive a message from the GMSEC Bus.
"""

import libgmsec_python3 as lp
import sys


class subscribe:
    def __init__(self):
        # Set up connection configuration options using values from the command line.
        self.config = lp.Config(sys.argv)

        # Initialize log level for output
        self.initialize_log_level()

        # Validate messages as they are received from the bus
        self.config.add_value("gmsec-msg-content-validate-recv", "true")

        # Deduce subscription topic for the example program
        self.subscription_topic = ""
        level = self.config.get_integer_value("gmsec-schema-level", lp.Specification.SchemaLevel_LEVEL_0)

        if level == lp.Specification.SchemaLevel_LEVEL_0:
            self.subscription_topic = "C2MS.>"

        elif level == lp.Specification.SchemaLevel_LEVEL_1:
            self.subscription_topic = "C2MS-PIPE.>"

        else:
            self.subscription_topic = "GMSEC.>"


    def initialize_log_level(self):
        level = lp.Log.from_string( self.config.get_value("loglevel", "info") )
        lp.Log.set_reporting_level(level)


    def run(self):
        try:
            # Create connection instance.
            conn = lp.Connection(self.config)

            # Establish connection to the GMSEC Bus.
            conn.connect()

            # Output information regarding the API version and connection
            lp.log_info(lp.Connection.get_api_version())
            lp.log_info("Middleware version = " + conn.get_library_version())

            # Subscribe to receive message
            conn.subscribe(self.subscription_topic)
            lp.log_info("Subscribed to: " + self.subscription_topic)

            # Define timeout period, or use Gmsec.WAIT_FOREVER to block
            # indefinitely to receive a message.
            timeout = 5000   # 5 seconds

            # Attempt to receive a message
            msg = conn.receive(timeout)

            if msg != None:
                # Received a message!
                lp.log_info("Received Message:\n" + msg.to_xml())

                # Acknowledge processing the response message
                msg.acknowledge()

                # Release received message
                lp.Message.destroy(msg)

            else:
                # Time out!
                lp.log_warning("Time out no message was received")

            # Disconnect from the GMSEC Bus, and terminate subscriptions.
            conn.disconnect()

            # Destroy the Connection
            del conn

        except lp.GmsecError as e:
            # If here, the most likely case is the middleware could not be
            # deduced, or a failure occurred when attempting to connect to
            # the GMSEC Bus.
            lp.log_error("Exception: " + str(e))


# Main entry point
ex = subscribe()
ex.run()
