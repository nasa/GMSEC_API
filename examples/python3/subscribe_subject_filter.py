#!/usr/bin/env python3

# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file subscribe_subject_filter.py
 
 This file contains a simple example outlining how to subscribe
 to and retrieve messages from the middleware bus while using subject
 exclusion filtering to reduce the scope of a wildcarded subject subscription.

 This example program is intended to be run before starting the
 'publish_subject_filter' example program.
"""

import libgmsec_python3 as lp
import sys


class subscribe_subject_filter:
    def __init__(self):
        # Set up connection configuration options using values from the command line.
        self.config = lp.Config(sys.argv)

        # Initialize log level for output
        self.initialize_log_level()

        # Deduce subscription topics for the example program
        self.ALL_MESSAGES_TOPIC = ""
        self.HB_MESSAGES_TOPIC  = ""
        level = self.config.get_integer_value("gmsec-schema-level", lp.Specification.SchemaLevel_LEVEL_0)

        if level == lp.Specification.SchemaLevel_LEVEL_0:
            self.ALL_MESSAGES_TOPIC = "C2MS.>"
            self.HB_MESSAGES_TOPIC  = "C2MS.*.*.*.*.*.MSG.HB.+"

        elif level == lp.Specification.SchemaLevel_LEVEL_1:
            self.ALL_MESSAGES_TOPIC = "C2MS-PIPE.>"
            self.HB_MESSAGES_TOPIC  = "C2MS-PIPE.*.*.*.*.*.MSG.HB.+"

        else:
            self.ALL_MESSAGES_TOPIC = "GMSEC.>"
            self.HB_MESSAGES_TOPIC  = "GMSEC.*.*.MSG.HB.+"


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

            # Subscribe to receive all messages using the GMSEC specification
            lp.log_info("Subscribe to: " + self.ALL_MESSAGES_TOPIC)
            conn.subscribe(self.ALL_MESSAGES_TOPIC)

            # Add specific message topic to the Connection's exclusion filter
            lp.log_info("Filtering out: " + self.HB_MESSAGES_TOPIC)
            conn.exclude_subject(self.HB_MESSAGES_TOPIC)

            # Receive first message (LOG) sent by publisher
            lp.log_info("Waiting to receive a message...")
            rcvdMsg = conn.receive()

            lp.log_info("Received LOG message:\n" + rcvdMsg.to_xml())
            rcvdMsg.acknowledge()
            lp.Message.destroy(rcvdMsg)

            # Although a second message (HB) is sent by the publisher, it will not
            # be delivered to this Connection object
            lp.log_info("Waiting to receive other message(s)...")
            rcvdMsg = conn.receive(5000)

            if rcvdMsg == None:
                lp.log_info("As expected, a timeout occurred (i.e. no HB message received)")
            else:
                lp.log_warning("Received unexpected message:\n" + rcvdMsg.to_xml())
                rcvdMsg.acknowledge()
                lp.Message.destroy(rcvdMsg)

            # Disconnect from the GMSEC Bus.
            conn.disconnect()

            # Destroy the Connection
            del conn

        except lp.GmsecError as e:
            # If here, the most likely case is the middleware could not be
            # deduced, or a failure occurred when attempting to connect to
            # the GMSEC Bus.
            lp.log_error("Exception: " + str(e))


# Main entry point
ex = subscribe_subject_filter()
ex.run()
