#!/usr/bin/env python3

# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file request.py

 @brief This example program provides a basic demonstration of issuing
 a synchronous Request Directive message to the GMSEC Bus, and then
 handling the receipt of a Response Directive message.
"""

import libgmsec_python3 as lp
import sys


class request:
    def __init__(self):
        # Set up connection configuration options using values from the command line.
        self.config = lp.Config(sys.argv)

        # Initialize log level for output
        self.initialize_log_level()

        # Validate messages before being sent to and as they are received from the bus
        self.config.add_value("gmsec-msg-content-validate-all", "true")


    def initialize_log_level(self):
        level = lp.Log.from_string( self.config.get_value("loglevel", "info") )
        lp.Log.set_reporting_level(level)


    def run(self):
        try:
            # Create connection instance.
            conn = lp.Connection(self.config)

            # Set up standard fields within the MessageFactory associated
            # with the connection object.
            self.set_standard_fields(conn.get_message_factory(), "REQUEST")

            # Establish connection to the GMSEC Bus.
            conn.connect()

            # Output information regarding the API version and connection
            lp.log_info(lp.Connection.get_api_version())
            lp.log_info("Middleware version = " + conn.get_library_version())

            # Create a Request Directive message using the MessageFactory.
            # Set both required and optional fields within the message.
            reqMsg = conn.get_message_factory().create_message("REQ.DIR")
            reqMsg.add_field( lp.StringField("DESTINATION-COMPONENT", "REPLY", True) )            # to whom is the request intended for
            reqMsg.add_field( lp.StringField("DIRECTIVE-STRING", "Turn on the lights") )    # operation to perform
            reqMsg.add_field( lp.BooleanField("RESPONSE", True) )                           # a response message is requested
            reqMsg.add_field( lp.U16Field("REQUEST-ID", 0) )

            # Output in XML what we intend to send
            lp.log_info("Sending Request Message:\n" + reqMsg.to_xml())

            # Timeout and republish periods have been arbitrarily chosen to be 5 seconds.
            # The republish period could also be set to GMSEC_REQUEST_REPUBLISH_NEVER to
            # prevent re-sending request message should a timeout occur.
            timeout   = 5000
            republish = 5000

            # Issue Request Directive.
            # Note, this is a blocking call (i.e. a synchronous operation).
            rspMsg = conn.request(reqMsg, timeout, republish)

            # We have returned from the previous blocking call check if we have a message.
            if rspMsg != None:
                lp.log_info("Received Response Directive message:\n" + rspMsg.to_xml())
                rspMsg.acknowledge()
                lp.Message.destroy(rspMsg)
            else:
                # This else-block is reached if a response message is not received.
                # If the republish period is set to a time period other than
                # GMSEC_REQUEST_REPUBLISH_NEVER, then this block will never be reached.
                lp.log_warning("Time out no Response Directive message received")

            # Disconnect from the GMSEC Bus, and terminate subscriptions.
            conn.disconnect()

            # Destroy the Connection
            del conn

        except lp.GmsecError as e:
            # If here, the most likely case is the middleware could not be
            # deduced, or a failure occurred when attempting to connect to
            # the GMSEC Bus.
            lp.log_error("GmsecException: " + str(e))


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
ex = request()
ex.run()
