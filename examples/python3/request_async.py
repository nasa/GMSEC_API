#!/usr/bin/env python3

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file request_async.py

 @brief This example program provides a basic demonstration of issuing
 an asynchronous Request Directive message to the GMSEC Bus, and then
 handling the receipt of a Response Directive message(s) via a
 ReplyCallback.
"""

import libgmsec_python3 as lp
import sys


class MyReplyCallback(lp.ReplyCallback):
    def __init__(self):
        super().__init__()
        self.responseComplete = False

    def on_reply(self, conn, reqMsg, repMsg):
        lp.log_info("MyReplyCallback on_reply:\n" + repMsg.to_xml())

        if repMsg.has_field("RESPONSE-STATUS"):
            try:
                status = repMsg.get_integer_value("RESPONSE-STATUS")

                if (status == lp.Message.ResponseStatus_ACKNOWLEDGEMENT or
                    status == lp.Message.ResponseStatus_WORKING_KEEP_ALIVE):
                    self.responseComplete = False

                if (status == lp.Message.ResponseStatus_SUCCESSFUL_COMPLETION or
                    status == lp.Message.ResponseStatus_FAILED_COMPLETION or
                    status == lp.Message.ResponseStatus_INVALID_REQUEST or
                    status == lp.Message.ResponseStatus_FINAL_MESSAGE):
                    self.responseComplete = True

            except lp.GmsecError as e:
                lp.log_error("Unexpected error: " + str(e))
        else:
            self.responseComplete = True

        # Acknowledge processing the response message
        repMsg.acknowledge()


    def on_event(self, conn, status, event):
        if status.has_error():
            lp.log_error("MyReplyCallback on_event: " + status.get() + ", event: " + str(event))
        else:
            lp.log_info("MyReplyCallback on_event: " + status.get() + ", event: " + str(event))


    def is_response_complete(self):
        return self.responseComplete


class request_async:
    def __init__(self):
        # Set up connection configuration options using values from the command line.
        self.config = lp.Config(sys.argv)

        # Initialize log level for output
        self.initialize_log_level()

        # Validate messages before being sent to and as they are received from the bus
        self.config.add_value("gmsec-msg-content-validate-all", "true")

        # IMPORTANT! This config option must be set to receive multiple response messages
        self.config.add_value("mw-multi-resp", "true")

        # Define instance of MyReplyCallback
        self.rcb = MyReplyCallback()


    def initialize_log_level(self):
        level = lp.Log.from_string( self.config.get_value("loglevel", "info") )
        lp.Log.set_reporting_level(level)


    def run(self):
        try:
            # Create connection instance.
            conn = lp.Connection(self.config)

            # Set up standard fields within the MessageFactory associated
            # with the connection object.
            self.set_standard_fields(conn.get_message_factory(), "REQUEST-ASYNC")

            # Establish connection to the GMSEC Bus.
            conn.connect()

            # Output information regarding the API version and connection
            lp.log_info(lp.Connection.get_api_version())
            lp.log_info("Middleware version = " + conn.get_library_version())

            # Create a Request Directive message using the MessageFactory.
            # Set both required and optional fields within the message.
            reqMsg = conn.get_message_factory().create_message("REQ.DIR")
            reqMsg.add_field( lp.StringField("DESTINATION-COMPONENT", "REPLY-MULTI", True) )      # to whom is the request intended for
            reqMsg.add_field( lp.StringField("DIRECTIVE-STRING", "Turn on the lights") )    # operation to perform
            reqMsg.add_field( lp.BooleanField("RESPONSE", True) )                          # a response message is requested
            reqMsg.add_field( lp.U16Field("REQUEST-ID", 0) )

            # Timeout and republish periods have been arbitrarily chosen to be 5 seconds.
            # The republish period could also be set to GMSEC_REQUEST_REPUBLISH_NEVER to
            # prevent re-sending request message should a timeout occur.
            timeout   = 5000
            republish = 5000

            # Issue Request Directive.
            # Note, this is a non-blocking call (i.e. an asynchronous operation).
            conn.request(reqMsg, timeout, self.rcb, republish)

            # Output in XML what we have sent
            lp.log_info("Sending Request Directive Message:\n" + reqMsg.to_xml())
            lp.log_info("Use reply_multi example program to issue response messages")

            # Normally an application would continue with other operation, however for
            # demonstration purposes, we will loop until it has been verified that a
            # response message has been received.
            while self.rcb.is_response_complete() == False:
                lp.log_info("Waiting to receive response message...")

                lp.TimeUtil.millisleep(1000)

            # Disconnect from the GMSEC Bus, and terminate subscriptions.
            conn.disconnect()

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
ex = request_async()
ex.run()
