#!/usr/bin/env python3

# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file reply_multi.py

 @brief This example program provides a basic demonstration of synchronously
 receiving a Request Directive message from the GMSEC Bus, and then issuing
 multiple Response Directive messages (should one be requested).
"""

import libgmsec_python3 as lp
import sys


class reply_multi:
    def __init__(self):
        # Set up connection configuration options using values from the command line.
        self.config = lp.Config(sys.argv)

        # Initialize log level for output
        self.initialize_log_level()

        # Validate messages before being sent to and as they are received from the bus
        self.config.add_value("gmsec-msg-content-validate-all", "true")

        # Define subscription topic for receiving request message(s).
        self.subscription_topic = ""
        level = self.config.get_integer_value("gmsec-schema-level", lp.Specification.SchemaLevel_LEVEL_0)

        if level == lp.Specification.SchemaLevel_LEVEL_0:
            self.subscription_topic = "C2MS.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY-MULTI"

        elif level == lp.Specification.SchemaLevel_LEVEL_1:
            self.subscription_topic = "C2MS-PIPE.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY-MULTI"

        else:
            self.subscription_topic = "GMSEC.MY-MISSION.MY-SAT-ID.REQ.DIR.REPLY-MULTI"


    def initialize_log_level(self):
        level = lp.Log.from_string( self.config.get_value("loglevel", "info") )
        lp.Log.set_reporting_level(level)


    def run(self):
        try:
            # Create connection instance
            conn = lp.Connection(self.config)

            # Set up standard fields within the MessageFactory associated
            # with the connection object.
            self.set_standard_fields(conn.get_message_factory(), "REPLY-MULTI")

            # Establish connection to the GMSEC Bus.
            conn.connect()

            # Set up our subscription for receiving a request message.
            conn.subscribe(self.subscription_topic)

            # Output information regarding the API version and connection
            lp.log_info(lp.Connection.get_api_version())
            lp.log_info("Middleware version = " + conn.get_library_version())
            lp.log_info("Subscribing to " + self.subscription_topic)

            # Timeout was arbitrarily chosen as lp.GMSEC_WAIT_FOREVER so that
            # we block indefinitely waiting to receive a request message.
            # It can be set to other time values (e.g. 1000 is 1 second) so
            # the application does not block indefinitely waiting for a
            # request message.
            timeout = lp.GMSEC_WAIT_FOREVER

            # Block for a Request Directive message to arrive.
            reqMsg = conn.receive(timeout)

            # Check if we have received a message.
            if reqMsg != None:
                # Output in XML the received message.
                lp.log_info("Received Request Message:\n" + reqMsg.to_xml())

                # Check if a response message should be returned.
                if reqMsg.has_field("RESPONSE") and reqMsg.get_boolean_value("RESPONSE"):
                    # Create a Response Directive message using the MessageFactory.
                    rspMsg = conn.get_message_factory().create_message("RESP.DIR")

                    # Insert required and optional fields into the Response Directive
                    # using values derived from the incoming Request Directive.
                    if reqMsg.has_field("COMPONENT"):
                        rspMsg.add_field( lp.StringField("DESTINATION-COMPONENT", reqMsg.get_string_value("COMPONENT"), True) )

                    if reqMsg.has_field("REQUEST-ID"):
                        rspMsg.add_field( reqMsg.get_field("REQUEST-ID") )

                    # Send multiple responses
                    for i in range(0,4):
                        if i == 0:
                            responseStatus = lp.Message.ResponseStatus_ACKNOWLEDGEMENT
                        elif i == 3:
                            responseStatus = lp.Message.ResponseStatus_SUCCESSFUL_COMPLETION
                        else:
                            responseStatus = lp.Message.ResponseStatus_WORKING_KEEP_ALIVE

                        rspMsg.set_field_value("RESPONSE-STATUS", responseStatus)

                        # Send the Response Directive message.
                        conn.reply(reqMsg, rspMsg)

                        # Output in XML the message that has been sent.
                        lp.log_info("Sent Response Message:\n" + rspMsg.to_xml())

                        # Arbitrary delay to mimic processing of task
                        lp.TimeUtil.millisleep(1000)

                # Acknowledge processing the request message
                reqMsg.acknowledge()

                # Release received request message
                lp.Message.destroy(reqMsg)

            else:
                # This else-block is reached if a reuqest message is not received.
                # When the timeout period is set to Gmsec.WAIT_FOREVER, then this
                # block would never be reached.
                lp.log_warning("Time out no Request Directive message received")

            # Disconnect from the GMSEC Bus, and terminate subscriptions.
            conn.disconnect()

        except lp.GmsecError as e:
            # If here, the most likely case is the middleware could not be
            # deduced, or a failure occurred when attempting to connect toi
            # the GMSEC Bus.
            lp.log_error("Exception: " + str(e))


    # Convenience method for associating standard (i.e. common) fields
    # with the MessageFactory which will be included automatically within
    # all messages created using the factory.
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
ex = reply_multi()
ex.run()
